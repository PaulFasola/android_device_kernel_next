/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and 
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may 
 * *    be used to endorse or promote products derived from this software 
 * *    without specific prior written permission.
 * 
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __BSP_PMU_HI6561_H__
#define __BSP_PMU_HI6561_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <bsp_shared_ddr.h>

#define SRAM_PASTAR_DPM_INFO SHM_OFFSET_PASTAR_DPM_INFO
#define PASTAR_DPM_SUSPEND_MASK_OFFSET 0
#define PASTAR_DPM_EN_TIMESTAMP_OFFSET 4
#define PASTAR_DPM_SWITCH_OFFSET 8

#if defined (__KERNEL__) || defined(__OS_RTOSCK__) || defined(__OS_VXWORKS__)
#include <mdrv_pmu.h>
#include <mdrv_pm.h>
#include <osl_spinlock.h>
#include <osl_types.h>

#define PMU_HI6561_OM_LOG            "/modem_log/log/pmu_hi6561_om.log"
#define RF_POWER_FROM_LDO 2
#define RF_POWER_FROM_HI6561 1
#define RF_POWER_FROM_NONE 0

#define PA_POWER_FROM_VSYS      2
#define PA_POWER_FROM_HI6561    1
#define PA_POWER_FROM_NONE      0

/*hi6561 power source id*/
typedef enum HI6561_POWER_ID_E
{
    PMU_HI6561_LDO1 = 0,
    PMU_HI6561_LDO2,
    PMU_HI6561_BUCK_PA,
    PMU_HI6561_BUCK1,
    PMU_HI6561_BUCK2,
    PMU_HI6561_POWER_ID_BUTT
}HI6561_POWER_ID;

typedef enum{
	HI6561_0=0,
	HI6561_1,
	HI6561_BUTT
}HI6561_ID_ENUM;

/****************************************************************
*
*function declare

*****************************************************************/

/*****************************************************************************
 函 数 名  : drv_pmu_hi6561_exc_check
 功能描述  : 通信模块检查PASTAR是否有异常接口
 输入参数  : modem_id       卡号
 输出参数  : 无
 返 回 值  : BSP_OK          没有异常
             BSP_ERROR       存在异常
*****************************************************************************/
s32 drv_pmu_hi6561_exc_check(PWC_COMM_MODEM_E modem_id);
/*****************************************************************************
 函 数 名  : drv_pmu_hi6561_voltage_set
 功能描述  : 通信模块设置电压接口
 输入参数  : consumer_id     用户id
             voltage_mv      设置的电压值毫伏
 输出参数  : 无
 返 回 值  : BSP_OK          设置成功
             BSP_ERROR       设置失败
*****************************************************************************/
s32 drv_pmu_hi6561_voltage_set(EM_MODEM_CONSUMER_ID_E consumer_id, u32 voltage_mv );

/*****************************************************************************
 函 数 名  : drv_pmu_hi6561_voltage_get
 功能描述  : 通信模块获取电压接口
 输入参数  : consumer_id     用户id
             voltage_mv      获得的电压值毫伏
 输出参数  : 无
 返 回 值  : BSP_OK          获取成功
             BSP_ERROR       获取失败
*****************************************************************************/
s32 drv_pmu_hi6561_voltage_get(EM_MODEM_CONSUMER_ID_E consumer_id, u32 *voltage_mv );

/*****************************************************************************
 函 数 名  : drv_pmu_hi6561_voltage_list
 功能描述  : 通信模块获取电压设置范围接口
 输入参数  : consumer_id     用户id
             list            电压范围数组
             size            数组大小
 输出参数  : 无
 返 回 值  : BSP_OK          获取成功
             BSP_ERROR       获取失败
*****************************************************************************/
s32 drv_pmu_hi6561_voltage_list(EM_MODEM_CONSUMER_ID_E consumer_id, u16 **list, u32 *size);

/*****************************************************************************
 函数	: drv_pmu_hi6561_apt_enable
 功能	: 通信模块使能APT状态接口
 输入	: modem_id       卡号
 输出	: 无
 返回	: 1  APT使能/   0    APT未使能/  -1    获取失败
*****************************************************************************/
s32 drv_pmu_hi6561_apt_enable(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id);

/*****************************************************************************
 函数	: drv_pmu_hi6561_apt_disable
 功能	: 通信模块去使能APT状态接口
 输入	: modem_id       卡号
 输出	: 无
 返回	: 1  APT使能/   0    APT未使能/  -1    获取失败
*****************************************************************************/
s32 drv_pmu_hi6561_apt_disable(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id);

/*****************************************************************************
 函数	: drv_pmu_hi6561_apt_status_get
 功能	: 通信模块获取当前APT状态接口
 输入	: modem_id       卡号
 输出	: 无
 返回	: 1  APT使能/   0    APT未使能/  -1    获取失败
*****************************************************************************/
s32 drv_pmu_hi6561_apt_status_get(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id);

/*****************************************************************************
 函数 	: drv_pmu_hi6561_mode_config
 功能	: 通信模块配置G模或W模接口
 输入	: modem_id       卡号
 输出	: 无
 返回  : BSP_OK         配置成功/  BSP_ERROR      配置失败
*****************************************************************************/
s32 drv_pmu_hi6561_mode_config(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id);

/*****************************************************************************
 fun_name	: pmu_hi6561_init
 function		: hi6561 init
 para_in		: void
 para_out		: no
 back_val    	:
 			  0:success
 			-1:fail
*****************************************************************************/
int pmu_hi6561_init_phase1(void);
int pmu_hi6561_init_phase2(void);

/*****************************************************************************
 函 数 名  : bsp_pmu_hi6561_pa_poweron
 功能描述  :PA 上电
 输入参数  : 无

 输出参数  : 无
 返 回 值  : 无
*****************************************************************************/
int bsp_pmu_hi6561_pa_poweron(PWC_COMM_MODEM_E modem_id);

/*****************************************************************************
 函 数 名  : bsp_pmu_hi6561_pa_poweroff
 功能描述  :PA 下电
 输入参数  : 无

 输出参数  : 无
 返 回 值  : 无
*****************************************************************************/
int bsp_pmu_hi6561_pa_poweroff(PWC_COMM_MODEM_E modem_id);

/*****************************************************************************
 函 数 名  : PWRCTRL_RfPowerDown
 功能描述  :RF 下电
 输入参数  : 无

 输出参数  : 无
 返 回 值  : 无
*****************************************************************************/

int bsp_pmu_hi6561_rf_poweroff(PWC_COMM_MODEM_E modem_id);

/*****************************************************************************
 函 数 名  : bsp_pmu_hi6561_rf_poweron
 功能描述  :RF 下电
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
*****************************************************************************/
int bsp_pmu_hi6561_rf_poweron(PWC_COMM_MODEM_E modem_id);

/*****************************************************************************
 函 数 名     : bsp_pmu_hi6561_get_rf_powerstatus
 功能描述  :RF 电源状态查询
 输入参数  : 无

 输出参数  :
 				PWC_COMM_STATUS_BUTT :error
 				-1:error

 				0x10:power on
 				0x20:power off
 返 回 值  : pa电源开关状态
*****************************************************************************/
PWC_COMM_STATUS_E bsp_pmu_hi6561_get_pa_powerstatus(PWC_COMM_MODEM_E modem_id);

/*****************************************************************************
 函 数 名     : bsp_pmu_hi6561_get_rf_powerstatus
 功能描述  :RF 电源状态查询
 输入参数  : 无

 输出参数  :
 				PWC_COMM_STATUS_BUTT :error

 				0x10:power on
 				0x20:power off
 返 回 值  : rf电源开关状态
*****************************************************************************/
PWC_COMM_STATUS_E bsp_pmu_hi6561_get_rf_powerstatus(PWC_COMM_MODEM_E modem_id);

int pmu_hi6561_voltage_list_get(HI6561_POWER_ID power_id,u16 **list, u32 *size);

int pmu_hi6561_debug(HI6561_ID_ENUM chip_id);

int pmu_hi6561_read_debug(HI6561_ID_ENUM chip_id,u8 reg_addr);


int adp_pmu_hi6561_initial(HI6561_ID_ENUM chip_id);

unsigned int pmu_hi6561_reg_save(void);


unsigned int pmu_hi6561_reg_resume(void);

/*****************************************************************************
 函 数 名  : pmu_hi6561_init
 功能描述  : pastar驱动初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 0 - 成功，else - 失败
*****************************************************************************/
int pmu_hi6561_init(void);


char *pmu_hi6561_exc_isr(HI6561_ID_ENUM chip_id );
int pmu_hi6561_voltage_set(HI6561_POWER_ID power_id,u16 voltage_mv,HI6561_ID_ENUM chip_id);
int pmu_hi6561_apt_disable(HI6561_ID_ENUM chip_id);
int pmu_hi6561_voltage_get(HI6561_POWER_ID power_id,u32 *voltage_mv,HI6561_ID_ENUM chip_id);
int pmu_hi6561_apt_enable(HI6561_ID_ENUM chip_id);
int pmu_hi6561_apt_status_get(HI6561_ID_ENUM chip_id);
int pmu_hi6561_power_on(HI6561_POWER_ID power_id ,HI6561_ID_ENUM chip_id);
int pmu_hi6561_power_off(HI6561_POWER_ID power_id,HI6561_ID_ENUM chip_id );
int pmu_hi6561_power_status(HI6561_POWER_ID power_id,u8 *status,HI6561_ID_ENUM chip_id);


#else
void pastar_resume_early(void);
void pastar_suspend_late(void);
void pastar_init(void);

#endif
#ifdef __cplusplus
}
#endif
#endif
