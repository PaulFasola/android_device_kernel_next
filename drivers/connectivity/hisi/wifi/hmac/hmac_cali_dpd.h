/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hal_hi1102_cali_dpd.h
  版 本 号   : 初稿
  作    者   : f00311400
  生成日期   : 2015年05月22日
  最近修改   :
  功能描述   : hal_hi1102_cali_dpd.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2015年05月22日
    作    者   : f00311400
    修改内容   : 创建文件

******************************************************************************/

#ifndef __HAL_HI1102_CALI_DPD_H__
#define __HAL_HI1102_CALI_DPD_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_RF_110X_CALI_DPD

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "oal_ext_if.h"
#include "wlan_types.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_CALI_DPD_H

/*****************************************************************************
  2 宏定义
*************************************************************************/
#define DPD_CALI_LUT_LENGTH 128

/*****************************************************************************
  4 全局变量声明
*****************************************************************************/


/*****************************************************************************
  5 消息头定义
*****************************************************************************/


/*****************************************************************************
  6 消息定义
*****************************************************************************/


/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/

/* 复数结构 */
typedef struct
{
    oal_int32 l_real;
    oal_int32 l_imag;
}hi1102_complex_stru;

/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/

/*****************************************************************************
  10 函数声明
*****************************************************************************/
extern oal_void hi1102_rf_cali_dpd_corr_calc(oal_uint32* hi1102_dpd_cali_data_read,
                                                          oal_uint32* hi1102_dpd_cali_data_calc);




#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of hal_hi1102_cali_dpd.h */
