

#ifndef ICC_BALONG_CORE_H
#define ICC_BALONG_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <osl_types.h>
#include <osl_sem.h>
#include <osl_spinlock.h>
#include <osl_wait.h>
#include <osl_malloc.h>
#include <bsp_reset.h>
#include <bsp_icc.h>
#include <icc_platform.h>

extern struct icc_dbg g_icc_dbg;

#ifndef BSP_ERR_ICC_BASE
#define BSP_ERR_ICC_BASE                ((int)(0x80000000 | 0x10180000))
#endif

#ifndef BSP_ERR_ICC_CCORE_RESETTING
#define BSP_ERR_ICC_CCORE_RESETTING    (BSP_ERR_ICC_BASE + 0x12)
#endif

#define icc_safe_free(p) \
do \
{ \
	if(p) \
	{ \
		osl_free((void *)p); \
		p = NULL; \
	} \
} \
while(0)

#define ICC_MIN(x, y)             ((x) < (y) ? (x) : (y))
#define GET_CHN_ID(channel_id)    (((channel_id)>>16) & 0xffff)
#define GET_FUNC_ID(channel_id)   ((channel_id) & 0xffff)

/* �궨��start */
#define ICC_CHN_MAGIC_SIGN    (0x87651234)
#define ICC_CHN_MAGIC_UNSIGN  (0xabcd8765)


#define PRIVATE_MODE          ((ICC_TASK_PRIVATE << 1) |(ICC_IPC_PRIVATE))
#define SHARED_MODE           ((ICC_TASK_SHARED << 1) |(ICC_IPC_SHARED))
#define ICC_SHARED_IPC_CHN_IDX (0xffffffff)

#define ICC_SDDR_ADDR_MAX      (SHM_ADDR_ICC + SHM_SIZE_ICC)   /* ����ICC�Ĺ���DDR���ܴ��ڴ˵�ַ */  
#define ICC_SRAM_ADDR_MAX      (SRAM_ADDR_ICC + SRAM_SIZE_ICC) /* ����ICC��SRAM���ܴ��ڴ˵�ַ */  

#define ICC_SEM_FULL          (1)
#define ICC_SEM_EMPTY         (0)

struct icc_init_info
{
	u32        real_channel_id;
	u32        mode;
	u32        fifo_size;
	char*      recv_addr;
	char*      send_addr;	
	u32        ipc_recv_irq_id;
	u32        ipc_send_irq_id;
	u32        func_size;
	const char *name;
};

/* ö�ٶ���start */
/* icc״̬ */
enum ICC_STATUS
{
	ICC_UNINITIALIZED = 0,
	ICC_INITIALIZED,
	ICC_STATUS_MAX
};

/* ���������� */
enum ICC_TASK_TYPE
{
	ICC_TASK_PRIVATE = 0,
	ICC_TASK_SHARED,
	ICC_TASK_TYPE_MAX
};

enum IPC_VEC_TYPE
{
	ICC_IPC_PRIVATE = 0,
	ICC_IPC_SHARED,
	IPC_VEC_TYPE_MAX
};

/* ͨ��״̬ */
enum ICC_CHN_STAT
{
	ICC_CHN_CLOSED = 0,
	ICC_CHN_OPENED,
	ICC_CHN_STAT_MAX
};

enum ICC_PACKET_RETURN
{
	ICC_PACKET_NONEED_RESPONSE = 0,
	ICC_PACKET_NEED_RESPONSE,
	ICC_PACKET_RETURN_MAX
};

/* ö�ٶ���end */

union ipc_task_shared
{
	u32 val;
	struct
	{
		u32 ipc_shared :1;
		u32 task_shared:1;
		u32 reserved   :30;
	}union_stru;
};

struct icc_channel
{
	u32                       id;               /* ͨ��id */
	const char                *name;            /* ͨ������ */
	u32                       state;            /* ͨ��״̬: ��ʶ����ͨ���Ƿ���� */
	u32                       ready_recv;       /* ͨ��״̬: ��ʶ����ͨ���Ƿ���� */
	union ipc_task_shared     mode;             /* ͨ�����մ�����������: shared|private */
	icc_task_id               private_task_id;  /* ͨ��˽�еĴ�������id */
	u32                       ipc_send_irq_id;  /* ͨ��˽�еķ�������ʹ�õ�ipc�ж� */
	u32                       ipc_recv_irq_id;  /* ͨ��˽�еĽ�������ʹ�õ�ipc�ж� */
	osl_sem_id                private_task_sem; /* ����ͨ��˽�еĽ���������ź��� */
	struct icc_channel_fifo   *fifo_recv;       /* ����fifo��ָ�� */
	struct icc_channel_fifo   *fifo_send;       /* ����fifo��ָ�� */
	spinlock_t                write_lock;       /* ����ͨ��д�����spin�� */
	spinlock_t                read_lock;        /* ����ͨ���������spin�� */
	struct icc_channel_vector *vector;          /* ��������(��ͨ��)ָ�� */
	u32                       func_size;        /* ��������(��ͨ��)��С */
};

struct channel_dts_cfg{
    unsigned int id;             /* ����ͨ��ID */
	unsigned int mode;           /* ͨ��ģʽ:����or���� */
	unsigned int size;           /* ��ͨ������fifo������fifo��С */
	unsigned int tx_ipc;         /* ���͵��Է��˵�IPC�жϺ� */
	unsigned int rx_ipc;         /* �ڴ�IPC�жϺ��Ͻ��� */
	unsigned int func_size;      /* ֧�ֵ�func_size���� */
	unsigned int rx_fifo_first;  /* ��ͨ�������ڱ��ˣ�����fifo��ǰ */
};

struct icc_wait_data
{
	u32 channel_id;
	u32 packet_seq_num;
};

/* ��Ϣ���Ͷ���for debug */
enum ICC_MSG_TYPE
{
	ICC_MSG_SEND = 0,
	ICC_MSG_RECV,
	ICC_MSG_TYPE_MAX
};


/*** ����/������Ϣͳ�ƣ�����ϢΪά�ȣ���Ҫ����dump�ӿڣ��Ա��쳣ʱ���� ***/
#define ICC_MSG_RECORED_DATA_LEN  40
#define ICC_STAT_MSG_NUM          10
struct icc_uni_msg_info
{
	u32 channel_id;    /* channel_id + func_id */
	u32 send_task_id;
	u32 recv_task_id;
	u32 len;            /* �û����ݳ��ȣ�������pkgͷ */
	u32 write_pos;      /* writeλ�� */
	u32 read_pos;       /* readλ�� */
	u32 duration_prev;  /* ���ͻ��߽���֮ǰ��slice */
	u32 duration_post;  /* ���ͻ��߽���֮���slice */
	u8 data[ICC_MSG_RECORED_DATA_LEN];       
};

struct icc_debug_perf
{
    u32 max_readcb_consume_slice; /* ���ص�ִ�����ʱ�� */
	u32 max_readcb_channel_id;    /* ���ص�ִ���ʱ���chan_id_func_id */
	u32 cnt;   /* ͳ�ƵĴ��� */
	u32 sum;   /* ͳ�Ƶ��ܵ�ִ��ʱ��(slice)���ӷ��͵�������� */
	u32 avg;   /* ͳ�Ƶ�ƽ��ִ��ʱ��(slice)���ӷ��͵�������� */
};

struct icc_recv_msg_info
{
	u32 channel_id;    /* channel_id + func_id */
	u32 send_task_id;
	u32 recv_task_id;
	u32 len;            /* �û����ݳ��ȣ�������pkgͷ */
	u32 write_pos;      /* writeλ�� */
	u32 read_pos;       /* readλ�� */
	u32 before_recv_slice;  /* ���ͻ��߽���֮ǰ��slice */
	u32 in_read_cb_slice;  /* ���ջص��е�slice */
	u32 after_recv_slice;  /* ���ͻ��߽���֮���slice */
	struct icc_debug_perf perf; /* ����ͳ�� */
};

struct icc_msg_fifo
{
	u32 front;
	u32 rear;
	u32 size;
	struct icc_uni_msg_info msg[ICC_STAT_MSG_NUM];
};

struct icc_msg_fifo_recv
{
	u32 front;
	u32 rear;
	u32 size;
	struct icc_recv_msg_info msg[ICC_STAT_MSG_NUM];
};

struct icc_msg_info
{
	struct icc_msg_fifo send;
	struct icc_msg_fifo_recv recv;
};

/*** ����/������Ϣͳ�ƣ���ͨ��Ϊά�ȣ�����ͨ������ ***/
struct icc_channel_stat_info
{
	u32 sum_len;
	u32 sum_num;
	u32 sum_num_prev;
	u32 task_id;
};

struct icc_uni_channel_info
{
	struct icc_channel_stat_info total;
	struct icc_channel_stat_info *sub_chn;
	u32 func_size;
};

struct icc_channel_info
{
	u32 id;
	struct icc_uni_channel_info send;
	struct icc_uni_channel_info recv;
};

struct icc_dbg
{
	u32 state;
	u32 ipc_int_cnt;
	u32 msg_print_sw;
	u32 send_cpu_id;
	u32 recv_task_id;
	u32 timestamp;
	char *dump_buf_addr;
	u32 dump_buf_size;
    u32 reserved; /* for 8byte allignment */
	struct icc_msg_info msg_stat;
	struct icc_channel_info *channel_stat[ICC_CHN_ID_MAX];
};

void icc_sleep_flag_set(void);
u32 fifo_get(struct icc_channel_fifo *fifo, u8 *data_buf, u32 data_len, u32 *read);
void icc_restore_recv_channel_flag(struct icc_channel_fifo * channel_fifo);
s32 icc_channel_has_data(void);
void icc_sem_init(void);
int icc_shared_task_init(void);
void icc_task_shared_func(void const *obj);
s32 bsp_icc_channel_reset(DRV_RESET_CB_MOMENT_E stage, int usrdata);

#ifdef __cplusplus
}
#endif

#endif

