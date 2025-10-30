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

#undef ENV_MEM_LAYOUT_SETTINGS
#undef CONFIG_EXTRA_ENV_SETTINGS

#define ENV_MEM_LAYOUT_SETTINGS \
	"fdt_addr_r=0x00063000\0"	\
	"kernel_addr_r=0x00108000\0"

#ifdef ROCKCHIP_BOOTDEV_SIMPLIFIED
#define ROCKCHIP_BOOTDEV_ENV \
	"devtype=mtd\0" \
	"devnum=1\0"
#else
#define ROCKCHIP_BOOTDEV_ENV ""
#endif

#define CONFIG_EXTRA_ENV_SETTINGS \
	ENV_MEM_LAYOUT_SETTINGS \
	"distro_bootcmd=" \
		"mtdparts; " \
		"mtd list; " \
		"ubi detach; " \
		"ubi part rootfs 2048; " \
		"ubifsmount ubi0:rootfs; " \
		"ubifsload ${kernel_addr_r} /boot/zImage; " \
		"ubifsload ${fdt_addr_r} /boot/rk3506b-evb1-v10.dtb; " \
		"bootz ${kernel_addr_r} - ${fdt_addr_r}\0" \
	"bootargs=ubi.mtd=0 root=ubi0:rootfs rootfstype=ubifs rw mtdparts=spi2.0:4M@4M(uboot),-(rootfs)\0" \
	"mtdids=spi-nand0=spi-nand0\0" \
	"mtdparts=mtdparts=spi-nand0:" \
		"0x00400000@0x00400000(uboot)," \
		"-@0x00800000(rootfs)\0" \
	ROCKCHIP_BOOTDEV_ENV

#endif /* __CONFIGS_RK3506_EVB_H */
