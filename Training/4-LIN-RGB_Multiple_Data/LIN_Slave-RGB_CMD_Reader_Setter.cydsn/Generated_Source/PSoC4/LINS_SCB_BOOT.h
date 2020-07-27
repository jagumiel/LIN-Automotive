/***************************************************************************//**
* \file LINS_SCB_BOOT.h
* \version 4.0
*
* \brief
*  This file provides constants and parameter values of the bootloader
*  communication APIs for the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2014-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_BOOT_LINS_SCB_H)
#define CY_SCB_BOOT_LINS_SCB_H

#include "LINS_SCB_PVT.h"

#if (LINS_SCB_SCB_MODE_I2C_INC)
    #include "LINS_SCB_I2C.h"
#endif /* (LINS_SCB_SCB_MODE_I2C_INC) */

#if (LINS_SCB_SCB_MODE_EZI2C_INC)
    #include "LINS_SCB_EZI2C.h"
#endif /* (LINS_SCB_SCB_MODE_EZI2C_INC) */

#if (LINS_SCB_SCB_MODE_SPI_INC || LINS_SCB_SCB_MODE_UART_INC)
    #include "LINS_SCB_SPI_UART.h"
#endif /* (LINS_SCB_SCB_MODE_SPI_INC || LINS_SCB_SCB_MODE_UART_INC) */


/***************************************
*  Conditional Compilation Parameters
****************************************/

/* Bootloader communication interface enable */
#define LINS_SCB_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_LINS_SCB) || \
                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

/* Enable I2C bootloader communication */
#if (LINS_SCB_SCB_MODE_I2C_INC)
    #define LINS_SCB_I2C_BTLDR_COMM_ENABLED     (LINS_SCB_BTLDR_COMM_ENABLED && \
                                                            (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             LINS_SCB_I2C_SLAVE_CONST))
#else
     #define LINS_SCB_I2C_BTLDR_COMM_ENABLED    (0u)
#endif /* (LINS_SCB_SCB_MODE_I2C_INC) */

/* EZI2C does not support bootloader communication. Provide empty APIs */
#if (LINS_SCB_SCB_MODE_EZI2C_INC)
    #define LINS_SCB_EZI2C_BTLDR_COMM_ENABLED   (LINS_SCB_BTLDR_COMM_ENABLED && \
                                                         LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
#else
    #define LINS_SCB_EZI2C_BTLDR_COMM_ENABLED   (0u)
#endif /* (LINS_SCB_EZI2C_BTLDR_COMM_ENABLED) */

/* Enable SPI bootloader communication */
#if (LINS_SCB_SCB_MODE_SPI_INC)
    #define LINS_SCB_SPI_BTLDR_COMM_ENABLED     (LINS_SCB_BTLDR_COMM_ENABLED && \
                                                            (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             LINS_SCB_SPI_SLAVE_CONST))
#else
        #define LINS_SCB_SPI_BTLDR_COMM_ENABLED (0u)
#endif /* (LINS_SCB_SPI_BTLDR_COMM_ENABLED) */

/* Enable UART bootloader communication */
#if (LINS_SCB_SCB_MODE_UART_INC)
       #define LINS_SCB_UART_BTLDR_COMM_ENABLED    (LINS_SCB_BTLDR_COMM_ENABLED && \
                                                            (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             (LINS_SCB_UART_RX_DIRECTION && \
                                                              LINS_SCB_UART_TX_DIRECTION)))
#else
     #define LINS_SCB_UART_BTLDR_COMM_ENABLED   (0u)
#endif /* (LINS_SCB_UART_BTLDR_COMM_ENABLED) */

/* Enable bootloader communication */
#define LINS_SCB_BTLDR_COMM_MODE_ENABLED    (LINS_SCB_I2C_BTLDR_COMM_ENABLED   || \
                                                     LINS_SCB_SPI_BTLDR_COMM_ENABLED   || \
                                                     LINS_SCB_EZI2C_BTLDR_COMM_ENABLED || \
                                                     LINS_SCB_UART_BTLDR_COMM_ENABLED)


/***************************************
*        Function Prototypes
***************************************/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (LINS_SCB_I2C_BTLDR_COMM_ENABLED)
    /* I2C Bootloader physical layer functions */
    void LINS_SCB_I2CCyBtldrCommStart(void);
    void LINS_SCB_I2CCyBtldrCommStop (void);
    void LINS_SCB_I2CCyBtldrCommReset(void);
    cystatus LINS_SCB_I2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus LINS_SCB_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map I2C specific bootloader communication APIs to SCB specific APIs */
    #if (LINS_SCB_SCB_MODE_I2C_CONST_CFG)
        #define LINS_SCB_CyBtldrCommStart   LINS_SCB_I2CCyBtldrCommStart
        #define LINS_SCB_CyBtldrCommStop    LINS_SCB_I2CCyBtldrCommStop
        #define LINS_SCB_CyBtldrCommReset   LINS_SCB_I2CCyBtldrCommReset
        #define LINS_SCB_CyBtldrCommRead    LINS_SCB_I2CCyBtldrCommRead
        #define LINS_SCB_CyBtldrCommWrite   LINS_SCB_I2CCyBtldrCommWrite
    #endif /* (LINS_SCB_SCB_MODE_I2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (LINS_SCB_I2C_BTLDR_COMM_ENABLED) */


#if defined(CYDEV_BOOTLOADER_IO_COMP) && (LINS_SCB_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void LINS_SCB_EzI2CCyBtldrCommStart(void);
    void LINS_SCB_EzI2CCyBtldrCommStop (void);
    void LINS_SCB_EzI2CCyBtldrCommReset(void);
    cystatus LINS_SCB_EzI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus LINS_SCB_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map EZI2C specific bootloader communication APIs to SCB specific APIs */
    #if (LINS_SCB_SCB_MODE_EZI2C_CONST_CFG)
        #define LINS_SCB_CyBtldrCommStart   LINS_SCB_EzI2CCyBtldrCommStart
        #define LINS_SCB_CyBtldrCommStop    LINS_SCB_EzI2CCyBtldrCommStop
        #define LINS_SCB_CyBtldrCommReset   LINS_SCB_EzI2CCyBtldrCommReset
        #define LINS_SCB_CyBtldrCommRead    LINS_SCB_EzI2CCyBtldrCommRead
        #define LINS_SCB_CyBtldrCommWrite   LINS_SCB_EzI2CCyBtldrCommWrite
    #endif /* (LINS_SCB_SCB_MODE_EZI2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (LINS_SCB_EZI2C_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (LINS_SCB_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void LINS_SCB_SpiCyBtldrCommStart(void);
    void LINS_SCB_SpiCyBtldrCommStop (void);
    void LINS_SCB_SpiCyBtldrCommReset(void);
    cystatus LINS_SCB_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus LINS_SCB_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map SPI specific bootloader communication APIs to SCB specific APIs */
    #if (LINS_SCB_SCB_MODE_SPI_CONST_CFG)
        #define LINS_SCB_CyBtldrCommStart   LINS_SCB_SpiCyBtldrCommStart
        #define LINS_SCB_CyBtldrCommStop    LINS_SCB_SpiCyBtldrCommStop
        #define LINS_SCB_CyBtldrCommReset   LINS_SCB_SpiCyBtldrCommReset
        #define LINS_SCB_CyBtldrCommRead    LINS_SCB_SpiCyBtldrCommRead
        #define LINS_SCB_CyBtldrCommWrite   LINS_SCB_SpiCyBtldrCommWrite
    #endif /* (LINS_SCB_SCB_MODE_SPI_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (LINS_SCB_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (LINS_SCB_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void LINS_SCB_UartCyBtldrCommStart(void);
    void LINS_SCB_UartCyBtldrCommStop (void);
    void LINS_SCB_UartCyBtldrCommReset(void);
    cystatus LINS_SCB_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus LINS_SCB_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map UART specific bootloader communication APIs to SCB specific APIs */
    #if (LINS_SCB_SCB_MODE_UART_CONST_CFG)
        #define LINS_SCB_CyBtldrCommStart   LINS_SCB_UartCyBtldrCommStart
        #define LINS_SCB_CyBtldrCommStop    LINS_SCB_UartCyBtldrCommStop
        #define LINS_SCB_CyBtldrCommReset   LINS_SCB_UartCyBtldrCommReset
        #define LINS_SCB_CyBtldrCommRead    LINS_SCB_UartCyBtldrCommRead
        #define LINS_SCB_CyBtldrCommWrite   LINS_SCB_UartCyBtldrCommWrite
    #endif /* (LINS_SCB_SCB_MODE_UART_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (LINS_SCB_UART_BTLDR_COMM_ENABLED) */

/**
* \addtogroup group_bootloader
* @{
*/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (LINS_SCB_BTLDR_COMM_ENABLED)
    #if (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Bootloader physical layer functions */
        void LINS_SCB_CyBtldrCommStart(void);
        void LINS_SCB_CyBtldrCommStop (void);
        void LINS_SCB_CyBtldrCommReset(void);
        cystatus LINS_SCB_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
        cystatus LINS_SCB_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    #endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Map SCB specific bootloader communication APIs to common APIs */
    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_LINS_SCB)
        #define CyBtldrCommStart    LINS_SCB_CyBtldrCommStart
        #define CyBtldrCommStop     LINS_SCB_CyBtldrCommStop
        #define CyBtldrCommReset    LINS_SCB_CyBtldrCommReset
        #define CyBtldrCommWrite    LINS_SCB_CyBtldrCommWrite
        #define CyBtldrCommRead     LINS_SCB_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_LINS_SCB) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (LINS_SCB_BTLDR_COMM_ENABLED) */

/** @} group_bootloader */

/***************************************
*           API Constants
***************************************/

/* Timeout unit in milliseconds */
#define LINS_SCB_WAIT_1_MS  (1u)

/* Return number of bytes to copy into bootloader buffer */
#define LINS_SCB_BYTES_TO_COPY(actBufSize, bufSize) \
                            ( ((uint32)(actBufSize) < (uint32)(bufSize)) ? \
                                ((uint32) (actBufSize)) : ((uint32) (bufSize)) )

/* Size of Read/Write buffers for I2C bootloader  */
#define LINS_SCB_I2C_BTLDR_SIZEOF_READ_BUFFER   (64u)
#define LINS_SCB_I2C_BTLDR_SIZEOF_WRITE_BUFFER  (64u)

/* Byte to byte time interval: calculated basing on current component
* data rate configuration, can be defined in project if required.
*/
#ifndef LINS_SCB_SPI_BYTE_TO_BYTE
    #define LINS_SCB_SPI_BYTE_TO_BYTE   (160u)
#endif

/* Byte to byte time interval: calculated basing on current component
* baud rate configuration, can be defined in the project if required.
*/
#ifndef LINS_SCB_UART_BYTE_TO_BYTE
    #define LINS_SCB_UART_BYTE_TO_BYTE  (1040u)
#endif /* LINS_SCB_UART_BYTE_TO_BYTE */

#endif /* (CY_SCB_BOOT_LINS_SCB_H) */


/* [] END OF FILE */
