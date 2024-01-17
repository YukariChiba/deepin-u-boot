/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * K3: J721S2 SoC definitions, structures etc.
 *
 * (C) Copyright (C) 2021 Texas Instruments Incorporated - https://www.ti.com/
 */
#ifndef __ASM_ARCH_J721S2_HARDWARE_H
#define __ASM_ARCH_J721S2_HARDWARE_H

#include <config.h>
#ifndef __ASSEMBLY__
#include <linux/bitops.h>
#endif

#define WKUP_CTRL_MMR0_BASE				0x43000000
#define MCU_CTRL_MMR0_BASE				0x40f00000
#define CTRL_MMR0_BASE					0x00100000

#define CTRLMMR_MAIN_DEVSTAT				(CTRL_MMR0_BASE + 0x30)
#define MAIN_DEVSTAT_BOOT_MODE_B_MASK			BIT(0)
#define MAIN_DEVSTAT_BOOT_MODE_B_SHIFT			0
#define MAIN_DEVSTAT_BKUP_BOOTMODE_MASK			GENMASK(3, 1)
#define MAIN_DEVSTAT_BKUP_BOOTMODE_SHIFT		1
#define MAIN_DEVSTAT_PRIM_BOOTMODE_MMC_PORT_MASK	BIT(6)
#define MAIN_DEVSTAT_PRIM_BOOTMODE_PORT_SHIFT		6
#define MAIN_DEVSTAT_BKUP_MMC_PORT_MASK			BIT(7)
#define MAIN_DEVSTAT_BKUP_MMC_PORT_SHIFT		7

#define CTRLMMR_WKUP_DEVSTAT				(WKUP_CTRL_MMR0_BASE + 0x30)
#define WKUP_DEVSTAT_PRIMARY_BOOTMODE_MASK		GENMASK(5, 3)
#define WKUP_DEVSTAT_PRIMARY_BOOTMODE_SHIFT		3
#define WKUP_DEVSTAT_MCU_OMLY_MASK			BIT(6)
#define WKUP_DEVSTAT_MCU_ONLY_SHIFT			6

/* ROM HANDOFF Structure location */
#define ROM_EXTENDED_BOOT_DATA_INFO			0x41cfdb00

/* MCU SCRATCHPAD usage */
#define TI_SRAM_SCRATCH_BOARD_EEPROM_START	CONFIG_SYS_K3_MCU_SCRATCHPAD_BASE

#if defined(CONFIG_SYS_K3_SPL_ATF) && !defined(__ASSEMBLY__)

#define J721S2_DEV_MCU_RTI0			295
#define J721S2_DEV_MCU_RTI1			296
#define J721S2_DEV_MCU_ARMSS0_CPU0		284
#define J721S2_DEV_MCU_ARMSS0_CPU1		285

static const u32 put_device_ids[] = {
	J721S2_DEV_MCU_RTI0,
	J721S2_DEV_MCU_RTI1,
};

static const u32 put_core_ids[] = {
	J721S2_DEV_MCU_ARMSS0_CPU1,
	J721S2_DEV_MCU_ARMSS0_CPU0,	/* Handle CPU0 after CPU1 */
};

#endif

#endif /* __ASM_ARCH_J721S2_HARDWARE_H */
