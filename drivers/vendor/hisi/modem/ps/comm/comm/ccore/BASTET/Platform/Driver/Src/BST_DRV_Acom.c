
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "BST_Platform.h"
#include "BST_DRV_Acom.h"
#include "BST_SRV_Event.h"
/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_BST_DRV_ACOM_C
/*lint +e767*/
/******************************************************************************
   2 �궨��
******************************************************************************/

/*****************************************************************************
  3 ��������
*****************************************************************************/
BST_ERR_ENUM_UINT8 BST_DRV_ACOM_Initial( BST_VOID *pvArg );
/******************************************************************************
   4 ˽�ж���
******************************************************************************/

/******************************************************************************
   5 ȫ�ֱ�������
******************************************************************************/
BST_DRV_STRU                            g_BstDrvAcomHandle =
{
    BST_DRV_ACOM_Initial,
    BST_DRV_ACOM_Send,
    BST_DRV_ACOM_Recv,
    BST_NULL_PTR,
    BST_FALSE
};
/******************************************************************************
   6 ����ʵ��
******************************************************************************/


BST_ERR_ENUM_UINT8  BST_DRV_ACOM_Initial( BST_VOID *pvArg )
{
    BST_Pal_DrvAcomInit( BST_SRV_ApiSendAcomRxEvent );
    g_BstDrvAcomHandle.ulIsInited       = BST_TRUE;
    return BST_NO_ERROR_MSG;
}