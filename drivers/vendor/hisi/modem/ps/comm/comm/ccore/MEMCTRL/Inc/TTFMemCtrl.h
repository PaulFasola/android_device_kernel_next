

#ifndef __TTF_MEMCTRL_H__
#define __TTF_MEMCTRL_H__


/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "TtfMemoryMap.h"
#include "vos.h"
#include "TtfNvInterface.h"
#include "NVIM_Interface.h"
#include "NasNvInterface.h"
#include "PsCommonDef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

/*****************************************************************************
  2 宏定义
*****************************************************************************/
/* TTF内存类型初始化标记 */
#define TTF_MEMCTRL_TYPE_INIT_FLG                       (1)

/* TTF内存类型存在不存在标记 */
#define TTF_MEMCTRL_TYPE_EXIST_FLG                      (1)

/* TTF内存类型可见不可见标记 */
#define TTF_MEMCTRL_TYPE_VISIBLE_FLG                    (1<<1)

#define TTF_MEMCTRL_INVAILD_SIZE                        (0xFFFFFFFF)

#define TTF_MEMCTRL_SET_ADDR_INIT_FLG(flg)              (g_stTtfMemCtrlAddr.stTtfMemCtrlAddrComm.ucInitFlg = flg)

#define TTF_MEMCTRL_GET_ADDR_INIT_FLG()                 (g_stTtfMemCtrlAddr.stTtfMemCtrlAddrComm.ucInitFlg)

#define TTF_MEMCTRL_SET_TYPE_FLG(type, flg)             (g_stTtfMemCtrlAddr.astTtfMemCtrlAddrType[type].ulAttribute |= flg)

#define TTF_MEMCTRL_GET_TYPE_ATTRIBUTE(type)            (g_stTtfMemCtrlAddr.astTtfMemCtrlAddrType[type].ulAttribute)

#define TTF_MEMCTRL_SET_TYPE_LEN(type, len)             (g_stTtfMemCtrlAddr.astTtfMemCtrlAddrType[type].ulLen = len)

#define TTF_MEMCTRL_GET_TYPE_LEN(type)                  (g_stTtfMemCtrlAddr.astTtfMemCtrlAddrType[type].ulLen)

#define TTF_MEMCTRL_SET_TYPE_OFFSET_BASE_LEN(type, len) (g_stTtfMemCtrlAddr.astTtfMemCtrlAddrType[type].ulOffsetBaseLen = len)

#define TTF_MEMCTRL_GET_TYPE_OFFSET_BASE_LEN(type)      (g_stTtfMemCtrlAddr.astTtfMemCtrlAddrType[type].ulOffsetBaseLen)

/* 内存保护魔术字长度 */
#define TTF_MEM_MAGIC_NUM_LEN                           (32)

/*魔术字初始化值 */
#define TTF_MEM_MAGIC_NUM_VALUE                         (0x5A)

#define TTF_DYNAMIC_MEM_LEN_ZERO                        (0)

#define TTF_MEMCTRL_PID_INVAILD                         (0xFF)

#if (OSA_CPU_CCPU == VOS_OSA_CPU)
/* 向BSP动态申请uncached内存, 申请成功返回实际地址，申请失败则返回VOS_NULL_PTR */
#define TTF_UNCACHED_MEM_BSP_ALLOC(ulTtfMemByteSize)   mdrv_cachedmem_alloc(ulTtfMemByteSize)
#else
/* 当前内存都是在C核的uncache内存，A核保证编译和lint OK */
#define TTF_UNCACHED_MEM_BSP_ALLOC(ulTtfMemByteSize)   kmalloc(ulTtfMemByteSize, GFP_KERNEL)
#endif
#define TTF_MEM_SET_DYNAMIC_INIT_ADDR_FLAG(ulInitFlag) \
    (g_stTtfMemDynamicCtrlEntity.ulTtfMemDynamicInitAddrFlag = (ulInitFlag))

#define TTF_MEM_GET_DYNAMIC_INIT_ADDR_FLAG() \
    (g_stTtfMemDynamicCtrlEntity.ulTtfMemDynamicInitAddrFlag)

#define TTF_MEM_SET_SUPPORT_WCDMA(Modem, IsSupportWcdma) \
    (g_stTtfMemDynamicCtrlEntity.astModelSupport[Modem].enSupportWcdma = (IsSupportWcdma))

#define TTF_MEM_SET_SUPPORT_GSM(Modem, IsSupportGsm) \
    (g_stTtfMemDynamicCtrlEntity.astModelSupport[Modem].enSupportGsm = (IsSupportGsm))

#define TTF_MEM_GET_DYNAMIC_ADDR_TYPE() \
    (g_stTtfMemDynamicCtrlEntity.astTtfMemAddrType[0])

#define TTF_MEM_SET_TTF_BASE_ADDR(ulTtfMemBaseAddr) \
    (g_stTtfMemDynamicCtrlEntity.pucTtfMemBaseAddr = (ulTtfMemBaseAddr))

#define TTF_MEM_GET_TTF_BASE_ADDR() \
    (g_stTtfMemDynamicCtrlEntity.pucTtfMemBaseAddr)

#define TTF_MEM_SET_TTF_TOTAL_LEN(ulDynamicTtfMemLen) \
    (g_stTtfMemDynamicCtrlEntity.ulMemTotalLen = (ulDynamicTtfMemLen))

#define TTF_MEM_GET_TTF_TOTAL_LEN() \
    (g_stTtfMemDynamicCtrlEntity.ulMemTotalLen)

#define TTF_DYNAMIC_MEM_SET_OFFSETBASE(type, OffsetBaseLen) \
    (g_stTtfMemDynamicCtrlEntity.astTtfMemAddrType[type].ulOffsetBaseLen = OffsetBaseLen)

#define TTF_DYNAMIC_MEM_GET_OFFSETBASE(type) \
    (g_stTtfMemDynamicCtrlEntity.astTtfMemAddrType[type].ulOffsetBaseLen)

#define TTF_DYNAMIC_MEM_SET_LEN(type, len) \
    (g_stTtfMemDynamicCtrlEntity.astTtfMemAddrType[type].ulLen = len)

#define TTF_DYNAMIC_MEM_GET_LEN(type) \
    (g_stTtfMemDynamicCtrlEntity.astTtfMemAddrType[type].ulLen)

#define TTF_DYNAMIC_MEM_SET_ATTRIBUTE(type, attribute) \
    (g_stTtfMemDynamicCtrlEntity.astTtfMemAddrType[type].ulAttribute |= attribute)

#define TTF_DYNAMIC_MEM_GET_ATTRIBUTE(type) \
    (g_stTtfMemDynamicCtrlEntity.astTtfMemAddrType[type].ulAttribute)

#define WTTF_BBPMST_MAC_TB_HEAD_NUM                 (g_stBbpMstTBHeadInfo.ulTBHeadNum)
#define WTTF_BBPMST_MAC_TB_DATA_NUM                 (g_stBbpMstTBHeadInfo.ulTBHeadNum - g_stBbpMstTBHeadInfo.ulTBHeadReserved)

/* 头部区长度，单位:字节 */
#define WTTF_BBPMST_MAC_TB_HEAD_BUFF_LEN            ((WTTF_BBPMST_MAC_TB_HEAD_NUM) * (WTTF_MAC_HSDPA_TB_HEAD_SIZE))

/* 数据区长度，单位:字节 */
#define WTTF_BBPMST_MAC_TB_DATA_BUFF_LEN            ((WTTF_BBPMST_MAC_TB_DATA_NUM) * (WTTF_MAC_HSDPA_TB_DATA_SIZE))

#define TTF_DYNAMIC_MEM_CHECK_PERIOD_COUNTER        (1000)

/*****************************************************************************
 枚举名    : TTF_MEM_PLATFORM_SUPPORT_WCDMA_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : 是否支持wcdma
*****************************************************************************/
enum TTF_MEM_PLATFORM_SUPPORT_WCDMA_ENUM
{
    TTF_MEM_PLATFORM_SUPPORT_WCDMA  = 0,                /* 支持WCDMA */
    TTF_MEM_PLATFORM_NOT_SUPPORT_WCDMA,                 /* 不支持WCDMA */
    TTF_MEM_PLATFORM_SUPPORT_BUTT
};
typedef VOS_UINT8 TTF_MEM_PLATFORM_SUPPORT_WCDMA_ENUM8;

/*****************************************************************************
 枚举名    : TTF_MEMCTRL_CALLBACK_FUNC_FLAG_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : 回调函数表示
*****************************************************************************/
enum TTF_MEMCTRL_CALLBACK_FUNC_FLAG_ENUM
{
    TTF_MEMCTRL_CALLBACK_FUNC_DRV  = 0,                /* 底软回调 */
    TTF_MEMCTRL_CALLBACK_FUNC_TTF,                     /* TTF回调 */

    TTF_MEMCTRL_CALLBACK_FUNC_BUTT
};
typedef VOS_UINT8 TTF_MEMCTRL_CALLBACK_FUNC_FLAG_ENUM_ENUM8;

/*****************************************************************************
 枚举名    : TTF_MEM_MSG_ID_ENUM_UINT16
 协议表格  :
 ASN.1描述 :
 枚举说明  : 消息上报ID
*****************************************************************************/
enum TTF_MEM_MSG_ID_ENUM
{
    ID_TRACE_TTF_MEM_LEAK_IND               = 0X0315,   /* _H2ASN_MsgChoice TTF_MEM_LEAK_INFO_IND_MSG */
    ID_TRACE_TTF_MEM_ERR_IND                = 0x0316,   /* _H2ASN_MsgChoice TTF_MEM_ERROR_INFO_IND_STRU */

    ID_TRACE_TTF_MEM_BUTT
};
typedef VOS_UINT16 TTF_MEM_MSG_ID_ENUM_UINT16;

enum TTF_MEM_POOL_BLK_INDEX_ENUM
{
    TTF_MEM_POOL_BLK_INDEX_0    = 0,
    TTF_MEM_POOL_BLK_INDEX_1,
    TTF_MEM_POOL_BLK_INDEX_2,
    TTF_MEM_POOL_BLK_INDEX_3,

    TTF_MEM_POOL_BLK_INDEX_BUTT
};
typedef VOS_UINT32 TTF_MEM_POOL_BLK_INDEX_ENUM_UINT32;

/*****************************************************************************
  5 消息头定义
*****************************************************************************/


/*****************************************************************************
  6 消息定义
*****************************************************************************/


/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/
/*****************************************************************************
结构名    : TTF_MEMCTRL_ADDR_COMM_STRU
协议表格  : 无
ASN.1描述 : 无
结构说明  : TTF内存地址公共结构
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucInitFlg;          /* 初始化标记 */
    VOS_UINT8                           ucRcv[3];           /* 保留位 */
} TTF_MEMCTRL_ADDR_COMM_STRU;

/*****************************************************************************
结构名    : TTF_MEMCTRL_ADDR_TYPE_STRU
协议表格  : 无
ASN.1描述 : 无
结构说明  : TTF内存地址类型结构
*****************************************************************************/
typedef struct
{
    TTF_MEMCTRL_ADDR_COMM_STRU          stTtfMemCtrlAddrComm;
    TTF_MEMCTRL_ADDR_TYPE_STRU          astTtfMemCtrlAddrType[TTF_MEMCTRL_ADDR_TYPE_BUTT];
} TTF_MEMCTRL_ADDR_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT16                              usMsgName;
    VOS_UINT8                               aucRsv[2];
    TTF_DYNAMIC_MEM_ADDR_TYPE_ENUM_UINT32   ulMagicNumFlag;
    VOS_UINT32                              aulMagicNum[TTF_MEM_MAGIC_NUM_LEN];
    TTF_MEMCTRL_ADDR_TYPE_STRU              astTtfMemAddrType[TTF_DYNAMIC_MEM_ADDR_TYPE_BUTT];
} TTF_MEM_ERROR_INFO_IND_STRU;

#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

/*****************************************************************************
  4 全局变量声明
*****************************************************************************/
extern TTF_MEMCTRL_ADDR_STRU                   g_stTtfMemCtrlAddr;

/* BBPMST头部TB块信息 */
extern WTTF_MACDL_BBPMST_TB_HEAD_STRU          g_stBbpMstTBHeadInfo;

extern TTF_MEM_SOLUTION_CFG_NV_STRU            g_stTtfMemSolution;
/*****************************************************************************
  5 OTHERS定义
*****************************************************************************/



/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/



/*****************************************************************************
  10 函数声明
*****************************************************************************/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of Fc.h */


