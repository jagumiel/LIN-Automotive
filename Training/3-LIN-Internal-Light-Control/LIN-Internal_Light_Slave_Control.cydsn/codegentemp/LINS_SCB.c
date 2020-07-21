/***************************************************************************//**
* \file LINS_SCB.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component.
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

#if (LINS_SCB_SCB_MODE_I2C_INC)
    #include "LINS_SCB_I2C_PVT.h"
#endif /* (LINS_SCB_SCB_MODE_I2C_INC) */

#if (LINS_SCB_SCB_MODE_EZI2C_INC)
    #include "LINS_SCB_EZI2C_PVT.h"
#endif /* (LINS_SCB_SCB_MODE_EZI2C_INC) */

#if (LINS_SCB_SCB_MODE_SPI_INC || LINS_SCB_SCB_MODE_UART_INC)
    #include "LINS_SCB_SPI_UART_PVT.h"
#endif /* (LINS_SCB_SCB_MODE_SPI_INC || LINS_SCB_SCB_MODE_UART_INC) */


/***************************************
*    Run Time Configuration Vars
***************************************/

/* Stores internal component configuration for Unconfigured mode */
#if (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    uint8 LINS_SCB_scbMode = LINS_SCB_SCB_MODE_UNCONFIG;
    uint8 LINS_SCB_scbEnableWake;
    uint8 LINS_SCB_scbEnableIntr;

    /* I2C configuration variables */
    uint8 LINS_SCB_mode;
    uint8 LINS_SCB_acceptAddr;

    /* SPI/UART configuration variables */
    volatile uint8 * LINS_SCB_rxBuffer;
    uint8  LINS_SCB_rxDataBits;
    uint32 LINS_SCB_rxBufferSize;

    volatile uint8 * LINS_SCB_txBuffer;
    uint8  LINS_SCB_txDataBits;
    uint32 LINS_SCB_txBufferSize;

    /* EZI2C configuration variables */
    uint8 LINS_SCB_numberOfAddr;
    uint8 LINS_SCB_subAddrSize;
#endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Common SCB Vars
***************************************/
/**
* \addtogroup group_general
* \{
*/

/** LINS_SCB_initVar indicates whether the LINS_SCB 
*  component has been initialized. The variable is initialized to 0 
*  and set to 1 the first time SCB_Start() is called. This allows 
*  the component to restart without reinitialization after the first 
*  call to the LINS_SCB_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  LINS_SCB_Init() function can be called before the 
*  LINS_SCB_Start() or LINS_SCB_Enable() function.
*/
uint8 LINS_SCB_initVar = 0u;


#if (! (LINS_SCB_SCB_MODE_I2C_CONST_CFG || \
        LINS_SCB_SCB_MODE_EZI2C_CONST_CFG))
    /** This global variable stores TX interrupt sources after 
    * LINS_SCB_Stop() is called. Only these TX interrupt sources 
    * will be restored on a subsequent LINS_SCB_Enable() call.
    */
    uint16 LINS_SCB_IntrTxMask = 0u;
#endif /* (! (LINS_SCB_SCB_MODE_I2C_CONST_CFG || \
              LINS_SCB_SCB_MODE_EZI2C_CONST_CFG)) */
/** \} globals */

#if (LINS_SCB_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_LINS_SCB_CUSTOM_INTR_HANDLER)
    void (*LINS_SCB_customIntrHandler)(void) = NULL;
#endif /* !defined (CY_REMOVE_LINS_SCB_CUSTOM_INTR_HANDLER) */
#endif /* (LINS_SCB_SCB_IRQ_INTERNAL) */


/***************************************
*    Private Function Prototypes
***************************************/

static void LINS_SCB_ScbEnableIntr(void);
static void LINS_SCB_ScbModeStop(void);
static void LINS_SCB_ScbModePostEnable(void);


/*******************************************************************************
* Function Name: LINS_SCB_Init
****************************************************************************//**
*
*  Initializes the LINS_SCB component to operate in one of the selected
*  configurations: I2C, SPI, UART or EZI2C.
*  When the configuration is set to "Unconfigured SCB", this function does
*  not do any initialization. Use mode-specific initialization APIs instead:
*  LINS_SCB_I2CInit, LINS_SCB_SpiInit, 
*  LINS_SCB_UartInit or LINS_SCB_EzI2CInit.
*
*******************************************************************************/
void LINS_SCB_Init(void)
{
#if (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    if (LINS_SCB_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        LINS_SCB_initVar = 0u;
    }
    else
    {
        /* Initialization was done before this function call */
    }

#elif (LINS_SCB_SCB_MODE_I2C_CONST_CFG)
    LINS_SCB_I2CInit();

#elif (LINS_SCB_SCB_MODE_SPI_CONST_CFG)
    LINS_SCB_SpiInit();

#elif (LINS_SCB_SCB_MODE_UART_CONST_CFG)
    LINS_SCB_UartInit();

#elif (LINS_SCB_SCB_MODE_EZI2C_CONST_CFG)
    LINS_SCB_EzI2CInit();

#endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: LINS_SCB_Enable
****************************************************************************//**
*
*  Enables LINS_SCB component operation: activates the hardware and 
*  internal interrupt. It also restores TX interrupt sources disabled after the 
*  LINS_SCB_Stop() function was called (note that level-triggered TX 
*  interrupt sources remain disabled to not cause code lock-up).
*  For I2C and EZI2C modes the interrupt is internal and mandatory for 
*  operation. For SPI and UART modes the interrupt can be configured as none, 
*  internal or external.
*  The LINS_SCB configuration should be not changed when the component
*  is enabled. Any configuration changes should be made after disabling the 
*  component.
*  When configuration is set to “Unconfigured LINS_SCB”, the component 
*  must first be initialized to operate in one of the following configurations: 
*  I2C, SPI, UART or EZ I2C, using the mode-specific initialization API. 
*  Otherwise this function does not enable the component.
*
*******************************************************************************/
void LINS_SCB_Enable(void)
{
#if (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Enable SCB block, only if it is already configured */
    if (!LINS_SCB_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        LINS_SCB_CTRL_REG |= LINS_SCB_CTRL_ENABLED;

        LINS_SCB_ScbEnableIntr();

        /* Call PostEnable function specific to current operation mode */
        LINS_SCB_ScbModePostEnable();
    }
#else
    LINS_SCB_CTRL_REG |= LINS_SCB_CTRL_ENABLED;

    LINS_SCB_ScbEnableIntr();

    /* Call PostEnable function specific to current operation mode */
    LINS_SCB_ScbModePostEnable();
#endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: LINS_SCB_Start
****************************************************************************//**
*
*  Invokes LINS_SCB_Init() and LINS_SCB_Enable().
*  After this function call, the component is enabled and ready for operation.
*  When configuration is set to "Unconfigured SCB", the component must first be
*  initialized to operate in one of the following configurations: I2C, SPI, UART
*  or EZI2C. Otherwise this function does not enable the component.
*
* \globalvars
*  LINS_SCB_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void LINS_SCB_Start(void)
{
    if (0u == LINS_SCB_initVar)
    {
        LINS_SCB_Init();
        LINS_SCB_initVar = 1u; /* Component was initialized */
    }

    LINS_SCB_Enable();
}


/*******************************************************************************
* Function Name: LINS_SCB_Stop
****************************************************************************//**
*
*  Disables the LINS_SCB component: disable the hardware and internal 
*  interrupt. It also disables all TX interrupt sources so as not to cause an 
*  unexpected interrupt trigger because after the component is enabled, the 
*  TX FIFO is empty.
*  Refer to the function LINS_SCB_Enable() for the interrupt 
*  configuration details.
*  This function disables the SCB component without checking to see if 
*  communication is in progress. Before calling this function it may be 
*  necessary to check the status of communication to make sure communication 
*  is complete. If this is not done then communication could be stopped mid 
*  byte and corrupted data could result.
*
*******************************************************************************/
void LINS_SCB_Stop(void)
{
#if (LINS_SCB_SCB_IRQ_INTERNAL)
    LINS_SCB_DisableInt();
#endif /* (LINS_SCB_SCB_IRQ_INTERNAL) */

    /* Call Stop function specific to current operation mode */
    LINS_SCB_ScbModeStop();

    /* Disable SCB IP */
    LINS_SCB_CTRL_REG &= (uint32) ~LINS_SCB_CTRL_ENABLED;

    /* Disable all TX interrupt sources so as not to cause an unexpected
    * interrupt trigger after the component will be enabled because the 
    * TX FIFO is empty.
    * For SCB IP v0, it is critical as it does not mask-out interrupt
    * sources when it is disabled. This can cause a code lock-up in the
    * interrupt handler because TX FIFO cannot be loaded after the block
    * is disabled.
    */
    LINS_SCB_SetTxInterruptMode(LINS_SCB_NO_INTR_SOURCES);

#if (LINS_SCB_SCB_IRQ_INTERNAL)
    LINS_SCB_ClearPendingInt();
#endif /* (LINS_SCB_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: LINS_SCB_SetRxFifoLevel
****************************************************************************//**
*
*  Sets level in the RX FIFO to generate a RX level interrupt.
*  When the RX FIFO has more entries than the RX FIFO level an RX level
*  interrupt request is generated.
*
*  \param level: Level in the RX FIFO to generate RX level interrupt.
*   The range of valid level values is between 0 and RX FIFO depth - 1.
*
*******************************************************************************/
void LINS_SCB_SetRxFifoLevel(uint32 level)
{
    uint32 rxFifoCtrl;

    rxFifoCtrl = LINS_SCB_RX_FIFO_CTRL_REG;

    rxFifoCtrl &= ((uint32) ~LINS_SCB_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    rxFifoCtrl |= ((uint32) (LINS_SCB_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    LINS_SCB_RX_FIFO_CTRL_REG = rxFifoCtrl;
}


/*******************************************************************************
* Function Name: LINS_SCB_SetTxFifoLevel
****************************************************************************//**
*
*  Sets level in the TX FIFO to generate a TX level interrupt.
*  When the TX FIFO has less entries than the TX FIFO level an TX level
*  interrupt request is generated.
*
*  \param level: Level in the TX FIFO to generate TX level interrupt.
*   The range of valid level values is between 0 and TX FIFO depth - 1.
*
*******************************************************************************/
void LINS_SCB_SetTxFifoLevel(uint32 level)
{
    uint32 txFifoCtrl;

    txFifoCtrl = LINS_SCB_TX_FIFO_CTRL_REG;

    txFifoCtrl &= ((uint32) ~LINS_SCB_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    txFifoCtrl |= ((uint32) (LINS_SCB_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    LINS_SCB_TX_FIFO_CTRL_REG = txFifoCtrl;
}


#if (LINS_SCB_SCB_IRQ_INTERNAL)
    /*******************************************************************************
    * Function Name: LINS_SCB_SetCustomInterruptHandler
    ****************************************************************************//**
    *
    *  Registers a function to be called by the internal interrupt handler.
    *  First the function that is registered is called, then the internal interrupt
    *  handler performs any operation such as software buffer management functions
    *  before the interrupt returns.  It is the user's responsibility not to break
    *  the software buffer operations. Only one custom handler is supported, which
    *  is the function provided by the most recent call.
    *  At the initialization time no custom handler is registered.
    *
    *  \param func: Pointer to the function to register.
    *        The value NULL indicates to remove the current custom interrupt
    *        handler.
    *
    *******************************************************************************/
    void LINS_SCB_SetCustomInterruptHandler(void (*func)(void))
    {
    #if !defined (CY_REMOVE_LINS_SCB_CUSTOM_INTR_HANDLER)
        LINS_SCB_customIntrHandler = func; /* Register interrupt handler */
    #else
        if (NULL != func)
        {
            /* Suppress compiler warning */
        }
    #endif /* !defined (CY_REMOVE_LINS_SCB_CUSTOM_INTR_HANDLER) */
    }
#endif /* (LINS_SCB_SCB_IRQ_INTERNAL) */


/*******************************************************************************
* Function Name: LINS_SCB_ScbModeEnableIntr
****************************************************************************//**
*
*  Enables an interrupt for a specific mode.
*
*******************************************************************************/
static void LINS_SCB_ScbEnableIntr(void)
{
#if (LINS_SCB_SCB_IRQ_INTERNAL)
    /* Enable interrupt in NVIC */
    #if (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
        if (0u != LINS_SCB_scbEnableIntr)
        {
            LINS_SCB_EnableInt();
        }

    #else
        LINS_SCB_EnableInt();

    #endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (LINS_SCB_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: LINS_SCB_ScbModePostEnable
****************************************************************************//**
*
*  Calls the PostEnable function for a specific operation mode.
*
*******************************************************************************/
static void LINS_SCB_ScbModePostEnable(void)
{
#if (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
#if (!LINS_SCB_CY_SCBIP_V1)
    if (LINS_SCB_SCB_MODE_SPI_RUNTM_CFG)
    {
        LINS_SCB_SpiPostEnable();
    }
    else if (LINS_SCB_SCB_MODE_UART_RUNTM_CFG)
    {
        LINS_SCB_UartPostEnable();
    }
    else
    {
        /* Unknown mode: do nothing */
    }
#endif /* (!LINS_SCB_CY_SCBIP_V1) */

#elif (LINS_SCB_SCB_MODE_SPI_CONST_CFG)
    LINS_SCB_SpiPostEnable();

#elif (LINS_SCB_SCB_MODE_UART_CONST_CFG)
    LINS_SCB_UartPostEnable();

#else
    /* Unknown mode: do nothing */
#endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: LINS_SCB_ScbModeStop
****************************************************************************//**
*
*  Calls the Stop function for a specific operation mode.
*
*******************************************************************************/
static void LINS_SCB_ScbModeStop(void)
{
#if (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    if (LINS_SCB_SCB_MODE_I2C_RUNTM_CFG)
    {
        LINS_SCB_I2CStop();
    }
    else if (LINS_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        LINS_SCB_EzI2CStop();
    }
#if (!LINS_SCB_CY_SCBIP_V1)
    else if (LINS_SCB_SCB_MODE_SPI_RUNTM_CFG)
    {
        LINS_SCB_SpiStop();
    }
    else if (LINS_SCB_SCB_MODE_UART_RUNTM_CFG)
    {
        LINS_SCB_UartStop();
    }
#endif /* (!LINS_SCB_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
#elif (LINS_SCB_SCB_MODE_I2C_CONST_CFG)
    LINS_SCB_I2CStop();

#elif (LINS_SCB_SCB_MODE_EZI2C_CONST_CFG)
    LINS_SCB_EzI2CStop();

#elif (LINS_SCB_SCB_MODE_SPI_CONST_CFG)
    LINS_SCB_SpiStop();

#elif (LINS_SCB_SCB_MODE_UART_CONST_CFG)
    LINS_SCB_UartStop();

#else
    /* Unknown mode: do nothing */
#endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: LINS_SCB_SetPins
    ****************************************************************************//**
    *
    *  Sets the pins settings accordingly to the selected operation mode.
    *  Only available in the Unconfigured operation mode. The mode specific
    *  initialization function calls it.
    *  Pins configuration is set by PSoC Creator when a specific mode of operation
    *  is selected in design time.
    *
    *  \param mode:      Mode of SCB operation.
    *  \param subMode:   Sub-mode of SCB operation. It is only required for SPI and UART
    *             modes.
    *  \param uartEnableMask: enables TX or RX direction and RTS and CTS signals.
    *
    *******************************************************************************/
    void LINS_SCB_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask)
    {
        uint32 pinsDm[LINS_SCB_SCB_PINS_NUMBER];
        uint32 i;
        
    #if (!LINS_SCB_CY_SCBIP_V1)
        uint32 pinsInBuf = 0u;
    #endif /* (!LINS_SCB_CY_SCBIP_V1) */
        
        uint32 hsiomSel[LINS_SCB_SCB_PINS_NUMBER] = 
        {
            LINS_SCB_RX_SCL_MOSI_HSIOM_SEL_GPIO,
            LINS_SCB_TX_SDA_MISO_HSIOM_SEL_GPIO,
            0u,
            0u,
            0u,
            0u,
            0u,
        };

    #if (LINS_SCB_CY_SCBIP_V1)
        /* Supress compiler warning. */
        if ((0u == subMode) || (0u == uartEnableMask))
        {
        }
    #endif /* (LINS_SCB_CY_SCBIP_V1) */

        /* Set default HSIOM to GPIO and Drive Mode to Analog Hi-Z */
        for (i = 0u; i < LINS_SCB_SCB_PINS_NUMBER; i++)
        {
            pinsDm[i] = LINS_SCB_PIN_DM_ALG_HIZ;
        }

        if ((LINS_SCB_SCB_MODE_I2C   == mode) ||
            (LINS_SCB_SCB_MODE_EZI2C == mode))
        {
        #if (LINS_SCB_RX_SCL_MOSI_PIN)
            hsiomSel[LINS_SCB_RX_SCL_MOSI_PIN_INDEX] = LINS_SCB_RX_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [LINS_SCB_RX_SCL_MOSI_PIN_INDEX] = LINS_SCB_PIN_DM_OD_LO;
        #elif (LINS_SCB_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[LINS_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX] = LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [LINS_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX] = LINS_SCB_PIN_DM_OD_LO;
        #else
        #endif /* (LINS_SCB_RX_SCL_MOSI_PIN) */
        
        #if (LINS_SCB_TX_SDA_MISO_PIN)
            hsiomSel[LINS_SCB_TX_SDA_MISO_PIN_INDEX] = LINS_SCB_TX_SDA_MISO_HSIOM_SEL_I2C;
            pinsDm  [LINS_SCB_TX_SDA_MISO_PIN_INDEX] = LINS_SCB_PIN_DM_OD_LO;
        #endif /* (LINS_SCB_TX_SDA_MISO_PIN) */
        }
    #if (!LINS_SCB_CY_SCBIP_V1)
        else if (LINS_SCB_SCB_MODE_SPI == mode)
        {
        #if (LINS_SCB_RX_SCL_MOSI_PIN)
            hsiomSel[LINS_SCB_RX_SCL_MOSI_PIN_INDEX] = LINS_SCB_RX_SCL_MOSI_HSIOM_SEL_SPI;
        #elif (LINS_SCB_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[LINS_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX] = LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI;
        #else
        #endif /* (LINS_SCB_RX_SCL_MOSI_PIN) */
        
        #if (LINS_SCB_TX_SDA_MISO_PIN)
            hsiomSel[LINS_SCB_TX_SDA_MISO_PIN_INDEX] = LINS_SCB_TX_SDA_MISO_HSIOM_SEL_SPI;
        #endif /* (LINS_SCB_TX_SDA_MISO_PIN) */
        
        #if (LINS_SCB_SCLK_PIN)
            hsiomSel[LINS_SCB_SCLK_PIN_INDEX] = LINS_SCB_SCLK_HSIOM_SEL_SPI;
        #endif /* (LINS_SCB_SCLK_PIN) */

            if (LINS_SCB_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[LINS_SCB_RX_SCL_MOSI_PIN_INDEX] = LINS_SCB_PIN_DM_DIG_HIZ;
                pinsDm[LINS_SCB_TX_SDA_MISO_PIN_INDEX] = LINS_SCB_PIN_DM_STRONG;
                pinsDm[LINS_SCB_SCLK_PIN_INDEX] = LINS_SCB_PIN_DM_DIG_HIZ;

            #if (LINS_SCB_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[LINS_SCB_SS0_PIN_INDEX] = LINS_SCB_SS0_HSIOM_SEL_SPI;
                pinsDm  [LINS_SCB_SS0_PIN_INDEX] = LINS_SCB_PIN_DM_DIG_HIZ;
            #endif /* (LINS_SCB_SS0_PIN) */

            #if (LINS_SCB_TX_SDA_MISO_PIN)
                /* Disable input buffer */
                 pinsInBuf |= LINS_SCB_TX_SDA_MISO_PIN_MASK;
            #endif /* (LINS_SCB_TX_SDA_MISO_PIN) */
            }
            else 
            {
                /* (Master) */
                pinsDm[LINS_SCB_RX_SCL_MOSI_PIN_INDEX] = LINS_SCB_PIN_DM_STRONG;
                pinsDm[LINS_SCB_TX_SDA_MISO_PIN_INDEX] = LINS_SCB_PIN_DM_DIG_HIZ;
                pinsDm[LINS_SCB_SCLK_PIN_INDEX] = LINS_SCB_PIN_DM_STRONG;

            #if (LINS_SCB_SS0_PIN)
                hsiomSel [LINS_SCB_SS0_PIN_INDEX] = LINS_SCB_SS0_HSIOM_SEL_SPI;
                pinsDm   [LINS_SCB_SS0_PIN_INDEX] = LINS_SCB_PIN_DM_STRONG;
                pinsInBuf |= LINS_SCB_SS0_PIN_MASK;
            #endif /* (LINS_SCB_SS0_PIN) */

            #if (LINS_SCB_SS1_PIN)
                hsiomSel [LINS_SCB_SS1_PIN_INDEX] = LINS_SCB_SS1_HSIOM_SEL_SPI;
                pinsDm   [LINS_SCB_SS1_PIN_INDEX] = LINS_SCB_PIN_DM_STRONG;
                pinsInBuf |= LINS_SCB_SS1_PIN_MASK;
            #endif /* (LINS_SCB_SS1_PIN) */

            #if (LINS_SCB_SS2_PIN)
                hsiomSel [LINS_SCB_SS2_PIN_INDEX] = LINS_SCB_SS2_HSIOM_SEL_SPI;
                pinsDm   [LINS_SCB_SS2_PIN_INDEX] = LINS_SCB_PIN_DM_STRONG;
                pinsInBuf |= LINS_SCB_SS2_PIN_MASK;
            #endif /* (LINS_SCB_SS2_PIN) */

            #if (LINS_SCB_SS3_PIN)
                hsiomSel [LINS_SCB_SS3_PIN_INDEX] = LINS_SCB_SS3_HSIOM_SEL_SPI;
                pinsDm   [LINS_SCB_SS3_PIN_INDEX] = LINS_SCB_PIN_DM_STRONG;
                pinsInBuf |= LINS_SCB_SS3_PIN_MASK;
            #endif /* (LINS_SCB_SS3_PIN) */

                /* Disable input buffers */
            #if (LINS_SCB_RX_SCL_MOSI_PIN)
                pinsInBuf |= LINS_SCB_RX_SCL_MOSI_PIN_MASK;
            #elif (LINS_SCB_RX_WAKE_SCL_MOSI_PIN)
                pinsInBuf |= LINS_SCB_RX_WAKE_SCL_MOSI_PIN_MASK;
            #else
            #endif /* (LINS_SCB_RX_SCL_MOSI_PIN) */

            #if (LINS_SCB_SCLK_PIN)
                pinsInBuf |= LINS_SCB_SCLK_PIN_MASK;
            #endif /* (LINS_SCB_SCLK_PIN) */
            }
        }
        else /* UART */
        {
            if (LINS_SCB_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
            #if (LINS_SCB_TX_SDA_MISO_PIN)
                hsiomSel[LINS_SCB_TX_SDA_MISO_PIN_INDEX] = LINS_SCB_TX_SDA_MISO_HSIOM_SEL_UART;
                pinsDm  [LINS_SCB_TX_SDA_MISO_PIN_INDEX] = LINS_SCB_PIN_DM_OD_LO;
            #endif /* (LINS_SCB_TX_SDA_MISO_PIN) */
            }
            else /* Standard or IrDA */
            {
                if (0u != (LINS_SCB_UART_RX_PIN_ENABLE & uartEnableMask))
                {
                #if (LINS_SCB_RX_SCL_MOSI_PIN)
                    hsiomSel[LINS_SCB_RX_SCL_MOSI_PIN_INDEX] = LINS_SCB_RX_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [LINS_SCB_RX_SCL_MOSI_PIN_INDEX] = LINS_SCB_PIN_DM_DIG_HIZ;
                #elif (LINS_SCB_RX_WAKE_SCL_MOSI_PIN)
                    hsiomSel[LINS_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX] = LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [LINS_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX] = LINS_SCB_PIN_DM_DIG_HIZ;
                #else
                #endif /* (LINS_SCB_RX_SCL_MOSI_PIN) */
                }

                if (0u != (LINS_SCB_UART_TX_PIN_ENABLE & uartEnableMask))
                {
                #if (LINS_SCB_TX_SDA_MISO_PIN)
                    hsiomSel[LINS_SCB_TX_SDA_MISO_PIN_INDEX] = LINS_SCB_TX_SDA_MISO_HSIOM_SEL_UART;
                    pinsDm  [LINS_SCB_TX_SDA_MISO_PIN_INDEX] = LINS_SCB_PIN_DM_STRONG;
                    
                    /* Disable input buffer */
                    pinsInBuf |= LINS_SCB_TX_SDA_MISO_PIN_MASK;
                #endif /* (LINS_SCB_TX_SDA_MISO_PIN) */
                }

            #if !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1)
                if (LINS_SCB_UART_MODE_STD == subMode)
                {
                    if (0u != (LINS_SCB_UART_CTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* CTS input is multiplexed with SCLK */
                    #if (LINS_SCB_SCLK_PIN)
                        hsiomSel[LINS_SCB_SCLK_PIN_INDEX] = LINS_SCB_SCLK_HSIOM_SEL_UART;
                        pinsDm  [LINS_SCB_SCLK_PIN_INDEX] = LINS_SCB_PIN_DM_DIG_HIZ;
                    #endif /* (LINS_SCB_SCLK_PIN) */
                    }

                    if (0u != (LINS_SCB_UART_RTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* RTS output is multiplexed with SS0 */
                    #if (LINS_SCB_SS0_PIN)
                        hsiomSel[LINS_SCB_SS0_PIN_INDEX] = LINS_SCB_SS0_HSIOM_SEL_UART;
                        pinsDm  [LINS_SCB_SS0_PIN_INDEX] = LINS_SCB_PIN_DM_STRONG;
                        
                        /* Disable input buffer */
                        pinsInBuf |= LINS_SCB_SS0_PIN_MASK;
                    #endif /* (LINS_SCB_SS0_PIN) */
                    }
                }
            #endif /* !(LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1) */
            }
        }
    #endif /* (!LINS_SCB_CY_SCBIP_V1) */

    /* Configure pins: set HSIOM, DM and InputBufEnable */
    /* Note: the DR register settings do not effect the pin output if HSIOM is other than GPIO */

    #if (LINS_SCB_RX_SCL_MOSI_PIN)
        LINS_SCB_SET_HSIOM_SEL(LINS_SCB_RX_SCL_MOSI_HSIOM_REG,
                                       LINS_SCB_RX_SCL_MOSI_HSIOM_MASK,
                                       LINS_SCB_RX_SCL_MOSI_HSIOM_POS,
                                        hsiomSel[LINS_SCB_RX_SCL_MOSI_PIN_INDEX]);

        LINS_SCB_uart_rx_i2c_scl_spi_mosi_SetDriveMode((uint8) pinsDm[LINS_SCB_RX_SCL_MOSI_PIN_INDEX]);

        #if (!LINS_SCB_CY_SCBIP_V1)
            LINS_SCB_SET_INP_DIS(LINS_SCB_uart_rx_i2c_scl_spi_mosi_INP_DIS,
                                         LINS_SCB_uart_rx_i2c_scl_spi_mosi_MASK,
                                         (0u != (pinsInBuf & LINS_SCB_RX_SCL_MOSI_PIN_MASK)));
        #endif /* (!LINS_SCB_CY_SCBIP_V1) */
    
    #elif (LINS_SCB_RX_WAKE_SCL_MOSI_PIN)
        LINS_SCB_SET_HSIOM_SEL(LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG,
                                       LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_MASK,
                                       LINS_SCB_RX_WAKE_SCL_MOSI_HSIOM_POS,
                                       hsiomSel[LINS_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi_SetDriveMode((uint8)
                                                               pinsDm[LINS_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        LINS_SCB_SET_INP_DIS(LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi_INP_DIS,
                                     LINS_SCB_uart_rx_wake_i2c_scl_spi_mosi_MASK,
                                     (0u != (pinsInBuf & LINS_SCB_RX_WAKE_SCL_MOSI_PIN_MASK)));

         /* Set interrupt on falling edge */
        LINS_SCB_SET_INCFG_TYPE(LINS_SCB_RX_WAKE_SCL_MOSI_INTCFG_REG,
                                        LINS_SCB_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK,
                                        LINS_SCB_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS,
                                        LINS_SCB_INTCFG_TYPE_FALLING_EDGE);
    #else
    #endif /* (LINS_SCB_RX_WAKE_SCL_MOSI_PIN) */

    #if (LINS_SCB_TX_SDA_MISO_PIN)
        LINS_SCB_SET_HSIOM_SEL(LINS_SCB_TX_SDA_MISO_HSIOM_REG,
                                       LINS_SCB_TX_SDA_MISO_HSIOM_MASK,
                                       LINS_SCB_TX_SDA_MISO_HSIOM_POS,
                                        hsiomSel[LINS_SCB_TX_SDA_MISO_PIN_INDEX]);

        LINS_SCB_uart_tx_i2c_sda_spi_miso_SetDriveMode((uint8) pinsDm[LINS_SCB_TX_SDA_MISO_PIN_INDEX]);

    #if (!LINS_SCB_CY_SCBIP_V1)
        LINS_SCB_SET_INP_DIS(LINS_SCB_uart_tx_i2c_sda_spi_miso_INP_DIS,
                                     LINS_SCB_uart_tx_i2c_sda_spi_miso_MASK,
                                    (0u != (pinsInBuf & LINS_SCB_TX_SDA_MISO_PIN_MASK)));
    #endif /* (!LINS_SCB_CY_SCBIP_V1) */
    #endif /* (LINS_SCB_RX_SCL_MOSI_PIN) */

    #if (LINS_SCB_SCLK_PIN)
        LINS_SCB_SET_HSIOM_SEL(LINS_SCB_SCLK_HSIOM_REG,
                                       LINS_SCB_SCLK_HSIOM_MASK,
                                       LINS_SCB_SCLK_HSIOM_POS,
                                       hsiomSel[LINS_SCB_SCLK_PIN_INDEX]);

        LINS_SCB_spi_sclk_SetDriveMode((uint8) pinsDm[LINS_SCB_SCLK_PIN_INDEX]);

        LINS_SCB_SET_INP_DIS(LINS_SCB_spi_sclk_INP_DIS,
                                     LINS_SCB_spi_sclk_MASK,
                                     (0u != (pinsInBuf & LINS_SCB_SCLK_PIN_MASK)));
    #endif /* (LINS_SCB_SCLK_PIN) */

    #if (LINS_SCB_SS0_PIN)
        LINS_SCB_SET_HSIOM_SEL(LINS_SCB_SS0_HSIOM_REG,
                                       LINS_SCB_SS0_HSIOM_MASK,
                                       LINS_SCB_SS0_HSIOM_POS,
                                       hsiomSel[LINS_SCB_SS0_PIN_INDEX]);

        LINS_SCB_spi_ss0_SetDriveMode((uint8) pinsDm[LINS_SCB_SS0_PIN_INDEX]);

        LINS_SCB_SET_INP_DIS(LINS_SCB_spi_ss0_INP_DIS,
                                     LINS_SCB_spi_ss0_MASK,
                                     (0u != (pinsInBuf & LINS_SCB_SS0_PIN_MASK)));
    #endif /* (LINS_SCB_SS0_PIN) */

    #if (LINS_SCB_SS1_PIN)
        LINS_SCB_SET_HSIOM_SEL(LINS_SCB_SS1_HSIOM_REG,
                                       LINS_SCB_SS1_HSIOM_MASK,
                                       LINS_SCB_SS1_HSIOM_POS,
                                       hsiomSel[LINS_SCB_SS1_PIN_INDEX]);

        LINS_SCB_spi_ss1_SetDriveMode((uint8) pinsDm[LINS_SCB_SS1_PIN_INDEX]);

        LINS_SCB_SET_INP_DIS(LINS_SCB_spi_ss1_INP_DIS,
                                     LINS_SCB_spi_ss1_MASK,
                                     (0u != (pinsInBuf & LINS_SCB_SS1_PIN_MASK)));
    #endif /* (LINS_SCB_SS1_PIN) */

    #if (LINS_SCB_SS2_PIN)
        LINS_SCB_SET_HSIOM_SEL(LINS_SCB_SS2_HSIOM_REG,
                                       LINS_SCB_SS2_HSIOM_MASK,
                                       LINS_SCB_SS2_HSIOM_POS,
                                       hsiomSel[LINS_SCB_SS2_PIN_INDEX]);

        LINS_SCB_spi_ss2_SetDriveMode((uint8) pinsDm[LINS_SCB_SS2_PIN_INDEX]);

        LINS_SCB_SET_INP_DIS(LINS_SCB_spi_ss2_INP_DIS,
                                     LINS_SCB_spi_ss2_MASK,
                                     (0u != (pinsInBuf & LINS_SCB_SS2_PIN_MASK)));
    #endif /* (LINS_SCB_SS2_PIN) */

    #if (LINS_SCB_SS3_PIN)
        LINS_SCB_SET_HSIOM_SEL(LINS_SCB_SS3_HSIOM_REG,
                                       LINS_SCB_SS3_HSIOM_MASK,
                                       LINS_SCB_SS3_HSIOM_POS,
                                       hsiomSel[LINS_SCB_SS3_PIN_INDEX]);

        LINS_SCB_spi_ss3_SetDriveMode((uint8) pinsDm[LINS_SCB_SS3_PIN_INDEX]);

        LINS_SCB_SET_INP_DIS(LINS_SCB_spi_ss3_INP_DIS,
                                     LINS_SCB_spi_ss3_MASK,
                                     (0u != (pinsInBuf & LINS_SCB_SS3_PIN_MASK)));
    #endif /* (LINS_SCB_SS3_PIN) */
    }

#endif /* (LINS_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: LINS_SCB_I2CSlaveNackGeneration
    ****************************************************************************//**
    *
    *  Sets command to generate NACK to the address or data.
    *
    *******************************************************************************/
    void LINS_SCB_I2CSlaveNackGeneration(void)
    {
        /* Check for EC_AM toggle condition: EC_AM and clock stretching for address are enabled */
        if ((0u != (LINS_SCB_CTRL_REG & LINS_SCB_CTRL_EC_AM_MODE)) &&
            (0u == (LINS_SCB_I2C_CTRL_REG & LINS_SCB_I2C_CTRL_S_NOT_READY_ADDR_NACK)))
        {
            /* Toggle EC_AM before NACK generation */
            LINS_SCB_CTRL_REG &= ~LINS_SCB_CTRL_EC_AM_MODE;
            LINS_SCB_CTRL_REG |=  LINS_SCB_CTRL_EC_AM_MODE;
        }

        LINS_SCB_I2C_SLAVE_CMD_REG = LINS_SCB_I2C_SLAVE_CMD_S_NACK;
    }
#endif /* (LINS_SCB_CY_SCBIP_V0 || LINS_SCB_CY_SCBIP_V1) */


/* [] END OF FILE */
