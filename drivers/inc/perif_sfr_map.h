/* 
 * File:   perif_sfr_map.h
 * Author: mac_mitch
 *
 * Created on February 20, 2014, 8:41 PM
 */

#pragma once
#ifndef PERIF_SFR_MAP_H
#define	PERIF_SFR_MAP_H

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define ADD_NXT         0x04
#define SFR_BASE        0xBF800000
#define SPI3_BASE       0xBF805800
#define SPI2_BASE       0xBF805A00
#define SPI4_BASE       0xBF805C00
#define SPI1_BASE       0xBF805E00
#define UART1_BASE      0xBF806000
#define UART4_BASE      0xBF806200
#define UART3_BASE      0xBF806400
#define UART6_BASE      0xBF806600
#define UART2_BASE      0xBF806800
#define UART5_BASE      0xBF806A00
#define I2C3_BASE       0xBF805000
#define I2C4_BASE       0xBF805100
#define I2C5_BASE       0xBF805200
#define I2C1_BASE       0xBF805300
#define I2C2_BASE       0xBF805400

/*******************************************************************************
 * Type Defs
 ******************************************************************************/
// General typedefs
typedef volatile unsigned int REG;

typedef struct
{
  volatile unsigned int     reg;
  volatile unsigned int     clr;
  volatile unsigned int     set;
  volatile unsigned int     inv;
}REG_SET;

/*******************************************************************************
 * Registers (Public definitions)
 ******************************************************************************/

/*******************************************************************************
 * SPI
 ******************************************************************************/

/*******************************************************************************
 * UART
 ******************************************************************************/
// MODE bits
#define     MODE_STSEL(x)   (x << 0)
#define     MODE_PDSEL(x)   (x << 2)
#define     MODE_BRGH(x)    (x << 3)
#define     MODE_RXINV(x)   (x << 4)
#define     MODE_ABAUD(x)   (x << 5)
#define     MODE_LPBACK(x)  (x << 6)
#define     MODE_WAKE(x)    (x << 7)
#define     MODE_UEN(x)     (x << 9)
#define     MODE_RTSMD(x)   (x << 11)
#define     MODE_IREN(x)    (x << 12)
#define     MODE_SIDL(x)    (x << 13)
#define     MODE_ON(x)      (x << 15)

// STA bits
#define     STA_URXDA(x)    (x << 0)
#define     STA_OERR(x)     (x << 1)
#define     STA_FERR(x)     (x << 2)
#define     STA_PERR(x)     (x << 3)
#define     STA_RIDLE(x)    (x << 4)
#define     STA_ADDEN(x)    (x << 5)
#define     STA_URXISEL(x)  (x << 7)
#define     STA_TRMT(x)     (x << 8)
#define     STA_UTXBF(x)    (x << 9)
#define     STA_UTXEN(x)    (x << 10)
#define     STA_UTXBRK(x)   (x << 11)
#define     STA_URXEN(x)    (x << 12)
#define     STA_UTXINV(x)   (x << 13)
#define     STA_UTXISEL(x)  (x << 15)
#define     STA_ADDR(x)     (x << 23)
#define     STA_ADM_EN(x)   (x << 24)

/*******************************************************************************
 * I2C
 ******************************************************************************/
#define			I2C_CON_SEN(x)		(x << 0)
#define			I2C_CON_RSEN(x)		(x << 1)
#define			I2C_CON_PEN(x)		(x << 2)
#define			I2C_CON_RCEN(x)		(x << 3)
#define			I2C_CON_ACKEN(x)	(x << 4)
#define			I2C_CON_ACKDT(x)	(x << 5)
#define			I2C_CON_STREN(x)	(x << 6)
#define			I2C_CON_GCEN(x)		(x << 7)
#define 		I2C_CON_SMEN(x)		(x << 8)
#define 		I2C_CON_DISSLW(x)	(x << 9)
#define 		I2C_CON_A10M(x)		(x << 10)
#define 		I2C_CON_STRICT(x)	(x << 11)
#define 		I2C_CON_SCLREL(x)	(x << 12)
#define 		I2C_CON_SIDL(x)		(x << 13)
#define 		I2C_CON_ON(x)			(x << 15)
#define 		I2C_CON_DHEN(x)		(x << 16)
#define			I2C_CON_AHEN(x)		(x << 17)
#define			I2C_CON_SBCDE(x)	(x << 18)
#define 		I2C_CON_SDAHT(x)	(x << 19)
#define 		I2C_CON_BOEN(x)		(x << 19)
#define 		I2C_CON_SCIE(x)		(x << 19)
#define			I2C_CON_PCIE(x)		(x << 20)

#define			I2C_STAT_TBF(x)			(x << 0)
#define			I2C_STAT_RBF(x)			(x << 1)
#define                 I2C_STAT_RW(x)			(x << 2)
#define			I2C_STAT_S(x)				(x << 3)
#define			I2C_STAT_P(x)				(x << 4)
#define			I2C_STAT_DA(x)			(x << 5)
#define			I2C_STAT_I2COV(x)		(x << 6)
#define 		I2C_STAT_IWCOL(x)		(x << 7)
#define  		I2C_STAT_ADD10(x)		(x << 8)
#define			I2C_STAT_GCSTAT(x)	(x << 9)
#define			I2C_STAT_BCL(x)			(x << 10)
#define			I2C_STAT_ACKTIM(x)	(x << 13)
#define			I2C_STAT_TRSTAT(x)	(x << 14)
#define			I2C_STAT_ACKSTAT(x)	(x << 15)

#endif	/* PERIF_SFR_MAP_H */

