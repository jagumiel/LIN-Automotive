/***************************************************************************//**
* \file LINS_SCB_PINS.h
* \version 4.0
*
* \brief
*  This file provides constants and parameter values for the pin components
*  buried into SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PINS_LINS_SCB_H)
#define CY_SCB_PINS_LINS_SCB_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Unconfigured pins */
#define LINS_SCB_REMOVE_RX_WAKE_SCL_MOSI_PIN  (1u)
#define LINS_SCB_REMOVE_RX_SCL_MOSI_PIN      (1u)
#define LINS_SCB_REMOVE_TX_SDA_MISO_PIN      (1u)
#define LINS_SCB_REMOVE_SCLK_PIN      (1u)
#define LINS_SCB_REMOVE_SS0_PIN      (1u)
#define LINS_SCB_REMOVE_SS1_PIN                 (1u)
#define LINS_SCB_REMOVE_SS2_PIN                 (1u)
#define LINS_SCB_REMOVE_SS3_PIN                 (1u)

/* Mode defined pins */
#define LINS_SCB_REMOVE_I2C_PINS                (1u)
#define LINS_SCB_REMOVE_SPI_MASTER_PINS         (1u)
#define LINS_SCB_REMOVE_SPI_MASTER_SCLK_PIN     (1u)
#define LINS_SCB_REMOVE_SPI_MASTER_MOSI_PIN     (1u)
#define LINS_SCB_REMOVE_SPI_MASTER_MISO_PIN     (1u)
#define LINS_SCB_REMOVE_SPI_MASTER_SS0_PIN      (1u)
#define LINS_SCB_REMOVE_SPI_MASTER_SS1_PIN      (1u)
#define LINS_SCB_REMOVE_SPI_MASTER_SS2_PIN      (1u)
#define LINS_SCB_REMOVE_SPI_MASTER_SS3_PIN      (1u)
#define LINS_SCB_REMOVE_SPI_SLAVE_PINS          (1u)
#define LINS_SCB_REMOVE_SPI_SLAVE_MOSI_PIN      (1u)
#define LINS_SCB_REMOVE_SPI_SLAVE_MISO_PIN      (1u)
#define LINS_SCB_REMOVE_UART_TX_PIN             (0u)
#define LINS_SCB_REMOVE_UART_RX_TX_PIN          (1u)
#define LINS_SCB_REMOVE_UART_RX_PIN             (0u)
#define LINS_SCB_REMOVE_UART_RX_WAKE_PIN        (1u)
#define LINS_SCB_REMOVE_UART_RTS_PIN            (1u)
#define LINS_SCB_REMOVE_UART_CTS_PIN            (1u)

/* Unconfigured pins */
#define LINS_SCB_RX_WAKE_SCL_MOSI_PIN (0u == LINS_SCB_REMOVE_RX_WAKE_SCL_MOSI_PIN)
#define LINS_SCB_RX_SCL_MOSI_PIN     (0u == LINS_SCB_REMOVE_RX_SCL_MOSI_PIN)
#define LINS_SCB_TX_SDA_MISO_PIN     (0u == LINS_SCB_REMOVE_TX_SDA_MISO_PIN)
#define LINS_SCB_SCLK_PIN     (0u == LINS_SCB_REMOVE_SCLK_PIN)
#define LINS_SCB_SS0_PIN     (0u == LINS_SCB_REMOVE_SS0_PIN)
#define LINS_SCB_SS1_PIN                (0u == LINS_SCB_REMOVE_SS1_PIN)
#define LINS_SCB_SS2_PIN                (0u == LINS_SCB_REMOVE_SS2_PIN)
#define LINS_SCB_SS3_PIN                (0u == LINS_SCB_REMOVE_SS3_PIN)

/* Mode defined pins */
#define LINS_SCB_I2C_PINS               (0u == LINS_SCB_REMOVE_I2C_PINS)
#define LINS_SCB_SPI_MASTER_PINS        (0u == LINS_SCB_REMOVE_SPI_MASTER_PINS)
#define LINS_SCB_SPI_MASTER_SCLK_PIN    (0u == LINS_SCB_REMOVE_SPI_MASTER_SCLK_PIN)
#define LINS_SCB_SPI_MASTER_MOSI_PIN    (0u == LINS_SCB_REMOVE_SPI_MASTER_MOSI_PIN)
#define LINS_SCB_SPI_MASTER_MISO_PIN    (0u == LINS_SCB_REMOVE_SPI_MASTER_MISO_PIN)
#define LINS_SCB_SPI_MASTER_SS0_PIN     (0u == LINS_SCB_REMOVE_SPI_MASTER_SS0_PIN)
#define LINS_SCB_SPI_MASTER_SS1_PIN     (0u == LINS_SCB_REMOVE_SPI_MASTER_SS1_PIN)
#define LINS_SCB_SPI_MASTER_SS2_PIN     (0u == LINS_SCB_REMOVE_SPI_MASTER_SS2_PIN)
#define LINS_SCB_SPI_MASTER_SS3_PIN     (0u == LINS_SCB_REMOVE_SPI_MASTER_SS3_PIN)
#define LINS_SCB_SPI_SLAVE_PINS         (0u == LINS_SCB_REMOVE_SPI_SLAVE_PINS)
#define LINS_SCB_SPI_SLAVE_MOSI_PIN     (0u == LINS_SCB_REMOVE_SPI_SLAVE_MOSI_PIN)
#define LINS_SCB_SPI_SLAVE_MISO_PIN     (0u == LINS_SCB_REMOVE_SPI_SLAVE_MISO_PIN)
#define LINS_SCB_UART_TX_PIN            (0u == LINS_SCB_REMOVE_UART_TX_PIN)
#define LINS_SCB_UART_RX_TX_PIN         (0u == LINS_SCB_REMOVE_UART_RX_TX_PIN)
#define LINS_SCB_UART_RX_PIN            (0u == LINS_SCB_REMOVE_UART_RX_PIN)
#define LINS_SCB_UART_RX_WAKE_PIN       (0u == LINS_SCB_REMOVE_UART_RX_WAKE_PIN)
#define LINS_SCB_UART_RTS_PIN           (0u == LINS_SCB_REMOVE_UART_RTS_PIN)
#define LINS_SCB_UART_CTS_PIN           (0u == LINS_SCB_REMOVE_UART_CTS_PIN)


/***************************************
*             Includes
****************************************/

#if (LINS_SCB_RX_WAKE_SCL_MOSI_PIN)
    #include "LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi.h"
#endif /* (LINS_SCB_RX_SCL_MOSI) */

#if (LINS_SCB_RX_SCL_MOSI_PIN)
    #include "LINS_SCB_uart_rx_i2c_scl_spi_mosi.h"
#endif /* (LINS_SCB_RX_SCL_MOSI) */

#if (LINS_SCB_TX_SDA_MISO_PIN)
    #include "LINS_SCB_uart_tx_i2c_sda_spi_miso.h"
#endif /* (LINS_SCB_TX_SDA_MISO) */

#if (LINS_SCB_SCLK_PIN)
    #include "LINS_SCB_spi_sclk.h"
#endif /* (LINS_SCB_SCLK) */

#if (LINS_SCB_SS0_PIN)
    #include "LINS_SCB_spi_ss0.h"
#endif /* (LINS_SCB_SS0_PIN) */

#if (LINS_SCB_SS1_PIN)
    #include "LINS_SCB_spi_ss1.h"
#endif /* (LINS_SCB_SS1_PIN) */

#if (LINS_SCB_SS2_PIN)
    #include "LINS_SCB_spi_ss2.h"
#endif /* (LINS_SCB_SS2_PIN) */

#if (LINS_SCB_SS3_PIN)
    #include "LINS_SCB_spi_ss3.h"
#endif /* (LINS_SCB_SS3_PIN) */

#if (LINS_SCB_I2C_PINS)
    #include "LINS_SCB_scl.h"
    #include "LINS_SCB_sda.h"
#endif /* (LINS_SCB_I2C_PINS) */

#if (LINS_SCB_SPI_MASTER_PINS)
#if (LINS_SCB_SPI_MASTER_SCLK_PIN)
    #include "LINS_SCB_sclk_m.h"
#endif /* (LINS_SCB_SPI_MASTER_SCLK_PIN) */

#if (LINS_SCB_SPI_MASTER_MOSI_PIN)
    #include "LINS_SCB_mosi_m.h"
#endif /* (LINS_SCB_SPI_MASTER_MOSI_PIN) */

#if (LINS_SCB_SPI_MASTER_MISO_PIN)
    #include "LINS_SCB_miso_m.h"
#endif /*(LINS_SCB_SPI_MASTER_MISO_PIN) */
#endif /* (LINS_SCB_SPI_MASTER_PINS) */

#if (LINS_SCB_SPI_SLAVE_PINS)
    #include "LINS_SCB_sclk_s.h"
    #include "LINS_SCB_ss_s.h"

#if (LINS_SCB_SPI_SLAVE_MOSI_PIN)
    #include "LINS_SCB_mosi_s.h"
#endif /* (LINS_SCB_SPI_SLAVE_MOSI_PIN) */

#if (LINS_SCB_SPI_SLAVE_MISO_PIN)
    #include "LINS_SCB_miso_s.h"
#endif /*(LINS_SCB_SPI_SLAVE_MISO_PIN) */
#endif /* (LINS_SCB_SPI_SLAVE_PINS) */

#if (LINS_SCB_SPI_MASTER_SS0_PIN)
    #include "LINS_SCB_ss0_m.h"
#endif /* (LINS_SCB_SPI_MASTER_SS0_PIN) */

#if (LINS_SCB_SPI_MASTER_SS1_PIN)
    #include "LINS_SCB_ss1_m.h"
#endif /* (LINS_SCB_SPI_MASTER_SS1_PIN) */

#if (LINS_SCB_SPI_MASTER_SS2_PIN)
    #include "LINS_SCB_ss2_m.h"
#endif /* (LINS_SCB_SPI_MASTER_SS2_PIN) */

#if (LINS_SCB_SPI_MASTER_SS3_PIN)
    #include "LINS_SCB_ss3_m.h"
#endif /* (LINS_SCB_SPI_MASTER_SS3_PIN) */

#if (LINS_SCB_UART_TX_PIN)
    #include "LINS_SCB_tx.h"
#endif /* (LINS_SCB_UART_TX_PIN) */

#if (LINS_SCB_UART_RX_TX_PIN)
    #include "LINS_SCB_rx_tx.h"
#endif /* (LINS_SCB_UART_RX_TX_PIN) */

#if (LINS_SCB_UART_RX_PIN)
    #include "LINS_SCB_rx.h"
#endif /* (LINS_SCB_UART_RX_PIN) */

#if (LINS_SCB_UART_RX_WAKE_PIN)
    #include "LINS_SCB_rx_wake.h"
#endif /* (LINS_SCB_UART_RX_WAKE_PIN) */

#if (LINS_SCB_UART_RTS_PIN)
    #include "LINS_SCB_rts.h"
#endif /* (LINS_SCB_UART_RTS_PIN) */

#if (LINS_SCB_UART_CTS_PIN)
    #include "LINS_SCB_cts.h"
#endif /* (LINS_SCB_UART_CTS_PIN) */


/***************************************
*              Registers
***************************************/

#if (LINS_SCB_RX_SCL_MOSI_PIN)
    #define LINS_SCB_RX_SCL_MOSI_HSIOM_REG   (*(reg32 *) LINS_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM)
    #define LINS_SCB_RX_SCL_MOSI_HSIOM_PTR   ( (reg32 *) LINS_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM)
    
    #define LINS_SCB_RX_SCL_MOSI_HSIOM_MASK      (LINS_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM_MASK)
    #define LINS_SCB_RX_SCL_MOSI_HSIOM_POS       (LINS_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM_SHIFT)
    #define LINS_SCB_RX_SCL_MOSI_HSIOM_SEL_GPIO  (LINS_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM_GPIO)
    #define LINS_SCB_RX_SCL_MOSI_HSIOM_SEL_I2C   (LINS_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM_I2C)
    #define LINS_SCB_RX_SCL_MOSI_HSIOM_SEL_SPI   (LINS_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM_SPI)
    #define LINS_SCB_RX_SCL_MOSI_HSIOM_SEL_UART  (LINS_SCB_uart_rx_i2c_scl_spi_mosi__0__HSIOM_UART)
    
#elif (LINS_SCB_RX_WAKE_SCL_MOSI_PIN)
    #define LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG   (*(reg32 *) LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM)
    #define LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_PTR   ( (reg32 *) LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM)
    
    #define LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_MASK      (LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_MASK)
    #define LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_POS       (LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_SHIFT)
    #define LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_SEL_GPIO  (LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_GPIO)
    #define LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C   (LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_I2C)
    #define LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI   (LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_SPI)
    #define LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART  (LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_UART)    
   
    #define LINS_SCB_RX_WAKE_SCL_MOSI_INTCFG_REG (*(reg32 *) LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__INTCFG)
    #define LINS_SCB_RX_WAKE_SCL_MOSI_INTCFG_PTR ( (reg32 *) LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi__0__INTCFG)
    #define LINS_SCB_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS  (LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi__SHIFT)
    #define LINS_SCB_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK ((uint32) LINS_SCB_INTCFG_TYPE_MASK << \
                                                                           LINS_SCB_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS)
#else
    /* None of pins LINS_SCB_RX_SCL_MOSI_PIN or LINS_SCB_RX_WAKE_SCL_MOSI_PIN present.*/
#endif /* (LINS_SCB_RX_SCL_MOSI_PIN) */

#if (LINS_SCB_TX_SDA_MISO_PIN)
    #define LINS_SCB_TX_SDA_MISO_HSIOM_REG   (*(reg32 *) LINS_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM)
    #define LINS_SCB_TX_SDA_MISO_HSIOM_PTR   ( (reg32 *) LINS_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM)
    
    #define LINS_SCB_TX_SDA_MISO_HSIOM_MASK      (LINS_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM_MASK)
    #define LINS_SCB_TX_SDA_MISO_HSIOM_POS       (LINS_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM_SHIFT)
    #define LINS_SCB_TX_SDA_MISO_HSIOM_SEL_GPIO  (LINS_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM_GPIO)
    #define LINS_SCB_TX_SDA_MISO_HSIOM_SEL_I2C   (LINS_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM_I2C)
    #define LINS_SCB_TX_SDA_MISO_HSIOM_SEL_SPI   (LINS_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM_SPI)
    #define LINS_SCB_TX_SDA_MISO_HSIOM_SEL_UART  (LINS_SCB_uart_tx_i2c_sda_spi_miso__0__HSIOM_UART)
#endif /* (LINS_SCB_TX_SDA_MISO_PIN) */

#if (LINS_SCB_SCLK_PIN)
    #define LINS_SCB_SCLK_HSIOM_REG   (*(reg32 *) LINS_SCB_spi_sclk__0__HSIOM)
    #define LINS_SCB_SCLK_HSIOM_PTR   ( (reg32 *) LINS_SCB_spi_sclk__0__HSIOM)
    
    #define LINS_SCB_SCLK_HSIOM_MASK      (LINS_SCB_spi_sclk__0__HSIOM_MASK)
    #define LINS_SCB_SCLK_HSIOM_POS       (LINS_SCB_spi_sclk__0__HSIOM_SHIFT)
    #define LINS_SCB_SCLK_HSIOM_SEL_GPIO  (LINS_SCB_spi_sclk__0__HSIOM_GPIO)
    #define LINS_SCB_SCLK_HSIOM_SEL_I2C   (LINS_SCB_spi_sclk__0__HSIOM_I2C)
    #define LINS_SCB_SCLK_HSIOM_SEL_SPI   (LINS_SCB_spi_sclk__0__HSIOM_SPI)
    #define LINS_SCB_SCLK_HSIOM_SEL_UART  (LINS_SCB_spi_sclk__0__HSIOM_UART)
#endif /* (LINS_SCB_SCLK_PIN) */

#if (LINS_SCB_SS0_PIN)
    #define LINS_SCB_SS0_HSIOM_REG   (*(reg32 *) LINS_SCB_spi_ss0__0__HSIOM)
    #define LINS_SCB_SS0_HSIOM_PTR   ( (reg32 *) LINS_SCB_spi_ss0__0__HSIOM)
    
    #define LINS_SCB_SS0_HSIOM_MASK      (LINS_SCB_spi_ss0__0__HSIOM_MASK)
    #define LINS_SCB_SS0_HSIOM_POS       (LINS_SCB_spi_ss0__0__HSIOM_SHIFT)
    #define LINS_SCB_SS0_HSIOM_SEL_GPIO  (LINS_SCB_spi_ss0__0__HSIOM_GPIO)
    #define LINS_SCB_SS0_HSIOM_SEL_I2C   (LINS_SCB_spi_ss0__0__HSIOM_I2C)
    #define LINS_SCB_SS0_HSIOM_SEL_SPI   (LINS_SCB_spi_ss0__0__HSIOM_SPI)
#if !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1)
    #define LINS_SCB_SS0_HSIOM_SEL_UART  (LINS_SCB_spi_ss0__0__HSIOM_UART)
#endif /* !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1) */
#endif /* (LINS_SCB_SS0_PIN) */

#if (LINS_SCB_SS1_PIN)
    #define LINS_SCB_SS1_HSIOM_REG  (*(reg32 *) LINS_SCB_spi_ss1__0__HSIOM)
    #define LINS_SCB_SS1_HSIOM_PTR  ( (reg32 *) LINS_SCB_spi_ss1__0__HSIOM)
    
    #define LINS_SCB_SS1_HSIOM_MASK     (LINS_SCB_spi_ss1__0__HSIOM_MASK)
    #define LINS_SCB_SS1_HSIOM_POS      (LINS_SCB_spi_ss1__0__HSIOM_SHIFT)
    #define LINS_SCB_SS1_HSIOM_SEL_GPIO (LINS_SCB_spi_ss1__0__HSIOM_GPIO)
    #define LINS_SCB_SS1_HSIOM_SEL_I2C  (LINS_SCB_spi_ss1__0__HSIOM_I2C)
    #define LINS_SCB_SS1_HSIOM_SEL_SPI  (LINS_SCB_spi_ss1__0__HSIOM_SPI)
#endif /* (LINS_SCB_SS1_PIN) */

#if (LINS_SCB_SS2_PIN)
    #define LINS_SCB_SS2_HSIOM_REG     (*(reg32 *) LINS_SCB_spi_ss2__0__HSIOM)
    #define LINS_SCB_SS2_HSIOM_PTR     ( (reg32 *) LINS_SCB_spi_ss2__0__HSIOM)
    
    #define LINS_SCB_SS2_HSIOM_MASK     (LINS_SCB_spi_ss2__0__HSIOM_MASK)
    #define LINS_SCB_SS2_HSIOM_POS      (LINS_SCB_spi_ss2__0__HSIOM_SHIFT)
    #define LINS_SCB_SS2_HSIOM_SEL_GPIO (LINS_SCB_spi_ss2__0__HSIOM_GPIO)
    #define LINS_SCB_SS2_HSIOM_SEL_I2C  (LINS_SCB_spi_ss2__0__HSIOM_I2C)
    #define LINS_SCB_SS2_HSIOM_SEL_SPI  (LINS_SCB_spi_ss2__0__HSIOM_SPI)
#endif /* (LINS_SCB_SS2_PIN) */

#if (LINS_SCB_SS3_PIN)
    #define LINS_SCB_SS3_HSIOM_REG     (*(reg32 *) LINS_SCB_spi_ss3__0__HSIOM)
    #define LINS_SCB_SS3_HSIOM_PTR     ( (reg32 *) LINS_SCB_spi_ss3__0__HSIOM)
    
    #define LINS_SCB_SS3_HSIOM_MASK     (LINS_SCB_spi_ss3__0__HSIOM_MASK)
    #define LINS_SCB_SS3_HSIOM_POS      (LINS_SCB_spi_ss3__0__HSIOM_SHIFT)
    #define LINS_SCB_SS3_HSIOM_SEL_GPIO (LINS_SCB_spi_ss3__0__HSIOM_GPIO)
    #define LINS_SCB_SS3_HSIOM_SEL_I2C  (LINS_SCB_spi_ss3__0__HSIOM_I2C)
    #define LINS_SCB_SS3_HSIOM_SEL_SPI  (LINS_SCB_spi_ss3__0__HSIOM_SPI)
#endif /* (LINS_SCB_SS3_PIN) */

#if (LINS_SCB_I2C_PINS)
    #define LINS_SCB_SCL_HSIOM_REG  (*(reg32 *) LINS_SCB_scl__0__HSIOM)
    #define LINS_SCB_SCL_HSIOM_PTR  ( (reg32 *) LINS_SCB_scl__0__HSIOM)
    
    #define LINS_SCB_SCL_HSIOM_MASK     (LINS_SCB_scl__0__HSIOM_MASK)
    #define LINS_SCB_SCL_HSIOM_POS      (LINS_SCB_scl__0__HSIOM_SHIFT)
    #define LINS_SCB_SCL_HSIOM_SEL_GPIO (LINS_SCB_sda__0__HSIOM_GPIO)
    #define LINS_SCB_SCL_HSIOM_SEL_I2C  (LINS_SCB_sda__0__HSIOM_I2C)
    
    #define LINS_SCB_SDA_HSIOM_REG  (*(reg32 *) LINS_SCB_sda__0__HSIOM)
    #define LINS_SCB_SDA_HSIOM_PTR  ( (reg32 *) LINS_SCB_sda__0__HSIOM)
    
    #define LINS_SCB_SDA_HSIOM_MASK     (LINS_SCB_sda__0__HSIOM_MASK)
    #define LINS_SCB_SDA_HSIOM_POS      (LINS_SCB_sda__0__HSIOM_SHIFT)
    #define LINS_SCB_SDA_HSIOM_SEL_GPIO (LINS_SCB_sda__0__HSIOM_GPIO)
    #define LINS_SCB_SDA_HSIOM_SEL_I2C  (LINS_SCB_sda__0__HSIOM_I2C)
#endif /* (LINS_SCB_I2C_PINS) */

#if (LINS_SCB_SPI_SLAVE_PINS)
    #define LINS_SCB_SCLK_S_HSIOM_REG   (*(reg32 *) LINS_SCB_sclk_s__0__HSIOM)
    #define LINS_SCB_SCLK_S_HSIOM_PTR   ( (reg32 *) LINS_SCB_sclk_s__0__HSIOM)
    
    #define LINS_SCB_SCLK_S_HSIOM_MASK      (LINS_SCB_sclk_s__0__HSIOM_MASK)
    #define LINS_SCB_SCLK_S_HSIOM_POS       (LINS_SCB_sclk_s__0__HSIOM_SHIFT)
    #define LINS_SCB_SCLK_S_HSIOM_SEL_GPIO  (LINS_SCB_sclk_s__0__HSIOM_GPIO)
    #define LINS_SCB_SCLK_S_HSIOM_SEL_SPI   (LINS_SCB_sclk_s__0__HSIOM_SPI)
    
    #define LINS_SCB_SS0_S_HSIOM_REG    (*(reg32 *) LINS_SCB_ss0_s__0__HSIOM)
    #define LINS_SCB_SS0_S_HSIOM_PTR    ( (reg32 *) LINS_SCB_ss0_s__0__HSIOM)
    
    #define LINS_SCB_SS0_S_HSIOM_MASK       (LINS_SCB_ss0_s__0__HSIOM_MASK)
    #define LINS_SCB_SS0_S_HSIOM_POS        (LINS_SCB_ss0_s__0__HSIOM_SHIFT)
    #define LINS_SCB_SS0_S_HSIOM_SEL_GPIO   (LINS_SCB_ss0_s__0__HSIOM_GPIO)  
    #define LINS_SCB_SS0_S_HSIOM_SEL_SPI    (LINS_SCB_ss0_s__0__HSIOM_SPI)
#endif /* (LINS_SCB_SPI_SLAVE_PINS) */

#if (LINS_SCB_SPI_SLAVE_MOSI_PIN)
    #define LINS_SCB_MOSI_S_HSIOM_REG   (*(reg32 *) LINS_SCB_mosi_s__0__HSIOM)
    #define LINS_SCB_MOSI_S_HSIOM_PTR   ( (reg32 *) LINS_SCB_mosi_s__0__HSIOM)
    
    #define LINS_SCB_MOSI_S_HSIOM_MASK      (LINS_SCB_mosi_s__0__HSIOM_MASK)
    #define LINS_SCB_MOSI_S_HSIOM_POS       (LINS_SCB_mosi_s__0__HSIOM_SHIFT)
    #define LINS_SCB_MOSI_S_HSIOM_SEL_GPIO  (LINS_SCB_mosi_s__0__HSIOM_GPIO)
    #define LINS_SCB_MOSI_S_HSIOM_SEL_SPI   (LINS_SCB_mosi_s__0__HSIOM_SPI)
#endif /* (LINS_SCB_SPI_SLAVE_MOSI_PIN) */

#if (LINS_SCB_SPI_SLAVE_MISO_PIN)
    #define LINS_SCB_MISO_S_HSIOM_REG   (*(reg32 *) LINS_SCB_miso_s__0__HSIOM)
    #define LINS_SCB_MISO_S_HSIOM_PTR   ( (reg32 *) LINS_SCB_miso_s__0__HSIOM)
    
    #define LINS_SCB_MISO_S_HSIOM_MASK      (LINS_SCB_miso_s__0__HSIOM_MASK)
    #define LINS_SCB_MISO_S_HSIOM_POS       (LINS_SCB_miso_s__0__HSIOM_SHIFT)
    #define LINS_SCB_MISO_S_HSIOM_SEL_GPIO  (LINS_SCB_miso_s__0__HSIOM_GPIO)
    #define LINS_SCB_MISO_S_HSIOM_SEL_SPI   (LINS_SCB_miso_s__0__HSIOM_SPI)
#endif /* (LINS_SCB_SPI_SLAVE_MISO_PIN) */

#if (LINS_SCB_SPI_MASTER_MISO_PIN)
    #define LINS_SCB_MISO_M_HSIOM_REG   (*(reg32 *) LINS_SCB_miso_m__0__HSIOM)
    #define LINS_SCB_MISO_M_HSIOM_PTR   ( (reg32 *) LINS_SCB_miso_m__0__HSIOM)
    
    #define LINS_SCB_MISO_M_HSIOM_MASK      (LINS_SCB_miso_m__0__HSIOM_MASK)
    #define LINS_SCB_MISO_M_HSIOM_POS       (LINS_SCB_miso_m__0__HSIOM_SHIFT)
    #define LINS_SCB_MISO_M_HSIOM_SEL_GPIO  (LINS_SCB_miso_m__0__HSIOM_GPIO)
    #define LINS_SCB_MISO_M_HSIOM_SEL_SPI   (LINS_SCB_miso_m__0__HSIOM_SPI)
#endif /* (LINS_SCB_SPI_MASTER_MISO_PIN) */

#if (LINS_SCB_SPI_MASTER_MOSI_PIN)
    #define LINS_SCB_MOSI_M_HSIOM_REG   (*(reg32 *) LINS_SCB_mosi_m__0__HSIOM)
    #define LINS_SCB_MOSI_M_HSIOM_PTR   ( (reg32 *) LINS_SCB_mosi_m__0__HSIOM)
    
    #define LINS_SCB_MOSI_M_HSIOM_MASK      (LINS_SCB_mosi_m__0__HSIOM_MASK)
    #define LINS_SCB_MOSI_M_HSIOM_POS       (LINS_SCB_mosi_m__0__HSIOM_SHIFT)
    #define LINS_SCB_MOSI_M_HSIOM_SEL_GPIO  (LINS_SCB_mosi_m__0__HSIOM_GPIO)
    #define LINS_SCB_MOSI_M_HSIOM_SEL_SPI   (LINS_SCB_mosi_m__0__HSIOM_SPI)
#endif /* (LINS_SCB_SPI_MASTER_MOSI_PIN) */

#if (LINS_SCB_SPI_MASTER_SCLK_PIN)
    #define LINS_SCB_SCLK_M_HSIOM_REG   (*(reg32 *) LINS_SCB_sclk_m__0__HSIOM)
    #define LINS_SCB_SCLK_M_HSIOM_PTR   ( (reg32 *) LINS_SCB_sclk_m__0__HSIOM)
    
    #define LINS_SCB_SCLK_M_HSIOM_MASK      (LINS_SCB_sclk_m__0__HSIOM_MASK)
    #define LINS_SCB_SCLK_M_HSIOM_POS       (LINS_SCB_sclk_m__0__HSIOM_SHIFT)
    #define LINS_SCB_SCLK_M_HSIOM_SEL_GPIO  (LINS_SCB_sclk_m__0__HSIOM_GPIO)
    #define LINS_SCB_SCLK_M_HSIOM_SEL_SPI   (LINS_SCB_sclk_m__0__HSIOM_SPI)
#endif /* (LINS_SCB_SPI_MASTER_SCLK_PIN) */

#if (LINS_SCB_SPI_MASTER_SS0_PIN)
    #define LINS_SCB_SS0_M_HSIOM_REG    (*(reg32 *) LINS_SCB_ss0_m__0__HSIOM)
    #define LINS_SCB_SS0_M_HSIOM_PTR    ( (reg32 *) LINS_SCB_ss0_m__0__HSIOM)
    
    #define LINS_SCB_SS0_M_HSIOM_MASK       (LINS_SCB_ss0_m__0__HSIOM_MASK)
    #define LINS_SCB_SS0_M_HSIOM_POS        (LINS_SCB_ss0_m__0__HSIOM_SHIFT)
    #define LINS_SCB_SS0_M_HSIOM_SEL_GPIO   (LINS_SCB_ss0_m__0__HSIOM_GPIO)
    #define LINS_SCB_SS0_M_HSIOM_SEL_SPI    (LINS_SCB_ss0_m__0__HSIOM_SPI)
#endif /* (LINS_SCB_SPI_MASTER_SS0_PIN) */

#if (LINS_SCB_SPI_MASTER_SS1_PIN)
    #define LINS_SCB_SS1_M_HSIOM_REG    (*(reg32 *) LINS_SCB_ss1_m__0__HSIOM)
    #define LINS_SCB_SS1_M_HSIOM_PTR    ( (reg32 *) LINS_SCB_ss1_m__0__HSIOM)
    
    #define LINS_SCB_SS1_M_HSIOM_MASK       (LINS_SCB_ss1_m__0__HSIOM_MASK)
    #define LINS_SCB_SS1_M_HSIOM_POS        (LINS_SCB_ss1_m__0__HSIOM_SHIFT)
    #define LINS_SCB_SS1_M_HSIOM_SEL_GPIO   (LINS_SCB_ss1_m__0__HSIOM_GPIO)
    #define LINS_SCB_SS1_M_HSIOM_SEL_SPI    (LINS_SCB_ss1_m__0__HSIOM_SPI)
#endif /* (LINS_SCB_SPI_MASTER_SS1_PIN) */

#if (LINS_SCB_SPI_MASTER_SS2_PIN)
    #define LINS_SCB_SS2_M_HSIOM_REG    (*(reg32 *) LINS_SCB_ss2_m__0__HSIOM)
    #define LINS_SCB_SS2_M_HSIOM_PTR    ( (reg32 *) LINS_SCB_ss2_m__0__HSIOM)
    
    #define LINS_SCB_SS2_M_HSIOM_MASK       (LINS_SCB_ss2_m__0__HSIOM_MASK)
    #define LINS_SCB_SS2_M_HSIOM_POS        (LINS_SCB_ss2_m__0__HSIOM_SHIFT)
    #define LINS_SCB_SS2_M_HSIOM_SEL_GPIO   (LINS_SCB_ss2_m__0__HSIOM_GPIO)
    #define LINS_SCB_SS2_M_HSIOM_SEL_SPI    (LINS_SCB_ss2_m__0__HSIOM_SPI)
#endif /* (LINS_SCB_SPI_MASTER_SS2_PIN) */

#if (LINS_SCB_SPI_MASTER_SS3_PIN)
    #define LINS_SCB_SS3_M_HSIOM_REG    (*(reg32 *) LINS_SCB_ss3_m__0__HSIOM)
    #define LINS_SCB_SS3_M_HSIOM_PTR    ( (reg32 *) LINS_SCB_ss3_m__0__HSIOM)
    
    #define LINS_SCB_SS3_M_HSIOM_MASK      (LINS_SCB_ss3_m__0__HSIOM_MASK)
    #define LINS_SCB_SS3_M_HSIOM_POS       (LINS_SCB_ss3_m__0__HSIOM_SHIFT)
    #define LINS_SCB_SS3_M_HSIOM_SEL_GPIO  (LINS_SCB_ss3_m__0__HSIOM_GPIO)
    #define LINS_SCB_SS3_M_HSIOM_SEL_SPI   (LINS_SCB_ss3_m__0__HSIOM_SPI)
#endif /* (LINS_SCB_SPI_MASTER_SS3_PIN) */

#if (LINS_SCB_UART_RX_PIN)
    #define LINS_SCB_RX_HSIOM_REG   (*(reg32 *) LINS_SCB_rx__0__HSIOM)
    #define LINS_SCB_RX_HSIOM_PTR   ( (reg32 *) LINS_SCB_rx__0__HSIOM)
    
    #define LINS_SCB_RX_HSIOM_MASK      (LINS_SCB_rx__0__HSIOM_MASK)
    #define LINS_SCB_RX_HSIOM_POS       (LINS_SCB_rx__0__HSIOM_SHIFT)
    #define LINS_SCB_RX_HSIOM_SEL_GPIO  (LINS_SCB_rx__0__HSIOM_GPIO)
    #define LINS_SCB_RX_HSIOM_SEL_UART  (LINS_SCB_rx__0__HSIOM_UART)
#endif /* (LINS_SCB_UART_RX_PIN) */

#if (LINS_SCB_UART_RX_WAKE_PIN)
    #define LINS_SCB_RX_WAKE_HSIOM_REG   (*(reg32 *) LINS_SCB_rx_wake__0__HSIOM)
    #define LINS_SCB_RX_WAKE_HSIOM_PTR   ( (reg32 *) LINS_SCB_rx_wake__0__HSIOM)
    
    #define LINS_SCB_RX_WAKE_HSIOM_MASK      (LINS_SCB_rx_wake__0__HSIOM_MASK)
    #define LINS_SCB_RX_WAKE_HSIOM_POS       (LINS_SCB_rx_wake__0__HSIOM_SHIFT)
    #define LINS_SCB_RX_WAKE_HSIOM_SEL_GPIO  (LINS_SCB_rx_wake__0__HSIOM_GPIO)
    #define LINS_SCB_RX_WAKE_HSIOM_SEL_UART  (LINS_SCB_rx_wake__0__HSIOM_UART)
#endif /* (LINS_SCB_UART_WAKE_RX_PIN) */

#if (LINS_SCB_UART_CTS_PIN)
    #define LINS_SCB_CTS_HSIOM_REG   (*(reg32 *) LINS_SCB_cts__0__HSIOM)
    #define LINS_SCB_CTS_HSIOM_PTR   ( (reg32 *) LINS_SCB_cts__0__HSIOM)
    
    #define LINS_SCB_CTS_HSIOM_MASK      (LINS_SCB_cts__0__HSIOM_MASK)
    #define LINS_SCB_CTS_HSIOM_POS       (LINS_SCB_cts__0__HSIOM_SHIFT)
    #define LINS_SCB_CTS_HSIOM_SEL_GPIO  (LINS_SCB_cts__0__HSIOM_GPIO)
    #define LINS_SCB_CTS_HSIOM_SEL_UART  (LINS_SCB_cts__0__HSIOM_UART)
#endif /* (LINS_SCB_UART_CTS_PIN) */

#if (LINS_SCB_UART_TX_PIN)
    #define LINS_SCB_TX_HSIOM_REG   (*(reg32 *) LINS_SCB_tx__0__HSIOM)
    #define LINS_SCB_TX_HSIOM_PTR   ( (reg32 *) LINS_SCB_tx__0__HSIOM)
    
    #define LINS_SCB_TX_HSIOM_MASK      (LINS_SCB_tx__0__HSIOM_MASK)
    #define LINS_SCB_TX_HSIOM_POS       (LINS_SCB_tx__0__HSIOM_SHIFT)
    #define LINS_SCB_TX_HSIOM_SEL_GPIO  (LINS_SCB_tx__0__HSIOM_GPIO)
    #define LINS_SCB_TX_HSIOM_SEL_UART  (LINS_SCB_tx__0__HSIOM_UART)
#endif /* (LINS_SCB_UART_TX_PIN) */

#if (LINS_SCB_UART_RX_TX_PIN)
    #define LINS_SCB_RX_TX_HSIOM_REG   (*(reg32 *) LINS_SCB_rx_tx__0__HSIOM)
    #define LINS_SCB_RX_TX_HSIOM_PTR   ( (reg32 *) LINS_SCB_rx_tx__0__HSIOM)
    
    #define LINS_SCB_RX_TX_HSIOM_MASK      (LINS_SCB_rx_tx__0__HSIOM_MASK)
    #define LINS_SCB_RX_TX_HSIOM_POS       (LINS_SCB_rx_tx__0__HSIOM_SHIFT)
    #define LINS_SCB_RX_TX_HSIOM_SEL_GPIO  (LINS_SCB_rx_tx__0__HSIOM_GPIO)
    #define LINS_SCB_RX_TX_HSIOM_SEL_UART  (LINS_SCB_rx_tx__0__HSIOM_UART)
#endif /* (LINS_SCB_UART_RX_TX_PIN) */

#if (LINS_SCB_UART_RTS_PIN)
    #define LINS_SCB_RTS_HSIOM_REG      (*(reg32 *) LINS_SCB_rts__0__HSIOM)
    #define LINS_SCB_RTS_HSIOM_PTR      ( (reg32 *) LINS_SCB_rts__0__HSIOM)
    
    #define LINS_SCB_RTS_HSIOM_MASK     (LINS_SCB_rts__0__HSIOM_MASK)
    #define LINS_SCB_RTS_HSIOM_POS      (LINS_SCB_rts__0__HSIOM_SHIFT)    
    #define LINS_SCB_RTS_HSIOM_SEL_GPIO (LINS_SCB_rts__0__HSIOM_GPIO)
    #define LINS_SCB_RTS_HSIOM_SEL_UART (LINS_SCB_rts__0__HSIOM_UART)    
#endif /* (LINS_SCB_UART_RTS_PIN) */


/***************************************
*        Registers Constants
***************************************/

/* HSIOM switch values. */ 
#define LINS_SCB_HSIOM_DEF_SEL      (0x00u)
#define LINS_SCB_HSIOM_GPIO_SEL     (0x00u)
/* The HSIOM values provided below are valid only for LINS_SCB_CY_SCBIP_V0 
* and LINS_SCB_CY_SCBIP_V1. It is not recommended to use them for 
* LINS_SCB_CY_SCBIP_V2. Use pin name specific HSIOM constants provided 
* above instead for any SCB IP block version.
*/
#define LINS_SCB_HSIOM_UART_SEL     (0x09u)
#define LINS_SCB_HSIOM_I2C_SEL      (0x0Eu)
#define LINS_SCB_HSIOM_SPI_SEL      (0x0Fu)

/* Pins settings index. */
#define LINS_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX   (0u)
#define LINS_SCB_RX_SCL_MOSI_PIN_INDEX       (0u)
#define LINS_SCB_TX_SDA_MISO_PIN_INDEX       (1u)
#define LINS_SCB_SCLK_PIN_INDEX       (2u)
#define LINS_SCB_SS0_PIN_INDEX       (3u)
#define LINS_SCB_SS1_PIN_INDEX                  (4u)
#define LINS_SCB_SS2_PIN_INDEX                  (5u)
#define LINS_SCB_SS3_PIN_INDEX                  (6u)

/* Pins settings mask. */
#define LINS_SCB_RX_WAKE_SCL_MOSI_PIN_MASK ((uint32) 0x01u << LINS_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX)
#define LINS_SCB_RX_SCL_MOSI_PIN_MASK     ((uint32) 0x01u << LINS_SCB_RX_SCL_MOSI_PIN_INDEX)
#define LINS_SCB_TX_SDA_MISO_PIN_MASK     ((uint32) 0x01u << LINS_SCB_TX_SDA_MISO_PIN_INDEX)
#define LINS_SCB_SCLK_PIN_MASK     ((uint32) 0x01u << LINS_SCB_SCLK_PIN_INDEX)
#define LINS_SCB_SS0_PIN_MASK     ((uint32) 0x01u << LINS_SCB_SS0_PIN_INDEX)
#define LINS_SCB_SS1_PIN_MASK                ((uint32) 0x01u << LINS_SCB_SS1_PIN_INDEX)
#define LINS_SCB_SS2_PIN_MASK                ((uint32) 0x01u << LINS_SCB_SS2_PIN_INDEX)
#define LINS_SCB_SS3_PIN_MASK                ((uint32) 0x01u << LINS_SCB_SS3_PIN_INDEX)

/* Pin interrupt constants. */
#define LINS_SCB_INTCFG_TYPE_MASK           (0x03u)
#define LINS_SCB_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin Drive Mode constants. */
#define LINS_SCB_PIN_DM_ALG_HIZ  (0u)
#define LINS_SCB_PIN_DM_DIG_HIZ  (1u)
#define LINS_SCB_PIN_DM_OD_LO    (4u)
#define LINS_SCB_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

/* Return drive mode of the pin */
#define LINS_SCB_DM_MASK    (0x7u)
#define LINS_SCB_DM_SIZE    (3u)
#define LINS_SCB_GET_P4_PIN_DM(reg, pos) \
    ( ((reg) & (uint32) ((uint32) LINS_SCB_DM_MASK << (LINS_SCB_DM_SIZE * (pos)))) >> \
                                                              (LINS_SCB_DM_SIZE * (pos)) )

#if (LINS_SCB_TX_SDA_MISO_PIN)
    #define LINS_SCB_CHECK_TX_SDA_MISO_PIN_USED \
                (LINS_SCB_PIN_DM_ALG_HIZ != \
                    LINS_SCB_GET_P4_PIN_DM(LINS_SCB_uart_tx_i2c_sda_spi_miso_PC, \
                                                   LINS_SCB_uart_tx_i2c_sda_spi_miso_SHIFT))
#endif /* (LINS_SCB_TX_SDA_MISO_PIN) */

#if (LINS_SCB_SS0_PIN)
    #define LINS_SCB_CHECK_SS0_PIN_USED \
                (LINS_SCB_PIN_DM_ALG_HIZ != \
                    LINS_SCB_GET_P4_PIN_DM(LINS_SCB_spi_ss0_PC, \
                                                   LINS_SCB_spi_ss0_SHIFT))
#endif /* (LINS_SCB_SS0_PIN) */

/* Set bits-mask in register */
#define LINS_SCB_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

/* Set bit in the register */
#define LINS_SCB_SET_REGISTER_BIT(reg, mask, val) \
                    ((val) ? ((reg) |= (mask)) : ((reg) &= ((uint32) ~((uint32) (mask)))))

#define LINS_SCB_SET_HSIOM_SEL(reg, mask, pos, sel) LINS_SCB_SET_REGISTER_BITS(reg, mask, pos, sel)
#define LINS_SCB_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        LINS_SCB_SET_REGISTER_BITS(reg, mask, pos, intType)
#define LINS_SCB_SET_INP_DIS(reg, mask, val) LINS_SCB_SET_REGISTER_BIT(reg, mask, val)

/* LINS_SCB_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  LINS_SCB_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* SCB I2C: scl signal */
#if (LINS_SCB_CY_SCBIP_V0)
#if (LINS_SCB_I2C_PINS)
    #define LINS_SCB_SET_I2C_SCL_DR(val) LINS_SCB_scl_Write(val)

    #define LINS_SCB_SET_I2C_SCL_HSIOM_SEL(sel) \
                          LINS_SCB_SET_HSIOM_SEL(LINS_SCB_SCL_HSIOM_REG,  \
                                                         LINS_SCB_SCL_HSIOM_MASK, \
                                                         LINS_SCB_SCL_HSIOM_POS,  \
                                                         (sel))
    #define LINS_SCB_WAIT_SCL_SET_HIGH  (0u == LINS_SCB_scl_Read())

/* Unconfigured SCB: scl signal */
#elif (LINS_SCB_RX_WAKE_SCL_MOSI_PIN)
    #define LINS_SCB_SET_I2C_SCL_DR(val) \
                            LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi_Write(val)

    #define LINS_SCB_SET_I2C_SCL_HSIOM_SEL(sel) \
                    LINS_SCB_SET_HSIOM_SEL(LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG,  \
                                                   LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_MASK, \
                                                   LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_POS,  \
                                                   (sel))

    #define LINS_SCB_WAIT_SCL_SET_HIGH  (0u == LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi_Read())

#elif (LINS_SCB_RX_SCL_MOSI_PIN)
    #define LINS_SCB_SET_I2C_SCL_DR(val) \
                            LINS_SCB_uart_rx_i2c_scl_spi_mosi_Write(val)


    #define LINS_SCB_SET_I2C_SCL_HSIOM_SEL(sel) \
                            LINS_SCB_SET_HSIOM_SEL(LINS_SCB_RX_SCL_MOSI_HSIOM_REG,  \
                                                           LINS_SCB_RX_SCL_MOSI_HSIOM_MASK, \
                                                           LINS_SCB_RX_SCL_MOSI_HSIOM_POS,  \
                                                           (sel))

    #define LINS_SCB_WAIT_SCL_SET_HIGH  (0u == LINS_SCB_uart_rx_i2c_scl_spi_mosi_Read())

#else
    #define LINS_SCB_SET_I2C_SCL_DR(val)        do{ /* Does nothing */ }while(0)
    #define LINS_SCB_SET_I2C_SCL_HSIOM_SEL(sel) do{ /* Does nothing */ }while(0)

    #define LINS_SCB_WAIT_SCL_SET_HIGH  (0u)
#endif /* (LINS_SCB_I2C_PINS) */

/* SCB I2C: sda signal */
#if (LINS_SCB_I2C_PINS)
    #define LINS_SCB_WAIT_SDA_SET_HIGH  (0u == LINS_SCB_sda_Read())
/* Unconfigured SCB: sda signal */
#elif (LINS_SCB_TX_SDA_MISO_PIN)
    #define LINS_SCB_WAIT_SDA_SET_HIGH  (0u == LINS_SCB_uart_tx_i2c_sda_spi_miso_Read())
#else
    #define LINS_SCB_WAIT_SDA_SET_HIGH  (0u)
#endif /* (LINS_SCB_MOSI_SCL_RX_PIN) */
#endif /* (LINS_SCB_CY_SCBIP_V0) */

/* Clear UART wakeup source */
#if (LINS_SCB_RX_SCL_MOSI_PIN)
    #define LINS_SCB_CLEAR_UART_RX_WAKE_INTR        do{ /* Does nothing */ }while(0)
    
#elif (LINS_SCB_RX_WAKE_SCL_MOSI_PIN)
    #define LINS_SCB_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi_ClearInterrupt(); \
            }while(0)

#elif(LINS_SCB_UART_RX_WAKE_PIN)
    #define LINS_SCB_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) LINS_SCB_rx_wake_ClearInterrupt(); \
            }while(0)
#else
#endif /* (LINS_SCB_RX_SCL_MOSI_PIN) */


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Unconfigured pins */
#define LINS_SCB_REMOVE_MOSI_SCL_RX_WAKE_PIN    LINS_SCB_REMOVE_RX_WAKE_SCL_MOSI_PIN
#define LINS_SCB_REMOVE_MOSI_SCL_RX_PIN         LINS_SCB_REMOVE_RX_SCL_MOSI_PIN
#define LINS_SCB_REMOVE_MISO_SDA_TX_PIN         LINS_SCB_REMOVE_TX_SDA_MISO_PIN
#ifndef LINS_SCB_REMOVE_SCLK_PIN
#define LINS_SCB_REMOVE_SCLK_PIN                LINS_SCB_REMOVE_SCLK_PIN
#endif /* LINS_SCB_REMOVE_SCLK_PIN */
#ifndef LINS_SCB_REMOVE_SS0_PIN
#define LINS_SCB_REMOVE_SS0_PIN                 LINS_SCB_REMOVE_SS0_PIN
#endif /* LINS_SCB_REMOVE_SS0_PIN */

/* Unconfigured pins */
#define LINS_SCB_MOSI_SCL_RX_WAKE_PIN   LINS_SCB_RX_WAKE_SCL_MOSI_PIN
#define LINS_SCB_MOSI_SCL_RX_PIN        LINS_SCB_RX_SCL_MOSI_PIN
#define LINS_SCB_MISO_SDA_TX_PIN        LINS_SCB_TX_SDA_MISO_PIN
#ifndef LINS_SCB_SCLK_PIN
#define LINS_SCB_SCLK_PIN               LINS_SCB_SCLK_PIN
#endif /* LINS_SCB_SCLK_PIN */
#ifndef LINS_SCB_SS0_PIN
#define LINS_SCB_SS0_PIN                LINS_SCB_SS0_PIN
#endif /* LINS_SCB_SS0_PIN */

#if (LINS_SCB_MOSI_SCL_RX_WAKE_PIN)
    #define LINS_SCB_MOSI_SCL_RX_WAKE_HSIOM_REG     LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define LINS_SCB_MOSI_SCL_RX_WAKE_HSIOM_PTR     LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define LINS_SCB_MOSI_SCL_RX_WAKE_HSIOM_MASK    LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define LINS_SCB_MOSI_SCL_RX_WAKE_HSIOM_POS     LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG

    #define LINS_SCB_MOSI_SCL_RX_WAKE_INTCFG_REG    LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define LINS_SCB_MOSI_SCL_RX_WAKE_INTCFG_PTR    LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG

    #define LINS_SCB_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS   LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define LINS_SCB_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK  LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG
#endif /* (LINS_SCB_RX_WAKE_SCL_MOSI_PIN) */

#if (LINS_SCB_MOSI_SCL_RX_PIN)
    #define LINS_SCB_MOSI_SCL_RX_HSIOM_REG      LINS_SCB_RX_SCL_MOSI_HSIOM_REG
    #define LINS_SCB_MOSI_SCL_RX_HSIOM_PTR      LINS_SCB_RX_SCL_MOSI_HSIOM_PTR
    #define LINS_SCB_MOSI_SCL_RX_HSIOM_MASK     LINS_SCB_RX_SCL_MOSI_HSIOM_MASK
    #define LINS_SCB_MOSI_SCL_RX_HSIOM_POS      LINS_SCB_RX_SCL_MOSI_HSIOM_POS
#endif /* (LINS_SCB_MOSI_SCL_RX_PIN) */

#if (LINS_SCB_MISO_SDA_TX_PIN)
    #define LINS_SCB_MISO_SDA_TX_HSIOM_REG      LINS_SCB_TX_SDA_MISO_HSIOM_REG
    #define LINS_SCB_MISO_SDA_TX_HSIOM_PTR      LINS_SCB_TX_SDA_MISO_HSIOM_REG
    #define LINS_SCB_MISO_SDA_TX_HSIOM_MASK     LINS_SCB_TX_SDA_MISO_HSIOM_REG
    #define LINS_SCB_MISO_SDA_TX_HSIOM_POS      LINS_SCB_TX_SDA_MISO_HSIOM_REG
#endif /* (LINS_SCB_MISO_SDA_TX_PIN_PIN) */

#if (LINS_SCB_SCLK_PIN)
    #ifndef LINS_SCB_SCLK_HSIOM_REG
    #define LINS_SCB_SCLK_HSIOM_REG     LINS_SCB_SCLK_HSIOM_REG
    #define LINS_SCB_SCLK_HSIOM_PTR     LINS_SCB_SCLK_HSIOM_PTR
    #define LINS_SCB_SCLK_HSIOM_MASK    LINS_SCB_SCLK_HSIOM_MASK
    #define LINS_SCB_SCLK_HSIOM_POS     LINS_SCB_SCLK_HSIOM_POS
    #endif /* LINS_SCB_SCLK_HSIOM_REG */
#endif /* (LINS_SCB_SCLK_PIN) */

#if (LINS_SCB_SS0_PIN)
    #ifndef LINS_SCB_SS0_HSIOM_REG
    #define LINS_SCB_SS0_HSIOM_REG      LINS_SCB_SS0_HSIOM_REG
    #define LINS_SCB_SS0_HSIOM_PTR      LINS_SCB_SS0_HSIOM_PTR
    #define LINS_SCB_SS0_HSIOM_MASK     LINS_SCB_SS0_HSIOM_MASK
    #define LINS_SCB_SS0_HSIOM_POS      LINS_SCB_SS0_HSIOM_POS
    #endif /* LINS_SCB_SS0_HSIOM_REG */
#endif /* (LINS_SCB_SS0_PIN) */

#define LINS_SCB_MOSI_SCL_RX_WAKE_PIN_INDEX LINS_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX
#define LINS_SCB_MOSI_SCL_RX_PIN_INDEX      LINS_SCB_RX_SCL_MOSI_PIN_INDEX
#define LINS_SCB_MISO_SDA_TX_PIN_INDEX      LINS_SCB_TX_SDA_MISO_PIN_INDEX
#ifndef LINS_SCB_SCLK_PIN_INDEX
#define LINS_SCB_SCLK_PIN_INDEX             LINS_SCB_SCLK_PIN_INDEX
#endif /* LINS_SCB_SCLK_PIN_INDEX */
#ifndef LINS_SCB_SS0_PIN_INDEX
#define LINS_SCB_SS0_PIN_INDEX              LINS_SCB_SS0_PIN_INDEX
#endif /* LINS_SCB_SS0_PIN_INDEX */

#define LINS_SCB_MOSI_SCL_RX_WAKE_PIN_MASK LINS_SCB_RX_WAKE_SCL_MOSI_PIN_MASK
#define LINS_SCB_MOSI_SCL_RX_PIN_MASK      LINS_SCB_RX_SCL_MOSI_PIN_MASK
#define LINS_SCB_MISO_SDA_TX_PIN_MASK      LINS_SCB_TX_SDA_MISO_PIN_MASK
#ifndef LINS_SCB_SCLK_PIN_MASK
#define LINS_SCB_SCLK_PIN_MASK             LINS_SCB_SCLK_PIN_MASK
#endif /* LINS_SCB_SCLK_PIN_MASK */
#ifndef LINS_SCB_SS0_PIN_MASK
#define LINS_SCB_SS0_PIN_MASK              LINS_SCB_SS0_PIN_MASK
#endif /* LINS_SCB_SS0_PIN_MASK */

#endif /* (CY_SCB_PINS_LINS_SCB_H) */


/* [] END OF FILE */
