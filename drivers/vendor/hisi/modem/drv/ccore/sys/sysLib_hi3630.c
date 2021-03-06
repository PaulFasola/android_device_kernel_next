/* sysLib.c - ARM RealView PBX-A9 board system-dependent routines */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,20oct09,z_l  created
*/

/*
DESCRIPTION
This library provides board-specific routines for the
ARM RealView PBX-A9 board BSP
*/

/* includes */

#include <vxWorks.h>
#include "config.h"
#include <sysLib.h>
#include <string.h>
#include <intLib.h>
#include <taskLib.h>
#include <vxLib.h>
#include <muxLib.h>
#include <cacheLib.h>

#include "product_config.h"
#include "bsp_memmap.h"
#include <soc_interrupts.h>
#include "sysTimer.h"
#include "bsp_abb.h"
#include "hi_dsp.h"
#include "osl_bio.h"

#include "MemoryMap.h"
#include "bsp_shared_ddr.h"

#ifdef INCLUDE_MMU
#   include <arch/arm/mmuArmLib.h>
#   include <private/vmLibP.h>
#endif /* INCLUDE_MMU */

#ifdef INCLUDE_VXIPI
#   include <vxIpiLib.h>
#endif /* INCLUDE_VXIPI */
IMPORT void sysClkInt (void);
IMPORT void sysAuxClkInt (void);
IMPORT void sysSerialHwInit(void);
IMPORT void sysSerialHwInit2(void);
IMPORT void armGicCtlInit(void);

#ifdef INCLUDE_SIO_UTILS
IMPORT void sysSerialConnectAll (void);
#endif /* INCLUDE_SIO_UTILS */

#ifdef INCLUDE_CACHE_SUPPORT
IMPORT void cacheCortexA9LibInstall (VIRT_ADDR(physToVirt) (PHYS_ADDR),
                                     PHYS_ADDR(virtToPhys) (VIRT_ADDR));
#endif

#ifdef INCLUDE_MMU
IMPORT void mmuCortexA8LibInstall (VIRT_ADDR(physToVirt) (PHYS_ADDR),
                                       PHYS_ADDR(virtToPhys) (VIRT_ADDR));
#endif

#ifdef _WRS_CONFIG_SMP
#   include <arch/arm/vxAtomicArchLib.h>
#endif /* _WRS_CONFIG_SMP */

/* imports */

#ifndef _ARCH_SUPPORTS_PROTECT_INTERRUPT_STACK
IMPORT VOIDFUNCPTR _func_armIntStackSplit;  /* ptr to fn to split stack */
#endif /* !_ARCH_SUPPORTS_PROTECT_INTERRUPT_STACK */

#define PBXA9_GIC1_BASE  HI_MDM_GIC_BASE_ADDR

/* globals */
#ifdef INCLUDE_MMU

/*
 * The following structure describes the various different parts of the
 * memory map to be used only during initialization by
 * vm(Base)GlobalMapInit() when INCLUDE_MMU_BASIC/FULL/GLOBAL_MAP are
 * defined.
 *
 * Clearly, this structure is only needed if the CPU has an MMU!
 *
 * The following are not the smallest areas that could be allocated for a
 * working system. If the amount of memory used by the page tables is
 * critical, they could be reduced.
 */

PHYS_MEM_DESC sysPhysMemDesc [] =
    {
    /* DRAM - Always the first entry */

    {
    DDR_MCORE_ADDR,    /* virtual address */
    DDR_MCORE_ADDR,    /* physical address */
    ROUND_UP (DDR_MCORE_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_WRITEALLOCATE_MSK,
#ifdef _WRS_CONFIG_SMP       /* needs to be shared */
    MMU_ATTR_VALID       | MMU_ATTR_SUP_RWX  | MMU_ATTR_WRITEALLOCATE_SHARED
#else
    MMU_ATTR_VALID       | MMU_ATTR_SUP_RWX  | MMU_ATTR_WRITEALLOCATE
#endif /* _WRS_CONFIG_SMP */
    },
	/**************************************GU   memmap   begin**************************/
	{/*GU reserved mem 1xxx*/
    DDR_GU_ADDR,    /* virtual address */
    DDR_GU_ADDR,    /* physical address */
    ROUND_UP (ECS_BBPHAC_BASE_ADDR - DDR_GU_ADDR, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_NORMAL_NONCACHEABLE_MSK,
    MMU_ATTR_VALID       | MMU_ATTR_SUP_RWX  | MMU_ATTR_NORMAL_NONCACHEABLE
    },
	{/*GU reserved mem 2xxx*/
    ECS_ARM_DSP_BUFFER_BASE_ADDR,    /* virtual address */
    ECS_ARM_DSP_BUFFER_BASE_ADDR,    /* physical address */
    ROUND_UP (ECS_NV_BASE_ADDR - ECS_ARM_DSP_BUFFER_BASE_ADDR, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_NORMAL_NONCACHEABLE_MSK,
    MMU_ATTR_VALID       | MMU_ATTR_SUP_RWX  | MMU_ATTR_NORMAL_NONCACHEABLE
    },
    {/*GU reserved mem 3xxx  ʣ�µ�����*/
    ECS_TTF_DICC_ADDR,    /* virtual address */
    ECS_TTF_DICC_ADDR,    /* physical address */
    ROUND_UP (DDR_GU_ADDR + DDR_GU_SIZE - ECS_TTF_DICC_ADDR , PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_NORMAL_NONCACHEABLE_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_NORMAL_NONCACHEABLE
    },
	/**************************************GU   memmap   end**************************/
	{
	DDR_TLPHY_IMAGE_ADDR,
	DDR_TLPHY_IMAGE_ADDR,
	ROUND_UP(DDR_TLPHY_IMAGE_SIZE, PAGE_SIZE),
	MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
	MMU_ATTR_VALID | MMU_ATTR_SUP_RWX | MMU_ATTR_STRONGLY_ORDERED
    },
	{
	DDR_LPHY_SDR_ADDR,
	DDR_LPHY_SDR_ADDR,
	ROUND_UP(DDR_LPHY_SDR_SIZE, PAGE_SIZE),
	MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
	MMU_ATTR_VALID | MMU_ATTR_SUP_RWX | MMU_ATTR_STRONGLY_ORDERED
    },
	{
	DDR_LCS_ADDR,
	DDR_LCS_ADDR,
	ROUND_UP(DDR_LCS_SIZE, PAGE_SIZE),
	MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
	MMU_ATTR_VALID | MMU_ATTR_SUP_RWX | MMU_ATTR_STRONGLY_ORDERED
    },
    {
	BBE_TCM_ADDR,
	BBE_TCM_ADDR,
	ROUND_UP(BBE_TCM_SIZE, PAGE_SIZE),
	MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
	MMU_ATTR_VALID | MMU_ATTR_SUP_RWX | MMU_ATTR_STRONGLY_ORDERED
    },
	{
	DDR_MNTN_ADDR,
	DDR_MNTN_ADDR,
	ROUND_UP(DDR_MNTN_SIZE, PAGE_SIZE),
	MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_NORMAL_NONCACHEABLE_MSK,
	MMU_ATTR_VALID | MMU_ATTR_SUP_RWX | MMU_ATTR_NORMAL_NONCACHEABLE
    },
	{
	DDR_SHARED_MEM_ADDR,
	DDR_SHARED_MEM_ADDR,
	ROUND_UP(DDR_SHARED_MEM_SIZE, PAGE_SIZE),
	MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
	MMU_ATTR_VALID | MMU_ATTR_SUP_RWX | MMU_ATTR_DEVICE_SHARED
    },
    /*
     * I/O space:
     * Do not map in all I/O space, only that which has something there.
     * Otherwise we will use all of RAM allocating page tables!
     */
	{
    HI_MDM_GIC_BASE_ADDR_VIRT, /* MPCore functions including SCU */
    HI_MDM_GIC_BASE_ADDR,
    ROUND_UP (HI_MDM_GIC_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
    {
    HI_SYSCTRL_BASE_ADDR_VIRT,    /* system registers */
    HI_SYSCTRL_BASE_ADDR,
    ROUND_UP (HI_SYSCTRL_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
    /*begin:K3V3 ap sysctrl reg*/
    #ifdef HI_AP_SYSCTRL_BASE_ADDR
    {
    HI_AP_SYSCTRL_BASE_ADDR_VIRT,    /* ap system registers */
    HI_AP_SYSCTRL_BASE_ADDR,
    ROUND_UP (HI_AP_SYSCTRL_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_LP_PERI_CRG_REG_ADDR,    /* ap system registers */
    HI_LP_PERI_CRG_REG_ADDR,
    ROUND_UP (HI_LP_PERI_CRG_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
    #endif
    /*end:K3V3 ap sysctrl reg*/
    {
    HI_WDT_BASE_ADDR_VIRT,    /* wdt*/
    HI_WDT_BASE_ADDR,
    ROUND_UP (HI_WDT_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
    {
    HI_UART0_REGBASE_ADDR_VIRT,  /* PrimeCell UART */
    HI_UART0_REGBASE_ADDR,
    ROUND_UP (HI_UART0_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_UART5_REGBASE_ADDR_VIRT,  /* PrimeCell UART5 */
    HI_UART5_REGBASE_ADDR,
    ROUND_UP (HI_UART5_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
    {
    HI_IPC_S_REGBASE_ADDR_VIRT,  /* PrimeCell UART */
    HI_IPC_S_REGBASE_ADDR,
    ROUND_UP (HI_IPC_S_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_RTC_REGBASE_ADDR_VIRT,  /* RTC */
    HI_RTC0_BASE_ADDR,
    ROUND_UP (HI_RTC0_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_EDMA_REGBASE_ADDR_VIRT,  /* EDMA */
    HI_EDMA_REGBASE_ADDR,
	ROUND_UP (HI_EDMA_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_CICOM0_REGBASE_ADDR_VIRT,    /* cicom0*/
    HI_CICOM0_REGBASE_ADDR,
    ROUND_UP (HI_CICOM0_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_CICOM1_REGBASE_ADDR_VIRT,    /* cicom1*/
    HI_CICOM1_REGBASE_ADDR,
    ROUND_UP (HI_CICOM1_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_CIPHER_BASE_ADDR_VIRT,
    HI_CIPHER_BASE_ADDR,
	ROUND_UP (HI_CIPHER_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_UPACC_BASE_ADDR_VIRT,    /* upacc*/
    HI_UPACC_BASE_ADDR,
    ROUND_UP (HI_UPACC_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_IPF_REGBASE_ADDR_VIRT,
    HI_IPF_REGBASE_ADDR,
	ROUND_UP (HI_IPF_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_IPCM_REGBASE_ADDR_VIRT,  /* IPCM */
    HI_IPCM_REGBASE_ADDR,
	ROUND_UP (HI_IPCM_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
	HI_SRAM_MEM_ADDR_VIRT,
	HI_SRAM_MEM_BASE_ADDR,
	ROUND_UP(HI_SRAM_MEM_SIZE, PAGE_SIZE),
	MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
	MMU_ATTR_VALID | MMU_ATTR_SUP_RWX | MMU_ATTR_DEVICE_SHARED
    },
	/* dsp subsystem, devided into 5 region*/
	{
    HI_DSP_SUBSYSTEM_BASE_ADDR_VIRT, /* region1: [HI_DSP_SUBSYSTEM_BASE_ADDR, HI_GBBP_REG_BASE_ADDR] */
    HI_DSP_SUBSYSTEM_BASE_ADDR,
	ROUND_UP ((HI_GBBP_REG_BASE_ADDR - HI_DSP_SUBSYSTEM_BASE_ADDR), PAGE_SIZE),
	MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
	MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
	},
	{
	HI_GBBP_REG_BASE_ADDR_VIRT,      /* region2: [HI_GBBP_REG_BASE_ADDR, HI_GBBP_REG_BASE_ADDR + SZ_4K] */
	HI_GBBP_REG_BASE_ADDR,
	ROUND_UP (SZ_4K, PAGE_SIZE),
	MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_NORMAL_NONCACHEABLE_MSK | MMU_ATTR_DEVICE_NONSHARED_MSK,
	MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_NORMAL_NONCACHEABLE | MMU_ATTR_DEVICE_NONSHARED
	},
	{
	(HI_GBBP_REG_BASE_ADDR + SZ_4K), /* region3: [HI_GBBP_REG_BASE_ADDR + SZ_4K, HI_GBBP1_REG_BASE_ADDR] */
	(HI_GBBP_REG_BASE_ADDR + SZ_4K),
	ROUND_UP ((HI_GBBP1_REG_BASE_ADDR - HI_GBBP_REG_BASE_ADDR - SZ_4K), PAGE_SIZE),
	MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
	MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
	},
	{
	(HI_GBBP1_REG_BASE_ADDR_VIRT), /* region4: [HI_GBBP1_REG_BASE_ADDR, HI_GBBP1_REG_BASE_ADDR + SZ_4K] */
	(HI_GBBP1_REG_BASE_ADDR),
	ROUND_UP (SZ_4K, PAGE_SIZE),
	MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_NORMAL_NONCACHEABLE_MSK | MMU_ATTR_DEVICE_NONSHARED_MSK,
	MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_NORMAL_NONCACHEABLE | MMU_ATTR_DEVICE_NONSHARED
	},
	{
	(HI_GBBP1_REG_BASE_ADDR + SZ_4K), /* region5: [HI_GBBP1_REG_BASE_ADDR + SZ_4K, HI_DSP_SUBSYSTEM_BASE_ADDR + HI_DSP_SUBSYSTEM_SIZE] */
	(HI_GBBP1_REG_BASE_ADDR + SZ_4K),
	ROUND_UP ((HI_DSP_SUBSYSTEM_BASE_ADDR + HI_DSP_SUBSYSTEM_SIZE - HI_GBBP1_REG_BASE_ADDR - SZ_4K), PAGE_SIZE),
	MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
	MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
	},
	/* end of dsp subsystem region */
	{
    HI_BBP_TSTU_BASE_ADDR_VIRT,/*BBP TDS INT*/
    HI_BBP_TSTU_BASE_ADDR,
    ROUND_UP (HI_BBP_TSTU_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_TIMER_00TO07_REGBASE_ADDR_VIRT,    /* timer 0~7 */
    HI_TIMER_00TO07_REGBASE_ADDR,
    ROUND_UP (HI_TIMER_00TO07_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_TIMER_08_REGBASE_ADDR_VIRT,    /* timer 08 */
    HI_TIMER_08_REGBASE_ADDR,
    ROUND_UP (K3_TIMER_ADDR_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
   {
    HI_TIMER_10_REGBASE_ADDR_VIRT,    /* timer 10 */
    HI_TIMER_10_REGBASE_ADDR,
    ROUND_UP (K3_TIMER_ADDR_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_TIMER_12_REGBASE_ADDR_VIRT,    /* timer 12 */
    HI_TIMER_12_REGBASE_ADDR,
    ROUND_UP (K3_TIMER_ADDR_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_TIMER_14_REGBASE_ADDR_VIRT,    /* timer 14 */
    HI_TIMER_14_REGBASE_ADDR,
    ROUND_UP (K3_TIMER_ADDR_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_TIMER_16_REGBASE_ADDR_VIRT,    /* timer 16 */
    HI_TIMER_16_REGBASE_ADDR,
    ROUND_UP (K3_TIMER_ADDR_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_TIMER_18_REGBASE_ADDR_VIRT,    /* timer 18 */
    HI_TIMER_18_REGBASE_ADDR,
    ROUND_UP (K3_TIMER_ADDR_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_TIMER_20_REGBASE_ADDR_VIRT,    /* timer 20 */
    HI_TIMER_20_REGBASE_ADDR,
    ROUND_UP (K3_TIMER_ADDR_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_TIMER_22_REGBASE_ADDR_VIRT,    /* timer 22 */
    HI_TIMER_22_REGBASE_ADDR,
    ROUND_UP (K3_TIMER_ADDR_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_PCTRL_REGBASE_ADDR_VIRT,    /* PCTRL */
    HI_PCTRL_REGBASE_ADDR,
    ROUND_UP (HI_PCTRL_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_PINTRL_REG_ADDR,            /* pintrl */
    HI_PINTRL_REG_ADDR,
    ROUND_UP (HI_PINTRL_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_PINTRL_SYS_REG_ADDR,        /* pintrl */
    HI_PINTRL_SYS_REG_ADDR,
    ROUND_UP (HI_PINTRL_SYS_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_GPIO10_REGBASE_ADDR,    /* gpio10-19 */
    HI_GPIO10_REGBASE_ADDR,
    ROUND_UP (HI_GPIO10_REG_SIZE*10, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_GPIO22_REGBASE_ADDR,    /* gpio22-24 */
    HI_GPIO22_REGBASE_ADDR,
    ROUND_UP (HI_GPIO22_REG_SIZE*3, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_SCI0_REGBASE_ADDR,  /* PrimeCell SCI0 */
    HI_SCI0_REGBASE_ADDR,
	ROUND_UP (HI_SCI0_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },

	{
    HI_SCI1_REGBASE_ADDR,  /* PrimeCell SCI1 */
    HI_SCI1_REGBASE_ADDR,
	ROUND_UP (HI_SCI1_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
#ifdef HI_AP_SYS_CNT_BASE_ADDR_VIRT
		{
		HI_AP_SYS_CNT_BASE_ADDR_VIRT,	/* sys cnt*/
		HI_AP_SYS_CNT_BASE_ADDR,
		ROUND_UP (HI_AP_SYS_CNT_SIZE, PAGE_SIZE),
		MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
		MMU_ATTR_VALID	   | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
		},
#endif
	{
    HI_MDDRC_REGBASE_ADDR_VIRT,
    HI_MDDRC_REGBASE_ADDR,
	ROUND_UP (HI_MDDRC_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_SOCP_REGBASE_ADDR_VIRT,  /* SOCP */
    HI_SOCP_REGBASE_ADDR,
	ROUND_UP (HI_SOCP_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_AMON_SOC_REGBASE_ADDR_VIRT,  /* AMON */
    HI_AMON_SOC_REGBASE_ADDR,
	ROUND_UP (HI_AMON_SOC_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_BBP_APB_REGBASE_ADDR_VIRT,
    HI_BBP_APB_REGBASE_ADDR,
	ROUND_UP (HI_BBP_APB_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_M3TCM0_MEM_ADDR,  /* LPM3 TCM */
    HI_M3TCM0_MEM_ADDR,
	ROUND_UP (HI_M3TCM0_MEM_SIZE + HI_M3TCM1_MEM_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },   
	{
    DDR_SOCP_ADDR,    
    DDR_SOCP_ADDR,    
    ROUND_UP (DDR_SOCP_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_NORMAL_NONCACHEABLE_MSK,
    MMU_ATTR_VALID       | MMU_ATTR_SUP_RWX  | MMU_ATTR_NORMAL_NONCACHEABLE
    },    
	{
    HI_CORESIGHT_PTM1_BASE_ADDR,    /* coresight modem ptm */
    HI_CORESIGHT_PTM1_BASE_ADDR,
    ROUND_UP (HI_CORESIGHT_PTM1_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_CORESIGHT_FUNNEL_BASE_ADDR,    /* coresight modem funnel */
    HI_CORESIGHT_FUNNEL_BASE_ADDR,
    ROUND_UP (HI_CORESIGHT_FUNNEL_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
	{
    HI_MDMA9_L2_REGBASE_ADDR_VIRT,    /* L2CACHE */
    HI_MDMA9_L2_REGBASE_ADDR,
    ROUND_UP (HI_MDMA9_L2_REG_SIZE, PAGE_SIZE),
    MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_DEVICE_SHARED_MSK,
    MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | MMU_ATTR_STRONGLY_ORDERED
    },
};

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);

#endif /* INCLUDE_MMU */

int    sysBus      = BUS_TYPE_NONE;            /* system bus type (VME_BUS, etc) */
int    sysCpu      = CPU;            /* system CPU type (e.g. ARMARCH6) */
char * sysBootLine = BOOT_LINE_ADRS; /* address of boot line */
char * sysExcMsg   = EXC_MSG_ADRS;   /* catastrophic message area */
int    sysProcNum;                   /* processor number of this CPU */

/* locals */

#ifdef _WRS_CONFIG_SMP

/* Non-Boot CPU Start info. Managed by sysCpuEnable */

struct sysMPCoreStartup
    {
    UINT32      newPC;          /* Address of 'C' based startup code */
    UINT32      newSP;          /* Stack pointer for startup */
    UINT32      newArg;         /* vxWorks kernel entry point */
    UINT32      newSync;        /* Translation Table Base and sync */
    };

extern struct sysMPCoreStartup sysMPCoreStartup[VX_SMP_NUM_CPUS];

#endif /* _WRS_CONFIG_SMP */

/* externals */

#ifndef _ARCH_SUPPORTS_PROTECT_INTERRUPT_STACK
IMPORT void sysIntStackSplit (char *, long);
#endif /* !_ARCH_SUPPORTS_PROTECT_INTERRUPT_STACK */

/* globals */

/* forward declarations */

#ifdef _WRS_CONFIG_SMP
IMPORT void   mmuCortexA8TtbrSet (MMU_LEVEL_1_DESC *);
IMPORT void   mmuCortexA8DacrSet (UINT32 dacrVal);
IMPORT void   mmuCortexA8AcrSet (UINT32 acrVal);
IMPORT STATUS sysArmGicDevInit(void);
IMPORT void   mmuCortexA8AEnable (UINT32 cacheState);
IMPORT void   mmuCortexA8ADisable ();
IMPORT void armInitExceptionModes (void);
IMPORT void sysMPCoreApStart(void);
IMPORT void sysMPCoreApResetLoop(void);
IMPORT void cacheCortexA9MPCoreSMPInit(void);
IMPORT void mmuCortexA8TLBIDFlushAll(void);
IMPORT MMU_LEVEL_1_DESC * mmuCortexA8TtbrGet ( void);
UINT32 sysCpuAvailableGet(void);
STATUS sysCpuEnable(unsigned int, void (*startFunc) (void), char *);
#endif /* _WRS_CONFIG_SMP */

/* included source files */

#ifdef INCLUDE_FLASH
#   include "flashMem.c"
#   include "nvRamToFlash.c"
#else /* INCLUDE_FLASH */
#   include <mem/nullNvRam.c>
#endif /* INCLUDE_FLASH */

#ifdef INCLUDE_RTC
STATUS sysRtcInit (void);
#   include "primeCellRtcPL031.c"
#endif /* INCLUDE_RTC */

#ifdef INCLUDE_L2_CACHE
#   include "sysL2Cache.c"
#endif /* INCLUDE_L2_CACHE */

#ifdef INCLUDE_VFP
extern void vfpEnable(void);
#endif
extern long ftpReplyTimeout;

/*******************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* RETURNS: A pointer to a string identifying the board and CPU.
*
* ERRNO: N/A
*/

char *sysModel (void)
    {
    return SYS_MODEL;
    }

/*******************************************************************************
*
* sysBspRev - return the BSP version with the revision eg 2.0/<x>
*
* This function returns a pointer to a BSP version with the revision.
* e.g. 2.0/<x>. BSP_REV is concatenated to BSP_VERSION to form the
* BSP identification string.
*
* RETURNS: A pointer to the BSP version/revision string.
*
* ERRNO: N/A
*/

char * sysBspRev (void)
    {
    return (BSP_VERSION BSP_REV);
    }

/*******************************************************************************
*
* sysHwInit0 - perform early BSP-specific initialization
*
* This routine performs such BSP-specific initialization as is necessary before
* the architecture-independent cacheLibInit can be called. It is called
* from usrInit() before cacheLibInit(), before sysHwInit() and before BSS
* has been cleared.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysHwInit0 (void)
    {

#ifdef INCLUDE_CACHE_SUPPORT
    /*
     * Install the appropriate cache library, no address translation
     * routines are required for this BSP, as the default memory map has
     * virtual and physical addresses the same.
     */

    cacheCortexA9LibInstall (mmuPhysToVirt, mmuVirtToPhys);

#ifdef INCLUDE_L2_CACHE
	if(*(u32 *)(SHM_BASE_ADDR+SHM_OFFSET_MEM_CCORE_L2CACHE)){
		sysL2CacheInit();
	}
#endif /* INCLUDE_L2_CACHE */

#endif /* INCLUDE_CACHE_SUPPORT */

#ifdef INCLUDE_MMU

    /* Install the appropriate MMU library and translation routines */

    mmuCortexA8LibInstall (mmuPhysToVirt, mmuVirtToPhys);

#endif /* defined(INCLUDE_MMU) */
#ifdef FTP_REPLY_TIME_OUT
    ftpReplyTimeout = FTP_REPLY_TIME_OUT;
#endif
    return;
    }


/*******************************************************************************
*
* sysHwInit - initialize the CPU board hardware
*
* This routine initializes various features of the hardware.
* Normally, it is called from usrInit() in usrConfig.c.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/
void sysHwInit (void)
    {
    dmesg_init();
    /* install the IRQ/SVC interrupt stack splitting routine */

#ifndef _ARCH_SUPPORTS_PROTECT_INTERRUPT_STACK
    _func_armIntStackSplit = sysIntStackSplit;
#endif    /* !_ARCH_SUPPORTS_PROTECT_INTERRUPT_STACK */

#if defined(INCLUDE_TTY_DEV) || defined(INCLUDE_SIO_POLL)
    sysSerialHwInit ();
#endif

#ifdef  FORCE_DEFAULT_BOOT_LINE
    strncpy(sysBootLine, DEFAULT_BOOT_LINE, strlen(DEFAULT_BOOT_LINE)+1);
#endif /* FORCE_DEFAULT_BOOT_LINE */
    }

/*******************************************************************************
*
* sysHwInit2 - additional system configuration and initialization
*
* This routine connects system interrupts and does any additional
* configuration necessary. Note that this is called from
* sysClkConnect() in the timer driver.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysHwInit2 (void)
    {
    static BOOL initialized = FALSE;
//    volatile UINT32 tempData;

    if (initialized)
        {
        return;
        }
#ifdef _WRS_CONFIG_SMP
    /* enable the snoop control unit */

    *(UINT32 *)(PBXA9_SCU_CTRL) = PBXA9_SCU_CTRL_EN;
#endif /* _WRS_CONFIG_SMP */

    intLibInit(INT_LVL_MAX,INT_LVL_MAX,INT_MODE);
    armGicCtlInit();

    (void)intConnect ((VOIDFUNCPTR*)INT_LVL_SYSTIMER, sysClkInt, 0);

	//(void)intConnect (INUM_TO_IVEC (AUX_TIMER_INT_LVL), sysAuxClkInt, 0);

#ifdef	INCLUDE_TTY_DEV
        sysSerialHwInit2 ();
#endif

    initialized = TRUE;

    }

/*******************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
*
* Normally, the user specifies the amount of physical memory with the
* macro LOCAL_MEM_SIZE in config.h.  BSPs that support run-time
* memory sizing do so only if the macro LOCAL_MEM_AUTOSIZE is defined.
* If not defined, then LOCAL_MEM_SIZE is assumed to be, and must be, the
* true size of physical memory.
*
* NOTE: Do no adjust LOCAL_MEM_SIZE to reserve memory for application
* use. See sysMemTop() for more information on reserving memory.
*
* RETURNS: The address of the top of physical memory.
*
* ERRNO: N/A
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop (void)
    {
    static char * physTop = NULL;

    if (physTop == NULL)
    {
        physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
    }

    return physTop;
    }

/*******************************************************************************
*
* sysMemTop - get the address of the top of VxWorks memory
*
* This routine returns a pointer to the first byte of memory not
* controlled or used by VxWorks.
*
* The user can reserve memory space by defining the macro USER_RESERVED_MEM
* in config.h.  This routine returns the address of the reserved memory
* area.  The value of USER_RESERVED_MEM is in bytes.
*
* RETURNS: The address of the top of VxWorks memory.
*
* ERRNO: N/A
*/

char * sysMemTop (void)
    {
    static char * memTop = NULL;

    if (memTop == NULL)
        {
#ifdef INCLUDE_EDR_PM
        memTop = (char *)sysPhysMemTop () - USER_RESERVED_MEM - PM_RESERVED_MEM;
#else
        memTop = (char *)sysPhysMemTop () - USER_RESERVED_MEM;
#endif /* INCLUDE_EDR_PM */
        }

    return (memTop);
    }

/*******************************************************************************
*
* sysToMonitor - transfer control to the ROM monitor
*
* This routine transfers control to the ROM monitor. It is usually called
* only by reboot() -- which services ^X -- and bus errors at interrupt
* level.  However, in some circumstances, the user may wish to introduce a
* new <startType> to enable special boot ROM facilities.
*
* RETURNS: Does not return.
*
* ERRNO: N/A
*/

STATUS sysToMonitor
    (
    int startType    /* passed to ROM to tell it how to boot */
    )
    {
#if 0
    FUNCPTR    pRom;
    int i;
    volatile UINT32 intrAck;

#ifdef _WRS_CONFIG_SMP
    cpuset_t cpuList;
    volatile int idx;
    int srcCpuId;

    /* if the current core is not core0, we won't run sysToMonitor directly */

    if (vxCpuIndexGet() != 0)
        {
        /*
         * 0xFEFEFEFE means that all other cores are indicated to shutdown
         * by core 0
         */

        if ((startType & 0xFFFFFFF0) != 0xFEFEFEF0)
            {
            /* If not directed here then tell CPU 0 to enter sysToMonitor */

            vxIpiConnect (INT_LVL_MPCORE_RESET,
                          (IPI_HANDLER_FUNC) (sysToMonitor),
                          (void *) (0xFEF00000 | startType | \
                                    (vxCpuIndexGet() << 16)));
            vxIpiEnable (INT_LVL_MPCORE_RESET);
            vxIpiEmit (INT_LVL_MPCORE_RESET, 1);

            /* wait for a reset from core 0 */

            while(1);
            }

         /*
          * If this is not core0, it must be informed by core0 to reboot
          * through IPI. Therefore, we must acknowkedge the IPI interrupt.
          * The IPI(SGI) interrupt information can not be got from Interrupt
          * Acknowledge Register, because ISR in interrupt controller driver
          * has read(to clear) this register.
          */

         srcCpuId = (startType & 0xF) << 10;
         *(volatile UINT32 *)(PBXA9_GIC_CPU_END_INTR) =
                               INT_LVL_MPCORE_RESET | srcCpuId;

        /* flush data cache */

        cacheFlush((CACHE_TYPE)DATA_CACHE, (void *)0, (size_t)ENTIRE_CACHE);
        cachePipeFlush();
        mmuCortexA8ADisable(); /* disable the MMU, cache(s) and write-buffer */

        /* Make sure we go back to the bootMonitor loop */

        pRom = (FUNCPTR)(ROM_TEXT_ADRS + 4);
        (*pRom)(0); /* jump to boot ROM */

        }

#endif /* _WRS_CONFIG_SMP */

    sysClkDisable();

    intIFLock();

#ifdef _WRS_CONFIG_SMP

    /* here is the core 0 */

    if ((startType & 0xFFF0000) == 0xFEF00000)
        {
        /*
         * core0 is informed by other core through IPI to reboot, so we must
         * acknowledge the IPI(SGI) interrupt.
         */

        srcCpuId = (startType & 0x000F0000) >> 16;
        *(volatile UINT32 *)(PBXA9_GIC_CPU_END_INTR) =
                                 INT_LVL_MPCORE_RESET + (srcCpuId << 10);

        startType = (startType & 0xFFFF);
        }

    /* clear entry address flags */

 //   *(volatile UINT32 *)PBXA9_SR_FLAGSCLR = 0xffffffff;

    /* direct all interrupts to go to CPU 0 */

   for (i = SPI_START_INT_NUM; i < SYS_INT_LEVELS_MAX; i += PRIOS_PER_WORD)
       {
       *(volatile UINT32 *)(PBXA9_GIC1_BASE + PBXA9_GIC_DIST_TARG + \
               (0x4 * (i / TARGETS_PER_WORD))) = GIC_CPU_DIR_DEFAULT;
       }

    /* reset all other cores */

    if ((cpuList = vxCpuEnabledGet()) && (cpuList > 1))
        {
        vxIpiConnect (INT_LVL_MPCORE_RESET,
                      (IPI_HANDLER_FUNC)(sysToMonitor),
                      (void *) (0xFEFEFEF0 | vxCpuIndexGet()));

        vxIpiEnable (INT_LVL_MPCORE_RESET);

        for (idx = 1; idx < VX_SMP_NUM_CPUS ; idx++)
            {
            if (cpuList & (1 << idx))
                {
                vxIpiEmit(INT_LVL_MPCORE_RESET, 1 << idx);
                }
            }
        }

    /* delay for non-boot core to shutdown */

    for (idx = 0; idx < 0x1ffff; idx++);

#endif /* _WRS_CONFIG_SMP */

    /* empty interrupt ack fifo */

    intrAck = *(UINT32 *)(PBXA9_GIC_CPU_ACK) & GIC_INT_SPURIOUS;
    while ((intrAck != GIC_INT_SPURIOUS) && (intrAck >= SGI_INT_MAX) )
        {
        *(UINT32 *)(PBXA9_GIC_CPU_END_INTR) = intrAck;
        intrAck = *(UINT32 *)(PBXA9_GIC_CPU_ACK) & GIC_INT_SPURIOUS;
        }

    /* disable all SPI interrupts */

    for(i = SPI_START_INT_NUM; i < SYS_INT_LEVELS_MAX; i += BITS_PER_WORD)
        {
        *(volatile UINT32 *)(PBXA9_GIC1_BASE + PBXA9_GIC_DIST_ENABLE_CLR1 + \
                                          (0x4 * NWORD(i))) = 0xffffffff;
        }

    /* disable GIC cpu interface */

    *(UINT32 *)(PBXA9_GIC_CPU_CONTROL) = 0;

    /* disable GIC distributor */

    *(UINT32 *)(PBXA9_GIC1_BASE + PBXA9_GIC_DIST_CONTROL) = 0;

#ifdef INCLUDE_MMU

    /* flush data cache */

    cacheFlush((CACHE_TYPE)DATA_CACHE, (void *)0, (size_t)ENTIRE_CACHE);

    cachePipeFlush();

#ifdef _WRS_CONFIG_SMP
    mmuCortexA8AcrSet(0);
    vfpDisable();
#endif /* _WRS_CONFIG_SMP */

    mmuCortexA8ADisable(); /* disable the MMU, cache(s) and write-buffer */

#endif /* INCLUDE_MMU */

    pRom = (FUNCPTR)(ROM_TEXT_ADRS + 4);

    (*pRom)(startType);    /* jump to boot ROM */
#endif
    return OK;        /* in case we ever continue from ROM monitor */
    }

/*******************************************************************************
*
* sysProcNumGet - get the processor number
*
* This routine returns the processor number for the CPU board, which is
* set with sysProcNumSet().
*
* RETURNS: The processor number for the CPU board.
*
* ERRNO: N/A
*
* SEE ALSO: sysProcNumSet()
*/

int sysProcNumGet (void)
    {
    return sysProcNum;
    }

/*******************************************************************************
*
* sysProcNumSet - set the processor number
*
* Set the processor number for the CPU board. Processor numbers should be
* unique on a single backplane.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: sysProcNumGet()
*/

void sysProcNumSet
    (
    int procNum        /* processor number */
    )
    {
    sysProcNum = procNum;
    }


/******************************************************************************
*
* bspSerialChanGet - get the SIO_CHAN device associated with a serial channel
*
* The sysSerialChanGet() routine returns a pointer to the SIO_CHAN
* device associated with a specified serial channel. It is called
* by usrRoot() to obtain pointers when creating the system serial
* devices, `/tyCo/x'. It is also used by the WDB agent to locate its
* serial channel.  The VxBus function requires that the BSP provide a
* function named bspSerialChanGet() to provide the information about
* any non-VxBus serial channels, provided by the BSP.  As this BSP
* does not support non-VxBus serial channels, this routine always
* returns ERROR.
*
* RETURNS: ERROR, always
*
* ERRNO: N/A
*
* \NOMANUAL
*/

SIO_CHAN * bspSerialChanGet
    (
    int channel     /* serial channel */
    )
    {
    return ((SIO_CHAN *) ERROR);
    }


/*******************************************************************************
*
* sysCpuAvailableGet - return the number of CPUs available
*
* This routine gets the number of CPUs available.
*
* RETURNS:  number of CPU cores available
*
* ERRNO: N/A
*/
#if 0
UINT32 sysCpuAvailableGet (void)
    {
    return ( ((*(volatile UINT32 *)PBXA9_SCU_CONFIG) & 0x3) + 1 );
    }
#endif
/*******************************************************************************
*
* sysMsDelay - time delay in the unit of ms
*
* This routine delays for approximately one ms. When system timer
* count register add 1 tick,
* 1 tick = 1/system timer clk = 1/SYS_TIMER_CLK = 1/1000000 (s) = 1000ns = 1us
* timer grows up.
*
* RETURNS: N/A
*/

void sysMsDelay
    (
    UINT        delay          /* length of time in MS to delay */
    )
    {
    volatile register UINT32 oldVal;
    volatile register UINT32 newVal;
    volatile register UINT32 decElapsed = 0;
    register UINT32 totalDelta;
    UINT32 maxTickCount;

    if(delay == 0)
        return;

    maxTickCount = 1000000 / sysClkRateGet(); /* 1000000 means 1000000us */

    totalDelta = delay * 1000; /* 1ms = 1000us */

    oldVal = *(volatile UINT32 *)(SYSTIMER_BASE_ADDR + 0x04);

    while (decElapsed < totalDelta)
        {
        newVal = *(volatile UINT32 *)(SYSTIMER_BASE_ADDR + 0x04);
        if(newVal == oldVal)
            continue;

        if (newVal < oldVal)   /* no rollover */
            {
            decElapsed += (oldVal - newVal);
            }
        else                   /* rollover */
            {
            decElapsed += ((maxTickCount - newVal) + oldVal);
            }

        oldVal = newVal;
        }
    }

/*******************************************************************************
*
* sysDelay - delay for approximately one millisecond
*
* This routine delays for approximately one milli-second.
*
* RETURNS: N/A
*/

void sysDelay (void)
    {
    sysMsDelay (1);
    }
