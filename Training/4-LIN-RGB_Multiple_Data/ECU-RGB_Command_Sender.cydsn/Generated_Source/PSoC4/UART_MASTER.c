/***************************************************************************//**
* \file UART_MASTER.c
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

#include "UART_MASTER_PVT.h"

#if (UART_MASTER_SCB_MODE_I2C_INC)
    #include "UART_MASTER_I2C_PVT.h"
#endif /* (UART_MASTER_SCB_MODE_I2C_INC) */

#if (UART_MASTER_SCB_MODE_EZI2C_INC)
    #include "UART_MASTER_EZI2C_PVT.h"
#endif /* (UART_MASTER_SCB_MODE_EZI2C_INC) */

#if (UART_MASTER_SCB_MODE_SPI_INC || UART_MASTER_SCB_MODE_UART_INC)
    #include "UART_MASTER_SPI_UART_PVT.h"
#endif /* (UART_MASTER_SCB_MODE_SPI_INC || UART_MASTER_SCB_MODE_UART_INC) */


/***************************************
*    Run Time Configuration Vars
***************************************/

/* Stores internal component configuration for Unconfigured mode */
#if (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    uint8 UART_MASTER_scbMode = UART_MASTER_SCB_MODE_UNCONFIG;
    uint8 UART_MASTER_scbEnableWake;
    uint8 UART_MASTER_scbEnableIntr;

    /* I2C configuration variables */
    uint8 UART_MASTER_mode;
    uint8 UART_MASTER_acceptAddr;

    /* SPI/UART configuration variables */
    volatile uint8 * UART_MASTER_rxBuffer;
    uint8  UART_MASTER_rxDataBits;
    uint32 UART_MASTER_rxBufferSize;

    volatile uint8 * UART_MASTER_txBuffer;
    uint8  UART_MASTER_txDataBits;
    uint32 UART_MASTER_txBufferSize;

    /* EZI2C configuration variables */
    uint8 UART_MASTER_numberOfAddr;
    uint8 UART_MASTER_subAddrSize;
#endif /* (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Common SCB Vars
***************************************/
/**
* \addtogroup group_general
* \{
*/

/** UART_MASTER_initVar indicates whether the UART_MASTER 
*  component has been initialized. The variable is initialized to 0 
*  and set to 1 the first time SCB_Start() is called. This allows 
*  the component to restart without reinitialization after the first 
*  call to the UART_MASTER_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  UART_MASTER_Init() function can be called before the 
*  UART_MASTER_Start() or UART_MASTER_Enable() function.
*/
uint8 UART_MASTER_initVar = 0u;


#if (! (UART_MASTER_SCB_MODE_I2C_CONST_CFG || \
        UART_MASTER_SCB_MODE_EZI2C_CONST_CFG))
    /** This global variable stores TX interrupt sources after 
    * UART_MASTER_Stop() is called. Only these TX interrupt sources 
    * will be restored on a subsequent UART_MASTER_Enable() call.
    */
    uint16 UART_MASTER_IntrTxMask = 0u;
#endif /* (! (UART_MASTER_SCB_MODE_I2C_CONST_CFG || \
              UART_MASTER_SCB_MODE_EZI2C_CONST_CFG)) */
/** \} globals */

#if (UART_MASTER_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_UART_MASTER_CUSTOM_INTR_HANDLER)
    void (*UART_MASTER_customIntrHandler)(void) = NULL;
#endif /* !defined (CY_REMOVE_UART_MASTER_CUSTOM_INTR_HANDLER) */
#endif /* (UART_MASTER_SCB_IRQ_INTERNAL) */


/***************************************
*    Private Function Prototypes
***************************************/

static void UART_MASTER_ScbEnableIntr(void);
static void UART_MASTER_ScbModeStop(void);
static void UART_MASTER_ScbModePostEnable(void);


/*******************************************************************************
* Function Name: UART_MASTER_Init
****************************************************************************//**
*
*  Initializes the UART_MASTER component to operate in one of the selected
*  configurations: I2C, SPI, UART or EZI2C.
*  When the configuration is set to "Unconfigured SCB", this function does
*  not do any initialization. Use mode-specific initialization APIs instead:
*  UART_MASTER_I2CInit, UART_MASTER_SpiInit, 
*  UART_MASTER_UartInit or UART_MASTER_EzI2CInit.
*
*******************************************************************************/
void UART_MASTER_Init(void)
{
#if (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG)
    if (UART_MASTER_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        UART_MASTER_initVar = 0u;
    }
    else
    {
        /* Initialization was done before this function call */
    }

#elif (UART_MASTER_SCB_MODE_I2C_CONST_CFG)
    UART_MASTER_I2CInit();

#elif (UART_MASTER_SCB_MODE_SPI_CONST_CFG)
    UART_MASTER_SpiInit();

#elif (UART_MASTER_SCB_MODE_UART_CONST_CFG)
    UART_MASTER_UartInit();

#elif (UART_MASTER_SCB_MODE_EZI2C_CONST_CFG)
    UART_MASTER_EzI2CInit();

#endif /* (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: UART_MASTER_Enable
****************************************************************************//**
*
*  Enables UART_MASTER component operation: activates the hardware and 
*  internal interrupt. It also restores TX interrupt sources disabled after the 
*  UART_MASTER_Stop() function was called (note that level-triggered TX 
*  interrupt sources remain disabled to not cause code lock-up).
*  For I2C and EZI2C modes the interrupt is internal and mandatory for 
*  operation. For SPI and UART modes the interrupt can be configured as none, 
*  internal or external.
*  The UART_MASTER configuration should be not changed when the component
*  is enabled. Any configuration changes should be made after disabling the 
*  component.
*  When configuration is set to “Unconfigured UART_MASTER”, the component 
*  must first be initialized to operate in one of the following configurations: 
*  I2C, SPI, UART or EZ I2C, using the mode-specific initialization API. 
*  Otherwise this function does not enable the component.
*
*******************************************************************************/
void UART_MASTER_Enable(void)
{
#if (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Enable SCB block, only if it is already configured */
    if (!UART_MASTER_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        UART_MASTER_CTRL_REG |= UART_MASTER_CTRL_ENABLED;

        UART_MASTER_ScbEnableIntr();

        /* Call PostEnable function specific to current operation mode */
        UART_MASTER_ScbModePostEnable();
    }
#else
    UART_MASTER_CTRL_REG |= UART_MASTER_CTRL_ENABLED;

    UART_MASTER_ScbEnableIntr();

    /* Call PostEnable function specific to current operation mode */
    UART_MASTER_ScbModePostEnable();
#endif /* (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: UART_MASTER_Start
****************************************************************************//**
*
*  Invokes UART_MASTER_Init() and UART_MASTER_Enable().
*  After this function call, the component is enabled and ready for operation.
*  When configuration is set to "Unconfigured SCB", the component must first be
*  initialized to operate in one of the following configurations: I2C, SPI, UART
*  or EZI2C. Otherwise this function does not enable the component.
*
* \globalvars
*  UART_MASTER_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void UART_MASTER_Start(void)
{
    if (0u == UART_MASTER_initVar)
    {
        UART_MASTER_Init();
        UART_MASTER_initVar = 1u; /* Component was initialized */
    }

    UART_MASTER_Enable();
}


/*******************************************************************************
* Function Name: UART_MASTER_Stop
****************************************************************************//**
*
*  Disables the UART_MASTER component: disable the hardware and internal 
*  interrupt. It also disables all TX interrupt sources so as not to cause an 
*  unexpected interrupt trigger because after the component is enabled, the 
*  TX FIFO is empty.
*  Refer to the function UART_MASTER_Enable() for the interrupt 
*  configuration details.
*  This function disables the SCB component without checking to see if 
*  communication is in progress. Before calling this function it may be 
*  necessary to check the status of communication to make sure communication 
*  is complete. If this is not done then communication could be stopped mid 
*  byte and corrupted data could result.
*
*******************************************************************************/
void UART_MASTER_Stop(void)
{
#if (UART_MASTER_SCB_IRQ_INTERNAL)
    UART_MASTER_DisableInt();
#endif /* (UART_MASTER_SCB_IRQ_INTERNAL) */

    /* Call Stop function specific to current operation mode */
    UART_MASTER_ScbModeStop();

    /* Disable SCB IP */
    UART_MASTER_CTRL_REG &= (uint32) ~UART_MASTER_CTRL_ENABLED;

    /* Disable all TX interrupt sources so as not to cause an unexpected
    * interrupt trigger after the component will be enabled because the 
    * TX FIFO is empty.
    * For SCB IP v0, it is critical as it does not mask-out interrupt
    * sources when it is disabled. This can cause a code lock-up in the
    * interrupt handler because TX FIFO cannot be loaded after the block
    * is disabled.
    */
    UART_MASTER_SetTxInterruptMode(UART_MASTER_NO_INTR_SOURCES);

#if (UART_MASTER_SCB_IRQ_INTERNAL)
    UART_MASTER_ClearPendingInt();
#endif /* (UART_MASTER_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: UART_MASTER_SetRxFifoLevel
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
void UART_MASTER_SetRxFifoLevel(uint32 level)
{
    uint32 rxFifoCtrl;

    rxFifoCtrl = UART_MASTER_RX_FIFO_CTRL_REG;

    rxFifoCtrl &= ((uint32) ~UART_MASTER_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    rxFifoCtrl |= ((uint32) (UART_MASTER_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    UART_MASTER_RX_FIFO_CTRL_REG = rxFifoCtrl;
}


/*******************************************************************************
* Function Name: UART_MASTER_SetTxFifoLevel
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
void UART_MASTER_SetTxFifoLevel(uint32 level)
{
    uint32 txFifoCtrl;

    txFifoCtrl = UART_MASTER_TX_FIFO_CTRL_REG;

    txFifoCtrl &= ((uint32) ~UART_MASTER_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    txFifoCtrl |= ((uint32) (UART_MASTER_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    UART_MASTER_TX_FIFO_CTRL_REG = txFifoCtrl;
}


#if (UART_MASTER_SCB_IRQ_INTERNAL)
    /*******************************************************************************
    * Function Name: UART_MASTER_SetCustomInterruptHandler
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
    void UART_MASTER_SetCustomInterruptHandler(void (*func)(void))
    {
    #if !defined (CY_REMOVE_UART_MASTER_CUSTOM_INTR_HANDLER)
        UART_MASTER_customIntrHandler = func; /* Register interrupt handler */
    #else
        if (NULL != func)
        {
            /* Suppress compiler warning */
        }
    #endif /* !defined (CY_REMOVE_UART_MASTER_CUSTOM_INTR_HANDLER) */
    }
#endif /* (UART_MASTER_SCB_IRQ_INTERNAL) */


/*******************************************************************************
* Function Name: UART_MASTER_ScbModeEnableIntr
****************************************************************************//**
*
*  Enables an interrupt for a specific mode.
*
*******************************************************************************/
static void UART_MASTER_ScbEnableIntr(void)
{
#if (UART_MASTER_SCB_IRQ_INTERNAL)
    /* Enable interrupt in NVIC */
    #if (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG)
        if (0u != UART_MASTER_scbEnableIntr)
        {
            UART_MASTER_EnableInt();
        }

    #else
        UART_MASTER_EnableInt();

    #endif /* (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (UART_MASTER_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: UART_MASTER_ScbModePostEnable
****************************************************************************//**
*
*  Calls the PostEnable function for a specific operation mode.
*
*******************************************************************************/
static void UART_MASTER_ScbModePostEnable(void)
{
#if (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG)
#if (!UART_MASTER_CY_SCBIP_V1)
    if (UART_MASTER_SCB_MODE_SPI_RUNTM_CFG)
    {
        UART_MASTER_SpiPostEnable();
    }
    else if (UART_MASTER_SCB_MODE_UART_RUNTM_CFG)
    {
        UART_MASTER_UartPostEnable();
    }
    else
    {
        /* Unknown mode: do nothing */
    }
#endif /* (!UART_MASTER_CY_SCBIP_V1) */

#elif (UART_MASTER_SCB_MODE_SPI_CONST_CFG)
    UART_MASTER_SpiPostEnable();

#elif (UART_MASTER_SCB_MODE_UART_CONST_CFG)
    UART_MASTER_UartPostEnable();

#else
    /* Unknown mode: do nothing */
#endif /* (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: UART_MASTER_ScbModeStop
****************************************************************************//**
*
*  Calls the Stop function for a specific operation mode.
*
*******************************************************************************/
static void UART_MASTER_ScbModeStop(void)
{
#if (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG)
    if (UART_MASTER_SCB_MODE_I2C_RUNTM_CFG)
    {
        UART_MASTER_I2CStop();
    }
    else if (UART_MASTER_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        UART_MASTER_EzI2CStop();
    }
#if (!UART_MASTER_CY_SCBIP_V1)
    else if (UART_MASTER_SCB_MODE_SPI_RUNTM_CFG)
    {
        UART_MASTER_SpiStop();
    }
    else if (UART_MASTER_SCB_MODE_UART_RUNTM_CFG)
    {
        UART_MASTER_UartStop();
    }
#endif /* (!UART_MASTER_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
#elif (UART_MASTER_SCB_MODE_I2C_CONST_CFG)
    UART_MASTER_I2CStop();

#elif (UART_MASTER_SCB_MODE_EZI2C_CONST_CFG)
    UART_MASTER_EzI2CStop();

#elif (UART_MASTER_SCB_MODE_SPI_CONST_CFG)
    UART_MASTER_SpiStop();

#elif (UART_MASTER_SCB_MODE_UART_CONST_CFG)
    UART_MASTER_UartStop();

#else
    /* Unknown mode: do nothing */
#endif /* (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: UART_MASTER_SetPins
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
    void UART_MASTER_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask)
    {
        uint32 pinsDm[UART_MASTER_SCB_PINS_NUMBER];
        uint32 i;
        
    #if (!UART_MASTER_CY_SCBIP_V1)
        uint32 pinsInBuf = 0u;
    #endif /* (!UART_MASTER_CY_SCBIP_V1) */
        
        uint32 hsiomSel[UART_MASTER_SCB_PINS_NUMBER] = 
        {
            UART_MASTER_RX_SCL_MOSI_HSIOM_SEL_GPIO,
            UART_MASTER_TX_SDA_MISO_HSIOM_SEL_GPIO,
            0u,
            0u,
            0u,
            0u,
            0u,
        };

    #if (UART_MASTER_CY_SCBIP_V1)
        /* Supress compiler warning. */
        if ((0u == subMode) || (0u == uartEnableMask))
        {
        }
    #endif /* (UART_MASTER_CY_SCBIP_V1) */

        /* Set default HSIOM to GPIO and Drive Mode to Analog Hi-Z */
        for (i = 0u; i < UART_MASTER_SCB_PINS_NUMBER; i++)
        {
            pinsDm[i] = UART_MASTER_PIN_DM_ALG_HIZ;
        }

        if ((UART_MASTER_SCB_MODE_I2C   == mode) ||
            (UART_MASTER_SCB_MODE_EZI2C == mode))
        {
        #if (UART_MASTER_RX_SCL_MOSI_PIN)
            hsiomSel[UART_MASTER_RX_SCL_MOSI_PIN_INDEX] = UART_MASTER_RX_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [UART_MASTER_RX_SCL_MOSI_PIN_INDEX] = UART_MASTER_PIN_DM_OD_LO;
        #elif (UART_MASTER_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[UART_MASTER_RX_WAKE_SCL_MOSI_PIN_INDEX] = UART_MASTER_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [UART_MASTER_RX_WAKE_SCL_MOSI_PIN_INDEX] = UART_MASTER_PIN_DM_OD_LO;
        #else
        #endif /* (UART_MASTER_RX_SCL_MOSI_PIN) */
        
        #if (UART_MASTER_TX_SDA_MISO_PIN)
            hsiomSel[UART_MASTER_TX_SDA_MISO_PIN_INDEX] = UART_MASTER_TX_SDA_MISO_HSIOM_SEL_I2C;
            pinsDm  [UART_MASTER_TX_SDA_MISO_PIN_INDEX] = UART_MASTER_PIN_DM_OD_LO;
        #endif /* (UART_MASTER_TX_SDA_MISO_PIN) */
        }
    #if (!UART_MASTER_CY_SCBIP_V1)
        else if (UART_MASTER_SCB_MODE_SPI == mode)
        {
        #if (UART_MASTER_RX_SCL_MOSI_PIN)
            hsiomSel[UART_MASTER_RX_SCL_MOSI_PIN_INDEX] = UART_MASTER_RX_SCL_MOSI_HSIOM_SEL_SPI;
        #elif (UART_MASTER_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[UART_MASTER_RX_WAKE_SCL_MOSI_PIN_INDEX] = UART_MASTER_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI;
        #else
        #endif /* (UART_MASTER_RX_SCL_MOSI_PIN) */
        
        #if (UART_MASTER_TX_SDA_MISO_PIN)
            hsiomSel[UART_MASTER_TX_SDA_MISO_PIN_INDEX] = UART_MASTER_TX_SDA_MISO_HSIOM_SEL_SPI;
        #endif /* (UART_MASTER_TX_SDA_MISO_PIN) */
        
        #if (UART_MASTER_SCLK_PIN)
            hsiomSel[UART_MASTER_SCLK_PIN_INDEX] = UART_MASTER_SCLK_HSIOM_SEL_SPI;
        #endif /* (UART_MASTER_SCLK_PIN) */

            if (UART_MASTER_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[UART_MASTER_RX_SCL_MOSI_PIN_INDEX] = UART_MASTER_PIN_DM_DIG_HIZ;
                pinsDm[UART_MASTER_TX_SDA_MISO_PIN_INDEX] = UART_MASTER_PIN_DM_STRONG;
                pinsDm[UART_MASTER_SCLK_PIN_INDEX] = UART_MASTER_PIN_DM_DIG_HIZ;

            #if (UART_MASTER_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[UART_MASTER_SS0_PIN_INDEX] = UART_MASTER_SS0_HSIOM_SEL_SPI;
                pinsDm  [UART_MASTER_SS0_PIN_INDEX] = UART_MASTER_PIN_DM_DIG_HIZ;
            #endif /* (UART_MASTER_SS0_PIN) */

            #if (UART_MASTER_TX_SDA_MISO_PIN)
                /* Disable input buffer */
                 pinsInBuf |= UART_MASTER_TX_SDA_MISO_PIN_MASK;
            #endif /* (UART_MASTER_TX_SDA_MISO_PIN) */
            }
            else 
            {
                /* (Master) */
                pinsDm[UART_MASTER_RX_SCL_MOSI_PIN_INDEX] = UART_MASTER_PIN_DM_STRONG;
                pinsDm[UART_MASTER_TX_SDA_MISO_PIN_INDEX] = UART_MASTER_PIN_DM_DIG_HIZ;
                pinsDm[UART_MASTER_SCLK_PIN_INDEX] = UART_MASTER_PIN_DM_STRONG;

            #if (UART_MASTER_SS0_PIN)
                hsiomSel [UART_MASTER_SS0_PIN_INDEX] = UART_MASTER_SS0_HSIOM_SEL_SPI;
                pinsDm   [UART_MASTER_SS0_PIN_INDEX] = UART_MASTER_PIN_DM_STRONG;
                pinsInBuf |= UART_MASTER_SS0_PIN_MASK;
            #endif /* (UART_MASTER_SS0_PIN) */

            #if (UART_MASTER_SS1_PIN)
                hsiomSel [UART_MASTER_SS1_PIN_INDEX] = UART_MASTER_SS1_HSIOM_SEL_SPI;
                pinsDm   [UART_MASTER_SS1_PIN_INDEX] = UART_MASTER_PIN_DM_STRONG;
                pinsInBuf |= UART_MASTER_SS1_PIN_MASK;
            #endif /* (UART_MASTER_SS1_PIN) */

            #if (UART_MASTER_SS2_PIN)
                hsiomSel [UART_MASTER_SS2_PIN_INDEX] = UART_MASTER_SS2_HSIOM_SEL_SPI;
                pinsDm   [UART_MASTER_SS2_PIN_INDEX] = UART_MASTER_PIN_DM_STRONG;
                pinsInBuf |= UART_MASTER_SS2_PIN_MASK;
            #endif /* (UART_MASTER_SS2_PIN) */

            #if (UART_MASTER_SS3_PIN)
                hsiomSel [UART_MASTER_SS3_PIN_INDEX] = UART_MASTER_SS3_HSIOM_SEL_SPI;
                pinsDm   [UART_MASTER_SS3_PIN_INDEX] = UART_MASTER_PIN_DM_STRONG;
                pinsInBuf |= UART_MASTER_SS3_PIN_MASK;
            #endif /* (UART_MASTER_SS3_PIN) */

                /* Disable input buffers */
            #if (UART_MASTER_RX_SCL_MOSI_PIN)
                pinsInBuf |= UART_MASTER_RX_SCL_MOSI_PIN_MASK;
            #elif (UART_MASTER_RX_WAKE_SCL_MOSI_PIN)
                pinsInBuf |= UART_MASTER_RX_WAKE_SCL_MOSI_PIN_MASK;
            #else
            #endif /* (UART_MASTER_RX_SCL_MOSI_PIN) */

            #if (UART_MASTER_SCLK_PIN)
                pinsInBuf |= UART_MASTER_SCLK_PIN_MASK;
            #endif /* (UART_MASTER_SCLK_PIN) */
            }
        }
        else /* UART */
        {
            if (UART_MASTER_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
            #if (UART_MASTER_TX_SDA_MISO_PIN)
                hsiomSel[UART_MASTER_TX_SDA_MISO_PIN_INDEX] = UART_MASTER_TX_SDA_MISO_HSIOM_SEL_UART;
                pinsDm  [UART_MASTER_TX_SDA_MISO_PIN_INDEX] = UART_MASTER_PIN_DM_OD_LO;
            #endif /* (UART_MASTER_TX_SDA_MISO_PIN) */
            }
            else /* Standard or IrDA */
            {
                if (0u != (UART_MASTER_UART_RX_PIN_ENABLE & uartEnableMask))
                {
                #if (UART_MASTER_RX_SCL_MOSI_PIN)
                    hsiomSel[UART_MASTER_RX_SCL_MOSI_PIN_INDEX] = UART_MASTER_RX_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [UART_MASTER_RX_SCL_MOSI_PIN_INDEX] = UART_MASTER_PIN_DM_DIG_HIZ;
                #elif (UART_MASTER_RX_WAKE_SCL_MOSI_PIN)
                    hsiomSel[UART_MASTER_RX_WAKE_SCL_MOSI_PIN_INDEX] = UART_MASTER_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [UART_MASTER_RX_WAKE_SCL_MOSI_PIN_INDEX] = UART_MASTER_PIN_DM_DIG_HIZ;
                #else
                #endif /* (UART_MASTER_RX_SCL_MOSI_PIN) */
                }

                if (0u != (UART_MASTER_UART_TX_PIN_ENABLE & uartEnableMask))
                {
                #if (UART_MASTER_TX_SDA_MISO_PIN)
                    hsiomSel[UART_MASTER_TX_SDA_MISO_PIN_INDEX] = UART_MASTER_TX_SDA_MISO_HSIOM_SEL_UART;
                    pinsDm  [UART_MASTER_TX_SDA_MISO_PIN_INDEX] = UART_MASTER_PIN_DM_STRONG;
                    
                    /* Disable input buffer */
                    pinsInBuf |= UART_MASTER_TX_SDA_MISO_PIN_MASK;
                #endif /* (UART_MASTER_TX_SDA_MISO_PIN) */
                }

            #if !(UART_MASTER_CY_SCBIP_V0 || UART_MASTER_CY_SCBIP_V1)
                if (UART_MASTER_UART_MODE_STD == subMode)
                {
                    if (0u != (UART_MASTER_UART_CTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* CTS input is multiplexed with SCLK */
                    #if (UART_MASTER_SCLK_PIN)
                        hsiomSel[UART_MASTER_SCLK_PIN_INDEX] = UART_MASTER_SCLK_HSIOM_SEL_UART;
                        pinsDm  [UART_MASTER_SCLK_PIN_INDEX] = UART_MASTER_PIN_DM_DIG_HIZ;
                    #endif /* (UART_MASTER_SCLK_PIN) */
                    }

                    if (0u != (UART_MASTER_UART_RTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* RTS output is multiplexed with SS0 */
                    #if (UART_MASTER_SS0_PIN)
                        hsiomSel[UART_MASTER_SS0_PIN_INDEX] = UART_MASTER_SS0_HSIOM_SEL_UART;
                        pinsDm  [UART_MASTER_SS0_PIN_INDEX] = UART_MASTER_PIN_DM_STRONG;
                        
                        /* Disable input buffer */
                        pinsInBuf |= UART_MASTER_SS0_PIN_MASK;
                    #endif /* (UART_MASTER_SS0_PIN) */
                    }
                }
            #endif /* !(UART_MASTER_CY_SCBIP_V0 || UART_MASTER_CY_SCBIP_V1) */
            }
        }
    #endif /* (!UART_MASTER_CY_SCBIP_V1) */

    /* Configure pins: set HSIOM, DM and InputBufEnable */
    /* Note: the DR register settings do not effect the pin output if HSIOM is other than GPIO */

    #if (UART_MASTER_RX_SCL_MOSI_PIN)
        UART_MASTER_SET_HSIOM_SEL(UART_MASTER_RX_SCL_MOSI_HSIOM_REG,
                                       UART_MASTER_RX_SCL_MOSI_HSIOM_MASK,
                                       UART_MASTER_RX_SCL_MOSI_HSIOM_POS,
                                        hsiomSel[UART_MASTER_RX_SCL_MOSI_PIN_INDEX]);

        UART_MASTER_uart_rx_i2c_scl_spi_mosi_SetDriveMode((uint8) pinsDm[UART_MASTER_RX_SCL_MOSI_PIN_INDEX]);

        #if (!UART_MASTER_CY_SCBIP_V1)
            UART_MASTER_SET_INP_DIS(UART_MASTER_uart_rx_i2c_scl_spi_mosi_INP_DIS,
                                         UART_MASTER_uart_rx_i2c_scl_spi_mosi_MASK,
                                         (0u != (pinsInBuf & UART_MASTER_RX_SCL_MOSI_PIN_MASK)));
        #endif /* (!UART_MASTER_CY_SCBIP_V1) */
    
    #elif (UART_MASTER_RX_WAKE_SCL_MOSI_PIN)
        UART_MASTER_SET_HSIOM_SEL(UART_MASTER_RX_WAKE_SCL_MOSI_HSIOM_REG,
                                       UART_MASTER_RX_WAKE_SCL_MOSI_HSIOM_MASK,
                                       UART_MASTER_RX_WAKE_SCL_MOSI_HSIOM_POS,
                                       hsiomSel[UART_MASTER_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        UART_MASTER_uart_rx_wake_i2c_scl_spi_mosi_SetDriveMode((uint8)
                                                               pinsDm[UART_MASTER_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        UART_MASTER_SET_INP_DIS(UART_MASTER_uart_rx_wake_i2c_scl_spi_mosi_INP_DIS,
                                     UART_MASTER_uart_rx_wake_i2c_scl_spi_mosi_MASK,
                                     (0u != (pinsInBuf & UART_MASTER_RX_WAKE_SCL_MOSI_PIN_MASK)));

         /* Set interrupt on falling edge */
        UART_MASTER_SET_INCFG_TYPE(UART_MASTER_RX_WAKE_SCL_MOSI_INTCFG_REG,
                                        UART_MASTER_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK,
                                        UART_MASTER_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS,
                                        UART_MASTER_INTCFG_TYPE_FALLING_EDGE);
    #else
    #endif /* (UART_MASTER_RX_WAKE_SCL_MOSI_PIN) */

    #if (UART_MASTER_TX_SDA_MISO_PIN)
        UART_MASTER_SET_HSIOM_SEL(UART_MASTER_TX_SDA_MISO_HSIOM_REG,
                                       UART_MASTER_TX_SDA_MISO_HSIOM_MASK,
                                       UART_MASTER_TX_SDA_MISO_HSIOM_POS,
                                        hsiomSel[UART_MASTER_TX_SDA_MISO_PIN_INDEX]);

        UART_MASTER_uart_tx_i2c_sda_spi_miso_SetDriveMode((uint8) pinsDm[UART_MASTER_TX_SDA_MISO_PIN_INDEX]);

    #if (!UART_MASTER_CY_SCBIP_V1)
        UART_MASTER_SET_INP_DIS(UART_MASTER_uart_tx_i2c_sda_spi_miso_INP_DIS,
                                     UART_MASTER_uart_tx_i2c_sda_spi_miso_MASK,
                                    (0u != (pinsInBuf & UART_MASTER_TX_SDA_MISO_PIN_MASK)));
    #endif /* (!UART_MASTER_CY_SCBIP_V1) */
    #endif /* (UART_MASTER_RX_SCL_MOSI_PIN) */

    #if (UART_MASTER_SCLK_PIN)
        UART_MASTER_SET_HSIOM_SEL(UART_MASTER_SCLK_HSIOM_REG,
                                       UART_MASTER_SCLK_HSIOM_MASK,
                                       UART_MASTER_SCLK_HSIOM_POS,
                                       hsiomSel[UART_MASTER_SCLK_PIN_INDEX]);

        UART_MASTER_spi_sclk_SetDriveMode((uint8) pinsDm[UART_MASTER_SCLK_PIN_INDEX]);

        UART_MASTER_SET_INP_DIS(UART_MASTER_spi_sclk_INP_DIS,
                                     UART_MASTER_spi_sclk_MASK,
                                     (0u != (pinsInBuf & UART_MASTER_SCLK_PIN_MASK)));
    #endif /* (UART_MASTER_SCLK_PIN) */

    #if (UART_MASTER_SS0_PIN)
        UART_MASTER_SET_HSIOM_SEL(UART_MASTER_SS0_HSIOM_REG,
                                       UART_MASTER_SS0_HSIOM_MASK,
                                       UART_MASTER_SS0_HSIOM_POS,
                                       hsiomSel[UART_MASTER_SS0_PIN_INDEX]);

        UART_MASTER_spi_ss0_SetDriveMode((uint8) pinsDm[UART_MASTER_SS0_PIN_INDEX]);

        UART_MASTER_SET_INP_DIS(UART_MASTER_spi_ss0_INP_DIS,
                                     UART_MASTER_spi_ss0_MASK,
                                     (0u != (pinsInBuf & UART_MASTER_SS0_PIN_MASK)));
    #endif /* (UART_MASTER_SS0_PIN) */

    #if (UART_MASTER_SS1_PIN)
        UART_MASTER_SET_HSIOM_SEL(UART_MASTER_SS1_HSIOM_REG,
                                       UART_MASTER_SS1_HSIOM_MASK,
                                       UART_MASTER_SS1_HSIOM_POS,
                                       hsiomSel[UART_MASTER_SS1_PIN_INDEX]);

        UART_MASTER_spi_ss1_SetDriveMode((uint8) pinsDm[UART_MASTER_SS1_PIN_INDEX]);

        UART_MASTER_SET_INP_DIS(UART_MASTER_spi_ss1_INP_DIS,
                                     UART_MASTER_spi_ss1_MASK,
                                     (0u != (pinsInBuf & UART_MASTER_SS1_PIN_MASK)));
    #endif /* (UART_MASTER_SS1_PIN) */

    #if (UART_MASTER_SS2_PIN)
        UART_MASTER_SET_HSIOM_SEL(UART_MASTER_SS2_HSIOM_REG,
                                       UART_MASTER_SS2_HSIOM_MASK,
                                       UART_MASTER_SS2_HSIOM_POS,
                                       hsiomSel[UART_MASTER_SS2_PIN_INDEX]);

        UART_MASTER_spi_ss2_SetDriveMode((uint8) pinsDm[UART_MASTER_SS2_PIN_INDEX]);

        UART_MASTER_SET_INP_DIS(UART_MASTER_spi_ss2_INP_DIS,
                                     UART_MASTER_spi_ss2_MASK,
                                     (0u != (pinsInBuf & UART_MASTER_SS2_PIN_MASK)));
    #endif /* (UART_MASTER_SS2_PIN) */

    #if (UART_MASTER_SS3_PIN)
        UART_MASTER_SET_HSIOM_SEL(UART_MASTER_SS3_HSIOM_REG,
                                       UART_MASTER_SS3_HSIOM_MASK,
                                       UART_MASTER_SS3_HSIOM_POS,
                                       hsiomSel[UART_MASTER_SS3_PIN_INDEX]);

        UART_MASTER_spi_ss3_SetDriveMode((uint8) pinsDm[UART_MASTER_SS3_PIN_INDEX]);

        UART_MASTER_SET_INP_DIS(UART_MASTER_spi_ss3_INP_DIS,
                                     UART_MASTER_spi_ss3_MASK,
                                     (0u != (pinsInBuf & UART_MASTER_SS3_PIN_MASK)));
    #endif /* (UART_MASTER_SS3_PIN) */
    }

#endif /* (UART_MASTER_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (UART_MASTER_CY_SCBIP_V0 || UART_MASTER_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: UART_MASTER_I2CSlaveNackGeneration
    ****************************************************************************//**
    *
    *  Sets command to generate NACK to the address or data.
    *
    *******************************************************************************/
    void UART_MASTER_I2CSlaveNackGeneration(void)
    {
        /* Check for EC_AM toggle condition: EC_AM and clock stretching for address are enabled */
        if ((0u != (UART_MASTER_CTRL_REG & UART_MASTER_CTRL_EC_AM_MODE)) &&
            (0u == (UART_MASTER_I2C_CTRL_REG & UART_MASTER_I2C_CTRL_S_NOT_READY_ADDR_NACK)))
        {
            /* Toggle EC_AM before NACK generation */
            UART_MASTER_CTRL_REG &= ~UART_MASTER_CTRL_EC_AM_MODE;
            UART_MASTER_CTRL_REG |=  UART_MASTER_CTRL_EC_AM_MODE;
        }

        UART_MASTER_I2C_SLAVE_CMD_REG = UART_MASTER_I2C_SLAVE_CMD_S_NACK;
    }
#endif /* (UART_MASTER_CY_SCBIP_V0 || UART_MASTER_CY_SCBIP_V1) */


/* [] END OF FILE */
