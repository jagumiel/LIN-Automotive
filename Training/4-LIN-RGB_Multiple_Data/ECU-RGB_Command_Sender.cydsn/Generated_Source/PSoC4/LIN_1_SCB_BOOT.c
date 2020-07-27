/***************************************************************************//**
* \file LIN_1_SCB_BOOT.c
* \version 3.20
*
* \brief
*  This file provides the source code of the bootloader communication APIs
*  for the SCB Component Unconfigured mode.
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

#include "LIN_1_SCB_BOOT.h"

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (LIN_1_SCB_BTLDR_COMM_ENABLED) && \
                                (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

/*******************************************************************************
* Function Name: LIN_1_SCB_CyBtldrCommStart
****************************************************************************//**
*
*  Starts LIN_1_SCB component. After this function call the component is 
*  ready for communication.
*
*******************************************************************************/
void LIN_1_SCB_CyBtldrCommStart(void)
{
    if (LIN_1_SCB_SCB_MODE_I2C_RUNTM_CFG)
    {
        LIN_1_SCB_I2CCyBtldrCommStart();
    }
    else if (LIN_1_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        LIN_1_SCB_EzI2CCyBtldrCommStart();
    }
#if (!LIN_1_SCB_CY_SCBIP_V1)
    else if (LIN_1_SCB_SCB_MODE_SPI_RUNTM_CFG)
    {
        LIN_1_SCB_SpiCyBtldrCommStart();
    }
    else if (LIN_1_SCB_SCB_MODE_UART_RUNTM_CFG)
    {
        LIN_1_SCB_UartCyBtldrCommStart();
    }
#endif /* (!LIN_1_SCB_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: LIN_1_SCB_CyBtldrCommStop
****************************************************************************//**
*
*  Stops LIN_1_SCB component.
*
*******************************************************************************/
void LIN_1_SCB_CyBtldrCommStop(void)
{
    if (LIN_1_SCB_SCB_MODE_I2C_RUNTM_CFG)
    {
        LIN_1_SCB_I2CCyBtldrCommStop();
    }
    else if (LIN_1_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        LIN_1_SCB_EzI2CCyBtldrCommStop();
    }
#if (!LIN_1_SCB_CY_SCBIP_V1)
    else if (LIN_1_SCB_SCB_MODE_SPI_RUNTM_CFG)
    {
        LIN_1_SCB_SpiCyBtldrCommStop();
    }
    else if (LIN_1_SCB_SCB_MODE_UART_RUNTM_CFG)
    {
        LIN_1_SCB_UartCyBtldrCommStop();
    }
#endif /* (!LIN_1_SCB_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: LIN_1_SCB_CyBtldrCommReset
****************************************************************************//**
*
*  Clears LIN_1_SCB component buffers.
*
*******************************************************************************/
void LIN_1_SCB_CyBtldrCommReset(void)
{
    if(LIN_1_SCB_SCB_MODE_I2C_RUNTM_CFG)
    {
        LIN_1_SCB_I2CCyBtldrCommReset();
    }
    else if(LIN_1_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        LIN_1_SCB_EzI2CCyBtldrCommReset();
    }
#if (!LIN_1_SCB_CY_SCBIP_V1)
    else if(LIN_1_SCB_SCB_MODE_SPI_RUNTM_CFG)
    {
        LIN_1_SCB_SpiCyBtldrCommReset();
    }
    else if(LIN_1_SCB_SCB_MODE_UART_RUNTM_CFG)
    {
        LIN_1_SCB_UartCyBtldrCommReset();
    }
#endif /* (!LIN_1_SCB_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: LIN_1_SCB_CyBtldrCommRead
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
cystatus LIN_1_SCB_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    if(LIN_1_SCB_SCB_MODE_I2C_RUNTM_CFG)
    {
        status = LIN_1_SCB_I2CCyBtldrCommRead(pData, size, count, timeOut);
    }
    else if(LIN_1_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        status = LIN_1_SCB_EzI2CCyBtldrCommRead(pData, size, count, timeOut);
    }
#if (!LIN_1_SCB_CY_SCBIP_V1)
    else if(LIN_1_SCB_SCB_MODE_SPI_RUNTM_CFG)
    {
        status = LIN_1_SCB_SpiCyBtldrCommRead(pData, size, count, timeOut);
    }
    else if(LIN_1_SCB_SCB_MODE_UART_RUNTM_CFG)
    {
        status = LIN_1_SCB_UartCyBtldrCommRead(pData, size, count, timeOut);
    }
#endif /* (!LIN_1_SCB_CY_SCBIP_V1) */
    else
    {
        status = CYRET_INVALID_STATE; /* Unknown mode: return invalid status */
    }

    return(status);
}


/*******************************************************************************
* Function Name: LIN_1_SCB_CyBtldrCommWrite
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
cystatus LIN_1_SCB_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    if(LIN_1_SCB_SCB_MODE_I2C_RUNTM_CFG)
    {
        status = LIN_1_SCB_I2CCyBtldrCommWrite(pData, size, count, timeOut);
    }
    else if(LIN_1_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        status = LIN_1_SCB_EzI2CCyBtldrCommWrite(pData, size, count, timeOut);
    }
#if (!LIN_1_SCB_CY_SCBIP_V1)
    else if(LIN_1_SCB_SCB_MODE_SPI_RUNTM_CFG)
    {
        status = LIN_1_SCB_SpiCyBtldrCommWrite(pData, size, count, timeOut);
    }
    else if(LIN_1_SCB_SCB_MODE_UART_RUNTM_CFG)
    {
        status = LIN_1_SCB_UartCyBtldrCommWrite(pData, size, count, timeOut);
    }
#endif /* (!LIN_1_SCB_CY_SCBIP_V1) */
    else
    {
        status = CYRET_INVALID_STATE; /* Unknown mode: return invalid status */
    }

    return(status);
}

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (LIN_1_SCB_BTLDR_COMM_MODE_ENABLED) */


/* [] END OF FILE */
