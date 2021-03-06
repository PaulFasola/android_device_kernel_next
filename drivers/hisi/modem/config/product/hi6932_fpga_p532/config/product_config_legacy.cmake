# MD5: 7b1387c9ca835dcf4f9d4bfcffff89da
set(CFG_BALONG_OBUILD_VERSION 10) 

set(CFG_CONFIG_VERSION_STUB YES) 

set(CFG_HW_VERSION_STUB 0XFF000200) 

set(CFG_PXXX_BOARD_TYPE_BBIT			 YES) 

set(CFG_BBIT_VERSION                	 0X2) 

set(CFG_FEATURE_OBJ_CMP 				 NO) 

set(CFG_FEATURE_OBJ_DUMP				 NO) 

set(CFG_FTP_REPLY_TIME_OUT 0) 

set(CFG_PRODUCT_CONTROL_VERSION "121") 

set(CFG_PRODUCT_HISILICON_VERSION hi6932) 

set(CFG_PRODUCT_NV_SPLITFILE binfilesplit "0") 

set(CFG_PRODUCT_NV_FILE_MAX_SIZE binfileLimitSize "100") 

set(CFG_PRODUCT_NV_BIN_MODEM_NUNBER bin_modem_number "2") 

set(CFG_PRODUCT_NV_XNV_MODEM_NUNBER xnv_modem_number "2") 

set(CFG_PRODUCT_NV_XNV_PDT_VERSION hi6932_fpga_p532) 

set(CFG_XTENSA_CORE				 p532_bbe16_nocache) 

set(CFG_XTENSA_SYSTEM			 $(ROOT_XTENSA_PATH_W)/XtDevTools/install/builds/RE-2014.5-linux/p532_bbe16_nocache/config) 

set(CFG_TENSILICA_BUILDS	 $(ROOT_XTENSA_PATH_W)/XtDevTools/install/builds/RE-2014.5-linux) 

set(CFG_TENSILICA_TOOLS	 $(ROOT_XTENSA_PATH_W)/XtDevTools/install/tools/RE-2014.5-linux) 

set(LPHY_LD_MAP_PUB_PATH modem/phy/lt/Tensilica_LSP/BBE16_LSP_P532_V722/mainlsp) 

set(LPHY_LD_MAP_LTE_PATH modem/phy/lt/Tensilica_LSP/BBE16_LSP_P532_V722/lte_lsp) 

set(LPHY_LD_MAP_TDS_PATH modem/phy/lt/Tensilica_LSP/BBE16_LSP_P532_V722/tds_lsp) 

set(LPHY_LD_SDR_LIB_FILE	 BBE16_NOCACHE_P532_02) 

set(LPHY_XTENSA_RULES cc_tensilica5.0.5_rules.mk) 

set(CFG_LPHY_PUB_DTCM_BASE	 0x49000000) 

set(CFG_LPHY_PUB_ITCM_BASE	 0x49100000) 

set(CFG_LPHY_PRV_DTCM_BASE	 0x49050000) 

set(CFG_LPHY_PRV_ITCM_BASE	 0x49158000) 

set(CFG_LPHY_PUB_DTCM_SIZE		 0x50000) 

set(CFG_LPHY_PUB_ITCM_SIZE		 0x58000) 

set(CFG_LPHY_PRV_DTCM_SIZE		 0x80000) 

set(CFG_LPHY_PRV_ITCM_SIZE		 0x78000) 

set(CFG_LPHY_LTE_DTCM_SIZE		 0x80000) 

set(CFG_LPHY_LTE_ITCM_SIZE		 0x78000) 

set(CFG_LPHY_TDS_DTCM_SIZE		 0x30000) 

set(CFG_LPHY_TDS_ITCM_SIZE		 0x38000) 

set(CFG_LPHY_TOTAL_IMG_SIZE ((LPHY_PUB_DTCM_SIZE)+(LPHY_PUB_ITCM_SIZE)+(LPHY_LTE_DTCM_SIZE)*2+(LPHY_LTE_ITCM_SIZE)*2+(LPHY_TDS_DTCM_SIZE)+(LPHY_TDS_DTCM_SIZE))) 

set(CFG_TL_PHY_LALG_PATH    	 modem/include/phy) 

set(CFG_TL_PHY_FPGA_P532    	 YES) 

set(CFG_TL_PHY_BBE16_CACHE NO) 

set(CFG_TL_PHY_HI6932         	 YES) 

set(CFG_TL_PHY_FEATURE_LTE_LCS  	 YES) 

set(CFG_TL_PHY_RTT_COMM_DEBUG YES) 

set(CFG_FEATURE_TLPHY_SINGLE_XO		 YES) 

set(CFG_GU_INC_PATH COMM_CODE_GU/Balong_GU_Inc/GUL_CS) 

set(CFG_GU_PRODUCT_VERSION VERSION_V7R1) 

set(CFG_RAT_GU 0) 

set(CFG_RAT_GUL 1) 

set(CFG_RAT_MODE RAT_GUL) 

set(CFG_GU_RAT_MODE RAT_GUL) 

set(CFG_TTF_SKB_EXP NO) 

set(CFG_HIFI_INCLUDE NO) 

set(CFG_HIFI_LINK_DIR_NAME hifi_ld_V7R2_SFT) 

set(CFG_XTENSA_CORE_HIFI v7r2_hifi) 

set(CFG_PLATFORM_CHIP_TYPE p532) 

set(CFG_CHIP_MODEM_ARM_ARCH CORTEX_A9) 

set(CFG_CONFIG_FASTBOOT_UART_NUM 1) 

set(CFG_CONFIG_MODULE_VIC YES) 

set(CFG_CONFIG_AT_UART NO) 

set(CFG_CONFIG_CSHELL NO) 

set(CFG_CONFIG_ICC YES) 

set(CFG_CONFIG_OF YES) 

set(dts true) 

set(CFG_CONFIG_CPUFREQ NO) 

set(CFG_CONFIG_CCORE_REGULATOR YES) 

set(CFG_CONFIG_CCORE_WDT YES) 

set(CFG_CONFIG_ACORE_WDT YES) 

set(CFG_CONFIG_PMIC_HI6551 YES) 

set(CFG_CONFIG_PMIC_FPGA YES) 

set(CFG_CONFIG_COUL NO) 

set(CFG_CONFIG_SPI NO) 

set(CFG_CONFIG_SFLASH NO) 

set(CFG_CONFIG_ABB YES) 

set(CFG_CONFIG_DSP YES) 

set(CFG_CONFIG_HIFI NO) 

set(CFG_CONFIG_HKADC YES) 

set(CFG_CONFIG_ONOFF YES) 

set(CFG_CONFIG_BBP YES) 

set(CFG_CONFIG_BBP_ON YES) 

set(CFG_CONFIG_PA_RF NO) 

set(CFG_CONFIG_PASTAR NO) 

set(CFG_CONFIG_MIPI NO) 

set(CFG_ENABLE_SHELL_SYM YES) 

set(CFG_ENABLE_DEBUG YES) 

set(CFG_CONFIG_BSP_TEST_CCORE NO) 

set(CFG_CONFIG_BALONG_CCLK NO) 

set(CFG_CONFIG_CCORE_PM NO) 

set(CFG_CONFIG_MODULE_IPC YES) 

set(CFG_CONFIG_IPCM_USE_FPGA_VIC YES) 

set(CFG_CONFIG_MODULE_TIMER YES) 

set(CFG_CONFIG_MODULE_SYNC YES) 

set(CFG_CONFIG_LEDS_CCORE NO) 

set(CFG_CONFIG_HAS_CCORE_WAKELOCK NO) 

set(CFG_CONFIG_CCORE_BALONG_PM NO) 

set(CFG_CONFIG_BALONG_EDMA YES) 

set(CFG_CONFIG_BALONG_EDMA_TEST YES) 

set(CFG_CONFIG_IPF_VESION 2) 

set(CFG_CONFIG_IPF_ADQ_LEN 3) 

set(CFG_CONFIG_IPF_BACKUP_ON_LPM3 YES) 

set(CFG_CONFIG_CCORE_I2C NO) 

set(CFG_TL_PS_MAX_TRCH_NUM_8 YES) 

set(CFG_FEATURE_ON 1) 

set(CFG_FEATURE_OFF 0) 

set(CFG_MEMORY_SIZE_32M 1) 

set(CFG_MEMORY_SIZE_64M 2) 

set(CFG_MEMORY_SIZE_128M 3) 

set(CFG_MEMORY_SIZE_256M 4) 

set(CFG_MEMORY_SIZE_512M 5) 

set(CFG_PS_PTL_VER_DANAMIC 20) 

set(CFG_FEATURE_CS FEATURE_ON) 

set(CFG_FEATURE_GFAX FEATURE_OFF) 

set(CFG_FEATURE_WFAX FEATURE_OFF) 

set(CFG_BALONG_UDP_V1R1 FEATURE_OFF) 

set(CFG_BALONG_UDP_V1R2 FEATURE_OFF) 

set(CFG_FEATURE_PTABLE_UDP FEATURE_ON) 

set(CFG_FEATURE_STICK								 FEATURE_OFF) 

set(CFG_FEATURE_E5 FEATURE_OFF) 

set(CFG_FEATURE_E5_UDP FEATURE_OFF) 

set(CFG_FEATURE_HILINK FEATURE_OFF) 

set(CFG_FEATURE_SEC_BOOT FEATURE_OFF) 

set(CFG_FEATURE_PPPOE FEATURE_OFF) 

set(CFG_FEATURE_MERGE_OM_CHAN						 FEATURE_ON) 

set(CFG_FEATURE_UPDATEONLINE FEATURE_OFF) 

set(CFG_FEATURE_SDIO FEATURE_OFF) 

set(CFG_FEATURE_KEYBOARD FEATURE_OFF) 

set(CFG_FEATURE_CHARGE FEATURE_OFF) 

set(CFG_FEATURE_ICC_DEBUG FEATURE_ON) 

set(CFG_FEATURE_OLED FEATURE_OFF) 

set(CFG_FEATURE_TFT FEATURE_OFF) 

set(CFG_FEATURE_MMI_TEST FEATURE_OFF) 

set(CFG_FEATURE_INTERPEAK FEATURE_OFF) 

set(CFG_FEATURE_INTERPEAK_MINI FEATURE_OFF) 

set(CFG_IPWEBS_FEATURE_E5 FEATURE_OFF) 

set(CFG_IPWEBS_FEATURE_WIRELESS_DONGLE FEATURE_OFF) 

set(CFG_IPWEBS_FEATURE_LIGHTNING_CARD FEATURE_OFF) 

set(CFG_FEATURE_WEBNAS FEATURE_OFF) 

set(CFG_FEATURE_WIRELESS_DONGLE FEATURE_OFF) 

set(CFG_FEATURE_PMU_PROTECT FEATURE_OFF) 

set(CFG_FEATURE_VXWORKS_TCPIP FEATURE_OFF) 

set(CFG_FEATURE_M2 FEATURE_OFF) 

set(CFG_FEATURE_MEMORY_SIZE MEMORY_SIZE_64M) 

set(CFG_FEATURE_ECM_RNDIS FEATURE_OFF) 

set(CFG_FEATURE_BREATH_LIGHT FEATURE_OFF) 

set(CFG_FEATURE_LEDSTATUS FEATURE_OFF) 

set(CFG_FEATURE_E5_LED FEATURE_OFF) 

set(CFG_FEATURE_RTC FEATURE_OFF) 

set(CFG_FEATURE_RECONFIG FEATURE_ON) 

set(CFG_FEATURE_HWENCRY_REWORK FEATURE_OFF) 

set(CFG_FEATURE_MMU_BIG FEATURE_OFF) 

set(CFG_FEATURE_TTFMEM_CACHE FEATURE_OFF) 

set(CFG_FEATURE_SDUPDATE FEATURE_OFF) 

set(CFG_FEATURE_EPAD FEATURE_OFF) 

set(CFG_FEATURE_HSIC_SLAVE FEATURE_OFF) 

set(CFG_FEATURE_MANUFACTURE_LOG FEATURE_OFF) 

set(CFG_FEATURE_MEM_MONITOR FEATURE_ON) 

set(CFG_FEATURE_DRV_REPLAY_DUMP						 FEATURE_OFF) 

set(CFG_FEATURE_SKB_EXP FEATURE_OFF) 

set(CFG_FEATURE_SOCP_CHANNEL_REDUCE FEATURE_OFF) 

set(CFG_FEATURE_SOCP_DECODE_INT_TIMEOUT FEATURE_ON) 

set(CFG_FEATURE_RTOSCK FEATURE_ON) 

set(CFG_FEATURE_PTM FEATURE_OFF) 

set(CFG_CONFIG_HISI_PTM FEATURE_OFF) 

set(CFG_FEATURE_COMPRESS_WRITE_LOG_FILE FEATURE_OFF) 

set(CFG_FEATURE_CBT_LOG FEATURE_OFF) 

set(CFG_FEATURE_IMS FEATURE_OFF) 

set(CFG_FEATURE_SAMPLE_LTE_CHAN 			 FEATURE_OFF) 

set(CFG_FEATURE_TTF_RLC_PIGGY_BACKED FEATURE_OFF) 

set(CFG_FEATURE_W_R99_DL_DUAL_MAILBOX FEATURE_ON) 

set(CFG_FEATURE_ASSEMBLY_MEM_CPY FEATURE_ON) 

set(CFG_FEATURE_HSPA_PERF_IMP FEATURE_ON) 

set(CFG_FEATURE_RACH_NO_ACK_DEBUG FEATURE_OFF) 

set(CFG_FEATURE_RLC_REASSEMBLE FEATURE_ON) 

set(CFG_FEATURE_CIPHER_MASTER FEATURE_ON) 

set(CFG_FEATURE_HDLC_ENHANCE FEATURE_ON) 

set(CFG_FEATURE_EDMAC FEATURE_OFF) 

set(CFG_FEATURE_POWER_DRV FEATURE_OFF) 

set(CFG_FEATURE_SIM_NOTIFY FEATURE_OFF) 

set(CFG_FEATURE_HW_CODEC FEATURE_OFF) 

set(CFG_FEATURE_ANT_SHARE				 FEATURE_OFF) 

set(CFG_FEATURE_BSP_LCH_OM FEATURE_ON) 

set(CFG_FEATURE_UE_MODE_CDMA						 FEATURE_OFF) 

set(CFG_FEATURE_NV_PARTRION_MULTIPLEX FEATURE_ON) 

set(CFG_FEATURE_LTE_R11 FEATURE_ON) 

set(CFG_FEATURE_LTE_MBMS FEATURE_OFF) 

set(CFG_FEATURE_LTE_NAS_R11 FEATURE_OFF) 

set(CFG_FEATURE_LPP FEATURE_OFF) 

set(CFG_FEATURE_LTE_CDMA_R11 FEATURE_OFF) 

set(CFG_FEATURE_LPHY_RFIC_HI6362 FEATURE_ON) 

set(CFG_FEATURE_BALONG_CL FEATURE_UE_MODE_CDMA) 

