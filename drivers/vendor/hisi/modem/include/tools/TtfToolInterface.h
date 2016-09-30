

#ifndef _TTF_TOOL_INTERFACE_H_
#define _TTF_TOOL_INTERFACE_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "vos.h"

/*****************************************************************************
  2 宏定义
*****************************************************************************/

/*******************************************************************************
  3 枚举定义
*******************************************************************************/


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
#pragma pack(4)

/*上行吞吐率实体*/
typedef struct
{
    VOS_UINT32                              ulUlThroughput;          /*上行吞吐率B/s，消息上报定时器设为1s，所以这里每次上报之后重新统计*/
    VOS_UINT32                              ulUlNewDataBlk;          /*上行新块数量，按1s统计*/
    VOS_UINT32                              ulUlNackDataBlk;         /*上行NACK重传块数量，按1s统计*/
    VOS_UINT32                              ulUlPendDataBlk;         /*上行PENDING重传块数量，按1s统计*/
    VOS_UINT32                              ulUlDummyBlk;            /*上行Dummy块数量，按1s统计*/
}GRM_MNTN_GRLC_UL_THROUGHPUT_ENTITY;

/*下行吞吐率实体*/
typedef struct
{
    VOS_UINT32                              ulDlThroughput;          /*下行吞吐率 B/s*/
    VOS_UINT32                              ulDlDataAmount;          /*下行数据块总数, 按1s统计*/
    VOS_UINT32                              ulDlCorrectData;         /*译码正确的数据块, 按1s统计*/
    VOS_UINT32                              ulDlDataOkBeforeHarq;    /*HARQ前译码正确的数据块, 在GPRS模式下没有HARQ译码，所以在GPRS模式下使用该域进行数据统计，按1s统计*/
    VOS_UINT32                              ulDlPendDataBlk;         /*下行PENGDING重传块数量，按1s统计*/
    VOS_UINT32                              ulDlDataErrBeforeHarq;   /* HARQ前译码错误的数据块，按1s统计*/
}GRM_MNTN_GRLC_DL_THROUGHPUT_ENTITY;

/*GRLC可维可测消息实体*/
typedef struct
{
    GRM_MNTN_GRLC_UL_THROUGHPUT_ENTITY      stUlThroughputEntity;   /*上行吞吐率信息*/
    GRM_MNTN_GRLC_DL_THROUGHPUT_ENTITY      stDlThroughputEntity;   /*下行吞吐率信息*/
}GRM_MNTN_GRM_OM_THROUGHPUT_INFO_STRU;

/****************************************************/


/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
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

#endif /* end of TtfToolInterface.h*/


