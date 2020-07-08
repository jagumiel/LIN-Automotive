/***************************************************************************//**
* \file LINS_SCB_UART.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  UART mode.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LINS_SCB_PVT.h"
#include "LINS_SCB_SPI_UART_PVT.h"


#if (LINS_SCB_UART_WAKE_ENABLE_CONST && LINS_SCB_UART_RX_WAKEUP_IRQ)
    /**
    * \addtogroup group_globals
    * \{
    */
    /** This global variable determines whether to enable Skip Start
    * functionality when LINS_SCB_Sleep() function is called:
    * 0 – disable, other values – enable. Default value is 1.
    * It is only available when Enable wakeup from Deep Sleep Mode is enabled.
    */
    uint8 LINS_SCB_skipStart = 1u;
    /** \} globals */
#endif /* (LINS_SCB_UART_WAKE_ENABLE_CONST && LINS_SCB_UART_RX_WAKEUP_IRQ) */

#if(LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    const LINS_SCB_UART_INIT_STRUCT LINS_SCB_configUart =
    {
        LINS_SCB_UART_SUB_MODE,
        LINS_SCB_UART_DIRECTION,
        LINS_SCB_UART_DATA_BITS_NUM,
        LINS_SCB_UART_PARITY_TYPE,
        LINS_SCB_UART_STOP_BITS_NUM,
        LINS_SCB_UART_OVS_FACTOR,
        LINS_SCB_UART_IRDA_LOW_POWER,
        LINS_SCB_UART_MEDIAN_FILTER_ENABLE,
        LINS_SCB_UART_RETRY_ON_NACK,
        LINS_SCB_UART_IRDA_POLARITY,
        LINS_SCB_UART_DROP_ON_PARITY_ERR,
        LINS_SCB_UART_DROP_ON_FRAME_ERR,
        LINS_SCB_UART_WAKE_ENABLE,
        0u,
        NULL,
        0u,
        NULL,
        LINS_SCB_UART_MP_MODE_ENABLE,
        LINS_SCB_UART_MP_ACCEPT_ADDRESS,
        LINS_SCB_UART_MP_RX_ADDRESS,
        LINS_SCB_UART_MP_RX_ADDRESS_MASK,
        (uint32) LINS_SCB_SCB_IRQ_INTERNAL,
        LINS_SCB_UART_INTR_RX_MASK,
        LINS_SCB_UART_RX_TRIGGER_LEVEL,
        LINS_SCB_UART_INTR_TX_MASK,
        LINS_SCB_UART_TX_TRIGGER_LEVEL,
        (uint8) LINS_SCB_UART_BYTE_MODE_ENABLE,
        (uint8) LINS_SCB_UART_CTS_ENABLE,
        (uint8) LINS_SCB_UART_CTS_POLARITY,
        (uint8) LINS_SCB_UART_RTS_POLARITY,
        (uint8) LINS_SCB_UART_RTS_FIFO_LEVEL,
        (uint8) LINS_SCB_UART_RX_BREAK_WIDTH
    };


    /*******************************************************************************
    * Function Name: LINS_SCB_UartInit
    ****************************************************************************//**
    *
    *  Configures the LINS_SCB for UART operation.
    *
    *  This function is intended specifically to be used when the LINS_SCB
    *  configuration is set to “Unconfigured LINS_SCB” in the customizer.
    *  After initializing the LINS_SCB in UART mode using this function,
    *  the component can be enabled using the LINS_SCB_Start() or
    * LINS_SCB_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration
    *  settings. This structure contains the same information that would otherwise
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of
    *   fields. These fields match the selections available in the customizer.
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void LINS_SCB_UartInit(const LINS_SCB_UART_INIT_STRUCT *config)
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

        #if !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1)
            /* Add RTS and CTS pins to configure */
            pinsConfig |= (0u != config->rtsRxFifoLevel) ? (LINS_SCB_UART_RTS_PIN_ENABLE) : (0u);
            pinsConfig |= (0u != config->enableCts)      ? (LINS_SCB_UART_CTS_PIN_ENABLE) : (0u);
        #endif /* !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1) */

            /* Configure pins */
            LINS_SCB_SetPins(LINS_SCB_SCB_MODE_UART, config->mode, pinsConfig);

            /* Store internal configuration */
            LINS_SCB_scbMode       = (uint8) LINS_SCB_SCB_MODE_UART;
            LINS_SCB_scbEnableWake = (uint8) config->enableWake;
            LINS_SCB_scbEnableIntr = (uint8) config->enableInterrupt;

            /* Set RX direction internal variables */
            LINS_SCB_rxBuffer      =         config->rxBuffer;
            LINS_SCB_rxDataBits    = (uint8) config->dataBits;
            LINS_SCB_rxBufferSize  =         config->rxBufferSize;

            /* Set TX direction internal variables */
            LINS_SCB_txBuffer      =         config->txBuffer;
            LINS_SCB_txDataBits    = (uint8) config->dataBits;
            LINS_SCB_txBufferSize  =         config->txBufferSize;

            /* Configure UART interface */
            if(LINS_SCB_UART_MODE_IRDA == config->mode)
            {
                /* OVS settings: IrDA */
                LINS_SCB_CTRL_REG  = ((0u != config->enableIrdaLowPower) ?
                                                (LINS_SCB_UART_GET_CTRL_OVS_IRDA_LP(config->oversample)) :
                                                (LINS_SCB_CTRL_OVS_IRDA_OVS16));
            }
            else
            {
                /* OVS settings: UART and SmartCard */
                LINS_SCB_CTRL_REG  = LINS_SCB_GET_CTRL_OVS(config->oversample);
            }

            LINS_SCB_CTRL_REG     |= LINS_SCB_GET_CTRL_BYTE_MODE  (config->enableByteMode)      |
                                             LINS_SCB_GET_CTRL_ADDR_ACCEPT(config->multiprocAcceptAddr) |
                                             LINS_SCB_CTRL_UART;

            /* Configure sub-mode: UART, SmartCard or IrDA */
            LINS_SCB_UART_CTRL_REG = LINS_SCB_GET_UART_CTRL_MODE(config->mode);

            /* Configure RX direction */
            LINS_SCB_UART_RX_CTRL_REG = LINS_SCB_GET_UART_RX_CTRL_MODE(config->stopBits)              |
                                        LINS_SCB_GET_UART_RX_CTRL_POLARITY(config->enableInvertedRx)          |
                                        LINS_SCB_GET_UART_RX_CTRL_MP_MODE(config->enableMultiproc)            |
                                        LINS_SCB_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(config->dropOnParityErr) |
                                        LINS_SCB_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(config->dropOnFrameErr)   |
                                        LINS_SCB_GET_UART_RX_CTRL_BREAK_WIDTH(config->breakWidth);

            if(LINS_SCB_UART_PARITY_NONE != config->parity)
            {
               LINS_SCB_UART_RX_CTRL_REG |= LINS_SCB_GET_UART_RX_CTRL_PARITY(config->parity) |
                                                    LINS_SCB_UART_RX_CTRL_PARITY_ENABLED;
            }

            LINS_SCB_RX_CTRL_REG      = LINS_SCB_GET_RX_CTRL_DATA_WIDTH(config->dataBits)       |
                                                LINS_SCB_GET_RX_CTRL_MEDIAN(config->enableMedianFilter) |
                                                LINS_SCB_GET_UART_RX_CTRL_ENABLED(config->direction);

            LINS_SCB_RX_FIFO_CTRL_REG = LINS_SCB_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure MP address */
            LINS_SCB_RX_MATCH_REG     = LINS_SCB_GET_RX_MATCH_ADDR(config->multiprocAddr) |
                                                LINS_SCB_GET_RX_MATCH_MASK(config->multiprocAddrMask);

            /* Configure RX direction */
            LINS_SCB_UART_TX_CTRL_REG = LINS_SCB_GET_UART_TX_CTRL_MODE(config->stopBits) |
                                                LINS_SCB_GET_UART_TX_CTRL_RETRY_NACK(config->enableRetryNack);

            if(LINS_SCB_UART_PARITY_NONE != config->parity)
            {
               LINS_SCB_UART_TX_CTRL_REG |= LINS_SCB_GET_UART_TX_CTRL_PARITY(config->parity) |
                                                    LINS_SCB_UART_TX_CTRL_PARITY_ENABLED;
            }

            LINS_SCB_TX_CTRL_REG      = LINS_SCB_GET_TX_CTRL_DATA_WIDTH(config->dataBits)    |
                                                LINS_SCB_GET_UART_TX_CTRL_ENABLED(config->direction);

            LINS_SCB_TX_FIFO_CTRL_REG = LINS_SCB_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

        #if !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1)
            LINS_SCB_UART_FLOW_CTRL_REG = LINS_SCB_GET_UART_FLOW_CTRL_CTS_ENABLE(config->enableCts) | \
                                            LINS_SCB_GET_UART_FLOW_CTRL_CTS_POLARITY (config->ctsPolarity)  | \
                                            LINS_SCB_GET_UART_FLOW_CTRL_RTS_POLARITY (config->rtsPolarity)  | \
                                            LINS_SCB_GET_UART_FLOW_CTRL_TRIGGER_LEVEL(config->rtsRxFifoLevel);
        #endif /* !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1) */

            /* Configure interrupt with UART handler but do not enable it */
            CyIntDisable    (LINS_SCB_ISR_NUMBER);
            CyIntSetPriority(LINS_SCB_ISR_NUMBER, LINS_SCB_ISR_PRIORITY);
            (void) CyIntSetVector(LINS_SCB_ISR_NUMBER, &LINS_SCB_SPI_UART_ISR);

            /* Configure WAKE interrupt */
        #if(LINS_SCB_UART_RX_WAKEUP_IRQ)
            CyIntDisable    (LINS_SCB_RX_WAKE_ISR_NUMBER);
            CyIntSetPriority(LINS_SCB_RX_WAKE_ISR_NUMBER, LINS_SCB_RX_WAKE_ISR_PRIORITY);
            (void) CyIntSetVector(LINS_SCB_RX_WAKE_ISR_NUMBER, &LINS_SCB_UART_WAKEUP_ISR);
        #endif /* (LINS_SCB_UART_RX_WAKEUP_IRQ) */

            /* Configure interrupt sources */
            LINS_SCB_INTR_I2C_EC_MASK_REG = LINS_SCB_NO_INTR_SOURCES;
            LINS_SCB_INTR_SPI_EC_MASK_REG = LINS_SCB_NO_INTR_SOURCES;
            LINS_SCB_INTR_SLAVE_MASK_REG  = LINS_SCB_NO_INTR_SOURCES;
            LINS_SCB_INTR_MASTER_MASK_REG = LINS_SCB_NO_INTR_SOURCES;
            LINS_SCB_INTR_RX_MASK_REG     = config->rxInterruptMask;
            LINS_SCB_INTR_TX_MASK_REG     = config->txInterruptMask;

            /* Configure TX interrupt sources to restore. */
            LINS_SCB_IntrTxMask = LO16(LINS_SCB_INTR_TX_MASK_REG);

            /* Clear RX buffer indexes */
            LINS_SCB_rxBufferHead     = 0u;
            LINS_SCB_rxBufferTail     = 0u;
            LINS_SCB_rxBufferOverflow = 0u;

            /* Clear TX buffer indexes */
            LINS_SCB_txBufferHead = 0u;
            LINS_SCB_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: LINS_SCB_UartInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the UART operation.
    *
    *******************************************************************************/
    void LINS_SCB_UartInit(void)
    {
        /* Configure UART interface */
        LINS_SCB_CTRL_REG = LINS_SCB_UART_DEFAULT_CTRL;

        /* Configure sub-mode: UART, SmartCard or IrDA */
        LINS_SCB_UART_CTRL_REG = LINS_SCB_UART_DEFAULT_UART_CTRL;

        /* Configure RX direction */
        LINS_SCB_UART_RX_CTRL_REG = LINS_SCB_UART_DEFAULT_UART_RX_CTRL;
        LINS_SCB_RX_CTRL_REG      = LINS_SCB_UART_DEFAULT_RX_CTRL;
        LINS_SCB_RX_FIFO_CTRL_REG = LINS_SCB_UART_DEFAULT_RX_FIFO_CTRL;
        LINS_SCB_RX_MATCH_REG     = LINS_SCB_UART_DEFAULT_RX_MATCH_REG;

        /* Configure TX direction */
        LINS_SCB_UART_TX_CTRL_REG = LINS_SCB_UART_DEFAULT_UART_TX_CTRL;
        LINS_SCB_TX_CTRL_REG      = LINS_SCB_UART_DEFAULT_TX_CTRL;
        LINS_SCB_TX_FIFO_CTRL_REG = LINS_SCB_UART_DEFAULT_TX_FIFO_CTRL;

    #if !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1)
        LINS_SCB_UART_FLOW_CTRL_REG = LINS_SCB_UART_DEFAULT_FLOW_CTRL;
    #endif /* !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1) */

        /* Configure interrupt with UART handler but do not enable it */
    #if(LINS_SCB_SCB_IRQ_INTERNAL)
        CyIntDisable    (LINS_SCB_ISR_NUMBER);
        CyIntSetPriority(LINS_SCB_ISR_NUMBER, LINS_SCB_ISR_PRIORITY);
        (void) CyIntSetVector(LINS_SCB_ISR_NUMBER, &LINS_SCB_SPI_UART_ISR);
    #endif /* (LINS_SCB_SCB_IRQ_INTERNAL) */

        /* Configure WAKE interrupt */
    #if(LINS_SCB_UART_RX_WAKEUP_IRQ)
        CyIntDisable    (LINS_SCB_RX_WAKE_ISR_NUMBER);
        CyIntSetPriority(LINS_SCB_RX_WAKE_ISR_NUMBER, LINS_SCB_RX_WAKE_ISR_PRIORITY);
        (void) CyIntSetVector(LINS_SCB_RX_WAKE_ISR_NUMBER, &LINS_SCB_UART_WAKEUP_ISR);
    #endif /* (LINS_SCB_UART_RX_WAKEUP_IRQ) */

        /* Configure interrupt sources */
        LINS_SCB_INTR_I2C_EC_MASK_REG = LINS_SCB_UART_DEFAULT_INTR_I2C_EC_MASK;
        LINS_SCB_INTR_SPI_EC_MASK_REG = LINS_SCB_UART_DEFAULT_INTR_SPI_EC_MASK;
        LINS_SCB_INTR_SLAVE_MASK_REG  = LINS_SCB_UART_DEFAULT_INTR_SLAVE_MASK;
        LINS_SCB_INTR_MASTER_MASK_REG = LINS_SCB_UART_DEFAULT_INTR_MASTER_MASK;
        LINS_SCB_INTR_RX_MASK_REG     = LINS_SCB_UART_DEFAULT_INTR_RX_MASK;
        LINS_SCB_INTR_TX_MASK_REG     = LINS_SCB_UART_DEFAULT_INTR_TX_MASK;

        /* Configure TX interrupt sources to restore. */
        LINS_SCB_IntrTxMask = LO16(LINS_SCB_INTR_TX_MASK_REG);

    #if(LINS_SCB_INTERNAL_RX_SW_BUFFER_CONST)
        LINS_SCB_rxBufferHead     = 0u;
        LINS_SCB_rxBufferTail     = 0u;
        LINS_SCB_rxBufferOverflow = 0u;
    #endif /* (LINS_SCB_INTERNAL_RX_SW_BUFFER_CONST) */

    #if(LINS_SCB_INTERNAL_TX_SW_BUFFER_CONST)
        LINS_SCB_txBufferHead = 0u;
        LINS_SCB_txBufferTail = 0u;
    #endif /* (LINS_SCB_INTERNAL_TX_SW_BUFFER_CONST) */
    }
#endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: LINS_SCB_UartPostEnable
****************************************************************************//**
*
*  Restores HSIOM settings for the UART output pins (TX and/or RTS) to be
*  controlled by the SCB UART.
*
*******************************************************************************/
void LINS_SCB_UartPostEnable(void)
{
#if (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (LINS_SCB_TX_SDA_MISO_PIN)
        if (LINS_SCB_CHECK_TX_SDA_MISO_PIN_USED)
        {
            /* Set SCB UART to drive the output pin */
            LINS_SCB_SET_HSIOM_SEL(LINS_SCB_TX_SDA_MISO_HSIOM_REG, LINS_SCB_TX_SDA_MISO_HSIOM_MASK,
                                           LINS_SCB_TX_SDA_MISO_HSIOM_POS, LINS_SCB_TX_SDA_MISO_HSIOM_SEL_UART);
        }
    #endif /* (LINS_SCB_TX_SDA_MISO_PIN_PIN) */

    #if !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1)
        #if (LINS_SCB_SS0_PIN)
            if (LINS_SCB_CHECK_SS0_PIN_USED)
            {
                /* Set SCB UART to drive the output pin */
                LINS_SCB_SET_HSIOM_SEL(LINS_SCB_SS0_HSIOM_REG, LINS_SCB_SS0_HSIOM_MASK,
                                               LINS_SCB_SS0_HSIOM_POS, LINS_SCB_SS0_HSIOM_SEL_UART);
            }
        #endif /* (LINS_SCB_SS0_PIN) */
    #endif /* !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1) */

#else
    #if (LINS_SCB_UART_TX_PIN)
         /* Set SCB UART to drive the output pin */
        LINS_SCB_SET_HSIOM_SEL(LINS_SCB_TX_HSIOM_REG, LINS_SCB_TX_HSIOM_MASK,
                                       LINS_SCB_TX_HSIOM_POS, LINS_SCB_TX_HSIOM_SEL_UART);
    #endif /* (LINS_SCB_UART_TX_PIN) */

    #if (LINS_SCB_UART_RTS_PIN)
        /* Set SCB UART to drive the output pin */
        LINS_SCB_SET_HSIOM_SEL(LINS_SCB_RTS_HSIOM_REG, LINS_SCB_RTS_HSIOM_MASK,
                                       LINS_SCB_RTS_HSIOM_POS, LINS_SCB_RTS_HSIOM_SEL_UART);
    #endif /* (LINS_SCB_UART_RTS_PIN) */
#endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Restore TX interrupt sources. */
    LINS_SCB_SetTxInterruptMode(LINS_SCB_IntrTxMask);
}


/*******************************************************************************
* Function Name: LINS_SCB_UartStop
****************************************************************************//**
*
*  Changes the HSIOM settings for the UART output pins (TX and/or RTS) to keep
*  them inactive after the block is disabled. The output pins are controlled by
*  the GPIO data register. Also, the function disables the skip start feature
*  to not cause it to trigger after the component is enabled.
*
*******************************************************************************/
void LINS_SCB_UartStop(void)
{
#if(LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (LINS_SCB_TX_SDA_MISO_PIN)
        if (LINS_SCB_CHECK_TX_SDA_MISO_PIN_USED)
        {
            /* Set GPIO to drive output pin */
            LINS_SCB_SET_HSIOM_SEL(LINS_SCB_TX_SDA_MISO_HSIOM_REG, LINS_SCB_TX_SDA_MISO_HSIOM_MASK,
                                           LINS_SCB_TX_SDA_MISO_HSIOM_POS, LINS_SCB_TX_SDA_MISO_HSIOM_SEL_GPIO);
        }
    #endif /* (LINS_SCB_TX_SDA_MISO_PIN_PIN) */

    #if !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1)
        #if (LINS_SCB_SS0_PIN)
            if (LINS_SCB_CHECK_SS0_PIN_USED)
            {
                /* Set output pin state after block is disabled */
                LINS_SCB_spi_ss0_Write(LINS_SCB_GET_UART_RTS_INACTIVE);

                /* Set GPIO to drive output pin */
                LINS_SCB_SET_HSIOM_SEL(LINS_SCB_SS0_HSIOM_REG, LINS_SCB_SS0_HSIOM_MASK,
                                               LINS_SCB_SS0_HSIOM_POS, LINS_SCB_SS0_HSIOM_SEL_GPIO);
            }
        #endif /* (LINS_SCB_SS0_PIN) */
    #endif /* !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1) */

#else
    #if (LINS_SCB_UART_TX_PIN)
        /* Set GPIO to drive output pin */
        LINS_SCB_SET_HSIOM_SEL(LINS_SCB_TX_HSIOM_REG, LINS_SCB_TX_HSIOM_MASK,
                                       LINS_SCB_TX_HSIOM_POS, LINS_SCB_TX_HSIOM_SEL_GPIO);
    #endif /* (LINS_SCB_UART_TX_PIN) */

    #if (LINS_SCB_UART_RTS_PIN)
        /* Set output pin state after block is disabled */
        LINS_SCB_rts_Write(LINS_SCB_GET_UART_RTS_INACTIVE);

        /* Set GPIO to drive output pin */
        LINS_SCB_SET_HSIOM_SEL(LINS_SCB_RTS_HSIOM_REG, LINS_SCB_RTS_HSIOM_MASK,
                                       LINS_SCB_RTS_HSIOM_POS, LINS_SCB_RTS_HSIOM_SEL_GPIO);
    #endif /* (LINS_SCB_UART_RTS_PIN) */

#endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (LINS_SCB_UART_WAKE_ENABLE_CONST)
    /* Disable skip start feature used for wakeup */
    LINS_SCB_UART_RX_CTRL_REG &= (uint32) ~LINS_SCB_UART_RX_CTRL_SKIP_START;
#endif /* (LINS_SCB_UART_WAKE_ENABLE_CONST) */

    /* Store TX interrupt sources (exclude level triggered). */
    LINS_SCB_IntrTxMask = LO16(LINS_SCB_GetTxInterruptMode() & LINS_SCB_INTR_UART_TX_RESTORE);
}


/*******************************************************************************
* Function Name: LINS_SCB_UartSetRxAddress
****************************************************************************//**
*
*  Sets the hardware detectable receiver address for the UART in the
*  Multiprocessor mode.
*
*  \param address: Address for hardware address detection.
*
*******************************************************************************/
void LINS_SCB_UartSetRxAddress(uint32 address)
{
     uint32 matchReg;

    matchReg = LINS_SCB_RX_MATCH_REG;

    matchReg &= ((uint32) ~LINS_SCB_RX_MATCH_ADDR_MASK); /* Clear address bits */
    matchReg |= ((uint32)  (address & LINS_SCB_RX_MATCH_ADDR_MASK)); /* Set address  */

    LINS_SCB_RX_MATCH_REG = matchReg;
}


/*******************************************************************************
* Function Name: LINS_SCB_UartSetRxAddressMask
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
void LINS_SCB_UartSetRxAddressMask(uint32 addressMask)
{
    uint32 matchReg;

    matchReg = LINS_SCB_RX_MATCH_REG;

    matchReg &= ((uint32) ~LINS_SCB_RX_MATCH_MASK_MASK); /* Clear address mask bits */
    matchReg |= ((uint32) (addressMask << LINS_SCB_RX_MATCH_MASK_POS));

    LINS_SCB_RX_MATCH_REG = matchReg;
}


#if(LINS_SCB_UART_RX_DIRECTION)
    /*******************************************************************************
    * Function Name: LINS_SCB_UartGetChar
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
    *   Check LINS_SCB_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 LINS_SCB_UartGetChar(void)
    {
        uint32 rxData = 0u;

        /* Reads data only if there is data to read */
        if (0u != LINS_SCB_SpiUartGetRxBufferSize())
        {
            rxData = LINS_SCB_SpiUartReadRxData();
        }

        if (LINS_SCB_CHECK_INTR_RX(LINS_SCB_INTR_RX_ERR))
        {
            rxData = 0u; /* Error occurred: returns zero */
            LINS_SCB_ClearRxInterruptSource(LINS_SCB_INTR_RX_ERR);
        }

        return (rxData);
    }


    /*******************************************************************************
    * Function Name: LINS_SCB_UartGetByte
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
    *   - LINS_SCB_UART_RX_OVERFLOW - Attempt to write to a full
    *     receiver FIFO.
    *   - LINS_SCB_UART_RX_UNDERFLOW    Attempt to read from an empty
    *     receiver FIFO.
    *   - LINS_SCB_UART_RX_FRAME_ERROR - UART framing error detected.
    *   - LINS_SCB_UART_RX_PARITY_ERROR - UART parity error detected.
    *
    *  \sideeffect
    *   The errors bits may not correspond with reading characters due to
    *   RX FIFO and software buffer usage.
    *   RX software buffer is enabled: The internal software buffer overflow
    *   is not treated as an error condition.
    *   Check LINS_SCB_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 LINS_SCB_UartGetByte(void)
    {
        uint32 rxData;
        uint32 tmpStatus;

        #if (LINS_SCB_CHECK_RX_SW_BUFFER)
        {
            LINS_SCB_DisableInt();
        }
        #endif

        if (0u != LINS_SCB_SpiUartGetRxBufferSize())
        {
            /* Enables interrupt to receive more bytes: at least one byte is in
            * buffer.
            */
            #if (LINS_SCB_CHECK_RX_SW_BUFFER)
            {
                LINS_SCB_EnableInt();
            }
            #endif

            /* Get received byte */
            rxData = LINS_SCB_SpiUartReadRxData();
        }
        else
        {
            /* Reads a byte directly from RX FIFO: underflow is raised in the
            * case of empty. Otherwise the first received byte will be read.
            */
            rxData = LINS_SCB_RX_FIFO_RD_REG;


            /* Enables interrupt to receive more bytes. */
            #if (LINS_SCB_CHECK_RX_SW_BUFFER)
            {

                /* The byte has been read from RX FIFO. Clear RX interrupt to
                * not involve interrupt handler when RX FIFO is empty.
                */
                LINS_SCB_ClearRxInterruptSource(LINS_SCB_INTR_RX_NOT_EMPTY);

                LINS_SCB_EnableInt();
            }
            #endif
        }

        /* Get and clear RX error mask */
        tmpStatus = (LINS_SCB_GetRxInterruptSource() & LINS_SCB_INTR_RX_ERR);
        LINS_SCB_ClearRxInterruptSource(LINS_SCB_INTR_RX_ERR);

        /* Puts together data and error status:
        * MP mode and accept address: 9th bit is set to notify mark.
        */
        rxData |= ((uint32) (tmpStatus << 8u));

        return (rxData);
    }


    #if !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: LINS_SCB_UartSetRtsPolarity
        ****************************************************************************//**
        *
        *  Sets active polarity of RTS output signal.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *  \param polarity: Active polarity of RTS output signal.
        *   - LINS_SCB_UART_RTS_ACTIVE_LOW  - RTS signal is active low.
        *   - LINS_SCB_UART_RTS_ACTIVE_HIGH - RTS signal is active high.
        *
        *******************************************************************************/
        void LINS_SCB_UartSetRtsPolarity(uint32 polarity)
        {
            if(0u != polarity)
            {
                LINS_SCB_UART_FLOW_CTRL_REG |= (uint32)  LINS_SCB_UART_FLOW_CTRL_RTS_POLARITY;
            }
            else
            {
                LINS_SCB_UART_FLOW_CTRL_REG &= (uint32) ~LINS_SCB_UART_FLOW_CTRL_RTS_POLARITY;
            }
        }


        /*******************************************************************************
        * Function Name: LINS_SCB_UartSetRtsFifoLevel
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
        void LINS_SCB_UartSetRtsFifoLevel(uint32 level)
        {
            uint32 uartFlowCtrl;

            uartFlowCtrl = LINS_SCB_UART_FLOW_CTRL_REG;

            uartFlowCtrl &= ((uint32) ~LINS_SCB_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
            uartFlowCtrl |= ((uint32) (LINS_SCB_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK & level));

            LINS_SCB_UART_FLOW_CTRL_REG = uartFlowCtrl;
        }
    #endif /* !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1) */

#endif /* (LINS_SCB_UART_RX_DIRECTION) */


#if(LINS_SCB_UART_TX_DIRECTION)
    /*******************************************************************************
    * Function Name: LINS_SCB_UartPutString
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
    void LINS_SCB_UartPutString(const char8 string[])
    {
        uint32 bufIndex;

        bufIndex = 0u;

        /* Blocks the control flow until all data has been sent */
        while(string[bufIndex] != ((char8) 0))
        {
            LINS_SCB_UartPutChar((uint32) string[bufIndex]);
            bufIndex++;
        }
    }


    /*******************************************************************************
    * Function Name: LINS_SCB_UartPutCRLF
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
    void LINS_SCB_UartPutCRLF(uint32 txDataByte)
    {
        LINS_SCB_UartPutChar(txDataByte);  /* Blocks control flow until all data has been sent */
        LINS_SCB_UartPutChar(0x0Du);       /* Blocks control flow until all data has been sent */
        LINS_SCB_UartPutChar(0x0Au);       /* Blocks control flow until all data has been sent */
    }


    #if !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: LINS_SCBSCB_UartEnableCts
        ****************************************************************************//**
        *
        *  Enables usage of CTS input signal by the UART transmitter.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *******************************************************************************/
        void LINS_SCB_UartEnableCts(void)
        {
            LINS_SCB_UART_FLOW_CTRL_REG |= (uint32)  LINS_SCB_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: LINS_SCB_UartDisableCts
        ****************************************************************************//**
        *
        *  Disables usage of CTS input signal by the UART transmitter.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *******************************************************************************/
        void LINS_SCB_UartDisableCts(void)
        {
            LINS_SCB_UART_FLOW_CTRL_REG &= (uint32) ~LINS_SCB_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: LINS_SCB_UartSetCtsPolarity
        ****************************************************************************//**
        *
        *  Sets active polarity of CTS input signal.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        * \param
        * polarity: Active polarity of CTS output signal.
        *   - LINS_SCB_UART_CTS_ACTIVE_LOW  - CTS signal is active low.
        *   - LINS_SCB_UART_CTS_ACTIVE_HIGH - CTS signal is active high.
        *
        *******************************************************************************/
        void LINS_SCB_UartSetCtsPolarity(uint32 polarity)
        {
            if (0u != polarity)
            {
                LINS_SCB_UART_FLOW_CTRL_REG |= (uint32)  LINS_SCB_UART_FLOW_CTRL_CTS_POLARITY;
            }
            else
            {
                LINS_SCB_UART_FLOW_CTRL_REG &= (uint32) ~LINS_SCB_UART_FLOW_CTRL_CTS_POLARITY;
            }
        }
    #endif /* !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1) */


    /*******************************************************************************
    * Function Name: LINS_SCB_UartSendBreakBlocking
    ****************************************************************************//**
    *
    * Sends a break condition (logic low) of specified width on UART TX line.
    * Blocks until break is completed. Only call this function when UART TX FIFO
    * and shifter are empty.
    *
    * \param breakWidth
    * Width of break condition. Valid range is 4 to 16 bits.
    *
    * \note
    * Before sending break all UART TX interrupt sources are disabled. The state
    * of UART TX interrupt sources is restored before function returns.
    *
    * \sideeffect
    * If this function is called while there is data in the TX FIFO or shifter that
    * data will be shifted out in packets the size of breakWidth.
    *
    *******************************************************************************/
    void LINS_SCB_UartSendBreakBlocking(uint32 breakWidth)
    {
        uint32 txCtrlReg;
        uint32 txIntrReg;

        /* Disable all UART TX interrupt source and clear UART TX Done history */
        txIntrReg = LINS_SCB_GetTxInterruptMode();
        LINS_SCB_SetTxInterruptMode(0u);
        LINS_SCB_ClearTxInterruptSource(LINS_SCB_INTR_TX_UART_DONE);

        /* Store TX CTRL configuration */
        txCtrlReg = LINS_SCB_TX_CTRL_REG;

        /* Set break width */
        LINS_SCB_TX_CTRL_REG = (LINS_SCB_TX_CTRL_REG & (uint32) ~LINS_SCB_TX_CTRL_DATA_WIDTH_MASK) |
                                        LINS_SCB_GET_TX_CTRL_DATA_WIDTH(breakWidth);

        /* Generate break */
        LINS_SCB_TX_FIFO_WR_REG = 0u;

        /* Wait for break completion */
        while (0u == (LINS_SCB_GetTxInterruptSource() & LINS_SCB_INTR_TX_UART_DONE))
        {
        }

        /* Clear all UART TX interrupt sources to  */
        LINS_SCB_ClearTxInterruptSource(LINS_SCB_INTR_TX_ALL);

        /* Restore TX interrupt sources and data width */
        LINS_SCB_TX_CTRL_REG = txCtrlReg;
        LINS_SCB_SetTxInterruptMode(txIntrReg);
    }
#endif /* (LINS_SCB_UART_TX_DIRECTION) */


#if (LINS_SCB_UART_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: LINS_SCB_UartSaveConfig
    ****************************************************************************//**
    *
    *  Clears and enables an interrupt on a falling edge of the Rx input. The GPIO
    *  interrupt does not track in the active mode, therefore requires to be
    *  cleared by this API.
    *
    *******************************************************************************/
    void LINS_SCB_UartSaveConfig(void)
    {
    #if (LINS_SCB_UART_RX_WAKEUP_IRQ)
        /* Set SKIP_START if requested (set by default). */
        if (0u != LINS_SCB_skipStart)
        {
            LINS_SCB_UART_RX_CTRL_REG |= (uint32)  LINS_SCB_UART_RX_CTRL_SKIP_START;
        }
        else
        {
            LINS_SCB_UART_RX_CTRL_REG &= (uint32) ~LINS_SCB_UART_RX_CTRL_SKIP_START;
        }

        /* Clear RX GPIO interrupt status and pending interrupt in NVIC because
        * falling edge on RX line occurs while UART communication in active mode.
        * Enable interrupt: next interrupt trigger should wakeup device.
        */
        LINS_SCB_CLEAR_UART_RX_WAKE_INTR;
        LINS_SCB_RxWakeClearPendingInt();
        LINS_SCB_RxWakeEnableInt();
    #endif /* (LINS_SCB_UART_RX_WAKEUP_IRQ) */
    }


    /*******************************************************************************
    * Function Name: LINS_SCB_UartRestoreConfig
    ****************************************************************************//**
    *
    *  Disables the RX GPIO interrupt. Until this function is called the interrupt
    *  remains active and triggers on every falling edge of the UART RX line.
    *
    *******************************************************************************/
    void LINS_SCB_UartRestoreConfig(void)
    {
    #if (LINS_SCB_UART_RX_WAKEUP_IRQ)
        /* Disable interrupt: no more triggers in active mode */
        LINS_SCB_RxWakeDisableInt();
    #endif /* (LINS_SCB_UART_RX_WAKEUP_IRQ) */
    }


    #if (LINS_SCB_UART_RX_WAKEUP_IRQ)
        /*******************************************************************************
        * Function Name: LINS_SCB_UART_WAKEUP_ISR
        ****************************************************************************//**
        *
        *  Handles the Interrupt Service Routine for the SCB UART mode GPIO wakeup
        *  event. This event is configured to trigger on a falling edge of the RX line.
        *
        *******************************************************************************/
        CY_ISR(LINS_SCB_UART_WAKEUP_ISR)
        {
        #ifdef LINS_SCB_UART_WAKEUP_ISR_ENTRY_CALLBACK
            LINS_SCB_UART_WAKEUP_ISR_EntryCallback();
        #endif /* LINS_SCB_UART_WAKEUP_ISR_ENTRY_CALLBACK */

            LINS_SCB_CLEAR_UART_RX_WAKE_INTR;

        #ifdef LINS_SCB_UART_WAKEUP_ISR_EXIT_CALLBACK
            LINS_SCB_UART_WAKEUP_ISR_ExitCallback();
        #endif /* LINS_SCB_UART_WAKEUP_ISR_EXIT_CALLBACK */
        }
    #endif /* (LINS_SCB_UART_RX_WAKEUP_IRQ) */
#endif /* (LINS_SCB_UART_RX_WAKEUP_IRQ) */


/* [] END OF FILE */
