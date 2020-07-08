/***************************************************************************//**
* \file LIN_1_SCB_BOOT.h
* \version 3.20
*
* \brief
*  This file provides constants and parameter values of the bootloader
*  communication APIs for the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2014-2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_BOOT_LIN_1_SCB_H)
#define CY_SCB_BOOT_LIN_1_SCB_H

#include "LIN_1_SCB_PVT.h"

#if (LIN_1_SCB_SCB_MODE_I2C_INC)
    #include "LIN_1_SCB_I2C.h"
#endif /* (LIN_1_SCB_SCB_MODE_I2C_INC) */

#if (LIN_1_SCB_SCB_MODE_EZI2C_INC)
    #include "LIN_1_SCB_EZI2C.h"
#endif /* (LIN_1_SCB_SCB_MODE_EZI2C_INC) */

#if (LIN_1_SCB_SCB_MODE_SPI_INC || LIN_1_SCB_SCB_MODE_UART_INC)
    #include "LIN_1_SCB_SPI_UART.h"
#endif /* (LIN_1_SCB_SCB_MODE_SPI_INC || LIN_1_SCB_SCB_MODE_UART_INC) */


/***************************************
*  Conditional Compilation Parameters
****************************************/

/* Bootloader communication interface enable */
#define LIN_1_SCB_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_LIN_1_SCB) || \
                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

/* Enable I2C bootloader communication */
#if (LIN_1_SCB_SCB_MODE_I2C_INC)
    #define LIN_1_SCB_I2C_BTLDR_COMM_ENABLED     (LIN_1_SCB_BTLDR_COMM_ENABLED && \
                                                            (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             LIN_1_SCB_I2C_SLAVE_CONST))
#else
     #define LIN_1_SCB_I2C_BTLDR_COMM_ENABLED    (0u)
#endif /* (LIN_1_SCB_SCB_MODE_I2C_INC) */

/* EZI2C does not support bootloader communication. Provide empty APIs */
#if (LIN_1_SCB_SCB_MODE_EZI2C_INC)
    #define LIN_1_SCB_EZI2C_BTLDR_COMM_ENABLED   (LIN_1_SCB_BTLDR_COMM_ENABLED && \
                                                         LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
#else
    #define LIN_1_SCB_EZI2C_BTLDR_COMM_ENABLED   (0u)
#endif /* (LIN_1_SCB_EZI2C_BTLDR_COMM_ENABLED) */

/* Enable SPI bootloader communication */
#if (LIN_1_SCB_SCB_MODE_SPI_INC)
    #define LIN_1_SCB_SPI_BTLDR_COMM_ENABLED     (LIN_1_SCB_BTLDR_COMM_ENABLED && \
                                                            (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             LIN_1_SCB_SPI_SLAVE_CONST))
#else
        #define LIN_1_SCB_SPI_BTLDR_COMM_ENABLED (0u)
#endif /* (LIN_1_SCB_SPI_BTLDR_COMM_ENABLED) */

/* Enable UART bootloader communication */
#if (LIN_1_SCB_SCB_MODE_UART_INC)
       #define LIN_1_SCB_UART_BTLDR_COMM_ENABLED    (LIN_1_SCB_BTLDR_COMM_ENABLED && \
                                                            (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             (LIN_1_SCB_UART_RX_DIRECTION && \
                                                              LIN_1_SCB_UART_TX_DIRECTION)))
#else
     #define LIN_1_SCB_UART_BTLDR_COMM_ENABLED   (0u)
#endif /* (LIN_1_SCB_UART_BTLDR_COMM_ENABLED) */

/* Enable bootloader communication */
#define LIN_1_SCB_BTLDR_COMM_MODE_ENABLED    (LIN_1_SCB_I2C_BTLDR_COMM_ENABLED   || \
                                                     LIN_1_SCB_SPI_BTLDR_COMM_ENABLED   || \
                                                     LIN_1_SCB_EZI2C_BTLDR_COMM_ENABLED || \
                                                     LIN_1_SCB_UART_BTLDR_COMM_ENABLED)


/***************************************
*        Function Prototypes
***************************************/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (LIN_1_SCB_I2C_BTLDR_COMM_ENABLED)
    /* I2C Bootloader physical layer functions */
    void LIN_1_SCB_I2CCyBtldrCommStart(void);
    void LIN_1_SCB_I2CCyBtldrCommStop (void);
    void LIN_1_SCB_I2CCyBtldrCommReset(void);
    cystatus LIN_1_SCB_I2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus LIN_1_SCB_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map I2C specific bootloader communication APIs to SCB specific APIs */
    #if (LIN_1_SCB_SCB_MODE_I2C_CONST_CFG)
        #define LIN_1_SCB_CyBtldrCommStart   LIN_1_SCB_I2CCyBtldrCommStart
        #define LIN_1_SCB_CyBtldrCommStop    LIN_1_SCB_I2CCyBtldrCommStop
        #define LIN_1_SCB_CyBtldrCommReset   LIN_1_SCB_I2CCyBtldrCommReset
        #define LIN_1_SCB_CyBtldrCommRead    LIN_1_SCB_I2CCyBtldrCommRead
        #define LIN_1_SCB_CyBtldrCommWrite   LIN_1_SCB_I2CCyBtldrCommWrite
    #endif /* (LIN_1_SCB_SCB_MODE_I2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (LIN_1_SCB_I2C_BTLDR_COMM_ENABLED) */


#if defined(CYDEV_BOOTLOADER_IO_COMP) && (LIN_1_SCB_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void LIN_1_SCB_EzI2CCyBtldrCommStart(void);
    void LIN_1_SCB_EzI2CCyBtldrCommStop (void);
    void LIN_1_SCB_EzI2CCyBtldrCommReset(void);
    cystatus LIN_1_SCB_EzI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus LIN_1_SCB_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map EZI2C specific bootloader communication APIs to SCB specific APIs */
    #if (LIN_1_SCB_SCB_MODE_EZI2C_CONST_CFG)
        #define LIN_1_SCB_CyBtldrCommStart   LIN_1_SCB_EzI2CCyBtldrCommStart
        #define LIN_1_SCB_CyBtldrCommStop    LIN_1_SCB_EzI2CCyBtldrCommStop
        #define LIN_1_SCB_CyBtldrCommReset   LIN_1_SCB_EzI2CCyBtldrCommReset
        #define LIN_1_SCB_CyBtldrCommRead    LIN_1_SCB_EzI2CCyBtldrCommRead
        #define LIN_1_SCB_CyBtldrCommWrite   LIN_1_SCB_EzI2CCyBtldrCommWrite
    #endif /* (LIN_1_SCB_SCB_MODE_EZI2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (LIN_1_SCB_EZI2C_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (LIN_1_SCB_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void LIN_1_SCB_SpiCyBtldrCommStart(void);
    void LIN_1_SCB_SpiCyBtldrCommStop (void);
    void LIN_1_SCB_SpiCyBtldrCommReset(void);
    cystatus LIN_1_SCB_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus LIN_1_SCB_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map SPI specific bootloader communication APIs to SCB specific APIs */
    #if (LIN_1_SCB_SCB_MODE_SPI_CONST_CFG)
        #define LIN_1_SCB_CyBtldrCommStart   LIN_1_SCB_SpiCyBtldrCommStart
        #define LIN_1_SCB_CyBtldrCommStop    LIN_1_SCB_SpiCyBtldrCommStop
        #define LIN_1_SCB_CyBtldrCommReset   LIN_1_SCB_SpiCyBtldrCommReset
        #define LIN_1_SCB_CyBtldrCommRead    LIN_1_SCB_SpiCyBtldrCommRead
        #define LIN_1_SCB_CyBtldrCommWrite   LIN_1_SCB_SpiCyBtldrCommWrite
    #endif /* (LIN_1_SCB_SCB_MODE_SPI_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (LIN_1_SCB_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (LIN_1_SCB_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void LIN_1_SCB_UartCyBtldrCommStart(void);
    void LIN_1_SCB_UartCyBtldrCommStop (void);
    void LIN_1_SCB_UartCyBtldrCommReset(void);
    cystatus LIN_1_SCB_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus LIN_1_SCB_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map UART specific bootloader communication APIs to SCB specific APIs */
    #if (LIN_1_SCB_SCB_MODE_UART_CONST_CFG)
        #define LIN_1_SCB_CyBtldrCommStart   LIN_1_SCB_UartCyBtldrCommStart
        #define LIN_1_SCB_CyBtldrCommStop    LIN_1_SCB_UartCyBtldrCommStop
        #define LIN_1_SCB_CyBtldrCommReset   LIN_1_SCB_UartCyBtldrCommReset
        #define LIN_1_SCB_CyBtldrCommRead    LIN_1_SCB_UartCyBtldrCommRead
        #define LIN_1_SCB_CyBtldrCommWrite   LIN_1_SCB_UartCyBtldrCommWrite
    #endif /* (LIN_1_SCB_SCB_MODE_UART_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (LIN_1_SCB_UART_BTLDR_COMM_ENABLED) */

/**
* \addtogroup group_bootloader
* @{
*/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (LIN_1_SCB_BTLDR_COMM_ENABLED)
    #if (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Bootloader physical layer functions */
        void LIN_1_SCB_CyBtldrCommStart(void);
        void LIN_1_SCB_CyBtldrCommStop (void);
        void LIN_1_SCB_CyBtldrCommReset(void);
        cystatus LIN_1_SCB_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
        cystatus LIN_1_SCB_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    #endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Map SCB specific bootloader communication APIs to common APIs */
    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_LIN_1_SCB)
        #define CyBtldrCommStart    LIN_1_SCB_CyBtldrCommStart
        #define CyBtldrCommStop     LIN_1_SCB_CyBtldrCommStop
        #define CyBtldrCommReset    LIN_1_SCB_CyBtldrCommReset
        #define CyBtldrCommWrite    LIN_1_SCB_CyBtldrCommWrite
        #define CyBtldrCommRead     LIN_1_SCB_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_LIN_1_SCB) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (LIN_1_SCB_BTLDR_COMM_ENABLED) */

/** @} group_bootloader */

/***************************************
*           API Constants
***************************************/

/* Timeout unit in milliseconds */
#define LIN_1_SCB_WAIT_1_MS  (1u)

/* Return number of bytes to copy into bootloader buffer */
#define LIN_1_SCB_BYTES_TO_COPY(actBufSize, bufSize) \
                            ( ((uint32)(actBufSize) < (uint32)(bufSize)) ? \
                                ((uint32) (actBufSize)) : ((uint32) (bufSize)) )

/* Size of Read/Write buffers for I2C bootloader  */
#define LIN_1_SCB_I2C_BTLDR_SIZEOF_READ_BUFFER   (64u)
#define LIN_1_SCB_I2C_BTLDR_SIZEOF_WRITE_BUFFER  (64u)

/* Byte to byte time interval: calculated basing on current component
* data rate configuration, can be defined in project if required.
*/
#ifndef LIN_1_SCB_SPI_BYTE_TO_BYTE
    #define LIN_1_SCB_SPI_BYTE_TO_BYTE   (160u)
#endif

/* Byte to byte time interval: calculated basing on current component
* baud rate configuration, can be defined in the project if required.
*/
#ifndef LIN_1_SCB_UART_BYTE_TO_BYTE
    #define LIN_1_SCB_UART_BYTE_TO_BYTE  (1040u)
#endif /* LIN_1_SCB_UART_BYTE_TO_BYTE */

#endif /* (CY_SCB_BOOT_LIN_1_SCB_H) */


/* [] END OF FILE */
