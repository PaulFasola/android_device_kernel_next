/*
 * hi3xxx_asp_dmac.c -- ALSA SoC HI3630 ASP DMA driver
 *
 * Copyright (c) 2013 Hisilicon Technologies CO., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define LOG_TAG "hi3xxx_asp_dmac"

#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/compiler.h>
#include <linux/regulator/consumer.h>
#include <linux/hwspinlock.h>
#include <linux/pm_runtime.h>
#include <sound/core.h>
#include <sound/dmaengine_pcm.h>
#include <sound/pcm.h>
#include <sound/initval.h>
#include <sound/pcm_params.h>
#include <sound/tlv.h>
#include <sound/soc.h>

#include "hi3630_log.h"
#include "hi3630_asp_common.h"
#include "hi3xxx_asp_dma.h"

#define HWLOCK_ID		2  /* hardware lock ID */
#define HWLOCK_WAIT_TIME	50 /* 50ms */

#define HI6210_MAX_BUFFER_SIZE  ( 128 * 1024 )    /* 0x20000 */

#undef CONFIG_PM_RUNTIME

struct hi3xxx_asp_dmac_data {
	struct hwspinlock *hwlock;
	struct resource	*res;
	void __iomem	*reg_base_addr;
	int		irq;
	spinlock_t 	lock;
	struct regulator *asp_ip;
	struct device *dev;
};

enum hi3xxx_asp_dmac_status {
	STATUS_DMAC_STOP = 0,
	STATUS_DMAC_RUNNING,
};

struct dma_lli_cfg
{
	unsigned int lli;
	unsigned int reserved[3];
	unsigned int a_count;
	unsigned int src_addr;
	unsigned int des_addr;
	unsigned int config;
} __aligned(32);

struct hi3xxx_asp_dmac_runtime_data {
	spinlock_t lock;
	struct hi3xxx_asp_dmac_data *pdata;
	struct mutex mutex;
	enum hi3xxx_asp_dmac_status status;
	bool two_dma_flag;
	unsigned int dma_addr;
	unsigned int period_size;
	unsigned int period_cur;
	unsigned int period_next;

	struct dma_lli_cfg *pdma_lli_cfg;
	unsigned int lli_dma_addr;
};

static const struct of_device_id hi3xxx_asp_dmac_of_match[] = {
	{
		.compatible = "hisilicon,hi3630-pcm-asp-dma",
	},
	{ },
};
MODULE_DEVICE_TABLE(of, hi3xxx_asp_dmac_of_match);

static inline int hi3xxx_dmac_reg_read(struct hi3xxx_asp_dmac_data *pdata, unsigned int reg)
{
	unsigned long flag = 0;
	int ret = 0;

	BUG_ON(NULL == pdata);

	if (hwspin_lock_timeout_irqsave(pdata->hwlock, HWLOCK_WAIT_TIME, &flag)) {
		loge("%s: hwspinlock timeout!\n", __func__);
		return ret;
	}

	ret = readl_relaxed(pdata->reg_base_addr + reg);

	hwspin_unlock_irqrestore(pdata->hwlock, &flag);
	return ret;
}

static inline void hi3xxx_dmac_reg_write(struct hi3xxx_asp_dmac_data *pdata,
					 unsigned int reg, unsigned int value)
{
	unsigned long flag = 0;

	BUG_ON(NULL == pdata);

	if (hwspin_lock_timeout_irqsave(pdata->hwlock, HWLOCK_WAIT_TIME, &flag)) {
		loge("%s: hwspinlock timeout!\n", __func__);
		return;
	}

	writel_relaxed(value, pdata->reg_base_addr + reg);

	hwspin_unlock_irqrestore(pdata->hwlock, &flag);
}

static inline void hi3xxx_dmac_set_bit(struct hi3xxx_asp_dmac_data *pdata,
				       unsigned int reg, unsigned int offset)
{
	unsigned int value = 0;
	unsigned long flag_hw = 0;
	unsigned long flag_sft = 0;

	BUG_ON(NULL == pdata);

	if (hwspin_lock_timeout_irqsave(pdata->hwlock, HWLOCK_WAIT_TIME, &flag_hw)) {
		loge("%s: hwspinlock timeout!\n", __func__);
		return;
	}

	spin_lock_irqsave(&pdata->lock, flag_sft);
	value = readl_relaxed(pdata->reg_base_addr + reg);
	value |= (1 << offset);
	writel_relaxed(value, pdata->reg_base_addr + reg);
	spin_unlock_irqrestore(&pdata->lock, flag_sft);

	hwspin_unlock_irqrestore(pdata->hwlock, &flag_hw);
}

static inline void hi3xxx_dmac_clr_bit(struct hi3xxx_asp_dmac_data *pdata,
				       unsigned int reg, unsigned int offset)
{
	unsigned int value = 0;
	unsigned long flag_hw = 0;
	unsigned long flag_sft = 0;

	BUG_ON(NULL == pdata);

	if (hwspin_lock_timeout_irqsave(pdata->hwlock, HWLOCK_WAIT_TIME, &flag_hw)) {
		loge("%s: hwspinlock timeout!\n", __func__);
		return;
	}

	spin_lock_irqsave(&pdata->lock, flag_sft);
	value = readl_relaxed(pdata->reg_base_addr + reg);
	value &= ~(1 << offset);
	writel_relaxed(value, pdata->reg_base_addr + reg);
	spin_unlock_irqrestore(&pdata->lock, flag_sft);

	hwspin_unlock_irqrestore(pdata->hwlock, &flag_hw);
}

static const struct snd_pcm_hardware hi3xxx_asp_dmac_hardware = {
	.info			= SNDRV_PCM_INFO_MMAP |
				  SNDRV_PCM_INFO_MMAP_VALID |
				  SNDRV_PCM_INFO_INTERLEAVED,
	.formats		= SNDRV_PCM_FMTBIT_S16_LE |
				  SNDRV_PCM_FMTBIT_S16_BE |
				  SNDRV_PCM_FMTBIT_S24_LE |
				  SNDRV_PCM_FMTBIT_S24_BE,
	.period_bytes_min	= 32,
	.period_bytes_max	= 16 * 1024,
	.periods_min		= 2,
	.periods_max		= 32,
	.buffer_bytes_max	= 128 * 1024,
};

/*
*dma operations
*
**/
static void asp_dma_init(struct hi3xxx_asp_dmac_data *pdata)
{
	/*clear interrupt*/
	hi3xxx_dmac_reg_write(pdata, HI3xxx_DMAC_INT_TC1_RAW_REG,  CH_0_INT_CLR);
	hi3xxx_dmac_reg_write(pdata, HI3xxx_DMAC_INT_TC2_RAW_REG,  CH_0_INT_CLR);
	hi3xxx_dmac_reg_write(pdata, HI3xxx_DMAC_INT_ERR1_RAW_REG, CH_0_INT_CLR);
	hi3xxx_dmac_reg_write(pdata, HI3xxx_DMAC_INT_ERR2_RAW_REG, CH_0_INT_CLR);
	hi3xxx_dmac_reg_write(pdata, HI3xxx_DMAC_INT_ERR3_RAW_REG, CH_0_INT_CLR);

	/*dma ch unmask*/
	hi3xxx_dmac_set_bit(pdata, HI3xxx_DMAC_INT_TC1_MASK_0_REG , CH_0_UNMASK_BIT);
	hi3xxx_dmac_set_bit(pdata, HI3xxx_DMAC_INT_TC2_MASK_0_REG , CH_0_UNMASK_BIT);
	hi3xxx_dmac_set_bit(pdata, HI3xxx_DMAC_INT_ERR1_MASK_0_REG , CH_0_UNMASK_BIT);
	hi3xxx_dmac_set_bit(pdata, HI3xxx_DMAC_INT_ERR2_MASK_0_REG , CH_0_UNMASK_BIT);
	hi3xxx_dmac_set_bit(pdata, HI3xxx_DMAC_INT_ERR3_MASK_0_REG , CH_0_UNMASK_BIT);
}

/*dma config lli node*/
static void asp_dma_cfg(int direction, struct hi3xxx_asp_dmac_runtime_data *prtd)
{
	unsigned int i				 = 0x0;
	unsigned int next_addr 		 = 0x0;
	unsigned int config			 = 0x0;
	unsigned int dma_lli_num 	 = DMA_LLI_NUM;
	unsigned int sio_tx_dma_addr = 0X0;
	unsigned int sio_rx_dma_addr = 0X0;
			
	if(SNDRV_PCM_STREAM_PLAYBACK == direction){
		config = DMA_CONFIG_TX;
		sio_tx_dma_addr = AUSTIN_BASE_ADDR_SIO_AUDIO + HI3630_SIO_I2S_TX_CHN_REG;
		logi("asp_dma_cfg SNDRV_PCM_STREAM_PLAYBACK  \n");
	} else {
		config = DMA_CONFIG_RX;
		sio_rx_dma_addr = AUSTIN_BASE_ADDR_SIO_AUDIO + HI3630_SIO_I2S_RX_CHN_REG;
		logi("asp_dma_cfg = SNDRV_PCM_STREAM_CAPTURE \n");
	}
	
	//memset(prtd->pdma_lli_cfg, 0, dma_lli_num * sizeof(struct dma_lli_cfg));
	for (i = 0; i < dma_lli_num; i++) {
		next_addr = (unsigned int)(prtd->lli_dma_addr + sizeof(struct dma_lli_cfg) * (i + 1));
		prtd->pdma_lli_cfg[i].lli 		= next_addr | DMA_LLI_ENABLE;
		prtd->pdma_lli_cfg[i].config 	= config;
		prtd->pdma_lli_cfg[i].des_addr 	= sio_tx_dma_addr;
		prtd->pdma_lli_cfg[i].src_addr 	= sio_rx_dma_addr;
		prtd->pdma_lli_cfg[i].a_count 	= prtd->period_size;
	}
	
	logi("size = %d \n",prtd->period_size);

	prtd->pdma_lli_cfg[dma_lli_num - 1].lli = prtd->lli_dma_addr | DMA_LLI_ENABLE;
}

static void check_and_stop_rxdma(struct hi3xxx_asp_dmac_runtime_data *prtd)
{
	struct hi3xxx_asp_dmac_data *pdata = prtd->pdata;
	unsigned int lli_index = prtd->period_next % DMA_LLI_NUM;
	unsigned int dma_config = 0;
	unsigned int dma_int    = 0;
	unsigned int i = 45;/* 1ms * 45; one buffer is about 20ms */

	do {
		dma_config = hi3xxx_dmac_reg_read(pdata, HI3xxx_DMAC_CX_CONFIG_0_REG) & CH_0_UNMASK;
		dma_int   = hi3xxx_dmac_reg_read(pdata, HI3xxx_DMAC_INT_STAT_0_REG) & CH_0_UNMASK;

		if (dma_config | dma_int) {
			logi("Stopping dma \n");
			prtd->pdma_lli_cfg[lli_index].lli= 0x0;
			usleep_range(HI3xxx_MIN_STOP_DMA_TIME_US, HI3xxx_MAX_STOP_DMA_TIME_US);
		} else {
			break;
		}
	} while(--i);

	hi3xxx_dmac_clr_bit(pdata, HI3xxx_DMAC_CX_CONFIG_0_REG, DMA_ENABLE_BIT);

	if (!i){
		loge("exit with dma_config=0x%#x, dma_int=0x%#x\n", dma_config, dma_int);
		} else {
		logi("Dma stop normal \n");
	}
}

/*dma write first node to registers*/
static void asp_dma_set_first_node(struct hi3xxx_asp_dmac_runtime_data *prtd)
{
	struct hi3xxx_asp_dmac_data *pdata;
	struct dma_lli_cfg *lli_cfg = &prtd->pdma_lli_cfg[0];

	pdata = prtd->pdata;

	hi3xxx_dmac_reg_write(pdata, HI3xxx_DMAC_CX_LLI_0_REG, 		lli_cfg->lli);
	hi3xxx_dmac_reg_write(pdata, HI3xxx_DMAC_CX_CNT0_0_REG, 	lli_cfg->a_count);
	hi3xxx_dmac_reg_write(pdata, HI3xxx_DMAC_CX_SRC_ADDR_0_REG, lli_cfg->src_addr);
	hi3xxx_dmac_reg_write(pdata, HI3xxx_DMAC_CX_DES_ADDR_0_REG, lli_cfg->des_addr);
	hi3xxx_dmac_reg_write(pdata, HI3xxx_DMAC_CX_CONFIG_0_REG, 	lli_cfg->config);
	
	logi("asp_dma_set_first_node  lli = 0x%x, count = %d, src_addr = 0x%x, des_addr = 0x%x, config = 0x%x\n",
									lli_cfg->lli, 
									lli_cfg->a_count,
									lli_cfg->src_addr,
									lli_cfg->des_addr,
									lli_cfg->config
									);
}

static void asp_dma_set_lli_node(int direction, struct hi3xxx_asp_dmac_runtime_data *prtd,
				 unsigned int dma_lli_num, unsigned int dma_addr)
{
	unsigned int lli_index = prtd->period_next % dma_lli_num;
	if (SNDRV_PCM_STREAM_PLAYBACK == direction) {
		prtd->pdma_lli_cfg[lli_index].src_addr = dma_addr + prtd->period_next * prtd->period_size;
	} else {
		prtd->pdma_lli_cfg[lli_index].des_addr = dma_addr + prtd->period_next * prtd->period_size;
	}
		
	prtd->pdma_lli_cfg[lli_index].a_count = prtd->period_size;
}

static bool error_interrupt_handle(struct hi3xxx_asp_dmac_runtime_data *ptr)
{
	unsigned int irs_tmp = 0;
	unsigned int count = 0;
	struct dma_lli_cfg *lli_cfg0 = NULL;
	struct dma_lli_cfg *lli_cfg1 = NULL;
	struct hi3xxx_asp_dmac_data *pdata = NULL;
	
	pdata = ptr->pdata;
	
	irs_tmp = hi3xxx_dmac_reg_read(pdata, HI3xxx_DMAC_INT_ERR1_0_REG) & CH_0_UNMASK;
	if (unlikely(0 != irs_tmp)) {
		loge("dmac ch0 interrupt config error happend!");
		hi3xxx_dmac_reg_write(pdata, HI3xxx_DMAC_INT_ERR1_RAW_REG, CH_0_INT_CLR);
		count++;
	}

	/*dma transit error interrupt*/
	irs_tmp = hi3xxx_dmac_reg_read(pdata, HI3xxx_DMAC_INT_ERR2_0_REG) & CH_0_UNMASK;
	if (unlikely(0 != irs_tmp)) {
		loge("dmac ch0 interrupt transit error happend!");
		hi3xxx_dmac_reg_write(pdata, HI3xxx_DMAC_INT_ERR2_RAW_REG, CH_0_INT_CLR);
		count++;
	}

	/*dma lli transit, read lli error interrupt*/
	irs_tmp = hi3xxx_dmac_reg_read(pdata, HI3xxx_DMAC_INT_ERR3_0_REG) & CH_0_UNMASK;
	if (unlikely(0 != irs_tmp)) {
		loge("dmac ch0 interrupt read lli error happend!");
		hi3xxx_dmac_reg_write(pdata, HI3xxx_DMAC_INT_ERR3_RAW_REG, CH_0_INT_CLR);
		count++;
	}

	if (unlikely(0 < count))
		return true;

	irs_tmp = hi3xxx_dmac_reg_read(pdata, HI3xxx_DMAC_INT_TC1_0_REG) & CH_0_UNMASK;
	if (unlikely(0 != irs_tmp)) {
		logi("dmac ch0 transit finished!");

		lli_cfg0 = &ptr->pdma_lli_cfg[0];
		lli_cfg1 = &ptr->pdma_lli_cfg[1];
	
		logi("node  lli = 0x%x, count = %d, src_addr = 0x%x, des_addr = 0x%x, config = 0x%x\n",
									lli_cfg0->lli, 
									lli_cfg0->a_count,
									lli_cfg0->src_addr,
									lli_cfg0->des_addr,
									lli_cfg0->config
									);

		logi("node  lli = 0x%x, count = %d, src_addr = 0x%x, des_addr = 0x%x, config = 0x%x\n",
									lli_cfg1->lli, 
									lli_cfg1->a_count,
									lli_cfg1->src_addr,
									lli_cfg1->des_addr,
									lli_cfg1->config
									);
		
		hi3xxx_dmac_reg_write(pdata, HI3xxx_DMAC_INT_TC1_RAW_REG, CH_0_INT_CLR);
		return true;
	}
	
	/*dma lli transit finish interrupt*/
	irs_tmp = hi3xxx_dmac_reg_read(pdata, HI3xxx_DMAC_INT_TC2_0_REG) & CH_0_UNMASK;
	if (likely(0 != irs_tmp)) {
		hi3xxx_dmac_reg_write(pdata, HI3xxx_DMAC_INT_TC2_RAW_REG, CH_0_INT_CLR);
		return false;
	}

	return false;
}

static int hi3xxx_asp_dmac_hw_params(struct snd_pcm_substream *substream,
				     struct snd_pcm_hw_params *params)
{
	struct hi3xxx_asp_dmac_runtime_data *prtd = substream->runtime->private_data;
	unsigned long bytes = params_buffer_bytes(params);
	unsigned int lli_dma_addr = 0;
	int ret = 0;

	logi("in\n");
	
	ret = snd_pcm_lib_malloc_pages(substream, bytes);
	if (ret < 0) {
		loge("snd_pcm_lib_malloc_pages ret is %d", ret);
		return ret;
	}
	
	mutex_lock(&prtd->mutex);

	prtd->pdma_lli_cfg = (struct dma_lli_cfg *)dma_alloc_coherent(prtd->pdata->dev,
		DMA_LLI_NUM * sizeof(struct dma_lli_cfg), (dma_addr_t *)&lli_dma_addr, GFP_KERNEL);

	if (NULL == prtd->pdma_lli_cfg) {
		loge("prtd->pdma_lli_cfg dma alloc coherent error!");
		ret = -ENOMEM;
		goto err_out;
	}

	prtd->lli_dma_addr = lli_dma_addr;
	prtd->period_size = params_period_bytes(params);
	prtd->period_next = 0;

err_out:
	
	if (0 > ret) {
		loge("hw params error, ret : %d\n", ret);
		snd_pcm_lib_free_pages(substream);
	}

	mutex_unlock(&prtd->mutex);	

	return ret;
}

static int hi3xxx_asp_dmac_hw_free(struct snd_pcm_substream *substream)
{
	struct hi3xxx_asp_dmac_runtime_data *prtd = substream->runtime->private_data;
	int ret = 0;

	logi("in \n");
	check_and_stop_rxdma(prtd);
	
	if(NULL != prtd->pdma_lli_cfg){
		dma_free_coherent(prtd->pdata->dev, DMA_LLI_NUM * sizeof(struct dma_lli_cfg),
				(void*)prtd->pdma_lli_cfg, prtd->lli_dma_addr);
		prtd->pdma_lli_cfg = NULL;
	}
		
	ret = snd_pcm_lib_free_pages(substream);

	return ret;
}

static int hi3xxx_asp_dmac_prepare(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct hi3xxx_asp_dmac_runtime_data *prtd = substream->runtime->private_data;
	unsigned int num_periods = substream->runtime->periods;

	logi(" in --------- 1\n");
	mutex_lock(&prtd->mutex);

	prtd->status = STATUS_DMAC_STOP;
	prtd->two_dma_flag = true;
	prtd->period_next = 0;
	prtd->period_cur = 0;
	prtd->dma_addr = runtime->dma_addr;

	logi(" in --------- 2\n");
	check_and_stop_rxdma(prtd);

	logi("direction = %d.\n",substream->stream);
	
	asp_dma_cfg(substream->stream,prtd);
	asp_dma_init(prtd->pdata);

	logi(" in --------- 3\n");
	//init the first node of dma list 
	asp_dma_set_lli_node(substream->stream, prtd, DMA_LLI_NUM, runtime->dma_addr);
	prtd->period_next = (prtd->period_next + 1) % num_periods;

	//init the second node of dma list 
	asp_dma_set_lli_node(substream->stream, prtd, DMA_LLI_NUM, runtime->dma_addr);
	prtd->period_next = (prtd->period_next + 1) % num_periods;

	logi(" in --------- 4\n");
	asp_dma_set_first_node(prtd);

	logi(" in --------- 5\n");
	mutex_unlock(&prtd->mutex);

	return 0;
}

static int hi3xxx_asp_dmac_trigger(struct snd_pcm_substream *substream, int cmd)
{
	int ret = 0;
	struct hi3xxx_asp_dmac_runtime_data *prtd = substream->runtime->private_data;

    logi("in cmd = %d \n",cmd);
	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		prtd->status = STATUS_DMAC_RUNNING;
		hi3xxx_dmac_set_bit(prtd->pdata, HI3xxx_DMAC_CX_CONFIG_0_REG, DMA_ENABLE_BIT);
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		prtd->status = STATUS_DMAC_STOP;
		hi3xxx_dmac_clr_bit(prtd->pdata, HI3xxx_DMAC_CX_CONFIG_0_REG, DMA_ENABLE_BIT);
		break;
	default:
		loge("error!");
		ret = -EINVAL;
		break;
	}

	return ret;
}

static snd_pcm_uframes_t hi3xxx_asp_dmac_pointer(struct snd_pcm_substream *substream)
{
	snd_pcm_uframes_t offset = 0;
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct hi3xxx_asp_dmac_runtime_data *prtd = substream->runtime->private_data;
	unsigned int period_cur = 0;
	unsigned int period_size = 0;

    //loge("%d in \n", __LINE__);
	spin_lock(&prtd->lock);
	period_cur = prtd->period_cur;
	period_size = prtd->period_size;
	spin_unlock(&prtd->lock);

	offset = bytes_to_frames(runtime, period_cur * period_size);

	if(offset >= runtime->buffer_size)
		offset = 0;

	return offset;
}

static int hi3xxx_asp_dmac_mmap(struct snd_pcm_substream *substream,
				struct vm_area_struct *vma)
{
	int ret = 0;
	struct snd_pcm_runtime *runtime = NULL;
	
    logi("in \n");
	runtime = substream->runtime;
	if (NULL != runtime){
		ret = dma_mmap_writecombine(substream->pcm->card->dev,
				vma,
				runtime->dma_area,
				runtime->dma_addr,
				runtime->dma_bytes);
         loge("%d dma_mmap_writecombine \n", __LINE__);
    }

	return ret;
}

static irqreturn_t hi3xxx_intr_dmac_handle(int irq, void *dev_id)
{
	struct snd_pcm_substream *substream = dev_id;
	struct snd_pcm_runtime *runtime = NULL;
	struct hi3xxx_asp_dmac_runtime_data *prtd = NULL;
	snd_pcm_uframes_t avail = 0;
	unsigned int irs_stat = 0;
	unsigned int rt_period_size = 0;
	unsigned int num_period = 0;
	struct hi3xxx_asp_dmac_data *pdata = NULL;

	if (NULL == substream) {
		loge("substream is NULL\n");
		return IRQ_HANDLED;
	}
		
	runtime = substream->runtime;
	BUG_ON(NULL == runtime);
	prtd = runtime->private_data;
	BUG_ON(NULL == prtd);
	pdata = prtd->pdata;
	BUG_ON(NULL == pdata);

	/*dma transit state*/
	irs_stat = hi3xxx_dmac_reg_read(pdata, HI3xxx_DMAC_INT_STAT_0_REG) & CH_0_UNMASK;
	if (0 == irs_stat) {
		/* when hifi dsp is incall, it will go to here */
		return IRQ_NONE;
	}

	if (error_interrupt_handle(prtd)) {
		return IRQ_HANDLED;
	}

	rt_period_size = runtime->period_size;
	num_period = runtime->periods;

	spin_lock(&prtd->lock);
	prtd->period_cur = (prtd->period_cur + 1) % num_period;
	spin_unlock(&prtd->lock);

	snd_pcm_period_elapsed(substream);

	spin_lock(&prtd->lock);

	/* DMA IS STOPPED */
	if (STATUS_DMAC_STOP == prtd->status) {
		spin_unlock(&prtd->lock);
		return IRQ_HANDLED;
	}

	if(SNDRV_PCM_STREAM_PLAYBACK == substream->stream){
		avail = snd_pcm_playback_hw_avail(runtime);
	}
	else {
		avail = snd_pcm_capture_hw_avail (runtime);	
	}

	if(avail < rt_period_size) {
		logi("There is no avail data to play \n");
	} else {
		asp_dma_set_lli_node(substream->stream, prtd, DMA_LLI_NUM, runtime->dma_addr);
		prtd->period_next = (prtd->period_next + 1) % num_period;
	}

	spin_unlock(&prtd->lock);
	return IRQ_HANDLED;
}

static int hi3xxx_asp_dmac_open(struct snd_pcm_substream *substream)
{
	struct hi3xxx_asp_dmac_runtime_data *prtd = NULL;
	struct snd_soc_pcm_runtime *rtd = NULL;
	struct hi3xxx_asp_dmac_data *pdata = NULL;
	struct snd_pcm *pcm = NULL ;
	int ret = 0;

	logi("%d in\n", __LINE__);
	pcm = substream->pcm;
	prtd = kzalloc(sizeof(struct hi3xxx_asp_dmac_runtime_data), GFP_KERNEL);
	if (NULL == prtd) {
		loge("kzalloc hi3xxx_asp_dmac_runtime_data error!\n");
		return -ENOMEM;
	}

	mutex_init(&prtd->mutex);
	spin_lock_init(&prtd->lock);

	rtd = (struct snd_soc_pcm_runtime *)substream->private_data;
	pdata = (struct hi3xxx_asp_dmac_data *)snd_soc_platform_get_drvdata(rtd->platform);

	BUG_ON(NULL == pdata);

#ifdef CONFIG_PM_RUNTIME
	pm_runtime_get_sync(pdata->dev);
#else
	ret = regulator_enable(pdata->asp_ip);
	if (ret != 0) {
		loge("couldn't enable regulators %d\n", ret);
		goto err_bulk;
	}
#endif

	prtd->pdata = pdata;

	substream->runtime->private_data = prtd;

	ret = request_irq(pdata->irq, hi3xxx_intr_dmac_handle,
			  IRQF_SHARED, "asp_dma_irq", substream);
	if (0 > ret) {
		loge("request irq error, error No. = %d\n", ret);
		goto err_irq;
	}
	logi("request_irq = %d,irq = %d \n", ret,pdata->irq);

	ret = snd_soc_set_runtime_hwparams(substream, &hi3xxx_asp_dmac_hardware);

	return ret;

err_irq:
#ifdef CONFIG_PM_RUNTIME
	pm_runtime_mark_last_busy(pdata->dev);
	pm_runtime_put_autosuspend(pdata->dev);
#else
	ret = regulator_disable(prtd->pdata->asp_ip);
err_bulk:
#endif
	kfree(prtd);
	return ret;
}

static int hi3xxx_asp_dmac_close(struct snd_pcm_substream *substream)
{
	struct hi3xxx_asp_dmac_runtime_data *prtd = substream->runtime->private_data;

	logi("in \n");
	if (NULL == prtd) {
		loge("prtd is NULL\n");
	} else {
		BUG_ON(NULL == prtd->pdata);
		logi("free irq = %d \n", prtd->pdata->irq);
		free_irq(prtd->pdata->irq, substream);
#ifdef CONFIG_PM_RUNTIME
		pm_runtime_mark_last_busy(prtd->pdata->dev);
		pm_runtime_put_autosuspend(prtd->pdata->dev);
#else
		regulator_disable(prtd->pdata->asp_ip);
#endif

		kfree(prtd);
		substream->runtime->private_data = NULL;
	}

	return 0;
}

static struct snd_pcm_ops hi3xxx_asp_dmac_ops = {
	.open	    = hi3xxx_asp_dmac_open,
	.close	    = hi3xxx_asp_dmac_close,
	.ioctl      = snd_pcm_lib_ioctl,
	.hw_params  = hi3xxx_asp_dmac_hw_params,
	.hw_free    = hi3xxx_asp_dmac_hw_free,
	.prepare    = hi3xxx_asp_dmac_prepare,
	.trigger    = hi3xxx_asp_dmac_trigger,
	.pointer    = hi3xxx_asp_dmac_pointer,
	.mmap       = hi3xxx_asp_dmac_mmap,
};

static unsigned long long hi3xxx_pcm_dmamask = DMA_BIT_MASK(32);

static int hi3xxx_asp_dmac_new(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_card *card = rtd->card->snd_card;
	struct snd_pcm  *pcm  = rtd->pcm;
	struct hi3xxx_asp_dmac_data *pdata =
		(struct hi3xxx_asp_dmac_data *)snd_soc_platform_get_drvdata(rtd->platform);
	int ret = 0;

	BUG_ON(NULL == card);
	BUG_ON(NULL == pcm);
	BUG_ON(NULL == pdata);

	if (!card->dev->dma_mask)
		card->dev->dma_mask = &hi3xxx_pcm_dmamask;
	if (!card->dev->coherent_dma_mask)
		card->dev->coherent_dma_mask = DMA_BIT_MASK(32);
	
	// Just deal with the device 0 which is for audio.
	if ( 0 == pcm->device)
    {
	//Manual allocating dma buff.
        ret = snd_pcm_lib_preallocate_pages_for_all(pcm, SNDRV_DMA_TYPE_DEV,
                pcm->card->dev, HI6210_MAX_BUFFER_SIZE, HI6210_MAX_BUFFER_SIZE);
        if (ret) {
            loge("snd_pcm_lib_preallocate_pages_for_all error : %d\n", ret);
            return ret;
        } else{
			logi("audio :snd_pcm_lib_preallocate_pages_for_all success\n");
		}
	}

	return ret;
}

static void hi3xxx_asp_dmac_free(struct snd_pcm *pcm)
{
	BUG_ON(NULL == pcm);
	snd_pcm_lib_preallocate_free_for_all(pcm);
}

static struct snd_soc_platform_driver hi3xxx_pcm_asp_dmac_platform = {
	.ops		= &hi3xxx_asp_dmac_ops,
	.pcm_new	= hi3xxx_asp_dmac_new,
	.pcm_free	= hi3xxx_asp_dmac_free,
};

static int hi3xxx_asp_dmac_probe(struct platform_device *pdev)
{
	int ret = -1;
	struct device *dev = &pdev->dev;
	struct hi3xxx_asp_dmac_data *pdata = NULL;

	if (!dev) {
		loge("platform_device has no device\n");
		return -ENOENT;
	}

	pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata) {
		dev_err(dev, "cannot allocate hi3630 asp dma platform data\n");
		return -ENOMEM;
	}

	/* get resources */
	pdata->res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!pdata->res) {
		dev_err(dev, "platform_get_resource err\n");
		return -ENOENT;
	}

	if (!devm_request_mem_region(dev, pdata->res->start,
				     resource_size(pdata->res),
				     pdev->name)) {
		dev_err(dev, "cannot claim register memory\n");
		return -ENOMEM;
	}

	pdata->reg_base_addr = devm_ioremap(dev, pdata->res->start,
					    resource_size(pdata->res));
	if (!pdata->reg_base_addr) {
		dev_err(dev, "cannot map register memory\n");
		return -ENOMEM;
	}
	dev_err(dev, "base = %x , virt = %p \n", pdata->res->start, pdata->reg_base_addr);

	pdata->irq = platform_get_irq_byname(pdev, "asp_dma_irq");
	if (0 > pdata->irq) {
		dev_err(dev, "cannot get irq\n");
		return -ENOENT;
	}
	logi("request_irq %d  success", pdata->irq);
	
	pdata->asp_ip = devm_regulator_get(dev, "asp-dmac");
	if (IS_ERR(pdata->asp_ip)) {
		dev_err(dev, "couldn't get regulators %d\n", ret);
		return -ENOENT;
	}
		
	pdata->hwlock = hwspin_lock_request_specific(HWLOCK_ID);
	if(NULL == pdata->hwlock) {
		dev_err(dev, "couldn't request hwlock:%d\n", HWLOCK_ID);
		return -ENOENT;
	}

	spin_lock_init(&pdata->lock);

	pdata->dev = dev;

#ifdef CONFIG_PM_RUNTIME
	pm_runtime_set_autosuspend_delay(dev, 100); /* 100ms*/
	pm_runtime_use_autosuspend(dev);

	pm_runtime_enable(dev);
#endif

	platform_set_drvdata(pdev, pdata);

	dev_set_name(dev, "hi3630-pcm-asp-dma");

	ret = snd_soc_register_platform(dev, &hi3xxx_pcm_asp_dmac_platform);
	if (ret) {
		loge("snd_soc_register_platform return %d\n" ,ret);
		return -ENODEV;
	}

	return 0;
}

static int hi3xxx_asp_dmac_remove(struct platform_device *pdev)
{
	snd_soc_unregister_platform(&pdev->dev);
#ifdef CONFIG_PM_RUNTIME
	pm_runtime_disable(&pdev->dev);
#endif
	return 0;
}

#ifdef CONFIG_PM_RUNTIME
int hi3xxx_asp_dmac_runtime_suspend(struct device *dev)
{
	struct hi3xxx_asp_dmac_data *pdata = dev_get_drvdata(dev);

	BUG_ON(NULL == pdata);

	dev_info(dev, "%s+", __FUNCTION__);

	regulator_disable(pdata->asp_ip);

	dev_info(dev, "%s-", __FUNCTION__);

	return 0;
}

int hi3xxx_asp_dmac_runtime_resume(struct device *dev)
{
	struct hi3xxx_asp_dmac_data *pdata = dev_get_drvdata(dev);
	int ret;

	BUG_ON(NULL == pdata);

	dev_info(dev, "%s+", __FUNCTION__);

	ret = regulator_enable(pdata->asp_ip);
	if (0 != ret)
		dev_err(dev, "couldn't enable regulators %d\n", ret);

	dev_info(dev, "%s-", __FUNCTION__);

	return ret;
}

const struct dev_pm_ops hi3xxx_asp_dmac_pm_ops = {
	.runtime_suspend	= hi3xxx_asp_dmac_runtime_suspend,
	.runtime_resume		= hi3xxx_asp_dmac_runtime_resume,
};
#endif

static struct platform_driver hi3xxx_asp_dmac_driver = {
	.driver = {
		.name	= "hi3630-pcm-asp-dma",
		.owner	= THIS_MODULE,
		.of_match_table = hi3xxx_asp_dmac_of_match,
#ifdef CONFIG_PM_RUNTIME
		.pm	= &hi3xxx_asp_dmac_pm_ops,
#endif
	},
	.probe	= hi3xxx_asp_dmac_probe,
	.remove	= hi3xxx_asp_dmac_remove,
};
module_platform_driver(hi3xxx_asp_dmac_driver);

MODULE_AUTHOR("chengong <apollo.chengong@huawei.com>");
MODULE_DESCRIPTION("Hi3630 ASP DMA Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:hi3xxx_asp_dmac");
