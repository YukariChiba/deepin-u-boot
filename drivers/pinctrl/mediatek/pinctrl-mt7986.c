// SPDX-License-Identifier: GPL-2.0
/*
 * The MT7986 driver based on Linux generic pinctrl binding.
 *
 * Copyright (C) 2022 MediaTek Inc.
 * Author: Sam Shih <sam.shih@mediatek.com>
 */

#include <dm.h>
#include "pinctrl-mtk-common.h"

#define MT7986_TYPE0_PIN(_number, _name)	\
	MTK_TYPED_PIN(_number, _name, DRV_GRP4, IO_TYPE_GRP0)

#define MT7986_TYPE1_PIN(_number, _name)	\
	MTK_TYPED_PIN(_number, _name, DRV_GRP4, IO_TYPE_GRP1)

#define PIN_FIELD_GPIO(_s_pin, _e_pin, _s_addr, _x_addrs, _s_bit, _x_bits)	\
	PIN_FIELD_BASE_CALC(_s_pin, _e_pin, GPIO_BASE, _s_addr, _x_addrs,	\
			    _s_bit, _x_bits, 32, 0)

#define PIN_FIELD_BASE(_s_pin, _e_pin, _i_base, _s_addr, _x_addrs, _s_bit,	\
		       _x_bits)							\
	PIN_FIELD_BASE_CALC(_s_pin, _e_pin, _i_base, _s_addr, _x_addrs, _s_bit,	\
	_x_bits, 32, 0)

/**
 * enum - Locking variants of the iocfg bases
 *
 * MT7986 have multiple bases to program pin configuration listed as the below:
 * iocfg_rt:0x11c30000, iocfg_rb:0x11c40000, iocfg_lt:0x11e20000,
 * iocfg_lb:0x11e30000, iocfg_tr:0x11f00000, iocfg_tl:0x11f10000,
 * _i_based could be used to indicate what base the pin should be mapped into.
 *
 * Each iocfg register base control different group of pads on the SoC
 *
 *
 *  chip carrier
 *
 *      A  B  C  D  E  F  G  H
 *    +------------------------+
 *  8 | o  o  o  o  o  o  o  o |
 *  7 | o  o  o  o  o  o  o  o |
 *  6 | o  o  o  o  o  o  o  o |
 *  5 | o  o  o  o  o  o  o  o |
 *  4 | o  o  o  o  o  o  o  o |
 *  3 | o  o  o  o  o  o  o  o |
 *  2 | o  o  o  o  o  o  o  o |
 *  1 | o  o  o  o  o  o  o  o |
 *    +------------------------+
 *
 *  inside Chip carrier
 *
 *      A  B  C  D  E  F  G  H
 *    +------------------------+
 *  8 |                        |
 *  7 |        TL  TR          |
 *  6 |      +---------+       |
 *  5 |   LT |         | RT    |
 *  4 |      |         |       |
 *  3 |   LB |         | RB    |
 *  2 |      +---------+       |
 *  1 |                        |
 *    +------------------------+
 *
 */

enum {
	GPIO_BASE,
	IOCFG_RT_BASE,
	IOCFG_RB_BASE,
	IOCFG_LT_BASE,
	IOCFG_LB_BASE,
	IOCFG_TR_BASE,
	IOCFG_TL_BASE,
};

static const char *const mt7986_pinctrl_register_base_names[] = {
	"gpio", "iocfg_rt", "iocfg_rb", "iocfg_lt", "iocfg_lb", "iocfg_tr",
	"iocfg_tl",
};

static const struct mtk_pin_field_calc mt7986_pin_mode_range[] = {
	PIN_FIELD_GPIO(0, 100, 0x300, 0x10, 0, 4),
};

static const struct mtk_pin_field_calc mt7986_pin_dir_range[] = {
	PIN_FIELD_GPIO(0, 100, 0x0, 0x10, 0, 1),
};

static const struct mtk_pin_field_calc mt7986_pin_di_range[] = {
	PIN_FIELD_GPIO(0, 100, 0x200, 0x10, 0, 1),
};

static const struct mtk_pin_field_calc mt7986_pin_do_range[] = {
	PIN_FIELD_GPIO(0, 100, 0x100, 0x10, 0, 1),
};

static const struct mtk_pin_field_calc mt7986_pin_ies_range[] = {
	PIN_FIELD_BASE(0, 0, IOCFG_RB_BASE, 0x40, 0x10, 17, 1),
	PIN_FIELD_BASE(1, 2, IOCFG_LT_BASE, 0x20, 0x10, 10, 1),
	PIN_FIELD_BASE(3, 4, IOCFG_LB_BASE, 0x20, 0x10, 0, 1),
	PIN_FIELD_BASE(5, 6, IOCFG_RB_BASE, 0x40, 0x10, 0, 1),
	PIN_FIELD_BASE(7, 10, IOCFG_LT_BASE, 0x20, 0x10, 0, 1),
	PIN_FIELD_BASE(11, 14, IOCFG_RB_BASE, 0x40, 0x10, 8, 1),
	PIN_FIELD_BASE(15, 20, IOCFG_RB_BASE, 0x40, 0x10, 2, 1),
	PIN_FIELD_BASE(21, 23, IOCFG_RT_BASE, 0x30, 0x10, 12, 1),
	PIN_FIELD_BASE(24, 24, IOCFG_RT_BASE, 0x30, 0x10, 18, 1),
	PIN_FIELD_BASE(25, 25, IOCFG_RT_BASE, 0x30, 0x10, 17, 1),
	PIN_FIELD_BASE(26, 27, IOCFG_RT_BASE, 0x30, 0x10, 15, 1),
	PIN_FIELD_BASE(28, 29, IOCFG_RT_BASE, 0x30, 0x10, 19, 1),
	PIN_FIELD_BASE(30, 30, IOCFG_RT_BASE, 0x30, 0x10, 23, 1),
	PIN_FIELD_BASE(31, 31, IOCFG_RT_BASE, 0x30, 0x10, 22, 1),
	PIN_FIELD_BASE(32, 32, IOCFG_RT_BASE, 0x30, 0x10, 21, 1),
	PIN_FIELD_BASE(33, 33, IOCFG_LT_BASE, 0x20, 0x10, 4, 1),
	PIN_FIELD_BASE(34, 34, IOCFG_LT_BASE, 0x20, 0x10, 8, 1),
	PIN_FIELD_BASE(35, 35, IOCFG_LT_BASE, 0x20, 0x10, 7, 1),
	PIN_FIELD_BASE(36, 37, IOCFG_LT_BASE, 0x20, 0x10, 5, 1),
	PIN_FIELD_BASE(38, 38, IOCFG_LT_BASE, 0x20, 0x10, 9, 1),
	PIN_FIELD_BASE(39, 40, IOCFG_RB_BASE, 0x40, 0x10, 18, 1),
	PIN_FIELD_BASE(41, 41, IOCFG_RB_BASE, 0x40, 0x10, 12, 1),
	PIN_FIELD_BASE(42, 43, IOCFG_RB_BASE, 0x40, 0x10, 22, 1),
	PIN_FIELD_BASE(44, 45, IOCFG_RB_BASE, 0x40, 0x10, 20, 1),
	PIN_FIELD_BASE(46, 47, IOCFG_RB_BASE, 0x40, 0x10, 26, 1),
	PIN_FIELD_BASE(48, 49, IOCFG_RB_BASE, 0x40, 0x10, 24, 1),
	PIN_FIELD_BASE(50, 57, IOCFG_RT_BASE, 0x30, 0x10, 2, 1),
	PIN_FIELD_BASE(58, 58, IOCFG_RT_BASE, 0x30, 0x10, 1, 1),
	PIN_FIELD_BASE(59, 59, IOCFG_RT_BASE, 0x30, 0x10, 0, 1),
	PIN_FIELD_BASE(60, 61, IOCFG_RT_BASE, 0x30, 0x10, 10, 1),
	PIN_FIELD_BASE(62, 62, IOCFG_RB_BASE, 0x40, 0x10, 15, 1),
	PIN_FIELD_BASE(63, 63, IOCFG_RB_BASE, 0x40, 0x10, 14, 1),
	PIN_FIELD_BASE(64, 64, IOCFG_RB_BASE, 0x40, 0x10, 13, 1),
	PIN_FIELD_BASE(65, 65, IOCFG_RB_BASE, 0x40, 0x10, 16, 1),
	PIN_FIELD_BASE(66, 68, IOCFG_LB_BASE, 0x20, 0x10, 2, 1),
	PIN_FIELD_BASE(69, 69, IOCFG_TR_BASE, 0x30, 0x10, 1, 1),
	PIN_FIELD_BASE(70, 70, IOCFG_TR_BASE, 0x30, 0x10, 0, 1),
	PIN_FIELD_BASE(71, 71, IOCFG_TR_BASE, 0x30, 0x10, 16, 1),
	PIN_FIELD_BASE(72, 73, IOCFG_TR_BASE, 0x30, 0x10, 14, 1),
	PIN_FIELD_BASE(74, 74, IOCFG_TR_BASE, 0x30, 0x10, 4, 1),
	PIN_FIELD_BASE(75, 77, IOCFG_TR_BASE, 0x30, 0x10, 6, 1),
	PIN_FIELD_BASE(78, 79, IOCFG_TR_BASE, 0x30, 0x10, 2, 1),
	PIN_FIELD_BASE(80, 84, IOCFG_TR_BASE, 0x30, 0x10, 9, 1),
	PIN_FIELD_BASE(85, 85, IOCFG_TR_BASE, 0x30, 0x10, 5, 1),
	PIN_FIELD_BASE(86, 86, IOCFG_TL_BASE, 0x30, 0x10, 1, 1),
	PIN_FIELD_BASE(87, 87, IOCFG_TL_BASE, 0x30, 0x10, 0, 1),
	PIN_FIELD_BASE(88, 88, IOCFG_TL_BASE, 0x30, 0x10, 14, 1),
	PIN_FIELD_BASE(89, 90, IOCFG_TL_BASE, 0x30, 0x10, 12, 1),
	PIN_FIELD_BASE(91, 94, IOCFG_TL_BASE, 0x30, 0x10, 4, 1),
	PIN_FIELD_BASE(95, 96, IOCFG_TL_BASE, 0x30, 0x10, 2, 1),
	PIN_FIELD_BASE(97, 100, IOCFG_TL_BASE, 0x30, 0x10, 8, 1),
};

static const struct mtk_pin_field_calc mt7986_pin_smt_range[] = {
	PIN_FIELD_BASE(0, 0, IOCFG_RB_BASE, 0xf0, 0x10, 17, 1),
	PIN_FIELD_BASE(1, 2, IOCFG_LT_BASE, 0x90, 0x10, 10, 1),
	PIN_FIELD_BASE(3, 4, IOCFG_LB_BASE, 0x90, 0x10, 0, 1),
	PIN_FIELD_BASE(5, 6, IOCFG_RB_BASE, 0xf0, 0x10, 0, 1),
	PIN_FIELD_BASE(7, 10, IOCFG_LT_BASE, 0x90, 0x10, 0, 1),
	PIN_FIELD_BASE(11, 14, IOCFG_RB_BASE, 0xf0, 0x10, 8, 1),
	PIN_FIELD_BASE(15, 20, IOCFG_RB_BASE, 0xf0, 0x10, 2, 1),
	PIN_FIELD_BASE(21, 23, IOCFG_RT_BASE, 0xc0, 0x10, 12, 1),
	PIN_FIELD_BASE(24, 24, IOCFG_RT_BASE, 0xc0, 0x10, 18, 1),
	PIN_FIELD_BASE(25, 25, IOCFG_RT_BASE, 0xc0, 0x10, 17, 1),
	PIN_FIELD_BASE(26, 27, IOCFG_RT_BASE, 0xc0, 0x10, 15, 1),
	PIN_FIELD_BASE(28, 29, IOCFG_RT_BASE, 0xc0, 0x10, 19, 1),
	PIN_FIELD_BASE(30, 30, IOCFG_RT_BASE, 0xc0, 0x10, 23, 1),
	PIN_FIELD_BASE(31, 31, IOCFG_RT_BASE, 0xc0, 0x10, 22, 1),
	PIN_FIELD_BASE(32, 32, IOCFG_RT_BASE, 0xc0, 0x10, 21, 1),
	PIN_FIELD_BASE(33, 33, IOCFG_LT_BASE, 0x90, 0x10, 4, 1),
	PIN_FIELD_BASE(34, 34, IOCFG_LT_BASE, 0x90, 0x10, 8, 1),
	PIN_FIELD_BASE(35, 35, IOCFG_LT_BASE, 0x90, 0x10, 7, 1),
	PIN_FIELD_BASE(36, 37, IOCFG_LT_BASE, 0x90, 0x10, 5, 1),
	PIN_FIELD_BASE(38, 38, IOCFG_LT_BASE, 0x90, 0x10, 9, 1),
	PIN_FIELD_BASE(39, 40, IOCFG_RB_BASE, 0xf0, 0x10, 18, 1),
	PIN_FIELD_BASE(41, 41, IOCFG_RB_BASE, 0xf0, 0x10, 12, 1),
	PIN_FIELD_BASE(42, 43, IOCFG_RB_BASE, 0xf0, 0x10, 22, 1),
	PIN_FIELD_BASE(44, 45, IOCFG_RB_BASE, 0xf0, 0x10, 20, 1),
	PIN_FIELD_BASE(46, 47, IOCFG_RB_BASE, 0xf0, 0x10, 26, 1),
	PIN_FIELD_BASE(48, 49, IOCFG_RB_BASE, 0xf0, 0x10, 24, 1),
	PIN_FIELD_BASE(50, 57, IOCFG_RT_BASE, 0xc0, 0x10, 2, 1),
	PIN_FIELD_BASE(58, 58, IOCFG_RT_BASE, 0xc0, 0x10, 1, 1),
	PIN_FIELD_BASE(59, 59, IOCFG_RT_BASE, 0xc0, 0x10, 0, 1),
	PIN_FIELD_BASE(60, 61, IOCFG_RT_BASE, 0xc0, 0x10, 10, 1),
	PIN_FIELD_BASE(62, 62, IOCFG_RB_BASE, 0xf0, 0x10, 15, 1),
	PIN_FIELD_BASE(63, 63, IOCFG_RB_BASE, 0xf0, 0x10, 14, 1),
	PIN_FIELD_BASE(64, 64, IOCFG_RB_BASE, 0xf0, 0x10, 13, 1),
	PIN_FIELD_BASE(65, 65, IOCFG_RB_BASE, 0xf0, 0x10, 16, 1),
	PIN_FIELD_BASE(66, 68, IOCFG_LB_BASE, 0x90, 0x10, 2, 1),
	PIN_FIELD_BASE(69, 69, IOCFG_TR_BASE, 0x80, 0x10, 1, 1),
	PIN_FIELD_BASE(70, 70, IOCFG_TR_BASE, 0x80, 0x10, 0, 1),
	PIN_FIELD_BASE(71, 71, IOCFG_TR_BASE, 0x80, 0x10, 16, 1),
	PIN_FIELD_BASE(72, 73, IOCFG_TR_BASE, 0x80, 0x10, 14, 1),
	PIN_FIELD_BASE(74, 74, IOCFG_TR_BASE, 0x80, 0x10, 4, 1),
	PIN_FIELD_BASE(75, 77, IOCFG_TR_BASE, 0x80, 0x10, 6, 1),
	PIN_FIELD_BASE(78, 79, IOCFG_TR_BASE, 0x80, 0x10, 2, 1),
	PIN_FIELD_BASE(80, 84, IOCFG_TR_BASE, 0x80, 0x10, 9, 1),
	PIN_FIELD_BASE(85, 85, IOCFG_TR_BASE, 0x80, 0x10, 5, 1),
	PIN_FIELD_BASE(86, 86, IOCFG_TL_BASE, 0x70, 0x10, 1, 1),
	PIN_FIELD_BASE(87, 87, IOCFG_TL_BASE, 0x70, 0x10, 0, 1),
	PIN_FIELD_BASE(88, 88, IOCFG_TL_BASE, 0x70, 0x10, 14, 1),
	PIN_FIELD_BASE(89, 90, IOCFG_TL_BASE, 0x70, 0x10, 12, 1),
	PIN_FIELD_BASE(91, 94, IOCFG_TL_BASE, 0x70, 0x10, 4, 1),
	PIN_FIELD_BASE(95, 96, IOCFG_TL_BASE, 0x70, 0x10, 2, 1),
	PIN_FIELD_BASE(97, 100, IOCFG_TL_BASE, 0x70, 0x10, 8, 1),
};

static const struct mtk_pin_field_calc mt7986_pin_pu_range[] = {
	PIN_FIELD_BASE(69, 69, IOCFG_TR_BASE, 0x50, 0x10, 1, 1),
	PIN_FIELD_BASE(70, 70, IOCFG_TR_BASE, 0x50, 0x10, 0, 1),
	PIN_FIELD_BASE(71, 71, IOCFG_TR_BASE, 0x50, 0x10, 16, 1),
	PIN_FIELD_BASE(72, 73, IOCFG_TR_BASE, 0x50, 0x10, 14, 1),
	PIN_FIELD_BASE(74, 74, IOCFG_TR_BASE, 0x50, 0x10, 4, 1),
	PIN_FIELD_BASE(75, 77, IOCFG_TR_BASE, 0x50, 0x10, 6, 1),
	PIN_FIELD_BASE(78, 79, IOCFG_TR_BASE, 0x50, 0x10, 2, 1),
	PIN_FIELD_BASE(80, 84, IOCFG_TR_BASE, 0x50, 0x10, 9, 1),
	PIN_FIELD_BASE(85, 85, IOCFG_TR_BASE, 0x50, 0x10, 5, 1),
	PIN_FIELD_BASE(86, 86, IOCFG_TL_BASE, 0x50, 0x10, 1, 1),
	PIN_FIELD_BASE(87, 87, IOCFG_TL_BASE, 0x50, 0x10, 0, 1),
	PIN_FIELD_BASE(88, 88, IOCFG_TL_BASE, 0x50, 0x10, 14, 1),
	PIN_FIELD_BASE(89, 90, IOCFG_TL_BASE, 0x50, 0x10, 12, 1),
	PIN_FIELD_BASE(91, 94, IOCFG_TL_BASE, 0x50, 0x10, 4, 1),
	PIN_FIELD_BASE(95, 96, IOCFG_TL_BASE, 0x50, 0x10, 2, 1),
	PIN_FIELD_BASE(97, 100, IOCFG_TL_BASE, 0x50, 0x10, 8, 1),
};

static const struct mtk_pin_field_calc mt7986_pin_pd_range[] = {
	PIN_FIELD_BASE(69, 69, IOCFG_TR_BASE, 0x40, 0x10, 1, 1),
	PIN_FIELD_BASE(70, 70, IOCFG_TR_BASE, 0x40, 0x10, 0, 1),
	PIN_FIELD_BASE(71, 71, IOCFG_TR_BASE, 0x40, 0x10, 16, 1),
	PIN_FIELD_BASE(72, 73, IOCFG_TR_BASE, 0x40, 0x10, 14, 1),
	PIN_FIELD_BASE(74, 74, IOCFG_TR_BASE, 0x40, 0x10, 4, 1),
	PIN_FIELD_BASE(75, 77, IOCFG_TR_BASE, 0x40, 0x10, 6, 1),
	PIN_FIELD_BASE(78, 79, IOCFG_TR_BASE, 0x40, 0x10, 2, 1),
	PIN_FIELD_BASE(80, 84, IOCFG_TR_BASE, 0x40, 0x10, 9, 1),
	PIN_FIELD_BASE(85, 85, IOCFG_TR_BASE, 0x40, 0x10, 5, 1),
	PIN_FIELD_BASE(86, 86, IOCFG_TL_BASE, 0x40, 0x10, 1, 1),
	PIN_FIELD_BASE(87, 87, IOCFG_TL_BASE, 0x40, 0x10, 0, 1),
	PIN_FIELD_BASE(88, 88, IOCFG_TL_BASE, 0x40, 0x10, 14, 1),
	PIN_FIELD_BASE(89, 90, IOCFG_TL_BASE, 0x40, 0x10, 12, 1),
	PIN_FIELD_BASE(91, 94, IOCFG_TL_BASE, 0x40, 0x10, 4, 1),
	PIN_FIELD_BASE(95, 96, IOCFG_TL_BASE, 0x40, 0x10, 2, 1),
	PIN_FIELD_BASE(97, 100, IOCFG_TL_BASE, 0x40, 0x10, 8, 1),
};

static const struct mtk_pin_field_calc mt7986_pin_drv_range[] = {
	PIN_FIELD_BASE(0, 0, IOCFG_RB_BASE, 0x10, 0x10, 21, 3),
	PIN_FIELD_BASE(1, 2, IOCFG_LT_BASE, 0x10, 0x10, 0, 3),
	PIN_FIELD_BASE(3, 4, IOCFG_LB_BASE, 0x00, 0x10, 0, 1),
	PIN_FIELD_BASE(5, 5, IOCFG_RB_BASE, 0x00, 0x10, 0, 3),
	PIN_FIELD_BASE(6, 6, IOCFG_RB_BASE, 0x00, 0x10, 21, 3),
	PIN_FIELD_BASE(7, 10, IOCFG_LT_BASE, 0x00, 0x10, 0, 3),
	PIN_FIELD_BASE(11, 12, IOCFG_RB_BASE, 0x00, 0x10, 24, 3),
	PIN_FIELD_BASE(13, 14, IOCFG_RB_BASE, 0x10, 0x10, 0, 3),
	PIN_FIELD_BASE(15, 20, IOCFG_RB_BASE, 0x00, 0x10, 3, 3),
	PIN_FIELD_BASE(21, 23, IOCFG_RT_BASE, 0x10, 0x10, 6, 3),
	PIN_FIELD_BASE(24, 24, IOCFG_RT_BASE, 0x10, 0x10, 24, 3),
	PIN_FIELD_BASE(25, 25, IOCFG_RT_BASE, 0x10, 0x10, 21, 3),
	PIN_FIELD_BASE(26, 27, IOCFG_RT_BASE, 0x10, 0x10, 15, 3),
	PIN_FIELD_BASE(28, 28, IOCFG_RT_BASE, 0x10, 0x10, 27, 3),
	PIN_FIELD_BASE(29, 29, IOCFG_RT_BASE, 0x20, 0x10, 0, 3),
	PIN_FIELD_BASE(30, 30, IOCFG_RT_BASE, 0x20, 0x10, 9, 3),
	PIN_FIELD_BASE(31, 31, IOCFG_RT_BASE, 0x20, 0x10, 6, 3),
	PIN_FIELD_BASE(32, 32, IOCFG_RT_BASE, 0x20, 0x10, 3, 3),
	PIN_FIELD_BASE(33, 33, IOCFG_LT_BASE, 0x00, 0x10, 12, 3),
	PIN_FIELD_BASE(34, 34, IOCFG_LT_BASE, 0x00, 0x10, 24, 3),
	PIN_FIELD_BASE(35, 35, IOCFG_LT_BASE, 0x00, 0x10, 21, 3),
	PIN_FIELD_BASE(36, 37, IOCFG_LT_BASE, 0x00, 0x10, 15, 3),
	PIN_FIELD_BASE(38, 38, IOCFG_LT_BASE, 0x00, 0x10, 27, 3),
	PIN_FIELD_BASE(39, 39, IOCFG_RB_BASE, 0x10, 0x10, 27, 3),
	PIN_FIELD_BASE(40, 40, IOCFG_RB_BASE, 0x20, 0x10, 0, 3),
	PIN_FIELD_BASE(41, 41, IOCFG_RB_BASE, 0x10, 0x10, 6, 3),
	PIN_FIELD_BASE(42, 43, IOCFG_RB_BASE, 0x20, 0x10, 9, 3),
	PIN_FIELD_BASE(44, 45, IOCFG_RB_BASE, 0x20, 0x10, 3, 3),
	PIN_FIELD_BASE(46, 47, IOCFG_RB_BASE, 0x20, 0x10, 21, 3),
	PIN_FIELD_BASE(48, 49, IOCFG_RB_BASE, 0x20, 0x10, 15, 3),
	PIN_FIELD_BASE(50, 57, IOCFG_RT_BASE, 0x00, 0x10, 6, 3),
	PIN_FIELD_BASE(58, 58, IOCFG_RT_BASE, 0x00, 0x10, 3, 3),
	PIN_FIELD_BASE(59, 59, IOCFG_RT_BASE, 0x00, 0x10, 0, 3),
	PIN_FIELD_BASE(60, 61, IOCFG_RT_BASE, 0x10, 0x10, 0, 3),
	PIN_FIELD_BASE(62, 62, IOCFG_RB_BASE, 0x10, 0x10, 15, 3),
	PIN_FIELD_BASE(63, 63, IOCFG_RB_BASE, 0x10, 0x10, 12, 3),
	PIN_FIELD_BASE(64, 64, IOCFG_RB_BASE, 0x10, 0x10, 9, 3),
	PIN_FIELD_BASE(65, 65, IOCFG_RB_BASE, 0x10, 0x10, 18, 3),
	PIN_FIELD_BASE(66, 68, IOCFG_LB_BASE, 0x00, 0x10, 2, 3),
	PIN_FIELD_BASE(69, 69, IOCFG_TR_BASE, 0x00, 0x10, 3, 3),
	PIN_FIELD_BASE(70, 70, IOCFG_TR_BASE, 0x00, 0x10, 0, 3),
	PIN_FIELD_BASE(71, 71, IOCFG_TR_BASE, 0x10, 0x10, 18, 3),
	PIN_FIELD_BASE(72, 73, IOCFG_TR_BASE, 0x10, 0x10, 12, 3),
	PIN_FIELD_BASE(74, 77, IOCFG_TR_BASE, 0x00, 0x10, 15, 3),
	PIN_FIELD_BASE(78, 79, IOCFG_TR_BASE, 0x00, 0x10, 6, 3),
	PIN_FIELD_BASE(80, 80, IOCFG_TR_BASE, 0x00, 0x10, 27, 3),
	PIN_FIELD_BASE(81, 84, IOCFG_TR_BASE, 0x10, 0x10, 0, 3),
	PIN_FIELD_BASE(85, 85, IOCFG_TR_BASE, 0x00, 0x10, 12, 3),
	PIN_FIELD_BASE(86, 86, IOCFG_TL_BASE, 0x00, 0x10, 3, 3),
	PIN_FIELD_BASE(87, 87, IOCFG_TL_BASE, 0x00, 0x10, 0, 3),
	PIN_FIELD_BASE(88, 88, IOCFG_TL_BASE, 0x10, 0x10, 12, 3),
	PIN_FIELD_BASE(89, 90, IOCFG_TL_BASE, 0x10, 0x10, 6, 3),
	PIN_FIELD_BASE(91, 94, IOCFG_TL_BASE, 0x00, 0x10, 12, 3),
	PIN_FIELD_BASE(95, 96, IOCFG_TL_BASE, 0x00, 0x10, 6, 3),
	PIN_FIELD_BASE(97, 98, IOCFG_TL_BASE, 0x00, 0x10, 24, 3),
	PIN_FIELD_BASE(99, 100, IOCFG_TL_BASE, 0x10, 0x10, 2, 3),
};

static const struct mtk_pin_field_calc mt7986_pin_pupd_range[] = {
	PIN_FIELD_BASE(0, 0, IOCFG_RB_BASE, 0x60, 0x10, 17, 1),
	PIN_FIELD_BASE(1, 2, IOCFG_LT_BASE, 0x30, 0x10, 10, 1),
	PIN_FIELD_BASE(3, 4, IOCFG_LB_BASE, 0x40, 0x10, 0, 1),
	PIN_FIELD_BASE(5, 6, IOCFG_RB_BASE, 0x60, 0x10, 0, 1),
	PIN_FIELD_BASE(7, 10, IOCFG_LT_BASE, 0x30, 0x10, 0, 1),
	PIN_FIELD_BASE(11, 14, IOCFG_RB_BASE, 0x60, 0x10, 8, 1),
	PIN_FIELD_BASE(15, 20, IOCFG_RB_BASE, 0x60, 0x10, 2, 1),
	PIN_FIELD_BASE(21, 23, IOCFG_RT_BASE, 0x40, 0x10, 12, 1),
	PIN_FIELD_BASE(24, 24, IOCFG_RT_BASE, 0x40, 0x10, 18, 1),
	PIN_FIELD_BASE(25, 25, IOCFG_RT_BASE, 0x40, 0x10, 17, 1),
	PIN_FIELD_BASE(26, 27, IOCFG_RT_BASE, 0x40, 0x10, 15, 1),
	PIN_FIELD_BASE(28, 29, IOCFG_RT_BASE, 0x40, 0x10, 19, 1),
	PIN_FIELD_BASE(30, 30, IOCFG_RT_BASE, 0x40, 0x10, 23, 1),
	PIN_FIELD_BASE(31, 31, IOCFG_RT_BASE, 0x40, 0x10, 22, 1),
	PIN_FIELD_BASE(32, 32, IOCFG_RT_BASE, 0x40, 0x10, 21, 1),
	PIN_FIELD_BASE(33, 33, IOCFG_LT_BASE, 0x30, 0x10, 4, 1),
	PIN_FIELD_BASE(34, 34, IOCFG_LT_BASE, 0x30, 0x10, 8, 1),
	PIN_FIELD_BASE(35, 35, IOCFG_LT_BASE, 0x30, 0x10, 7, 1),
	PIN_FIELD_BASE(36, 37, IOCFG_LT_BASE, 0x30, 0x10, 5, 1),
	PIN_FIELD_BASE(38, 38, IOCFG_LT_BASE, 0x30, 0x10, 9, 1),
	PIN_FIELD_BASE(39, 40, IOCFG_RB_BASE, 0x60, 0x10, 18, 1),
	PIN_FIELD_BASE(41, 41, IOCFG_RB_BASE, 0x60, 0x10, 12, 1),
	PIN_FIELD_BASE(42, 43, IOCFG_RB_BASE, 0x60, 0x10, 23, 1),
	PIN_FIELD_BASE(44, 45, IOCFG_RB_BASE, 0x60, 0x10, 21, 1),
	PIN_FIELD_BASE(46, 47, IOCFG_RB_BASE, 0x60, 0x10, 27, 1),
	PIN_FIELD_BASE(48, 49, IOCFG_RB_BASE, 0x60, 0x10, 25, 1),
	PIN_FIELD_BASE(50, 57, IOCFG_RT_BASE, 0x40, 0x10, 2, 1),
	PIN_FIELD_BASE(58, 58, IOCFG_RT_BASE, 0x40, 0x10, 1, 1),
	PIN_FIELD_BASE(59, 59, IOCFG_RT_BASE, 0x40, 0x10, 0, 1),
	PIN_FIELD_BASE(60, 61, IOCFG_RT_BASE, 0x40, 0x10, 10, 1),
	PIN_FIELD_BASE(62, 62, IOCFG_RB_BASE, 0x60, 0x10, 15, 1),
	PIN_FIELD_BASE(63, 63, IOCFG_RB_BASE, 0x60, 0x10, 14, 1),
	PIN_FIELD_BASE(64, 64, IOCFG_RB_BASE, 0x60, 0x10, 13, 1),
	PIN_FIELD_BASE(65, 65, IOCFG_RB_BASE, 0x60, 0x10, 16, 1),
	PIN_FIELD_BASE(66, 68, IOCFG_LB_BASE, 0x40, 0x10, 2, 1),
};

static const struct mtk_pin_field_calc mt7986_pin_r0_range[] = {
	PIN_FIELD_BASE(0, 0, IOCFG_RB_BASE, 0x70, 0x10, 17, 1),
	PIN_FIELD_BASE(1, 2, IOCFG_LT_BASE, 0x40, 0x10, 10, 1),
	PIN_FIELD_BASE(3, 4, IOCFG_LB_BASE, 0x50, 0x10, 0, 1),
	PIN_FIELD_BASE(5, 6, IOCFG_RB_BASE, 0x70, 0x10, 0, 1),
	PIN_FIELD_BASE(7, 10, IOCFG_LT_BASE, 0x40, 0x10, 0, 1),
	PIN_FIELD_BASE(11, 14, IOCFG_RB_BASE, 0x70, 0x10, 8, 1),
	PIN_FIELD_BASE(15, 20, IOCFG_RB_BASE, 0x70, 0x10, 2, 1),
	PIN_FIELD_BASE(21, 23, IOCFG_RT_BASE, 0x50, 0x10, 12, 1),
	PIN_FIELD_BASE(24, 24, IOCFG_RT_BASE, 0x50, 0x10, 18, 1),
	PIN_FIELD_BASE(25, 25, IOCFG_RT_BASE, 0x50, 0x10, 17, 1),
	PIN_FIELD_BASE(26, 27, IOCFG_RT_BASE, 0x50, 0x10, 15, 1),
	PIN_FIELD_BASE(28, 29, IOCFG_RT_BASE, 0x50, 0x10, 19, 1),
	PIN_FIELD_BASE(30, 30, IOCFG_RT_BASE, 0x50, 0x10, 23, 1),
	PIN_FIELD_BASE(31, 31, IOCFG_RT_BASE, 0x50, 0x10, 22, 1),
	PIN_FIELD_BASE(32, 32, IOCFG_RT_BASE, 0x50, 0x10, 21, 1),
	PIN_FIELD_BASE(33, 33, IOCFG_LT_BASE, 0x40, 0x10, 4, 1),
	PIN_FIELD_BASE(34, 34, IOCFG_LT_BASE, 0x40, 0x10, 8, 1),
	PIN_FIELD_BASE(35, 35, IOCFG_LT_BASE, 0x40, 0x10, 7, 1),
	PIN_FIELD_BASE(36, 37, IOCFG_LT_BASE, 0x40, 0x10, 5, 1),
	PIN_FIELD_BASE(38, 38, IOCFG_LT_BASE, 0x40, 0x10, 9, 1),
	PIN_FIELD_BASE(39, 40, IOCFG_RB_BASE, 0x70, 0x10, 18, 1),
	PIN_FIELD_BASE(41, 41, IOCFG_RB_BASE, 0x70, 0x10, 12, 1),
	PIN_FIELD_BASE(42, 43, IOCFG_RB_BASE, 0x70, 0x10, 23, 1),
	PIN_FIELD_BASE(44, 45, IOCFG_RB_BASE, 0x70, 0x10, 21, 1),
	PIN_FIELD_BASE(46, 47, IOCFG_RB_BASE, 0x70, 0x10, 27, 1),
	PIN_FIELD_BASE(48, 49, IOCFG_RB_BASE, 0x70, 0x10, 25, 1),
	PIN_FIELD_BASE(50, 57, IOCFG_RT_BASE, 0x50, 0x10, 2, 1),
	PIN_FIELD_BASE(58, 58, IOCFG_RT_BASE, 0x50, 0x10, 1, 1),
	PIN_FIELD_BASE(59, 59, IOCFG_RT_BASE, 0x50, 0x10, 0, 1),
	PIN_FIELD_BASE(60, 61, IOCFG_RT_BASE, 0x50, 0x10, 10, 1),
	PIN_FIELD_BASE(62, 62, IOCFG_RB_BASE, 0x70, 0x10, 15, 1),
	PIN_FIELD_BASE(63, 63, IOCFG_RB_BASE, 0x70, 0x10, 14, 1),
	PIN_FIELD_BASE(64, 64, IOCFG_RB_BASE, 0x70, 0x10, 13, 1),
	PIN_FIELD_BASE(65, 65, IOCFG_RB_BASE, 0x70, 0x10, 16, 1),
	PIN_FIELD_BASE(66, 68, IOCFG_LB_BASE, 0x50, 0x10, 2, 1),
};

static const struct mtk_pin_field_calc mt7986_pin_r1_range[] = {
	PIN_FIELD_BASE(0, 0, IOCFG_RB_BASE, 0x80, 0x10, 17, 1),
	PIN_FIELD_BASE(1, 2, IOCFG_LT_BASE, 0x50, 0x10, 10, 1),
	PIN_FIELD_BASE(3, 4, IOCFG_LB_BASE, 0x60, 0x10, 0, 1),
	PIN_FIELD_BASE(5, 6, IOCFG_RB_BASE, 0x80, 0x10, 0, 1),
	PIN_FIELD_BASE(7, 10, IOCFG_LT_BASE, 0x50, 0x10, 0, 1),
	PIN_FIELD_BASE(11, 14, IOCFG_RB_BASE, 0x80, 0x10, 8, 1),
	PIN_FIELD_BASE(15, 20, IOCFG_RB_BASE, 0x80, 0x10, 2, 1),
	PIN_FIELD_BASE(21, 23, IOCFG_RT_BASE, 0x60, 0x10, 12, 1),
	PIN_FIELD_BASE(24, 24, IOCFG_RT_BASE, 0x60, 0x10, 18, 1),
	PIN_FIELD_BASE(25, 25, IOCFG_RT_BASE, 0x60, 0x10, 17, 1),
	PIN_FIELD_BASE(26, 27, IOCFG_RT_BASE, 0x60, 0x10, 15, 1),
	PIN_FIELD_BASE(28, 29, IOCFG_RT_BASE, 0x60, 0x10, 19, 1),
	PIN_FIELD_BASE(30, 30, IOCFG_RT_BASE, 0x60, 0x10, 23, 1),
	PIN_FIELD_BASE(31, 31, IOCFG_RT_BASE, 0x60, 0x10, 22, 1),
	PIN_FIELD_BASE(32, 32, IOCFG_RT_BASE, 0x60, 0x10, 21, 1),
	PIN_FIELD_BASE(33, 33, IOCFG_LT_BASE, 0x50, 0x10, 4, 1),
	PIN_FIELD_BASE(34, 34, IOCFG_LT_BASE, 0x50, 0x10, 8, 1),
	PIN_FIELD_BASE(35, 35, IOCFG_LT_BASE, 0x50, 0x10, 7, 1),
	PIN_FIELD_BASE(36, 37, IOCFG_LT_BASE, 0x50, 0x10, 5, 1),
	PIN_FIELD_BASE(38, 38, IOCFG_LT_BASE, 0x50, 0x10, 9, 1),
	PIN_FIELD_BASE(39, 40, IOCFG_RB_BASE, 0x80, 0x10, 18, 1),
	PIN_FIELD_BASE(41, 41, IOCFG_RB_BASE, 0x80, 0x10, 12, 1),
	PIN_FIELD_BASE(42, 43, IOCFG_RB_BASE, 0x80, 0x10, 23, 1),
	PIN_FIELD_BASE(44, 45, IOCFG_RB_BASE, 0x80, 0x10, 21, 1),
	PIN_FIELD_BASE(46, 47, IOCFG_RB_BASE, 0x80, 0x10, 27, 1),
	PIN_FIELD_BASE(48, 49, IOCFG_RB_BASE, 0x80, 0x10, 25, 1),
	PIN_FIELD_BASE(50, 57, IOCFG_RT_BASE, 0x60, 0x10, 2, 1),
	PIN_FIELD_BASE(58, 58, IOCFG_RT_BASE, 0x60, 0x10, 1, 1),
	PIN_FIELD_BASE(59, 59, IOCFG_RT_BASE, 0x60, 0x10, 0, 1),
	PIN_FIELD_BASE(60, 61, IOCFG_RT_BASE, 0x60, 0x10, 10, 1),
	PIN_FIELD_BASE(62, 62, IOCFG_RB_BASE, 0x80, 0x10, 15, 1),
	PIN_FIELD_BASE(63, 63, IOCFG_RB_BASE, 0x80, 0x10, 14, 1),
	PIN_FIELD_BASE(64, 64, IOCFG_RB_BASE, 0x80, 0x10, 13, 1),
	PIN_FIELD_BASE(65, 65, IOCFG_RB_BASE, 0x80, 0x10, 16, 1),
	PIN_FIELD_BASE(66, 68, IOCFG_LB_BASE, 0x60, 0x10, 2, 1),
};

static const struct mtk_pin_reg_calc mt7986_reg_cals[] = {
	[PINCTRL_PIN_REG_MODE] = MTK_RANGE(mt7986_pin_mode_range),
	[PINCTRL_PIN_REG_DIR] = MTK_RANGE(mt7986_pin_dir_range),
	[PINCTRL_PIN_REG_DI] = MTK_RANGE(mt7986_pin_di_range),
	[PINCTRL_PIN_REG_DO] = MTK_RANGE(mt7986_pin_do_range),
	[PINCTRL_PIN_REG_SMT] = MTK_RANGE(mt7986_pin_smt_range),
	[PINCTRL_PIN_REG_IES] = MTK_RANGE(mt7986_pin_ies_range),
	[PINCTRL_PIN_REG_DRV] = MTK_RANGE(mt7986_pin_drv_range),
	[PINCTRL_PIN_REG_PU] = MTK_RANGE(mt7986_pin_pu_range),
	[PINCTRL_PIN_REG_PD] = MTK_RANGE(mt7986_pin_pd_range),
	[PINCTRL_PIN_REG_PUPD] = MTK_RANGE(mt7986_pin_pupd_range),
	[PINCTRL_PIN_REG_R0] = MTK_RANGE(mt7986_pin_r0_range),
	[PINCTRL_PIN_REG_R1] = MTK_RANGE(mt7986_pin_r1_range),
};

static const struct mtk_pin_desc mt7986_pins[] = {
	MT7986_TYPE0_PIN(0, "SYS_WATCHDOG"),
	MT7986_TYPE0_PIN(1, "WF2G_LED"),
	MT7986_TYPE0_PIN(2, "WF5G_LED"),
	MT7986_TYPE0_PIN(3, "I2C_SCL"),
	MT7986_TYPE0_PIN(4, "I2C_SDA"),
	MT7986_TYPE0_PIN(5, "GPIO_0"),
	MT7986_TYPE0_PIN(6, "GPIO_1"),
	MT7986_TYPE0_PIN(7, "GPIO_2"),
	MT7986_TYPE0_PIN(8, "GPIO_3"),
	MT7986_TYPE0_PIN(9, "GPIO_4"),
	MT7986_TYPE0_PIN(10, "GPIO_5"),
	MT7986_TYPE0_PIN(11, "GPIO_6"),
	MT7986_TYPE0_PIN(12, "GPIO_7"),
	MT7986_TYPE0_PIN(13, "GPIO_8"),
	MT7986_TYPE0_PIN(14, "GPIO_9"),
	MT7986_TYPE0_PIN(15, "GPIO_10"),
	MT7986_TYPE0_PIN(16, "GPIO_11"),
	MT7986_TYPE0_PIN(17, "GPIO_12"),
	MT7986_TYPE0_PIN(18, "GPIO_13"),
	MT7986_TYPE0_PIN(19, "GPIO_14"),
	MT7986_TYPE0_PIN(20, "GPIO_15"),
	MT7986_TYPE0_PIN(21, "PWM0"),
	MT7986_TYPE0_PIN(22, "PWM1"),
	MT7986_TYPE0_PIN(23, "SPI0_CLK"),
	MT7986_TYPE0_PIN(24, "SPI0_MOSI"),
	MT7986_TYPE0_PIN(25, "SPI0_MISO"),
	MT7986_TYPE0_PIN(26, "SPI0_CS"),
	MT7986_TYPE0_PIN(27, "SPI0_HOLD"),
	MT7986_TYPE0_PIN(28, "SPI0_WP"),
	MT7986_TYPE0_PIN(29, "SPI1_CLK"),
	MT7986_TYPE0_PIN(30, "SPI1_MOSI"),
	MT7986_TYPE0_PIN(31, "SPI1_MISO"),
	MT7986_TYPE0_PIN(32, "SPI1_CS"),
	MT7986_TYPE0_PIN(33, "SPI2_CLK"),
	MT7986_TYPE0_PIN(34, "SPI2_MOSI"),
	MT7986_TYPE0_PIN(35, "SPI2_MISO"),
	MT7986_TYPE0_PIN(36, "SPI2_CS"),
	MT7986_TYPE0_PIN(37, "SPI2_HOLD"),
	MT7986_TYPE0_PIN(38, "SPI2_WP"),
	MT7986_TYPE0_PIN(39, "UART0_RXD"),
	MT7986_TYPE0_PIN(40, "UART0_TXD"),
	MT7986_TYPE0_PIN(41, "PCIE_PERESET_N"),
	MT7986_TYPE0_PIN(42, "UART1_RXD"),
	MT7986_TYPE0_PIN(43, "UART1_TXD"),
	MT7986_TYPE0_PIN(44, "UART1_CTS"),
	MT7986_TYPE0_PIN(45, "UART1_RTS"),
	MT7986_TYPE0_PIN(46, "UART2_RXD"),
	MT7986_TYPE0_PIN(47, "UART2_TXD"),
	MT7986_TYPE0_PIN(48, "UART2_CTS"),
	MT7986_TYPE0_PIN(49, "UART2_RTS"),
	MT7986_TYPE0_PIN(50, "EMMC_DATA_0"),
	MT7986_TYPE0_PIN(51, "EMMC_DATA_1"),
	MT7986_TYPE0_PIN(52, "EMMC_DATA_2"),
	MT7986_TYPE0_PIN(53, "EMMC_DATA_3"),
	MT7986_TYPE0_PIN(54, "EMMC_DATA_4"),
	MT7986_TYPE0_PIN(55, "EMMC_DATA_5"),
	MT7986_TYPE0_PIN(56, "EMMC_DATA_6"),
	MT7986_TYPE0_PIN(57, "EMMC_DATA_7"),
	MT7986_TYPE0_PIN(58, "EMMC_CMD"),
	MT7986_TYPE0_PIN(59, "EMMC_CK"),
	MT7986_TYPE0_PIN(60, "EMMC_DSL"),
	MT7986_TYPE0_PIN(61, "EMMC_RSTB"),
	MT7986_TYPE0_PIN(62, "PCM_DTX"),
	MT7986_TYPE0_PIN(63, "PCM_DRX"),
	MT7986_TYPE0_PIN(64, "PCM_CLK"),
	MT7986_TYPE0_PIN(65, "PCM_FS"),
	MT7986_TYPE0_PIN(66, "MT7531_INT"),
	MT7986_TYPE0_PIN(67, "SMI_MDC"),
	MT7986_TYPE0_PIN(68, "SMI_MDIO"),
	MT7986_TYPE1_PIN(69, "WF0_DIG_RESETB"),
	MT7986_TYPE1_PIN(70, "WF0_CBA_RESETB"),
	MT7986_TYPE1_PIN(71, "WF0_XO_REQ"),
	MT7986_TYPE1_PIN(72, "WF0_TOP_CLK"),
	MT7986_TYPE1_PIN(73, "WF0_TOP_DATA"),
	MT7986_TYPE1_PIN(74, "WF0_HB1"),
	MT7986_TYPE1_PIN(75, "WF0_HB2"),
	MT7986_TYPE1_PIN(76, "WF0_HB3"),
	MT7986_TYPE1_PIN(77, "WF0_HB4"),
	MT7986_TYPE1_PIN(78, "WF0_HB0"),
	MT7986_TYPE1_PIN(79, "WF0_HB0_B"),
	MT7986_TYPE1_PIN(80, "WF0_HB5"),
	MT7986_TYPE1_PIN(81, "WF0_HB6"),
	MT7986_TYPE1_PIN(82, "WF0_HB7"),
	MT7986_TYPE1_PIN(83, "WF0_HB8"),
	MT7986_TYPE1_PIN(84, "WF0_HB9"),
	MT7986_TYPE1_PIN(85, "WF0_HB10"),
	MT7986_TYPE1_PIN(86, "WF1_DIG_RESETB"),
	MT7986_TYPE1_PIN(87, "WF1_CBA_RESETB"),
	MT7986_TYPE1_PIN(88, "WF1_XO_REQ"),
	MT7986_TYPE1_PIN(89, "WF1_TOP_CLK"),
	MT7986_TYPE1_PIN(90, "WF1_TOP_DATA"),
	MT7986_TYPE1_PIN(91, "WF1_HB1"),
	MT7986_TYPE1_PIN(92, "WF1_HB2"),
	MT7986_TYPE1_PIN(93, "WF1_HB3"),
	MT7986_TYPE1_PIN(94, "WF1_HB4"),
	MT7986_TYPE1_PIN(95, "WF1_HB0"),
	MT7986_TYPE1_PIN(96, "WF1_HB0_B"),
	MT7986_TYPE1_PIN(97, "WF1_HB5"),
	MT7986_TYPE1_PIN(98, "WF1_HB6"),
	MT7986_TYPE1_PIN(99, "WF1_HB7"),
	MT7986_TYPE1_PIN(100, "WF1_HB8"),
};

static const struct mtk_io_type_desc mt7986_io_type_desc[] = {
	[IO_TYPE_GRP0] = {
		.name = "18OD33",
		.bias_set = mtk_pinconf_bias_set_pupd_r1_r0,
		.drive_set = mtk_pinconf_drive_set_v1,
		.input_enable = mtk_pinconf_input_enable_v1,
	},
	[IO_TYPE_GRP1] = {
		.name = "18A01",
		.bias_set = mtk_pinconf_bias_set_pu_pd,
		.drive_set = mtk_pinconf_drive_set_v1,
		.input_enable = mtk_pinconf_input_enable_v1,
	},
};

/* List all groups consisting of these pins dedicated to the enablement of
 * certain hardware block and the corresponding mode for all of the pins.
 * The hardware probably has multiple combinations of these pinouts.
 */

static const int mt7986_watchdog_pins[] = { 0, };
static const int mt7986_watchdog_funcs[] = { 1, };

static const int mt7986_wifi_led_pins[] = { 1, 2, };
static const int mt7986_wifi_led_funcs[] = { 1, 1, };

static const int mt7986_i2c_pins[] = { 3, 4, };
static const int mt7986_i2c_funcs[] = { 1, 1, };

static const int mt7986_uart1_0_pins[] = { 7, 8, 9, 10, };
static const int mt7986_uart1_0_funcs[] = { 3, 3, 3, 3, };

static const int mt7986_spi1_0_pins[] = { 11, 12, 13, 14, };
static const int mt7986_spi1_0_funcs[] = { 3, 3, 3, 3, };

static const int mt7986_pwm1_1_pins[] = { 20, };
static const int mt7986_pwm1_1_funcs[] = { 2, };

static const int mt7986_pwm0_pins[] = { 21, };
static const int mt7986_pwm0_funcs[] = { 1, };

static const int mt7986_pwm1_0_pins[] = { 22, };
static const int mt7986_pwm1_0_funcs[] = { 1, };

static const int mt7986_emmc_45_pins[] = {
	22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, };
static const int mt7986_emmc_45_funcs[] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, };

static const int mt7986_snfi_pins[] = { 23, 24, 25, 26, 27, 28, };
static const int mt7986_snfi_funcs[] = { 1, 1, 1, 1, 1, 1, };

static const int mt7986_spi1_1_pins[] = { 23, 24, 25, 26, };
static const int mt7986_spi1_1_funcs[] = { 3, 3, 3, 3, };

static const int mt7986_uart1_1_pins[] = { 23, 24, 25, 26, };
static const int mt7986_uart1_1_funcs[] = { 4, 4, 4, 4, };

static const int mt7986_spi1_2_pins[] = { 29, 30, 31, 32, };
static const int mt7986_spi1_2_funcs[] = { 1, 1, 1, 1, };

static const int mt7986_uart1_2_pins[] = { 29, 30, 31, 32, };
static const int mt7986_uart1_2_funcs[] = { 3, 3, 3, 3, };

static const int mt7986_uart2_0_pins[] = { 29, 30, 31, 32, };
static const int mt7986_uart2_0_funcs[] = { 4, 4, 4, 4, };

static const int mt7986_spi0_pins[] = { 33, 34, 35, 36, };
static const int mt7986_spi0_funcs[] = { 1, 1, 1, 1, };

static const int mt7986_spi0_wp_hold_pins[] = { 37, 38, };
static const int mt7986_spi0_wp_hold_funcs[] = { 1, 1, };

static const int mt7986_uart2_1_pins[] = { 33, 34, 35, 36, };
static const int mt7986_uart2_1_funcs[] = { 3, 3, 3, 3, };

static const int mt7986_uart1_3_rx_tx_pins[] = { 35, 36, };
static const int mt7986_uart1_3_rx_tx_funcs[] = { 2, 2, };

static const int mt7986_uart1_3_cts_rts_pins[] = { 37, 38, };
static const int mt7986_uart1_3_cts_rts_funcs[] = { 2, 2, };

static const int mt7986_spi1_3_pins[] = { 33, 34, 35, 36, };
static const int mt7986_spi1_3_funcs[] = { 4, 4, 4, 4, };

static const int mt7986_uart0_pins[] = { 39, 40, };
static const int mt7986_uart0_funcs[] = { 1, 1, };

static const int mt7986_pcie_reset_pins[] = { 41, };
static const int mt7986_pcie_reset_funcs[] = { 1, };

static const int mt7986_uart1_pins[] = { 42, 43, 44, 45, };
static const int mt7986_uart1_funcs[] = { 1, 1, 1, 1, };

static const int mt7986_uart2_pins[] = { 46, 47, 48, 49, };
static const int mt7986_uart2_funcs[] = { 1, 1, 1, 1, };

static const int mt7986_emmc_51_pins[] = {
	50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, };
static const int mt7986_emmc_51_funcs[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, };

static const int mt7986_pcm_pins[] = { 62, 63, 64, 65, };
static const int mt7986_pcm_funcs[] = { 1, 1, 1, 1, };

static const int mt7986_i2s_pins[] = { 62, 63, 64, 65, };
static const int mt7986_i2s_funcs[] = { 1, 1, 1, 1, };

static const int mt7986_switch_int_pins[] = { 66, };
static const int mt7986_switch_int_funcs[] = { 1, };

static const int mt7986_mdc_mdio_pins[] = { 67, 68, };
static const int mt7986_mdc_mdio_funcs[] = { 1, 1, };

static const int mt7986_wf_2g_pins[] = {
	74, 75, 76, 77, 78, 79, 80, 81, 82, 83, };
static const int mt7986_wf_2g_funcs[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, };

static const int mt7986_wf_5g_pins[] = {
	91, 92, 93, 94, 95, 96, 97, 98, 99, 100, };
static const int mt7986_wf_5g_funcs[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, };

static const int mt7986_wf_dbdc_pins[] = {
	74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, };
static const int mt7986_wf_dbdc_funcs[] = {
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, };

static const int mt7986_pcie_clk_pins[] = { 9, };
static const int mt7986_pcie_clk_funcs[] = { 1, };

static const int mt7986_pcie_wake_pins[] = { 10, };
static const int mt7986_pcie_wake_funcs[] = { 1, };

static const struct mtk_group_desc mt7986_groups[] = {
	PINCTRL_PIN_GROUP("watchdog", mt7986_watchdog),
	PINCTRL_PIN_GROUP("wifi_led", mt7986_wifi_led),
	PINCTRL_PIN_GROUP("i2c", mt7986_i2c),
	PINCTRL_PIN_GROUP("uart1_0", mt7986_uart1_0),
	PINCTRL_PIN_GROUP("pcie_clk", mt7986_pcie_clk),
	PINCTRL_PIN_GROUP("pcie_wake", mt7986_pcie_wake),
	PINCTRL_PIN_GROUP("spi1_0", mt7986_spi1_0),
	PINCTRL_PIN_GROUP("pwm1_1", mt7986_pwm1_1),
	PINCTRL_PIN_GROUP("pwm0", mt7986_pwm0),
	PINCTRL_PIN_GROUP("pwm1_0", mt7986_pwm1_0),
	PINCTRL_PIN_GROUP("emmc_45", mt7986_emmc_45),
	PINCTRL_PIN_GROUP("snfi", mt7986_snfi),
	PINCTRL_PIN_GROUP("spi1_1", mt7986_spi1_1),
	PINCTRL_PIN_GROUP("uart1_1", mt7986_uart1_1),
	PINCTRL_PIN_GROUP("spi1_2", mt7986_spi1_2),
	PINCTRL_PIN_GROUP("uart1_2", mt7986_uart1_2),
	PINCTRL_PIN_GROUP("uart2_0", mt7986_uart2_0),
	PINCTRL_PIN_GROUP("spi0", mt7986_spi0),
	PINCTRL_PIN_GROUP("spi0_wp_hold", mt7986_spi0_wp_hold),
	PINCTRL_PIN_GROUP("uart2_1", mt7986_uart2_1),
	PINCTRL_PIN_GROUP("uart1_3_rx_tx", mt7986_uart1_3_rx_tx),
	PINCTRL_PIN_GROUP("uart1_3_cts_rts", mt7986_uart1_3_cts_rts),
	PINCTRL_PIN_GROUP("spi1_3", mt7986_spi1_3),
	PINCTRL_PIN_GROUP("uart0", mt7986_uart0),
	PINCTRL_PIN_GROUP("switch_int", mt7986_switch_int),
	PINCTRL_PIN_GROUP("mdc_mdio", mt7986_mdc_mdio),
	PINCTRL_PIN_GROUP("pcie_pereset", mt7986_pcie_reset),
	PINCTRL_PIN_GROUP("uart1", mt7986_uart1),
	PINCTRL_PIN_GROUP("uart2", mt7986_uart2),
	PINCTRL_PIN_GROUP("emmc_51", mt7986_emmc_51),
	PINCTRL_PIN_GROUP("pcm", mt7986_pcm),
	PINCTRL_PIN_GROUP("i2s", mt7986_i2s),
	PINCTRL_PIN_GROUP("wf_2g", mt7986_wf_2g),
	PINCTRL_PIN_GROUP("wf_5g", mt7986_wf_5g),
	PINCTRL_PIN_GROUP("wf_dbdc", mt7986_wf_dbdc),
};

/* Joint those groups owning the same capability in user point of view which
 * allows that people tend to use through the device tree.
 */

static const char *const mt7986_audio_groups[] = { "pcm", "i2s" };
static const char *const mt7986_emmc_groups[] = { "emmc_45", "emmc_51", };
static const char *const mt7986_ethernet_groups[] = { "switch_int",
	"mdc_mdio", };
static const char *const mt7986_i2c_groups[] = { "i2c", };
static const char *const mt7986_led_groups[] = { "wifi_led", };
static const char *const mt7986_flash_groups[] = { "snfi", };
static const char *const mt7986_pcie_groups[] = { "pcie_clk", "pcie_wake",
	"pcie_pereset" };
static const char *const mt7986_pwm_groups[] = { "pwm0", "pwm1_0", "pwm1_1", };
static const char *const mt7986_spi_groups[] = { "spi0", "spi0_wp_hold",
	"spi1_0", "spi1_1", "spi1_2", "spi1_3", };
static const char *const mt7986_uart_groups[] = { "uart1_0", "uart1_1",
	"uart1_2", "uart1_3_rx_tx", "uart1_3_cts_rts", "uart2_0", "uart2_1",
	"uart0", "uart1", "uart2", };
static const char *const mt7986_wdt_groups[] = { "watchdog", };
static const char *const mt7986_wf_groups[] = { "wf_2g", "wf_5g", "wf_dbdc", };

static const struct mtk_function_desc mt7986_functions[] = {
	{"audio", mt7986_audio_groups, ARRAY_SIZE(mt7986_audio_groups)},
	{"emmc", mt7986_emmc_groups, ARRAY_SIZE(mt7986_emmc_groups)},
	{"eth", mt7986_ethernet_groups, ARRAY_SIZE(mt7986_ethernet_groups)},
	{"i2c", mt7986_i2c_groups, ARRAY_SIZE(mt7986_i2c_groups)},
	{"led", mt7986_led_groups, ARRAY_SIZE(mt7986_led_groups)},
	{"flash", mt7986_flash_groups, ARRAY_SIZE(mt7986_flash_groups)},
	{"pcie", mt7986_pcie_groups, ARRAY_SIZE(mt7986_pcie_groups)},
	{"pwm", mt7986_pwm_groups, ARRAY_SIZE(mt7986_pwm_groups)},
	{"spi", mt7986_spi_groups, ARRAY_SIZE(mt7986_spi_groups)},
	{"uart", mt7986_uart_groups, ARRAY_SIZE(mt7986_uart_groups)},
	{"watchdog", mt7986_wdt_groups, ARRAY_SIZE(mt7986_wdt_groups)},
	{"wifi", mt7986_wf_groups, ARRAY_SIZE(mt7986_wf_groups)},
};

static const struct mtk_pinctrl_soc mt7986_data = {
	.name = "mt7986_pinctrl",
	.reg_cal = mt7986_reg_cals,
	.pins = mt7986_pins,
	.npins = ARRAY_SIZE(mt7986_pins),
	.grps = mt7986_groups,
	.ngrps = ARRAY_SIZE(mt7986_groups),
	.funcs = mt7986_functions,
	.nfuncs = ARRAY_SIZE(mt7986_functions),
	.io_type = mt7986_io_type_desc,
	.ntype = ARRAY_SIZE(mt7986_io_type_desc),
	.gpio_mode = 0,
	.base_names = mt7986_pinctrl_register_base_names,
	.nbase_names = ARRAY_SIZE(mt7986_pinctrl_register_base_names),
	.base_calc = 1,
};

static int mtk_pinctrl_mt7986_probe(struct udevice *dev)
{
	return mtk_pinctrl_common_probe(dev, &mt7986_data);
}

static const struct udevice_id mt7986_pctrl_match[] = {
	{.compatible = "mediatek,mt7986-pinctrl"},
	{ /* sentinel */ }
};

U_BOOT_DRIVER(mt7986_pinctrl) = {
	.name = "mt7986_pinctrl",
	.id = UCLASS_PINCTRL,
	.of_match = mt7986_pctrl_match,
	.ops = &mtk_pinctrl_ops,
	.probe = mtk_pinctrl_mt7986_probe,
	.priv_auto = sizeof(struct mtk_pinctrl_priv),
};
