/***************************************************************************//**
* \file LIN_1_SCB_PINS.h
* \version 3.20
*
* \brief
*  This file provides constants and parameter values for the pin components
*  buried into SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PINS_LIN_1_SCB_H)
#define CY_SCB_PINS_LIN_1_SCB_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Unconfigured pins */
#define LIN_1_SCB_REMOVE_RX_WAKE_SCL_MOSI_PIN  (1u)
#define LIN_1_SCB_REMOVE_RX_SCL_MOSI_PIN      (1u)
#define LIN_1_SCB_REMOVE_TX_SDA_MISO_PIN      (1u)
#define LIN_1_SCB_REMOVE_CTS_SCLK_PIN      (1u)
#define LIN_1_SCB_REMOVE_RTS_SS0_PIN      (1u)
#define LIN_1_SCB_REMOVE_SS1_PIN                 (1u)
#define LIN_1_SCB_REMOVE_SS2_PIN                 (1u)
#define LIN_1_SCB_REMOVE_SS3_PIN                 (1u)

/* Mode defined pins */
#define LIN_1_SCB_REMOVE_I2C_PINS                (1u)
#define LIN_1_SCB_REMOVE_SPI_MASTER_PINS         (1u)
#define LIN_1_SCB_REMOVE_SPI_MASTER_SCLK_PIN     (1u)
#define LIN_1_SCB_REMOVE_SPI_MASTER_MOSI_PIN     (1u)
#define LIN_1_SCB_REMOVE_SPI_MASTER_MISO_PIN     (1u)
#define LIN_1_SCB_REMOVE_SPI_MASTER_SS0_PIN      (1u)
#define LIN_1_SCB_REMOVE_SPI_MASTER_SS1_PIN      (1u)
#define LIN_1_SCB_REMOVE_SPI_MASTER_SS2_PIN      (1u)
#define LIN_1_SCB_REMOVE_SPI_MASTER_SS3_PIN      (1u)
#define LIN_1_SCB_REMOVE_SPI_SLAVE_PINS          (1u)
#define LIN_1_SCB_REMOVE_SPI_SLAVE_MOSI_PIN      (1u)
#define LIN_1_SCB_REMOVE_SPI_SLAVE_MISO_PIN      (1u)
#define LIN_1_SCB_REMOVE_UART_TX_PIN             (0u)
#define LIN_1_SCB_REMOVE_UART_RX_TX_PIN          (1u)
#define LIN_1_SCB_REMOVE_UART_RX_PIN             (0u)
#define LIN_1_SCB_REMOVE_UART_RX_WAKE_PIN        (1u)
#define LIN_1_SCB_REMOVE_UART_RTS_PIN            (1u)
#define LIN_1_SCB_REMOVE_UART_CTS_PIN            (1u)

/* Unconfigured pins */
#define LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN (0u == LIN_1_SCB_REMOVE_RX_WAKE_SCL_MOSI_PIN)
#define LIN_1_SCB_RX_SCL_MOSI_PIN     (0u == LIN_1_SCB_REMOVE_RX_SCL_MOSI_PIN)
#define LIN_1_SCB_TX_SDA_MISO_PIN     (0u == LIN_1_SCB_REMOVE_TX_SDA_MISO_PIN)
#define LIN_1_SCB_CTS_SCLK_PIN     (0u == LIN_1_SCB_REMOVE_CTS_SCLK_PIN)
#define LIN_1_SCB_RTS_SS0_PIN     (0u == LIN_1_SCB_REMOVE_RTS_SS0_PIN)
#define LIN_1_SCB_SS1_PIN                (0u == LIN_1_SCB_REMOVE_SS1_PIN)
#define LIN_1_SCB_SS2_PIN                (0u == LIN_1_SCB_REMOVE_SS2_PIN)
#define LIN_1_SCB_SS3_PIN                (0u == LIN_1_SCB_REMOVE_SS3_PIN)

/* Mode defined pins */
#define LIN_1_SCB_I2C_PINS               (0u == LIN_1_SCB_REMOVE_I2C_PINS)
#define LIN_1_SCB_SPI_MASTER_PINS        (0u == LIN_1_SCB_REMOVE_SPI_MASTER_PINS)
#define LIN_1_SCB_SPI_MASTER_SCLK_PIN    (0u == LIN_1_SCB_REMOVE_SPI_MASTER_SCLK_PIN)
#define LIN_1_SCB_SPI_MASTER_MOSI_PIN    (0u == LIN_1_SCB_REMOVE_SPI_MASTER_MOSI_PIN)
#define LIN_1_SCB_SPI_MASTER_MISO_PIN    (0u == LIN_1_SCB_REMOVE_SPI_MASTER_MISO_PIN)
#define LIN_1_SCB_SPI_MASTER_SS0_PIN     (0u == LIN_1_SCB_REMOVE_SPI_MASTER_SS0_PIN)
#define LIN_1_SCB_SPI_MASTER_SS1_PIN     (0u == LIN_1_SCB_REMOVE_SPI_MASTER_SS1_PIN)
#define LIN_1_SCB_SPI_MASTER_SS2_PIN     (0u == LIN_1_SCB_REMOVE_SPI_MASTER_SS2_PIN)
#define LIN_1_SCB_SPI_MASTER_SS3_PIN     (0u == LIN_1_SCB_REMOVE_SPI_MASTER_SS3_PIN)
#define LIN_1_SCB_SPI_SLAVE_PINS         (0u == LIN_1_SCB_REMOVE_SPI_SLAVE_PINS)
#define LIN_1_SCB_SPI_SLAVE_MOSI_PIN     (0u == LIN_1_SCB_REMOVE_SPI_SLAVE_MOSI_PIN)
#define LIN_1_SCB_SPI_SLAVE_MISO_PIN     (0u == LIN_1_SCB_REMOVE_SPI_SLAVE_MISO_PIN)
#define LIN_1_SCB_UART_TX_PIN            (0u == LIN_1_SCB_REMOVE_UART_TX_PIN)
#define LIN_1_SCB_UART_RX_TX_PIN         (0u == LIN_1_SCB_REMOVE_UART_RX_TX_PIN)
#define LIN_1_SCB_UART_RX_PIN            (0u == LIN_1_SCB_REMOVE_UART_RX_PIN)
#define LIN_1_SCB_UART_RX_WAKE_PIN       (0u == LIN_1_SCB_REMOVE_UART_RX_WAKE_PIN)
#define LIN_1_SCB_UART_RTS_PIN           (0u == LIN_1_SCB_REMOVE_UART_RTS_PIN)
#define LIN_1_SCB_UART_CTS_PIN           (0u == LIN_1_SCB_REMOVE_UART_CTS_PIN)


/***************************************
*             Includes
****************************************/

#if (LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN)
    #include "LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi.h"
#endif /* (LIN_1_SCB_RX_SCL_MOSI) */

#if (LIN_1_SCB_RX_SCL_MOSI_PIN)
    #include "LIN_1_SCB_uart_rx_i2c_scl_spi_mosi.h"
#endif /* (LIN_1_SCB_RX_SCL_MOSI) */

#if (LIN_1_SCB_TX_SDA_MISO_PIN)
    #include "LIN_1_SCB_uart_tx_i2c_sda_spi_miso.h"
#endif /* (LIN_1_SCB_TX_SDA_MISO) */

#if (LIN_1_SCB_CTS_SCLK_PIN)
    #include "LIN_1_SCB_uart_cts_spi_sclk.h"
#endif /* (LIN_1_SCB_CTS_SCLK) */

#if (LIN_1_SCB_RTS_SS0_PIN)
    #include "LIN_1_SCB_uart_rts_spi_ss0.h"
#endif /* (LIN_1_SCB_RTS_SS0_PIN) */

#if (LIN_1_SCB_SS1_PIN)
    #include "LIN_1_SCB_spi_ss1.h"
#endif /* (LIN_1_SCB_SS1_PIN) */

#if (LIN_1_SCB_SS2_PIN)
    #include "LIN_1_SCB_spi_ss2.h"
#endif /* (LIN_1_SCB_SS2_PIN) */

#if (LIN_1_SCB_SS3_PIN)
    #include "LIN_1_SCB_spi_ss3.h"
#endif /* (LIN_1_SCB_SS3_PIN) */

#if (LIN_1_SCB_I2C_PINS)
    #include "LIN_1_SCB_scl.h"
    #include "LIN_1_SCB_sda.h"
#endif /* (LIN_1_SCB_I2C_PINS) */

#if (LIN_1_SCB_SPI_MASTER_PINS)
#if (LIN_1_SCB_SPI_MASTER_SCLK_PIN)
    #include "LIN_1_SCB_sclk_m.h"
#endif /* (LIN_1_SCB_SPI_MASTER_SCLK_PIN) */

#if (LIN_1_SCB_SPI_MASTER_MOSI_PIN)
    #include "LIN_1_SCB_mosi_m.h"
#endif /* (LIN_1_SCB_SPI_MASTER_MOSI_PIN) */

#if (LIN_1_SCB_SPI_MASTER_MISO_PIN)
    #include "LIN_1_SCB_miso_m.h"
#endif /*(LIN_1_SCB_SPI_MASTER_MISO_PIN) */
#endif /* (LIN_1_SCB_SPI_MASTER_PINS) */

#if (LIN_1_SCB_SPI_SLAVE_PINS)
    #include "LIN_1_SCB_sclk_s.h"
    #include "LIN_1_SCB_ss_s.h"

#if (LIN_1_SCB_SPI_SLAVE_MOSI_PIN)
    #include "LIN_1_SCB_mosi_s.h"
#endif /* (LIN_1_SCB_SPI_SLAVE_MOSI_PIN) */

#if (LIN_1_SCB_SPI_SLAVE_MISO_PIN)
    #include "LIN_1_SCB_miso_s.h"
#endif /*(LIN_1_SCB_SPI_SLAVE_MISO_PIN) */
#endif /* (LIN_1_SCB_SPI_SLAVE_PINS) */

#if (LIN_1_SCB_SPI_MASTER_SS0_PIN)
    #include "LIN_1_SCB_ss0_m.h"
#endif /* (LIN_1_SCB_SPI_MASTER_SS0_PIN) */

#if (LIN_1_SCB_SPI_MASTER_SS1_PIN)
    #include "LIN_1_SCB_ss1_m.h"
#endif /* (LIN_1_SCB_SPI_MASTER_SS1_PIN) */

#if (LIN_1_SCB_SPI_MASTER_SS2_PIN)
    #include "LIN_1_SCB_ss2_m.h"
#endif /* (LIN_1_SCB_SPI_MASTER_SS2_PIN) */

#if (LIN_1_SCB_SPI_MASTER_SS3_PIN)
    #include "LIN_1_SCB_ss3_m.h"
#endif /* (LIN_1_SCB_SPI_MASTER_SS3_PIN) */

#if (LIN_1_SCB_UART_TX_PIN)
    #include "LIN_1_SCB_tx.h"
#endif /* (LIN_1_SCB_UART_TX_PIN) */

#if (LIN_1_SCB_UART_RX_TX_PIN)
    #include "LIN_1_SCB_rx_tx.h"
#endif /* (LIN_1_SCB_UART_RX_TX_PIN) */

#if (LIN_1_SCB_UART_RX_PIN)
    #include "LIN_1_SCB_rx.h"
#endif /* (LIN_1_SCB_UART_RX_PIN) */

#if (LIN_1_SCB_UART_RX_WAKE_PIN)
    #include "LIN_1_SCB_rx_wake.h"
#endif /* (LIN_1_SCB_UART_RX_WAKE_PIN) */

#if (LIN_1_SCB_UART_RTS_PIN)
    #include "LIN_1_SCB_rts.h"
#endif /* (LIN_1_SCB_UART_RTS_PIN) */

#if (LIN_1_SCB_UART_CTS_PIN)
    #include "LIN_1_SCB_cts.h"
#endif /* (LIN_1_SCB_UART_CTS_PIN) */


/***************************************
*              Registers
***************************************/

#if (LIN_1_SCB_RX_SCL_MOSI_PIN)
    #define LIN_1_SCB_RX_SCL_MOSI_HSIOM_REG   (*(reg32 *) LIN_1_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM)
    #define LIN_1_SCB_RX_SCL_MOSI_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM)
    
    #define LIN_1_SCB_RX_SCL_MOSI_HSIOM_MASK      (LIN_1_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM_MASK)
    #define LIN_1_SCB_RX_SCL_MOSI_HSIOM_POS       (LIN_1_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM_SHIFT)
    #define LIN_1_SCB_RX_SCL_MOSI_HSIOM_SEL_GPIO  (LIN_1_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM_GPIO)
    #define LIN_1_SCB_RX_SCL_MOSI_HSIOM_SEL_I2C   (LIN_1_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM_I2C)
    #define LIN_1_SCB_RX_SCL_MOSI_HSIOM_SEL_SPI   (LIN_1_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM_SPI)
    #define LIN_1_SCB_RX_SCL_MOSI_HSIOM_SEL_UART  (LIN_1_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM_UART)
    
#elif (LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN)
    #define LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG   (*(reg32 *) LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM)
    #define LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM)
    
    #define LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_MASK      (LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_MASK)
    #define LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_POS       (LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_SHIFT)
    #define LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_SEL_GPIO  (LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_GPIO)
    #define LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C   (LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_I2C)
    #define LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI   (LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_SPI)
    #define LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART  (LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_UART)    
   
    #define LIN_1_SCB_RX_WAKE_SCL_MOSI_INTCFG_REG (*(reg32 *) LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__INTCFG)
    #define LIN_1_SCB_RX_WAKE_SCL_MOSI_INTCFG_PTR ( (reg32 *) LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__INTCFG)
    #define LIN_1_SCB_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS  (LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi__SHIFT)
    #define LIN_1_SCB_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK ((uint32) LIN_1_SCB_INTCFG_TYPE_MASK << \
                                                                           LIN_1_SCB_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS)
#else
    /* None of pins LIN_1_SCB_RX_SCL_MOSI_PIN or LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN present.*/
#endif /* (LIN_1_SCB_RX_SCL_MOSI_PIN) */

#if (LIN_1_SCB_TX_SDA_MISO_PIN)
    #define LIN_1_SCB_TX_SDA_MISO_HSIOM_REG   (*(reg32 *) LIN_1_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM)
    #define LIN_1_SCB_TX_SDA_MISO_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM)
    
    #define LIN_1_SCB_TX_SDA_MISO_HSIOM_MASK      (LIN_1_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM_MASK)
    #define LIN_1_SCB_TX_SDA_MISO_HSIOM_POS       (LIN_1_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM_SHIFT)
    #define LIN_1_SCB_TX_SDA_MISO_HSIOM_SEL_GPIO  (LIN_1_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM_GPIO)
    #define LIN_1_SCB_TX_SDA_MISO_HSIOM_SEL_I2C   (LIN_1_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM_I2C)
    #define LIN_1_SCB_TX_SDA_MISO_HSIOM_SEL_SPI   (LIN_1_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM_SPI)
    #define LIN_1_SCB_TX_SDA_MISO_HSIOM_SEL_UART  (LIN_1_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM_UART)
#endif /* (LIN_1_SCB_TX_SDA_MISO_PIN) */

#if (LIN_1_SCB_CTS_SCLK_PIN)
    #define LIN_1_SCB_CTS_SCLK_HSIOM_REG   (*(reg32 *) LIN_1_SCB_uart_cts_spi_sclk__0__HSIOM)
    #define LIN_1_SCB_CTS_SCLK_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_uart_cts_spi_sclk__0__HSIOM)
    
    #define LIN_1_SCB_CTS_SCLK_HSIOM_MASK      (LIN_1_SCB_uart_cts_spi_sclk__0__HSIOM_MASK)
    #define LIN_1_SCB_CTS_SCLK_HSIOM_POS       (LIN_1_SCB_uart_cts_spi_sclk__0__HSIOM_SHIFT)
    #define LIN_1_SCB_CTS_SCLK_HSIOM_SEL_GPIO  (LIN_1_SCB_uart_cts_spi_sclk__0__HSIOM_GPIO)
    #define LIN_1_SCB_CTS_SCLK_HSIOM_SEL_I2C   (LIN_1_SCB_uart_cts_spi_sclk__0__HSIOM_I2C)
    #define LIN_1_SCB_CTS_SCLK_HSIOM_SEL_SPI   (LIN_1_SCB_uart_cts_spi_sclk__0__HSIOM_SPI)
    #define LIN_1_SCB_CTS_SCLK_HSIOM_SEL_UART  (LIN_1_SCB_uart_cts_spi_sclk__0__HSIOM_UART)
#endif /* (LIN_1_SCB_CTS_SCLK_PIN) */

#if (LIN_1_SCB_RTS_SS0_PIN)
    #define LIN_1_SCB_RTS_SS0_HSIOM_REG   (*(reg32 *) LIN_1_SCB_uart_rts_spi_ss0__0__HSIOM)
    #define LIN_1_SCB_RTS_SS0_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_uart_rts_spi_ss0__0__HSIOM)
    
    #define LIN_1_SCB_RTS_SS0_HSIOM_MASK      (LIN_1_SCB_uart_rts_spi_ss0__0__HSIOM_MASK)
    #define LIN_1_SCB_RTS_SS0_HSIOM_POS       (LIN_1_SCB_uart_rts_spi_ss0__0__HSIOM_SHIFT)
    #define LIN_1_SCB_RTS_SS0_HSIOM_SEL_GPIO  (LIN_1_SCB_uart_rts_spi_ss0__0__HSIOM_GPIO)
    #define LIN_1_SCB_RTS_SS0_HSIOM_SEL_I2C   (LIN_1_SCB_uart_rts_spi_ss0__0__HSIOM_I2C)
    #define LIN_1_SCB_RTS_SS0_HSIOM_SEL_SPI   (LIN_1_SCB_uart_rts_spi_ss0__0__HSIOM_SPI)
#if !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1)
    #define LIN_1_SCB_RTS_SS0_HSIOM_SEL_UART  (LIN_1_SCB_uart_rts_spi_ss0__0__HSIOM_UART)
#endif /* !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1) */
#endif /* (LIN_1_SCB_RTS_SS0_PIN) */

#if (LIN_1_SCB_SS1_PIN)
    #define LIN_1_SCB_SS1_HSIOM_REG  (*(reg32 *) LIN_1_SCB_spi_ss1__0__HSIOM)
    #define LIN_1_SCB_SS1_HSIOM_PTR  ( (reg32 *) LIN_1_SCB_spi_ss1__0__HSIOM)
    
    #define LIN_1_SCB_SS1_HSIOM_MASK     (LIN_1_SCB_spi_ss1__0__HSIOM_MASK)
    #define LIN_1_SCB_SS1_HSIOM_POS      (LIN_1_SCB_spi_ss1__0__HSIOM_SHIFT)
    #define LIN_1_SCB_SS1_HSIOM_SEL_GPIO (LIN_1_SCB_spi_ss1__0__HSIOM_GPIO)
    #define LIN_1_SCB_SS1_HSIOM_SEL_I2C  (LIN_1_SCB_spi_ss1__0__HSIOM_I2C)
    #define LIN_1_SCB_SS1_HSIOM_SEL_SPI  (LIN_1_SCB_spi_ss1__0__HSIOM_SPI)
#endif /* (LIN_1_SCB_SS1_PIN) */

#if (LIN_1_SCB_SS2_PIN)
    #define LIN_1_SCB_SS2_HSIOM_REG     (*(reg32 *) LIN_1_SCB_spi_ss2__0__HSIOM)
    #define LIN_1_SCB_SS2_HSIOM_PTR     ( (reg32 *) LIN_1_SCB_spi_ss2__0__HSIOM)
    
    #define LIN_1_SCB_SS2_HSIOM_MASK     (LIN_1_SCB_spi_ss2__0__HSIOM_MASK)
    #define LIN_1_SCB_SS2_HSIOM_POS      (LIN_1_SCB_spi_ss2__0__HSIOM_SHIFT)
    #define LIN_1_SCB_SS2_HSIOM_SEL_GPIO (LIN_1_SCB_spi_ss2__0__HSIOM_GPIO)
    #define LIN_1_SCB_SS2_HSIOM_SEL_I2C  (LIN_1_SCB_spi_ss2__0__HSIOM_I2C)
    #define LIN_1_SCB_SS2_HSIOM_SEL_SPI  (LIN_1_SCB_spi_ss2__0__HSIOM_SPI)
#endif /* (LIN_1_SCB_SS2_PIN) */

#if (LIN_1_SCB_SS3_PIN)
    #define LIN_1_SCB_SS3_HSIOM_REG     (*(reg32 *) LIN_1_SCB_spi_ss3__0__HSIOM)
    #define LIN_1_SCB_SS3_HSIOM_PTR     ( (reg32 *) LIN_1_SCB_spi_ss3__0__HSIOM)
    
    #define LIN_1_SCB_SS3_HSIOM_MASK     (LIN_1_SCB_spi_ss3__0__HSIOM_MASK)
    #define LIN_1_SCB_SS3_HSIOM_POS      (LIN_1_SCB_spi_ss3__0__HSIOM_SHIFT)
    #define LIN_1_SCB_SS3_HSIOM_SEL_GPIO (LIN_1_SCB_spi_ss3__0__HSIOM_GPIO)
    #define LIN_1_SCB_SS3_HSIOM_SEL_I2C  (LIN_1_SCB_spi_ss3__0__HSIOM_I2C)
    #define LIN_1_SCB_SS3_HSIOM_SEL_SPI  (LIN_1_SCB_spi_ss3__0__HSIOM_SPI)
#endif /* (LIN_1_SCB_SS3_PIN) */

#if (LIN_1_SCB_I2C_PINS)
    #define LIN_1_SCB_SCL_HSIOM_REG  (*(reg32 *) LIN_1_SCB_scl__0__HSIOM)
    #define LIN_1_SCB_SCL_HSIOM_PTR  ( (reg32 *) LIN_1_SCB_scl__0__HSIOM)
    
    #define LIN_1_SCB_SCL_HSIOM_MASK     (LIN_1_SCB_scl__0__HSIOM_MASK)
    #define LIN_1_SCB_SCL_HSIOM_POS      (LIN_1_SCB_scl__0__HSIOM_SHIFT)
    #define LIN_1_SCB_SCL_HSIOM_SEL_GPIO (LIN_1_SCB_sda__0__HSIOM_GPIO)
    #define LIN_1_SCB_SCL_HSIOM_SEL_I2C  (LIN_1_SCB_sda__0__HSIOM_I2C)
    
    #define LIN_1_SCB_SDA_HSIOM_REG  (*(reg32 *) LIN_1_SCB_sda__0__HSIOM)
    #define LIN_1_SCB_SDA_HSIOM_PTR  ( (reg32 *) LIN_1_SCB_sda__0__HSIOM)
    
    #define LIN_1_SCB_SDA_HSIOM_MASK     (LIN_1_SCB_sda__0__HSIOM_MASK)
    #define LIN_1_SCB_SDA_HSIOM_POS      (LIN_1_SCB_sda__0__HSIOM_SHIFT)
    #define LIN_1_SCB_SDA_HSIOM_SEL_GPIO (LIN_1_SCB_sda__0__HSIOM_GPIO)
    #define LIN_1_SCB_SDA_HSIOM_SEL_I2C  (LIN_1_SCB_sda__0__HSIOM_I2C)
#endif /* (LIN_1_SCB_I2C_PINS) */

#if (LIN_1_SCB_SPI_SLAVE_PINS)
    #define LIN_1_SCB_SCLK_S_HSIOM_REG   (*(reg32 *) LIN_1_SCB_sclk_s__0__HSIOM)
    #define LIN_1_SCB_SCLK_S_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_sclk_s__0__HSIOM)
    
    #define LIN_1_SCB_SCLK_S_HSIOM_MASK      (LIN_1_SCB_sclk_s__0__HSIOM_MASK)
    #define LIN_1_SCB_SCLK_S_HSIOM_POS       (LIN_1_SCB_sclk_s__0__HSIOM_SHIFT)
    #define LIN_1_SCB_SCLK_S_HSIOM_SEL_GPIO  (LIN_1_SCB_sclk_s__0__HSIOM_GPIO)
    #define LIN_1_SCB_SCLK_S_HSIOM_SEL_SPI   (LIN_1_SCB_sclk_s__0__HSIOM_SPI)
    
    #define LIN_1_SCB_SS0_S_HSIOM_REG    (*(reg32 *) LIN_1_SCB_ss0_s__0__HSIOM)
    #define LIN_1_SCB_SS0_S_HSIOM_PTR    ( (reg32 *) LIN_1_SCB_ss0_s__0__HSIOM)
    
    #define LIN_1_SCB_SS0_S_HSIOM_MASK       (LIN_1_SCB_ss0_s__0__HSIOM_MASK)
    #define LIN_1_SCB_SS0_S_HSIOM_POS        (LIN_1_SCB_ss0_s__0__HSIOM_SHIFT)
    #define LIN_1_SCB_SS0_S_HSIOM_SEL_GPIO   (LIN_1_SCB_ss0_s__0__HSIOM_GPIO)  
    #define LIN_1_SCB_SS0_S_HSIOM_SEL_SPI    (LIN_1_SCB_ss0_s__0__HSIOM_SPI)
#endif /* (LIN_1_SCB_SPI_SLAVE_PINS) */

#if (LIN_1_SCB_SPI_SLAVE_MOSI_PIN)
    #define LIN_1_SCB_MOSI_S_HSIOM_REG   (*(reg32 *) LIN_1_SCB_mosi_s__0__HSIOM)
    #define LIN_1_SCB_MOSI_S_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_mosi_s__0__HSIOM)
    
    #define LIN_1_SCB_MOSI_S_HSIOM_MASK      (LIN_1_SCB_mosi_s__0__HSIOM_MASK)
    #define LIN_1_SCB_MOSI_S_HSIOM_POS       (LIN_1_SCB_mosi_s__0__HSIOM_SHIFT)
    #define LIN_1_SCB_MOSI_S_HSIOM_SEL_GPIO  (LIN_1_SCB_mosi_s__0__HSIOM_GPIO)
    #define LIN_1_SCB_MOSI_S_HSIOM_SEL_SPI   (LIN_1_SCB_mosi_s__0__HSIOM_SPI)
#endif /* (LIN_1_SCB_SPI_SLAVE_MOSI_PIN) */

#if (LIN_1_SCB_SPI_SLAVE_MISO_PIN)
    #define LIN_1_SCB_MISO_S_HSIOM_REG   (*(reg32 *) LIN_1_SCB_miso_s__0__HSIOM)
    #define LIN_1_SCB_MISO_S_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_miso_s__0__HSIOM)
    
    #define LIN_1_SCB_MISO_S_HSIOM_MASK      (LIN_1_SCB_miso_s__0__HSIOM_MASK)
    #define LIN_1_SCB_MISO_S_HSIOM_POS       (LIN_1_SCB_miso_s__0__HSIOM_SHIFT)
    #define LIN_1_SCB_MISO_S_HSIOM_SEL_GPIO  (LIN_1_SCB_miso_s__0__HSIOM_GPIO)
    #define LIN_1_SCB_MISO_S_HSIOM_SEL_SPI   (LIN_1_SCB_miso_s__0__HSIOM_SPI)
#endif /* (LIN_1_SCB_SPI_SLAVE_MISO_PIN) */

#if (LIN_1_SCB_SPI_MASTER_MISO_PIN)
    #define LIN_1_SCB_MISO_M_HSIOM_REG   (*(reg32 *) LIN_1_SCB_miso_m__0__HSIOM)
    #define LIN_1_SCB_MISO_M_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_miso_m__0__HSIOM)
    
    #define LIN_1_SCB_MISO_M_HSIOM_MASK      (LIN_1_SCB_miso_m__0__HSIOM_MASK)
    #define LIN_1_SCB_MISO_M_HSIOM_POS       (LIN_1_SCB_miso_m__0__HSIOM_SHIFT)
    #define LIN_1_SCB_MISO_M_HSIOM_SEL_GPIO  (LIN_1_SCB_miso_m__0__HSIOM_GPIO)
    #define LIN_1_SCB_MISO_M_HSIOM_SEL_SPI   (LIN_1_SCB_miso_m__0__HSIOM_SPI)
#endif /* (LIN_1_SCB_SPI_MASTER_MISO_PIN) */

#if (LIN_1_SCB_SPI_MASTER_MOSI_PIN)
    #define LIN_1_SCB_MOSI_M_HSIOM_REG   (*(reg32 *) LIN_1_SCB_mosi_m__0__HSIOM)
    #define LIN_1_SCB_MOSI_M_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_mosi_m__0__HSIOM)
    
    #define LIN_1_SCB_MOSI_M_HSIOM_MASK      (LIN_1_SCB_mosi_m__0__HSIOM_MASK)
    #define LIN_1_SCB_MOSI_M_HSIOM_POS       (LIN_1_SCB_mosi_m__0__HSIOM_SHIFT)
    #define LIN_1_SCB_MOSI_M_HSIOM_SEL_GPIO  (LIN_1_SCB_mosi_m__0__HSIOM_GPIO)
    #define LIN_1_SCB_MOSI_M_HSIOM_SEL_SPI   (LIN_1_SCB_mosi_m__0__HSIOM_SPI)
#endif /* (LIN_1_SCB_SPI_MASTER_MOSI_PIN) */

#if (LIN_1_SCB_SPI_MASTER_SCLK_PIN)
    #define LIN_1_SCB_SCLK_M_HSIOM_REG   (*(reg32 *) LIN_1_SCB_sclk_m__0__HSIOM)
    #define LIN_1_SCB_SCLK_M_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_sclk_m__0__HSIOM)
    
    #define LIN_1_SCB_SCLK_M_HSIOM_MASK      (LIN_1_SCB_sclk_m__0__HSIOM_MASK)
    #define LIN_1_SCB_SCLK_M_HSIOM_POS       (LIN_1_SCB_sclk_m__0__HSIOM_SHIFT)
    #define LIN_1_SCB_SCLK_M_HSIOM_SEL_GPIO  (LIN_1_SCB_sclk_m__0__HSIOM_GPIO)
    #define LIN_1_SCB_SCLK_M_HSIOM_SEL_SPI   (LIN_1_SCB_sclk_m__0__HSIOM_SPI)
#endif /* (LIN_1_SCB_SPI_MASTER_SCLK_PIN) */

#if (LIN_1_SCB_SPI_MASTER_SS0_PIN)
    #define LIN_1_SCB_SS0_M_HSIOM_REG    (*(reg32 *) LIN_1_SCB_ss0_m__0__HSIOM)
    #define LIN_1_SCB_SS0_M_HSIOM_PTR    ( (reg32 *) LIN_1_SCB_ss0_m__0__HSIOM)
    
    #define LIN_1_SCB_SS0_M_HSIOM_MASK       (LIN_1_SCB_ss0_m__0__HSIOM_MASK)
    #define LIN_1_SCB_SS0_M_HSIOM_POS        (LIN_1_SCB_ss0_m__0__HSIOM_SHIFT)
    #define LIN_1_SCB_SS0_M_HSIOM_SEL_GPIO   (LIN_1_SCB_ss0_m__0__HSIOM_GPIO)
    #define LIN_1_SCB_SS0_M_HSIOM_SEL_SPI    (LIN_1_SCB_ss0_m__0__HSIOM_SPI)
#endif /* (LIN_1_SCB_SPI_MASTER_SS0_PIN) */

#if (LIN_1_SCB_SPI_MASTER_SS1_PIN)
    #define LIN_1_SCB_SS1_M_HSIOM_REG    (*(reg32 *) LIN_1_SCB_ss1_m__0__HSIOM)
    #define LIN_1_SCB_SS1_M_HSIOM_PTR    ( (reg32 *) LIN_1_SCB_ss1_m__0__HSIOM)
    
    #define LIN_1_SCB_SS1_M_HSIOM_MASK       (LIN_1_SCB_ss1_m__0__HSIOM_MASK)
    #define LIN_1_SCB_SS1_M_HSIOM_POS        (LIN_1_SCB_ss1_m__0__HSIOM_SHIFT)
    #define LIN_1_SCB_SS1_M_HSIOM_SEL_GPIO   (LIN_1_SCB_ss1_m__0__HSIOM_GPIO)
    #define LIN_1_SCB_SS1_M_HSIOM_SEL_SPI    (LIN_1_SCB_ss1_m__0__HSIOM_SPI)
#endif /* (LIN_1_SCB_SPI_MASTER_SS1_PIN) */

#if (LIN_1_SCB_SPI_MASTER_SS2_PIN)
    #define LIN_1_SCB_SS2_M_HSIOM_REG    (*(reg32 *) LIN_1_SCB_ss2_m__0__HSIOM)
    #define LIN_1_SCB_SS2_M_HSIOM_PTR    ( (reg32 *) LIN_1_SCB_ss2_m__0__HSIOM)
    
    #define LIN_1_SCB_SS2_M_HSIOM_MASK       (LIN_1_SCB_ss2_m__0__HSIOM_MASK)
    #define LIN_1_SCB_SS2_M_HSIOM_POS        (LIN_1_SCB_ss2_m__0__HSIOM_SHIFT)
    #define LIN_1_SCB_SS2_M_HSIOM_SEL_GPIO   (LIN_1_SCB_ss2_m__0__HSIOM_GPIO)
    #define LIN_1_SCB_SS2_M_HSIOM_SEL_SPI    (LIN_1_SCB_ss2_m__0__HSIOM_SPI)
#endif /* (LIN_1_SCB_SPI_MASTER_SS2_PIN) */

#if (LIN_1_SCB_SPI_MASTER_SS3_PIN)
    #define LIN_1_SCB_SS3_M_HSIOM_REG    (*(reg32 *) LIN_1_SCB_ss3_m__0__HSIOM)
    #define LIN_1_SCB_SS3_M_HSIOM_PTR    ( (reg32 *) LIN_1_SCB_ss3_m__0__HSIOM)
    
    #define LIN_1_SCB_SS3_M_HSIOM_MASK      (LIN_1_SCB_ss3_m__0__HSIOM_MASK)
    #define LIN_1_SCB_SS3_M_HSIOM_POS       (LIN_1_SCB_ss3_m__0__HSIOM_SHIFT)
    #define LIN_1_SCB_SS3_M_HSIOM_SEL_GPIO  (LIN_1_SCB_ss3_m__0__HSIOM_GPIO)
    #define LIN_1_SCB_SS3_M_HSIOM_SEL_SPI   (LIN_1_SCB_ss3_m__0__HSIOM_SPI)
#endif /* (LIN_1_SCB_SPI_MASTER_SS3_PIN) */

#if (LIN_1_SCB_UART_RX_PIN)
    #define LIN_1_SCB_RX_HSIOM_REG   (*(reg32 *) LIN_1_SCB_rx__0__HSIOM)
    #define LIN_1_SCB_RX_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_rx__0__HSIOM)
    
    #define LIN_1_SCB_RX_HSIOM_MASK      (LIN_1_SCB_rx__0__HSIOM_MASK)
    #define LIN_1_SCB_RX_HSIOM_POS       (LIN_1_SCB_rx__0__HSIOM_SHIFT)
    #define LIN_1_SCB_RX_HSIOM_SEL_GPIO  (LIN_1_SCB_rx__0__HSIOM_GPIO)
    #define LIN_1_SCB_RX_HSIOM_SEL_UART  (LIN_1_SCB_rx__0__HSIOM_UART)
#endif /* (LIN_1_SCB_UART_RX_PIN) */

#if (LIN_1_SCB_UART_RX_WAKE_PIN)
    #define LIN_1_SCB_RX_WAKE_HSIOM_REG   (*(reg32 *) LIN_1_SCB_rx_wake__0__HSIOM)
    #define LIN_1_SCB_RX_WAKE_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_rx_wake__0__HSIOM)
    
    #define LIN_1_SCB_RX_WAKE_HSIOM_MASK      (LIN_1_SCB_rx_wake__0__HSIOM_MASK)
    #define LIN_1_SCB_RX_WAKE_HSIOM_POS       (LIN_1_SCB_rx_wake__0__HSIOM_SHIFT)
    #define LIN_1_SCB_RX_WAKE_HSIOM_SEL_GPIO  (LIN_1_SCB_rx_wake__0__HSIOM_GPIO)
    #define LIN_1_SCB_RX_WAKE_HSIOM_SEL_UART  (LIN_1_SCB_rx_wake__0__HSIOM_UART)
#endif /* (LIN_1_SCB_UART_WAKE_RX_PIN) */

#if (LIN_1_SCB_UART_CTS_PIN)
    #define LIN_1_SCB_CTS_HSIOM_REG   (*(reg32 *) LIN_1_SCB_cts__0__HSIOM)
    #define LIN_1_SCB_CTS_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_cts__0__HSIOM)
    
    #define LIN_1_SCB_CTS_HSIOM_MASK      (LIN_1_SCB_cts__0__HSIOM_MASK)
    #define LIN_1_SCB_CTS_HSIOM_POS       (LIN_1_SCB_cts__0__HSIOM_SHIFT)
    #define LIN_1_SCB_CTS_HSIOM_SEL_GPIO  (LIN_1_SCB_cts__0__HSIOM_GPIO)
    #define LIN_1_SCB_CTS_HSIOM_SEL_UART  (LIN_1_SCB_cts__0__HSIOM_UART)
#endif /* (LIN_1_SCB_UART_CTS_PIN) */

#if (LIN_1_SCB_UART_TX_PIN)
    #define LIN_1_SCB_TX_HSIOM_REG   (*(reg32 *) LIN_1_SCB_tx__0__HSIOM)
    #define LIN_1_SCB_TX_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_tx__0__HSIOM)
    
    #define LIN_1_SCB_TX_HSIOM_MASK      (LIN_1_SCB_tx__0__HSIOM_MASK)
    #define LIN_1_SCB_TX_HSIOM_POS       (LIN_1_SCB_tx__0__HSIOM_SHIFT)
    #define LIN_1_SCB_TX_HSIOM_SEL_GPIO  (LIN_1_SCB_tx__0__HSIOM_GPIO)
    #define LIN_1_SCB_TX_HSIOM_SEL_UART  (LIN_1_SCB_tx__0__HSIOM_UART)
#endif /* (LIN_1_SCB_UART_TX_PIN) */

#if (LIN_1_SCB_UART_RX_TX_PIN)
    #define LIN_1_SCB_RX_TX_HSIOM_REG   (*(reg32 *) LIN_1_SCB_rx_tx__0__HSIOM)
    #define LIN_1_SCB_RX_TX_HSIOM_PTR   ( (reg32 *) LIN_1_SCB_rx_tx__0__HSIOM)
    
    #define LIN_1_SCB_RX_TX_HSIOM_MASK      (LIN_1_SCB_rx_tx__0__HSIOM_MASK)
    #define LIN_1_SCB_RX_TX_HSIOM_POS       (LIN_1_SCB_rx_tx__0__HSIOM_SHIFT)
    #define LIN_1_SCB_RX_TX_HSIOM_SEL_GPIO  (LIN_1_SCB_rx_tx__0__HSIOM_GPIO)
    #define LIN_1_SCB_RX_TX_HSIOM_SEL_UART  (LIN_1_SCB_rx_tx__0__HSIOM_UART)
#endif /* (LIN_1_SCB_UART_RX_TX_PIN) */

#if (LIN_1_SCB_UART_RTS_PIN)
    #define LIN_1_SCB_RTS_HSIOM_REG      (*(reg32 *) LIN_1_SCB_rts__0__HSIOM)
    #define LIN_1_SCB_RTS_HSIOM_PTR      ( (reg32 *) LIN_1_SCB_rts__0__HSIOM)
    
    #define LIN_1_SCB_RTS_HSIOM_MASK     (LIN_1_SCB_rts__0__HSIOM_MASK)
    #define LIN_1_SCB_RTS_HSIOM_POS      (LIN_1_SCB_rts__0__HSIOM_SHIFT)    
    #define LIN_1_SCB_RTS_HSIOM_SEL_GPIO (LIN_1_SCB_rts__0__HSIOM_GPIO)
    #define LIN_1_SCB_RTS_HSIOM_SEL_UART (LIN_1_SCB_rts__0__HSIOM_UART)    
#endif /* (LIN_1_SCB_UART_RTS_PIN) */


/***************************************
*        Registers Constants
***************************************/

/* HSIOM switch values. */ 
#define LIN_1_SCB_HSIOM_DEF_SEL      (0x00u)
#define LIN_1_SCB_HSIOM_GPIO_SEL     (0x00u)
/* The HSIOM values provided below are valid only for LIN_1_SCB_CY_SCBIP_V0 
* and LIN_1_SCB_CY_SCBIP_V1. It is not recommended to use them for 
* LIN_1_SCB_CY_SCBIP_V2. Use pin name specific HSIOM constants provided 
* above instead for any SCB IP block version.
*/
#define LIN_1_SCB_HSIOM_UART_SEL     (0x09u)
#define LIN_1_SCB_HSIOM_I2C_SEL      (0x0Eu)
#define LIN_1_SCB_HSIOM_SPI_SEL      (0x0Fu)

/* Pins settings index. */
#define LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX   (0u)
#define LIN_1_SCB_RX_SCL_MOSI_PIN_INDEX       (0u)
#define LIN_1_SCB_TX_SDA_MISO_PIN_INDEX       (1u)
#define LIN_1_SCB_CTS_SCLK_PIN_INDEX       (2u)
#define LIN_1_SCB_RTS_SS0_PIN_INDEX       (3u)
#define LIN_1_SCB_SS1_PIN_INDEX                  (4u)
#define LIN_1_SCB_SS2_PIN_INDEX                  (5u)
#define LIN_1_SCB_SS3_PIN_INDEX                  (6u)

/* Pins settings mask. */
#define LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN_MASK ((uint32) 0x01u << LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX)
#define LIN_1_SCB_RX_SCL_MOSI_PIN_MASK     ((uint32) 0x01u << LIN_1_SCB_RX_SCL_MOSI_PIN_INDEX)
#define LIN_1_SCB_TX_SDA_MISO_PIN_MASK     ((uint32) 0x01u << LIN_1_SCB_TX_SDA_MISO_PIN_INDEX)
#define LIN_1_SCB_CTS_SCLK_PIN_MASK     ((uint32) 0x01u << LIN_1_SCB_CTS_SCLK_PIN_INDEX)
#define LIN_1_SCB_RTS_SS0_PIN_MASK     ((uint32) 0x01u << LIN_1_SCB_RTS_SS0_PIN_INDEX)
#define LIN_1_SCB_SS1_PIN_MASK                ((uint32) 0x01u << LIN_1_SCB_SS1_PIN_INDEX)
#define LIN_1_SCB_SS2_PIN_MASK                ((uint32) 0x01u << LIN_1_SCB_SS2_PIN_INDEX)
#define LIN_1_SCB_SS3_PIN_MASK                ((uint32) 0x01u << LIN_1_SCB_SS3_PIN_INDEX)

/* Pin interrupt constants. */
#define LIN_1_SCB_INTCFG_TYPE_MASK           (0x03u)
#define LIN_1_SCB_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin Drive Mode constants. */
#define LIN_1_SCB_PIN_DM_ALG_HIZ  (0u)
#define LIN_1_SCB_PIN_DM_DIG_HIZ  (1u)
#define LIN_1_SCB_PIN_DM_OD_LO    (4u)
#define LIN_1_SCB_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

/* Return drive mode of the pin */
#define LIN_1_SCB_DM_MASK    (0x7u)
#define LIN_1_SCB_DM_SIZE    (3u)
#define LIN_1_SCB_GET_P4_PIN_DM(reg, pos) \
    ( ((reg) & (uint32) ((uint32) LIN_1_SCB_DM_MASK << (LIN_1_SCB_DM_SIZE * (pos)))) >> \
                                                              (LIN_1_SCB_DM_SIZE * (pos)) )

#if (LIN_1_SCB_TX_SDA_MISO_PIN)
    #define LIN_1_SCB_CHECK_TX_SDA_MISO_PIN_USED \
                (LIN_1_SCB_PIN_DM_ALG_HIZ != \
                    LIN_1_SCB_GET_P4_PIN_DM(LIN_1_SCB_uart_tx_i2c_sda_spi_miso_PC, \
                                                   LIN_1_SCB_uart_tx_i2c_sda_spi_miso_SHIFT))
#endif /* (LIN_1_SCB_TX_SDA_MISO_PIN) */

#if (LIN_1_SCB_RTS_SS0_PIN)
    #define LIN_1_SCB_CHECK_RTS_SS0_PIN_USED \
                (LIN_1_SCB_PIN_DM_ALG_HIZ != \
                    LIN_1_SCB_GET_P4_PIN_DM(LIN_1_SCB_uart_rts_spi_ss0_PC, \
                                                   LIN_1_SCB_uart_rts_spi_ss0_SHIFT))
#endif /* (LIN_1_SCB_RTS_SS0_PIN) */

/* Set bits-mask in register */
#define LIN_1_SCB_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

/* Set bit in the register */
#define LIN_1_SCB_SET_REGISTER_BIT(reg, mask, val) \
                    ((val) ? ((reg) |= (mask)) : ((reg) &= ((uint32) ~((uint32) (mask)))))

#define LIN_1_SCB_SET_HSIOM_SEL(reg, mask, pos, sel) LIN_1_SCB_SET_REGISTER_BITS(reg, mask, pos, sel)
#define LIN_1_SCB_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        LIN_1_SCB_SET_REGISTER_BITS(reg, mask, pos, intType)
#define LIN_1_SCB_SET_INP_DIS(reg, mask, val) LIN_1_SCB_SET_REGISTER_BIT(reg, mask, val)

/* LIN_1_SCB_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  LIN_1_SCB_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* SCB I2C: scl signal */
#if (LIN_1_SCB_CY_SCBIP_V0)
#if (LIN_1_SCB_I2C_PINS)
    #define LIN_1_SCB_SET_I2C_SCL_DR(val) LIN_1_SCB_scl_Write(val)

    #define LIN_1_SCB_SET_I2C_SCL_HSIOM_SEL(sel) \
                          LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_SCL_HSIOM_REG,  \
                                                         LIN_1_SCB_SCL_HSIOM_MASK, \
                                                         LIN_1_SCB_SCL_HSIOM_POS,  \
                                                         (sel))
    #define LIN_1_SCB_WAIT_SCL_SET_HIGH  (0u == LIN_1_SCB_scl_Read())

/* Unconfigured SCB: scl signal */
#elif (LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN)
    #define LIN_1_SCB_SET_I2C_SCL_DR(val) \
                            LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi_Write(val)

    #define LIN_1_SCB_SET_I2C_SCL_HSIOM_SEL(sel) \
                    LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG,  \
                                                   LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_MASK, \
                                                   LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_POS,  \
                                                   (sel))

    #define LIN_1_SCB_WAIT_SCL_SET_HIGH  (0u == LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi_Read())

#elif (LIN_1_SCB_RX_SCL_MOSI_PIN)
    #define LIN_1_SCB_SET_I2C_SCL_DR(val) \
                            LIN_1_SCB_uart_rx_i2c_scl_spi_mosi_Write(val)


    #define LIN_1_SCB_SET_I2C_SCL_HSIOM_SEL(sel) \
                            LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_RX_SCL_MOSI_HSIOM_REG,  \
                                                           LIN_1_SCB_RX_SCL_MOSI_HSIOM_MASK, \
                                                           LIN_1_SCB_RX_SCL_MOSI_HSIOM_POS,  \
                                                           (sel))

    #define LIN_1_SCB_WAIT_SCL_SET_HIGH  (0u == LIN_1_SCB_uart_rx_i2c_scl_spi_mosi_Read())

#else
    #define LIN_1_SCB_SET_I2C_SCL_DR(val)        do{ /* Does nothing */ }while(0)
    #define LIN_1_SCB_SET_I2C_SCL_HSIOM_SEL(sel) do{ /* Does nothing */ }while(0)

    #define LIN_1_SCB_WAIT_SCL_SET_HIGH  (0u)
#endif /* (LIN_1_SCB_I2C_PINS) */

/* SCB I2C: sda signal */
#if (LIN_1_SCB_I2C_PINS)
    #define LIN_1_SCB_WAIT_SDA_SET_HIGH  (0u == LIN_1_SCB_sda_Read())
/* Unconfigured SCB: sda signal */
#elif (LIN_1_SCB_TX_SDA_MISO_PIN)
    #define LIN_1_SCB_WAIT_SDA_SET_HIGH  (0u == LIN_1_SCB_uart_tx_i2c_sda_spi_miso_Read())
#else
    #define LIN_1_SCB_WAIT_SDA_SET_HIGH  (0u)
#endif /* (LIN_1_SCB_MOSI_SCL_RX_PIN) */
#endif /* (LIN_1_SCB_CY_SCBIP_V0) */

/* Clear UART wakeup source */
#if (LIN_1_SCB_RX_SCL_MOSI_PIN)
    #define LIN_1_SCB_CLEAR_UART_RX_WAKE_INTR        do{ /* Does nothing */ }while(0)
    
#elif (LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN)
    #define LIN_1_SCB_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi_ClearInterrupt(); \
            }while(0)

#elif(LIN_1_SCB_UART_RX_WAKE_PIN)
    #define LIN_1_SCB_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) LIN_1_SCB_rx_wake_ClearInterrupt(); \
            }while(0)
#else
#endif /* (LIN_1_SCB_RX_SCL_MOSI_PIN) */


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Unconfigured pins */
#define LIN_1_SCB_REMOVE_MOSI_SCL_RX_WAKE_PIN    LIN_1_SCB_REMOVE_RX_WAKE_SCL_MOSI_PIN
#define LIN_1_SCB_REMOVE_MOSI_SCL_RX_PIN         LIN_1_SCB_REMOVE_RX_SCL_MOSI_PIN
#define LIN_1_SCB_REMOVE_MISO_SDA_TX_PIN         LIN_1_SCB_REMOVE_TX_SDA_MISO_PIN
#ifndef LIN_1_SCB_REMOVE_SCLK_PIN
#define LIN_1_SCB_REMOVE_SCLK_PIN                LIN_1_SCB_REMOVE_CTS_SCLK_PIN
#endif /* LIN_1_SCB_REMOVE_SCLK_PIN */
#ifndef LIN_1_SCB_REMOVE_SS0_PIN
#define LIN_1_SCB_REMOVE_SS0_PIN                 LIN_1_SCB_REMOVE_RTS_SS0_PIN
#endif /* LIN_1_SCB_REMOVE_SS0_PIN */

/* Unconfigured pins */
#define LIN_1_SCB_MOSI_SCL_RX_WAKE_PIN   LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN
#define LIN_1_SCB_MOSI_SCL_RX_PIN        LIN_1_SCB_RX_SCL_MOSI_PIN
#define LIN_1_SCB_MISO_SDA_TX_PIN        LIN_1_SCB_TX_SDA_MISO_PIN
#ifndef LIN_1_SCB_SCLK_PIN
#define LIN_1_SCB_SCLK_PIN               LIN_1_SCB_CTS_SCLK_PIN
#endif /* LIN_1_SCB_SCLK_PIN */
#ifndef LIN_1_SCB_SS0_PIN
#define LIN_1_SCB_SS0_PIN                LIN_1_SCB_RTS_SS0_PIN
#endif /* LIN_1_SCB_SS0_PIN */

#if (LIN_1_SCB_MOSI_SCL_RX_WAKE_PIN)
    #define LIN_1_SCB_MOSI_SCL_RX_WAKE_HSIOM_REG     LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define LIN_1_SCB_MOSI_SCL_RX_WAKE_HSIOM_PTR     LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define LIN_1_SCB_MOSI_SCL_RX_WAKE_HSIOM_MASK    LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define LIN_1_SCB_MOSI_SCL_RX_WAKE_HSIOM_POS     LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG

    #define LIN_1_SCB_MOSI_SCL_RX_WAKE_INTCFG_REG    LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define LIN_1_SCB_MOSI_SCL_RX_WAKE_INTCFG_PTR    LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG

    #define LIN_1_SCB_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS   LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define LIN_1_SCB_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK  LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG
#endif /* (LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN) */

#if (LIN_1_SCB_MOSI_SCL_RX_PIN)
    #define LIN_1_SCB_MOSI_SCL_RX_HSIOM_REG      LIN_1_SCB_RX_SCL_MOSI_HSIOM_REG
    #define LIN_1_SCB_MOSI_SCL_RX_HSIOM_PTR      LIN_1_SCB_RX_SCL_MOSI_HSIOM_PTR
    #define LIN_1_SCB_MOSI_SCL_RX_HSIOM_MASK     LIN_1_SCB_RX_SCL_MOSI_HSIOM_MASK
    #define LIN_1_SCB_MOSI_SCL_RX_HSIOM_POS      LIN_1_SCB_RX_SCL_MOSI_HSIOM_POS
#endif /* (LIN_1_SCB_MOSI_SCL_RX_PIN) */

#if (LIN_1_SCB_MISO_SDA_TX_PIN)
    #define LIN_1_SCB_MISO_SDA_TX_HSIOM_REG      LIN_1_SCB_TX_SDA_MISO_HSIOM_REG
    #define LIN_1_SCB_MISO_SDA_TX_HSIOM_PTR      LIN_1_SCB_TX_SDA_MISO_HSIOM_REG
    #define LIN_1_SCB_MISO_SDA_TX_HSIOM_MASK     LIN_1_SCB_TX_SDA_MISO_HSIOM_REG
    #define LIN_1_SCB_MISO_SDA_TX_HSIOM_POS      LIN_1_SCB_TX_SDA_MISO_HSIOM_REG
#endif /* (LIN_1_SCB_MISO_SDA_TX_PIN_PIN) */

#if (LIN_1_SCB_SCLK_PIN)
    #ifndef LIN_1_SCB_SCLK_HSIOM_REG
    #define LIN_1_SCB_SCLK_HSIOM_REG     LIN_1_SCB_CTS_SCLK_HSIOM_REG
    #define LIN_1_SCB_SCLK_HSIOM_PTR     LIN_1_SCB_CTS_SCLK_HSIOM_PTR
    #define LIN_1_SCB_SCLK_HSIOM_MASK    LIN_1_SCB_CTS_SCLK_HSIOM_MASK
    #define LIN_1_SCB_SCLK_HSIOM_POS     LIN_1_SCB_CTS_SCLK_HSIOM_POS
    #endif /* LIN_1_SCB_SCLK_HSIOM_REG */
#endif /* (LIN_1_SCB_SCLK_PIN) */

#if (LIN_1_SCB_SS0_PIN)
    #ifndef LIN_1_SCB_SS0_HSIOM_REG
    #define LIN_1_SCB_SS0_HSIOM_REG      LIN_1_SCB_RTS_SS0_HSIOM_REG
    #define LIN_1_SCB_SS0_HSIOM_PTR      LIN_1_SCB_RTS_SS0_HSIOM_PTR
    #define LIN_1_SCB_SS0_HSIOM_MASK     LIN_1_SCB_RTS_SS0_HSIOM_MASK
    #define LIN_1_SCB_SS0_HSIOM_POS      LIN_1_SCB_RTS_SS0_HSIOM_POS
    #endif /* LIN_1_SCB_SS0_HSIOM_REG */
#endif /* (LIN_1_SCB_SS0_PIN) */

#define LIN_1_SCB_MOSI_SCL_RX_WAKE_PIN_INDEX LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX
#define LIN_1_SCB_MOSI_SCL_RX_PIN_INDEX      LIN_1_SCB_RX_SCL_MOSI_PIN_INDEX
#define LIN_1_SCB_MISO_SDA_TX_PIN_INDEX      LIN_1_SCB_TX_SDA_MISO_PIN_INDEX
#ifndef LIN_1_SCB_SCLK_PIN_INDEX
#define LIN_1_SCB_SCLK_PIN_INDEX             LIN_1_SCB_CTS_SCLK_PIN_INDEX
#endif /* LIN_1_SCB_SCLK_PIN_INDEX */
#ifndef LIN_1_SCB_SS0_PIN_INDEX
#define LIN_1_SCB_SS0_PIN_INDEX              LIN_1_SCB_RTS_SS0_PIN_INDEX
#endif /* LIN_1_SCB_SS0_PIN_INDEX */

#define LIN_1_SCB_MOSI_SCL_RX_WAKE_PIN_MASK LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN_MASK
#define LIN_1_SCB_MOSI_SCL_RX_PIN_MASK      LIN_1_SCB_RX_SCL_MOSI_PIN_MASK
#define LIN_1_SCB_MISO_SDA_TX_PIN_MASK      LIN_1_SCB_TX_SDA_MISO_PIN_MASK
#ifndef LIN_1_SCB_SCLK_PIN_MASK
#define LIN_1_SCB_SCLK_PIN_MASK             LIN_1_SCB_CTS_SCLK_PIN_MASK
#endif /* LIN_1_SCB_SCLK_PIN_MASK */
#ifndef LIN_1_SCB_SS0_PIN_MASK
#define LIN_1_SCB_SS0_PIN_MASK              LIN_1_SCB_RTS_SS0_PIN_MASK
#endif /* LIN_1_SCB_SS0_PIN_MASK */

#endif /* (CY_SCB_PINS_LIN_1_SCB_H) */


/* [] END OF FILE */
