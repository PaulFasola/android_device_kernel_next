/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : AgingTest.h
  版 本 号   : 初稿
  作    者   : 甘兰 47350
  生成日期   : 2009年10月27日
  最近修改   :
  功能描述   : OmRl.h 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2009年10月27日
    作    者   : 甘兰 47350
    修改内容   : 创建文件

******************************************************************************/

#ifndef __AGINGTEST_H__
#define __AGINGTEST_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#include "vos.h"
#include "omprivate.h"

#pragma pack(4)

/*****************************************************************************
  2 宏定义
*****************************************************************************/

/*配置GRF使用的掩码，指示需要配置哪些项*/
#define OM_G_RF_TX_BAND_CHANNEL_MASK              0x2
#define OM_G_RF_TX_ONOFF_MASK                     0x4
#define OM_G_RF_TX_MODE_MASK                      0x6
#define OM_G_RF_TX_MODE_TYPE_MASK                 0x8
#define OM_G_RF_TX_POWER_MASK                     0x40
/*配置WRF使用的掩码，指示需要配置哪些项*/
#define OM_W_RF_TX_BAND_CHANNEL_MASK              0x2
#define OM_W_RF_TX_ONOFF_MASK                     0x4
#define OM_W_RF_TX_PAMODE_MASK                    0x8
#define OM_W_RF_TX_POWER_MASK                     0x20
/*GSM Band的移位值*/
#define OM_G_RF_BAND_SHIFT                        12

#define OM_AGING_START_TIMER                       1 /*指示老化测试时启动的超时定时器*/
#define OM_AGING_TEST_TIMER                        2 /*老化测试超时定时器*/

/*启动定时器*/
#define OM_START_AGING_TEST_TIMER(phTimer, ulLen)  VOS_StartRelTimer(phTimer, \
                      WUEPS_PID_AGING, ulLen, OM_AGING_TEST_TIMER, 0, VOS_RELTIMER_NOLOOP, VOS_TIMER_NO_PRECISION)

/*关闭定时器*/
#define OM_STOP_AGING_TEST_TIMER(phTimer)          VOS_StopRelTimer(phTimer)

/*****************************************************************************
  3 枚举定义
*****************************************************************************/


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

typedef struct
{
    VOS_UINT16                          usBandIndex;
    MODEM_ID_ENUM_UINT16                enModemID;
    VOS_UINT16                          usNetworkMode;
}OM_AGING_TEST_CTRL_STRU;


/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/


#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of OmRl.h */

