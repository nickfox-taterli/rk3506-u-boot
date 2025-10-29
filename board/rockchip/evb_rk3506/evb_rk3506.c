/*
 * SPDX-License-Identifier:     GPL-2.0+
 *
 * (C) Copyright 2024 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dwc3-uboot.h>
#include <usb.h>
#include <asm/io.h>

#define GRF_BASE         0xFF288000
#define PMUGRF_BASE      0xFF910000
#define GPIO0_IOC_BASE   0xFF950000

#define GRF_SOC_CON0           0x0000
#define PMUGRF_SOC_CON0        0x0000
#define PMUGRF_SOC_CON4        0x0010
#define GPIO0C_IOMUX_SEL_1     0x0014

#define HIWORD_UPDATE(val, m)  (((m) << 16) | ((val) & (m)))

int board_early_init_f(void)
{

#if CONFIG_UART0_AS_SWD_INTERFACE
    /* 配置UART0作为SWD接口 */
    // GPIO0_C6/C7 -> JTAG_TCK_M1 / JTAG_TMS_M1
    writel(0xFF002200, GPIO0_IOC_BASE + GPIO0C_IOMUX_SEL_1); // IOC: C7=2, C6=2

    // grf_jtag_sel
    writel(HIWORD_UPDATE(0x0000, 0x0030), GRF_BASE + GRF_SOC_CON0);

    // 当RXD接低一段时间,也会切换.
    writel(0x0000FFFF, PMUGRF_BASE + PMUGRF_SOC_CON4);
    writel(0x00020002, PMUGRF_BASE + PMUGRF_SOC_CON0);
#endif

    return 0;
}
