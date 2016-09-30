

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "TafXCallProcApp.h"
#include "TafXCallSndXcc.h"
#include "TafXCallSndApp.h"
#include "TafXCallSndMma.h"
#include "TafAgentInterface.h"
#include "MnComm.h"
/* Added by f279542 for CDMA 1X Iteration 4, 2014-11-17, begin */
#include "TafLog.h"
/* Added by f279542 for CDMA 1X Iteration 4, 2014-11-17, end */

#include "TafSdcCtx.h"
#include "TafXCallProcXcc.h"

#include "TafCdmaPlusTranslate.h"

/* Added by y00322978 for CDMA Iteration 17, 2015-7-15, begin */
#include "TafMmaSndOm.h"
/* Added by y00322978 for CDMA Iteration 17, 2015-7-15, end */

/* Added by y00307564 for CDMA Iteration 16 2015-7-25 begin */
#include "TafStdlib.h"
/* Added by y00307564 for CDMA Iteration 16 2015-7-25 end */

#ifdef  __cplusplus
#if  __cplusplus
extern "C"{
#endif
#endif

#define THIS_FILE_ID                    PS_FILE_ID_TAF_X_CALL_PROC_APP_C

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

/*****************************************************************************
  2 全局变量定义
*****************************************************************************/

/*****************************************************************************
  3 函数定义
*****************************************************************************/
/*lint -save -e958*/

VOS_VOID TAF_XCALL_RcvAppCallOrigReq(VOS_VOID *pMsg)
{
    VOS_UINT32                          ulRet;
    VOS_UINT8                           ucCallId;
    MN_CALL_APP_REQ_MSG_STRU           *pstCallOrig = VOS_NULL_PTR;
    /* Added by h00313353 for Iteration 17, 2015-7-28, begin */
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enCsSrvStatus;

    enCsSrvStatus                       = TAF_SDC_GetCsServiceStatus();
    /* Added by h00313353 for Iteration 17, 2015-7-28, end */

    ucCallId                            = 0;
    pstCallOrig                         = (MN_CALL_APP_REQ_MSG_STRU *)pMsg;

    /* 判断当前是否可以发起呼叫 */
    ulRet = TAF_XCALL_IsAllowMoCallInCurrentStatus(pstCallOrig->unParm.stOrig.enCallType);

    /* 判断是否允许发起呼叫 */
    if (VOS_FALSE == ulRet)
    {
        /* 上报呼叫失败 */
        TAF_XCALL_SndCallOrigCnf(TAF_CS_CAUSE_NOT_ALLOW,
                                 pstCallOrig->ulSenderPid,
                                 pstCallOrig->clientId,
                                 pstCallOrig->unParm.stOrig.enCallType);

        return;
    }

    /* 分配call Id */
    ulRet = TAF_XCALL_AllocCallId(&ucCallId);
    if (VOS_FALSE == ulRet)
    {
        /* 上报呼叫失败 */
        TAF_XCALL_SndCallOrigCnf(TAF_CS_CAUSE_NO_CALL_ID,
                                 pstCallOrig->ulSenderPid,
                                 pstCallOrig->clientId,
                                 pstCallOrig->unParm.stOrig.enCallType);

        return;
    }

    /* 新建一个呼叫实体 */
    TAF_XCALL_CreateMoCallEntity(ucCallId, pstCallOrig);

    /* 设置SDC中CS呼叫标志 */
    TAF_SDC_SetCsCallExistFlg(VOS_TRUE);

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    /* TO DO:给MTC模块上报当前CS域业务状态 */
    /* TAF_SendMtcCsSrvInfoInd(); */
#endif

    /* 根据当前1X的服务状态判定是否需要发送呼叫请求给XCC,如果当前非正常服务,
       不发给XCC呼叫请求,重拨情况下直接通知XSD进行重拨;否则发给XCC */

    /* 通知MMA CALL NTF */
    TAF_SndMmaCdmaMoCallStartNtf(WUEPS_PID_TAF, pstCallOrig->clientId, TAF_XCALL_BuildMmaCallType(pstCallOrig->unParm.stOrig.enCallType));

    /* 回复呼叫成功 */
    TAF_XCALL_SndCallOrigCnf(TAF_CS_CAUSE_SUCCESS,
                             pstCallOrig->ulSenderPid,
                             pstCallOrig->clientId,
                             pstCallOrig->unParm.stOrig.enCallType);

    /* 上报呼叫事件 */
    TAF_XCALL_SndCallOrigInd(ucCallId);

    /* Modified by h00313353 for Iteration 17, 2015-7-28, begin */
    /* 启动silent redial定时器 */
    TAF_XCALL_StartTimer(TI_TAF_XCALL_WAIT_CALL_REDIAL_PERIOD, TAF_XCALL_GetRedialPeriodTimerLen(ucCallId), ucCallId);

    if (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE == enCsSrvStatus)
    {
        /* 发送呼叫消息给XCC */
        TAF_XCALL_SndXccOrigCallReq(ucCallId);

        return;
    }

    if (TAF_SDC_SERVICE_STATUS_LIMITED_SERVICE == enCsSrvStatus)
    {
        /* 紧急呼且Limit Service，直接发起呼叫 */
        if (MN_CALL_TYPE_EMERGENCY == TAF_XCALL_GetCallType(ucCallId))
        {
            /* 发送呼叫消息给XCC */
            TAF_XCALL_SndXccOrigCallReq(ucCallId);

            return;
        }

        TAF_XCALL_StartTimer(TI_TAF_XCALL_WAIT_CALL_REDAIL_INTERVAL, TI_TAF_XCALL_WAIT_CALL_REDAIL_INTERVAL_LEN, ucCallId);

        return;
    }

    if (MN_CALL_TYPE_EMERGENCY == TAF_XCALL_GetCallType(ucCallId))
    {
        TAF_SndMmaCdmaCallRedialSystemAcquireNtf(WUEPS_PID_TAF,
                                     pstCallOrig->clientId,
                                     TAF_XCALL_BuildMmaCallType(pstCallOrig->unParm.stOrig.enCallType),
                                     TAF_CS_CAUSE_XCALL_INTERNAL_NO_SERVICE);
    }

    TAF_XCALL_StartTimer(TI_TAF_XCALL_WAIT_CALL_REDAIL_INTERVAL, TI_TAF_XCALL_WAIT_CALL_REDAIL_INTERVAL_LEN, ucCallId);
    /* Modified by h00313353 for Iteration 17, 2015-7-28, end */

    return;
}
VOS_VOID TAF_XCALL_RcvAppSupsCmdReq(VOS_VOID *pMsg)
{
    MN_CALL_APP_REQ_MSG_STRU           *pstAppCallSupsReq = VOS_NULL_PTR;
    MN_CALL_SUPS_PARAM_STRU            *pstCallSupsParam = VOS_NULL_PTR;

    pstAppCallSupsReq = (MN_CALL_APP_REQ_MSG_STRU *)pMsg;
    pstCallSupsParam  = &(pstAppCallSupsReq->unParm.stCallMgmtCmd);

    /* 1、当前有一路处于active状态的通话:
        如果再收到APP的orig请求，直接回复失败，
        也不会收到XCC的incoming消息，
        只处理flash消息
        所以处于active状态的通话只有一路，而且不会有其它状态的通话存在

        2、暂时存在两路通话的场景:
        有一路处于dialing状态的通话，此时又收到XCC的incoming消息
     */

    switch (pstCallSupsParam->enCallSupsCmd)
    {
        /* CHLD=0, Releases all held calls or sets User Determined User Busy (UDUB) for a waiting call */
        case MN_CALL_SUPS_CMD_REL_HELD_OR_UDUB:
            TAF_XCALL_ProcCallSupsCmdRelHoldOrUdubReq(pstAppCallSupsReq);
            break;

        /* CHLD=1, Releases all active calls (if any exist) and accepts the other (held or waiting) call */
        case MN_CALL_SUPS_CMD_REL_ACT_ACPT_OTH:
            TAF_XCALL_ProcCallSupsCmdRelActAcptOthReq(pstAppCallSupsReq);
            break;

        /* CHLD=1x, Releases a specific active call X*/
        case MN_CALL_SUPS_CMD_REL_CALL_X:
            TAF_XCALL_ProcCallSupsCmdRelCallXReq(pstAppCallSupsReq);
            break;

        /* ATH, Release all calls */
        case MN_CALL_SUPS_CMD_REL_ALL_CALL:
            TAF_XCALL_ProcCallSupsCmdRelAllCallReq(pstAppCallSupsReq);
            break;

        /* CHLD=2, Places all active calls (if any exist) on hold and accepts the other (held or waiting) call */
        case MN_CALL_SUPS_CMD_HOLD_ACT_ACPT_OTH:
            TAF_XCALL_ProcCallSupsCmdHoldActAcptOthReq(pstAppCallSupsReq);
            break;

        default:
            /* 其它情况，直接回复失败 */
            TAF_XCALL_SndSupsCmdCnf(pstAppCallSupsReq->ulSenderPid,
                                    pstAppCallSupsReq->clientId,
                                    pstAppCallSupsReq->opId,
                                    pstCallSupsParam->callId,
                                    TAF_CS_CAUSE_NOT_ALLOW);
            break;
    }

    return;
}


VOS_VOID TAF_XCALL_RcvAppSendFlashReq(VOS_VOID *pMsg)
{
    VOS_UINT32                          ulNumOfCalls;
    VOS_UINT8                           aucCallIds[TAF_XCALL_CALL_ENTITY_MAX_NUM];
    TAF_CALL_APP_SEND_FLASH_REQ_STRU   *pstFlashReqMsg;
    /* Added by y00307564 for CDMA Iteration 16 2015-7-23 begin */
    VOS_UINT8                           ucDstDigitNum;
    VOS_UINT8                           aucDstDigit[TAF_CALL_MAX_FLASH_DIGIT_LEN + 1];
    /* Added by y00307564 for CDMA Iteration 16 2015-7-23 end */

    /* Added by y00307564 for CDMA Iteration 16 2015-8-11 begin */
    TAF_XCALL_CALL_ENTITY_STRU         *pstCallEntity = VOS_NULL_PTR;
    /* Added by y00307564 for CDMA Iteration 16 2015-8-11 end */

    pstFlashReqMsg = (TAF_CALL_APP_SEND_FLASH_REQ_STRU *)pMsg;

    /* 根据状态查找处于ACTIVE状态的call id，目前只支持在ACTIVE状态下发送Flash */
    TAF_XCALL_GetCallsByState(MN_CALL_S_ACTIVE, &ulNumOfCalls, aucCallIds);

    /* 处于Active状态的实例至多只能有1个 */
    if (1 != ulNumOfCalls)
    {
        TAF_XCALL_SndFlashRslt(pstFlashReqMsg->usClientId, pstFlashReqMsg->ucOpId, VOS_ERR);
        return;
    }

    /* Modified by y00307564 for CDMA Iteration 16 2015-7-23 begin */
    ucDstDigitNum = 0;
    PS_MEM_SET(aucDstDigit, 0x00, sizeof(aucDstDigit));

    TAF_XCALL_ReplaceFlashPlusWithDigit(pstFlashReqMsg->stFlashPara.ucDigitNum,
                                        pstFlashReqMsg->stFlashPara.aucDigit,
                                        &ucDstDigitNum,
                                        aucDstDigit);

    /* 向XCC发送ID_XCALL_XCC_SEND_FLASH_REQ */
    if (VOS_OK != TAF_XCALL_SndXccFlashReq(aucCallIds[0],
                                           ucDstDigitNum,
                                           aucDstDigit))
    {
        TAF_XCALL_SndFlashRslt(pstFlashReqMsg->usClientId, pstFlashReqMsg->ucOpId, VOS_ERR);
        return;
    }
    /* Modified by y00307564 for CDMA Iteration 16 2015-7-23 end */

    pstCallEntity = TAF_XCALL_GetCallEntityAddr(aucCallIds[0]);
    pstCallEntity->stFlashClientInfo.ulModuleId = pstFlashReqMsg->ulSenderPid;
    pstCallEntity->stFlashClientInfo.usClientId = pstFlashReqMsg->usClientId;
    pstCallEntity->stFlashClientInfo.ucOpId     = pstFlashReqMsg->ucOpId;

    return;
}
VOS_VOID TAF_XCALL_ProcCallSupsCmdRelHoldOrUdubReq(
    MN_CALL_APP_REQ_MSG_STRU           *pstAppCallSupsReq
)
{
    VOS_UINT32                          ulNumOfCalls;
    VOS_UINT8                           aucCallIds[TAF_XCALL_CALL_ENTITY_MAX_NUM];
    MN_CALL_SUPS_PARAM_STRU            *pstCallSupsParam = VOS_NULL_PTR;

    pstCallSupsParam  = &(pstAppCallSupsReq->unParm.stCallMgmtCmd);

    ulNumOfCalls = 0;

    PS_MEM_SET(aucCallIds, 0, sizeof(aucCallIds));

    TAF_XCALL_GetCallsByState(MN_CALL_S_INCOMING, &ulNumOfCalls, aucCallIds);

    /* 挂断Incoming状态的呼叫，Incoming状态的呼叫只可能一个 */
    if (1 == ulNumOfCalls)
    {
        /* 挂断该呼叫 */
        TAF_XCALL_SndXccHangUpCallReq(aucCallIds[0], TAF_XCC_END_REASON_NORMAL_RELEASE);

        TAF_XCALL_RegSsKeyEvent(aucCallIds[0], TAF_XCALL_SS_PROG_EVT_REL, TAF_XCALL_SS_SUBSEQ_OP_RPT_SUCC);

        /* 更新client info */
        TAF_XCALL_UpdateClientInfo(aucCallIds[0],
                                   pstAppCallSupsReq->ulSenderPid,
                                   pstAppCallSupsReq->clientId,
                                   pstAppCallSupsReq->opId);

        /* 记录主动挂断的原因值 */
        TAF_XCALL_UpdateCcCause(aucCallIds[0], TAF_CS_CAUSE_XCALL_NORMAL_RELEASE);

        /* 记录呼叫挂断的方向 */
        TAF_XCALL_UpdateDiscCallDir(aucCallIds[0], VOS_TRUE);

        /* 回复请求结果 */
        TAF_XCALL_SndSupsCmdCnf(pstAppCallSupsReq->ulSenderPid,
                                pstAppCallSupsReq->clientId,
                                pstAppCallSupsReq->opId,
                                aucCallIds[0], TAF_CS_CAUSE_SUCCESS);
    }
    else
    {
        /* 回复失败 */
        TAF_XCALL_SndSupsCmdCnf(pstAppCallSupsReq->ulSenderPid,
                                pstAppCallSupsReq->clientId,
                                pstAppCallSupsReq->opId,
                                pstCallSupsParam->callId, TAF_CS_CAUSE_NOT_ALLOW);
    }

    return;
}
VOS_VOID TAF_XCALL_ProcCallSupsCmdRelActAcptOthReq(
    MN_CALL_APP_REQ_MSG_STRU           *pstAppCallSupsReq
)
{
    /* active call和incoming call不可能同时存在，所以或者挂断active call或者接听incoming call */
    VOS_UINT32                          ulNumOfCalls;
    VOS_UINT8                           aucCallIds[TAF_XCALL_CALL_ENTITY_MAX_NUM];
    MN_CALL_SUPS_PARAM_STRU            *pstCallSupsParam = VOS_NULL_PTR;
    VOS_UINT8                           i;

    pstCallSupsParam  = &(pstAppCallSupsReq->unParm.stCallMgmtCmd);

    ulNumOfCalls = 0;

    PS_MEM_SET(aucCallIds, 0, sizeof(aucCallIds));

    TAF_XCALL_GetCallsByState(MN_CALL_S_ACTIVE, &ulNumOfCalls, aucCallIds);

    if (0 != ulNumOfCalls)
    {
        /* 目前只可能有一个active状态的呼叫 */
        for (i = 0; i < TAF_XCALL_MIN(ulNumOfCalls, TAF_XCALL_CALL_ENTITY_MAX_NUM); i++)
        {
            /* 挂断该呼叫 */
            TAF_XCALL_SndXccHangUpCallReq(aucCallIds[i], TAF_XCC_END_REASON_NORMAL_RELEASE);

            TAF_XCALL_DeRegSsKeyEvent(aucCallIds[i], TAF_XCALL_SS_PROG_EVT_REL);

            /* 更新client info */
            TAF_XCALL_UpdateClientInfo(aucCallIds[i],
                                       pstAppCallSupsReq->ulSenderPid,
                                       pstAppCallSupsReq->clientId,
                                       pstAppCallSupsReq->opId);

            /* 记录主动挂断的原因值 */
            TAF_XCALL_UpdateCcCause(aucCallIds[i], TAF_CS_CAUSE_XCALL_NORMAL_RELEASE);

            /* 记录呼叫挂断的方向 */
            TAF_XCALL_UpdateDiscCallDir(aucCallIds[i], VOS_TRUE);

        }

        TAF_XCALL_RegSsKeyEvent(aucCallIds[ulNumOfCalls - 1], TAF_XCALL_SS_PROG_EVT_REL, TAF_XCALL_SS_SUBSEQ_OP_RPT_SUCC);

        /* 回复请求结果 */
        TAF_XCALL_SndSupsCmdCnf(pstAppCallSupsReq->ulSenderPid,
                                pstAppCallSupsReq->clientId,
                                pstAppCallSupsReq->opId,
                                pstCallSupsParam->callId, TAF_CS_CAUSE_SUCCESS);

        return;
    }

    ulNumOfCalls = 0;

    PS_MEM_SET(aucCallIds, 0, sizeof(aucCallIds));

    TAF_XCALL_GetCallsByState(MN_CALL_S_INCOMING, &ulNumOfCalls, aucCallIds);

    /* Incoming状态的呼叫只可能一个 */
    if (1 == ulNumOfCalls)
    {
        /* 接听该呼叫 */
        TAF_XCALL_SndXccAnswerCallReq(aucCallIds[0]);

        TAF_XCALL_RegSsKeyEvent(aucCallIds[0], TAF_XCALL_SS_PROG_EVT_ANS, TAF_XCALL_SS_SUBSEQ_OP_RPT_SUCC);

        /* 更新client info */
        TAF_XCALL_UpdateClientInfo(aucCallIds[0],
                                   pstAppCallSupsReq->ulSenderPid,
                                   pstAppCallSupsReq->clientId,
                                   pstAppCallSupsReq->opId);

        /* 回复请求结果 */
        TAF_XCALL_SndSupsCmdCnf(pstAppCallSupsReq->ulSenderPid,
                                pstAppCallSupsReq->clientId,
                                pstAppCallSupsReq->opId,
                                aucCallIds[0], TAF_CS_CAUSE_SUCCESS);

        return;
    }

    TAF_XCALL_SndSupsCmdCnf(pstAppCallSupsReq->ulSenderPid,
                            pstAppCallSupsReq->clientId,
                            pstAppCallSupsReq->opId,
                            pstCallSupsParam->callId, TAF_CS_CAUSE_NOT_ALLOW);

    return;
}
VOS_VOID TAF_XCALL_ProcCallSupsCmdRelCallXReq(
    MN_CALL_APP_REQ_MSG_STRU           *pstAppCallSupsReq
)
{
    MN_CALL_SUPS_PARAM_STRU            *pstCallSupsParam = VOS_NULL_PTR;
    MN_CALL_STATE_ENUM_U8               enCallState;

    MN_CALL_TYPE_ENUM_U8                enCallType;
    VOS_UINT16                          usClientId;

    pstCallSupsParam  = &(pstAppCallSupsReq->unParm.stCallMgmtCmd);

    /* 判断Call Id的有效性 */
    if (VOS_TRUE != TAF_XCALL_IsAvailCallId(pstCallSupsParam->callId))
    {
        TAF_XCALL_SndSupsCmdCnf(pstAppCallSupsReq->ulSenderPid,
                                pstAppCallSupsReq->clientId,
                                pstAppCallSupsReq->opId,
                                pstCallSupsParam->callId, TAF_CS_CAUSE_NOT_ALLOW);
        return;
    }

    enCallState = TAF_XCALL_GetCallState(pstCallSupsParam->callId);

    if (MN_CALL_S_IDLE == enCallState)
    {
        TAF_XCALL_SndSupsCmdCnf(pstAppCallSupsReq->ulSenderPid,
                                pstAppCallSupsReq->clientId,
                                pstAppCallSupsReq->opId,
                                pstCallSupsParam->callId, TAF_CS_CAUSE_NOT_ALLOW);

        return;
    }

    if (TAF_XCALL_TIMER_STATUS_RUNING == TAF_XCALL_GetTimerStatus(TI_TAF_XCALL_WAIT_CALL_REDAIL_INTERVAL, pstCallSupsParam->callId))
    {
        /* 停止定时器 */
        TAF_XCALL_StopTimer(TI_TAF_XCALL_WAIT_CALL_REDIAL_PERIOD, pstCallSupsParam->callId);
        TAF_XCALL_StopTimer(TI_TAF_XCALL_WAIT_CALL_REDAIL_INTERVAL, pstCallSupsParam->callId);

        /* 通知呼叫终止 */
        enCallType = TAF_XCALL_GetCallType(pstCallSupsParam->callId);
        usClientId = TAF_XCALL_GetClientInfo(pstCallSupsParam->callId)->usClientId;
        TAF_SndMmaCdmaMoCallEndNtf(WUEPS_PID_TAF, usClientId, TAF_XCALL_BuildMmaCallType(enCallType));

        /* 记录主动挂断的原因值 */
        TAF_XCALL_UpdateCcCause(pstCallSupsParam->callId, TAF_CS_CAUSE_XCALL_NORMAL_RELEASE);

        /* 记录呼叫挂断的方向 */
        TAF_XCALL_UpdateDiscCallDir(pstCallSupsParam->callId, VOS_TRUE);

        /* 回复请求结果 */
        TAF_XCALL_SndSupsCmdCnf(pstAppCallSupsReq->ulSenderPid,
                                pstAppCallSupsReq->clientId,
                                pstAppCallSupsReq->opId,
                                pstCallSupsParam->callId, TAF_CS_CAUSE_SUCCESS);

        /* 给APP上报MN_CALL_EVT_RELEASED事件 */
        TAF_XCALL_SndCallReleaseInd(pstCallSupsParam->callId);

        /* 清空对应的实体信息 */
        TAF_XCALL_FreeCallEntity(pstCallSupsParam->callId);

        if (VOS_FALSE == TAF_XCALL_ExistCallInstance())
        {
            /* 清除SDC中CS呼叫标志 */
            TAF_SDC_SetCsCallExistFlg(VOS_FALSE);
        }
       
        return;

    }

    /* 停止重拨定时器,如果定时器未启动或call id不一致则跳过 */
    TAF_XCALL_StopTimer(TI_TAF_XCALL_WAIT_CALL_REDIAL_PERIOD, pstCallSupsParam->callId);
    TAF_XCALL_StopTimer(TI_TAF_XCALL_WAIT_CALL_REDAIL_INTERVAL, pstCallSupsParam->callId);

    /* 挂断该呼叫 */
    TAF_XCALL_SndXccHangUpCallReq(pstCallSupsParam->callId, TAF_XCC_END_REASON_NORMAL_RELEASE);

    TAF_XCALL_RegSsKeyEvent(pstCallSupsParam->callId, TAF_XCALL_SS_PROG_EVT_REL, TAF_XCALL_SS_SUBSEQ_OP_RPT_SUCC);

    /* 更新client info */
    TAF_XCALL_UpdateClientInfo(pstCallSupsParam->callId,
                               pstAppCallSupsReq->ulSenderPid,
                               pstAppCallSupsReq->clientId,
                               pstAppCallSupsReq->opId);

    /* 记录主动挂断的原因值 */
    TAF_XCALL_UpdateCcCause(pstCallSupsParam->callId, TAF_CS_CAUSE_XCALL_NORMAL_RELEASE);

    /* 记录呼叫挂断的方向 */
    TAF_XCALL_UpdateDiscCallDir(pstCallSupsParam->callId, VOS_TRUE);

    /* 回复请求结果 */
    TAF_XCALL_SndSupsCmdCnf(pstAppCallSupsReq->ulSenderPid,
                            pstAppCallSupsReq->clientId,
                            pstAppCallSupsReq->opId,
                            pstCallSupsParam->callId, TAF_CS_CAUSE_SUCCESS);

    return;
}
VOS_VOID TAF_XCALL_ProcCallSupsCmdRelAllCallReq(
    MN_CALL_APP_REQ_MSG_STRU           *pstAppCallSupsReq
)
{
    /* 暂时存在两路通话的场景:
        有一路处于dialing状态的通话，此时又收到XCC的incoming消息
     */
    VOS_UINT32                          ulNumOfCalls;
    VOS_UINT8                           aucCallIds[TAF_XCALL_CALL_ENTITY_MAX_NUM];
    MN_CALL_SUPS_PARAM_STRU            *pstCallSupsParam = VOS_NULL_PTR;
    VOS_UINT8                           i;

    MN_CALL_TYPE_ENUM_U8                enCallType;
    VOS_UINT16                          usClientId;
    VOS_UINT32                          ulModuleId;

    pstCallSupsParam  = &(pstAppCallSupsReq->unParm.stCallMgmtCmd);

    ulNumOfCalls = 0;

    PS_MEM_SET(aucCallIds, 0, sizeof(aucCallIds));

    TAF_XCALL_GetNotIdleStateCalls(&ulNumOfCalls, aucCallIds);

    if (0 == ulNumOfCalls)
    {
        TAF_XCALL_SndSupsCmdCnf(pstAppCallSupsReq->ulSenderPid,
                                pstAppCallSupsReq->clientId,
                                pstAppCallSupsReq->opId,
                                pstCallSupsParam->callId, TAF_CS_CAUSE_SUCCESS);

        return;
    }

    ulModuleId = pstAppCallSupsReq->ulSenderPid;

    /* 循环挂断所有非idle的呼叫 */
    for (i = 0; i < TAF_XCALL_MIN(ulNumOfCalls, TAF_XCALL_CALL_ENTITY_MAX_NUM); i++)
    {
        /* 停止重拨时长定时器 */
        TAF_XCALL_StopTimer(TI_TAF_XCALL_WAIT_CALL_REDIAL_PERIOD, aucCallIds[i]);

        if (TAF_XCALL_TIMER_STATUS_RUNING == TAF_XCALL_GetTimerStatus(TI_TAF_XCALL_WAIT_CALL_REDAIL_INTERVAL, aucCallIds[i]))
        {
            /* 停止重拨间隔定时器 */
            TAF_XCALL_StopTimer(TI_TAF_XCALL_WAIT_CALL_REDAIL_INTERVAL, aucCallIds[i]);

            /* 通知呼叫终止 */
            enCallType = TAF_XCALL_GetCallType(aucCallIds[i]);
            usClientId = TAF_XCALL_GetClientInfo(aucCallIds[i])->usClientId;
            TAF_SndMmaCdmaMoCallEndNtf(WUEPS_PID_TAF, usClientId, TAF_XCALL_BuildMmaCallType(enCallType));

            /* 记录主动挂断的原因值 */
            TAF_XCALL_UpdateCcCause(aucCallIds[i], TAF_CS_CAUSE_XCALL_NORMAL_RELEASE);

            /* 给APP上报MN_CALL_EVT_RELEASED事件 */
            TAF_XCALL_SndCallReleaseInd(aucCallIds[i]);

            /* 清空对应的实体信息 */
            TAF_XCALL_FreeCallEntity(aucCallIds[i]);

            /* 记录ulModuleId*/
            ulModuleId = TAF_XCALL_GetClientInfo(aucCallIds[i])->ulModuleId;
        }
        else
        {
            /* 挂断该呼叫 */
            TAF_XCALL_SndXccHangUpCallReq(aucCallIds[i], TAF_XCC_END_REASON_NORMAL_RELEASE);

            TAF_XCALL_DeRegSsKeyEvent(aucCallIds[i], TAF_XCALL_SS_PROG_EVT_REL);

            /* 更新client info */
            TAF_XCALL_UpdateClientInfo(aucCallIds[i],
                                       pstAppCallSupsReq->ulSenderPid,
                                       pstAppCallSupsReq->clientId,
                                       pstAppCallSupsReq->opId);

            /* 记录主动挂断的原因值 */
            TAF_XCALL_UpdateCcCause(aucCallIds[i], TAF_CS_CAUSE_XCALL_NORMAL_RELEASE);

            /* 记录呼叫挂断的方向 */
            TAF_XCALL_UpdateDiscCallDir(aucCallIds[i], VOS_TRUE);
        }
    }

    TAF_XCALL_GetNotIdleStateCalls(&ulNumOfCalls, aucCallIds);

    if (0 == ulNumOfCalls)
    {
        if (VOS_FALSE == TAF_XCALL_ExistCallInstance())
        {
            /* 清除SDC中CS呼叫标志 */
            TAF_SDC_SetCsCallExistFlg(VOS_FALSE);
        }

        /* 给APP上报MN_CALL_EVT_ALL_RELEASED事件 */
        TAF_XCALL_SndCallAllReleaseInd(ulModuleId);
    }
    else
    {

        TAF_XCALL_RegSsKeyEvent(aucCallIds[ulNumOfCalls - 1], TAF_XCALL_SS_PROG_EVT_REL, TAF_XCALL_SS_SUBSEQ_OP_RPT_SUCC);
    }


    /* 回复请求结果 */
    TAF_XCALL_SndSupsCmdCnf(pstAppCallSupsReq->ulSenderPid,
                            pstAppCallSupsReq->clientId,
                            pstAppCallSupsReq->opId,
                            pstCallSupsParam->callId, TAF_CS_CAUSE_SUCCESS);

    return;
}
VOS_VOID TAF_XCALL_ProcCallSupsCmdHoldActAcptOthReq(
    MN_CALL_APP_REQ_MSG_STRU           *pstAppCallSupsReq
)
{
    VOS_UINT32                          ulNumOfCalls;
    VOS_UINT8                           aucCallIds[TAF_XCALL_CALL_ENTITY_MAX_NUM];
    MN_CALL_SUPS_PARAM_STRU            *pstCallSupsParam = VOS_NULL_PTR;

    pstCallSupsParam  = &(pstAppCallSupsReq->unParm.stCallMgmtCmd);

    /* 处于active状态的通话只有一路，而且不会有其它状态的通话存在

        存在两路通话的场景:
        有一路处于dialing状态的通话，此时又收到XCC的incoming消息 */

    /* 3GPP2中hold操作都是通过FLASH消息，所以收到该请求只判断当前是否有incoming call */

    /* 如果没有incoming call，有一个active的，并且bypass为true，回复成功 */

    ulNumOfCalls = 0;

    PS_MEM_SET(aucCallIds, 0, sizeof(aucCallIds));

    TAF_XCALL_GetCallsByState(MN_CALL_S_ACTIVE, &ulNumOfCalls, aucCallIds);

    if (1 == ulNumOfCalls)
    {
        TAF_XCALL_SndSupsCmdCnf(pstAppCallSupsReq->ulSenderPid,
                                pstAppCallSupsReq->clientId,
                                pstAppCallSupsReq->opId,
                                aucCallIds[0], TAF_CS_CAUSE_SUCCESS);

        /* 给APP回复MN_CALL_EVT_SS_CMD_RSLT事件 */
        TAF_XCALL_SndSupsResult(pstAppCallSupsReq->ulSenderPid,
                                pstAppCallSupsReq->clientId,
                                pstAppCallSupsReq->opId,
                                aucCallIds[0], MN_CALL_SS_RES_SUCCESS);

        /* 不需要给APP上报MN_CALL_EVT_CONNECT事件，在收到XCC的connect ind时已上报 */

        return;
    }

    ulNumOfCalls = 0;

    PS_MEM_SET(aucCallIds, 0, sizeof(aucCallIds));

    TAF_XCALL_GetCallsByState(MN_CALL_S_INCOMING, &ulNumOfCalls, aucCallIds);

    /* Incoming状态的呼叫只可能一个 */
    if (1 == ulNumOfCalls)
    {
        /* 接听该呼叫 */
        TAF_XCALL_SndXccAnswerCallReq(aucCallIds[0]);

        TAF_XCALL_RegSsKeyEvent(aucCallIds[0], TAF_XCALL_SS_PROG_EVT_ANS, TAF_XCALL_SS_SUBSEQ_OP_RPT_SUCC);

        /* 更新client info */
        TAF_XCALL_UpdateClientInfo(aucCallIds[0],
                                   pstAppCallSupsReq->ulSenderPid,
                                   pstAppCallSupsReq->clientId,
                                   pstAppCallSupsReq->opId);

        /* 回复请求结果 */
        TAF_XCALL_SndSupsCmdCnf(pstAppCallSupsReq->ulSenderPid,
                                pstAppCallSupsReq->clientId,
                                pstAppCallSupsReq->opId,
                                aucCallIds[0], TAF_CS_CAUSE_SUCCESS);

        return;
    }

    TAF_XCALL_SndSupsCmdCnf(pstAppCallSupsReq->ulSenderPid,
                            pstAppCallSupsReq->clientId,
                            pstAppCallSupsReq->opId,
                            pstCallSupsParam->callId, TAF_CS_CAUSE_NOT_ALLOW);

    return;
}

/* Added by f279542 for CDMA 1X Iteration 4, 2014-11-10, begin */

VOS_VOID TAF_XCALL_RcvAppSendBurstDTMFReq(VOS_VOID *pMsg)
{
    TAF_CALL_BURST_DTMF_REQ_MSG_STRU                       *pstBurstDTMFReqMsg;
    TAF_CALL_SEND_BURST_DTMF_CNF_RESULT_ENUM_U8             enResult;

    pstBurstDTMFReqMsg = (TAF_CALL_BURST_DTMF_REQ_MSG_STRU *)pMsg;

    /* 判断该call_id对应的状态是否为ACTIVE,目前只支持在该状态下发送Burst DTMF  */
    if(MN_CALL_S_ACTIVE != TAF_XCALL_GetCallState(pstBurstDTMFReqMsg->stBurstDTMFPara.ucCallId))
    {
        /* send TAF_CALL_EVT_SEND_BURST_DTMF_CNF */
        TAF_XCALL_SndBurstDTMFCnf(pstBurstDTMFReqMsg->usClientId,
                                  pstBurstDTMFReqMsg->ucOpId,
                                  TAF_CALL_SEND_BURST_DTMF_CNF_RESULT_ABNORMAL_STATE,
                                  pstBurstDTMFReqMsg->stBurstDTMFPara.ucCallId);
        return;
    }

    /*如果等待XCC回复的Burst DTMF CNF消息的标识为VOS_FALSE，XCALL可以给XCC发送ID_XCALL_XCC_BURST_DTMF_REQ消息
      如果等待XCC回复的Burst DTMF CNF消息的标识为VOS_TRUE,XCALL需要等待发送中的ID_XCALL_XCC_BURST_DTMF_REQ的回复消息，
      同时把最新收到的TAF_CALL_SEND_BURST_DTMF_REQ消息缓存起来
    */
    if(TAF_XCALL_WAIT_NO_XCC_CNF == TAF_XCALL_GetWaitForXCCDTMFCnfFlag(pstBurstDTMFReqMsg->stBurstDTMFPara.ucCallId))
    {
        /* 如果此时状态为:收到Start Cont Dtmf Req的响应，但是定时器处于运行态，此时收到消息，应该缓存
            等待定时器超时，再处理
        */
        if (TAF_XCALL_TIMER_STATUS_RUNING
            == TAF_XCALL_GetTimerStatus(TI_TAF_XCALL_START_AND_STOP_CONT_DTMF_INTERVAL, pstBurstDTMFReqMsg->stBurstDTMFPara.ucCallId))
        {
            enResult = TAF_XCALL_SaveBurstDtmfInfo(pstBurstDTMFReqMsg);
        }
        else
        {
            enResult = TAF_XCALL_SndXccBurstDTMFReq(&pstBurstDTMFReqMsg->stBurstDTMFPara);

            TAF_XCALL_SetWaitForXCCDTMFCnfFlag(pstBurstDTMFReqMsg->stBurstDTMFPara.ucCallId, TAF_XCALL_WAIT_XCC_CNF_BURST_DTMF);
        }

    }
    else
    {
        enResult = TAF_XCALL_SaveBurstDtmfInfo(pstBurstDTMFReqMsg);
    }

    /* 回复XCALL TAF_CALL_EVT_SEND_BURST_DTMF_CNF (ucResult)*/
    TAF_XCALL_SndBurstDTMFCnf(pstBurstDTMFReqMsg->usClientId,
                              pstBurstDTMFReqMsg->ucOpId,
                              enResult,
                              pstBurstDTMFReqMsg->stBurstDTMFPara.ucCallId);

    return;
}

/* Deleted by l00324781 for CDMA Iteration 16, 2015-7-29, begin */
/* Buffer操作函数从XCallProcApp挪到XCallCtx.c中 */
/* Deleted by l00324781 for CDMA Iteration 16, 2015-7-29, end */


/* Added by y00314741 for  CDMA Iteration 17, 2015-7-14, begin */
/*****************************************************************************
 函 数 名  : TAF_XCALL_GetRedialPeriodTimerLen
 功能描述  : 获取重拨定时器时长
 输入参数  : VOS_UINT8    ucCallIds
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年07月14日
    作    者   : y00314741
    修改内容   : 新生成函数
*****************************************************************************/
VOS_UINT32 TAF_XCALL_GetRedialPeriodTimerLen(
    VOS_UINT8                           ucCallId
)
{
    if (MN_CALL_TYPE_EMERGENCY == TAF_XCALL_GetCallType(ucCallId))
    {
        return TAF_XCALL_GetConfigInfo()->ulEmcRedialPeriod;
    }
    else
    {
        return TI_TAF_XCALL_WAIT_CALL_REDIAL_PERIOD_LEN;
    }
}
/* Added by y00314741 for  CDMA Iteration 17, 2015-7-14, end */

/* Deleted by y00307564 for CDMA Iteration 16 2015-8-11 begin */
/* 删除无效的函数 */
/* Deleted by y00307564 for CDMA Iteration 16 2015-8-11 end */
/* Added by f279542 for CDMA 1X Iteration 4, 2014-11-10, end */

VOS_VOID TAF_XCALL_RcvAgentGetCallInfoReq(VOS_VOID *pMsg)
{
    TAFAGERNT_MN_GET_CALL_INFO_CNF_STRU                    *pstCnf;
    VOS_UINT8                                               ucCallId;
    TAF_XCALL_CALL_ENTITY_STRU                             *pstXcallEntity = VOS_NULL_PTR;

    pstCnf = (TAFAGERNT_MN_GET_CALL_INFO_CNF_STRU *)PS_ALLOC_MSG(WUEPS_PID_TAF,
                                                              sizeof(TAFAGERNT_MN_GET_CALL_INFO_CNF_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstCnf)
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF,"TAF_XCALL_RcvAppGetCallInfoReq:alloc msg failed.");
        return;
    }

    PS_MEM_SET(pstCnf, 0x00, sizeof(TAFAGERNT_MN_GET_CALL_INFO_CNF_STRU));


    for (ucCallId = 1; ucCallId <= TAF_XCALL_CALL_ENTITY_MAX_NUM; ucCallId++)
    {
        pstXcallEntity = TAF_XCALL_GetCallEntityAddr(ucCallId);
        if ((VOS_TRUE == pstXcallEntity->ulUsed)
         && (MN_CALL_S_IDLE != pstXcallEntity->enCallState))

        {
            pstCnf->stCallInfo[pstCnf->ucNumOfCalls].callId       = ucCallId;
            pstCnf->stCallInfo[pstCnf->ucNumOfCalls].enCallType   = pstXcallEntity->enCallType;
            pstCnf->stCallInfo[pstCnf->ucNumOfCalls].enCallState  = pstXcallEntity->enCallState;

            (pstCnf->ucNumOfCalls)++;
        }
    }

    pstCnf->ulSenderCpuId                 = VOS_LOCAL_CPUID;
    pstCnf->ulSenderPid                   = WUEPS_PID_TAF;
    pstCnf->ulReceiverCpuId               = VOS_LOCAL_CPUID;
    pstCnf->ulReceiverPid                 = ACPU_PID_TAFAGENT;
    pstCnf->ulLength                      = sizeof(TAFAGERNT_MN_GET_CALL_INFO_CNF_STRU) - VOS_MSG_HEAD_LENGTH;
    pstCnf->enMsgId                       = ID_TAFAGENT_MN_GET_CALL_INFO_CNF;

    PS_SEND_MSG(WUEPS_PID_TAF, pstCnf);

    return;

}


VOS_VOID TAF_XCALL_RcvAppGetCallInfoReq(VOS_VOID *pMsg)
{
    MN_CALL_INFO_QRY_CNF_STRU          *pstAppCnfMsg   = VOS_NULL_PTR;
    TAF_XCALL_CALL_ENTITY_STRU         *pstXcallEntity = VOS_NULL_PTR;
    MN_CALL_APP_REQ_MSG_STRU           *pstCallMsg = VOS_NULL_PTR;
    VOS_UINT16                          usLength;
    VOS_UINT8                           ucCallId;


    pstCallMsg = (MN_CALL_APP_REQ_MSG_STRU *)pMsg;

    /* 初始化 */
    usLength     = 0;

    pstAppCnfMsg = (MN_CALL_INFO_QRY_CNF_STRU *)PS_MEM_ALLOC(WUEPS_PID_TAF,
                                                    sizeof(MN_CALL_INFO_QRY_CNF_STRU));

    if (VOS_NULL_PTR == pstAppCnfMsg)
    {
        return;
    }
    /* 获取当前所有状态不为IDLE的呼叫信息 */
    /*遍历所有呼叫控制实体*/
    VOS_MemSet(pstAppCnfMsg, 0x00, sizeof(MN_CALL_INFO_QRY_CNF_STRU));

    for (ucCallId = 1; ucCallId <= TAF_XCALL_CALL_ENTITY_MAX_NUM; ucCallId++)
    {
        pstXcallEntity = TAF_XCALL_GetCallEntityAddr(ucCallId);
        if ((VOS_TRUE == pstXcallEntity->ulUsed)
         && (MN_CALL_S_IDLE != pstXcallEntity->enCallState))

        {
            pstAppCnfMsg->astCallInfos[pstAppCnfMsg->ucNumOfCalls].callId      = ucCallId;
            pstAppCnfMsg->astCallInfos[pstAppCnfMsg->ucNumOfCalls].enCallDir   = pstXcallEntity->enCallDir;
            pstAppCnfMsg->astCallInfos[pstAppCnfMsg->ucNumOfCalls].enCallState = pstXcallEntity->enCallState;
            pstAppCnfMsg->astCallInfos[pstAppCnfMsg->ucNumOfCalls].enMptyState = MN_CALL_NOT_IN_MPTY;
            pstAppCnfMsg->astCallInfos[pstAppCnfMsg->ucNumOfCalls].enCallType  = pstXcallEntity->enCallType;
            pstAppCnfMsg->astCallInfos[pstAppCnfMsg->ucNumOfCalls].enVoiceDomain = TAF_CALL_VOICE_DOMAIN_3GPP2;

            PS_MEM_CPY(&(pstAppCnfMsg->astCallInfos[pstAppCnfMsg->ucNumOfCalls].stCallNumber),
                       &(pstXcallEntity->stCallNumber),
                       sizeof(MN_CALL_BCD_NUM_STRU));

            PS_MEM_CPY(&(pstAppCnfMsg->astCallInfos[pstAppCnfMsg->ucNumOfCalls].stConnectNumber),
                       &(pstXcallEntity->stConnectNumber),
                       sizeof(MN_CALL_BCD_NUM_STRU));

            PS_MEM_CPY(&(pstAppCnfMsg->astCallInfos[pstAppCnfMsg->ucNumOfCalls].stCalledNumber),
                       &(pstXcallEntity->stCalledNumber),
                       sizeof(MN_CALL_CALLED_NUM_STRU));

            (pstAppCnfMsg->ucNumOfCalls)++;
        }
    }

    /* 填写事件名称 */
    pstAppCnfMsg->enEvent  = MN_CALL_EVT_CLCC_INFO;
    pstAppCnfMsg->clientId = MN_GetRealClientId(pstCallMsg->clientId, WUEPS_PID_TAF);

    /* 确定消息长度 */
    usLength = (TAF_UINT16)sizeof(MN_CALL_INFO_QRY_CNF_STRU);

    /* 通过异步消息发送给AT */
    MN_SendReportMsg(MN_CALLBACK_CS_CALL, (TAF_UINT8 *)pstAppCnfMsg, usLength);

    PS_MEM_FREE(WUEPS_PID_TAF, pstAppCnfMsg);
    return;
}


/*****************************************************************************
 函 数 名  : TAF_XCALL_RcvAppXlemaReq
 功能描述  : 紧急呼号码查询处理
 输入参数  :
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年01月6日
    作    者   : h0246512
    修改内容   : 新增函数

*****************************************************************************/
VOS_VOID TAF_XCALL_RcvAppXlemaReq(VOS_VOID *pMsg)
{
    MN_CALL_APP_REQ_MSG_STRU           *pstCallMsg      = VOS_NULL_PTR;
    MN_CALL_ECC_NUM_INFO_STRU          *pstEccNumInfo   = VOS_NULL_PTR;

    pstCallMsg = (MN_CALL_APP_REQ_MSG_STRU*)pMsg;

    pstEccNumInfo = (MN_CALL_ECC_NUM_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_TAF, sizeof(MN_CALL_ECC_NUM_INFO_STRU));
    if (VOS_NULL_PTR == pstEccNumInfo)
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF,"TAF_XCALL_RcvAppXlemaReq: Alloc Mem Fail.");
        return;
    }

    /* 消息初始化 */
    PS_MEM_SET(pstEccNumInfo, 0, sizeof(MN_CALL_ECC_NUM_INFO_STRU));

    pstEccNumInfo->enEvent    = MN_CALL_EVT_XLEMA_CNF;
    pstEccNumInfo->usClientId = pstCallMsg->clientId;

    /* 获取紧急呼列表 */
    TAF_XCALL_GetEccNumList(pstEccNumInfo);

    TAF_XCALL_SndXlemaCnf(pstEccNumInfo);

    /* 内存释放 */
    PS_MEM_FREE(WUEPS_PID_TAF, pstEccNumInfo);
    return;
}
VOS_VOID TAF_XCALL_RcvAppCustomDialReq(VOS_VOID *pMsg)
{
    VOS_UINT32                                    ulRet;
    VOS_UINT8                                     ucCallId;
    TAF_CALL_APP_SEND_CUSTOM_DIAL_REQ_STRU       *pstCallOrig = VOS_NULL_PTR;
    TAF_XCALL_CALL_ENTITY_STRU                   *pstCallEntity = VOS_NULL_PTR;

    ucCallId                            = 0;
    pstCallOrig                         = (TAF_CALL_APP_SEND_CUSTOM_DIAL_REQ_STRU *)pMsg;

    /* 判断当前是否可以发起呼叫 */
    ulRet = TAF_XCALL_IsAllowMoCallInCurrentStatus(pstCallOrig->stCustomDialPara.enCallType);

    /* 判断是否允许发起呼叫 */
    if (VOS_FALSE == ulRet)
    {
        /* 上报呼叫失败 */
        TAF_XCALL_SndCallOrigCnf(TAF_CS_CAUSE_NOT_ALLOW,
                                 pstCallOrig->ulSenderPid,
                                 pstCallOrig->usClientId,
                                 pstCallOrig->stCustomDialPara.enCallType);

        return;
    }

    /* 分配call Id */
    ulRet = TAF_XCALL_AllocCallId(&ucCallId);
    if (VOS_FALSE == ulRet)
    {
        /* 上报呼叫失败 */
        TAF_XCALL_SndCallOrigCnf(TAF_CS_CAUSE_NO_CALL_ID,
                                 pstCallOrig->ulSenderPid,
                                 pstCallOrig->usClientId,
                                 pstCallOrig->stCustomDialPara.enCallType);

        return;
    }

    /* 新建一个呼叫实体 */
    pstCallEntity = TAF_XCALL_GetCallEntityAddr(ucCallId);

    /* 记录发起call业务用户的信息(AT/STK) */
    TAF_XCALL_UpdateClientInfo(ucCallId,
                               pstCallOrig->ulSenderPid,
                               pstCallOrig->usClientId,
                               pstCallOrig->ucOpId);
    /* 初始化呼叫参数 */
    pstCallEntity->enCallDir            = MN_CALL_DIR_MO;
    pstCallEntity->enCallState          = MN_CALL_S_DIALING;
    pstCallEntity->enCallType           = pstCallOrig->stCustomDialPara.enCallType;
    pstCallEntity->enCause              = TAF_CS_CAUSE_SUCCESS;
    pstCallEntity->ucConnectId          = 0;
    pstCallEntity->ucConRef             = 0;
    pstCallEntity->enSo                 = (TAF_XCALL_SO_TYPE_ENUM_UINT16)pstCallOrig->stCustomDialPara.ulSo;
    pstCallEntity->ucIsAlertAnsByPass   = VOS_FALSE;


    /* 初始化被叫号码 */
    PS_MEM_CPY(&pstCallEntity->stCalledNumber,
               &pstCallOrig->stCustomDialPara.stDialNumber,
               sizeof(MN_CALL_CALLED_NUM_STRU));

    /* TO DO:设置SDC中CS呼叫标志 */
    /* TAF_SDC_SetCsCallExistFlg(VOS_TRUE); */

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    /* TO DO:给MTC模块上报当前CS域业务状态 */
    /* TAF_SendMtcCsSrvInfoInd(); */
#endif

    /* 根据当前1X的服务状态判定是否需要发送呼叫请求给XCC,如果当前非正常服务,
       不发给XCC呼叫请求,重拨情况下直接通知XSD进行重拨;否则发给XCC */

    /* 通知MMA CALL NTF */
    TAF_SndMmaCdmaMoCallStartNtf(WUEPS_PID_TAF, pstCallOrig->usClientId, TAF_XCALL_BuildMmaCallType(pstCallOrig->stCustomDialPara.enCallType));

    if (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE == TAF_SDC_GetCsServiceStatus())
    {
        /* 发送呼叫消息给XCC */
        TAF_XCALL_SndXccOrigCallReq(ucCallId);

        /* 启动silent redial定时器 */
        TAF_XCALL_StartTimer(TI_TAF_XCALL_WAIT_CALL_REDIAL_PERIOD, TI_TAF_XCALL_WAIT_CALL_REDIAL_PERIOD_LEN, ucCallId);
    }
    else
    {
        /* Deleted by h00313353 for Iteration 17, 2015-8-13, begin */

        /* Deleted by h00313353 for Iteration 17, 2015-8-13, end */

        /* 启动silent redial定时器 */
        TAF_XCALL_StartTimer(TI_TAF_XCALL_WAIT_CALL_REDIAL_PERIOD, TI_TAF_XCALL_WAIT_CALL_REDIAL_PERIOD_LEN, ucCallId);

        TAF_XCALL_StartTimer(TI_TAF_XCALL_WAIT_CALL_REDAIL_INTERVAL, TI_TAF_XCALL_WAIT_CALL_REDAIL_INTERVAL_LEN, ucCallId);
    }

    /* 回复呼叫成功 */
    TAF_XCALL_SndCallOrigCnf(TAF_CS_CAUSE_SUCCESS,
                             pstCallOrig->ulSenderPid,
                             pstCallOrig->usClientId,
                             pstCallOrig->stCustomDialPara.enCallType);

    /* 上报呼叫事件 */
    TAF_XCALL_SndCallOrigInd(ucCallId);

    return;
}

/* Added by l00324781 for CDMA Iteration 16, 2015-7-24, begin */

VOS_VOID TAF_XCALL_RcvAppSendContDTMFReq(VOS_VOID *pMsg)
{
    TAF_CALL_CONT_DTMF_REQ_MSG_STRU                        *pstContDTMFReqMsg;
    TAF_CALL_SEND_CONT_DTMF_CNF_RESULT_ENUM_U8              enResult;
    MN_CALL_STATE_ENUM_U8                                   enCallState;

    pstContDTMFReqMsg = (TAF_CALL_CONT_DTMF_REQ_MSG_STRU *)pMsg;

    /* 判断该call_id对应的状态是否为ACTIVE,目前只支持在该状态下发送CONT DTMF  */
    enCallState = TAF_XCALL_GetCallState(pstContDTMFReqMsg->stContDTMFPara.ucCallId);
    if (MN_CALL_S_ACTIVE != enCallState)
    {
        TAF_ERROR_LOG2(WUEPS_PID_TAF,"TAF_XCALL_RcvAppSendContDTMFReq: Call State is error. CallID:, State:",
                      pstContDTMFReqMsg->stContDTMFPara.ucCallId, enCallState);

        /* 不是ACTIVE态，向AT返回TAF_CALL_EVT_SEND_CONT_DTMF_CNF */
        TAF_XCALL_SndContDTMFCnf(pstContDTMFReqMsg->usClientId,
                                 pstContDTMFReqMsg->ucOpId,
                                 TAF_CALL_SEND_CONT_DTMF_CNF_RESULT_ABNORMAL_STATE,
                                 pstContDTMFReqMsg->stContDTMFPara.ucCallId);
        return;
    }

    /*如果等待XCC回复的Cont DTMF CNF消息的标识为VOS_FALSE，XCALL可以给XCC发送ID_XCALL_XCC_CONT_DTMF_REQ消息
      如果等待XCC回复的Cont DTMF CNF消息的标识为VOS_TRUE,XCALL需要等待发送中的ID_XCALL_XCC_CONT_DTMF_REQ的回复消息，
      同时把最新收到的TAF_CALL_SEND_CONT_DTMF_REQ消息缓存起来
    */
    if (TAF_XCALL_WAIT_NO_XCC_CNF == TAF_XCALL_GetWaitForXCCDTMFCnfFlag(pstContDTMFReqMsg->stContDTMFPara.ucCallId))
    {
        /* 如果此时状态为:收到Start Cont Dtmf Req的响应，但是定时器处于运行态，此时收到消息，应该缓存,
            等待定时器超时，再处理
        */
        if (TAF_XCALL_TIMER_STATUS_RUNING == TAF_XCALL_GetTimerStatus(TI_TAF_XCALL_START_AND_STOP_CONT_DTMF_INTERVAL,
                                                                      pstContDTMFReqMsg->stContDTMFPara.ucCallId))
        {
            enResult = TAF_XCALL_SaveContDtmfInfo(pstContDTMFReqMsg);
        }
        else
        {
            enResult = TAF_XCALL_SndXccContDTMFReq(&pstContDTMFReqMsg->stContDTMFPara);

            /* 向XCC发送ID_XCALL_XCC_SEND_CONT_DTMF_REQ后，设置标识,区分是start还是stop */
            if (TAF_CALL_CONT_DTMF_START == pstContDTMFReqMsg->stContDTMFPara.enSwitch)
            {
                TAF_XCALL_SetWaitForXCCDTMFCnfFlag(pstContDTMFReqMsg->stContDTMFPara.ucCallId, TAF_XCALL_WAIT_XCC_CNF_START_CONT_DTMF);
            }
            else
            {
                TAF_XCALL_SetWaitForXCCDTMFCnfFlag(pstContDTMFReqMsg->stContDTMFPara.ucCallId, TAF_XCALL_WAIT_XCC_CNF_STOP_CONT_DTMF);
            }
        }
    }
    else
    {
        enResult = TAF_XCALL_SaveContDtmfInfo(pstContDTMFReqMsg);
    }

    /* 回复XCALL TAF_CALL_EVT_SEND_CONT_DTMF_CNF (ucResult)*/
    TAF_XCALL_SndContDTMFCnf(pstContDTMFReqMsg->usClientId,
                             pstContDTMFReqMsg->ucOpId,
                             enResult,
                             pstContDTMFReqMsg->stContDTMFPara.ucCallId);

    return;
}

/* Added by l00324781 for CDMA Iteration 16, 2015-7-24, end */

/* Added by y00307564 for CDMA Iteration 16 2015-7-23 begin */
/*****************************************************************************
 函 数 名  : TAF_XCALL_ReplaceFlashPlusWithDigit
 功能描述  : 将^CFSH命令中号码携带的加号转换为数字
 输入参数  : VOS_UINT8      ucSrcDigitNum ---下发的^CFSH命令中携带的号码个数
             VOS_UINT8     *pucSrcDigit   ---下发的^CFSH命令中携带的号码
 输出参数  : VOS_UINT8      ucDstDigitNum ---转换后的号码个数
             VOS_UINT8     *pucDstDigit   ---转换后的号码
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年07月23日
    作    者   : y00307564
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID TAF_XCALL_ReplaceFlashPlusWithDigit(
    VOS_UINT8                           ucSrcDigitNum,
    VOS_UINT8                          *pucSrcDigit,
    VOS_UINT8                          *pucDstDigitNum,
    VOS_UINT8                          *pucDstDigit
)
{
    VOS_UINT32                          i;
    VOS_UINT8                           j;
    MN_CALL_NUM_TYPE_ENUM_U8            enCurNumType;
    VOS_UINT16                          usCurMcc;
    TAF_SDC_1X_SYS_INFO_STRU           *pstSdc1xSysInfo = VOS_NULL_PTR;

    if (0 != ucSrcDigitNum)
    {
        /* 若有加号，记录该号码为国际号码，否则为unknown */
        if ('+' == pucSrcDigit[0])
        {
            enCurNumType = (VOS_UINT8)(0x80 | (MN_CALL_TON_INTERNATIONAL << 4) | MN_CALL_NPI_ISDN);

            i = 1;
        }
        else
        {
            enCurNumType = (VOS_UINT8)(0x80 | (MN_CALL_TON_UNKNOWN << 4) | MN_CALL_NPI_ISDN);

            i = 0;
        }

        j = 0;
        for (; i < ucSrcDigitNum; i++)
        {
            pucDstDigit[j] = pucSrcDigit[i];

            j++;
        }

        *pucDstDigitNum = j;

        /*获取mcc */
        pstSdc1xSysInfo   = TAF_SDC_Get1xSysInfo();
        usCurMcc          = TAF_STD_TransformBcdMccToDeciDigit(pstSdc1xSysInfo->stServiceInfo.ulMcc);

        /* 加号转换 */
        TAF_TranslatePlusToNum(usCurMcc, TAF_PLUS_TRANSLATE_DIAL_TYPE_VOICE, &enCurNumType, pucDstDigit, pucDstDigitNum);
    }
    else
    {
        *pucDstDigitNum = 0;
        pucDstDigit[0]  = '\0';
    }
}
/* Added by y00307564 for CDMA Iteration 16 2015-7-23 end */

VOS_VOID TAF_XCALL_RcvAtCclprGetReq(VOS_VOID *pstMsg)
{
    TAF_CALL_EVT_CCLPR_GET_CNF_STRU                         stAppCnfMsg;
    TAF_XCALL_CALL_ENTITY_STRU                             *pstXcallEntity = VOS_NULL_PTR;
    TAF_CALL_SND_CCLPR_REQ_MSG_STRU                        *pstCallMsg = VOS_NULL_PTR;

    pstCallMsg = (TAF_CALL_SND_CCLPR_REQ_MSG_STRU *)pstMsg;

    PS_MEM_SET(&stAppCnfMsg, 0x00, sizeof(TAF_CALL_EVT_CCLPR_GET_CNF_STRU));

    pstXcallEntity = TAF_XCALL_GetCallEntityAddr(pstCallMsg->ucCallId);
    if ((VOS_TRUE == pstXcallEntity->ulUsed)
     && (MN_CALL_DIR_MT == pstXcallEntity->enCallDir))

    {
        stAppCnfMsg.enPI = pstXcallEntity->ucPi;
    }
    else
    {
        stAppCnfMsg.enPI = TAF_CALL_PRESENTATION_BUTT;
    }

    stAppCnfMsg.ucCallId        = pstCallMsg->ucCallId;
    stAppCnfMsg.ulRet           = TAF_ERR_NO_ERROR;

    /* 上报TAF_CALL_EVT_CCLPR_SET_CNF */
    TAF_XCALL_SndCsEvtToAt(TAF_CALL_EVT_CCLPR_SET_CNF,
                           pstCallMsg->usClientId,
                           sizeof(TAF_CALL_EVT_CCLPR_GET_CNF_STRU),
                          &stAppCnfMsg);

    return;
}


/*lint -restore*/

#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */





