


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "oal_net.h"
#include "oal_cfg80211.h"
#include "wlan_spec.h"
#include "wal_linux_event.h"
#include "wal_linux_scan.h"
#include "wal_linux_cfg80211.h"
#include "wal_main.h"
#include "wal_linux_rx_rsp.h"
#include "hmac_vap.h"
#include "hmac_device.h"
#include "hmac_resource.h"

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)&&(_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include <net/cfg80211.h>
#elif (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)

#endif

#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION) && (_PRE_TEST_MODE == _PRE_TEST_MODE_UT)
    /* UT模式下调用frw_event_process_all_event */
    extern oal_void  frw_event_process_all_event(oal_uint ui_data);
#endif

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_SCAN_C

/*****************************************************************************
  2 全局变量定义
*****************************************************************************/

/*****************************************************************************
  3 函数实现
*****************************************************************************/


OAL_STATIC oal_void  wal_inform_bss_frame(wal_scanned_bss_info_stru *pst_scanned_bss_info, oal_void *p_data)
{
    oal_cfg80211_bss_stru        *pst_cfg80211_bss;
    oal_wiphy_stru               *pst_wiphy;
    oal_ieee80211_channel_stru   *pst_ieee80211_channel;

    if ((OAL_PTR_NULL == pst_scanned_bss_info) || (OAL_PTR_NULL == p_data))
    {
        OAM_ERROR_LOG2(0, OAM_SF_SCAN, "{wal_inform_bss_frame::input param pointer is null, pst_scanned_bss_info[%p], p_data[%p]!}",
                       pst_scanned_bss_info, p_data);
        return;
    }

    pst_wiphy = (oal_wiphy_stru *)p_data;

    pst_ieee80211_channel = oal_ieee80211_get_channel(pst_wiphy, (oal_int32)pst_scanned_bss_info->s_freq);

    OAM_INFO_LOG1(0, OAM_SF_SCAN, "{wal_inform_bss_frame::freq = [%u]!}", (oal_uint16)pst_scanned_bss_info->s_freq);

    pst_scanned_bss_info->l_signal = pst_scanned_bss_info->l_signal * 100;

    /* 逐个上报内核bss 信息 */
    pst_cfg80211_bss = oal_cfg80211_inform_bss_frame(pst_wiphy, pst_ieee80211_channel, pst_scanned_bss_info->pst_mgmt, pst_scanned_bss_info->ul_mgmt_len, pst_scanned_bss_info->l_signal, GFP_ATOMIC);
    if (pst_cfg80211_bss != NULL)
    {
        oal_cfg80211_put_bss(pst_cfg80211_bss);
    }

    return;
}
oal_void  wal_inform_all_bss(oal_wiphy_stru  *pst_wiphy, hmac_bss_mgmt_stru  *pst_bss_mgmt, oal_uint8   uc_vap_id)
{
    mac_bss_dscr_stru              *pst_bss_dscr;
    hmac_scanned_bss_info          *pst_scanned_bss;
    oal_dlist_head_stru            *pst_entry;
    mac_ieee80211_frame_stru       *pst_frame_hdr;
    wal_scanned_bss_info_stru       st_scanned_bss_info;
    oal_uint32                      ul_ret;
    oal_uint32                      ul_bss_num_not_in_regdomain = 0;
    oal_uint8                       uc_chan_number;
    enum ieee80211_band             en_band;

    /* 获取锁 */
    oal_spin_lock(&(pst_bss_mgmt->st_lock));

    /* 遍历扫描到的bss信息 */
    OAL_DLIST_SEARCH_FOR_EACH(pst_entry, &(pst_bss_mgmt->st_bss_list_head))
    {
        pst_scanned_bss = OAL_DLIST_GET_ENTRY(pst_entry, hmac_scanned_bss_info, st_dlist_head);
        pst_bss_dscr    = &(pst_scanned_bss->st_bss_dscr_info);
        if ((OAL_PTR_NULL == pst_bss_dscr) || (OAL_PTR_NULL == pst_bss_dscr->auc_mgmt_buff))
        {
            continue;
        }

        uc_chan_number = pst_bss_dscr->st_channel.uc_chan_number;
        en_band        = (enum ieee80211_band)pst_bss_dscr->st_channel.en_band;

        /* 判断信道是不是在管制域内，如果不在，则不上报内核 */
        ul_ret = mac_is_channel_num_valid(en_band, uc_chan_number);
        if (OAL_SUCC != ul_ret)
        {
            OAM_WARNING_LOG2(uc_vap_id, OAM_SF_SCAN,
                             "{wal_inform_all_bss::curr channel[%d] and band[%d] is not in regdomain.}",
                             uc_chan_number, en_band);
            ul_bss_num_not_in_regdomain++;
            continue;
        }

        /* 初始化 */
        oal_memset(&st_scanned_bss_info, 0, OAL_SIZEOF(wal_scanned_bss_info_stru));

        st_scanned_bss_info.l_signal    = pst_bss_dscr->c_rssi;

        /*填bss所在信道的中心频率*/
        st_scanned_bss_info.s_freq      = (oal_int16)oal_ieee80211_channel_to_frequency(uc_chan_number, en_band);
        OAM_INFO_LOG3(uc_vap_id, OAM_SF_SCAN, "{wal_inform_all_bss::scan_comp freq[%u] chan[%d] band[%d] !}",
                      (oal_uint16)st_scanned_bss_info.s_freq, uc_chan_number, en_band);

        /* 填管理帧指针和长度 */
        st_scanned_bss_info.pst_mgmt    = (oal_ieee80211_mgmt_stru *)(pst_bss_dscr->auc_mgmt_buff);
        st_scanned_bss_info.ul_mgmt_len = pst_bss_dscr->ul_mgmt_len;

        /* 获取上报的扫描结果的管理帧的帧头 */
        pst_frame_hdr  = (mac_ieee80211_frame_stru *)pst_bss_dscr->auc_mgmt_buff;

        /* 如果扫描请求接收到的帧类型有beacon类型，统一修改为probe rsp类型上报，
           为了解决上报内核的扫描结果beacon帧不够敏感的问题，此问题，在01出现过 */
    	if (WLAN_BEACON == pst_frame_hdr->st_frame_control.bit_sub_type)
    	{
    	    /* 修改beacon帧类型为probe rsp */
    		pst_frame_hdr->st_frame_control.bit_sub_type = WLAN_PROBE_RSP;
    	}

        /* 上报扫描结果给内核 */
        wal_inform_bss_frame(&st_scanned_bss_info, pst_wiphy);
    }

    /* 解除锁 */
    oal_spin_unlock(&(pst_bss_mgmt->st_lock));

    OAM_WARNING_LOG2(uc_vap_id, OAM_SF_SCAN,
                     "{wal_inform_all_bss::there are %d bss not in regdomain, so inform kernal bss num is [%d]!}",
                     ul_bss_num_not_in_regdomain, (pst_bss_mgmt->ul_bss_num - ul_bss_num_not_in_regdomain));

    return;
}
OAL_STATIC oal_void  wal_set_scan_channel(
                    oal_cfg80211_scan_request_stru    *pst_request,
					oal_uint32                        *pul_channels_2g,
					oal_uint32                        *pul_num_channels_2g,
					oal_uint32                        *pul_channels_5g,
					oal_uint32                        *pul_num_channels_5g)

{
    oal_uint32  ul_loop;
    oal_uint32  ul_num_chan_2G;
    oal_uint32  ul_num_chan_5G;
    oal_uint16  us_center_freq;
    oal_int32   l_chan;

    if ((OAL_PTR_NULL == pst_request) || (OAL_PTR_NULL == pul_channels_2g))
    {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{wal_set_scan_channel::scan failed, null ptr, pst_request[%p], pul_channels_2g[%p].}\r\n",
          pst_request, pul_channels_2g);

        return ;
    }

    if ((OAL_PTR_NULL == pul_num_channels_2g) || (OAL_PTR_NULL == pul_channels_5g)|| (OAL_PTR_NULL == pul_num_channels_5g))
    {
        OAM_ERROR_LOG3(0, OAM_SF_ANY, "{wal_set_scan_channel::scan failed, null ptr, pul_num_channels_2g[%p], pul_channels_5g[%p], pul_num_channels_5g[%p].}\r\n",
          pul_num_channels_2g, pul_channels_5g, pul_num_channels_5g);

        return ;
    }

    ul_num_chan_2G = 0;
    ul_num_chan_5G = 0;

    for (ul_loop = 0; ul_loop < pst_request->n_channels; ul_loop++)
    {
        us_center_freq = pst_request->channels[ul_loop]->center_freq;

        /* 根据中心频率，计算信道号 */
        l_chan = (oal_int32)oal_ieee80211_frequency_to_channel((oal_int32)us_center_freq);

        if (us_center_freq <= WAL_MAX_FREQ_2G)
        {
            pul_channels_2g[ul_num_chan_2G++] = (oal_uint32)l_chan;
        }
        else
        {
            pul_channels_5g[ul_num_chan_5G++] = (oal_uint32)l_chan;
        }
    }

    *pul_num_channels_2g = ul_num_chan_2G;
    *pul_num_channels_5g = ul_num_chan_5G;
}
OAL_STATIC oal_void wal_set_scan_ssid(oal_cfg80211_scan_request_stru *pst_request, mac_cfg80211_scan_param_stru *pst_scan_param)
{
    oal_int32   l_loop;
    oal_int32   l_ssid_num;

    if ((OAL_PTR_NULL == pst_request) || (OAL_PTR_NULL == pst_scan_param))
    {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{wal_set_scan_ssid::scan failed, null ptr, pst_request[%p], pst_scan_param[%p].}",
          pst_request, pst_scan_param);

        return;
    }

    pst_scan_param->en_scan_type = OAL_ACTIVE_SCAN; /* active scan */
    pst_scan_param->l_ssid_num   = 0;

    /* 取内核下发的ssid的个数 */
    l_ssid_num = pst_request->n_ssids;
    if (l_ssid_num > WLAN_SCAN_REQ_MAX_SSID)
    {
        /* 如果用户下发的指定ssid的个数大于驱动支持的最大个数，则取驱动支持的指定ssid的最大个数 */
        l_ssid_num = WLAN_SCAN_REQ_MAX_SSID;
    }

    /* 将用户下发的ssid信息拷贝到对应的结构体中 */
    if ((l_ssid_num > 0) && (OAL_PTR_NULL != pst_request->ssids))
    {
        pst_scan_param->l_ssid_num = l_ssid_num;

        for (l_loop = 0; l_loop < l_ssid_num; l_loop++)
        {
            pst_scan_param->st_ssids[l_loop].uc_ssid_len = pst_request->ssids[l_loop].ssid_len;
            if (pst_scan_param->st_ssids[l_loop].uc_ssid_len > OAL_IEEE80211_MAX_SSID_LEN)
            {
                OAM_WARNING_LOG2(0, OAM_SF_ANY, "{wal_set_scan_ssid::ssid scan set failed, ssid_len[%d] is exceed, max[%d].}",
                  pst_scan_param->st_ssids[l_loop].uc_ssid_len, OAL_IEEE80211_MAX_SSID_LEN);

                pst_scan_param->st_ssids[l_loop].uc_ssid_len = OAL_IEEE80211_MAX_SSID_LEN;
            }

            oal_memcopy(pst_scan_param->st_ssids[l_loop].auc_ssid, pst_request->ssids[l_loop].ssid,
              pst_scan_param->st_ssids[l_loop].uc_ssid_len);
        }
	}
}


OAL_STATIC oal_uint32  wal_wait_for_scan_timeout_fn(void *p_arg)
{
    hmac_vap_stru                  *pst_hmac_vap = (hmac_vap_stru *)p_arg;
    mac_vap_stru                   *pst_mac_vap  = &(pst_hmac_vap->st_vap_base_info);
    hmac_device_stru               *pst_hmac_device;
    mac_device_stru                *pst_mac_device;
    hmac_bss_mgmt_stru             *pst_bss_mgmt;
    oal_wiphy_mgmt_stru            *pst_wiphy_mgmt;
    oal_wiphy_stru                 *pst_wiphy;

    OAM_WARNING_LOG0(0, OAM_SF_SCAN, "{wal_wait_for_scan_complete_time_out::scan timeout proc.}");

    /* 根据当前扫描的类型和当前vap的状态，决定切换vap的状态，扫描异常保护中，上报内核扫描状态为扫描完成 */
    if ((WLAN_VAP_MODE_BSS_STA == pst_mac_vap->en_vap_mode) &&
        (MAC_VAP_STATE_STA_WAIT_SCAN == pst_mac_vap->en_vap_state))
    {
        /* 改变vap状态到SCAN_COMP */
        mac_vap_state_change_to_sdt(pst_mac_vap, MAC_VAP_STATE_STA_SCAN_COMP);
        pst_mac_vap->en_vap_state = MAC_VAP_STATE_STA_SCAN_COMP;
    }

    /* 获取hmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_hmac_device)
    {
        OAM_WARNING_LOG0(0, OAM_SF_SCAN, "{wal_wait_for_scan_complete_time_out::pst_hmac_device is null.}");
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)&&(_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
        wlan_pm_enable();
#endif
        return OAL_FAIL;
    }

    /* 获取mac device */
    pst_mac_device = pst_hmac_device->pst_device_base_info;

    pst_wiphy_mgmt = &(pst_mac_device->st_wiphy_mgmt);
    pst_wiphy      = pst_mac_device->pst_wiphy;

    /* 获取扫描结果的管理结构地址 */
    pst_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);

    if (OAL_PTR_NULL != pst_wiphy_mgmt->pst_request)
    {
        /* 上报扫描到的所有的bss */
        wal_inform_all_bss(pst_wiphy, pst_bss_mgmt, pst_mac_vap->uc_vap_id);

        /* 通知 kernel scan 已经结束 */
        oal_cfg80211_scan_done(pst_wiphy_mgmt->pst_request, 0);

        pst_wiphy_mgmt->pst_request = OAL_PTR_NULL;
        pst_wiphy_mgmt->en_complete = OAL_TRUE;

        /* 让编译器优化时保证OAL_WAIT_QUEUE_WAKE_UP在最后执行 */
        OAL_SMP_MB();
        OAL_WAIT_QUEUE_WAKE_UP(&pst_wiphy_mgmt->st_wait_queue);
    }

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)&&(_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    wlan_pm_enable();
#endif
    return OAL_SUCC;
}


OAL_STATIC oal_void  wal_start_timer_for_scan_timeout(oal_uint8   uc_vap_id)
{
    hmac_vap_stru               *pst_hmac_vap = OAL_PTR_NULL;
    mac_device_stru             *pst_mac_device = OAL_PTR_NULL;
    oal_uint32                   ul_timeout;

    /* 超时时间为5秒 */
    ul_timeout = WAL_MAX_SCAN_TIME_PER_SCAN_REQ;

    /* 获取hmac vap */
    pst_hmac_vap = mac_res_get_hmac_vap(uc_vap_id);
    if (OAL_PTR_NULL == pst_hmac_vap)
    {
        OAM_ERROR_LOG0(uc_vap_id, OAM_SF_SCAN, "{wal_start_timer_for_scan_timeout::pst_hmac_vap is null!}");
        return;
    }

    /* 获取mac device */
    pst_mac_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (OAL_PTR_NULL == pst_mac_device)
    {
        OAM_ERROR_LOG0(uc_vap_id, OAM_SF_SCAN, "{wal_start_timer_for_scan_timeout::pst_mac_device is null!}");
        return;
    }

    /* 启动扫描保护定时器，在指定时间没有上报扫描结果，主动上报扫描完成 */
    FRW_TIMER_CREATE_TIMER(&(pst_hmac_vap->st_scan_timeout),
                           wal_wait_for_scan_timeout_fn,
                           ul_timeout,
                           pst_hmac_vap,
                           OAL_FALSE,
                           OAM_MODULE_ID_WAL,
                           pst_mac_device->ul_core_id);

    OAM_WARNING_LOG1(uc_vap_id, OAM_SF_SCAN,
                     "{wal_start_timer_for_scan_timeout::start a timer, wait time[%d] for scan timeout proc.}", ul_timeout);

    return;
}


oal_void wal_scan_work_func(oal_wiphy_mgmt_stru                   *pst_wiphy_mgmt,
                                  oal_net_device_stru                   *pst_netdev,
                                  oal_cfg80211_scan_request_stru        *pst_request)
{
    mac_cfg80211_scan_param_stru st_scan_param;
    oal_uint32                  *pul_channels_2G = OAL_PTR_NULL;
    oal_uint32                  *pul_channels_5G = OAL_PTR_NULL;
    oal_uint32                   ul_num_channels_2G = 0;
    oal_uint32                   ul_num_channels_5G = 0;
    oal_uint32                   ul_channel_num = 0;
    oal_uint32                   ul_ret;
    mac_vap_stru                *pst_mac_vap = OAL_NET_DEV_PRIV(pst_netdev);
    oal_uint8                    uc_vap_id = pst_mac_vap->uc_vap_id;

    oal_memset(&st_scan_param, 0, sizeof(mac_cfg80211_scan_param_stru));

    /* 记录内核下发的信道的总数 */
    ul_channel_num = pst_request->n_channels;

    pul_channels_2G = oal_memalloc(ul_channel_num * OAL_SIZEOF(oal_uint32));
    if (OAL_UNLIKELY(OAL_PTR_NULL == pul_channels_2G))
    {
        OAM_ERROR_LOG0(uc_vap_id, OAM_SF_CFG, "{wal_scan_work_func::2.4G channel alloc mem return null ptr!}");
        return;
    }

    pul_channels_5G = oal_memalloc(ul_channel_num * OAL_SIZEOF(oal_uint32));
    if (OAL_UNLIKELY(OAL_PTR_NULL == pul_channels_5G))
    {
        OAM_ERROR_LOG0(uc_vap_id, OAM_SF_CFG, "{wal_scan_work_func::5G channel alloc mem return null ptr!}");
        oal_free(pul_channels_2G);
        return;
    }

    /* 解析内核下发的扫描信道列表 */
    wal_set_scan_channel(pst_request, pul_channels_2G, &ul_num_channels_2G, pul_channels_5G, &ul_num_channels_5G);

    OAM_INFO_LOG3(uc_vap_id, OAM_SF_CFG, "{wal_scan_work_func::channel num is %d, 2.4G: %d, 5G: %d!}",
      ul_channel_num, ul_num_channels_2G, ul_num_channels_5G);

    if (ul_num_channels_2G > 0)
    {
        st_scan_param.uc_num_channels_2G = (oal_uint8)ul_num_channels_2G;
        st_scan_param.pul_channels_2G    = pul_channels_2G;
    }

    if (ul_num_channels_5G > 0)
    {
        st_scan_param.uc_num_channels_5G = (oal_uint8)ul_num_channels_5G;
        st_scan_param.pul_channels_5G    = pul_channels_5G;
    }

    /* 解析内核下发的ssid */
    wal_set_scan_ssid(pst_request, &st_scan_param);

    /* 解析内核下发的ie */
    st_scan_param.puc_ie    = pst_request->ie;
    st_scan_param.ul_ie_len = pst_request->ie_len;

    /* P2P WLAN/P2P 特性情况下，根据扫描的ssid 判断是否为p2p device 发起的扫描，
        ssid 为"DIRECT-"则认为是p2p device 发起的扫描 */
    /* 解析下发扫描的device 是否为p2p device(p2p0) */
    st_scan_param.bit_is_p2p0_scan      = OAL_FALSE;

    if ((pst_request->n_ssids > 0) && (NULL != pst_request->ssids)
        && (pst_request->ssids[0].ssid_len == OAL_STRLEN("DIRECT-"))
        && (0 == oal_memcmp(pst_request->ssids[0].ssid, "DIRECT-", OAL_STRLEN("DIRECT-"))))
    {
        st_scan_param.bit_is_p2p0_scan = OAL_TRUE;
    }

    /* 在事件前防止异步调度完成扫描后,发生同步问题 */
    pst_wiphy_mgmt->en_complete = OAL_FALSE;

    /* 抛事件，通知驱动启动扫描 */
    ul_ret = wal_cfg80211_start_scan(pst_netdev, &st_scan_param);
    if( OAL_SUCC != ul_ret)
    {
        OAM_ERROR_LOG1(0, OAM_SF_CFG, "{wal_scan_work_func::wal_cfg80211_start_scan proc failed, err_code[%d]!}", ul_ret);

        oal_free(pul_channels_2G);
        oal_free(pul_channels_5G);
        return;
    }

    /* win32 UT模式，触发一次事件调度 */
#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION) && (_PRE_TEST_MODE == _PRE_TEST_MODE_UT)
    frw_event_process_all_event(0);
#endif

    /* 启动定时器，进行扫描超时处理 */
    wal_start_timer_for_scan_timeout(uc_vap_id);

    return;
}
oal_int32 wal_send_scan_abort_msg(oal_net_device_stru   *pst_net_dev)
{
    wal_msg_write_stru              st_write_msg;
    oal_uint32                      ul_pedding_data = 0;       /* 填充数据，不使用，只是为了复用接口 */
    oal_int32                       l_ret = 0;

    /* 拋事件通知device侧终止扫描 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SCAN_ABORT, OAL_SIZEOF(ul_pedding_data));

    oal_memcopy(st_write_msg.auc_value, (oal_int8 *)&ul_pedding_data, OAL_SIZEOF(ul_pedding_data));

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(ul_pedding_data),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_SUCC != l_ret)
    {
        OAM_WARNING_LOG1(0, OAM_SF_SCAN, "{wal_send_scan_abort_msg::fail to stop scan, error[%d]}", l_ret);
    }

    return l_ret;
}


oal_int32 wal_force_scan_complete(oal_net_device_stru   *pst_net_dev,
                                         oal_bool_enum          en_is_aborted)
{
    mac_vap_stru            *pst_mac_vap;
    hmac_vap_stru           *pst_hmac_vap;
    hmac_device_stru        *pst_hmac_device;
    mac_device_stru         *pst_mac_device;
    oal_wiphy_mgmt_stru     *pst_wiphy_mgmt;

    pst_mac_vap  = OAL_NET_DEV_PRIV(pst_net_dev);

    /* 获取hmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (OAL_PTR_NULL == pst_hmac_device)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{wal_cfg80211_notify_scan_complete::pst_hmac_device is null, dev_id[%d]!}",
                         pst_mac_vap->uc_device_id);
        return -OAL_EINVAL;
    }

    pst_mac_device = pst_hmac_device->pst_device_base_info;
    pst_wiphy_mgmt = &(pst_mac_device->st_wiphy_mgmt);

    /* 判断扫描请求来自上层或者内部 */
    if (OAL_PTR_NULL == pst_wiphy_mgmt->pst_request)
    {
        /* 判断是否存在内部扫描，如果存在，也需要停止 */
        if ((OAL_TRUE == pst_hmac_device->st_scan_mgmt.en_is_scanning) &&
            (pst_mac_vap->uc_vap_id == pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.uc_vap_id))
        {
            OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                             "{wal_cfg80211_notify_scan_complete::may be internal scan, stop scan!}");
            /* 终止扫描 */
            wal_send_scan_abort_msg(pst_net_dev);
        }

        return OAL_SUCC;
    }

    /* stop的vap和正在扫描的vap不相同则直接返回 */
    if (pst_mac_vap->uc_vap_id != pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.uc_vap_id)
    {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{wal_cfg80211_notify_scan_complete::stop_vap[%d] is different scan_vap[%d]!}",
                         pst_mac_vap->uc_vap_id,
                         pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.uc_vap_id);
        return OAL_SUCC;
    }

    /* 获取hmac vap */
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (OAL_PTR_NULL == pst_hmac_vap)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{wal_cfg80211_notify_scan_complete::hmac_vap is null, vap_id[%d]!}", pst_mac_vap->uc_vap_id);
        return -OAL_EINVAL;
    }

    /* 删除等待扫描超时定时器 */
    if (OAL_TRUE == pst_hmac_vap->st_scan_timeout.en_is_registerd)
    {
        FRW_TIMER_IMMEDIATE_DESTROY_TIMER(&(pst_hmac_vap->st_scan_timeout));
    }

    /* 如果是上层下发的扫描请求，则通知内核扫描结束，内部扫描不需通知 */
    if (OAL_PTR_NULL != pst_wiphy_mgmt->pst_request)
    {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{wal_cfg80211_notify_scan_complete::vap_id[%d] notify kernel scan abort!}",
                         pst_mac_vap->uc_vap_id);

        /* 终止扫描 */
        wal_send_scan_abort_msg(pst_net_dev);

        /* 上报内核扫描结果 */
        wal_inform_all_bss(pst_hmac_device->pst_device_base_info->pst_wiphy,
                           &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt),
                           pst_mac_vap->uc_vap_id);

        /* 通知内核扫描终止 */
        oal_cfg80211_scan_done(pst_wiphy_mgmt->pst_request, en_is_aborted);
        pst_wiphy_mgmt->pst_request = OAL_PTR_NULL;
        pst_wiphy_mgmt->en_complete = OAL_TRUE;

        /* 让编译器优化时保证OAL_WAIT_QUEUE_WAKE_UP在最后执行 */
        OAL_SMP_MB();
        OAL_WAIT_QUEUE_WAKE_UP(&pst_wiphy_mgmt->st_wait_queue);
    }

    return OAL_SUCC;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

