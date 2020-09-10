/***************************************************************************//**
* \file LIN_1_SCB_UART.c
* \version 3.20
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  UART mode.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LIN_1_SCB_PVT.h"
#include "LIN_1_SCB_SPI_UART_PVT.h"
#include "cyapicallbacks.h"

#if (LIN_1_SCB_UART_WAKE_ENABLE_CONST && LIN_1_SCB_UART_RX_WAKEUP_IRQ)
    /**
    * \addtogroup group_globals
    * \{
    */
    /** This global variable determines whether to enable Skip Start
    * functionality when LIN_1_SCB_Sleep() function is called:
    * 0 – disable, other values – enable. Default value is 1.
    * It is only available when Enable wakeup from Deep Sleep Mode is enabled.
    */
    uint8 LIN_1_SCB_skipStart = 1u;
    /** \} globals */
#endif /* (LIN_1_SCB_UART_WAKE_ENABLE_CONST && LIN_1_SCB_UART_RX_WAKEUP_IRQ) */

#if(LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    const LIN_1_SCB_UART_INIT_STRUCT LIN_1_SCB_configUart =
    {
        LIN_1_SCB_UART_SUB_MODE,
        LIN_1_SCB_UART_DIRECTION,
        LIN_1_SCB_UART_DATA_BITS_NUM,
        LIN_1_SCB_UART_PARITY_TYPE,
        LIN_1_SCB_UART_STOP_BITS_NUM,
        LIN_1_SCB_UART_OVS_FACTOR,
        LIN_1_SCB_UART_IRDA_LOW_POWER,
        LIN_1_SCB_UART_MEDIAN_FILTER_ENABLE,
        LIN_1_SCB_UART_RETRY_ON_NACK,
        LIN_1_SCB_UART_IRDA_POLARITY,
        LIN_1_SCB_UART_DROP_ON_PARITY_ERR,
        LIN_1_SCB_UART_DROP_ON_FRAME_ERR,
        LIN_1_SCB_UART_WAKE_ENABLE,
        0u,
        NULL,
        0u,
        NULL,
        LIN_1_SCB_UART_MP_MODE_ENABLE,
        LIN_1_SCB_UART_MP_ACCEPT_ADDRESS,
        LIN_1_SCB_UART_MP_RX_ADDRESS,
        LIN_1_SCB_UART_MP_RX_ADDRESS_MASK,
        (uint32) LIN_1_SCB_SCB_IRQ_INTERNAL,
        LIN_1_SCB_UART_INTR_RX_MASK,
        LIN_1_SCB_UART_RX_TRIGGER_LEVEL,
        LIN_1_SCB_UART_INTR_TX_MASK,
        LIN_1_SCB_UART_TX_TRIGGER_LEVEL,
        (uint8) LIN_1_SCB_UART_BYTE_MODE_ENABLE,
        (uint8) LIN_1_SCB_UART_CTS_ENABLE,
        (uint8) LIN_1_SCB_UART_CTS_POLARITY,
        (uint8) LIN_1_SCB_UART_RTS_POLARITY,
        (uint8) LIN_1_SCB_UART_RTS_FIFO_LEVEL
    };


    /*******************************************************************************
    * Function Name: LIN_1_SCB_UartInit
    ****************************************************************************//**
    *
    *  Configures the LIN_1_SCB for UART operation.
    *
    *  This function is intended specifically to be used when the LIN_1_SCB
    *  configuration is set to “Unconfigured LIN_1_SCB” in the customizer.
    *  After initializing the LIN_1_SCB in UART mode using this function,
    *  the component can be enabled using the LIN_1_SCB_Start() or
    * LIN_1_SCB_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration
    *  settings. This structure contains the same information that would otherwise
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of
    *   fields. These fields match the selections available in the customizer.
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void LIN_1_SCB_UartInit(const LIN_1_SCB_UART_INIT_STRUCT *config)
    {
        uint32 pinsConfig;

        if (NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Get direction to configure UART pins: TX, RX or TX+RX */
            pinsConfig  = config->direction;

        #if !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1)
            /* Add RTS and CTS pins to configure */
            pinsConfig |= (0u != config->rtsRxFifoLevel) ? (LIN_1_SCB_UART_RTS_PIN_ENABLE) : (0u);
            pinsConfig |= (0u != config->enableCts)      ? (LIN_1_SCB_UART_CTS_PIN_ENABLE) : (0u);
        #endif /* !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1) */

            /* Configure pins */
            LIN_1_SCB_SetPins(LIN_1_SCB_SCB_MODE_UART, config->mode, pinsConfig);

            /* Store internal configuration */
            LIN_1_SCB_scbMode       = (uint8) LIN_1_SCB_SCB_MODE_UART;
            LIN_1_SCB_scbEnableWake = (uint8) config->enableWake;
            LIN_1_SCB_scbEnableIntr = (uint8) config->enableInterrupt;

            /* Set RX direction internal variables */
            LIN_1_SCB_rxBuffer      =         config->rxBuffer;
            LIN_1_SCB_rxDataBits    = (uint8) config->dataBits;
            LIN_1_SCB_rxBufferSize  = (uint8) config->rxBufferSize;

            /* Set TX direction internal variables */
            LIN_1_SCB_txBuffer      =         config->txBuffer;
            LIN_1_SCB_txDataBits    = (uint8) config->dataBits;
            LIN_1_SCB_txBufferSize  = (uint8) config->txBufferSize;

            /* Configure UART interface */
            if(LIN_1_SCB_UART_MODE_IRDA == config->mode)
            {
                /* OVS settings: IrDA */
                LIN_1_SCB_CTRL_REG  = ((0u != config->enableIrdaLowPower) ?
                                                (LIN_1_SCB_UART_GET_CTRL_OVS_IRDA_LP(config->oversample)) :
                                                (LIN_1_SCB_CTRL_OVS_IRDA_OVS16));
            }
            else
            {
                /* OVS settings: UART and SmartCard */
                LIN_1_SCB_CTRL_REG  = LIN_1_SCB_GET_CTRL_OVS(config->oversample);
            }

            LIN_1_SCB_CTRL_REG     |= LIN_1_SCB_GET_CTRL_BYTE_MODE  (config->enableByteMode)      |
                                             LIN_1_SCB_GET_CTRL_ADDR_ACCEPT(config->multiprocAcceptAddr) |
                                             LIN_1_SCB_CTRL_UART;

            /* Configure sub-mode: UART, SmartCard or IrDA */
            LIN_1_SCB_UART_CTRL_REG = LIN_1_SCB_GET_UART_CTRL_MODE(config->mode);

            /* Configure RX direction */
            LIN_1_SCB_UART_RX_CTRL_REG = LIN_1_SCB_GET_UART_RX_CTRL_MODE(config->stopBits)              |
                                        LIN_1_SCB_GET_UART_RX_CTRL_POLARITY(config->enableInvertedRx)          |
                                        LIN_1_SCB_GET_UART_RX_CTRL_MP_MODE(config->enableMultiproc)            |
                                        LIN_1_SCB_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(config->dropOnParityErr) |
                                        LIN_1_SCB_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(config->dropOnFrameErr);

            if(LIN_1_SCB_UART_PARITY_NONE != config->parity)
            {
               LIN_1_SCB_UART_RX_CTRL_REG |= LIN_1_SCB_GET_UART_RX_CTRL_PARITY(config->parity) |
                                                    LIN_1_SCB_UART_RX_CTRL_PARITY_ENABLED;
            }

            LIN_1_SCB_RX_CTRL_REG      = LIN_1_SCB_GET_RX_CTRL_DATA_WIDTH(config->dataBits)       |
                                                LIN_1_SCB_GET_RX_CTRL_MEDIAN(config->enableMedianFilter) |
                                                LIN_1_SCB_GET_UART_RX_CTRL_ENABLED(config->direction);

            LIN_1_SCB_RX_FIFO_CTRL_REG = LIN_1_SCB_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure MP address */
            LIN_1_SCB_RX_MATCH_REG     = LIN_1_SCB_GET_RX_MATCH_ADDR(config->multiprocAddr) |
                                                LIN_1_SCB_GET_RX_MATCH_MASK(config->multiprocAddrMask);

            /* Configure RX direction */
            LIN_1_SCB_UART_TX_CTRL_REG = LIN_1_SCB_GET_UART_TX_CTRL_MODE(config->stopBits) |
                                                LIN_1_SCB_GET_UART_TX_CTRL_RETRY_NACK(config->enableRetryNack);

            if(LIN_1_SCB_UART_PARITY_NONE != config->parity)
            {
               LIN_1_SCB_UART_TX_CTRL_REG |= LIN_1_SCB_GET_UART_TX_CTRL_PARITY(config->parity) |
                                                    LIN_1_SCB_UART_TX_CTRL_PARITY_ENABLED;
            }

            LIN_1_SCB_TX_CTRL_REG      = LIN_1_SCB_GET_TX_CTRL_DATA_WIDTH(config->dataBits)    |
                                                LIN_1_SCB_GET_UART_TX_CTRL_ENABLED(config->direction);

            LIN_1_SCB_TX_FIFO_CTRL_REG = LIN_1_SCB_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

        #if !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1)
            LIN_1_SCB_UART_FLOW_CTRL_REG = LIN_1_SCB_GET_UART_FLOW_CTRL_CTS_ENABLE(config->enableCts) | \
                                            LIN_1_SCB_GET_UART_FLOW_CTRL_CTS_POLARITY (config->ctsPolarity)  | \
                                            LIN_1_SCB_GET_UART_FLOW_CTRL_RTS_POLARITY (config->rtsPolarity)  | \
                                            LIN_1_SCB_GET_UART_FLOW_CTRL_TRIGGER_LEVEL(config->rtsRxFifoLevel);
        #endif /* !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1) */

            /* Configure interrupt with UART handler but do not enable it */
            CyIntDisable    (LIN_1_SCB_ISR_NUMBER);
            CyIntSetPriority(LIN_1_SCB_ISR_NUMBER, LIN_1_SCB_ISR_PRIORITY);
            (void) CyIntSetVector(LIN_1_SCB_ISR_NUMBER, &LIN_1_SCB_SPI_UART_ISR);

            /* Configure WAKE interrupt */
        #if(LIN_1_SCB_UART_RX_WAKEUP_IRQ)
            CyIntDisable    (LIN_1_SCB_RX_WAKE_ISR_NUMBER);
            CyIntSetPriority(LIN_1_SCB_RX_WAKE_ISR_NUMBER, LIN_1_SCB_RX_WAKE_ISR_PRIORITY);
            (void) CyIntSetVector(LIN_1_SCB_RX_WAKE_ISR_NUMBER, &LIN_1_SCB_UART_WAKEUP_ISR);
        #endif /* (LIN_1_SCB_UART_RX_WAKEUP_IRQ) */

            /* Configure interrupt sources */
            LIN_1_SCB_INTR_I2C_EC_MASK_REG = LIN_1_SCB_NO_INTR_SOURCES;
            LIN_1_SCB_INTR_SPI_EC_MASK_REG = LIN_1_SCB_NO_INTR_SOURCES;
            LIN_1_SCB_INTR_SLAVE_MASK_REG  = LIN_1_SCB_NO_INTR_SOURCES;
            LIN_1_SCB_INTR_MASTER_MASK_REG = LIN_1_SCB_NO_INTR_SOURCES;
            LIN_1_SCB_INTR_RX_MASK_REG     = config->rxInterruptMask;
            LIN_1_SCB_INTR_TX_MASK_REG     = config->txInterruptMask;
        
            /* Configure TX interrupt sources to restore. */
            LIN_1_SCB_IntrTxMask = LO16(LIN_1_SCB_INTR_TX_MASK_REG);

            /* Clear RX buffer indexes */
            LIN_1_SCB_rxBufferHead     = 0u;
            LIN_1_SCB_rxBufferTail     = 0u;
            LIN_1_SCB_rxBufferOverflow = 0u;

            /* Clear TX buffer indexes */
            LIN_1_SCB_txBufferHead = 0u;
            LIN_1_SCB_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: LIN_1_SCB_UartInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the UART operation.
    *
    *******************************************************************************/
    void LIN_1_SCB_UartInit(void)
    {
        /* Configure UART interface */
        LIN_1_SCB_CTRL_REG = LIN_1_SCB_UART_DEFAULT_CTRL;

        /* Configure sub-mode: UART, SmartCard or IrDA */
        LIN_1_SCB_UART_CTRL_REG = LIN_1_SCB_UART_DEFAULT_UART_CTRL;

        /* Configure RX direction */
        LIN_1_SCB_UART_RX_CTRL_REG = LIN_1_SCB_UART_DEFAULT_UART_RX_CTRL;
        LIN_1_SCB_RX_CTRL_REG      = LIN_1_SCB_UART_DEFAULT_RX_CTRL;
        LIN_1_SCB_RX_FIFO_CTRL_REG = LIN_1_SCB_UART_DEFAULT_RX_FIFO_CTRL;
        LIN_1_SCB_RX_MATCH_REG     = LIN_1_SCB_UART_DEFAULT_RX_MATCH_REG;

        /* Configure TX direction */
        LIN_1_SCB_UART_TX_CTRL_REG = LIN_1_SCB_UART_DEFAULT_UART_TX_CTRL;
        LIN_1_SCB_TX_CTRL_REG      = LIN_1_SCB_UART_DEFAULT_TX_CTRL;
        LIN_1_SCB_TX_FIFO_CTRL_REG = LIN_1_SCB_UART_DEFAULT_TX_FIFO_CTRL;

    #if !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1)
        LIN_1_SCB_UART_FLOW_CTRL_REG = LIN_1_SCB_UART_DEFAULT_FLOW_CTRL;
    #endif /* !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1) */

        /* Configure interrupt with UART handler but do not enable it */
    #if(LIN_1_SCB_SCB_IRQ_INTERNAL)
        CyIntDisable    (LIN_1_SCB_ISR_NUMBER);
        CyIntSetPriority(LIN_1_SCB_ISR_NUMBER, LIN_1_SCB_ISR_PRIORITY);
        (void) CyIntSetVector(LIN_1_SCB_ISR_NUMBER, &LIN_1_SCB_SPI_UART_ISR);
    #endif /* (LIN_1_SCB_SCB_IRQ_INTERNAL) */

        /* Configure WAKE interrupt */
    #if(LIN_1_SCB_UART_RX_WAKEUP_IRQ)
        CyIntDisable    (LIN_1_SCB_RX_WAKE_ISR_NUMBER);
        CyIntSetPriority(LIN_1_SCB_RX_WAKE_ISR_NUMBER, LIN_1_SCB_RX_WAKE_ISR_PRIORITY);
        (void) CyIntSetVector(LIN_1_SCB_RX_WAKE_ISR_NUMBER, &LIN_1_SCB_UART_WAKEUP_ISR);
    #endif /* (LIN_1_SCB_UART_RX_WAKEUP_IRQ) */

        /* Configure interrupt sources */
        LIN_1_SCB_INTR_I2C_EC_MASK_REG = LIN_1_SCB_UART_DEFAULT_INTR_I2C_EC_MASK;
        LIN_1_SCB_INTR_SPI_EC_MASK_REG = LIN_1_SCB_UART_DEFAULT_INTR_SPI_EC_MASK;
        LIN_1_SCB_INTR_SLAVE_MASK_REG  = LIN_1_SCB_UART_DEFAULT_INTR_SLAVE_MASK;
        LIN_1_SCB_INTR_MASTER_MASK_REG = LIN_1_SCB_UART_DEFAULT_INTR_MASTER_MASK;
        LIN_1_SCB_INTR_RX_MASK_REG     = LIN_1_SCB_UART_DEFAULT_INTR_RX_MASK;
        LIN_1_SCB_INTR_TX_MASK_REG     = LIN_1_SCB_UART_DEFAULT_INTR_TX_MASK;
    
        /* Configure TX interrupt sources to restore. */
        LIN_1_SCB_IntrTxMask = LO16(LIN_1_SCB_INTR_TX_MASK_REG);

    #if(LIN_1_SCB_INTERNAL_RX_SW_BUFFER_CONST)
        LIN_1_SCB_rxBufferHead     = 0u;
        LIN_1_SCB_rxBufferTail     = 0u;
        LIN_1_SCB_rxBufferOverflow = 0u;
    #endif /* (LIN_1_SCB_INTERNAL_RX_SW_BUFFER_CONST) */

    #if(LIN_1_SCB_INTERNAL_TX_SW_BUFFER_CONST)
        LIN_1_SCB_txBufferHead = 0u;
        LIN_1_SCB_txBufferTail = 0u;
    #endif /* (LIN_1_SCB_INTERNAL_TX_SW_BUFFER_CONST) */
    }
#endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: LIN_1_SCB_UartPostEnable
****************************************************************************//**
*
*  Restores HSIOM settings for the UART output pins (TX and/or RTS) to be
*  controlled by the SCB UART.
*
*******************************************************************************/
void LIN_1_SCB_UartPostEnable(void)
{
#if (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (LIN_1_SCB_TX_SDA_MISO_PIN)
        if (LIN_1_SCB_CHECK_TX_SDA_MISO_PIN_USED)
        {
            /* Set SCB UART to drive the output pin */
            LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_TX_SDA_MISO_HSIOM_REG, LIN_1_SCB_TX_SDA_MISO_HSIOM_MASK,
                                           LIN_1_SCB_TX_SDA_MISO_HSIOM_POS, LIN_1_SCB_TX_SDA_MISO_HSIOM_SEL_UART);
        }
    #endif /* (LIN_1_SCB_TX_SDA_MISO_PIN_PIN) */

    #if !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1)
        #if (LIN_1_SCB_RTS_SS0_PIN)
            if (LIN_1_SCB_CHECK_RTS_SS0_PIN_USED)
            {
                /* Set SCB UART to drive the output pin */
                LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_RTS_SS0_HSIOM_REG, LIN_1_SCB_RTS_SS0_HSIOM_MASK,
                                               LIN_1_SCB_RTS_SS0_HSIOM_POS, LIN_1_SCB_RTS_SS0_HSIOM_SEL_UART);
            }
        #endif /* (LIN_1_SCB_RTS_SS0_PIN) */
    #endif /* !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1) */

#else
    #if (LIN_1_SCB_UART_TX_PIN)
         /* Set SCB UART to drive the output pin */
        LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_TX_HSIOM_REG, LIN_1_SCB_TX_HSIOM_MASK,
                                       LIN_1_SCB_TX_HSIOM_POS, LIN_1_SCB_TX_HSIOM_SEL_UART);
    #endif /* (LIN_1_SCB_UART_TX_PIN) */

    #if (LIN_1_SCB_UART_RTS_PIN)
        /* Set SCB UART to drive the output pin */
        LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_RTS_HSIOM_REG, LIN_1_SCB_RTS_HSIOM_MASK,
                                       LIN_1_SCB_RTS_HSIOM_POS, LIN_1_SCB_RTS_HSIOM_SEL_UART);
    #endif /* (LIN_1_SCB_UART_RTS_PIN) */
#endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Restore TX interrupt sources. */
    LIN_1_SCB_SetTxInterruptMode(LIN_1_SCB_IntrTxMask);
}


/*******************************************************************************
* Function Name: LIN_1_SCB_UartStop
****************************************************************************//**
*
*  Changes the HSIOM settings for the UART output pins (TX and/or RTS) to keep
*  them inactive after the block is disabled. The output pins are controlled by
*  the GPIO data register. Also, the function disables the skip start feature
*  to not cause it to trigger after the component is enabled.
*
*******************************************************************************/
void LIN_1_SCB_UartStop(void)
{
#if(LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (LIN_1_SCB_TX_SDA_MISO_PIN)
        if (LIN_1_SCB_CHECK_TX_SDA_MISO_PIN_USED)
        {
            /* Set GPIO to drive output pin */
            LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_TX_SDA_MISO_HSIOM_REG, LIN_1_SCB_TX_SDA_MISO_HSIOM_MASK,
                                           LIN_1_SCB_TX_SDA_MISO_HSIOM_POS, LIN_1_SCB_TX_SDA_MISO_HSIOM_SEL_GPIO);
        }
    #endif /* (LIN_1_SCB_TX_SDA_MISO_PIN_PIN) */

    #if !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1)
        #if (LIN_1_SCB_RTS_SS0_PIN)
            if (LIN_1_SCB_CHECK_RTS_SS0_PIN_USED)
            {
                /* Set output pin state after block is disabled */
                LIN_1_SCB_uart_rts_spi_ss0_Write(LIN_1_SCB_GET_UART_RTS_INACTIVE);

                /* Set GPIO to drive output pin */
                LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_RTS_SS0_HSIOM_REG, LIN_1_SCB_RTS_SS0_HSIOM_MASK,
                                               LIN_1_SCB_RTS_SS0_HSIOM_POS, LIN_1_SCB_RTS_SS0_HSIOM_SEL_GPIO);
            }
        #endif /* (LIN_1_SCB_RTS_SS0_PIN) */
    #endif /* !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1) */

#else
    #if (LIN_1_SCB_UART_TX_PIN)
        /* Set GPIO to drive output pin */
        LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_TX_HSIOM_REG, LIN_1_SCB_TX_HSIOM_MASK,
                                       LIN_1_SCB_TX_HSIOM_POS, LIN_1_SCB_TX_HSIOM_SEL_GPIO);
    #endif /* (LIN_1_SCB_UART_TX_PIN) */

    #if (LIN_1_SCB_UART_RTS_PIN)
        /* Set output pin state after block is disabled */
        LIN_1_SCB_rts_Write(LIN_1_SCB_GET_UART_RTS_INACTIVE);

        /* Set GPIO to drive output pin */
        LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_RTS_HSIOM_REG, LIN_1_SCB_RTS_HSIOM_MASK,
                                       LIN_1_SCB_RTS_HSIOM_POS, LIN_1_SCB_RTS_HSIOM_SEL_GPIO);
    #endif /* (LIN_1_SCB_UART_RTS_PIN) */

#endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (LIN_1_SCB_UART_WAKE_ENABLE_CONST)
    /* Disable skip start feature used for wakeup */
    LIN_1_SCB_UART_RX_CTRL_REG &= (uint32) ~LIN_1_SCB_UART_RX_CTRL_SKIP_START;
#endif /* (LIN_1_SCB_UART_WAKE_ENABLE_CONST) */

    /* Store TX interrupt sources (exclude level triggered). */
    LIN_1_SCB_IntrTxMask = LO16(LIN_1_SCB_GetTxInterruptMode() & LIN_1_SCB_INTR_UART_TX_RESTORE);
}


/*******************************************************************************
* Function Name: LIN_1_SCB_UartSetRxAddress
****************************************************************************//**
*
*  Sets the hardware detectable receiver address for the UART in the
*  Multiprocessor mode.
*
*  \param address: Address for hardware address detection.
*
*******************************************************************************/
void LIN_1_SCB_UartSetRxAddress(uint32 address)
{
     uint32 matchReg;

    matchReg = LIN_1_SCB_RX_MATCH_REG;

    matchReg &= ((uint32) ~LIN_1_SCB_RX_MATCH_ADDR_MASK); /* Clear address bits */
    matchReg |= ((uint32)  (address & LIN_1_SCB_RX_MATCH_ADDR_MASK)); /* Set address  */

    LIN_1_SCB_RX_MATCH_REG = matchReg;
}


/*******************************************************************************
* Function Name: LIN_1_SCB_UartSetRxAddressMask
****************************************************************************//**
*
*  Sets the hardware address mask for the UART in the Multiprocessor mode.
*
*  \param addressMask: Address mask.
*   - Bit value 0 – excludes bit from address comparison.
*   - Bit value 1 – the bit needs to match with the corresponding bit
*     of the address.
*
*******************************************************************************/
void LIN_1_SCB_UartSetRxAddressMask(uint32 addressMask)
{
    uint32 matchReg;

    matchReg = LIN_1_SCB_RX_MATCH_REG;

    matchReg &= ((uint32) ~LIN_1_SCB_RX_MATCH_MASK_MASK); /* Clear address mask bits */
    matchReg |= ((uint32) (addressMask << LIN_1_SCB_RX_MATCH_MASK_POS));

    LIN_1_SCB_RX_MATCH_REG = matchReg;
}


#if(LIN_1_SCB_UART_RX_DIRECTION)
    /*******************************************************************************
    * Function Name: LIN_1_SCB_UartGetChar
    ****************************************************************************//**
    *
    *  Retrieves next data element from receive buffer.
    *  This function is designed for ASCII characters and returns a char where
    *  1 to 255 are valid characters and 0 indicates an error occurred or no data
    *  is present.
    *  - RX software buffer is disabled: Returns data element retrieved from RX
    *    FIFO.
    *  - RX software buffer is enabled: Returns data element from the software
    *    receive buffer.
    *
    *  \return
    *   Next data element from the receive buffer. ASCII character values from
    *   1 to 255 are valid. A returned zero signifies an error condition or no
    *   data available.
    *
    *  \sideeffect
    *   The errors bits may not correspond with reading characters due to
    *   RX FIFO and software buffer usage.
    *   RX software buffer is enabled: The internal software buffer overflow
    *   is not treated as an error condition.
    *   Check LIN_1_SCB_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 LIN_1_SCB_UartGetChar(void)
    {
        uint32 rxData = 0u;

        /* Reads data only if there is data to read */
        if (0u != LIN_1_SCB_SpiUartGetRxBufferSize())
        {
            rxData = LIN_1_SCB_SpiUartReadRxData();
        }

        if (LIN_1_SCB_CHECK_INTR_RX(LIN_1_SCB_INTR_RX_ERR))
        {
            rxData = 0u; /* Error occurred: returns zero */
            LIN_1_SCB_ClearRxInterruptSource(LIN_1_SCB_INTR_RX_ERR);
        }

        return (rxData);
    }


    /*******************************************************************************
    * Function Name: LIN_1_SCB_UartGetByte
    ****************************************************************************//**
    *
    *  Retrieves the next data element from the receive buffer, returns the
    *  received byte and error condition.
    *   - The RX software buffer is disabled: returns the data element retrieved
    *     from the RX FIFO. Undefined data will be returned if the RX FIFO is
    *     empty.
    *   - The RX software buffer is enabled: returns data element from the
    *     software receive buffer.
    *
    *  \return
    *   Bits 7-0 contain the next data element from the receive buffer and
    *   other bits contain the error condition.
    *   - LIN_1_SCB_UART_RX_OVERFLOW - Attempt to write to a full
    *     receiver FIFO.
    *   - LIN_1_SCB_UART_RX_UNDERFLOW	Attempt to read from an empty
    *     receiver FIFO.
    *   - LIN_1_SCB_UART_RX_FRAME_ERROR - UART framing error detected.
    *   - LIN_1_SCB_UART_RX_PARITY_ERROR - UART parity error detected.
    *
    *  \sideeffect
    *   The errors bits may not correspond with reading characters due to
    *   RX FIFO and software buffer usage.
    *   RX software buffer is enabled: The internal software buffer overflow
    *   is not treated as an error condition.
    *   Check LIN_1_SCB_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 LIN_1_SCB_UartGetByte(void)
    {
        uint32 rxData;
        uint32 tmpStatus;

        #if (LIN_1_SCB_CHECK_RX_SW_BUFFER)
        {
            LIN_1_SCB_DisableInt();
        }
        #endif

        if (0u != LIN_1_SCB_SpiUartGetRxBufferSize())
        {
            /* Enables interrupt to receive more bytes: at least one byte is in
            * buffer.
            */
            #if (LIN_1_SCB_CHECK_RX_SW_BUFFER)
            {
                LIN_1_SCB_EnableInt();
            }
            #endif

            /* Get received byte */
            rxData = LIN_1_SCB_SpiUartReadRxData();
        }
        else
        {
            /* Reads a byte directly from RX FIFO: underflow is raised in the
            * case of empty. Otherwise the first received byte will be read.
            */
            rxData = LIN_1_SCB_RX_FIFO_RD_REG;


            /* Enables interrupt to receive more bytes. */
            #if (LIN_1_SCB_CHECK_RX_SW_BUFFER)
            {

                /* The byte has been read from RX FIFO. Clear RX interrupt to
                * not involve interrupt handler when RX FIFO is empty.
                */
                LIN_1_SCB_ClearRxInterruptSource(LIN_1_SCB_INTR_RX_NOT_EMPTY);

                LIN_1_SCB_EnableInt();
            }
            #endif
        }

        /* Get and clear RX error mask */
        tmpStatus = (LIN_1_SCB_GetRxInterruptSource() & LIN_1_SCB_INTR_RX_ERR);
        LIN_1_SCB_ClearRxInterruptSource(LIN_1_SCB_INTR_RX_ERR);

        /* Puts together data and error status:
        * MP mode and accept address: 9th bit is set to notify mark.
        */
        rxData |= ((uint32) (tmpStatus << 8u));

        return (rxData);
    }


    #if !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: LIN_1_SCB_UartSetRtsPolarity
        ****************************************************************************//**
        *
        *  Sets active polarity of RTS output signal.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *  \param polarity: Active polarity of RTS output signal.
        *   - LIN_1_SCB_UART_RTS_ACTIVE_LOW  - RTS signal is active low.
        *   - LIN_1_SCB_UART_RTS_ACTIVE_HIGH - RTS signal is active high.
        *
        *******************************************************************************/
        void LIN_1_SCB_UartSetRtsPolarity(uint32 polarity)
        {
            if(0u != polarity)
            {
                LIN_1_SCB_UART_FLOW_CTRL_REG |= (uint32)  LIN_1_SCB_UART_FLOW_CTRL_RTS_POLARITY;
            }
            else
            {
                LIN_1_SCB_UART_FLOW_CTRL_REG &= (uint32) ~LIN_1_SCB_UART_FLOW_CTRL_RTS_POLARITY;
            }
        }


        /*******************************************************************************
        * Function Name: LIN_1_SCB_UartSetRtsFifoLevel
        ****************************************************************************//**
        *
        *  Sets level in the RX FIFO for RTS signal activation.
        *  While the RX FIFO has fewer entries than the RX FIFO level the RTS signal
        *  remains active, otherwise the RTS signal becomes inactive.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *  \param level: Level in the RX FIFO for RTS signal activation.
        *   The range of valid level values is between 0 and RX FIFO depth - 1.
        *   Setting level value to 0 disables RTS signal activation.
        *
        *******************************************************************************/
        void LIN_1_SCB_UartSetRtsFifoLevel(uint32 level)
        {
            uint32 uartFlowCtrl;

            uartFlowCtrl = LIN_1_SCB_UART_FLOW_CTRL_REG;

            uartFlowCtrl &= ((uint32) ~LIN_1_SCB_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
            uartFlowCtrl |= ((uint32) (LIN_1_SCB_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK & level));

            LIN_1_SCB_UART_FLOW_CTRL_REG = uartFlowCtrl;
        }
    #endif /* !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1) */

#endif /* (LIN_1_SCB_UART_RX_DIRECTION) */


#if(LIN_1_SCB_UART_TX_DIRECTION)
    /*******************************************************************************
    * Function Name: LIN_1_SCB_UartPutString
    ****************************************************************************//**
    *
    *  Places a NULL terminated string in the transmit buffer to be sent at the
    *  next available bus time.
    *  This function is blocking and waits until there is a space available to put
    *  requested data in transmit buffer.
    *
    *  \param string: pointer to the null terminated string array to be placed in the
    *   transmit buffer.
    *
    *******************************************************************************/
    void LIN_1_SCB_UartPutString(const char8 string[])
    {
        uint32 bufIndex;

        bufIndex = 0u;

        /* Blocks the control flow until all data has been sent */
        while(string[bufIndex] != ((char8) 0))
        {
            LIN_1_SCB_UartPutChar((uint32) string[bufIndex]);
            bufIndex++;
        }
    }


    /*******************************************************************************
    * Function Name: LIN_1_SCB_UartPutCRLF
    ****************************************************************************//**
    *
    *  Places byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) in the transmit buffer.
    *  This function is blocking and waits until there is a space available to put
    *  all requested data in transmit buffer.
    *
    *  \param txDataByte: the data to be transmitted.
    *
    *******************************************************************************/
    void LIN_1_SCB_UartPutCRLF(uint32 txDataByte)
    {
        LIN_1_SCB_UartPutChar(txDataByte);  /* Blocks control flow until all data has been sent */
        LIN_1_SCB_UartPutChar(0x0Du);       /* Blocks control flow until all data has been sent */
        LIN_1_SCB_UartPutChar(0x0Au);       /* Blocks control flow until all data has been sent */
    }


    #if !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: LIN_1_SCBSCB_UartEnableCts
        ****************************************************************************//**
        *
        *  Enables usage of CTS input signal by the UART transmitter.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *******************************************************************************/
        void LIN_1_SCB_UartEnableCts(void)
        {
            LIN_1_SCB_UART_FLOW_CTRL_REG |= (uint32)  LIN_1_SCB_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: LIN_1_SCB_UartDisableCts
        ****************************************************************************//**
        *
        *  Disables usage of CTS input signal by the UART transmitter.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *******************************************************************************/
        void LIN_1_SCB_UartDisableCts(void)
        {
            LIN_1_SCB_UART_FLOW_CTRL_REG &= (uint32) ~LIN_1_SCB_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: LIN_1_SCB_UartSetCtsPolarity
        ****************************************************************************//**
        *
        *  Sets active polarity of CTS input signal.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *  \param polarity: Active polarity of CTS output signal.
        *   - LIN_1_SCB_UART_CTS_ACTIVE_LOW  - CTS signal is active low.
        *   - LIN_1_SCB_UART_CTS_ACTIVE_HIGH - CTS signal is active high.
        *
        *******************************************************************************/
        void LIN_1_SCB_UartSetCtsPolarity(uint32 polarity)
        {
            if (0u != polarity)
            {
                LIN_1_SCB_UART_FLOW_CTRL_REG |= (uint32)  LIN_1_SCB_UART_FLOW_CTRL_CTS_POLARITY;
            }
            else
            {
                LIN_1_SCB_UART_FLOW_CTRL_REG &= (uint32) ~LIN_1_SCB_UART_FLOW_CTRL_CTS_POLARITY;
            }
        }
    #endif /* !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1) */

#endif /* (LIN_1_SCB_UART_TX_DIRECTION) */


#if (LIN_1_SCB_UART_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: LIN_1_SCB_UartSaveConfig
    ****************************************************************************//**
    *
    *  Clears and enables an interrupt on a falling edge of the Rx input. The GPIO
    *  interrupt does not track in the active mode, therefore requires to be 
    *  cleared by this API.
    *
    *******************************************************************************/
    void LIN_1_SCB_UartSaveConfig(void)
    {
    #if (LIN_1_SCB_UART_RX_WAKEUP_IRQ)
        /* Set SKIP_START if requested (set by default). */
        if (0u != LIN_1_SCB_skipStart)
        {
            LIN_1_SCB_UART_RX_CTRL_REG |= (uint32)  LIN_1_SCB_UART_RX_CTRL_SKIP_START;
        }
        else
        {
            LIN_1_SCB_UART_RX_CTRL_REG &= (uint32) ~LIN_1_SCB_UART_RX_CTRL_SKIP_START;
        }
        
        /* Clear RX GPIO interrupt status and pending interrupt in NVIC because
        * falling edge on RX line occurs while UART communication in active mode.
        * Enable interrupt: next interrupt trigger should wakeup device.
        */
        LIN_1_SCB_CLEAR_UART_RX_WAKE_INTR;
        LIN_1_SCB_RxWakeClearPendingInt();
        LIN_1_SCB_RxWakeEnableInt();
    #endif /* (LIN_1_SCB_UART_RX_WAKEUP_IRQ) */
    }


    /*******************************************************************************
    * Function Name: LIN_1_SCB_UartRestoreConfig
    ****************************************************************************//**
    *
    *  Disables the RX GPIO interrupt. Until this function is called the interrupt
    *  remains active and triggers on every falling edge of the UART RX line.
    *
    *******************************************************************************/
    void LIN_1_SCB_UartRestoreConfig(void)
    {
    #if (LIN_1_SCB_UART_RX_WAKEUP_IRQ)
        /* Disable interrupt: no more triggers in active mode */
        LIN_1_SCB_RxWakeDisableInt();
    #endif /* (LIN_1_SCB_UART_RX_WAKEUP_IRQ) */
    }


    #if (LIN_1_SCB_UART_RX_WAKEUP_IRQ)
        /*******************************************************************************
        * Function Name: LIN_1_SCB_UART_WAKEUP_ISR
        ****************************************************************************//**
        *
        *  Handles the Interrupt Service Routine for the SCB UART mode GPIO wakeup
        *  event. This event is configured to trigger on a falling edge of the RX line.
        *
        *******************************************************************************/
        CY_ISR(LIN_1_SCB_UART_WAKEUP_ISR)
        {
        #ifdef LIN_1_SCB_UART_WAKEUP_ISR_ENTRY_CALLBACK
            LIN_1_SCB_UART_WAKEUP_ISR_EntryCallback();
        #endif /* LIN_1_SCB_UART_WAKEUP_ISR_ENTRY_CALLBACK */

            LIN_1_SCB_CLEAR_UART_RX_WAKE_INTR;

        #ifdef LIN_1_SCB_UART_WAKEUP_ISR_EXIT_CALLBACK
            LIN_1_SCB_UART_WAKEUP_ISR_ExitCallback();
        #endif /* LIN_1_SCB_UART_WAKEUP_ISR_EXIT_CALLBACK */
        }
    #endif /* (LIN_1_SCB_UART_RX_WAKEUP_IRQ) */
#endif /* (LIN_1_SCB_UART_RX_WAKEUP_IRQ) */


/* [] END OF FILE */
