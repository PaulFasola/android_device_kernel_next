

#ifndef __TEST_ATPARSEEXTENDCMD_H__
#define __TEST_ATPARSEEXTENDCMD_H__

/*****************************************************************************
  1 其他头文件包含                            
*****************************************************************************/
#include "ATCmdProc.h"
#include "AtCheckFunc.h"
#include "AtParseCmd.h"
#include "AtCheckFunc.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 宏定义 
*****************************************************************************/


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


/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/

extern VOS_UINT32 atParseExtendCmd(IN VOS_UINT8 * pData, VOS_UINT16 usLen);
extern VOS_UINT32 AT_ParseSetDockCmd(
           VOS_UINT8                          *pucData,
           VOS_UINT16                          usLen
       );



extern void UT_STUB_INIT(void);




#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of Test_AtParseExtendCmd.h */
