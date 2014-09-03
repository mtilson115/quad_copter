/*******************************************************************************
 * File:    perif_sfr_map.c
 * Author:  Mitchell S. Tilson
 * Created: 02/20/2014
 *
 * Description: This file defines the sfrs that I'm using and I'm interested in.
 ******************************************************************************/

#include "perif_sfr_map.h"

#if defined (__LANGUAGE_C__) || defined (__LANGUAGE_C_PLUS_PLUS)

#ifdef __cplusplus
extern "C" {
#endif

// SPI CON registers
volatile SPI_CON_type SPI3_CON_bits __attribute__((section("SPI3_CON"), address (SPI3_BASE)));
volatile SPI_CON_type SPI2_CON_bits __attribute__((section("SPI2_CON"), address (SPI2_BASE)));
volatile SPI_CON_type SPI4_CON_bits __attribute__((section("SPI4_CON"), address (SPI4_BASE)));
volatile SPI_CON_type SPI1_CON_bits __attribute__((section("SPI1_CON"), address (SPI1_BASE)));

// SPI STAT registers
volatile SPI_STAT_type SPI3_STAT_bits __attribute__((section("SPI3_STAT"), address (SPI3_BASE+0x10)));
volatile SPI_STAT_type SPI2_STAT_bits __attribute__((section("SPI2_STAT"), address (SPI2_BASE+0x10)));
volatile SPI_STAT_type SPI4_STAT_bits __attribute__((section("SPI4_STAT"), address (SPI4_BASE+0x10)));
volatile SPI_STAT_type SPI1_STAT_bits __attribute__((section("SPI1_STAT"), address (SPI1_BASE+0x10)));

// SPI BUF registers
volatile REGISTER SPI3_BUFF __attribute__((section("SPI3_BUF"), address (SPI3_BASE+0x20)));
volatile REGISTER SPI2_BUFF __attribute__((section("SPI2_BUF"), address (SPI2_BASE+0x20)));
volatile REGISTER SPI4_BUFF __attribute__((section("SPI4_BUF"), address (SPI4_BASE+0x20)));
volatile REGISTER SPI1_BUFF __attribute__((section("SPI1_BUF"), address (SPI1_BASE+0x20)));

// SPI BRG registers
volatile SPI_BRG_type SPI3_BRG __attribute__((section("SPI3_BRG"), address (SPI3_BASE+0x30)));
volatile SPI_BRG_type SPI2_BRG __attribute__((section("SPI2_BRG"), address (SPI2_BASE+0x30)));
volatile SPI_BRG_type SPI4_BRG __attribute__((section("SPI4_BRG"), address (SPI4_BASE+0x30)));
volatile SPI_BRG_type SPI1_BRG __attribute__((section("SPI1_BRG"), address (SPI1_BASE+0x30)));

// UART MODE registers
volatile UART_MODE_type UART1_MODE __attribute__ ((section("UART1_MODE"), address(UART1_BASE)));
volatile UART_MODE_type UART2_MODE __attribute__ ((section("UART2_MODE"), address(UART2_BASE)));
volatile UART_MODE_type UART3_MODE __attribute__ ((section("UART3_MODE"), address(UART3_BASE)));
volatile UART_MODE_type UART4_MODE __attribute__ ((section("UART4_MODE"), address(UART4_BASE)));
volatile UART_MODE_type UART5_MODE __attribute__ ((section("UART5_MODE"), address(UART5_BASE)));
volatile UART_MODE_type UART6_MODE __attribute__ ((section("UART6_MODE"), address(UART6_BASE)));

// UART Status registers
volatile UART_STAT_type UART1_STAT __attribute__ ((section("UART1_STAT"), address(UART1_BASE+0x10)));
volatile UART_STAT_type UART2_STAT __attribute__ ((section("UART2_STAT"), address(UART2_BASE+0x10)));
volatile UART_STAT_type UART3_STAT __attribute__ ((section("UART3_STAT"), address(UART3_BASE+0x10)));
volatile UART_STAT_type UART4_STAT __attribute__ ((section("UART4_STAT"), address(UART4_BASE+0x10)));
volatile UART_STAT_type UART5_STAT __attribute__ ((section("UART5_STAT"), address(UART5_BASE+0x10)));
volatile UART_STAT_type UART6_STAT __attribute__ ((section("UART6_STAT"), address(UART6_BASE+0x10)));

// UART TX buffer
volatile UART_BUF_REG_type UART1_TX __attribute__ ((section("UART1_TX"), address(UART1_BASE+0x20)));
volatile UART_BUF_REG_type UART2_TX __attribute__ ((section("UART2_TX"), address(UART2_BASE+0x20)));
volatile UART_BUF_REG_type UART3_TX __attribute__ ((section("UART3_TX"), address(UART3_BASE+0x20)));
volatile UART_BUF_REG_type UART4_TX __attribute__ ((section("UART4_TX"), address(UART4_BASE+0x20)));
volatile UART_BUF_REG_type UART5_TX __attribute__ ((section("UART5_TX"), address(UART5_BASE+0x20)));
volatile UART_BUF_REG_type UART6_TX __attribute__ ((section("UART6_TX"), address(UART6_BASE+0x20)));

// UART RX buffer
volatile UART_BUF_REG_type UART1_RX __attribute__ ((section("UART1_RX"), address(UART1_BASE+0x30)));
volatile UART_BUF_REG_type UART2_RX __attribute__ ((section("UART2_RX"), address(UART2_BASE+0x30)));
volatile UART_BUF_REG_type UART3_RX __attribute__ ((section("UART3_RX"), address(UART3_BASE+0x30)));
volatile UART_BUF_REG_type UART4_RX __attribute__ ((section("UART4_RX"), address(UART4_BASE+0x30)));
volatile UART_BUF_REG_type UART5_RX __attribute__ ((section("UART5_RX"), address(UART5_BASE+0x30)));
volatile UART_BUF_REG_type UART6_RX __attribute__ ((section("UART6_RX"), address(UART6_BASE+0x30)));

// UART BRG
volatile UART_BRG_type UART1_BRG __attribute__ ((section("UART1_BRG"), address(UART1_BASE+0x40)));
volatile UART_BRG_type UART2_BRG __attribute__ ((section("UART2_BRG"), address(UART2_BASE+0x40)));
volatile UART_BRG_type UART3_BRG __attribute__ ((section("UART3_BRG"), address(UART3_BASE+0x40)));
volatile UART_BRG_type UART4_BRG __attribute__ ((section("UART4_BRG"), address(UART4_BASE+0x40)));
volatile UART_BRG_type UART5_BRG __attribute__ ((section("UART5_BRG"), address(UART5_BASE+0x40)));
volatile UART_BRG_type UART6_BRG __attribute__ ((section("UART6_BRG"), address(UART6_BASE+0x40)));

#ifdef __cplusplus
}
#endif

#endif


