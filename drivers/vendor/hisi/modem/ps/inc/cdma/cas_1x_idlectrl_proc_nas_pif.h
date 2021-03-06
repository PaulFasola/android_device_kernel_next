/** ****************************************************************************

                    Huawei Technologies Sweden AB (C), 2001-2015

 ********************************************************************************
 * @author    Automatically generated by DAISY
 * @version
 * @date      2015-06-17
 * @file
 * @brief
 * @copyright Huawei Technologies Sweden AB
 *******************************************************************************/
#ifndef CAS_1X_IDLECTRL_PROC_NAS_PIF_H
#define CAS_1X_IDLECTRL_PROC_NAS_PIF_H

/*******************************************************************************
 1. Other files included
*******************************************************************************/

#include "vos.h"
#include "cas_1x_control_initialstate_nas_pif.h"
#include "PsTypeDef.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#pragma pack(4)

/*******************************************************************************
 2. Macro definitions
*******************************************************************************/


/*******************************************************************************
 3. Enumerations declarations
*******************************************************************************/

/** ****************************************************************************
 * Name        : CAS_1X_IDLECTRL_PROC_NAS_PIF_MSG_TYPE_ENUM_UINT16
 *
 *******************************************************************************/
enum CAS_1X_IDLECTRL_PROC_NAS_PIF_MSG_TYPE_ENUM
{
    ID_CNAS_CAS_1X_BETTER_SYNC_REQ                          = 0x7000, /**< @sa CNAS_CAS_1X_BETTER_SYNC_REQ_STRU */
    ID_CAS_CNAS_1X_BETTER_SYNC_CNF                          = 0x7001, /**< @sa CAS_CNAS_1X_BETTER_SYNC_CNF_STRU */
    ID_CNAS_CAS_1X_STOP_BETTER_SYNC_REQ                     = 0x7002, /**< @sa CNAS_CAS_1X_STOP_BETTER_SYNC_REQ_STRU */
    ID_CAS_CNAS_1X_STOP_BETTER_SYNC_CNF                     = 0x7003, /**< @sa CAS_CNAS_1X_STOP_BETTER_SYNC_CNF_STRU */
    ID_CNAS_CAS_1X_BETTER_SERVICE_REQ                       = 0x7004, /**< @sa CNAS_CAS_1X_BETTER_SERVICE_REQ_STRU */
    ID_CAS_CNAS_1X_BETTER_SERVICE_CNF                       = 0x7005, /**< @sa CAS_CNAS_1X_BETTER_SERVICE_CNF_STRU */
    ID_CNAS_CAS_1X_STOP_BETTER_SERVICE_REQ                  = 0x7006, /**< @sa CNAS_CAS_1X_STOP_BETTER_SERVICE_REQ_STRU */
    ID_CAS_CNAS_1X_STOP_BETTER_SERVICE_CNF                  = 0x7007, /**< @sa CAS_CNAS_1X_STOP_BETTER_SERVICE_CNF_STRU */
    ID_CAS_CNAS_1X_OHM_IND                                  = 0x7008, /**< @sa CAS_CNAS_1X_OHM_IND_STRU */
    ID_CNAS_CAS_1X_SLOT_CYCLE_INDEX_NTF                     = 0x7009, /**< @sa CNAS_CAS_1X_SLOT_CYCLE_INDEX_NTF_STRU */
    ID_CAS_CNAS_1X_SYNC_TIME_IND                            = 0x700A, /**< @sa CAS_CNAS_1X_SYNC_TIME_IND_STRU */
    ID_CAS_1X_IDLECTRL_PROC_NAS_PIF_MSG_TYPE_ENUM_BUTT      = 0xFFFF
};
typedef VOS_UINT16 CAS_1X_IDLECTRL_PROC_NAS_PIF_MSG_TYPE_ENUM_UINT16;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_BSR_RSLT_ENUM_UINT8
 *
 *******************************************************************************/
enum CNAS_CAS_1X_BSR_RSLT_ENUM
{
    CNAS_CAS_1X_BSR_RSLT_SUCCESS        = 0x00,
    CNAS_CAS_1X_BSR_RSLT_FAILURE        = 0x01,
    CNAS_CAS_1X_BSR_RSLT_REJECT         = 0x02,
    CNAS_CAS_1X_BSR_RSLT_BUTT           = 0x03
};
typedef VOS_UINT8 CNAS_CAS_1X_BSR_RSLT_ENUM_UINT8;

/*******************************************************************************
 4. Message Header declaration
*******************************************************************************/

/*******************************************************************************
 5. Message declaration
*******************************************************************************/

/*******************************************************************************
 6. STRUCT and UNION declaration
*******************************************************************************/

/** ****************************************************************************
 * Name        : CAS_CNAS_1X_SYS_TIME_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT8                           aucSysTime[5];
    VOS_UINT8                           ucLpSec;
    VOS_INT8                            cLtmOff;
    VOS_UINT8                           ucDaylt;
} CAS_CNAS_1X_SYS_TIME_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_OHM_REG_INFO_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT16                          usRegZone;      /**< Registration zone */
    VOS_UINT8                           ucRegZoneNum;   /**< Number of registration zones to be retained for reg */
    VOS_UINT8                           ucZoneTimer;    /**< Zone timer length for reg */
    VOS_UINT8                           ucMultiSidFlg;  /**< Multiple SID storage indicator for reg */
    VOS_UINT8                           ucMultiNidFlg;  /**< Multiple NID storage indicator for reg */
    VOS_UINT16                          usRegDistance;  /**< Registration distance for reg */
    VOS_UINT8                           ucRegPeriod;    /**< Registration period for reg */
    VOS_UINT8                           ucHomeReg;      /**< Home registration indicator */
    VOS_UINT8                           ucSidRoamReg;   /**< SID roamer registration indicator for roam */
    VOS_UINT8                           ucNidRoamReg;   /**< NID roamer registration indicator for roam */
    VOS_UINT8                           ucPowerUpReg;   /**< Power-up registration indicator for reg */
    VOS_UINT8                           ucPowerDownReg; /**< Power-down registration indicator for reg */
    VOS_UINT8                           ucParameterReg; /**< Parameter-change registration indicator for reg */
    VOS_UINT8                           aucReserve[1];
} CNAS_CAS_1X_OHM_REG_INFO_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_OHM_BASE_STATION_INFO_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT16                          usBaseId;       /**< Base station identification for user zone */
    VOS_UINT8                           ucBaseClass;    /**< Base station class */
    VOS_UINT8                           aucReserve[1];
    VOS_INT32                           lBaseLatitude;  /**< Base station latitude reg & zone */
    VOS_INT32                           lBaseLongitude; /**< Base station longitude reg & zone */
} CNAS_CAS_1X_OHM_BASE_STATION_INFO_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_OHM_SERVICE_INFO_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT8                           ucMaxAltSo;            /**< Max. Num. of alternative service options,for Origination or Page Response Message */
    VOS_UINT8                           ucSDBSupported;        /**< Short Data Burst supported indicator */
    VOS_UINT8                           ucMoQos;               /**< permission indicator for QoS in Origination message */
    VOS_UINT8                           ucConcurrentSupported; /**< concurrent service supported */
    VOS_UINT8                           ucMoPosSupported;      /**< MS initiated position location supported */
    VOS_UINT8                           ucImsi11_12;           /**< 11th and 12th digits of the IMSI,invalid value=0xff */
    VOS_UINT16                          usMcc;                 /**< Mobile country code,invalid value=0xff */
} CNAS_CAS_1X_OHM_SERVICE_INFO_STRU;

/*******************************************************************************
 7. OTHER declarations
*******************************************************************************/

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_BETTER_SYNC_REQ_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_IDLECTRL_PROC_NAS_PIF_MSG_TYPE_ENUM_UINT16       enMsgId;
    VOS_UINT16                                              usOpId;
    VOS_UINT32                                              ulFreqNum;                             /**< Number of frequencies included in list */
    CNAS_CAS_1X_FREQENCY_CHANNEL_STRU                       astFreqList[CNAS_CAS_1X_MAX_FREQ_NUM]; /**< List of frequencies to search, in priority order */
} CNAS_CAS_1X_BETTER_SYNC_REQ_STRU;

/** ****************************************************************************
 * Name        : CAS_CNAS_1X_BETTER_SYNC_CNF_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_IDLECTRL_PROC_NAS_PIF_MSG_TYPE_ENUM_UINT16       enMsgId;
    VOS_UINT16                                              usOpId;
    CNAS_CAS_1X_BSR_RSLT_ENUM_UINT8                         enSyncRslt;
    VOS_UINT8                                               aucReserve[3];
    CNAS_CAS_1X_SYSTEM_STRU                                 stSyncSystem;
    VOS_UINT32                                              ulSyncFreqNum;                             /**< Number of frequencies included in list */
    CNAS_CAS_1X_SYNC_RSLT_INFO_STRU                         astSyncRsltList[CNAS_CAS_1X_MAX_FREQ_NUM]; /**< List of frequencies */
} CAS_CNAS_1X_BETTER_SYNC_CNF_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_STOP_BETTER_SYNC_REQ_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_IDLECTRL_PROC_NAS_PIF_MSG_TYPE_ENUM_UINT16       enMsgId;
    VOS_UINT16                                              usOpId;
    VOS_UINT8                                               aucReserve[4];
} CNAS_CAS_1X_STOP_BETTER_SYNC_REQ_STRU;

/** ****************************************************************************
 * Name        : CAS_CNAS_1X_STOP_BETTER_SYNC_CNF_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_IDLECTRL_PROC_NAS_PIF_MSG_TYPE_ENUM_UINT16       enMsgId;
    VOS_UINT16                                              usOpId;
    VOS_UINT8                                               aucReserve[4];
} CAS_CNAS_1X_STOP_BETTER_SYNC_CNF_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_BETTER_SERVICE_REQ_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_IDLECTRL_PROC_NAS_PIF_MSG_TYPE_ENUM_UINT16       enMsgId;
    VOS_UINT16                                              usOpId;
    VOS_UINT32                                              ulFreqNum;                                 /**< Number of frequencies included in list */
    CNAS_CAS_1X_SYSTEM_STRU                                 astSystemList[CNAS_CAS_1X_MAX_SYSTEM_NUM]; /**< List of frequencies to search, in priority order */
} CNAS_CAS_1X_BETTER_SERVICE_REQ_STRU;

/** ****************************************************************************
 * Name        : CAS_CNAS_1X_BETTER_SERVICE_CNF_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_IDLECTRL_PROC_NAS_PIF_MSG_TYPE_ENUM_UINT16       enMsgId;
    VOS_UINT16                                              usOpId;
    CNAS_CAS_1X_BSR_RSLT_ENUM_UINT8                         enAcqRslt;
    VOS_UINT8                                               aucReserve[3];
    CNAS_CAS_1X_SYSTEM_STRU                                 stBetterSystem;
    VOS_UINT32                                              ulAcqSysNum;
    CNAS_CAS_1X_ACQ_RSLT_INFO_STRU                          astAcqRsltList[CNAS_CAS_1X_MAX_SYSTEM_NUM];
} CAS_CNAS_1X_BETTER_SERVICE_CNF_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_STOP_BETTER_SERVICE_REQ_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_IDLECTRL_PROC_NAS_PIF_MSG_TYPE_ENUM_UINT16       enMsgId;
    VOS_UINT16                                              usOpId;
    VOS_UINT8                                               aucReserve[4];
} CNAS_CAS_1X_STOP_BETTER_SERVICE_REQ_STRU;

/** ****************************************************************************
 * Name        : CAS_CNAS_1X_STOP_BETTER_SERVICE_CNF_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_IDLECTRL_PROC_NAS_PIF_MSG_TYPE_ENUM_UINT16       enMsgId;
    VOS_UINT16                                              usOpId;
    VOS_UINT8                                               aucReserve[4];
} CAS_CNAS_1X_STOP_BETTER_SERVICE_CNF_STRU;

/** ****************************************************************************
 * Name        : CAS_CNAS_1X_OHM_IND_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_IDLECTRL_PROC_NAS_PIF_MSG_TYPE_ENUM_UINT16       enMsgId;
    VOS_UINT16                                              usOpId;
    VOS_UINT16                                              usBandClass;
    VOS_UINT16                                              usFreq;
    VOS_UINT16                                              usSid;                 /**< System identification */
    VOS_UINT16                                              usNid;                 /**< Network identification */
    VOS_UINT8                                               ucPacketZoneId;        /**< Packet Data service zone identifier */
    VOS_UINT8                                               ucPRevInUse;           /**< P_REV_IN_USEs-Protocol revision level currently in use by a mobile station */
    VOS_UINT8                                               ucMaxSlotCycleIndex;   /**< Max Slot Cycle Index, 0.....7 */
    VOS_UINT8                                               aucReserve[2];
    PS_BOOL_ENUM_UINT8                                      enRegInfoIncl;
    PS_BOOL_ENUM_UINT8                                      enBaseStationInfoIncl;
    PS_BOOL_ENUM_UINT8                                      enServiceInfoIncl;
    CNAS_CAS_1X_OHM_REG_INFO_STRU                           stRegInfo;
    CNAS_CAS_1X_OHM_BASE_STATION_INFO_STRU                  stBaseStationInfo;
    CNAS_CAS_1X_OHM_SERVICE_INFO_STRU                       stServiceInfo;
} CAS_CNAS_1X_OHM_IND_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_SLOT_CYCLE_INDEX_NTF_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_IDLECTRL_PROC_NAS_PIF_MSG_TYPE_ENUM_UINT16       enMsgId;
    VOS_UINT16                                              usOpId;
    VOS_UINT8                                               ucSlotCycleIndex; /**< Slot Cycle Index, 0....7 */
    VOS_UINT8                                               aucReserved[3];
} CNAS_CAS_1X_SLOT_CYCLE_INDEX_NTF_STRU;

/** ****************************************************************************
 * Name        : CAS_CNAS_1X_SYNC_TIME_IND_STRU
 *
 * Description : This signal is sent by CAS to sync time in Sync channel
 * message. CAS will send when every timing succ.
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_IDLECTRL_PROC_NAS_PIF_MSG_TYPE_ENUM_UINT16       enMsgId;
    VOS_UINT16                                              usOpId;
    CAS_CNAS_1X_SYS_TIME_STRU                               stSysTime;
} CAS_CNAS_1X_SYNC_TIME_IND_STRU;

/*******************************************************************************
 8. Global  declaration
*******************************************************************************/

/*******************************************************************************
 9. Function declarations
*******************************************************************************/

#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
