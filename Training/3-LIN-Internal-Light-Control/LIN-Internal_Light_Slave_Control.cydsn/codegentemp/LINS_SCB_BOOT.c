/***************************************************************************//**
* \file LINS_SCB_BOOT.c
* \version 4.0
*
* \brief
*  This file provides the source code of the bootloader communication APIs
*  for the SCB Component Unconfigured mode.
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

#include "LINS_SCB_BOOT.h"

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (LINS_SCB_BTLDR_COMM_ENABLED) && \
                                (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

/*******************************************************************************
* Function Name: LINS_SCB_CyBtldrCommStart
****************************************************************************//**
*
*  Starts LINS_SCB component. After this function call the component is 
*  ready for communication.
*
*******************************************************************************/
void LINS_SCB_CyBtldrCommStart(void)
{
    if (LINS_SCB_SCB_MODE_I2C_RUNTM_CFG)
    {
        LINS_SCB_I2CCyBtldrCommStart();
    }
    else if (LINS_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        LINS_SCB_EzI2CCyBtldrCommStart();
    }
#if (!LINS_SCB_CY_SCBIP_V1)
    else if (LINS_SCB_SCB_MODE_SPI_RUNTM_CFG)
    {
        LINS_SCB_SpiCyBtldrCommStart();
    }
    else if (LINS_SCB_SCB_MODE_UART_RUNTM_CFG)
    {
        LINS_SCB_UartCyBtldrCommStart();
    }
#endif /* (!LINS_SCB_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: LINS_SCB_CyBtldrCommStop
****************************************************************************//**
*
*  Stops LINS_SCB component.
*
*******************************************************************************/
void LINS_SCB_CyBtldrCommStop(void)
{
    if (LINS_SCB_SCB_MODE_I2C_RUNTM_CFG)
    {
        LINS_SCB_I2CCyBtldrCommStop();
    }
    else if (LINS_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        LINS_SCB_EzI2CCyBtldrCommStop();
    }
#if (!LINS_SCB_CY_SCBIP_V1)
    else if (LINS_SCB_SCB_MODE_SPI_RUNTM_CFG)
    {
        LINS_SCB_SpiCyBtldrCommStop();
    }
    else if (LINS_SCB_SCB_MODE_UART_RUNTM_CFG)
    {
        LINS_SCB_UartCyBtldrCommStop();
    }
#endif /* (!LINS_SCB_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: LINS_SCB_CyBtldrCommReset
****************************************************************************//**
*
*  Clears LINS_SCB component buffers.
*
*******************************************************************************/
void LINS_SCB_CyBtldrCommReset(void)
{
    if(LINS_SCB_SCB_MODE_I2C_RUNTM_CFG)
    {
        LINS_SCB_I2CCyBtldrCommReset();
    }
    else if(LINS_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        LINS_SCB_EzI2CCyBtldrCommReset();
    }
#if (!LINS_SCB_CY_SCBIP_V1)
    else if(LINS_SCB_SCB_MODE_SPI_RUNTM_CFG)
    {
        LINS_SCB_SpiCyBtldrCommReset();
    }
    else if(LINS_SCB_SCB_MODE_UART_RUNTM_CFG)
    {
        LINS_SCB_UartCyBtldrCommReset();
    }
#endif /* (!LINS_SCB_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: LINS_SCB_CyBtldrCommRead
****************************************************************************//**
*
*  Allows the caller to read data from the bootloader host (the host writes the 
*  data). The function handles polling to allow a block of data to be completely
*  received from the host device.
*
*  \param pData: Pointer to storage for the block of data to be read from the
*   bootloader host.
*  \param size: Number of bytes to be read.
*  \param count: Pointer to the variable to write the number of bytes actually
*   read.
*  \param timeOut: Number of units in 10 ms to wait before returning because of a
*   timeout.
*
* \return
*  \return
*  cystatus: Returns CYRET_SUCCESS if no problem was encountered or returns the
*  value that best describes the problem. For more information refer to 
*  the “Return Codes” section of the System Reference Guide.
*
*******************************************************************************/
cystatus LINS_SCB_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    if(LINS_SCB_SCB_MODE_I2C_RUNTM_CFG)
    {
        status = LINS_SCB_I2CCyBtldrCommRead(pData, size, count, timeOut);
    }
    else if(LINS_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        status = LINS_SCB_EzI2CCyBtldrCommRead(pData, size, count, timeOut);
    }
#if (!LINS_SCB_CY_SCBIP_V1)
    else if(LINS_SCB_SCB_MODE_SPI_RUNTM_CFG)
    {
        status = LINS_SCB_SpiCyBtldrCommRead(pData, size, count, timeOut);
    }
    else if(LINS_SCB_SCB_MODE_UART_RUNTM_CFG)
    {
        status = LINS_SCB_UartCyBtldrCommRead(pData, size, count, timeOut);
    }
#endif /* (!LINS_SCB_CY_SCBIP_V1) */
    else
    {
        status = CYRET_INVALID_STATE; /* Unknown mode: return invalid status */
    }

    return(status);
}


/*******************************************************************************
* Function Name: LINS_SCB_CyBtldrCommWrite
****************************************************************************//**
*
*  Allows the caller to write data to the bootloader host (the host reads the 
*  data). The function does not use timeout and returns after data has been copied
*  into the slave read buffer. This data available to be read by the bootloader
*  host until following host data write.
*
*  \param pData: Pointer to the block of data to be written to the bootloader host.
*  \param size: Number of bytes to be written.
*  \param count: Pointer to the variable to write the number of bytes actually
*   written.
*  \param timeOut: Number of units in 10 ms to wait before returning because of a
*   timeout.
*
*  \return
*  cystatus: Returns CYRET_SUCCESS if no problem was encountered or returns the
*  value that best describes the problem. For more information refer to 
*  the “Return Codes” section of the System Reference Guide.
*
*******************************************************************************/
cystatus LINS_SCB_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    if(LINS_SCB_SCB_MODE_I2C_RUNTM_CFG)
    {
        status = LINS_SCB_I2CCyBtldrCommWrite(pData, size, count, timeOut);
    }
    else if(LINS_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        status = LINS_SCB_EzI2CCyBtldrCommWrite(pData, size, count, timeOut);
    }
#if (!LINS_SCB_CY_SCBIP_V1)
    else if(LINS_SCB_SCB_MODE_SPI_RUNTM_CFG)
    {
        status = LINS_SCB_SpiCyBtldrCommWrite(pData, size, count, timeOut);
    }
    else if(LINS_SCB_SCB_MODE_UART_RUNTM_CFG)
    {
        status = LINS_SCB_UartCyBtldrCommWrite(pData, size, count, timeOut);
    }
#endif /* (!LINS_SCB_CY_SCBIP_V1) */
    else
    {
        status = CYRET_INVALID_STATE; /* Unknown mode: return invalid status */
    }

    return(status);
}

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (LINS_SCB_BTLDR_COMM_MODE_ENABLED) */


/* [] END OF FILE */
