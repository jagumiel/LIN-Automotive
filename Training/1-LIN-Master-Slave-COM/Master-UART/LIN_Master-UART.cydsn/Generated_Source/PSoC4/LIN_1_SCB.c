/***************************************************************************//**
* \file LIN_1_SCB.c
* \version 3.20
*
* \brief
*  This file provides the source code to the API for the SCB Component.
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

#if (LIN_1_SCB_SCB_MODE_I2C_INC)
    #include "LIN_1_SCB_I2C_PVT.h"
#endif /* (LIN_1_SCB_SCB_MODE_I2C_INC) */

#if (LIN_1_SCB_SCB_MODE_EZI2C_INC)
    #include "LIN_1_SCB_EZI2C_PVT.h"
#endif /* (LIN_1_SCB_SCB_MODE_EZI2C_INC) */

#if (LIN_1_SCB_SCB_MODE_SPI_INC || LIN_1_SCB_SCB_MODE_UART_INC)
    #include "LIN_1_SCB_SPI_UART_PVT.h"
#endif /* (LIN_1_SCB_SCB_MODE_SPI_INC || LIN_1_SCB_SCB_MODE_UART_INC) */


/***************************************
*    Run Time Configuration Vars
***************************************/

/* Stores internal component configuration for Unconfigured mode */
#if (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    uint8 LIN_1_SCB_scbMode = LIN_1_SCB_SCB_MODE_UNCONFIG;
    uint8 LIN_1_SCB_scbEnableWake;
    uint8 LIN_1_SCB_scbEnableIntr;

    /* I2C configuration variables */
    uint8 LIN_1_SCB_mode;
    uint8 LIN_1_SCB_acceptAddr;

    /* SPI/UART configuration variables */
    volatile uint8 * LIN_1_SCB_rxBuffer;
    uint8  LIN_1_SCB_rxDataBits;
    uint32 LIN_1_SCB_rxBufferSize;

    volatile uint8 * LIN_1_SCB_txBuffer;
    uint8  LIN_1_SCB_txDataBits;
    uint32 LIN_1_SCB_txBufferSize;

    /* EZI2C configuration variables */
    uint8 LIN_1_SCB_numberOfAddr;
    uint8 LIN_1_SCB_subAddrSize;
#endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Common SCB Vars
***************************************/
/**
* \addtogroup group_general
* \{
*/

/** LIN_1_SCB_initVar indicates whether the LIN_1_SCB 
*  component has been initialized. The variable is initialized to 0 
*  and set to 1 the first time SCB_Start() is called. This allows 
*  the component to restart without reinitialization after the first 
*  call to the LIN_1_SCB_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  LIN_1_SCB_Init() function can be called before the 
*  LIN_1_SCB_Start() or LIN_1_SCB_Enable() function.
*/
uint8 LIN_1_SCB_initVar = 0u;


#if (! (LIN_1_SCB_SCB_MODE_I2C_CONST_CFG || \
        LIN_1_SCB_SCB_MODE_EZI2C_CONST_CFG))
    /** This global variable stores TX interrupt sources after 
    * LIN_1_SCB_Stop() is called. Only these TX interrupt sources 
    * will be restored on a subsequent LIN_1_SCB_Enable() call.
    */
    uint16 LIN_1_SCB_IntrTxMask = 0u;
#endif /* (! (LIN_1_SCB_SCB_MODE_I2C_CONST_CFG || \
              LIN_1_SCB_SCB_MODE_EZI2C_CONST_CFG)) */
/** \} globals */

#if (LIN_1_SCB_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_LIN_1_SCB_CUSTOM_INTR_HANDLER)
    void (*LIN_1_SCB_customIntrHandler)(void) = NULL;
#endif /* !defined (CY_REMOVE_LIN_1_SCB_CUSTOM_INTR_HANDLER) */
#endif /* (LIN_1_SCB_SCB_IRQ_INTERNAL) */


/***************************************
*    Private Function Prototypes
***************************************/

static void LIN_1_SCB_ScbEnableIntr(void);
static void LIN_1_SCB_ScbModeStop(void);
static void LIN_1_SCB_ScbModePostEnable(void);


/*******************************************************************************
* Function Name: LIN_1_SCB_Init
****************************************************************************//**
*
*  Initializes the LIN_1_SCB component to operate in one of the selected
*  configurations: I2C, SPI, UART or EZI2C.
*  When the configuration is set to "Unconfigured SCB", this function does
*  not do any initialization. Use mode-specific initialization APIs instead:
*  LIN_1_SCB_I2CInit, LIN_1_SCB_SpiInit, 
*  LIN_1_SCB_UartInit or LIN_1_SCB_EzI2CInit.
*
*******************************************************************************/
void LIN_1_SCB_Init(void)
{
#if (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    if (LIN_1_SCB_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        LIN_1_SCB_initVar = 0u;
    }
    else
    {
        /* Initialization was done before this function call */
    }

#elif (LIN_1_SCB_SCB_MODE_I2C_CONST_CFG)
    LIN_1_SCB_I2CInit();

#elif (LIN_1_SCB_SCB_MODE_SPI_CONST_CFG)
    LIN_1_SCB_SpiInit();

#elif (LIN_1_SCB_SCB_MODE_UART_CONST_CFG)
    LIN_1_SCB_UartInit();

#elif (LIN_1_SCB_SCB_MODE_EZI2C_CONST_CFG)
    LIN_1_SCB_EzI2CInit();

#endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: LIN_1_SCB_Enable
****************************************************************************//**
*
*  Enables LIN_1_SCB component operation: activates the hardware and 
*  internal interrupt. It also restores TX interrupt sources disabled after the 
*  LIN_1_SCB_Stop() function was called (note that level-triggered TX 
*  interrupt sources remain disabled to not cause code lock-up).
*  For I2C and EZI2C modes the interrupt is internal and mandatory for 
*  operation. For SPI and UART modes the interrupt can be configured as none, 
*  internal or external.
*  The LIN_1_SCB configuration should be not changed when the component
*  is enabled. Any configuration changes should be made after disabling the 
*  component.
*  When configuration is set to “Unconfigured LIN_1_SCB”, the component 
*  must first be initialized to operate in one of the following configurations: 
*  I2C, SPI, UART or EZ I2C, using the mode-specific initialization API. 
*  Otherwise this function does not enable the component.
*
*******************************************************************************/
void LIN_1_SCB_Enable(void)
{
#if (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Enable SCB block, only if it is already configured */
    if (!LIN_1_SCB_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        LIN_1_SCB_CTRL_REG |= LIN_1_SCB_CTRL_ENABLED;

        LIN_1_SCB_ScbEnableIntr();

        /* Call PostEnable function specific to current operation mode */
        LIN_1_SCB_ScbModePostEnable();
    }
#else
    LIN_1_SCB_CTRL_REG |= LIN_1_SCB_CTRL_ENABLED;

    LIN_1_SCB_ScbEnableIntr();

    /* Call PostEnable function specific to current operation mode */
    LIN_1_SCB_ScbModePostEnable();
#endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: LIN_1_SCB_Start
****************************************************************************//**
*
*  Invokes LIN_1_SCB_Init() and LIN_1_SCB_Enable().
*  After this function call, the component is enabled and ready for operation.
*  When configuration is set to "Unconfigured SCB", the component must first be
*  initialized to operate in one of the following configurations: I2C, SPI, UART
*  or EZI2C. Otherwise this function does not enable the component.
*
* \globalvars
*  LIN_1_SCB_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void LIN_1_SCB_Start(void)
{
    if (0u == LIN_1_SCB_initVar)
    {
        LIN_1_SCB_Init();
        LIN_1_SCB_initVar = 1u; /* Component was initialized */
    }

    LIN_1_SCB_Enable();
}


/*******************************************************************************
* Function Name: LIN_1_SCB_Stop
****************************************************************************//**
*
*  Disables the LIN_1_SCB component: disable the hardware and internal 
*  interrupt. It also disables all TX interrupt sources so as not to cause an 
*  unexpected interrupt trigger because after the component is enabled, the 
*  TX FIFO is empty.
*  Refer to the function LIN_1_SCB_Enable() for the interrupt 
*  configuration details.
*  This function disables the SCB component without checking to see if 
*  communication is in progress. Before calling this function it may be 
*  necessary to check the status of communication to make sure communication 
*  is complete. If this is not done then communication could be stopped mid 
*  byte and corrupted data could result.
*
*******************************************************************************/
void LIN_1_SCB_Stop(void)
{
#if (LIN_1_SCB_SCB_IRQ_INTERNAL)
    LIN_1_SCB_DisableInt();
#endif /* (LIN_1_SCB_SCB_IRQ_INTERNAL) */

    /* Call Stop function specific to current operation mode */
    LIN_1_SCB_ScbModeStop();

    /* Disable SCB IP */
    LIN_1_SCB_CTRL_REG &= (uint32) ~LIN_1_SCB_CTRL_ENABLED;

    /* Disable all TX interrupt sources so as not to cause an unexpected
    * interrupt trigger after the component will be enabled because the 
    * TX FIFO is empty.
    * For SCB IP v0, it is critical as it does not mask-out interrupt
    * sources when it is disabled. This can cause a code lock-up in the
    * interrupt handler because TX FIFO cannot be loaded after the block
    * is disabled.
    */
    LIN_1_SCB_SetTxInterruptMode(LIN_1_SCB_NO_INTR_SOURCES);

#if (LIN_1_SCB_SCB_IRQ_INTERNAL)
    LIN_1_SCB_ClearPendingInt();
#endif /* (LIN_1_SCB_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: LIN_1_SCB_SetRxFifoLevel
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
void LIN_1_SCB_SetRxFifoLevel(uint32 level)
{
    uint32 rxFifoCtrl;

    rxFifoCtrl = LIN_1_SCB_RX_FIFO_CTRL_REG;

    rxFifoCtrl &= ((uint32) ~LIN_1_SCB_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    rxFifoCtrl |= ((uint32) (LIN_1_SCB_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    LIN_1_SCB_RX_FIFO_CTRL_REG = rxFifoCtrl;
}


/*******************************************************************************
* Function Name: LIN_1_SCB_SetTxFifoLevel
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
void LIN_1_SCB_SetTxFifoLevel(uint32 level)
{
    uint32 txFifoCtrl;

    txFifoCtrl = LIN_1_SCB_TX_FIFO_CTRL_REG;

    txFifoCtrl &= ((uint32) ~LIN_1_SCB_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    txFifoCtrl |= ((uint32) (LIN_1_SCB_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    LIN_1_SCB_TX_FIFO_CTRL_REG = txFifoCtrl;
}


#if (LIN_1_SCB_SCB_IRQ_INTERNAL)
    /*******************************************************************************
    * Function Name: LIN_1_SCB_SetCustomInterruptHandler
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
    void LIN_1_SCB_SetCustomInterruptHandler(void (*func)(void))
    {
    #if !defined (CY_REMOVE_LIN_1_SCB_CUSTOM_INTR_HANDLER)
        LIN_1_SCB_customIntrHandler = func; /* Register interrupt handler */
    #else
        if (NULL != func)
        {
            /* Suppress compiler warning */
        }
    #endif /* !defined (CY_REMOVE_LIN_1_SCB_CUSTOM_INTR_HANDLER) */
    }
#endif /* (LIN_1_SCB_SCB_IRQ_INTERNAL) */


/*******************************************************************************
* Function Name: LIN_1_SCB_ScbModeEnableIntr
****************************************************************************//**
*
*  Enables an interrupt for a specific mode.
*
*******************************************************************************/
static void LIN_1_SCB_ScbEnableIntr(void)
{
#if (LIN_1_SCB_SCB_IRQ_INTERNAL)
    /* Enable interrupt in NVIC */
    #if (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
        if (0u != LIN_1_SCB_scbEnableIntr)
        {
            LIN_1_SCB_EnableInt();
        }

    #else
        LIN_1_SCB_EnableInt();

    #endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (LIN_1_SCB_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: LIN_1_SCB_ScbModePostEnable
****************************************************************************//**
*
*  Calls the PostEnable function for a specific operation mode.
*
*******************************************************************************/
static void LIN_1_SCB_ScbModePostEnable(void)
{
#if (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
#if (!LIN_1_SCB_CY_SCBIP_V1)
    if (LIN_1_SCB_SCB_MODE_SPI_RUNTM_CFG)
    {
        LIN_1_SCB_SpiPostEnable();
    }
    else if (LIN_1_SCB_SCB_MODE_UART_RUNTM_CFG)
    {
        LIN_1_SCB_UartPostEnable();
    }
    else
    {
        /* Unknown mode: do nothing */
    }
#endif /* (!LIN_1_SCB_CY_SCBIP_V1) */

#elif (LIN_1_SCB_SCB_MODE_SPI_CONST_CFG)
    LIN_1_SCB_SpiPostEnable();

#elif (LIN_1_SCB_SCB_MODE_UART_CONST_CFG)
    LIN_1_SCB_UartPostEnable();

#else
    /* Unknown mode: do nothing */
#endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: LIN_1_SCB_ScbModeStop
****************************************************************************//**
*
*  Calls the Stop function for a specific operation mode.
*
*******************************************************************************/
static void LIN_1_SCB_ScbModeStop(void)
{
#if (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    if (LIN_1_SCB_SCB_MODE_I2C_RUNTM_CFG)
    {
        LIN_1_SCB_I2CStop();
    }
    else if (LIN_1_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        LIN_1_SCB_EzI2CStop();
    }
#if (!LIN_1_SCB_CY_SCBIP_V1)
    else if (LIN_1_SCB_SCB_MODE_SPI_RUNTM_CFG)
    {
        LIN_1_SCB_SpiStop();
    }
    else if (LIN_1_SCB_SCB_MODE_UART_RUNTM_CFG)
    {
        LIN_1_SCB_UartStop();
    }
#endif /* (!LIN_1_SCB_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
#elif (LIN_1_SCB_SCB_MODE_I2C_CONST_CFG)
    LIN_1_SCB_I2CStop();

#elif (LIN_1_SCB_SCB_MODE_EZI2C_CONST_CFG)
    LIN_1_SCB_EzI2CStop();

#elif (LIN_1_SCB_SCB_MODE_SPI_CONST_CFG)
    LIN_1_SCB_SpiStop();

#elif (LIN_1_SCB_SCB_MODE_UART_CONST_CFG)
    LIN_1_SCB_UartStop();

#else
    /* Unknown mode: do nothing */
#endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: LIN_1_SCB_SetPins
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
    void LIN_1_SCB_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask)
    {
        uint32 pinsDm[LIN_1_SCB_SCB_PINS_NUMBER];
        uint32 i;
        
    #if (!LIN_1_SCB_CY_SCBIP_V1)
        uint32 pinsInBuf = 0u;
    #endif /* (!LIN_1_SCB_CY_SCBIP_V1) */
        
        uint32 hsiomSel[LIN_1_SCB_SCB_PINS_NUMBER] = 
        {
            LIN_1_SCB_RX_SCL_MOSI_HSIOM_SEL_GPIO,
            LIN_1_SCB_TX_SDA_MISO_HSIOM_SEL_GPIO,
            0u,
            0u,
            0u,
            0u,
            0u,
        };

    #if (LIN_1_SCB_CY_SCBIP_V1)
        /* Supress compiler warning. */
        if ((0u == subMode) || (0u == uartEnableMask))
        {
        }
    #endif /* (LIN_1_SCB_CY_SCBIP_V1) */

        /* Set default HSIOM to GPIO and Drive Mode to Analog Hi-Z */
        for (i = 0u; i < LIN_1_SCB_SCB_PINS_NUMBER; i++)
        {
            pinsDm[i] = LIN_1_SCB_PIN_DM_ALG_HIZ;
        }

        if ((LIN_1_SCB_SCB_MODE_I2C   == mode) ||
            (LIN_1_SCB_SCB_MODE_EZI2C == mode))
        {
        #if (LIN_1_SCB_RX_SCL_MOSI_PIN)
            hsiomSel[LIN_1_SCB_RX_SCL_MOSI_PIN_INDEX] = LIN_1_SCB_RX_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [LIN_1_SCB_RX_SCL_MOSI_PIN_INDEX] = LIN_1_SCB_PIN_DM_OD_LO;
        #elif (LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX] = LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX] = LIN_1_SCB_PIN_DM_OD_LO;
        #else
        #endif /* (LIN_1_SCB_RX_SCL_MOSI_PIN) */
        
        #if (LIN_1_SCB_TX_SDA_MISO_PIN)
            hsiomSel[LIN_1_SCB_TX_SDA_MISO_PIN_INDEX] = LIN_1_SCB_TX_SDA_MISO_HSIOM_SEL_I2C;
            pinsDm  [LIN_1_SCB_TX_SDA_MISO_PIN_INDEX] = LIN_1_SCB_PIN_DM_OD_LO;
        #endif /* (LIN_1_SCB_TX_SDA_MISO_PIN) */
        }
    #if (!LIN_1_SCB_CY_SCBIP_V1)
        else if (LIN_1_SCB_SCB_MODE_SPI == mode)
        {
        #if (LIN_1_SCB_RX_SCL_MOSI_PIN)
            hsiomSel[LIN_1_SCB_RX_SCL_MOSI_PIN_INDEX] = LIN_1_SCB_RX_SCL_MOSI_HSIOM_SEL_SPI;
        #elif (LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX] = LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI;
        #else
        #endif /* (LIN_1_SCB_RX_SCL_MOSI_PIN) */
        
        #if (LIN_1_SCB_TX_SDA_MISO_PIN)
            hsiomSel[LIN_1_SCB_TX_SDA_MISO_PIN_INDEX] = LIN_1_SCB_TX_SDA_MISO_HSIOM_SEL_SPI;
        #endif /* (LIN_1_SCB_TX_SDA_MISO_PIN) */
        
        #if (LIN_1_SCB_CTS_SCLK_PIN)
            hsiomSel[LIN_1_SCB_CTS_SCLK_PIN_INDEX] = LIN_1_SCB_CTS_SCLK_HSIOM_SEL_SPI;
        #endif /* (LIN_1_SCB_CTS_SCLK_PIN) */

            if (LIN_1_SCB_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[LIN_1_SCB_RX_SCL_MOSI_PIN_INDEX] = LIN_1_SCB_PIN_DM_DIG_HIZ;
                pinsDm[LIN_1_SCB_TX_SDA_MISO_PIN_INDEX] = LIN_1_SCB_PIN_DM_STRONG;
                pinsDm[LIN_1_SCB_CTS_SCLK_PIN_INDEX] = LIN_1_SCB_PIN_DM_DIG_HIZ;

            #if (LIN_1_SCB_RTS_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[LIN_1_SCB_RTS_SS0_PIN_INDEX] = LIN_1_SCB_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm  [LIN_1_SCB_RTS_SS0_PIN_INDEX] = LIN_1_SCB_PIN_DM_DIG_HIZ;
            #endif /* (LIN_1_SCB_RTS_SS0_PIN) */

            #if (LIN_1_SCB_TX_SDA_MISO_PIN)
                /* Disable input buffer */
                 pinsInBuf |= LIN_1_SCB_TX_SDA_MISO_PIN_MASK;
            #endif /* (LIN_1_SCB_TX_SDA_MISO_PIN) */
            }
            else 
            {
                /* (Master) */
                pinsDm[LIN_1_SCB_RX_SCL_MOSI_PIN_INDEX] = LIN_1_SCB_PIN_DM_STRONG;
                pinsDm[LIN_1_SCB_TX_SDA_MISO_PIN_INDEX] = LIN_1_SCB_PIN_DM_DIG_HIZ;
                pinsDm[LIN_1_SCB_CTS_SCLK_PIN_INDEX] = LIN_1_SCB_PIN_DM_STRONG;

            #if (LIN_1_SCB_RTS_SS0_PIN)
                hsiomSel [LIN_1_SCB_RTS_SS0_PIN_INDEX] = LIN_1_SCB_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm   [LIN_1_SCB_RTS_SS0_PIN_INDEX] = LIN_1_SCB_PIN_DM_STRONG;
                pinsInBuf |= LIN_1_SCB_RTS_SS0_PIN_MASK;
            #endif /* (LIN_1_SCB_RTS_SS0_PIN) */

            #if (LIN_1_SCB_SS1_PIN)
                hsiomSel [LIN_1_SCB_SS1_PIN_INDEX] = LIN_1_SCB_SS1_HSIOM_SEL_SPI;
                pinsDm   [LIN_1_SCB_SS1_PIN_INDEX] = LIN_1_SCB_PIN_DM_STRONG;
                pinsInBuf |= LIN_1_SCB_SS1_PIN_MASK;
            #endif /* (LIN_1_SCB_SS1_PIN) */

            #if (LIN_1_SCB_SS2_PIN)
                hsiomSel [LIN_1_SCB_SS2_PIN_INDEX] = LIN_1_SCB_SS2_HSIOM_SEL_SPI;
                pinsDm   [LIN_1_SCB_SS2_PIN_INDEX] = LIN_1_SCB_PIN_DM_STRONG;
                pinsInBuf |= LIN_1_SCB_SS2_PIN_MASK;
            #endif /* (LIN_1_SCB_SS2_PIN) */

            #if (LIN_1_SCB_SS3_PIN)
                hsiomSel [LIN_1_SCB_SS3_PIN_INDEX] = LIN_1_SCB_SS3_HSIOM_SEL_SPI;
                pinsDm   [LIN_1_SCB_SS3_PIN_INDEX] = LIN_1_SCB_PIN_DM_STRONG;
                pinsInBuf |= LIN_1_SCB_SS3_PIN_MASK;
            #endif /* (LIN_1_SCB_SS3_PIN) */

                /* Disable input buffers */
            #if (LIN_1_SCB_RX_SCL_MOSI_PIN)
                pinsInBuf |= LIN_1_SCB_RX_SCL_MOSI_PIN_MASK;
            #elif (LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN)
                pinsInBuf |= LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN_MASK;
            #else
            #endif /* (LIN_1_SCB_RX_SCL_MOSI_PIN) */

            #if (LIN_1_SCB_CTS_SCLK_PIN)
                pinsInBuf |= LIN_1_SCB_CTS_SCLK_PIN_MASK;
            #endif /* (LIN_1_SCB_CTS_SCLK_PIN) */
            }
        }
        else /* UART */
        {
            if (LIN_1_SCB_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
            #if (LIN_1_SCB_TX_SDA_MISO_PIN)
                hsiomSel[LIN_1_SCB_TX_SDA_MISO_PIN_INDEX] = LIN_1_SCB_TX_SDA_MISO_HSIOM_SEL_UART;
                pinsDm  [LIN_1_SCB_TX_SDA_MISO_PIN_INDEX] = LIN_1_SCB_PIN_DM_OD_LO;
            #endif /* (LIN_1_SCB_TX_SDA_MISO_PIN) */
            }
            else /* Standard or IrDA */
            {
                if (0u != (LIN_1_SCB_UART_RX_PIN_ENABLE & uartEnableMask))
                {
                #if (LIN_1_SCB_RX_SCL_MOSI_PIN)
                    hsiomSel[LIN_1_SCB_RX_SCL_MOSI_PIN_INDEX] = LIN_1_SCB_RX_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [LIN_1_SCB_RX_SCL_MOSI_PIN_INDEX] = LIN_1_SCB_PIN_DM_DIG_HIZ;
                #elif (LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN)
                    hsiomSel[LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX] = LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX] = LIN_1_SCB_PIN_DM_DIG_HIZ;
                #else
                #endif /* (LIN_1_SCB_RX_SCL_MOSI_PIN) */
                }

                if (0u != (LIN_1_SCB_UART_TX_PIN_ENABLE & uartEnableMask))
                {
                #if (LIN_1_SCB_TX_SDA_MISO_PIN)
                    hsiomSel[LIN_1_SCB_TX_SDA_MISO_PIN_INDEX] = LIN_1_SCB_TX_SDA_MISO_HSIOM_SEL_UART;
                    pinsDm  [LIN_1_SCB_TX_SDA_MISO_PIN_INDEX] = LIN_1_SCB_PIN_DM_STRONG;
                    
                    /* Disable input buffer */
                    pinsInBuf |= LIN_1_SCB_TX_SDA_MISO_PIN_MASK;
                #endif /* (LIN_1_SCB_TX_SDA_MISO_PIN) */
                }

            #if !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1)
                if (LIN_1_SCB_UART_MODE_STD == subMode)
                {
                    if (0u != (LIN_1_SCB_UART_CTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* CTS input is multiplexed with SCLK */
                    #if (LIN_1_SCB_CTS_SCLK_PIN)
                        hsiomSel[LIN_1_SCB_CTS_SCLK_PIN_INDEX] = LIN_1_SCB_CTS_SCLK_HSIOM_SEL_UART;
                        pinsDm  [LIN_1_SCB_CTS_SCLK_PIN_INDEX] = LIN_1_SCB_PIN_DM_DIG_HIZ;
                    #endif /* (LIN_1_SCB_CTS_SCLK_PIN) */
                    }

                    if (0u != (LIN_1_SCB_UART_RTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* RTS output is multiplexed with SS0 */
                    #if (LIN_1_SCB_RTS_SS0_PIN)
                        hsiomSel[LIN_1_SCB_RTS_SS0_PIN_INDEX] = LIN_1_SCB_RTS_SS0_HSIOM_SEL_UART;
                        pinsDm  [LIN_1_SCB_RTS_SS0_PIN_INDEX] = LIN_1_SCB_PIN_DM_STRONG;
                        
                        /* Disable input buffer */
                        pinsInBuf |= LIN_1_SCB_RTS_SS0_PIN_MASK;
                    #endif /* (LIN_1_SCB_RTS_SS0_PIN) */
                    }
                }
            #endif /* !(LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1) */
            }
        }
    #endif /* (!LIN_1_SCB_CY_SCBIP_V1) */

    /* Configure pins: set HSIOM, DM and InputBufEnable */
    /* Note: the DR register settings do not effect the pin output if HSIOM is other than GPIO */

    #if (LIN_1_SCB_RX_SCL_MOSI_PIN)
        LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_RX_SCL_MOSI_HSIOM_REG,
                                       LIN_1_SCB_RX_SCL_MOSI_HSIOM_MASK,
                                       LIN_1_SCB_RX_SCL_MOSI_HSIOM_POS,
                                        hsiomSel[LIN_1_SCB_RX_SCL_MOSI_PIN_INDEX]);

        LIN_1_SCB_uart_rx_i2c_scl_spi_mosi_SetDriveMode((uint8) pinsDm[LIN_1_SCB_RX_SCL_MOSI_PIN_INDEX]);

        #if (!LIN_1_SCB_CY_SCBIP_V1)
            LIN_1_SCB_SET_INP_DIS(LIN_1_SCB_uart_rx_i2c_scl_spi_mosi_INP_DIS,
                                         LIN_1_SCB_uart_rx_i2c_scl_spi_mosi_MASK,
                                         (0u != (pinsInBuf & LIN_1_SCB_RX_SCL_MOSI_PIN_MASK)));
        #endif /* (!LIN_1_SCB_CY_SCBIP_V1) */
    
    #elif (LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN)
        LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_REG,
                                       LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_MASK,
                                       LIN_1_SCB_RX_WAKE_SCL_MOSI_HSIOM_POS,
                                       hsiomSel[LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi_SetDriveMode((uint8)
                                                               pinsDm[LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        LIN_1_SCB_SET_INP_DIS(LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi_INP_DIS,
                                     LIN_1_SCB_uart_rx_wake_i2c_scl_spi_mosi_MASK,
                                     (0u != (pinsInBuf & LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN_MASK)));

         /* Set interrupt on falling edge */
        LIN_1_SCB_SET_INCFG_TYPE(LIN_1_SCB_RX_WAKE_SCL_MOSI_INTCFG_REG,
                                        LIN_1_SCB_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK,
                                        LIN_1_SCB_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS,
                                        LIN_1_SCB_INTCFG_TYPE_FALLING_EDGE);
    #else
    #endif /* (LIN_1_SCB_RX_WAKE_SCL_MOSI_PIN) */

    #if (LIN_1_SCB_TX_SDA_MISO_PIN)
        LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_TX_SDA_MISO_HSIOM_REG,
                                       LIN_1_SCB_TX_SDA_MISO_HSIOM_MASK,
                                       LIN_1_SCB_TX_SDA_MISO_HSIOM_POS,
                                        hsiomSel[LIN_1_SCB_TX_SDA_MISO_PIN_INDEX]);

        LIN_1_SCB_uart_tx_i2c_sda_spi_miso_SetDriveMode((uint8) pinsDm[LIN_1_SCB_TX_SDA_MISO_PIN_INDEX]);

    #if (!LIN_1_SCB_CY_SCBIP_V1)
        LIN_1_SCB_SET_INP_DIS(LIN_1_SCB_uart_tx_i2c_sda_spi_miso_INP_DIS,
                                     LIN_1_SCB_uart_tx_i2c_sda_spi_miso_MASK,
                                    (0u != (pinsInBuf & LIN_1_SCB_TX_SDA_MISO_PIN_MASK)));
    #endif /* (!LIN_1_SCB_CY_SCBIP_V1) */
    #endif /* (LIN_1_SCB_RX_SCL_MOSI_PIN) */

    #if (LIN_1_SCB_CTS_SCLK_PIN)
        LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_CTS_SCLK_HSIOM_REG,
                                       LIN_1_SCB_CTS_SCLK_HSIOM_MASK,
                                       LIN_1_SCB_CTS_SCLK_HSIOM_POS,
                                       hsiomSel[LIN_1_SCB_CTS_SCLK_PIN_INDEX]);

        LIN_1_SCB_uart_cts_spi_sclk_SetDriveMode((uint8) pinsDm[LIN_1_SCB_CTS_SCLK_PIN_INDEX]);

        LIN_1_SCB_SET_INP_DIS(LIN_1_SCB_uart_cts_spi_sclk_INP_DIS,
                                     LIN_1_SCB_uart_cts_spi_sclk_MASK,
                                     (0u != (pinsInBuf & LIN_1_SCB_CTS_SCLK_PIN_MASK)));
    #endif /* (LIN_1_SCB_CTS_SCLK_PIN) */

    #if (LIN_1_SCB_RTS_SS0_PIN)
        LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_RTS_SS0_HSIOM_REG,
                                       LIN_1_SCB_RTS_SS0_HSIOM_MASK,
                                       LIN_1_SCB_RTS_SS0_HSIOM_POS,
                                       hsiomSel[LIN_1_SCB_RTS_SS0_PIN_INDEX]);

        LIN_1_SCB_uart_rts_spi_ss0_SetDriveMode((uint8) pinsDm[LIN_1_SCB_RTS_SS0_PIN_INDEX]);

        LIN_1_SCB_SET_INP_DIS(LIN_1_SCB_uart_rts_spi_ss0_INP_DIS,
                                     LIN_1_SCB_uart_rts_spi_ss0_MASK,
                                     (0u != (pinsInBuf & LIN_1_SCB_RTS_SS0_PIN_MASK)));
    #endif /* (LIN_1_SCB_RTS_SS0_PIN) */

    #if (LIN_1_SCB_SS1_PIN)
        LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_SS1_HSIOM_REG,
                                       LIN_1_SCB_SS1_HSIOM_MASK,
                                       LIN_1_SCB_SS1_HSIOM_POS,
                                       hsiomSel[LIN_1_SCB_SS1_PIN_INDEX]);

        LIN_1_SCB_spi_ss1_SetDriveMode((uint8) pinsDm[LIN_1_SCB_SS1_PIN_INDEX]);

        LIN_1_SCB_SET_INP_DIS(LIN_1_SCB_spi_ss1_INP_DIS,
                                     LIN_1_SCB_spi_ss1_MASK,
                                     (0u != (pinsInBuf & LIN_1_SCB_SS1_PIN_MASK)));
    #endif /* (LIN_1_SCB_SS1_PIN) */

    #if (LIN_1_SCB_SS2_PIN)
        LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_SS2_HSIOM_REG,
                                       LIN_1_SCB_SS2_HSIOM_MASK,
                                       LIN_1_SCB_SS2_HSIOM_POS,
                                       hsiomSel[LIN_1_SCB_SS2_PIN_INDEX]);

        LIN_1_SCB_spi_ss2_SetDriveMode((uint8) pinsDm[LIN_1_SCB_SS2_PIN_INDEX]);

        LIN_1_SCB_SET_INP_DIS(LIN_1_SCB_spi_ss2_INP_DIS,
                                     LIN_1_SCB_spi_ss2_MASK,
                                     (0u != (pinsInBuf & LIN_1_SCB_SS2_PIN_MASK)));
    #endif /* (LIN_1_SCB_SS2_PIN) */

    #if (LIN_1_SCB_SS3_PIN)
        LIN_1_SCB_SET_HSIOM_SEL(LIN_1_SCB_SS3_HSIOM_REG,
                                       LIN_1_SCB_SS3_HSIOM_MASK,
                                       LIN_1_SCB_SS3_HSIOM_POS,
                                       hsiomSel[LIN_1_SCB_SS3_PIN_INDEX]);

        LIN_1_SCB_spi_ss3_SetDriveMode((uint8) pinsDm[LIN_1_SCB_SS3_PIN_INDEX]);

        LIN_1_SCB_SET_INP_DIS(LIN_1_SCB_spi_ss3_INP_DIS,
                                     LIN_1_SCB_spi_ss3_MASK,
                                     (0u != (pinsInBuf & LIN_1_SCB_SS3_PIN_MASK)));
    #endif /* (LIN_1_SCB_SS3_PIN) */
    }

#endif /* (LIN_1_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: LIN_1_SCB_I2CSlaveNackGeneration
    ****************************************************************************//**
    *
    *  Sets command to generate NACK to the address or data.
    *
    *******************************************************************************/
    void LIN_1_SCB_I2CSlaveNackGeneration(void)
    {
        /* Check for EC_AM toggle condition: EC_AM and clock stretching for address are enabled */
        if ((0u != (LIN_1_SCB_CTRL_REG & LIN_1_SCB_CTRL_EC_AM_MODE)) &&
            (0u == (LIN_1_SCB_I2C_CTRL_REG & LIN_1_SCB_I2C_CTRL_S_NOT_READY_ADDR_NACK)))
        {
            /* Toggle EC_AM before NACK generation */
            LIN_1_SCB_CTRL_REG &= ~LIN_1_SCB_CTRL_EC_AM_MODE;
            LIN_1_SCB_CTRL_REG |=  LIN_1_SCB_CTRL_EC_AM_MODE;
        }

        LIN_1_SCB_I2C_SLAVE_CMD_REG = LIN_1_SCB_I2C_SLAVE_CMD_S_NACK;
    }
#endif /* (LIN_1_SCB_CY_SCBIP_V0 || LIN_1_SCB_CY_SCBIP_V1) */


/* [] END OF FILE */
