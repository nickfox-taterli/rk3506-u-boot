#include <common.h>
#include <asm/io.h>

#define GRF_BASE              0xFF288000U
#define PMUGRF_BASE           0xFF910000U
#define GPIO0_IOC_BASE        0xFF950000U

#define GRF_SOC_CON0          0x0000
#define PMUGRF_SOC_CON0       0x0000
#define PMUGRF_SOC_CON4       0x0010
#define GPIO0C_IOMUX_SEL_1    0x0014

/* UART0 寄存器(DW apb uart/16550 兼容) */
#define UART0_BASE            0xFF0A0000U
#define UART_RBR              0x00  /* 读: RBR */
#define UART_THR              0x00  /* 写: THR */
#define UART_DLL              0x00  /* DLAB=1 时 */
#define UART_IER              0x04  /* DLAB=0 时 */
#define UART_DLH              0x04  /* DLAB=1 时 */
#define UART_FCR              0x08
#define UART_IIR              0x08
#define UART_LCR              0x0C
#define UART_MCR              0x10
#define UART_LSR              0x14
#define UART_USR              0x7C  /* 部分 DW 版本有该寄存器,可选用 */

#define LCR_DLAB              (1U << 7)
#define LCR_8N1               0x03
#define FCR_FIFO_EN           (1U << 0)
#define FCR_RXSR              (1U << 1)  /* RX FIFO reset */
#define FCR_TXSR              (1U << 2)  /* TX FIFO reset */
#define LSR_THRE              (1U << 5)

#define HIWORD_UPDATE(val, m)  ( ((m) << 16) | ((val) & (m)) )

/* GPIO0C6/7 复用字段位于 GPIO0C_IOMUX_SEL_1 的 [11:8] / [15:12](每个 4bit) */
#define C6_SHIFT              8
#define C7_SHIFT              12
#define IOMUX_NIBBLE_MASK(n)  (0xFU << (n))
#define IOMUX_VAL(n, v)       ((unsigned)((v) & 0xF) << (n))

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
#elif !defined(CONFIG_ROCKCHIP_PRELOADER_SERIAL)
    writel(HIWORD_UPDATE(0x0000, 0x0030), GRF_BASE + GRF_SOC_CON0);  /* hiword: 0x0030<<16 有效 */

    const unsigned c6_mask = IOMUX_NIBBLE_MASK(C6_SHIFT);
    const unsigned c7_mask = IOMUX_NIBBLE_MASK(C7_SHIFT);
    const unsigned c6_uart = IOMUX_VAL(C6_SHIFT, 0x1);
    const unsigned c7_uart = IOMUX_VAL(C7_SHIFT, 0x1);
    writel( HIWORD_UPDATE(c6_uart, c6_mask) |
            HIWORD_UPDATE(c7_uart, c7_mask),
            GPIO0_IOC_BASE + GPIO0C_IOMUX_SEL_1 );

    writel(LCR_DLAB, UART0_BASE + UART_LCR);   /* 置 DLAB,切到 DLL/DLH 映射 */
    writel(0x01,    UART0_BASE + UART_DLL);    /* 24MHz / (16*1.5M) = 1 → DLL=1 */
    writel(0x00,    UART0_BASE + UART_DLH);

    writel(LCR_8N1, UART0_BASE + UART_LCR);    /* 清 DLAB,设置 8N1 */
    writel(FCR_FIFO_EN | FCR_RXSR | FCR_TXSR, UART0_BASE + UART_FCR); /* 使能并复位 FIFO */
#endif
    return 0;
}
