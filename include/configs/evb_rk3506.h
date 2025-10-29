/*
 * SPDX-License-Identifier:     GPL-2.0+
 *
 * Copyright (c) 2024 Rockchip Electronics Co., Ltd
 */

#ifndef __CONFIGS_RK3506_EVB_H
#define __CONFIGS_RK3506_EVB_H

#include <configs/rk3506_common.h>

#ifndef CONFIG_SPL_BUILD
#undef ROCKCHIP_DEVICE_SETTINGS
#define ROCKCHIP_DEVICE_SETTINGS \
		"stdin=serial\0" \
		"stdout=serial\0" \
		"stderr=serial\0"
#endif

#undef CONFIG_BOOTCOMMAND
#undef CONFIG_EXTRA_ENV_SETTINGS
#undef RKIMG_BOOTCOMMAND
#undef ENV_MEM_LAYOUT_SETTINGS
#undef RKIMG_DET_BOOTDEV

#define ENV_MEM_LAYOUT_SETTINGS \
	"fdt_addr_r=0x00063000\0"	\
	"kernel_addr_r=0x00108000\0"

#endif /* __CONFIGS_RK3506_EVB_H */
