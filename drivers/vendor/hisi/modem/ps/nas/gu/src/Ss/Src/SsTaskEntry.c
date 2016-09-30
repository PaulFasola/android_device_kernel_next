/*******************************************************************************
  Copyright    : 2005-2007, Huawei Tech. Co., Ltd.
  File name    : SsTaskEntry.c
  Description  : SS TASK管理
  Function List:
      1.  Ss_TaskEntry_T    SS模块TASK入口处理
      2.  Ss_TaskInit       TASK 初始化处理
      3.  WuepsSsPidInit    WUEPS SS PID的初始化函数
  History:
      1.  张志勇 2004.03.08   新版作成
      2.  丁  庆 2006.11.07   问题单A32D07063
*******************************************************************************/

#include "SsInclude.h"
#include "MnComm.h"
#include "NVIM_Interface.h"

#include "NasNvInterface.h"
#include "TafNvInterface.h"
#include "TafSdcLib.h"

#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif


/*****************************************************************************
    协议栈打印打点方式下的.C文件宏定义
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_SS_TASKENTRY_C


/*lint -save -e958 */

VOS_VOID SSA_ReportErrorEvent(
    VOS_UINT16                          ClientId,
    VOS_UINT8                           OpId,
    TAF_SS_ERROR                        ErrorCode
)
{
    TAF_SS_CALL_INDEPENDENT_EVENT_STRU *pstSsEvent = VOS_NULL_PTR;

    pstSsEvent = (TAF_SS_CALL_INDEPENDENT_EVENT_STRU *)PS_MEM_ALLOC(WUEPS_PID_SS,
                            sizeof(TAF_SS_CALL_INDEPENDENT_EVENT_STRU));
    if (VOS_NULL_PTR == pstSsEvent)
    {
        SSA_LOG( ERROR_PRINT, "TAF_SSA_ReportEvtError: Alloc Mem Fail");
        return;
    }

    /* 出错事件的OP项初始化 */
    PS_MEM_SET(pstSsEvent, 0, sizeof(VOS_UINT32));
    pstSsEvent->OP_Error = VOS_TRUE;

    pstSsEvent->SsEvent  = TAF_SS_EVT_ERROR;

    pstSsEvent->ClientId = ClientId;
    pstSsEvent->OpId     = OpId;

    pstSsEvent->ErrorCode = ErrorCode;

    TAF_SsEventReport(pstSsEvent);

    PS_MEM_FREE(WUEPS_PID_SS, pstSsEvent);

    return;
}
VOS_VOID SSA_ReportUssdDataSndEvent(VOS_UINT8 ucTi)
{
    TAF_SS_CALL_INDEPENDENT_EVENT_STRU *pstSsEvent;

    pstSsEvent = (TAF_SS_CALL_INDEPENDENT_EVENT_STRU *)PS_MEM_ALLOC(WUEPS_PID_SS,
                                                                    sizeof(TAF_SS_CALL_INDEPENDENT_EVENT_STRU));
    if (VOS_NULL_PTR == pstSsEvent)
    {
        SSA_LOG(ERROR_PRINT, "SSA_ReportUssdDataSndEvent:ERROR: Alloc Mem Fail");
        SSA_TiFree(ucTi);
        return;
    }

    /*对事件上报的内容赋初值*/
    PS_MEM_SET(pstSsEvent, 0 , 4);
    pstSsEvent->SsEvent               =  TAF_SS_EVT_USSD_DATA_SND;
    pstSsEvent->OpId                  = gastSsaStatetable[ucTi].OpId;
    pstSsEvent->ClientId              = gastSsaStatetable[ucTi].ClientId;

    MN_SendClientEvent(pstSsEvent->ClientId,
                       MN_CALLBACK_SS,
                       pstSsEvent->SsEvent,
                       pstSsEvent);

    PS_MEM_FREE(WUEPS_PID_SS, pstSsEvent);

    return;

}


VOS_VOID SSA_GetUssdMode(VOS_VOID)
{
    TAF_USSD_NVIM_TRANS_MODE_STRU       stUssdTranMode;
    VOS_UINT32                          ulLength;

    ulLength = 0;


    PS_MEM_SET(&stUssdTranMode, 0x00, sizeof(stUssdTranMode));

    (VOS_VOID)NV_GetLength(en_NV_Item_CUST_USSD_MODE, &ulLength);

    /*读取USSD是否透传设置 */
    if (NV_OK == NV_Read(en_NV_Item_CUST_USSD_MODE,
                          &stUssdTranMode,
                          ulLength))
    {
        if (TAF_SSA_NVIM_ITEM_ACTIVE == stUssdTranMode.ucStatus)
        {
            g_usTafSsaUssdTransMode = stUssdTranMode.ucUssdTransMode;
        }
        else
        {
            g_usTafSsaUssdTransMode = AT_USSD_TRAN_MODE;
        }
    }
    else
    {
        g_usTafSsaUssdTransMode = AT_USSD_TRAN_MODE;
    }

    return;
}

/* 删除FDN检查函数的处理,移到SPM中 */


VOS_VOID Ss_TaskEntry_T( struct MsgCB* pMsg )
{
    MSG_HEADER_STRU         *pMsgHeader;
    REL_TIMER_MSG           *pTimer;                                            /* 定义TIMER消息指针                        */
    VOS_UINT16               usMsgType;
    VOS_UINT16               usClientId;
    VOS_UINT8                ucOpId;
    MN_APP_REQ_MSG_STRU     *pMnAppReqMsg;

    pMsgHeader = ( MSG_HEADER_STRU* )pMsg;                                      /* 得到消息头                               */
    pTimer     = ( REL_TIMER_MSG* ) pMsg;                                       /* 获取TIMER消息指针                        */
    pMnAppReqMsg = ( MN_APP_REQ_MSG_STRU* )pMsg;

    if (VOS_TRUE == TAF_SDC_GetSvlteSupportFlag())
    {
        /* svlte特性开启时，modem1可能收到usim0的消息，需要将消息中send pid进行转换 */
        pMsg->ulSenderPid = TAF_SDC_ConvertOamSendPid(pMsg->ulSenderPid);
    }

    if ( VOS_PID_TIMER == pMsg->ulSenderPid )
    {
        Ss_RcvTimerOut((VOS_UINT8)pTimer->ulPara , (VOS_UINT8)pTimer->ulName);      /* TIMER溢出消息分发函数                    */
    }
    else
    {
        switch( pMsgHeader->ulSenderPid )
        {                                                                   /* 根据Src Tsk Id分发消息                   */
            case WUEPS_PID_MM:                                                  /* MM 发来的消息                            */
                Ss_RcvMmMsg( pMsg );                                            /* 调用MM消息处理                           */
                break;

            case WUEPS_PID_ADMIN:
                break;

            /* Delete MAPS_STK_PID and WUEPS_PID_AT */
            case WUEPS_PID_TAF:
                usMsgType = pMnAppReqMsg->usMsgName;

                /*usClientId*/
                usClientId = pMnAppReqMsg->clientId;

                /*ucOpId*/
                ucOpId = pMnAppReqMsg->opId;

                Taf_SsaProc(usMsgType, usClientId, ucOpId, pMnAppReqMsg->aucContent);
                break;
            default :                                                           /* 其他TASK发来的消息                       */
                SS_WARN_LOG( "\n [Error]SenderPid Error! \r" );
                break;
        }
    }

}


/***********************************************************************
*  MODULE   : Ss_Init
*  FUNCTION : Ss 初始化处理
*  INPUT    : VOS_VOID
*  OUTPUT   : VOS_VOID
*  RETURN   : VOS_VOID
*  NOTE     : 无
*  HISTORY  :
  3.日    期   : 2015年9月18日
    作    者   : c00318887
    修改内容   : 新版生成
************************************************************************/

VOS_VOID Ss_Init( VOS_VOID )
{
    VOS_UINT8   i;                                                                  /* 用于循环                                 */

    for( i = 0 ; i < SS_MAX_ENTITY_NUM ; i++ )
    {                                                                           /* 初始化SS实体信息                         */
        g_SsEntity[i].ucState = SS_S_NULL;
        SS_NORM_LOG( "\n SS state = SS_S_NULL \r" );
        g_SsEntity[i].ucCr = SS_NONE_CR;
    }

    for( i = 0 ; i < SS_MO_ENTITY_NUM ; i++ )
    {                                                                           /* 初始化消息缓存BUFFER                     */
        g_SsSaveMsg[i].ulMsgLen = 0;
        if (VOS_NULL_PTR != g_SsSaveMsg[i].pucMsg)
        {
            PS_MEM_FREE(WUEPS_PID_SS, g_SsSaveMsg[i].pucMsg);
            g_SsSaveMsg[i].pucMsg = VOS_NULL_PTR;
        }
    }
}

VOS_VOID Ss_TaskInit( VOS_VOID )
{
    Ss_Init();
    

    SSA_GetUssdMode();
/*=>A32D07063*/
#if 0
    for( i = 0 ; i < SS_MAX_ENTITY_NUM ; i++ )
    {                                                                           /* 初始化TIMER管理信息                      */
        Ss_MemSet(&g_SsTimerMng[i].SsTimerId,0,sizeof(HTIMER));
        g_SsTimerMng[i].ucTimerState = SS_TIMER_IDLE;
    }
#endif
/*<=A32D07063*/
}

/******************************************************************************
 * 函数名称 ： WuepsSsPidInit
 * 功能描述 ： WUEPS SS PID的初始化函数
 * 参数说明 ： 无
 * 备    注 ：
 * 返 回 值 ： VOS_UINT32 初始化结果：
 *             0  : 成功
 *             非0: 失败
 *
 * 变更历史 ：
 *           No.  姓名      变更                                    日   期
 *           1    张志勇    新建                                   2005.02.24
 *****************************************************************************/
VOS_UINT32 WuepsSsPidInit ( enum VOS_INIT_PHASE_DEFINE ip )
{
    switch( ip )
    {
    case VOS_IP_LOAD_CONFIG:
        Ss_TaskInit();                                                      /* CC TASK初始化等相应处理                  */
        break;
    case VOS_IP_FARMALLOC:
    case VOS_IP_INITIAL:
    case VOS_IP_ENROLLMENT:
    case VOS_IP_LOAD_DATA:
    case VOS_IP_FETCH_DATA:
    case VOS_IP_STARTUP:
    case VOS_IP_RIVAL:
    case VOS_IP_KICKOFF:
    case VOS_IP_STANDBY:
    case VOS_IP_BROADCAST_STATE:
    case VOS_IP_RESTART:
    case VOS_IP_BUTT:
        break;
    default:
        break;
    }

    return VOS_OK;
}

/*lint -restore */


#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif


