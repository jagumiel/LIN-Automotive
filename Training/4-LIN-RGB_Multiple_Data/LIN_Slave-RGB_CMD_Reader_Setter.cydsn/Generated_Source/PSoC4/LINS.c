/******************************************************************************
* File Name: LINS.c
* Version 5.0
*
* Description:
*  This file contains the implementation of the LIN Slave component.
*
*******************************************************************************
* Copyright 2011-2019, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LINS.h"
#include "LIN_Dynamic.h"


/* Internal APIs */
static l_u8 LINS_FindPidIndex(l_u8 pid);
static void LINS_EndFrame(l_u8 status);
static void LINS_SetAssociatedFlags(l_u8 pIndex);

#if (LINS_SAE_J2602_ENABLED)
    /* SAE J2602 is enabled */
    static void LINS_j2602Status_Set(l_u8 STS_code);
    static l_u8 LINS_j2602Status_Get(void);
    static void LINS_j2602Status_Clear(void);
    static l_u8 LINS_UpdateMessageIDs(l_u8 newNAD);
#else 
    /* SAE J2602 is disabled */
    static l_bool LINS_GetEtFlagValue(l_u8 pidIndex);
    static void   LINS_ClearEtFlagValue(l_u8 pidIndex);
#endif /* LINS_SAE_J2602_ENABLED */

#if (LINS_TL_USED)
    static l_u8 LINS_TransmitTLFrame(l_u8 stimulus);
    static void LINS_ClrRxBuffer(l_u8 stimulus);
    static void LINS_ClrTxBuffer(l_u8 stimulus);
    static void LINS_CheckNodeState(l_u8 stimulus);
    static l_u8 LINS_ProcessMrf(l_u8 frame[]);

    #if (LINS_CFG_SERVICES_ENABLED)
        #if ((0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB0_SEL)) || \
             (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB2_SEL)))
            static l_bool LINS_LinProductId(volatile const l_u8 frameData[]);
        #endif /* (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB0_SEL)) ||
                  (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB2_SEL)) */

        #if ((0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB1_SEL)) && \
             (1u == LINS_LIN_2_0))
            static l_u8 LINS_MessageId(volatile const l_u8* frameData);
        #endif /* (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB1_SEL)) && 
                  (1u == LINS_LIN_2_0) */
    #endif /* LINS_CFG_SERVICES_ENABLED */

    /* RAM copy of Slave NAD */
    static volatile l_u8 LINS_nad;
#endif /* LINS_TL_USED */

/******************************************************************************
*                            Parity Lookup Table
* A 6-bit identifier is given as an index. 
* The indexed value provides the correct value with the parity bit-set.
******************************************************************************/
static const l_u8 CYCODE LINS_parityTable[] =
{
    0x80u, 0xC1u, 0x42u, 0x03u, 0xC4u, 0x85u, 0x06u, 0x47u, 0x08u, 0x49u, 0xCAu,
    0x8Bu, 0x4Cu, 0x0Du, 0x8Eu, 0xCFu, 0x50u, 0x11u, 0x92u, 0xD3u, 0x14u, 0x55u,
    0xD6u, 0x97u, 0xD8u, 0x99u, 0x1Au, 0x5Bu, 0x9Cu, 0xDDu, 0x5Eu, 0x1Fu, 0x20u,
    0x61u, 0xE2u, 0xA3u, 0x64u, 0x25u, 0xA6u, 0xE7u, 0xA8u, 0xE9u, 0x6Au, 0x2Bu,
    0xECu, 0xADu, 0x2Eu, 0x6Fu, 0xF0u, 0xB1u, 0x32u, 0x73u, 0xB4u, 0xF5u, 0x76u,
    0x37u, 0x78u, 0x39u, 0xBAu, 0xFBu, 0x3Cu, 0x7Du, 0xFEu, 0xBFu
};
#if (1u == LINS_INACTIVITY_ENABLED)
    /* Free-running timer */
    static uint32 LINS_periodCounter;
#endif /* (1u == LINS_INACTIVITY_ENABLED) */

static volatile l_u8  LINS_status;                /* Internal Status                  */
static volatile l_u16 LINS_syncCounts;            /* Sync Field Timer Counts          */
static volatile l_u16 LINS_breakToSyncCounts = 0u;/* Break to Sync timeout counter    */

/* Baud detect counts till LINS_BREAK_TO_SYNC_TIMEOUT */
static volatile l_u16 LINS_breakToSyncCounts1;                                               
static volatile l_u16 LINS_breakToSyncCounts2;   /* Baud detect counts on timeout+1ms*/
static volatile l_u8  LINS_auxStatus;             /* Internal AUX ISR shadow status   */
static volatile l_u16 LINS_ioctlStatus;           /* Status used by l_ifc_ioctl()     */
static volatile l_u16 LINS_ifcStatus;             /* Interface communication status   */
static volatile l_u8  LINS_uartFsmState;          /* Current state of the UART ISR    */
static volatile l_u8  LINS_fsmFlags;

#if (1u == LINS_AUTO_BAUD_RATE_SYNC)
    /* Initial clock divider */
    l_u16 LINS_initialClockDivider; 
#endif /* (1u == LINS_AUTO_BAUD_RATE_SYNC) */

#if (LINS_SAE_J2602_ENABLED)
    /* J2602 status variable 
    * Stores J2602_APINFO[4:0] bits after calling the
    * l_u8_wr() or l_bool_wr() from application, which writes to APINFO. 
    * These bits are saved when updating ERR[7:5] bits from ISR *
    */
    static volatile l_u8 LINS_j2602Status = 0u;
    static volatile l_u8 LINS_j2602StatusStack = 0u;  /* Stores internal set of node ERR states */
#endif  /* LINS_SAE_J2602_ENABLED */

/* Notification API statuses */
static l_u8 LINS_statusFlagArray[LINS_SIG_FRAME_FLAGS_SIZE];

/* SAE J2602 is disabled */
#if ((!LINS_SAE_J2602_ENABLED) && (0u != LINS_NUM_ET_FRAMES))
    static l_u8 LINS_etFrameFlags[LINS_ET_FRAMES_FLAGS_SIZE];
#endif /* (!LINS_SAE_J2602_ENABLED) && (0u != LINS_NUM_ET_FRAMES) */

/* RAM copy of the slave configuration data */
static volatile l_u8 LINS_volatileConfig[LINS_NUM_FRAMES];


/****************************************************
*   Transport Layer API
*****************************************************/
#if (LINS_TL_USED)
    #if (LINS_CFG_SERVICES_ENABLED)
        #if (0u != (LINS_CS_SEL_SERVICES01 & (LINS_NCS_0xB0_SEL | LINS_NCS_0xB1_SEL \
            | LINS_NCS_0xB2_SEL | LINS_NCS_0xB3_SEL)))
            /* LIN Slave Identification */
            static const LINS_SLAVE_ID CYCODE LINS_slaveId =
            {
                LINS_CS_SUPPLIER_ID,
                LINS_CS_FUNCTION_ID,
                LINS_CS_VARIANT
            };
        #endif /*(0u != (LINS_CS_SEL_SERVICES01 & (LINS_NCS_0xB0_SEL
               * | LINS_NCS_0xB1_SEL | LINS_NCS_0xB2_SEL | LINS_NCS_0xB3_SEL))
               */
        /* Serial Number */
        static l_u8* LINS_serialNumber;
    #endif /* LINS_CFG_SERVICES_ENABLED */

    /* MRF buffer */
    static l_u8 LINS_mrfBuffer[LINS_FRAME_BUFF_LEN];

    /* SRF buffer */
    static volatile l_u8 LINS_srfBuffer[LINS_FRAME_BUFF_LEN];

    /* Transport Layer Rx and Tx Statuses */
    static volatile l_u8 LINS_txTlStatus;
    static volatile l_u8 LINS_rxTlStatus;

    /* Flags that are used for Transport Layer */
    static volatile l_u8 LINS_tlFlags;
    static l_u8 LINS_nodeState = LINS_IDLE;

    /* Internal variable used to store the PCI of the previously transmitted frame */
    static l_u8 LINS_txPrevPci;

    /* Internal variable used to store the PCI of the previously received frame */
    static l_u8 LINS_rxPrevPci;

    static volatile l_u16  LINS_txMessageLength = 0u;
    static volatile l_u16  LINS_rxMessageLength = 0u;
    static volatile l_u8   LINS_txFrameCounter  = 0u;
    static volatile l_u8   LINS_rxFrameCounter  = 0u;
    static volatile uint32 LINS_tlTimeoutCnt    = 0u;

    #if (LINS_COOKED_TL_API)
        static const    l_u8*  LINS_txTlDataPointer     = NULL;
        static volatile l_u16  LINS_txTlDataCount;
        static volatile l_u8*  LINS_rxTlDataPointer     = NULL;
        static volatile l_u8*  LINS_rxTlInitDataPointer = NULL;
        static volatile l_u8*  LINS_tlNadPointer        = NULL;
        static volatile l_u16* LINS_tlLengthPointer     = NULL;
    #else
        /* Internal variables for buffer indexing */
        static l_u8 LINS_txBufDepth;
        static l_u8 LINS_rxBufDepth;

        /* The Master Request Frame (MRF) buffer */
        static volatile l_u8 LINS_rawRxQueue[LINS_TL_RX_QUEUE_LEN];

        /* The Slave Response Frame (SRF) buffer */
        static volatile l_u8 LINS_rawTxQueue[LINS_TL_TX_QUEUE_LEN];

        /* TX buffer indexes */
        static volatile l_u16 LINS_txWrIndex;
        static volatile l_u16 LINS_txRdIndex;

        /* RX buffer indexes */
        static volatile l_u16 LINS_rxWrIndex;
        static volatile l_u16 LINS_rxRdIndex;
    #endif /* LINS_COOKED_TL_API */
#endif /* LINS_TL_USED */


/* LIN slave configuration data */
static LINS_SLAVE_CONFIG const CYCODE LINS_LinSlaveConfig = {
    LINS_TL_INITIAL_NAD,

    /* Frame PID Table */
    {0x50u, 0x11u}
};


/* Frames declaration with initial signals values */
static volatile l_u8  LINS_Buffer_InFrame[8u] = {0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu};
static volatile l_u8  LINS_Buffer_OutFrame[8u] = {0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu};


#if (LINS_SAE_J2602_ENABLED)
    /*******************************************************************************
    * Function Name: LINS_j2602Status_Set
    ********************************************************************************
    *
    * Summary:
    *  Sets the J2602 error according to LINS_J2602_STS code.
    *
    *******************************************************************************/
    static void LINS_j2602Status_Set(l_u8 STS_code)
    {
        LINS_j2602StatusStack |= (l_u8)(1u << STS_code);
    }
    
    
    /*******************************************************************************
    * Function Name: LINS_j2602Status_Get
    ********************************************************************************
    *
    * Summary:
    *  Gets the J2602 error with the highest priority.
    *
    * Return:
    *  LINS_J2602_STS code
    *
    *******************************************************************************/
    static l_u8 LINS_j2602Status_Get(void)
    {
        l_u8 priority;
        /* Error flags are scanned from highest (7) to lowest (1) priority */
        for (priority = 7u; priority != 0u; priority--)
        {
            /* If set flag found, its number is returned immediately */
            if (0u != (LINS_j2602StatusStack & (l_u8)(1u << priority)))
            {
                break;
            }
        }
        /* Updates internal J2602_Status variable to be ready for the next reply. */
        LINS_j2602Status &= LINS_J2602_STS_APINFO_MASK;
        LINS_j2602Status |= (l_u8)(priority << 5u);
        
        return (priority); /* result is NOT shifted to ERR bits position */
    }
    
    
    /*******************************************************************************
    * Function Name: LINS_j2602Status_Clear
    ********************************************************************************
    *
    * Summary:
    *  Clears the highest priority J2602 error.
    *
    *******************************************************************************/
    static void LINS_j2602Status_Clear(void)
    {
        l_u8 priority;
        /* Error flags are scanned from highest (7) to lowest (1) priority */
        for (priority = 7u; priority != 0u; priority--)
        {
            /* If set flag found, it is cleared */
            if (0u != (LINS_j2602StatusStack & (l_u8)(1u << priority)))
            {
                LINS_j2602StatusStack &= (l_u8)~((l_u8)(1u << priority));
                break;
            }
        }    
    }
    
    
    /*******************************************************************************
    * Function Name: LINS_j2602Status_Reset
    ********************************************************************************
    *
    * Summary:
    *  Clears all pending errors and sets Reset flag.
    *  Should be called after application reset (B5 Target Reset command received).
    *
    *******************************************************************************/
    void LINS_j2602Status_Reset(void)
    {
        l_u8 interruptState;
        /* Save current global interrupt enable and disable it */
        interruptState = CyEnterCriticalSection();    
        
        LINS_j2602StatusStack = (l_u8)(1u << LINS_J2602_STS_RESET);
        
        /* Restore global interrupt enable state */
        CyExitCriticalSection(interruptState);        
    }
#endif  /* LINS_SAE_J2602_ENABLED */


/*******************************************************************************
* Function Name: LINS_Start
********************************************************************************
*
* Summary:
*  Starts the component operation. This function is not required to use.
*
* Return:
*  Zero     - If the initialization succeeds.
*  Non-zero - If the initialization fails.
*
*******************************************************************************/
l_bool LINS_Start(void)
{
    l_bool returnValue;

    /* Call the initialization function
    *  which do nothing so can be called safely from any instance of API */
    (void) l_sys_init();

    /* Start LIN components */
    returnValue = l_ifc_init_LINS();

    return (returnValue);
}


/******************************************************************************
* Function Name: LINS_Stop
*******************************************************************************
*
* Summary:
*  Stops the component operation. This function is not required to be used.
*
******************************************************************************/
void LINS_Stop(void)
{
    /* Stop UART */
    LINS_SCB_Stop();

    /* Clear bit corresponding to current instance in SysTimerEnabledFlag
     * and stop SySTick timer only if both flags are cleared */

    #if (1u == LINS_INACTIVITY_ENABLED)
        /* Stop SysTick timer if it was enabled before */
        LINS_TimerDisableInt();
    #endif /* (1u == LINS_INACTIVITY_ENABLED) */

    /* Disables interrupts from this instance  */
    LINS_ISR_Disable();
}


/* Signal interaction and notification API */


/*******************************************************************************
* Function Name: l_u8_rd_LINS
********************************************************************************
*
* Summary:
*  Reads and returns the current value of the signal for signals of the size
*  2 - 8 bits. If an invalid signal handle is passed into the function, no
*  action is done.
*
* Parameters:
*  sss - signal handle of the signal to read.
*
* Return:
*  Returns current value of signal.
*
*******************************************************************************/
l_u8 l_u8_rd_LINS(l_signal_handle sss) 
{
    l_u8 returnValue;

    switch(sss)
    {
        case RedValue_SIGNAL_HANDLE:
            returnValue = l_u8_rd_RedValue();
        break;

        case GreenValue_SIGNAL_HANDLE:
            returnValue = l_u8_rd_GreenValue();
        break;

        case BlueValue_SIGNAL_HANDLE:
            returnValue = l_u8_rd_BlueValue();
        break;

        default:
            returnValue = (l_u8) 0xFFu;
        break;
    }

    return (returnValue);
}

/*******************************************************************************
* Function Name: l_u8_rd_RedValue
********************************************************************************
*
* Summary:
*  Reads and returns the current value of the signal for signals of the size
*  2 - 8 bits.
*
* Return:
*  Returns current value of signal.
*
*******************************************************************************/
l_u8 l_u8_rd_RedValue(void) 
{
    /* Get one complete byte signal from frame */
    return(LINS_Buffer_InFrame[LINS_RedValue_InFrame_SIG_BYTE_OFFSET]);
}


/*******************************************************************************
* Function Name: l_u8_rd_GreenValue
********************************************************************************
*
* Summary:
*  Reads and returns the current value of the signal for signals of the size
*  2 - 8 bits.
*
* Return:
*  Returns current value of signal.
*
*******************************************************************************/
l_u8 l_u8_rd_GreenValue(void) 
{
    /* Get one complete byte signal from frame */
    return(LINS_Buffer_InFrame[LINS_GreenValue_InFrame_SIG_BYTE_OFFSET]);
}


/*******************************************************************************
* Function Name: l_u8_rd_BlueValue
********************************************************************************
*
* Summary:
*  Reads and returns the current value of the signal for signals of the size
*  2 - 8 bits.
*
* Return:
*  Returns current value of signal.
*
*******************************************************************************/
l_u8 l_u8_rd_BlueValue(void) 
{
    /* Get one complete byte signal from frame */
    return(LINS_Buffer_InFrame[LINS_BlueValue_InFrame_SIG_BYTE_OFFSET]);
}










/*******************************************************************************
* Function Name: l_bool_wr_LINS
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for one bit signals to "v".
*  If an invalid signal handle is passed into the function, no action is done. 
*
* Parameters:
*  sss - signal handle of the signal to be set.
*  v - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_bool_wr_LINS(l_signal_handle sss, l_bool v)
{
    switch(sss)
    {
        case Response_Error_SIGNAL_HANDLE:
            l_bool_wr_Response_Error(v);
        break;

        default:
        break;
    }
}

/*******************************************************************************
* Function Name: l_bool_wr_Response_Error
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for one bit signals to v.
*
* Parameters:
*  v - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_bool_wr_Response_Error(l_bool v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    if(0u != v)
    {
        LINS_Buffer_OutFrame[LINS_Response_Error_OutFrame_SIG_BYTE_OFFSET] |= 
                            LINS_Response_Error_OutFrame_SIG_MASK_0;
    }
    else
    {
        LINS_Buffer_OutFrame[LINS_Response_Error_OutFrame_SIG_BYTE_OFFSET] &= 
                            (l_u8) ~LINS_Response_Error_OutFrame_SIG_MASK_0;
    }

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}






/*******************************************************************************
* Function Name: l_u8_wr_LINS
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*  If an invalid signal handle is passed into the function, no action is done.
*
* Parameters:
*  sss - signal handle of the signal to write.
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_LINS(l_signal_handle sss, l_u8 v)
{
    switch(sss)
    {
        case OutSig_SIGNAL_HANDLE:
            l_u8_wr_OutSig(v);
        break;

        case OutRedValue_SIGNAL_HANDLE:
            l_u8_wr_OutRedValue(v);
        break;

        case OutGreenValue_SIGNAL_HANDLE:
            l_u8_wr_OutGreenValue(v);
        break;

        case OutBlueValue_SIGNAL_HANDLE:
            l_u8_wr_OutBlueValue(v);
        break;

        default:
        break;
    }
}

/*******************************************************************************
* Function Name: l_u8_wr_OutSig
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_OutSig(l_u8 v)
{
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    /* Clear bits to be set */
    LINS_Buffer_OutFrame[LINS_OutSig_OutFrame_SIG_BYTE_OFFSET] &=
                  (l_u8) ~LINS_OutSig_OutFrame_SIG_MASK_0;

    LINS_Buffer_OutFrame[LINS_OutSig_OutFrame_SIG_BYTE_OFFSET] |=
                    ((l_u8) (v & LINS_OutSig_OutFrame_SIG_MASK_0));

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: l_u8_wr_OutRedValue
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_OutRedValue(l_u8 v)
{
    LINS_Buffer_OutFrame[LINS_OutRedValue_OutFrame_SIG_BYTE_OFFSET] = v;
}


/*******************************************************************************
* Function Name: l_u8_wr_OutGreenValue
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_OutGreenValue(l_u8 v)
{
    LINS_Buffer_OutFrame[LINS_OutGreenValue_OutFrame_SIG_BYTE_OFFSET] = v;
}


/*******************************************************************************
* Function Name: l_u8_wr_OutBlueValue
********************************************************************************
*
* Summary:
*  Sets the current value of the signal for signals of the size 2 - 8 bits to v.
*
* Parameters:
*  v   - value of the signal to be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr_OutBlueValue(l_u8 v)
{
    LINS_Buffer_OutFrame[LINS_OutBlueValue_OutFrame_SIG_BYTE_OFFSET] = v;
}










/*******************************************************************************
* Function Name: l_flg_tst_LINS
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Parameters:
*  fff - is the name of the flag handle.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_LINS(l_flag_handle fff)
{
    l_bool returnValue;

    switch(fff)
    {
        case MRF_FLAG_HANDLE:
            returnValue = l_flg_tst_MRF();
        break;

        case SRF_FLAG_HANDLE:
            returnValue = l_flg_tst_SRF();
        break;

        case RedValue_FLAG_HANDLE:
            returnValue = l_flg_tst_RedValue();
        break;

        case GreenValue_FLAG_HANDLE:
            returnValue = l_flg_tst_GreenValue();
        break;

        case BlueValue_FLAG_HANDLE:
            returnValue = l_flg_tst_BlueValue();
        break;

        case OutSig_FLAG_HANDLE:
            returnValue = l_flg_tst_OutSig();
        break;

        case OutRedValue_FLAG_HANDLE:
            returnValue = l_flg_tst_OutRedValue();
        break;

        case OutGreenValue_FLAG_HANDLE:
            returnValue = l_flg_tst_OutGreenValue();
        break;

        case OutBlueValue_FLAG_HANDLE:
            returnValue = l_flg_tst_OutBlueValue();
        break;

        case InFrame_FLAG_HANDLE:
            returnValue = l_flg_tst_InFrame();
        break;

        case OutFrame_FLAG_HANDLE:
            returnValue = l_flg_tst_OutFrame();
        break;

        default:
            returnValue = LINS_TRUE;
        break;
    }

    return(returnValue);
}


/*******************************************************************************
* Function Name: l_flg_tst_MRF
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the Master Request Frame
*  (MRF), i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the Master Request Frame
*  (MRF):
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_MRF(void)
{
    l_bool result;

    if (0u != (LINS_statusFlagArray[LINS_MRF_FRAME_FLAG_BYTE_OFFSET_0] &
    LINS_MRF_FRAME_FLAG_MASK_0))
    {
        result = LINS_TRUE;
    }
    else
    {
        result = LINS_FALSE;
    }
    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_SRF
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the Slave Response Frame
*  (SRF), i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the Slave Response Frame
*  (SRF):
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_SRF(void)
{
    l_bool result;

    if (0u != (LINS_statusFlagArray[LINS_SRF_FRAME_FLAG_BYTE_OFFSET_0] &
    LINS_SRF_FRAME_FLAG_MASK_0))
    {
        result = LINS_TRUE;
    }
    else
    {
        result = LINS_FALSE;
    }
    return (result);
}



/*******************************************************************************
* Function Name: l_flg_tst_RedValue
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_RedValue(void)
{
    l_bool result = LINS_FALSE;

    if (0u != (LINS_statusFlagArray[LINS_RedValue_FRAME_FLAG_BYTE_OFFSET_0] &
    LINS_RedValue_FRAME_FLAG_MASK_0))
    {
        result = LINS_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_GreenValue
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_GreenValue(void)
{
    l_bool result = LINS_FALSE;

    if (0u != (LINS_statusFlagArray[LINS_GreenValue_FRAME_FLAG_BYTE_OFFSET_0] &
    LINS_GreenValue_FRAME_FLAG_MASK_0))
    {
        result = LINS_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_BlueValue
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_BlueValue(void)
{
    l_bool result = LINS_FALSE;

    if (0u != (LINS_statusFlagArray[LINS_BlueValue_FRAME_FLAG_BYTE_OFFSET_0] &
    LINS_BlueValue_FRAME_FLAG_MASK_0))
    {
        result = LINS_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_OutSig
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_OutSig(void)
{
    l_bool result = LINS_FALSE;

    if (0u != (LINS_statusFlagArray[LINS_OutSig_FRAME_FLAG_BYTE_OFFSET_0] &
    LINS_OutSig_FRAME_FLAG_MASK_0))
    {
        result = LINS_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_OutRedValue
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_OutRedValue(void)
{
    l_bool result = LINS_FALSE;

    if (0u != (LINS_statusFlagArray[LINS_OutRedValue_FRAME_FLAG_BYTE_OFFSET_0] &
    LINS_OutRedValue_FRAME_FLAG_MASK_0))
    {
        result = LINS_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_OutGreenValue
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_OutGreenValue(void)
{
    l_bool result = LINS_FALSE;

    if (0u != (LINS_statusFlagArray[LINS_OutGreenValue_FRAME_FLAG_BYTE_OFFSET_0] &
    LINS_OutGreenValue_FRAME_FLAG_MASK_0))
    {
        result = LINS_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_OutBlueValue
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_OutBlueValue(void)
{
    l_bool result = LINS_FALSE;

    if (0u != (LINS_statusFlagArray[LINS_OutBlueValue_FRAME_FLAG_BYTE_OFFSET_0] &
    LINS_OutBlueValue_FRAME_FLAG_MASK_0))
    {
        result = LINS_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_InFrame
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_InFrame(void)
{
    l_bool result = LINS_FALSE;

    if (0u != (LINS_statusFlagArray[LINS_InFrame_FRAME_FLAG_BYTE_OFFSET_0] &
    LINS_InFrame_FRAME_FLAG_MASK_0))
    {
        result = LINS_TRUE;
    }

    return (result);
}


/*******************************************************************************
* Function Name: l_flg_tst_OutFrame
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_OutFrame(void)
{
    l_bool result = LINS_FALSE;

    if (0u != (LINS_statusFlagArray[LINS_OutFrame_FRAME_FLAG_BYTE_OFFSET_0] &
    LINS_OutFrame_FRAME_FLAG_MASK_0))
    {
        result = LINS_TRUE;
    }

    return (result);
}






/*******************************************************************************
* Function Name: l_flg_clr_LINS
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Parameters:
*  fff - is the name of the flag handle.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_LINS(l_flag_handle fff)
{
    switch(fff)
    {
        case MRF_FLAG_HANDLE:
            l_flg_clr_MRF();
        break;

        case SRF_FLAG_HANDLE:
            l_flg_clr_SRF();        break;

        case RedValue_FLAG_HANDLE:
            l_flg_clr_RedValue();
        break;

        case GreenValue_FLAG_HANDLE:
            l_flg_clr_GreenValue();
        break;

        case BlueValue_FLAG_HANDLE:
            l_flg_clr_BlueValue();
        break;

        case OutSig_FLAG_HANDLE:
            l_flg_clr_OutSig();
        break;

        case OutRedValue_FLAG_HANDLE:
            l_flg_clr_OutRedValue();
        break;

        case OutGreenValue_FLAG_HANDLE:
            l_flg_clr_OutGreenValue();
        break;

        case OutBlueValue_FLAG_HANDLE:
            l_flg_clr_OutBlueValue();
        break;

        case InFrame_FLAG_HANDLE:
            l_flg_clr_InFrame();
        break;

        case OutFrame_FLAG_HANDLE:
            l_flg_clr_OutFrame();
        break;

        default:
        break;
    }
}


/*******************************************************************************
* Function Name: l_flg_clr_MRF
********************************************************************************
*
* Summary:
*  Sets the current value of the Master Request Frame (MRF) flag.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MRF(void)
{
    LINS_statusFlagArray[LINS_MRF_FRAME_FLAG_BYTE_OFFSET_0] &=
        ((l_u8) ~LINS_MRF_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_SRF
********************************************************************************
*
* Summary:
*  Sets the current value of the Slave Response Frame (SRF) flag.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_SRF(void)
{
    LINS_statusFlagArray[LINS_SRF_FRAME_FLAG_BYTE_OFFSET_0] &=
    ((l_u8) ~LINS_SRF_FRAME_FLAG_MASK_0);
}



/*******************************************************************************
* Function Name: l_flg_clr_RedValue
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_RedValue(void)
{
    LINS_statusFlagArray[LINS_RedValue_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LINS_RedValue_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_GreenValue
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_GreenValue(void)
{
    LINS_statusFlagArray[LINS_GreenValue_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LINS_GreenValue_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_BlueValue
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_BlueValue(void)
{
    LINS_statusFlagArray[LINS_BlueValue_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LINS_BlueValue_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_OutSig
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_OutSig(void)
{
    LINS_statusFlagArray[LINS_OutSig_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LINS_OutSig_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_OutRedValue
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_OutRedValue(void)
{
    LINS_statusFlagArray[LINS_OutRedValue_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LINS_OutRedValue_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_OutGreenValue
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_OutGreenValue(void)
{
    LINS_statusFlagArray[LINS_OutGreenValue_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LINS_OutGreenValue_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_OutBlueValue
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_OutBlueValue(void)
{
    LINS_statusFlagArray[LINS_OutBlueValue_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LINS_OutBlueValue_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_InFrame
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_InFrame(void)
{
    LINS_statusFlagArray[LINS_InFrame_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LINS_InFrame_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_OutFrame
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_OutFrame(void)
{
    LINS_statusFlagArray[LINS_OutFrame_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LINS_OutFrame_FRAME_FLAG_MASK_0);
}






#if (1u == LINS_LIN_1_3)
    /**************************************************************************
    * Function Name: l_ifc_disconnect
    ***************************************************************************
    *
    * Summary:
    *  Stops the component operation.
    *  This function is implemented because it is mandatory for LIN v.1.3 API.
    *
    * Return:
    *  Zero     - If the initialization succeeds.
    *  Non-zero - If the initialization fails.
    *
    **************************************************************************/
    l_bool l_ifc_disconnect_LINS(void)
    {
        l_u8 return_value = LINS_FALSE;
        LINS_Stop();

        return (return_value);
    }


    /**************************************************************************
    * Function Name: l_ifc_connect
    ***************************************************************************
    *
    * Summary:
    *  Inilizes LIN interface.
    *  This function is implemented because it is mandatory for LIN v.1.3 API.
    *
    * Return:
    *  Zero     - If the initialization succeeds.
    *  Non-zero - If the initialization fails.
    *
    **************************************************************************/
    l_bool l_ifc_connect_LINS(void)
    {
        return (l_ifc_init_LINS());
    }
#endif /* (1u == LINS_LIN_1_3) */


/******************************************************************************
* Function Name: l_ifc_init_LINS
*******************************************************************************
*
* Summary:
*  The function initializes the LIN Slave component instance that is specified
*  by the name iii. It sets up internal functions such as the Baud rate and
*  starts up digital blocks used by the LIN Slave component. This is
*  the first call that must be performed before using any other interface-
*  related LIN Slave API functions.
*
* Return:
*  0        - If the operation succeeds.
*  1        - If an invalid operation parameter is passed to the function.
*
******************************************************************************/
l_bool l_ifc_init_LINS(void)
{
    l_u8 interruptState;
    l_u8 i;
    l_bool returnValue = LINS_FALSE;

    interruptState = CyEnterCriticalSection();

    /* Sets the internal clock divider with effect on the end of a cycle.
     *  The divider's value is decremented to be compatible with the
     *  SetFractionalDividerRegister() parameters */
    #if (1u == LINS_AUTO_BAUD_RATE_SYNC)
        LINS_IntClk_Stop();
        LINS_IntClk_SetFractionalDividerRegister((LINS_initialClockDivider - 1u), 0u);
        LINS_IntClk_Start();
    #endif  /* (1u == LINS_AUTO_BAUD_RATE_SYNC) */

    /* Initialize UART */
    LINS_SCB_Init();

    /* Set Break Detection Threshold in counts */
    LINS_SCB_UART_RX_CTRL_REG |= (uint32)((uint32)(LINS_BREAK_THRESHOLD - 1u) <<
                                             LINS_SCB_UART_RX_CTRL_BREAK_WIDTH_POS);

    /* Place SCB UART in LIN compatibility mode  */
    LINS_SCB_UART_RX_CTRL_REG |= (uint32)LINS_SCB_UART_RX_CTRL_LIN_MODE;

    /* Configure BREAK DETECT, BAUD DETECT and FIFO NOT EMPTY as interrupt sources */
    LINS_SCB_INTR_RX_MASK_REG |= LINS_SCB_INTR_RX_BREAK_DETECT |
            LINS_SCB_INTR_RX_BAUD_DETECT | LINS_SCB_INTR_RX_NOT_EMPTY;

    /* Copy PIDs from NVRAM to VRAM */
    for (i = 0u; i < LINS_NUM_FRAMES; i++)
    {
        LINS_volatileConfig[i] = LINS_LinSlaveConfig.pidTable[i];
    }

    #if (LINS_TL_USED)
        LINS_nad = LINS_LinSlaveConfig.initialNad;
        
        #if (LINS_SAE_J2602_ENABLED)
            (void) LINS_UpdateMessageIDs(LINS_nad);
        #endif /* LINS_SAE_J2602_ENABLED */
    #endif /* LINS_TL_USED */    

    /* Clears interface status */
    LINS_ifcStatus &= (l_u16) ~LINS_IFC_STS_MASK;

    #if (1u == LINS_INACTIVITY_ENABLED)
        /* Clear period timer counter */
        LINS_periodCounter = 0x00u;
    #endif /* (1u == LINS_INACTIVITY_ENABLED) */

    /* Clears the ioctl status register */
    LINS_ioctlStatus = 0x00u;

    LINS_SCB_Enable();

    CyExitCriticalSection(interruptState);

    /* Bus inactivity block configuration */
    #if (1u == LINS_INACTIVITY_ENABLED)
        /* Starts SysTick timer with default period = 1mS */
        if (CYRET_SUCCESS != LINS_TimerEnableInt())
        {
            /* Reports an error because all SysTick callback slots are busy */
            returnValue = LINS_TRUE;
        }
    #endif  /* (1u == LINS_INACTIVITY_ENABLED) */

    /* Clears pending SCB interrupts */
    LINS_ISR_ClearPending();

    /* Enables SCB interrupt */
    LINS_ISR_StartEx(&LINS_UART_ISR);

    return (returnValue);
}


/******************************************************************************
* Function Name: l_ifc_wake_up
*******************************************************************************
*
* Summary:
*  This function transmits one wakeup signal. The wakeup signal is transmitted
*  directly when this function is called. When you call this API function, the
*  application is blocked until the wakeup signal is transmitted to the LIN bus.
*  The CyDelayUs() function is used as the timing source. The delay is
*  calculated based on the clock configuration entered in PSoC Creator.
*
******************************************************************************/
void l_ifc_wake_up_LINS(void)
{
    #if (0u == LINS_LIN_1_3)
        /* For LIN 2.x: Force TXD low by transmission of N dominant bits
        *  (N depends on baudrate)
        *  To calculate how much of bit-interval takes 300uS wake-up pulse:
        *  1) Calculate bit period: 1000000(uS) divide by BAUD_RATE(bps)
        *  2) Divide WAKE_UP_SIGNAL_LENGTH by result of 1)
        *  3) Add 1 to compensate rounding error
        *  4) Shift 0xFF constant by number of bit obtained in 3)
        *  form dominant level pulse with duration proportional to Baud rate
        */
        LINS_SCB_SpiUartWriteTxData((uint8) (0xffu << ((LINS_WAKE_UP_SIGNAL_LENGTH /
            (1000000u / LINS_BAUD_RATE)) + 1u)));

        /* Waits until symbol transmission ends, Tdelay > 11mS ,
        *  11 bit-times at min. baudrate 1000bps,for  8N1.5 symbol */
        CyDelay(LINS_WAKEUP_TX_DELAY);
    #else
        /* For LIN 1.3: Simply send 0x80 as LIN 1.3 specifies */
        LINS_SCB_SpiUartWriteTxData((uint8) 0x80);
    #endif /* (0u == LINS_LIN_1_3) */
}


/******************************************************************************
* Function Name: l_ifc_ioctl
*******************************************************************************
*
* Summary:
*  This function controls functionality that is not covered by the other API
*  calls. It is used for protocol specific parameters or hardware specific
*  functionality.
*
* Parameters:
*  op - The operation to be applied.
*  pv - The pointer to the optional parameter.
*
* Return:
*  There is no error code value returned for the operation selected. This means
*  that you must ensure that the values passed into the function are correct.
*
*  L_IOCTL_READ_STATUS operation:
*  The first bit in this byte is the flag that indicates there was no
*  signalling on the bus for a certain elapsed time (available when the
*  Bus Inactivity Timeout Detection option is enabled). If the elapsed time
*  passes a certain threshold, this flag is set. Calling this API clears
*  all the status bits after they are returned. The second bit is the flag that
*  indicates that a Targeted Reset service request (0xB5) is received
*  (when J2602-1 Compliance is enabled).
*
*  Symbolic Name : LINS_IOCTL_STS_BUS_INACTIVITY
*  Value         : 0x0001u
*  Description   : No signal has been detected on the bus for a certain elapsed
*                  time
*
*  Symbolic Name : LINS_IOCTL_STS_TARGET_RESET
*  Value         : 0x0002u
*  Description   : A Targeted Reset service request (0xB5) has been received.
*
*  L_IOCTL_SET_BAUD_RATE operation:
*  0 is returned if operation succeeds and 1 if an invalid operation
*  parameter is passed to the function.
*
*  L_IOCTL_SLEEP operation:
*  CYRET_SUCCESS is returned if operation succeeds and CYRET_BAD_PARAM if
*  an invalid operation parameter is passed to the function.
*
*  L_IOCTL_WAKEUP operation:
*  CYRET_SUCCESS is returned if operation succeeds and CYRET_BAD_PARAM if
*  an invalid operation parameter is passed to the function.
*
*  L_IOCTL_SYNC_COUNTS operation:
*  Returns the current number of sync field timer counts as l_u16 in *pv.
*
*  L_IOCTL_SET_SERIAL_NUMBER operation:
*  CYRET_SUCCESS is returned if operation succeeds and CYRET_BAD_PARAM if
*  an invalid operation parameter is passed to the function.
*  
*  L_IOCTL_GET_NAD      
*  Returns configured NAD to l_u8 *pv
*
*  L_IOCTL_SET_NAD      
*  CYRET_SUCCESS is returned if operation succeeds and CYRET_BAD_PARAM if
*  an invalid operation parameter or invalid NAD is passed to the function.
*
*  L_IOCTL_GET_FRAME_PID
*  Frame PID by its index in frame table, starting from 0. 
*  Uses LINS_NEW_PID structure to return PID.
*  LINS_INVALID_FRAME_PID is returned for indexes out of range.
*  CYRET_SUCCESS is returned if operation succeeds and CYRET_BAD_PARAM if
*  an invalid operation parameter or out of range value is passed to the function.
*
*  L_IOCTL_SET_FRAME_PID
*  Sets Frame PID by its index in frame table, starting from 0.
*  Uses a pointer to LINS_NEW_PID structure as input parameter.
*  CYRET_SUCCESS is returned if operation succeeds and CYRET_BAD_PARAM if
*  an invalid operation parameter or out of range value is passed to the function.
*
*  L_IOCTL_SET_DNN      
*  Sets DNN value to initial NAD and configured NAD, and 
*  limits NAD range to 0x60-0x6F. DNN and NAD are always set. 
*  CYRET_BAD_PARAM is returned if Message ID table can not be updated correctly.
*
*  L_IOCTL_SET_FRAME_PID_BY_MESSAGE_ID
*  Finds a frame with specified messageID and sets its PID.
*  CYRET_BAD_PARAM is returned if Message ID is not found in the table.
*
*******************************************************************************/
l_u16 l_ifc_ioctl_LINS(l_ioctl_op op, void* pv)
{
    l_u16 returnValue = (l_u16) CYRET_SUCCESS;
    l_u8 interruptState;
    #if (LINS_TL_USED && LINS_CFG_SERVICES_ENABLED)
        LINS_NEW_PID *newPID;
    #endif /* LINS_TL_USED && LINS_CFG_SERVICES_ENABLED*/
    switch (op)
    {
        /***********************************************************************
        * Read Status
        ***********************************************************************/
        case L_IOCTL_READ_STATUS:
            /* Return status */
            returnValue = LINS_ioctlStatus;

            /* Clear status */
        #if (LINS_SAE_J2602_ENABLED && \
            (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB5_SEL)))
                LINS_ioctlStatus &= (l_u16)~(l_u16)LINS_IOCTL_STS_TARGET_RESET;
        #endif /* (LINS_SAE_J2602_ENABLED && 
                * (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB5_SEL)) */
        break;
        /***********************************************************************
        * Set Baud Rate
        ***********************************************************************/
        case L_IOCTL_SET_BAUD_RATE:
            interruptState = CyEnterCriticalSection();
            /* Set new internal clock divider with effect on the end of a cycle.
            *  The divider's value is decremented to be compatible with the
            *  SetFractionalDividerRegister() parameters */
            LINS_IntClk_Stop();
            LINS_IntClk_SetFractionalDividerRegister(((*((l_u16*)pv)) - 1u), 0u);
            #if (1u == LINS_AUTO_BAUD_RATE_SYNC)
                LINS_initialClockDivider = *(((l_u16*)pv));
            #endif /* (1u == LINS_AUTO_BAUD_RATE_SYNC) */
            LINS_IntClk_Start();
            CyExitCriticalSection(interruptState);
        break;
        /***********************************************************************
        * Prepare for low power modes
        ***********************************************************************/
        case L_IOCTL_SLEEP:
            LINS_SCB_Stop();
            
            /* Clears SCB FIFOs*/
            LINS_SCB_CLEAR_TX_FIFO;
            LINS_SCB_CLEAR_RX_FIFO;
            
            /* Disables interrupt from this instance */
            LINS_ISR_Disable();
            
             /* Clears pending interrupts */
            LINS_ISR_ClearPending();
            LINS_SCB_ClearRxInterruptSource(LINS_SCB_INTR_RX_ALL);
        break;
        /***********************************************************************
        * Restore after wakeup from low power modes
        ***********************************************************************/
        case L_IOCTL_WAKEUP:
            (void)l_ifc_init_LINS();
        break;
    #if (1u == LINS_AUTO_BAUD_RATE_SYNC)
        case L_IOCTL_SYNC_COUNTS:
            /* Returns current number of sync field timer counts */
            *((l_u16*) pv) = (l_u16) LINS_syncCounts;
        break;
    #endif  /* (1u == LINS_AUTO_BAUD_RATE_SYNC) */
    
    #if (LINS_TL_USED && LINS_CFG_SERVICES_ENABLED)
        case L_IOCTL_SET_SERIAL_NUMBER:
            LINS_serialNumber = (l_u8*) pv;
        break;
        /***********************************************************************
        * Gets configured NAD. NAD must not be 00, 7E nor 7F.
        ***********************************************************************/
        case L_IOCTL_GET_NAD:
            *((l_u8*) pv) = LINS_nad;
        break;
        /***********************************************************************
        * Sets configured NAD. NAD must not be 00, 7E nor 7F.
        ***********************************************************************/
        case L_IOCTL_SET_NAD:
            if ((((*(l_u8*) pv)) != LINS_NAD_FUNCTIONAL) &&
                (((*(l_u8*) pv)) != LINS_NAD_BROADCAST)  &&
                (((*(l_u8*) pv)) != LINS_NAD_GO_TO_SLEEP))
            {
                #if (LINS_SAE_J2602_ENABLED) 
                    returnValue = (l_u16) LINS_UpdateMessageIDs(*((l_u8*) pv));
                    if (returnValue == (l_u16) CYRET_BAD_PARAM)
                    {
                        break; 
                    }
                #endif /* LINS_SAE_J2602_ENABLED */
                
                LINS_nad = *((l_u8*) pv);                
            }
            else
            {
                returnValue = (l_u16) CYRET_BAD_PARAM;
            }
        break;
        /***********************************************************************
        * Gets frame PIDs by the frame table index.
        * Uses pointer to the LINS_NEW_PID structure
        * as parameter. Input LINS_NEW_PID.index. 
        * Returns LINS_NEW_PID.PID 
        ***********************************************************************/
        case L_IOCTL_GET_FRAME_PID:
            newPID = (LINS_NEW_PID*) pv;
            if (newPID->index < LINS_NUM_FRAMES)
            {
                newPID->PID = LINS_volatileConfig[newPID->index];
            }
            else
            {
                returnValue = (l_u16) CYRET_BAD_PARAM;
                newPID->PID = LINS_INVALID_FRAME_PID;
            }
        break;
        /***********************************************************************
        * Sets frame PIDs by frame table index.
        * Uses LINS_NEW_PID structure
        * as input parameter.
        ***********************************************************************/
        case L_IOCTL_SET_FRAME_PID:
            newPID = (LINS_NEW_PID*) pv;
            
            if ((newPID->index < LINS_NUM_FRAMES) &&
                ((newPID->PID & LINS_PID_PARITY_MASK) < LINS_FRAME_PID_MRF))
            {
                LINS_volatileConfig[newPID->index] = newPID->PID;
            }
            else
            {
            returnValue = (l_u16) CYRET_BAD_PARAM;
            }
        break;
        
        #if (LINS_SAE_J2602_ENABLED)
            /***********************************************************************
            * Sets DNN value to initial NAD and configured NAD.
            * Limits NAD range to 0x60-0x6F.
            ***********************************************************************/
            case L_IOCTL_SET_DNN:
                    LINS_LinSlaveConfig.initialNad = (*((l_u8*) pv) &
                    LINS_NAD_DNN_MASK) | LINS_NAD_J2602_BASE;
                    LINS_nad = LINS_LinSlaveConfig.initialNad;
                    returnValue = (l_u16) LINS_UpdateMessageIDs(LINS_nad);
            break;
        #endif /* LINS_SAE_J2602_ENABLED */
        
        #if (LINS_LIN_2_0 == 1u)
        /***********************************************************************
        * Sets frame PIDs by Message ID. (LIN 2.0 only)
        * Uses LINS_NEW_PID_BY_MSG_ID structure
        * as input parameter.
        ***********************************************************************/
        l_u8 i;
        case L_IOCTL_SET_FRAME_PID_BY_MESSAGE_ID:
            returnValue = (l_u16) CYRET_BAD_PARAM;
            
            for (i = 0u; i < LINS_NUM_FRAMES; i++)
            {
                if (messageIdTable[i] == (*((LINS_NEW_PID_BY_MSG_ID*) pv)).messageID)
                {
                    /* MessageID is found in the table */
                    LINS_volatileConfig[i] = (*((LINS_NEW_PID_BY_MSG_ID*) pv)).PID;
                    returnValue = (l_u16) CYRET_SUCCESS;
                    break;
                }
            }
        break;
        #endif /* (LINS_LIN_2_0 == 1u) */
    #endif /* (LINS_TL_USED && LINS_CFG_SERVICES_ENABLED) */

        /***********************************************************************
        * Unknown operation            
        ***********************************************************************/
        default:
            /* Unknown operation */
            returnValue = (l_u16) CYRET_BAD_PARAM;
        break;
    }

    return (returnValue);
}


/*******************************************************************************
* Function Name: l_ifc_rx
********************************************************************************
*
* Summary:
*  The LIN Slave component takes care of calling this API routine automatically.
*  Therefore, this API routine must not be called by the application code.
*
*******************************************************************************/
void l_ifc_rx_LINS(void)
{
    l_u8 i;
    l_u8 interruptState;
#if (1u == LINS_AUTO_BAUD_RATE_SYNC)
    uint32 tmp;
    static uint32 LINS_correctedClockDivider;
#endif  /* (1u == LINS_AUTO_BAUD_RATE_SYNC) */
    static l_u16 LINS_interimChecksum;   /* Holds interim checksum value      */
    static l_u8 LINS_framePid;           /* PID of current frame              */
    static l_u8 LINS_frameSize;          /* Size of frame being processed     */
    static l_u8 LINS_bytesTransferred;   /* Number of transferred bytes       */
    static l_u8 LINS_tmpData;            /* Used to store transmitted byte    */
    static l_u8 LINS_pidIndex;           /* Index in pidInfoTable             */
    static l_u8 LINS_tmpRxFrameData[8u]; /* RXed data before checksum checked */
    static volatile l_u8* LINS_frameData = NULL;  /* Pointer to frame data, points to byte to be sent.*/
    static l_u8 LINS_frameErrorAfterPID;

    /* PID information table */
    static LINS_PID_INFO_TABLE const CYCODE LINS_pidInfoTable[LINS_NUM_FRAMES] =
{
    {(LINS_FRAME_DIR_SUBSCRIBE | LINS_FRAME_DATA_SIZE_8 | LINS_FRAME_TYPE_UNCOND),
     LINS_Buffer_InFrame}, 
    {(LINS_FRAME_DIR_PUBLISH | LINS_FRAME_DATA_SIZE_8 | LINS_FRAME_TYPE_UNCOND),
     LINS_Buffer_OutFrame}
};

   /* Sets bus activity interface status bit */
    LINS_ifcStatus |= LINS_IFC_STS_BUS_ACTIVITY;

#if (1u == LINS_INACTIVITY_ENABLED)
    /* Clears the period timer counter */
    LINS_periodCounter = 0x00u;
    /* Clears bus inactivity ioctl status bit */
    LINS_ioctlStatus &= (l_u16)(~(l_u16) LINS_IOCTL_STS_BUS_INACTIVITY);
#endif /* (1u == LINS_INACTIVITY_ENABLED) */

    /* Checks for RX UART framing error and overflow  */
    if (0u != (LINS_SCB_INTR_RX_REG & (LINS_SCB_INTR_RX_FRAME_ERROR |
               LINS_SCB_INTR_RX_OVERFLOW)))
    {
        LINS_SCB_ClearRxInterruptSource(LINS_SCB_INTR_RX_ERR | 
                                                    LINS_SCB_INTR_RX_OVERFLOW);
         /* Set framing error */
        LINS_fsmFlags |= LINS_FSM_FRAMING_ERROR_FLAG;
    }

    /***************************************************************************
    *                       Break Field Detected
    ***************************************************************************/
    if (0u != (LINS_SCB_INTR_RX_REG & LINS_SCB_INTR_RX_BREAK_DETECT))
    {
        /* Resets pending flag */
        LINS_SCB_INTR_RX_REG = LINS_SCB_INTR_RX_BREAK_DETECT;
        /* Framing error or data transfer is aborted */
        if (0u != (LINS_FSM_DATA_RECEIVE & LINS_fsmFlags))
        {
            /* Sets response error */
            LINS_ifcStatus |= LINS_IFC_STS_ERROR_IN_RESPONSE;
        #if (LINS_SAE_J2602_ENABLED)
            /* Sets ERR2 bit */
            LINS_j2602Status_Set(LINS_J2602_STS_DATA_ERR);
        #endif  /* LINS_SAE_J2602_ENABLED */
           
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LINS_Buffer_OutFrame[LINS_Response_Error_OutFrame_SIG_BYTE_OFFSET] |= 
                            LINS_Response_Error_OutFrame_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

        }   /* No response error, continue */
        
    /* Auto Baud Rate Sync Enabled */
    #if (1u == LINS_AUTO_BAUD_RATE_SYNC)
        LINS_IntClk_Stop();
        /* Restores the initial clock divider */
        LINS_IntClk_SetFractionalDividerRegister((uint16) 
            (LINS_initialClockDivider - 1u), 0u);
        LINS_IntClk_Start();
    #endif  /* (1u == LINS_AUTO_BAUD_RATE_SYNC) */
    
        LINS_fsmFlags &= ((l_u8) ~(LINS_FSM_DATA_RECEIVE |
                                LINS_FSM_FRAMING_ERROR_FLAG));
                                
        /* Sets UART ISR FSM to IDLE state */
        LINS_uartFsmState = LINS_UART_ISR_STATE_0_IDLE;
        /* Resets UART state machine */
        LINS_EndFrame(LINS_HANDLING_RESET_FSM_ERR);
        
        /* Starts the BREAK to SYNC timeout counter for SYNC failure detection*/
        LINS_breakToSyncCounts = 1u; /* Counter increments the value if non-zero. */
        return;
    }

    /***********************************************************************
    *                       Sync Field Complete                            *
    ***********************************************************************/
    if (0u != (LINS_SCB_INTR_RX_REG & LINS_SCB_INTR_RX_BAUD_DETECT))
    {
        /* Clears SYNC source first */
        LINS_SCB_INTR_RX_REG = LINS_SCB_INTR_RX_BAUD_DETECT;
        
        /* Stops and disable the BREAK to SYNC timeout counter */
        LINS_breakToSyncCounts=0u; /* counter does not increment if zero */
        
    #if (1u == LINS_AUTO_BAUD_RATE_SYNC)    /* Auto Baud Rate Sync Enabled */        
        LINS_syncCounts = (l_u16)(LINS_SCB_UART_RX_STATUS_REG &
            LINS_SCB_UART_RX_STATUS_BR_COUNTER_MASK);
            
        /* Checks if correction is necessary */
        if (LINS_syncCounts != LINS_EXPECTED_TIME_COUNTS)
        {
            /* Clock max deviation < 14%. +0.5% from Master
             * So if deviation > (14.5*128/100) = 18,56 (0x13) - drop the packet
             */
            if ( (int32) LINS_TIME_COUNTS_MAX_DEVIATION > (LINS_ABS(
                 (int32) LINS_syncCounts - (int32) LINS_EXPECTED_TIME_COUNTS)) )
            {
                /* BR_COUNTER uses 0x80 as the base to measure the frequency,
                 * so divide its constant by 0x80 and round off the result checking the last thrown bit */
                tmp = ((uint32)LINS_initialClockDivider * (uint32)LINS_syncCounts);
                LINS_correctedClockDivider = (uint32) (tmp >> LINS_BR_BASE_SHIFT);
                LINS_correctedClockDivider += (l_u16)
                    (( 0u != (tmp & (l_u8)(1u << (LINS_BR_BASE_SHIFT - 1u))) ) ? 1u : 0u);
                LINS_IntClk_Stop();
                LINS_IntClk_SetFractionalDividerRegister((uint16)
                    (LINS_correctedClockDivider - 1u), 0u);
                LINS_IntClk_Start();
            }
            else /* Clocks differ > +/-14% so reset the state machine */
            {
                /* Reset UART state machine */
                LINS_EndFrame(LINS_HANDLING_RESET_FSM_ERR);
                /* Nothing to process in RX/TX state machine due to bad SYNC */
                return;                
            }
        }
    #else   /* If SAE_J2602_ENABLED and there is no auto-bauding, check the SYNC byte field for errors. */
        #if (LINS_SAE_J2602_ENABLED)
            LINS_syncCounts = (l_u16)(LINS_SCB_UART_RX_STATUS_REG &
                LINS_SCB_UART_RX_STATUS_BR_COUNTER_MASK);
                
            /* Check if Baud rate is in the range */
            if (LINS_syncCounts != LINS_EXPECTED_TIME_COUNTS)
            {
                /* Clock max deviation < 2% from Master
                 * So if deviation > (2*128/100) = 2,56 (0x03) - drop the packet
                 */
                if ( (int32) LINS_TIME_COUNTS_MAX_DEVIATION < (LINS_ABS(
                     (int32) LINS_syncCounts - (int32) LINS_EXPECTED_TIME_COUNTS)) )
                {
                 /* Set J2602 response error  */
                
                    /* Set ERR2, ERR1 and ERR0 bits */
                    LINS_j2602Status_Set(LINS_J2602_STS_DATA_ERR);
                    LINS_ifcStatus |= LINS_IFC_STS_ERROR_IN_RESPONSE;
                    
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LINS_Buffer_OutFrame[LINS_Response_Error_OutFrame_SIG_BYTE_OFFSET] |= 
                            LINS_Response_Error_OutFrame_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

            
                    /* Reset UART state machine */
                    LINS_EndFrame(LINS_HANDLING_RESET_FSM_ERR);
                    /* Nothing to process in RX/TX state machine due to bad SYNC */
                    return;
                }
            }
        #endif /* LINS_SAE_J2602_ENABLED */
    #endif /* (1u == LINS_AUTO_BAUD_RATE_SYNC) */

         /* Sets the UART ISR FSM to state 1 (PID awaiting) */
        LINS_uartFsmState = LINS_UART_ISR_STATE_1_PID;
         /* Sets UART enabled flag */
        LINS_fsmFlags |= LINS_FSM_UART_ENABLE_FLAG;
         /* Clears any pending UART interrupt and RX FIFO */
        LINS_SCB_SpiUartClearRxBuffer();
        /* Clears all UART pending interrupts */
        LINS_SCB_INTR_RX_REG = LINS_SCB_INTR_RX_ALL;
    }

    /* Returns if there is nothing to process in RX/TX state machine */
    if (0u == (LINS_SCB_INTR_RX_REG & LINS_SCB_INTR_RX_NOT_EMPTY))
    {
        return;
    }

    switch (LINS_uartFsmState)
    {
        /***********************************************************************
        *                       IDLE state
        * State description:
        *  - Receives a sporadic byte not predicted by the BREAK/SYNC sequence,
        *    so does not set Response Error
        ***********************************************************************/
        case LINS_UART_ISR_STATE_0_IDLE:
            LINS_SCB_SpiUartClearRxBuffer();
            LINS_SCB_INTR_RX_REG = (uint32) LINS_SCB_INTR_RX_NOT_EMPTY;
            /* Reset UART state machine */
            LINS_EndFrame(LINS_HANDLING_RESET_FSM_ERR);
        break;
        /***********************************************************************
        *                       PID Field Byte Receive
        * State description:
        *  - Receives protected identifier (PID)
        *  - Checks PID parity
        *  - Set flags
        *  - Determine next state (RX or TX)
        ***********************************************************************/
        case LINS_UART_ISR_STATE_1_PID:
            if (0u != (LINS_FSM_FRAMING_ERROR_FLAG & LINS_fsmFlags))
            {
                /* Reset UART State Machine */
                LINS_EndFrame(LINS_HANDLING_DONT_SAVE_PID);
                break;
            }
        #if (LINS_SAE_J2602_ENABLED)
            /* Updates J2602_Status signal to be ready for next SRF */
            LINS_J2602_Status_Write(LINS_j2602Status_Get());
        #endif /* LINS_SAE_J2602_ENABLED */
            
            /* Saves PID */
            LINS_framePid = (l_u8) LINS_SCB_SpiUartReadRxData();
            
            /* Resets number of transferred bytes */
            LINS_bytesTransferred = 0u;
            
            /* Clears checksum byte */
            LINS_interimChecksum = 0u;
            
            /* Clears Frame Error after PID flag - such a condition must be processed separately,
             * as it can be Frame Error caused by the Break field of the next frame
             */
            LINS_frameErrorAfterPID = 0u;
            
            /* Verifies PID parity */
            if (LINS_parityTable[LINS_framePid & LINS_PID_PARITY_MASK] !=
                LINS_framePid)
            {
            /* Sets J2602 response error  */
            #if (LINS_SAE_J2602_ENABLED)
                /* Sets ERR2, ERR1 and ERR0 bits */
                LINS_j2602Status_Set(LINS_J2602_STS_PARITY_ERR);
                LINS_ifcStatus |= LINS_IFC_STS_ERROR_IN_RESPONSE;
                
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LINS_Buffer_OutFrame[LINS_Response_Error_OutFrame_SIG_BYTE_OFFSET] |= 
                            LINS_Response_Error_OutFrame_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

            #endif /* LINS_SAE_J2602_ENABLED */
            
                /* Resets UART State Machine */
                LINS_EndFrame(LINS_HANDLING_DONT_SAVE_PID);
            }
            else    /* PID parity is correct */
            {
                /* Checks if MRF or SRF frame */
            #if (LINS_SAE_J2602_ENABLED)
                if ((LINS_FRAME_PID_MRF == LINS_framePid) ||
                    (LINS_FRAME_PID_MRF_J2602 == LINS_framePid) ||
                    (LINS_FRAME_PID_SRF == LINS_framePid))
            #else
                if ((LINS_FRAME_PID_MRF == LINS_framePid) ||
                    (LINS_FRAME_PID_SRF == LINS_framePid))                    
            #endif /* (LINS_SAE_J2602_ENABLED) */
                {
                    /*  Transport Layer section. MRF and SRF detection */
                #if (LINS_TL_USED)                    
                    #if (LINS_SAE_J2602_ENABLED)
                        if ((LINS_FRAME_PID_MRF == LINS_framePid) ||
                        (LINS_FRAME_PID_MRF_J2602 == LINS_framePid))
                    #else
                        if (LINS_FRAME_PID_MRF == LINS_framePid)
                    #endif /* LINS_SAE_J2602_ENABLED */
                    {
                        /* Indicates that Slave is required to receive data. */
                        LINS_tlFlags |= LINS_TL_RX_DIRECTION;

                        /*******************************************************
                        *               Cooked & RAW API
                        *******************************************************/
                        /* If MRF PID is detected, passes the pointer to the start of
                        * Frame Buffer and size of data to RX state to handle data receiving */

                        /* Frame equals 8 bytes */
                        LINS_frameSize = LINS_FRAME_DATA_SIZE_8;
                        /* Sets the frame data pointer to start of frame buffer. */
                        LINS_frameData = LINS_mrfBuffer;
                        /* Switches to subscribe data state */
                        LINS_uartFsmState = LINS_UART_ISR_STATE_3_RX;
                        
                        #if (LINS_SAE_J2602_ENABLED)
                            /* Checks for frame with ID == 0x3E (PID == 0xFE) */
                            if (LINS_framePid == LINS_FRAME_PID_MRF_J2602)
                            {
                                LINS_auxStatus |= LINS_AUXSTS_MRF_WAS_3E;
                            }
                            else /* and if the frame is 0x3C, clears the status */
                            {
                                LINS_auxStatus &= (l_u8)~LINS_AUXSTS_MRF_WAS_3E;
                            }
                        #endif /* LINS_SAE_J2602_ENABLED */
                    }

                    if (LINS_FRAME_PID_SRF == LINS_framePid)
                    {
                        if (0u != (LINS_status & LINS_STATUS_SRVC_RSP_RDY))
                        {
                            /* Frame is always equal to 8 bytes for TL */
                            LINS_frameSize = LINS_FRAME_DATA_SIZE_8;
                            /* Sets the frame data pointer to the frame buffer start. */
                            LINS_frameData = LINS_srfBuffer;
                            
                            /* Sends the first byte to LIN Master. */
                            LINS_tmpData = *LINS_frameData;
                            LINS_frameData++;
                            LINS_SCB_SpiUartWriteTxData((uint32) LINS_tmpData);
                            LINS_bytesTransferred = 1u;
                            
                            /* Switch to publish data state. */
                            LINS_uartFsmState = LINS_UART_ISR_STATE_2_TX;
                            /* One or more data bytes have been received */
                            LINS_fsmFlags |= LINS_FSM_DATA_RECEIVE;
                            /* Indicates to Transport layer that Slave transmits the frame. */
                            LINS_tlFlags |= LINS_TL_TX_DIRECTION;
                        }
                        else
                        {
                            /* This part of code handles LIN Transport Layer. Sends one frame of a message if
                             * applications prepared the message in the TX message buffer or TX queue and LIN Slave node
                             * state machine has the appropriate state (is ready for physical response transmission).
                             */
                            if ((0u != (LINS_tlFlags & LINS_TL_TX_REQUESTED)) &&
                                       (LINS_nodeState == LINS_TX_PHY_RESPONSE))
                            {
                            #if (LINS_RAW_TL_API)
                                #if (LINS_SAE_J2602_ENABLED)
                                    /* Inserts the J2602 status byte as byte 0 for frames with ID = 0x3E or NAD >= 0x80 */
                                    if ((0u != (LINS_auxStatus & LINS_AUXSTS_MRF_WAS_3E)) ||
                                        (LINS_mrfBuffer[LINS_PDU_NAD_IDX] > LINS_NAD_BROADCAST))
                                        {
                                            /* Indicate that J2602 will be placed to the outgoing frame and
                                            can be cleared if transmitted successfuly. 
                                            Used for MRF with ID = 0x3E or NAD >= 0x80. */
                                            LINS_auxStatus |= LINS_AUXSTS_MRF_REQUIRES_J2602STATUS;    
                                        }
                                        else
                                        {
                                            LINS_auxStatus &= (l_u8)(~LINS_AUXSTS_MRF_REQUIRES_J2602STATUS);
                                        }
                                #endif /* LINS_SAE_J2602_ENABLED */
                                        
                                if (0u != LINS_txBufDepth)
                                {
                                    /* Fills SRF from the frame buffer */
                                    #if (LINS_SAE_J2602_ENABLED)
                                        /* Insert the J2602 status byte as byte 0 for frames with ID = 0x3E or NAD >= 0x80 */
                                        if (0u != (LINS_auxStatus & LINS_AUXSTS_MRF_REQUIRES_J2602STATUS))
                                        {
                                            LINS_srfBuffer[0] = LINS_j2602Status;
                                            
                                            /* Uncomment the next line to replace byte 0 of user data 
                                            *  but keep the numbering of the rest of the bytes. 
                                            */
                                            /* LINS_txRdIndex++; */
                                        }
                                        else
                                        {
                                            LINS_srfBuffer[0] =
                                                LINS_rawTxQueue[LINS_txRdIndex];
                                            LINS_txRdIndex++;
                                        }
                                        
                                        for (i = 1u; i < LINS_FRAME_DATA_SIZE_8; i++)
                                    #else
                                        for (i = 0u; i < LINS_FRAME_DATA_SIZE_8; i++)
                                    #endif /* LINS_SAE_J2602_ENABLED */
                                    {
                                        LINS_srfBuffer[i] =
                                            LINS_rawTxQueue[LINS_txRdIndex];
                                        /* Updates the index to TX queue */
                                        LINS_txRdIndex++;
                                    }
                                    /* The Read index should point to the next byte in MRF */
                                    if (LINS_TL_TX_QUEUE_LEN == LINS_txRdIndex)
                                    {
                                        LINS_txRdIndex = 0u;
                                    }
                                    /* 8 bytes were read from the SRF, so decrement the buffer depth. */
                                    LINS_txBufDepth--;
                                    /* Updates status properly */
                                    LINS_txTlStatus = (0u == LINS_txBufDepth) ?
                                                                  LD_QUEUE_EMPTY : LD_QUEUE_AVAILABLE;
                                    if (LINS_PDU_PCI_TYPE_UNKNOWN == LINS_txPrevPci)
                                    {
                                        if (LINS_srfBuffer[LINS_PDU_PCI_IDX] <=
                                            LINS_PDU_SF_DATA_LEN)
                                        {
                                            /* Get length of message from PCI field of SF */
                                            LINS_txMessageLength |=
                                                ((l_u16) LINS_srfBuffer[LINS_PDU_PCI_IDX]);
                                        }
                                        else
                                        {
                                            /* Get length of Segmented message from PCI and LEN fields of FF 
                                             * NOTE The PCI field contains four MSb of the length (Length / 256).
                                             */
                                            LINS_txMessageLength = (l_u16)((((l_u16)
                                                LINS_srfBuffer[LINS_PDU_PCI_IDX]) &
                                                ((l_u16) ((l_u8) ~LINS_PDU_PCI_TYPE_MASK))) << 8u);
                                            LINS_txMessageLength |=
                                            ((l_u16) LINS_srfBuffer[LINS_PDU_LEN_IDX]);
                                        }
                                    }
                                }
                            #endif /* LINS_RAW_TL_API */

                                /* Handles PDU packing for Cooked API. */
                                /* Checks the length so it shows if the message is already sent. */
                                if (LINS_txMessageLength == 0u)
                                {
                                    LINS_tlFlags &= (l_u8)~LINS_TL_TX_REQUESTED;
                                    /* Resets UART State Machine */
                                    LINS_EndFrame(LINS_HANDLING_RESET_FSM_ERR);
                                }
                                /* Processes message sending */
                                else
                                {
                                    /* Analyze length to find the type of frame message to be sent */
                                    if (LINS_txMessageLength > LINS_FRAME_DATA_SIZE_6)
                                    {
                                        /* Process the FF Frame */
                                        if (LINS_txPrevPci == LINS_PDU_PCI_TYPE_UNKNOWN)
                                        {
                                            /* Fill Frame PCI field */
                                            /* Save the previous PCI */
                                            LINS_txPrevPci = LINS_PDU_PCI_TYPE_FF;
                                            LINS_tlFlags |= LINS_TL_N_AS_TIMEOUT_ON;
                                            LINS_tlTimeoutCnt = 0u;

                                        #if (LINS_COOKED_TL_API)
                                            /* Fill Frame NAD field */
                                            #if (LINS_SAE_J2602_ENABLED)
                                                /* Insert J2602 status byte as byte 0 of the frame *
                                                * for MRF with ID = 0x3E or NAD >= 0x80.              */
                                                if (0u != (LINS_auxStatus & LINS_AUXSTS_MRF_REQUIRES_J2602STATUS))
                                                { 
                                                    LINS_srfBuffer[0u] = LINS_j2602Status;
                                                }
                                                else
                                                {
                                                    LINS_srfBuffer[0u] = LINS_nad;    
                                                }
                                            #else
                                                LINS_srfBuffer[0u] = LINS_nad;
                                            #endif /* LINS_SAE_J2602_ENABLED */
                                            
                                            LINS_srfBuffer[1u] = (LINS_PDU_PCI_TYPE_FF |
                                                (HI8(LINS_txMessageLength)));
                                                
                                            /* Fill Frame LEN field */
                                            LINS_srfBuffer[2u] = LO8(LINS_txMessageLength);
                                            /* Fill Frame Data fields */
                                            for (i = 3u; i < LINS_FRAME_DATA_SIZE_8; i++)
                                            {
                                                LINS_srfBuffer[i] = LINS_txTlDataPointer[(i +
                                                    LINS_txTlDataCount) - 3u];
                                            }
                                            /* Update user buffer pointer */
                                            LINS_txTlDataCount += LINS_FRAME_DATA_SIZE_5;
                                            LINS_txMessageLength -= LINS_FRAME_DATA_SIZE_5;
                                        #endif /* LINS_COOKED_TL_API */
                                        }
                                        else    /* Process the CF Frame */
                                        {
                                        #if (LINS_COOKED_TL_API)
                                            #if (LINS_SAE_J2602_ENABLED)
                                                /* Inserts J2602 status byte as byte 0 of the frame */
                                                /* for MRF with ID = 0x3E or NAD >= 0x80.           */
                                                if (0u != (LINS_auxStatus & LINS_AUXSTS_MRF_REQUIRES_J2602STATUS))
                                                { 
                                                    LINS_srfBuffer[0u] = LINS_j2602Status;
                                                }
                                            #endif /* LINS_SAE_J2602_ENABLED */
                                             /* Fills Frame PCI field */
                                            LINS_srfBuffer[1u] = (LINS_PDU_PCI_TYPE_CF |
                                                LINS_txFrameCounter);
                                            /* Fills Frame Data fields */
                                            for (i = 2u; i < LINS_FRAME_DATA_SIZE_8; i++)
                                            {
                                                LINS_srfBuffer[i] = LINS_txTlDataPointer[(i +
                                                    LINS_txTlDataCount) - 2u];
                                            }
                                            /* Updates the user buffer pointer */
                                            LINS_txTlDataCount += LINS_FRAME_DATA_SIZE_6;
                                        #endif /* LINS_COOKED_TL_API */

                                            /* Saves the previous PCI */
                                            LINS_txPrevPci = LINS_PDU_PCI_TYPE_CF;
                                            
                                            /* Update length pointer properly */
                                            LINS_txMessageLength -= LINS_FRAME_DATA_SIZE_6;
                                            LINS_tlFlags |= LINS_TL_N_AS_TIMEOUT_ON;
                                            LINS_tlTimeoutCnt = 0u;
                                        }
                                    }
                                    else    /* Processes SF Frame or last CF Frame */
                                    {
                                        /* Checks if the Previous frame is in "Unknown" state, which indicates that
                                         * the current frame is SF, otherwise it is the last CF frame.
                                         * Fill the Frame PCI field properly.
                                         */
                                        LINS_tlFlags |= LINS_TL_N_AS_TIMEOUT_ON;
                                        LINS_tlTimeoutCnt = 0u;

                                            if (LINS_PDU_PCI_TYPE_UNKNOWN == LINS_txPrevPci)
                                            {
                                            #if (LINS_COOKED_TL_API)
                                                /* Fill Frame NAD field */
                                                #if (LINS_SAE_J2602_ENABLED)
                                                /* Inserts J2602 status byte as byte 0 of the frame */
                                                /* for MRF with ID = 0x3E or NAD >= 0x80.           */
                                                if (0u !=(LINS_auxStatus & LINS_AUXSTS_MRF_REQUIRES_J2602STATUS))
                                                { 
                                                    LINS_srfBuffer[0u] = LINS_j2602Status;
                                                }
                                                else
                                                {
                                                    LINS_srfBuffer[0u] = LINS_nad;    
                                                }
                                                #else
                                                LINS_srfBuffer[0u] = LINS_nad;
                                                #endif /* LINS_SAE_J2602_ENABLED */
                                                LINS_srfBuffer[1u] = (l_u8)LINS_txMessageLength;
                                            #endif /* LINS_COOKED_TL_API */
                                                /* Save the previous PCI */
                                                LINS_txPrevPci = LINS_PDU_PCI_TYPE_SF;
                                            }
                                            else
                                            {
                                            #if (LINS_COOKED_TL_API)
                                                /* Fill Frame NAD field */
                                                
                                                #if (LINS_SAE_J2602_ENABLED)
                                                    /* Inserts J2602 status byte as byte 0 of the frame */
                                                    /* for MRF with ID = 0x3E or NAD >= 0x80.           */
                                                    if (0u != (LINS_auxStatus & LINS_AUXSTS_MRF_REQUIRES_J2602STATUS))
                                                    { 
                                                        LINS_srfBuffer[0u] = LINS_j2602Status;
                                                    }
                                                    else
                                                    {
                                                        LINS_srfBuffer[0u] = LINS_nad;    
                                                    }
                                                #else
                                                    LINS_srfBuffer[0u] = LINS_nad;
                                                #endif /* LINS_SAE_J2602_ENABLED */
                                                
                                                LINS_srfBuffer[1u] = (LINS_PDU_PCI_TYPE_CF |
                                                    LINS_txFrameCounter);
                                            #endif /* LINS_COOKED_TL_API */
                                            
                                                /* Save previous PCI */
                                                LINS_txPrevPci = LINS_PDU_PCI_TYPE_CF;
                                            }

                                    #if (LINS_COOKED_TL_API)
                                        /* Fill Frame Data fields */
                                        for (i = 2u; i < LINS_FRAME_DATA_SIZE_8; i++)
                                        {
                                            if (LINS_txMessageLength >= ((l_u8) (i - 1u)))
                                            {
                                                LINS_srfBuffer[i] = LINS_txTlDataPointer[(i +
                                                    LINS_txTlDataCount) - 2u];
                                            }
                                            else
                                            {
                                                /* Fill unused data bytes with FFs */
                                                LINS_srfBuffer[i] = 0xFFu;
                                            }
                                        }
                                        /* Update the user buffer pointer */
                                        LINS_txTlDataCount += LINS_FRAME_DATA_SIZE_6;
                                    #endif /* LINS_COOKED_TL_API */

                                        /* Update length pointer properly */
                                        if (LINS_txMessageLength > LINS_FRAME_DATA_SIZE_6)
                                        {
                                            LINS_txMessageLength -= LINS_FRAME_DATA_SIZE_6;
                                        }
                                        else
                                        {
                                            LINS_txMessageLength = 0u;
                                        }
                                    }
                                    /* Update the frame counter */
                                    if (LINS_txFrameCounter != 15u)
                                    {
                                        LINS_txFrameCounter++;
                                    }
                                    else
                                    {
                                        /* If frame counter is larger than 15, reset it */
                                        LINS_txFrameCounter = 0u;
                                    }
                                    /* Frame equals 8 bytes */
                                    LINS_frameSize = LINS_FRAME_DATA_SIZE_8;
                                    /* Set frame data pointer to a start of a frame buffer */
                                    LINS_frameData = LINS_srfBuffer;
                                    
                                    /* Send first byte to the LIN Master */
                                    LINS_tmpData = *LINS_frameData;
                                    LINS_frameData++;
                                    LINS_SCB_SpiUartWriteTxData((uint32)LINS_tmpData);
                                    LINS_bytesTransferred = 1u;
                                    
                                    /* Switch to the publish data state. */
                                    LINS_uartFsmState = LINS_UART_ISR_STATE_2_TX;
                                    
                                    /* One or more data bytes are received */
                                    LINS_fsmFlags |= LINS_FSM_DATA_RECEIVE;
                                    
                                    /* Indicates to transport layer that Slave is transmitting the frame */
                                    LINS_tlFlags |= LINS_TL_TX_DIRECTION;

                                    /* Checks if the SRF is Pending response frame */
                                    if ((LINS_srfBuffer[LINS_PDU_PCI_IDX] == 0x03u) &&
                                        (LINS_srfBuffer[LINS_PDU_SID_IDX] == 0x7Fu) &&
                                        (LINS_srfBuffer[LINS_PDU_D2_IDX ] == 0x78u)   )
                                    {
                                        LINS_status |= LINS_STATUS_RESPONSE_PENDING;
                                    }
                                    else
                                    {
                                        LINS_status &= (l_u8) ~LINS_STATUS_RESPONSE_PENDING;
                                    }
                                }
                            }
                            else
                            {
                                /* Reset UART State Machine */
                                LINS_EndFrame(LINS_HANDLING_RESET_FSM_ERR);
                            }
                        }
                    } /* If SRF */
                #else
                    /* These are invalid PIDs when TL is disabled - reset UART state machine */
                
                    /* Set response error */
                    #if (LINS_SAE_J2602_ENABLED)
                        /* Set ERR2, ERR1 and ERR0 bits */
                        LINS_j2602Status_Set(LINS_J2602_STS_PARITY_ERR);
                        LINS_ifcStatus |= LINS_IFC_STS_ERROR_IN_RESPONSE;
                        
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LINS_Buffer_OutFrame[LINS_Response_Error_OutFrame_SIG_BYTE_OFFSET] |= 
                            LINS_Response_Error_OutFrame_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

                    #endif /* LINS_SAE_J2602_ENABLED */
                    
                    /* Reset UART State Machine */
                    LINS_EndFrame(LINS_HANDLING_RESET_FSM_ERR);
                #endif /* LINS_TL_USED */
                }
                else    /* Not MRF or SRF */
                {
                    /* Gets PID index in LINS_pidInfoTable */
                    LINS_pidIndex = LINS_FindPidIndex(LINS_framePid);
                    if (LINS_INVALID_FRAME_PID != LINS_pidIndex)
                    {
                    /* Valid ID */
                    #if (0u == LINS_LIN_1_3)
                        /* Starts enhanced checksum calculation  */
                        LINS_interimChecksum = LINS_framePid;
                    #endif /* (0u == LINS_LIN_1_3) */

                        /* Gets the size of the frame */
                        LINS_frameSize = LINS_pidInfoTable[LINS_pidIndex].param &
                                                        LINS_FRAME_DATA_SIZE_MASK;
                        /* TX response (publish action) is requested by Master */
                        if (0u != (LINS_FRAME_DIR_PUBLISH &
                               LINS_pidInfoTable[LINS_pidIndex].param))
                        {
                        /* SAE J2602 is disabled */
                        #if (!LINS_SAE_J2602_ENABLED)
                            /* This frame is event-triggered */
                            if (0u  != (LINS_FRAME_TYPE_EVENT &
                                       LINS_pidInfoTable[LINS_pidIndex].param))
                            {
                                /* Checks whether to process event-triggered frame */
                                if (0u == LINS_GetEtFlagValue(LINS_pidIndex))
                                {
                                   /* Resets UART State Machine */
                                    LINS_EndFrame(LINS_HANDLING_RESET_FSM_ERR);
                                   return;
                                }
                            }
                        #endif /* (!LINS_SAE_J2602_ENABLED) */

                            /* Gets the pointer to frame data */
                            LINS_frameData =
                                LINS_pidInfoTable[LINS_pidIndex].dataPtr;
                            /* Sends the first byte to LIN master */
                            LINS_tmpData = *LINS_frameData;

                        /* SAE J2602 is disabled */
                        #if (!LINS_SAE_J2602_ENABLED)
                            /* This frame is event-triggered */
                            if (0u != (LINS_FRAME_TYPE_EVENT &
                                       LINS_pidInfoTable[LINS_pidIndex].param))
                            {
                                /* Sets the first byte equal to associated unconditional frame's PID. */
                                LINS_tmpData = LINS_parityTable[
                                    LINS_pidInfoTable[LINS_pidIndex].associatedPID];
                            }
                            else
                            {
                                LINS_fsmFlags |= LINS_FSM_DATA_RECEIVE;
                            }
                        #endif /* (!LINS_SAE_J2602_ENABLED) */

                            LINS_frameData++;
                            LINS_SCB_SpiUartWriteTxData((uint32)LINS_tmpData);
                            LINS_bytesTransferred = 1u;
                            
                            /* Switches to publish data state. */
                            LINS_uartFsmState = LINS_UART_ISR_STATE_2_TX;
                            /* Sets DATA RECEIVE flag */
                            LINS_fsmFlags |= LINS_FSM_DATA_RECEIVE;
                        }
                        else    /* RX response (subscribe action) is requested by Master */
                        {
                            LINS_bytesTransferred = 0u;
                            /* Gets the pointer to the temp RX frame data buffer */
                            LINS_frameData = LINS_tmpRxFrameData;
                            /* Switches to subscribe data state. */
                            LINS_uartFsmState = LINS_UART_ISR_STATE_3_RX;
                        }
                    }
                    else    /* Invalid ID */
                    {
                        /* Resets UART State Machine */
                        LINS_EndFrame(LINS_HANDLING_RESET_FSM_ERR);
                    }
                }
            }
        break;
        /***********************************************************************
        *                       TX response (Publish)
        * State description:
        *  - Transmits data to LIN Master.
        *  - Transmits the next data byte if there have been no any errors.
        *  - Transmits checksum when data is send correctly.
        ***********************************************************************/
        case LINS_UART_ISR_STATE_2_TX:
            /* Sets the response active flag */
            LINS_status |= LINS_STATUS_RESPONSE_ACTIVE;
            
            /* Previously transmitted and read back bytes are not equal */
            if ((LINS_tmpData != (l_u8)LINS_SCB_SpiUartReadRxData()) ||
                (0u != (LINS_FSM_FRAMING_ERROR_FLAG & LINS_fsmFlags)))
            {
            /* Mismatch Error */
            #if (LINS_SAE_J2602_ENABLED)
                /* Sets ERR2 bit */
                LINS_j2602Status_Set(LINS_J2602_STS_DATA_ERR);
                /* Readback error - sets the response error flag */
                LINS_ifcStatus |= LINS_IFC_STS_ERROR_IN_RESPONSE;
                
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LINS_Buffer_OutFrame[LINS_Response_Error_OutFrame_SIG_BYTE_OFFSET] |= 
                            LINS_Response_Error_OutFrame_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

            #else
                /* Skips the event-triggered frame */
                if (0u  == (LINS_FRAME_TYPE_EVENT &
                            LINS_pidInfoTable[LINS_pidIndex].param))
                {
                    /* Readback error - sets the response error flag */
                    LINS_ifcStatus |= LINS_IFC_STS_ERROR_IN_RESPONSE;
                    
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LINS_Buffer_OutFrame[LINS_Response_Error_OutFrame_SIG_BYTE_OFFSET] |= 
                            LINS_Response_Error_OutFrame_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

                }
            #endif  /* LINS_SAE_J2602_ENABLED */

                /* Checks for framing error */
                if (0u == (LINS_fsmFlags & LINS_FSM_FRAMING_ERROR_FLAG))
                {
                    /* Saves the last processed on bus PID to status variable. */
                    LINS_ifcStatus &= ((l_u16) ~LINS_IFC_STS_PID_MASK);
                    LINS_ifcStatus |= ((l_u16) (((l_u16)LINS_framePid) << 8u));
                    #if (LINS_SAE_J2602_ENABLED)
                        if (LINS_framePid == LINS_FRAME_PID_MRF_J2602)
                        {
                            LINS_auxStatus |= LINS_AUXSTS_MRF_WAS_3E;
                        }
                        else
                        {
                            LINS_auxStatus &= (l_u8)~LINS_AUXSTS_MRF_WAS_3E;
                        }
                    #endif  /* LINS_SAE_J2602_ENABLED */
                }
                /* End frame with response error */
                LINS_EndFrame(LINS_HANDLING_DONT_SAVE_PID);
            }
            else    /* If readback is successful, continue transmitting */
            {
                /* Adds the transmitted byte to interim checksum. */
                LINS_interimChecksum += LINS_tmpData;
                if (LINS_interimChecksum >= 256u)
                {
                    LINS_interimChecksum -= 255u;
                }
                /* Checks to see if all data bytes are sent. */
                if (LINS_frameSize > LINS_bytesTransferred)
                {
                    /* Sends out the next byte of the buffer. */
                    LINS_tmpData = *LINS_frameData;
                    LINS_frameData++;
                    LINS_SCB_SpiUartWriteTxData((uint32)LINS_tmpData);
                    LINS_bytesTransferred++;
                }
                else    /* All data bytes are sent - compute and transmit the checksum. */
                {
                    /* Computes and sends out the checksum byte */
                    LINS_SCB_SpiUartWriteTxData((((l_u8)LINS_interimChecksum) ^ (uint32)0xFFu));
                    LINS_bytesTransferred = 0u;
                    /* Switches to the checksum state */
                    LINS_uartFsmState = LINS_UART_ISR_STATE_4_CHS;
                }
            }
        break;
        /***********************************************************************
        *                       RX response (Subscribe)
        * State description:
        *  - Receives data from LIN Master.
        *  - Received data saved to the temporary buffer.
        ***********************************************************************/
        case LINS_UART_ISR_STATE_3_RX:
            /* Sets Response error only if the error appears during receive */
            if (0u  != (LINS_FSM_FRAMING_ERROR_FLAG & LINS_fsmFlags))
            {
                /* This is a workaround to suppress Response Error when only Break header + PID are received. */
                if ((0u == LINS_bytesTransferred) && (0u == LINS_frameErrorAfterPID))
                {
                    /* Reads the erroneous byte from FIFO. */
                    (void)LINS_SCB_SpiUartReadRxData();
                    /* Bypasses the first Framing error after PID */
                    LINS_frameErrorAfterPID = 1u;
                }
                else
                {
                    /* Sets a response error */
                    LINS_ifcStatus |= LINS_IFC_STS_ERROR_IN_RESPONSE;
                    
                #if (LINS_SAE_J2602_ENABLED)
                    /* Sets framing error bits */
                   LINS_j2602Status_Set(LINS_J2602_STS_FRAMING_ERR);
                #endif /* LINS_SAE_J2602_ENABLED */

                   
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LINS_Buffer_OutFrame[LINS_Response_Error_OutFrame_SIG_BYTE_OFFSET] |= 
                            LINS_Response_Error_OutFrame_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);


                   /* Clear FE  flag and DATA_RECEIVE flags anyway */
                   LINS_fsmFlags &= (l_u8)(~LINS_FSM_DATA_RECEIVE);
                    /* Finishes frame processing */
                    LINS_EndFrame(LINS_HANDLING_DONT_SAVE_PID);
                }
                /* Clear FE  flag and DATA_RECEIVE flags anyway */
                LINS_fsmFlags &= (l_u8)(~LINS_FSM_FRAMING_ERROR_FLAG);
            }
            else
            {
                /* Saves received byte */
                LINS_tmpData = (l_u8)LINS_SCB_SpiUartReadRxData();
                *LINS_frameData = LINS_tmpData;
                LINS_frameData++;
                LINS_bytesTransferred++;
                
                /* Sets the response active flag */
                LINS_status |= LINS_STATUS_RESPONSE_ACTIVE;
                
                /* One or more data bytes are received */
                LINS_fsmFlags |= LINS_FSM_DATA_RECEIVE;
                
                /* Adds the received byte to  theinterim checksum. */
                LINS_interimChecksum += LINS_tmpData;
                
                if (LINS_interimChecksum >= 256u)
                {
                    LINS_interimChecksum -= 255u;
                }
                /* Checks if the data section completed. */
                if (LINS_frameSize > LINS_bytesTransferred)
                {
                    /* There is data to be received. */
                }
                else
                {
                    /* There is no data to be received. */
                    LINS_bytesTransferred = 0u;
                    /* Switches to the checksum state. */
                    LINS_uartFsmState = LINS_UART_ISR_STATE_4_CHS;
                }
            }
        break;
        /***********************************************************************
        *                              Checksum
        ***********************************************************************/
        case LINS_UART_ISR_STATE_4_CHS:
            /* Previously transmitted and read back bytes are not equal */
            if (((((l_u8) LINS_interimChecksum) ^ 0xFFu) != (l_u8) LINS_SCB_SpiUartReadRxData())
                || (0u != (LINS_fsmFlags & LINS_FSM_FRAMING_ERROR_FLAG)))
            {
                /* Mismatch or Checksum Error */
                /* Sets a response error */
                LINS_ifcStatus |= LINS_IFC_STS_ERROR_IN_RESPONSE;
                
            #if (LINS_SAE_J2602_ENABLED)
                /* Sets the ERR2 and ERR0 bits */
                LINS_j2602Status_Set(LINS_J2602_STS_CHECKSUM_ERR);
            #endif /* LINS_SAE_J2602_ENABLED */

                
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LINS_Buffer_OutFrame[LINS_Response_Error_OutFrame_SIG_BYTE_OFFSET] |= 
                            LINS_Response_Error_OutFrame_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

                
                

                /* Checks for a framing error */
                if (0u == (LINS_fsmFlags & LINS_FSM_FRAMING_ERROR_FLAG))
                {
                    /* Saves the last processed on bus PID to the status variable. */
                    LINS_ifcStatus &= ((l_u16) ~LINS_IFC_STS_PID_MASK);
                    LINS_ifcStatus |= ((l_u16) (((l_u16)LINS_framePid) << 8u));
                }
                /* Resets the UART state machine with a checksum or mismatch error. */
                LINS_EndFrame(LINS_HANDLING_DONT_SAVE_PID);
            }
            else
            {
            /* Clears all error bits in the interface status. */
            #if (LINS_SAE_J2602_ENABLED)
                /* J2602 errors are retained until they are successfully reported 
                 * to the Master node in a message without any detected errors. 
                 * If frame containing J2602_Status was sent (not 3C frames)
                 * clears last reported error state.
                 */
                 if (((LINS_framePid & LINS_PID_PARITY_MASK) < 
                    (LINS_FRAME_PID_MRF & LINS_PID_PARITY_MASK)) || 
                    ((0u != (LINS_auxStatus & LINS_AUXSTS_MRF_REQUIRES_J2602STATUS)) &&
                    (LINS_framePid == LINS_FRAME_PID_SRF)))
                 {
                    LINS_j2602Status_Clear();    /* clears only last reported error state */
                 }
            #endif  /* LINS_SAE_J2602_ENABLED */

                /* Clears the framing error and data receive flags */
                LINS_fsmFlags &=
                   ((l_u8) ~(LINS_FSM_FRAMING_ERROR_FLAG | LINS_FSM_DATA_RECEIVE));
                /* Sets the successful transfer interface flag */
                LINS_ifcStatus |= LINS_IFC_STS_SUCCESSFUL_TRANSFER;
                /* Saves the last processed on bus PID to the status variable */
                LINS_ifcStatus &= ((l_u16) ~LINS_IFC_STS_PID_MASK);
                LINS_ifcStatus |= ((l_u16) (((l_u16)LINS_framePid) << 8u));
                /* Sets the overrun interface flag */
                if (0u != (LINS_FSM_OVERRUN & LINS_fsmFlags))
                {
                    LINS_ifcStatus |= LINS_IFC_STS_OVERRUN;
                }
                /* Sets the Overrun flag */
                LINS_fsmFlags |= LINS_FSM_OVERRUN;
                
                /* Clears response error signal if the frame contains RESPONSE ERROR signal */
            #if (1u == LINS_RESPONSE_ERROR_SIGNAL)
                if (LINS_RESPONSE_ERROR_FRAME_INDEX == LINS_pidIndex)
                {
                    
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LINS_Buffer_OutFrame[LINS_Response_Error_OutFrame_SIG_BYTE_OFFSET] &= 
                            (l_u8) ~LINS_Response_Error_OutFrame_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);


                }
            #endif /* (1u == LINS_RESPONSE_ERROR_SIGNAL) */

                if ( (!LINS_TL_USED) || (LINS_TL_USED && 
                     (!((LINS_FRAME_PID_MRF == LINS_framePid) ||
                        (LINS_FRAME_PID_SRF == LINS_framePid)))) )
                {
                /* SAE J2602 is disabled */
                #if (!LINS_SAE_J2602_ENABLED)
                    /* This frame is event-triggered */
                    if (0u != (LINS_FRAME_TYPE_EVENT &
                               LINS_pidInfoTable[LINS_pidIndex].param))
                    {
                        /* Clears event-triggered flags */
                        LINS_ClearEtFlagValue(LINS_pidIndex);
                        /* Resets UART State Machine */
                        LINS_EndFrame(LINS_HANDLING_RESET_FSM_ERR);
                    }
                #endif /* (!LINS_SAE_J2602_ENABLED) */

                    /* Sets associated with current frame flags */
                    LINS_SetAssociatedFlags(LINS_pidIndex);
                }

            #if (LINS_TL_USED)
                /* Checks if received data is a "master request frame" */
                #if (LINS_SAE_J2602_ENABLED)
                if ((LINS_FRAME_PID_MRF == LINS_framePid) ||
                    (LINS_FRAME_PID_MRF_J2602 == LINS_framePid))
                #else
                if (LINS_FRAME_PID_MRF == LINS_framePid)
                #endif /* LINS_SAE_J2602_ENABLED */
                {                    
                    l_bool LINS_NAD_accepted;     /* Received NAD accepted */
                    /* Check NAD */
                    LINS_NAD_accepted = (l_bool)((LINS_nad == LINS_mrfBuffer[LINS_PDU_NAD_IDX]) ||
                                                             (LINS_NAD_BROADCAST == LINS_mrfBuffer[LINS_PDU_NAD_IDX]) ||
                                                             (LINS_LinSlaveConfig.initialNad == LINS_mrfBuffer[LINS_PDU_NAD_IDX]));                     

                #if (LINS_SAE_J2602_ENABLED)                    
                    l_bool LINS_is_own_DNN;       /* Received DNN accepted */
                    /* Check DNN */
                    LINS_is_own_DNN = (l_bool)((0x0Fu & LINS_nad) == 
                        (0x0Fu & LINS_mrfBuffer[LINS_PDU_NAD_IDX]));
                #endif /* LINS_SAE_J2602_ENABLED */
                    
                    if ((LINS_NAD_GO_TO_SLEEP == LINS_mrfBuffer[LINS_PDU_NAD_IDX])
                        #if (LINS_SAE_J2602_ENABLED)
                            /* For J2602 0x00 is a valid NAD for 0x3E requests, don't go to sleep */
                            && (LINS_FRAME_PID_MRF == LINS_framePid)
                       #endif /* LINS_SAE_J2602_ENABLED */
                       )
                    {
                        LINS_ifcStatus |= LINS_IFC_STS_GO_TO_SLEEP;
                    } 
    
                #if (LINS_SAE_J2602_ENABLED)
                    /* J2602 allows additional NADs above 0x7F, where lower nibble equals DNN */ 
                    else if ((0u != LINS_NAD_accepted)       ||
                         ((0u != LINS_is_own_DNN) &&
                         (LINS_mrfBuffer[LINS_PDU_NAD_IDX] > LINS_NAD_BROADCAST) &&
                            (LINS_FRAME_PID_MRF == LINS_framePid))      ||
                         ((0u != LINS_is_own_DNN) &&
                            (LINS_FRAME_PID_MRF_J2602 == LINS_framePid)))
                #else
                    else if (0u != LINS_NAD_accepted)                        
                #endif /* LINS_SAE_J2602_ENABLED */
                    {
                        LINS_CheckNodeState(LINS_MRF_OWN_NAD);
                    }
                    else if (LINS_NAD_FUNCTIONAL == LINS_mrfBuffer[LINS_PDU_NAD_IDX])
                    {
                        LINS_CheckNodeState(LINS_MRF_FUNC_NAD);
                    }
                    else
                    {
                        LINS_CheckNodeState(LINS_MRF_ALIEN_NAD);
                    }
                    /* Clears TL RX direction flag */
                    LINS_tlFlags &= ((l_u8) ~LINS_TL_RX_DIRECTION);
                    /* Resets UART state machine */
                    LINS_EndFrame(LINS_HANDLING_RESET_FSM_ERR);
                }
                else if (LINS_FRAME_PID_SRF == LINS_framePid)
                {
                    LINS_CheckNodeState(LINS_SRF);
                    /* Resets UART state machine */
                    LINS_EndFrame(LINS_HANDLING_RESET_FSM_ERR);
                }
                else
                {
                    /* RX response (subscribe action) is requested by Master */
                    if (0u == (LINS_FRAME_DIR_PUBLISH &
                           LINS_pidInfoTable[LINS_pidIndex].param))
                    {
                    interruptState = CyEnterCriticalSection();
                        /* Copoies received data from temporary buffer to frame buffer */
                        for (i = 0u; i < LINS_frameSize; i++)
                        {
                            *(LINS_pidInfoTable[LINS_pidIndex].dataPtr + i) =
                                LINS_tmpRxFrameData[i];
                        }
                    CyExitCriticalSection(interruptState);
                    }
                    /* Resets UART state machine */
                    LINS_EndFrame(LINS_HANDLING_RESET_FSM_ERR);
                }
            #else
                /* RX response (subscribe action) is requested by Master */
                if (0u == (LINS_FRAME_DIR_PUBLISH &
                           LINS_pidInfoTable[LINS_pidIndex].param))
                {
                interruptState = CyEnterCriticalSection();
                    /* Copies received data from temporary buffer to frame buffer */
                    for (i = 0u; i < LINS_frameSize; i++)
                    {
                        *(LINS_pidInfoTable[LINS_pidIndex].dataPtr + i) =
                            LINS_tmpRxFrameData[i];
                    }
                CyExitCriticalSection(interruptState);
                }

                /* Resets UART state machine */
                LINS_EndFrame(LINS_HANDLING_RESET_FSM_ERR);
            #endif  /* LINS_TL_USED */
            }
        break;
        default:
            /* Resets UART state machine */
            LINS_EndFrame(LINS_HANDLING_RESET_FSM_ERR);
        break;
    }

    /* Clears SCB UART interrupt */
    LINS_SCB_INTR_RX_REG = LINS_SCB_INTR_RX_NOT_EMPTY;

    #ifdef LINS_L_IFC_RX_CALLBACK
        l_ifc_rx_LINS_Callback();
    #endif /* LINS_L_IFC_RX_CALLBACK */
}


/******************************************************************************
* Function Name: l_ifc_tx
*******************************************************************************
*
* Summary:
*  The LIN Slave component calls this API routine automatically.
*  Therefore, this API routine must not be called by the application code.
*
******************************************************************************/
void l_ifc_tx_LINS(void)
{
    l_ifc_rx_LINS();
}


/******************************************************************************
* Function Name: l_ifc_aux
*******************************************************************************
*
* Summary:
*  The LIN Slave component calls this API routine automatically.
*  Therefore, this API routine must not be called by the application code.
*
******************************************************************************/
void l_ifc_aux_LINS(void)
{
    /**************************************************************************
    *                       Bus Inactivity Interrupt Detected
    **************************************************************************/
    #if (1u == LINS_INACTIVITY_ENABLED)
            #if (LINS_TL_USED)
                /* If timeout is enabled, proceeds the timeout manage. */
                if (0u !=(LINS_tlFlags & LINS_TL_N_AS_TIMEOUT_ON))
                {
                    /* Increments the timeout. */
                    LINS_tlTimeoutCnt++;

                    if ((LINS_TL_N_AS_TIMEOUT_VALUE) <= LINS_tlTimeoutCnt)
                    {
                        LINS_CheckNodeState(LINS_TX_TIMEOUT);
                    }
                }
                else if (0u !=(LINS_tlFlags & LINS_TL_N_CR_TIMEOUT_ON))
                {
                    /* Increments the timeout */
                    LINS_tlTimeoutCnt++;

                    if ((LINS_TL_N_CR_TIMEOUT_VALUE) <= LINS_tlTimeoutCnt)
                    {
                        LINS_CheckNodeState(LINS_RX_TIMEOUT);
                    }
                }
                else
                {
                    /* Resets the timeout counter */
                    LINS_tlTimeoutCnt = 0u;
                }
            #endif /* LINS_TL_USED */

            if ((LINS_INACTIVITY_THRESHOLD_IN_1_MS) <= LINS_periodCounter)
            {
                /* Inactivity threshold is achieved. */

                /* Sets the bus inactivity ioctl status bit */
                LINS_ioctlStatus |= LINS_IOCTL_STS_BUS_INACTIVITY;
            }
            else
            {
                LINS_periodCounter++;
            }
    #endif  /* (1u == LINS_INACTIVITY_ENABLED) */
        
    /***************************************************************************
    *                       Brake to Sync Timeout Tick
    ***************************************************************************/
    #if (LINS_SAE_J2602_ENABLED && (0u == LINS_AUTO_BAUD_RATE_SYNC))
    /* Counts time from the break detect interrupt to latest time of the SYNC-PID sequence. */
    if (0u != LINS_breakToSyncCounts) /* BREAK to SYNC timer counts if >0 */
    {
        LINS_breakToSyncCounts++; /* Increments time from the break detect interrupt. */

        /* If Break to Sync timeout is reached, checks the baud detect register two times. */
        if (LINS_breakToSyncCounts >= LINS_BREAK_TO_SYNC_TIMEOUT)
        {
            if (LINS_breakToSyncCounts == LINS_BREAK_TO_SYNC_TIMEOUT)
            {
                /* Saves the first sample of the baud detect register */
                LINS_breakToSyncCounts1 = (l_u16)(LINS_SCB_UART_RX_STATUS_REG &
                LINS_SCB_UART_RX_STATUS_BR_COUNTER_MASK);
            }
            if (LINS_breakToSyncCounts == (LINS_BREAK_TO_SYNC_TIMEOUT+1u))
            {
                /* Saves the second sample of the baud detect register after one ms */
                LINS_breakToSyncCounts2 = (l_u16)(LINS_SCB_UART_RX_STATUS_REG &
                LINS_SCB_UART_RX_STATUS_BR_COUNTER_MASK);
                
                if (LINS_breakToSyncCounts2 > LINS_breakToSyncCounts1)
                /* Baud detection failed. Baud detect counter is still running after a timeout. */
                {
                    LINS_j2602Status_Set(LINS_J2602_STS_DATA_ERR);                
                }
                LINS_breakToSyncCounts = 0u; /* stop the BREAK to SYNC timer */
            } 
        }
    }
    #endif /* LINS_SAE_J2602_ENABLED && (0u == LINS_AUTO_BAUD_RATE_SYNC)*/

    #ifdef LINS_L_IFC_AUX_CALLBACK
        l_ifc_aux_LINS_Callback();
    #endif /* LINS_L_IFC_AUX_CALLBACK */
}


/******************************************************************************
* Function Name: l_ifc_read_status_LINS
*******************************************************************************
*
* Summary:
*  This function is defined by the LIN specification. This returns the status of
*  the specified LIN interface and clears all status bits for that
*  interface. See Section 7.2.5.8 of the LIN 2.1 specification.
*
* Return:
*  The status bits of the specified LIN interface are returned. These bits have
*  the following meanings:
*    [15:8]    Last Received PID
*    [7]        0
*    [6]        Save Configuration flag
*    [5]        0
*    [4]        Bus Activity flag
*    [3]        Go To Sleep flag
*    [2]        Overrun flag
*    [1]        Successful Transfer flag
*    [0]        Error in Response flag
*
******************************************************************************/
l_u16 l_ifc_read_status_LINS(void)
{
    l_u16 returnValue;
    l_u8 interruptState;

    interruptState = CyEnterCriticalSection();

    /* Copies the global status variable to local temp variable. */
    returnValue = LINS_ifcStatus;

    /* Clears the status variable */
    LINS_ifcStatus &= (l_u16) ~LINS_IFC_STS_MASK;

    CyExitCriticalSection(interruptState);
    
    /* Returns the status in temp variable */
    return (returnValue);
}


/******************************************************************************
* Function Name: LINS_FindPidIndex
*******************************************************************************
*
* Summary:
*  Returns the PID index in LINS_volatileConfig.
*
* Parameters:
*  l_u8 pid - PID of the frame whose index is required.
*
* Return:
*  The index if PID in LINS_volatileConfig.
*  0xFFu - If PID is not found.
*
*******************************************************************************/
static l_u8 LINS_FindPidIndex(l_u8 pid)
{
    l_u8 i;
    l_u8 returnValue = LINS_INVALID_FRAME_PID;

    for (i = 0u; i < LINS_NUM_FRAMES; i++)
    {
        if (pid == LINS_volatileConfig[i])
        {
            returnValue = i;
            /* Break the loop */
            break;
        }
    }

    return (returnValue);
}


/*******************************************************************************
* Function Name: LINS_EndFrame
********************************************************************************
*
* Summary:
*  Finishes the frame transmission.
*
*******************************************************************************/
static void LINS_EndFrame(l_u8 status)
{
    l_u8 interruptState;

    /* Clears the data received flag */
    LINS_fsmFlags &= ((l_u8) ~LINS_FSM_DATA_RECEIVE);
    
#if (LINS_TL_USED)
    /* Clears the TL TX direction flag */
    LINS_tlFlags &= ((l_u8) ~LINS_TL_TX_DIRECTION);
#endif /* LINS_TL_USED */

    if (status == LINS_HANDLING_DONT_SAVE_PID)
    {
        /* Clears the data received flag */
        LINS_fsmFlags &= ((l_u8) ~LINS_FSM_DATA_RECEIVE);
        if (0u != (LINS_fsmFlags & LINS_FSM_OVERRUN))
        {
            /* Sets overrun */
            LINS_ifcStatus |= LINS_IFC_STS_OVERRUN;
        }
    }
    if (status == LINS_HANDLING_SKIP_OVERRUN)
    {
        /* Sets the overrun flag */
        LINS_fsmFlags |= LINS_FSM_OVERRUN;
        
    #if (LINS_TL_USED)
        if (0u != (LINS_tlFlags & LINS_TL_TX_DIRECTION))
        {
        /* Transport Layer Functions: Cooked Transport Layer API */
        #if (LINS_COOKED_TL_API)
            /* Sets TL TX error status */
            LINS_txTlStatus = LD_FAILED;
        #else
            /* Sets TL TX error status */
            LINS_txTlStatus = LD_TRANSMIT_ERROR;
        #endif /* LINS_COOKED_TL_API */

            /* Clears TL flags register */
            LINS_tlFlags = 0u;
        }

        if (0u != (LINS_tlFlags & LINS_TL_RX_DIRECTION))
        {
        #if (LINS_COOKED_TL_API)
            /* Sets TL RX error status */
            LINS_rxTlStatus = LD_FAILED;
        #else
            /* Sets TL RX error status */
            LINS_rxTlStatus = LD_RECEIVE_ERROR;
        #endif /* LINS_COOKED_TL_API */

            /* Clears TL flags register */
            LINS_tlFlags = 0u;
        }
    #endif /* LINS_TL_USED */
    }
    else if ((status == LINS_HANDLING_RESET_FSM_ERR) || (status == LINS_HANDLING_DONT_SAVE_PID))
    {
        /* Clears UART enable flag */
        LINS_fsmFlags &= ((l_u8) ~(LINS_FSM_UART_ENABLE_FLAG |
                        LINS_FSM_FRAMING_ERROR_FLAG));

    interruptState = CyEnterCriticalSection();
    
        /* Clears RX and TX FIFOs after Frame or Overrun error */
        LINS_SCB_SpiUartClearRxBuffer();
        LINS_SCB_SpiUartClearTxBuffer();
        LINS_SCB_INTR_RX_REG = LINS_SCB_INTR_RX_NOT_EMPTY;
        
        /* Set IDLE FSM State */
        LINS_uartFsmState = LINS_UART_ISR_STATE_0_IDLE;
        
    CyExitCriticalSection(interruptState);
    }
    else
    {
    /* MISRA requires this else. Rule-14.10 */
    }
}


/*******************************************************************************
* Function Name: LINS_SetAssociatedFlags
********************************************************************************
*
* Summary:
*  This function sets appropriate flags. Used by ISR.
*
* Parameters:
*  The PID index in LINS_LinSlaveConfig.
*
*******************************************************************************/
static void LINS_SetAssociatedFlags(l_u8 pIndex)
{
    switch(pIndex)
    {
        case 0u:
            /* Mark frame "InFrame" (Type:Unconditional,Associated With:None) and associated signals processed. */ 
            LINS_statusFlagArray[LINS_ISR_AUX_SET_FLAGS_InFrame_FLAG_BYTE_OFFSET_0] |=
                LINS_ISR_AUX_SET_FLAGS_InFrame_FLAG_MASK_0;
            LINS_statusFlagArray[LINS_ISR_AUX_SET_FLAGS_InFrame_FLAG_BYTE_OFFSET_1] |=
                LINS_ISR_AUX_SET_FLAGS_InFrame_FLAG_MASK_1;
        break;

        case 1u:
            /* Mark frame "OutFrame" (Type:Unconditional,Associated With:None) and associated signals processed. */ 
            LINS_statusFlagArray[LINS_ISR_AUX_SET_FLAGS_OutFrame_FLAG_BYTE_OFFSET_0] |=
                LINS_ISR_AUX_SET_FLAGS_OutFrame_FLAG_MASK_0;
            LINS_statusFlagArray[LINS_ISR_AUX_SET_FLAGS_OutFrame_FLAG_BYTE_OFFSET_1] |=
                LINS_ISR_AUX_SET_FLAGS_OutFrame_FLAG_MASK_1;
        break;


        default:
        break;    
    }
}


/* SAE J2602 is disabled */
#if (!LINS_SAE_J2602_ENABLED)

/*******************************************************************************
* Function Name: LINS_GetEtFlagValue
********************************************************************************
*
* Summary:
*  This function gets the value of an appropriate event-triggered frame flag.
*
* Parameters:
*  The PID index in LINS_LinSlaveConfig.
*
* Return:
*  Current flag value.
*
*******************************************************************************/
static l_bool LINS_GetEtFlagValue(l_u8 pidIndex)
{
    l_bool returnValue;

    switch(pidIndex)
    {
        default:
             returnValue = LINS_FALSE;
        break;    
    }

    return (returnValue);
}



/*******************************************************************************
* Function Name: LINS_ClearEtFlagValue
********************************************************************************
*
* Summary:
*  This function clears the value of an appropriate event-triggered frame flag.
*
* Parameters:
*  The PID index in LINS_LinSlaveConfig.
*
*******************************************************************************/
static void LINS_ClearEtFlagValue(l_u8 pidIndex)
{
    switch(pidIndex)
    {
        default:
        break;    
    }
}


#endif /* (!LINS_SAE_J2602_ENABLED) */


#if (LINS_TL_USED)

/*******************************************************************************
* Function Name: ld_init
********************************************************************************
*
* Summary:
*  This call (re)initializes the raw and cooked layers on interface
*  iii.
*
*  All transport layer buffers will be initialized.
*
*  If there is an ongoing diagnostic frame transporting a cooked or raw message
*  on the bus, it will not be aborted.
*
* Parameters:
*  iii - Interface.
*
*******************************************************************************/
void ld_init_LINS(void)
{
    l_u8 interruptState;

    #if (LINS_COOKED_TL_API)
        while (0u != (LINS_tlFlags & LINS_TL_TX_DIRECTION))
        {
            /* Waits until the current message is processed. */
        }

        /* Saves the interrupt state and disable interrupts. */
    interruptState = CyEnterCriticalSection();

        /* Initializes TX and RX status variables correctly */
        LINS_txTlStatus = LD_COMPLETED;
        LINS_rxTlStatus = LD_COMPLETED;

        LINS_tlLengthPointer = NULL;

        /* Resets frame counters */
        LINS_rxMessageLength = 0u;
        LINS_txMessageLength = 0u;

        /* Resets frame counters */
        LINS_txFrameCounter = 0u;
        LINS_rxFrameCounter = 0u;

        LINS_rxTlDataPointer = NULL;
        LINS_rxTlInitDataPointer = NULL;

        /* Previous PCI requires to be unknown after initialization */
        LINS_rxPrevPci = LINS_PDU_PCI_TYPE_UNKNOWN;
        LINS_txPrevPci = LINS_PDU_PCI_TYPE_UNKNOWN;
    #else
        /* Saves interrupt state and disable interrupts */
    interruptState = CyEnterCriticalSection();

        /* Resets buffers depth to 0 to indicate that buffers are empty. */
        LINS_txBufDepth = 0u;
        LINS_rxBufDepth = 0u;

        /* Raw API buffers initialization */
        LINS_txWrIndex = 0u;
        LINS_txRdIndex = 0u;

        LINS_rxWrIndex = 0u;
        LINS_rxRdIndex = 0u;

        LINS_txTlStatus = LD_QUEUE_EMPTY;
        LINS_rxTlStatus = LD_NO_DATA;
    #endif /* LINS_COOKED_TL_API */

    /* Sets initial NAD as the current active NAD before initializing TL */
    LINS_nad = LINS_LinSlaveConfig.initialNad;

    LINS_tlFlags = 0u;

    /* Changes the node state to next state */
   LINS_nodeState = LINS_IDLE;

    /* Enables interrupts */
    CyExitCriticalSection(interruptState);
}


/******************************************************************************
* Function Name: ld_read_configuration
*******************************************************************************
*
* Summary:
*  This function reads the NAD and PID values from the volatile memory.
*  This function reads the current configuration data, and
*  saves this data into the non-volatile (flash) memory. The application should
*  save the configuration data to the flash when the "Save Configuration" bit
*  is set in the LIN status register (returned by 
*  l_ifc_read_status_LINS).
*  The configuration data that is read is a series of bytes. The first byte is
*  the current NAD of the slave. The next bytes are the current PID values for
*  the frames that the slave responds to. The PID values are in the order in
*  which the frames appear in the LDF or NCF file.
*
* Parameters:
*  iii - Interface.
*
* Return:
*  LD_READ_OK - If the service is successful.
*
*  LD_LENGTH_TOO_SHORT - If the configuration size is greater than the length.
*                        It means that the data area does not contain valid
*                        configuration.
*
*******************************************************************************/
l_u8 ld_read_configuration_LINS(l_u8* pData, l_u8* const length)
{
    l_u8 i;
    l_u8 result = LINS_LD_READ_OK;

    if (*length < (LINS_NUM_FRAMES + 1u))
    {
        /* Return with no action when requested length is smaller
        *  than configuration data length.
        */
        result = LINS_LD_LENGTH_TOO_SHORT;
    }
    else
    {
        /* Copy over configured NAD */
        pData[0u] = LINS_nad;

        /* Copy data from PID array to data array */
        for (i = 0u; i < LINS_NUM_FRAMES; i++)
        {
            pData[i + 1u] = LINS_volatileConfig[i];
        }

        /* Set length parameter to actual length of configuration data */
        *length = LINS_NUM_FRAMES + 1u;
    }

    /* Return status */
    return (result);
}


/*******************************************************************************
* Function Name: ld_set_configuration
********************************************************************************
*
* Summary:
*  This call does not transport anything to the bus.
*
*  The function configures NAD and PIDs accordingly to the
*  configuration given by data. The intended usage is to restore the saved
*  configuration or set initial configuration (e.g. coded by I/O pins).
*
*  The function is called after calling l_ifc_init.
*
*  The caller must set the size of the data area before calling the function.
*
*  The data contains NAD and PIDs each occupying one byte.
*  The data structure is: NAD and all PIDs for the frames.
*  The PIDs order is the same as the frame list in LDF,
*  Section 9.2.2.2, and NCF, Section 8.2.5.
*
* Parameters:
*  iii - Interface.
*
* Return:
*  LD_SET_OK - If the service is successful.
*
*  LD_LENGTH_NOT_CORRECT - If the required size of the configuration is not
*                          equal to the given length.
*
*  LD_DATA_ERROR - The set of configuration could not be set. An error
*                  occurred while setting the configuration and the read back
*                  configuration settings don't match the required settings.
*
*  LINS_volatileConfig - New frame PIDs are copied.
*
*******************************************************************************/
l_u8 ld_set_configuration_LINS(const l_u8* const pData, l_u16 length)
{
    l_u8 i;
    l_u8 result = LINS_LD_SET_OK;

    if (length != (LINS_NUM_FRAMES + 1u))
    {
        /* Returns an error if the length isn't correct. */
        result = LINS_LD_LENGTH_NOT_CORRECT;
    }
    else
    {
        /* Copies NAD to volatile memory */
        LINS_nad = pData[0u];

        /* Data read back */
        if (LINS_nad != pData[0u])
        {
            /* Indicates a data error if NAD is not set correctly. */
            result = LINS_LD_DATA_ERROR;
        }

        /* Copies Frame PIDs to volatile memory */
        for (i = 0u; i < LINS_NUM_FRAMES; i++)
        {
            LINS_volatileConfig[i] = pData[i + 1u];

            /* Data read back */
            if (LINS_volatileConfig[i] != pData[i + 1u])
            {
                /* Indicates a data error if NAD is not set correctly. */
                result = LINS_LD_DATA_ERROR;
            }
        }
    }

    /* Returns success code if the copy is completed. */
    return(result);
}


#if (LINS_SAE_J2602_ENABLED)
    /*******************************************************************************
    * Function Name: LINS_UpdateMessageIDs
    ********************************************************************************
    *  [J2602-1 5.7.2.2 Message ID Assignment]
    * Summary:
    *  Updates message IDs and frame PIDs according to NAD.
    *  Messages with IDs 0x38, 0x39, 0x3A and 0x3B are considered broadcast messaes
    *  and are not updated.
    *
    * Parameters:
    *  l_u8 new NAD.
    *
    * Return:
    *  CYRET_SUCCESS      - On success.
    *  CYRET_BAD_PARAM    - On an error.
    *
    *******************************************************************************/
    static l_u8 LINS_UpdateMessageIDs(l_u8 newNAD)
    {
        l_u8 i = 0u;
        l_u8 returnValue = CYRET_SUCCESS;
        /* Message IDs 0x38, 0x39, 0x3A and 0x3B are not configurable. 
         * Excluded from frames count later.*/
        l_u8 configurable_frames = LINS_NUM_FRAMES;
        /* Checks if NAD is in range 0x60-0x6F */
        if ((newNAD & (l_u8)(~LINS_NAD_DNN_MASK)) == LINS_NAD_J2602_BASE)
        {             
            /* Checks for NADs 0x6E and 0x6F, which have no assigned frame IDs. */
            if ((newNAD == 0x6Eu) || (newNAD == LINS_NAD_UNINITIALIZED))
            {            
                for (i = 0u; i < LINS_NUM_FRAMES; i++)
                {
                    if ((messageIdTable[i] < LINS_FRAME_PID_BROADCAST_MIN) || 
                        (messageIdTable[i] > LINS_FRAME_PID_BROADCAST_MAX))
                    {
                        /* Write message ID table (these are unprotected IDs) */                
                        messageIdTable[i] = LINS_MESSAGE_ID_UNASSIGNED;
                        /* Write frame PIDs table (protected IDs are found in the lookup table)*/
                        LINS_volatileConfig[i] = LINS_FRAME_PID_UNASSIGNED;
                    }
                }
                returnValue = CYRET_SUCCESS;                
            }
            else
            {            
                /* Excludes messages with IDs 0x38, 0x39, 0x3A and 0x3B from frame count. */
                for (i = 0u; i < LINS_NUM_FRAMES; i++)
                {
                    if ((messageIdTable[i] >= LINS_FRAME_PID_BROADCAST_MIN) && 
                        (messageIdTable[i] <= LINS_FRAME_PID_BROADCAST_MAX))
                    {
                        configurable_frames--;
                    }
                }
                /* Checks for valid NAD according to configurable messages count 
                   (J2602-1 5.7.2.2 Message ID Assignment). */
                if (configurable_frames > 16u) /* 17 to 32 messages*/
                {
                    if (newNAD != LINS_NAD_J2602_BASE)
                    {
                        /* NAD is not 0x60 */
                        returnValue = CYRET_BAD_PARAM;
                    }
                }                
                else if (configurable_frames > 8u) /* 9 to 16 messages*/
                {
                    if ((0u != (newNAD & LINS_NAD_MULTIPLE4_MASK)) ||
                        (newNAD > 0x68u))
                    {
                        /* NAD is not multiple of 4 or NAD is above 0x68 */
                        returnValue = CYRET_BAD_PARAM;
                    }
                }
                else if (configurable_frames > 4u)  /* 5 to 8 messages */
                {
                    if ((0u != (newNAD & LINS_NAD_MULTIPLE2_MASK)) ||
                        (newNAD == 0x6Du))
                    {
                        /* NAD is not multiple of 2 */
                        returnValue = CYRET_BAD_PARAM;
                    }
                }
                else
                {
                    /* MISRA requires this else */
                }
                if (returnValue == CYRET_SUCCESS)
                {
                    /* Calculates new messages IDs based on newNAD*/
                    for (i = 0u; i < LINS_NUM_FRAMES; i++)
                    {
                        if ((messageIdTable[i] >= LINS_FRAME_PID_BROADCAST_MIN) && 
                            (messageIdTable[i] <= LINS_FRAME_PID_BROADCAST_MAX))
                        {
                            /* Stops if broadcast frames are reached. */
                            break; 
                        }
                        /* Write message ID table (these are unprotected IDs) */                
                        messageIdTable[i] = ((4u * ((l_u16)newNAD & LINS_NAD_DNN_MASK)) + i);
                        
                        /* Write frame PIDs table (protected IDs are found in lookup table) */
                        LINS_volatileConfig[i] = LINS_parityTable[messageIdTable[i] &
                                                                LINS_PID_PARITY_MASK];
                    }
                }
            }
        }
        else
        {
            /* NAD is outside the allowed range */
            returnValue = CYRET_BAD_PARAM;
        }
        return (returnValue);
    }
#endif  /* LINS_SAE_J2602_ENABLED */

#if (LINS_CFG_SERVICES_ENABLED)
    #if ((0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB0_SEL)) || \
         (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB2_SEL)))
        /*******************************************************************************
        * Function Name: LINS_LinProductId
        ********************************************************************************
        *
        * Summary:
        *  Verify that the received LIN product identification matches.
        *
        * Parameters:
        *  frameData - The pointer to the 4 bytes that hold LIN product ID.
        *
        * Return:
        *  0 - If the LIN product IDs doesn't match.
        *  1 - If the LIN product IDs match.
        *
        *******************************************************************************/
        static l_bool LINS_LinProductId(volatile const l_u8 frameData[])
        {
            l_bool i = LINS_TRUE;

            if ((frameData[0u] != LO8(LINS_slaveId.supplierId)) &&
                (frameData[0u] != LO8(LINS_CS_SUPPLIER_ID_WILDCARD)))
            {
                i = LINS_FALSE;        /* Data isn't for this slave */
            }

            if ((frameData[1u] != HI8(LINS_slaveId.supplierId)) &&
                (frameData[1u] != HI8(LINS_CS_SUPPLIER_ID_WILDCARD)))
            {
                i = LINS_FALSE;        /* Data isn't for this slave */
            }

            if ((frameData[2u] != LO8(LINS_slaveId.functionId)) &&
                (frameData[2u] != LO8(LINS_CS_FUNCTION_ID_WILDCARD)))
            {
                i = LINS_FALSE;        /* Data isn't for this slave */
            }

            if ((frameData[3u] != HI8(LINS_slaveId.functionId)) &&
                (frameData[3u] != HI8(LINS_CS_FUNCTION_ID_WILDCARD)))
            {
                i = LINS_FALSE;        /* Data isn't for this slave */
            }

            return (i);
        }
    #endif /* ((0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB0_SEL)) ||
           *  (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB2_SEL)))
           */
           
           
    #if (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB1_SEL))
        #if (1u == LINS_LIN_2_0)
            /*******************************************************************************
            * Function Name: LINS_MessageId
            ********************************************************************************
            *
            * Summary:
            *  Looks for the message ID in the LIN message ID table.In case of success
            *  returns message ID index in the table.
            *
            * Parameters:
            *  frameData - The data pointer points to a data area with 2 bytes.
            *
            * Return:
            *  message ID index - If operation is successful.
            *  LD_INVALID_MESSAGE_INDEX - If the message ID hasn't been found.
            *
            *******************************************************************************/
            static l_u8 LINS_MessageId(volatile const l_u8* frameData)
            {
                l_u8 i = 0u;
                l_u8 result = LD_INVALID_MESSAGE_INDEX;

                while ((i < LINS_NUM_FRAMES) && (result == LD_INVALID_MESSAGE_INDEX))
                {
                    /* If LSB of the message ID from table is equal to that received,
                     * Compare MSB and in case of success set result to message index.
                     */
                    if (frameData[0u] == LO8(messageIdTable[i]))
                    {
                        if (frameData[1u] == HI8(messageIdTable[i]))
                        {
                            result = i;
                        }
                    }
                    i++;
                }
                return (result);
            }
        #endif /* (1u == LINS_LIN_2_0) */
    #endif /*(0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB1_SEL)) */

    #if ((0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB2_SEL)) || \
         (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB3_SEL)))
        /*******************************************************************************
        * Function Name: ld_read_by_id_callout
        ********************************************************************************
        *
        * Summary:
        *  This function calls out when the master node transmits a "read" by identifier
        *  request with the identifier in the user defined area. The slave node
        *  application is called from the driver when such a request is received.
        *
        * Parameters:
        *  iii - Interface.
        *  id - The id parameter is the identifier in the user defined area (32 to 63),
        *  from the "read" by identifier configuration request.
        *  frameData - The data pointer points to a data area with 5 bytes. This area
        *  is used by the application to set up a positive response.
        *
        * Return:
        *  LD_NEGATIVE_RESPONSE - The slave node responds with a negative response.
        *  In this case the data area is not considered.
        *
        *  LD_POSTIVE_RESPONSE - The slave node sets up a positive response using
        *  the data provided by the application.
        *
        *  LD_NO_RESPONSE - The slave node does not answer.
        *
        *******************************************************************************/
        l_u8 ld_read_by_id_callout_LINS(l_u8 id, l_u8* frameData)
        {
            /* NOTE: frameData will be modified by callback function */
            l_u8 result = LD_NEGATIVE_RESPONSE;

            #ifdef LINS_LD_READ_BY_ID_CALLOUT_CALLBACK
                result = ld_read_by_id_callout_LINS_Callback(id, frameData);
            #else
                (void) id;
                (void) frameData;
            #endif /* LINS_LD_READ_BY_ID_CALLOUT_CALLBACK */

            return (result);
        }
    #endif /* ((0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB2_SEL)) ||
            *  (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB3_SEL)))
            */
#endif /* LINS_CFG_SERVICES_ENABLED */


/******************************************************************************
* Function Name: CheckNodeState
*******************************************************************************
*
* Summary:
*  This function implements LIN Slave Node state machine as defined in LIN2.2.a,
*  item 5.5 SLAVE NODE TRANSMISSION HANDLER.
*  Its states are:
*       IDLE - initial state;
*       RX_PHY_REQUEST  - processing of incoming segmented message;
*       TX_PHY_RESPONSE - processing of outcoming RESPONSE.
*
* Parameters:
*  stimulus - The event passed from LIN Protocol Layer:
*     MRF_ALIEN_NAD  - Frame with NAD not equal to this node NAD arrives.
*     MRF_OWN_NAD    - Frame with NAD equal to this node NAD arrives.
*     MRF_FUNC_NAD   - Frame with functional NAD equal arrives.
*     SRF            - SRF header arrives.
*     RX_TIMEOUT     - N_CR timeout was expired.
*     TX_TIMEOUT     - N_AS timeout was expired.
*
******************************************************************************/
static void LINS_CheckNodeState(l_u8 stimulus)
{
    l_u8 return_value;

    switch (LINS_nodeState)
    {
        case LINS_IDLE:
            switch (stimulus)
            {
                case LINS_MRF_ALIEN_NAD:
                case LINS_SRF:
                case LINS_RX_TIMEOUT:
                case LINS_TX_TIMEOUT:
                    LINS_tlFlags &= (l_u8) (~(LINS_TL_N_CR_TIMEOUT_ON |
                                                LINS_TL_N_AS_TIMEOUT_ON ));
                break;
                case LINS_MRF_FUNC_NAD:
                    (void)LINS_ProcessMrf(LINS_mrfBuffer);
                    LINS_ClrTxBuffer(stimulus);
                break;
                case LINS_MRF_OWN_NAD:
                    /* Receives an incoming TL frame (FF or SF) */
                    LINS_ClrTxBuffer(stimulus);

                    return_value = LINS_ProcessMrf(LINS_mrfBuffer);

                    if (LINS_RESPONSE_REQUIRED == return_value)
                    {
                        /* Changes the node state to TX state */
                        LINS_nodeState = LINS_TX_PHY_RESPONSE;
                    }
                    else if (LINS_RECEIVE_CONTINUES == return_value)
                    {
                        /* Changes the node state to RX state */
                        LINS_nodeState = LINS_RX_PHY_REQUEST;
                    }
                    else
                    {
                        /* Does nothing. */
                    }
                break;
                default:
                    /* Does nothing. */
                break;
            }
        break;
        case LINS_RX_PHY_REQUEST:
            switch(stimulus)
            {
                case LINS_MRF_ALIEN_NAD:
                case LINS_SRF:
                case LINS_RX_TIMEOUT:
                case LINS_TX_TIMEOUT:
                    LINS_ClrTxBuffer(stimulus);
                    LINS_ClrRxBuffer(stimulus);
                    LINS_nodeState = LINS_IDLE;
                break;
                case LINS_MRF_FUNC_NAD:
                    /* Misses the functional request */
                break;
                case LINS_MRF_OWN_NAD:
                    /* Receives the incoming TL frame (CF) */
                    return_value = LINS_ProcessMrf(LINS_mrfBuffer);

                    if (LINS_RESPONSE_REQUIRED == return_value)
                    {
                        /* Changes the node state to TX state */
                        LINS_nodeState = LINS_TX_PHY_RESPONSE;
                    }
                    else if (LINS_RECEIVE_CONTINUES == return_value)
                    {
                        /* Changes the node state to RX state */
                        LINS_nodeState = LINS_RX_PHY_REQUEST;
                    }
                    else    /* Does nothing. */
                    {
                        LINS_ClrTxBuffer(stimulus);
                        LINS_ClrRxBuffer(stimulus);
                        LINS_nodeState = LINS_IDLE;
                    }
                break;
                default:
                    /* Does nothing. */
                break;
            }
        break;
        case LINS_TX_PHY_RESPONSE:
            switch(stimulus)
            {
                case LINS_MRF_ALIEN_NAD:
                case LINS_RX_TIMEOUT:
                case LINS_TX_TIMEOUT:
                    LINS_ClrTxBuffer(stimulus);
                    LINS_nodeState = LINS_IDLE;
                break;
            case LINS_MRF_FUNC_NAD:
                /* Does nothing, misses the functional request here. */
            break;
            case LINS_SRF:
                /* Transmit RESPONSE message */
                return_value = LINS_TransmitTLFrame(stimulus);
                if (LINS_TRANSMISSION_CONTINUES != return_value)
                {
                    LINS_ClrTxBuffer(stimulus);
                    if (0u == (LINS_status & LINS_STATUS_RESPONSE_PENDING))
                    {
                        /* Changes the node state to idle state. */
                        LINS_nodeState = LINS_IDLE;
                    }
                }
            break;
            case LINS_MRF_OWN_NAD:
                /* MRF arrives during the RESPONSE transmission , drop
                transmitted message and receive new diagnostic message */

                LINS_ClrTxBuffer(stimulus);

                return_value = LINS_ProcessMrf(LINS_mrfBuffer);

                if (LINS_RESPONSE_REQUIRED == return_value)
                {
                    /* Changes the node state to TX state. */
                    LINS_nodeState = LINS_TX_PHY_RESPONSE;
                }
                else if (LINS_RECEIVE_CONTINUES == return_value)
                {
                    /* Changes the node state to RX state. */
                    LINS_nodeState = LINS_RX_PHY_REQUEST;
                }
                else
                {
                    LINS_ClrRxBuffer(stimulus);
                    LINS_ClrTxBuffer(stimulus);
                    LINS_nodeState = LINS_IDLE;
                }
            break;
            default:
                break;
            }
        break;
        default:
            break;
    }
}


/******************************************************************************
* Function Name: LINS_TransmitTLFrame
*******************************************************************************
*
* Summary:
*  Transmits frame of segmented message.
*
* Parameters:
*  stimulus - The event passed from LIN Protocol Layer:
*     MRF_ALIEN_NAD  - Frame with NAD not equal to this node NAD arrives.
*     MRF_OWN_NAD    - Frame with NAD equal to this node NAD arrives.
*     MRF_FUNC_NAD   - Frame with functional NAD equal arrives.
*     SRF            - SRF header arrives.
*     RX_TIMEOUT     - N_CR timeout expired.
*     TX_TIMEOUT     - N_AS timeout expired.
* Return:
*  NO_RESPONSE_REQUIRED      - The message transmission completed.
*  TRANSMISSION_CONTINUES    - There are more segments to be transmitted.
*
******************************************************************************/
static l_u8 LINS_TransmitTLFrame(l_u8 stimulus)
{
    l_u8 return_value = LINS_NO_RESPONSE_REQUIRED;

    (void) stimulus;

    /* Clears CR Timeout after answering SRF. */
    LINS_tlFlags &= ((l_u8) ~LINS_TL_N_CR_TIMEOUT_ON);

    if (0u == (LINS_status & LINS_STATUS_SRVC_RSP_RDY))
    {
        if ((0u != LINS_txMessageLength) &&
            (0u != (LINS_tlFlags & LINS_TL_TX_REQUESTED)))
        {
            return_value = LINS_TRANSMISSION_CONTINUES;
        }
    }

    return(return_value);
}


/******************************************************************************
* Function Name: LINS_ClrRxBuffer
*******************************************************************************
*
* Summary:
*  This function initializes the receive part of Transport Layer variables .
*
* Parameters:
*  stimulus - The event passed from LIN Protocol Layer:
*     MRF_ALIEN_NAD  - Frame with NAD not equal to this node NAD arrives.
*     MRF_OWN_NAD    - Frame with NAD equal to this node NAD arrives.
*     MRF_FUNC_NAD   - Frame with functional NAD equal arrives.
*     SRF            - SRF header arrives.
*     RX_TIMEOUT     - N_Cr timeout expired.
*     TX_TIMEOUT     - N_As timeout expired.
*
******************************************************************************/
static void LINS_ClrRxBuffer(l_u8 stimulus)
{
    /* Previous PCI requires to be unknown after the initialization */
    LINS_rxPrevPci = LINS_PDU_PCI_TYPE_UNKNOWN;
    /* Resets the frame counters. */
    LINS_rxMessageLength = 0u;
    LINS_rxFrameCounter = 0u;

#if (LINS_COOKED_TL_API)
    if (LINS_tlLengthPointer != NULL)
    {
        /* Clears the length of the erroneous frame. */
        *LINS_tlLengthPointer = 0u;
    }
    
    if (stimulus == LINS_RX_TIMEOUT)
    {
        /* Sets the error status as a timeout occurrs. */
        LINS_rxTlStatus = LD_N_CR_TIMEOUT;
    }
    else if (stimulus == LINS_MRF_ALIEN_NAD)
    {
        LINS_rxTlStatus = LD_FAILED;
    }  
    else
    {
        if ((LINS_rxTlStatus != LD_FAILED)   &&
            (LINS_rxTlStatus != LD_WRONG_SN) &&
            (LINS_rxTlStatus != LD_N_CR_TIMEOUT))
        {
            /* Initializes the RX status variable properly. */
            LINS_rxTlStatus = LD_COMPLETED;
        }
    }

    LINS_rxTlDataPointer = NULL;
    LINS_rxTlInitDataPointer = NULL;
#else
    (void) stimulus;

    /* Resets the buffers depth to 0 to indicate the buffers are empty. */
    LINS_rxBufDepth = 0u;
    /* Raw API buffers initialization */
    LINS_rxWrIndex = 0u;
    LINS_rxRdIndex = 0u;
    LINS_rxTlStatus = LD_NO_DATA;
#endif /* LINS_COOKED_TL_API */

    /* Clears the Service Response ready status bit */
    LINS_status &= ((l_u8) ~LINS_STATUS_SRVC_RSP_RDY);

    LINS_tlFlags &= (l_u8) (~(LINS_TL_RX_REQUESTED | LINS_TL_N_CR_TIMEOUT_ON ));

    LINS_tlTimeoutCnt = 0u;
}


/******************************************************************************
* Function Name: LINS_ClrTxBuffer
*******************************************************************************
*
* Summary:
*  This function initializes transmit part of Transport Layer variables.
*
* Parameters:
*  stimulus - The event passed from LIN Protocol Layer:
*     MRF_ALIEN_NAD  - Frame with NAD not equal to this node NAD arrives.
*     MRF_OWN_NAD    - Frame with NAD equal to this node NAD arrives.
*     MRF_FUNC_NAD   - Frame with functional NAD equal arrives.
*     SRF            - SRF header arrives.
*     RX_TIMEOUT     - N_Cr timeout expired.
*     TX_TIMEOUT     - N_As timeout expired.
*
*******************************************************************************/
static void LINS_ClrTxBuffer(l_u8 stimulus)
{
    /* Previous PCI requires to be unknown after the initialization */
    LINS_txPrevPci = LINS_PDU_PCI_TYPE_UNKNOWN;

#if (LINS_COOKED_TL_API)
    /* Resets the frame counters */
    LINS_txMessageLength = 0u;
    /* Resets the frame counters */
    LINS_txFrameCounter = 0u;

    if (stimulus == LINS_TX_TIMEOUT)
    {
        /* Set error status as the timeout occurred */
        LINS_txTlStatus = LD_N_AS_TIMEOUT;
    }
    else if (stimulus == LINS_MRF_ALIEN_NAD)
    {
        LINS_txTlStatus = LD_FAILED;
    }
    else
    {
        if ((LINS_txTlStatus != LD_FAILED) && (LINS_txTlStatus != LD_N_AS_TIMEOUT))
        {
            /* Initializes the TX status variable properly. */
            LINS_txTlStatus = LD_COMPLETED;
        }
    }
    LINS_tlFlags &= (l_u8)(~(LINS_TL_TX_REQUESTED | LINS_TL_N_AS_TIMEOUT_ON | 
        LINS_TL_N_CR_TIMEOUT_ON));
#else
    (void) stimulus;
    
    if (0u == (LINS_status & LINS_STATUS_RESPONSE_PENDING))
    {   /* Regular frame */
        /* Resets the frame counters */
        LINS_txMessageLength = 0u;
        /* Resets the frame counters */
        LINS_txFrameCounter = 0u;
        /* Resets the buffers depth to 0 to indicate the buffers are empty. */
        LINS_txBufDepth = 0u;
        
        /* Raw API buffers initialization */
        LINS_txWrIndex = 0u;
        LINS_txRdIndex = 0u;
        LINS_txTlStatus = LD_QUEUE_EMPTY;
        LINS_tlFlags &= (l_u8)(~(LINS_TL_TX_REQUESTED | LINS_TL_N_AS_TIMEOUT_ON | 
        LINS_TL_N_CR_TIMEOUT_ON));
    }
    else /* Response pending frame */
    {
        /* Disables N_AS and N_CR timers, LIN Master controls P2* timeout for response pending frame. */
        LINS_tlFlags &= (l_u8)(~(LINS_TL_N_AS_TIMEOUT_ON | LINS_TL_N_CR_TIMEOUT_ON));
    }
#endif /* LINS_COOKED_TL_API */

    /* Clears Service Response Ready status bit. */
    LINS_status &= ((l_u8) ~LINS_STATUS_SRVC_RSP_RDY);
    LINS_tlTimeoutCnt = 0u;
}


/******************************************************************************
* Function Name: LINS_ProcessMrf
*******************************************************************************
*
* Summary:
*  This API is called from ISR. It is responsible to parse the RX frames
*  that come from the LIN Master. This API handles Automatic Configuration
*  requests and receives both Raw and Cooked API frames.
*
* Parameters:
*  frame - The pointer to an array that contains a last received frame.
*
* Return:
*  NO_RESPONSE_REQUIRED      - No response to a received message is expected.
*  RESPONSE_REQUIRED         - A response to a received message is expected.
*
******************************************************************************/
static l_u8 LINS_ProcessMrf(l_u8 frame[])
{
    l_u8 i;
    l_u8 return_value = LINS_NO_RESPONSE_REQUIRED;
    l_u8 tempStatus = 0u;
    l_u8 tmpPci;
    l_u8 tmpByte;
#if (LINS_COOKED_TL_API)
    l_u16 tmpWord;
#endif /* LINS_COOKED_TL_API */

#if (LINS_CFG_SERVICES_ENABLED)
    l_u8 tempStatusErr = LINS_NO_RESPONSE_REQUIRED;
    #if (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB3_SEL))
        l_u8 idByte;
        l_u8 idMask;
        l_u8 idInvert;
    #endif /* (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB3_SEL)) */
    /* To avoid 'unused variable' warning when relevant services are disabled */
    (void) tempStatusErr;
#endif /* LINS_CFG_SERVICES_ENABLED */
    (void) tempStatus;

    if ((LINS_NAD_GO_TO_SLEEP == frame[LINS_PDU_NAD_IDX]) 
        #if (LINS_SAE_J2602_ENABLED) /* Enable MRF with ID=0x3E for J2602 */
            /* For J2602 0x00 is a valid NAD for 0x3E requests, don't go to sleep */
            && (0u ==(LINS_auxStatus & LINS_AUXSTS_MRF_WAS_3E))
        #endif /* LINS_SAE_J2602_ENABLED */
       )
    {
        LINS_ifcStatus |= LINS_IFC_STS_GO_TO_SLEEP;
    }
    else if ((LINS_NAD_BROADCAST  == frame[LINS_PDU_NAD_IDX]) ||
             (LINS_NAD_FUNCTIONAL == frame[LINS_PDU_NAD_IDX]) ||
             (LINS_nad            == frame[LINS_PDU_NAD_IDX]) ||
         #if (LINS_SAE_J2602_ENABLED) /* Enable MRF with ID=0x3E for J2602 */
             (((LINS_nad & 0x0Fu) == (frame[LINS_PDU_NAD_IDX] & 0x0Fu)) &&
             ((frame[LINS_PDU_NAD_IDX] > LINS_NAD_BROADCAST) || 
             (0u !=(LINS_auxStatus & LINS_AUXSTS_MRF_WAS_3E)))) ||
         #endif /* LINS_SAE_J2602_ENABLED */
             ((LINS_LinSlaveConfig.initialNad == frame[LINS_PDU_NAD_IDX]) &&
             (LINS_NCS_ASSIGN_NAD            == frame[LINS_PDU_SID_IDX])))
    {
            #if (LINS_SAE_J2602_ENABLED)
            /* Inserts J2602 status byte as byte 0 for frames with ID=0x3E or NAD>=0x80 */
                if ((0u != (LINS_auxStatus & LINS_AUXSTS_MRF_WAS_3E)) ||
                    (LINS_mrfBuffer[LINS_PDU_NAD_IDX] > LINS_NAD_BROADCAST))
                    {
                /* Indicates that J2602 will be placed to the outgoing frame and
                   can be cleared if transmitted successfuly. 
                   Used for MRF with ID=0x3E or NAD>=0x80. */
                    LINS_auxStatus |= LINS_AUXSTS_MRF_REQUIRES_J2602STATUS;                
                }
                else
                {
                    LINS_auxStatus &= (l_u8)(~LINS_AUXSTS_MRF_REQUIRES_J2602STATUS);    
                }                
            #endif /* LINS_SAE_J2602_ENABLED */
        /* Single Frame is detected */
        if (0u == (frame[LINS_PDU_PCI_IDX] & LINS_PDU_PCI_TYPE_MASK))
        {
          #if (LINS_SAE_J2602_ENABLED)  /* Check MRF for ID=0x3E or NAD>0x7F for J2602 */ 
              if ((0u != (LINS_auxStatus & LINS_AUXSTS_MRF_WAS_3E)) ||
                 (frame[LINS_PDU_NAD_IDX] > LINS_NAD_BROADCAST))
                {
                    /* This means the user should process this Frame properly using TL API and  indicates    *
                    * that the detected SID is a diagnostic SID and it should be passed to Transport Layer. */
                    LINS_tlFlags |= LINS_TL_CS_SERVICE_DISABLED |
                                                LINS_TL_DIAG_FRAME_DETECTED;
                }
                else
            {
          #endif /* LINS_SAE_J2602_ENABLED */
            
            /* SID used for node configuration */
            switch (frame[LINS_PDU_SID_IDX])
            {
            #if (LINS_CFG_SERVICES_ENABLED)
                #if (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB0_SEL))

                case LINS_NCS_ASSIGN_NAD:
                    /* Checks the data length. Do not respond if the PCI length does not match this service's data length. */
                    if (LINS_PDU_SF_DATA_LEN != (frame[LINS_PDU_PCI_IDX] & LINS_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                    /* Checks LIN Product ID and if MRF has NAD equal to the initial NAD. */
                    if ((1u == LINS_LinProductId(&frame[3u])) &&
                        (LINS_LinSlaveConfig.initialNad == frame[LINS_PDU_NAD_IDX]))
                    {
                        #if (LINS_SAE_J2602_ENABLED)
                        /* Only change from uninitialized NAD==6F is allowed. */
                        if (LINS_NAD_UNINITIALIZED != LINS_LinSlaveConfig.initialNad)
                        {
                            break; /* NAD change is not allowed. Returns with no response. */
                        }
                        /* Tries updating messages IDs according to the new NAD.
                           Checks if the new NAD is valid in scope of the frame count. */
                        tempStatus = LINS_UpdateMessageIDs(frame[LINS_PDU_D5_NEW_NAD_IDX]);
                        if (tempStatus == CYRET_BAD_PARAM)
                        {
                            break; /* NAD is invalid for frame count. Returns with no response */
                        }
                        #endif /* LINS_SAE_J2602_ENABLED */
                        
                        /* Saves the received NAD */
                        LINS_nad = frame[LINS_PDU_D5_NEW_NAD_IDX];
                        /* Fills SRF Buffer with a response to the service. NAD field should contain the initial NAD. */
                        LINS_srfBuffer[LINS_PDU_NAD_IDX] =
                            LINS_LinSlaveConfig.initialNad;
                            
                        /* PCI is 0 so only length is required. */
                        LINS_srfBuffer[LINS_PDU_PCI_IDX] = 1u;
                        
                        /* RSID for a positive response is always SID + 0x40. */
                        LINS_srfBuffer[LINS_PDU_SID_IDX] =
                            LINS_NCS_POS_RESP_ASSIGN_NAD;
                            
                        /* Fills unused data bytes with 0xFFs. */
                        for (i = 3u; i < LINS_FRAME_LEN; i++)
                        {
                            LINS_srfBuffer[i] = 0xFFu;
                        }
                        /* Sets a service response bit that indicates that a response is ready to be sent to Master node. */
                        LINS_status |= LINS_STATUS_SRVC_RSP_RDY;
                    }
                    else
                    {
                        /* No response because Supplier ID and Function ID are invalid or MRF NAD is not equal to the initial. */
                    }
                break;

                #endif /* (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB0_SEL) */

                #if (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB1_SEL))
                    /* LIN 2.0 specification only. Obsolete, and not implemented for LIN lin 2.1. */
                    case LINS_NCS_ASSIGN_FRAME_ID:
                    /* Checks the data length. Does not respond if the PCI length does not match this service data length */
                    if (LINS_PDU_SF_DATA_LEN != (frame[LINS_PDU_PCI_IDX] & LINS_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                    #if (1u == LINS_LIN_2_0)
                        /* tempStatus is used in Supplier ID verification */
                        tempStatus = 1u;
                        
                        if ((frame[LINS_PDU_D1_IDX] != LO8(LINS_slaveId.supplierId)) &&
                            (frame[LINS_PDU_D1_IDX] != LO8(LINS_CS_SUPPLIER_ID_WILDCARD)))
                        {
                            tempStatus = 0u;  /* Zeroes out tempStatus if data isn't for this slave */
                        }
                        if ((frame[LINS_PDU_D2_IDX] != HI8(LINS_slaveId.supplierId)) &&
                            (frame[LINS_PDU_D2_IDX] != HI8(LINS_CS_SUPPLIER_ID_WILDCARD)))
                        {
                            tempStatus = 0u;   /* Zeroes out tempStatus if data isn't for this slave */
                        }
                        /* If tempStatus is not 0 then verification passed successfully */
                        if (tempStatus != 0u)
                        {
                            /* Now tempStatus is used to hold Message ID Index */
                            tempStatus = LINS_MessageId(frame + 5u);
                            
                            /* If Message ID index is valid, processes the request and prepares a positive response. */
                            if (tempStatus != LD_INVALID_MESSAGE_INDEX)
                            {
                                /* Set PID in the position that valid index points to */
                                LINS_volatileConfig[tempStatus] = frame[LINS_PDU_D5_IDX];
                                
                                /* Fills SRF Buffer with a response to the service. Nad field should contain the current NAD. */
                                LINS_srfBuffer[LINS_PDU_NAD_IDX] = LINS_nad;
                                /* PCI is 0, so only the length is required. */
                                LINS_srfBuffer[LINS_PDU_PCI_IDX] = 1u;
                                /* RSID for a positive response is always SID + 0x40 */
                                LINS_srfBuffer[LINS_PDU_SID_IDX] =
                                    LINS_NCS_POS_RESP_ASSIGN_FRAME_ID;
                                    
                                /* Fills unused data bytes with 0xFFs */
                                for (i = 3u; i < LINS_FRAME_LEN; i++)
                                {
                                    LINS_srfBuffer[i] = 0xFFu;
                                }
                                /* Sets service response bit that indicates that response is
                                 * ready to be sent to Master node.
                                 */
                                LINS_status |= LINS_STATUS_SRVC_RSP_RDY;
                            }
                        }
                    #else
                        /* Does nothing, ignores an obsolete request. */
                    #endif /* (1u == LINS_LIN_2_0) */
                    break;
                #endif /* (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB1_SEL) */

                #if (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB2_SEL))
                    case LINS_NCS_READ_BY_ID:
                    /* Checks the data length. Does not respond if the PCI length does not match this service data length. */
                    if (LINS_PDU_SF_DATA_LEN != (frame[LINS_PDU_PCI_IDX] & LINS_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                        /* tempStatus is used to hold status of LINS_LinProductId() */
                        tempStatus = LINS_LinProductId(&frame[4u]);
                        
                        /* LIN Product Identification (only identifier is supported) */
                        if (LINS_NCS_READ_BY_ID_ID == frame[LINS_PDU_D1_IDX])
                        {
                            if (LINS_TRUE == tempStatus)
                            {
                                /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                                LINS_srfBuffer[LINS_PDU_NAD_IDX] = LINS_nad;
                                /* PCI is 0, so only the length is required. */
                                LINS_srfBuffer[LINS_PDU_PCI_IDX] = 6u;
                                /* RSID for a positive response is always SID + 0x40 */
                                LINS_srfBuffer[LINS_PDU_SID_IDX] =
                                    LINS_NCS_POS_RESP_READ_BY_ID;
                                    
                                /* Fills data fields with Supplier and function IDs */
                                LINS_srfBuffer[LINS_PDU_D1_IDX] =
                                    LO8(LINS_slaveId.supplierId);
                                LINS_srfBuffer[LINS_PDU_D2_IDX] =
                                    HI8(LINS_slaveId.supplierId);
                                LINS_srfBuffer[LINS_PDU_D3_IDX] =
                                    LO8(LINS_slaveId.functionId);
                                LINS_srfBuffer[LINS_PDU_D4_IDX] =
                                    HI8(LINS_slaveId.functionId);
                                LINS_srfBuffer[LINS_PDU_D5_IDX] =
                                    LINS_slaveId.variant;
                                    
                                /* Set a service response bit that indicates that response is
                                 * ready to be sent to Master node.
                                 */
                                LINS_status |= LINS_STATUS_SRVC_RSP_RDY;
                            }
                            else
                            {
                                /* No action */
                            }
                        }
                        else if (LINS_NCS_READ_BY_ID_SERIAL == frame[LINS_PDU_D1_IDX])
                        {
                            /* Serial number identification */
                            /* If Slave IDs match the received ID and SN is set, then prepares a positive response. */
                            if ((LINS_TRUE == tempStatus) && (NULL != LINS_serialNumber))
                            {
                                /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                                LINS_srfBuffer[LINS_PDU_NAD_IDX] = LINS_nad;
                                /* PCI is 0, so only the length is required. */
                                LINS_srfBuffer[LINS_PDU_PCI_IDX] = 5u;
                                /* RSID for a positive response is always SID + 0x40 */
                                LINS_srfBuffer[LINS_PDU_SID_IDX] =
                                    LINS_NCS_POS_RESP_READ_BY_ID;
                                /* Fills unused data bytes with the serial number ID. */
                                for (i = 3u; i < (LINS_FRAME_LEN - 1u); i++)
                                {
                                    LINS_srfBuffer[i] = LINS_serialNumber[i - 3u];
                                }
                                /* The serial number is 4-byte length, sets the last unused byte to 0xFF. */
                                LINS_srfBuffer[LINS_PDU_D5_IDX] = 0xFFu;
                                /* Sets the service response bit that indicates that the response is
                                * ready to be sent to master node.
                                */
                                LINS_status |= LINS_STATUS_SRVC_RSP_RDY;
                            }
                            else
                            {
                                 /* Returns a negative response, the serial number is invalid. */
                                 tempStatusErr = LINS_NCS_NEGATIVE_RESPONSE_REQUIRED;
                            }
                        }
                        #ifdef LINS_FILE_REVISION_DEFINED
                        else if (LINS_NCS_READ_BY_ID_FILE_REVISION == frame[LINS_PDU_D1_IDX])
                        {
                            /* Version of LDF/NCF identification */
                            /* If Slave IDs match the received ID and FILE_REVISION is set, then prepares a positive response. */
                            if ((LINS_TRUE == tempStatus) && (LINS_FILE_REVISION_DEFINED))
                            {
                                /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                                LINS_srfBuffer[LINS_PDU_NAD_IDX] = LINS_nad;
                                /* PCI is 0, so only the length is required. */
                                LINS_srfBuffer[LINS_PDU_PCI_IDX] = 5u;
                                /* RSID for a positive response is always SID + 0x40 */
                                LINS_srfBuffer[LINS_PDU_SID_IDX] =
                                    LINS_NCS_POS_RESP_READ_BY_ID;
                                /* Fills data bytes with revision numbers. */
                                LINS_srfBuffer[LINS_PDU_D1_IDX] = LINS_FILE_REVISION_major;
                                LINS_srfBuffer[LINS_PDU_D2_IDX] = LINS_FILE_REVISION_minor;
                                LINS_srfBuffer[LINS_PDU_D3_IDX] = LINS_FILE_REVISION_sub;
                                LINS_srfBuffer[LINS_PDU_D4_IDX] = LINS_FILE_REVISION_source;
                                /* Reserved for future use and transmitted with value 0x00. */
                                LINS_srfBuffer[LINS_PDU_D5_IDX] = 0x00u;
                                /* Sets a service response bit that indicates that the response is
                                * ready to be sent to master node.
                                */
                                LINS_status |= LINS_STATUS_SRVC_RSP_RDY;
                            }
                            else
                            {
                                 /* Returns a negative response, Slave IDs are invalid. */
                                 tempStatusErr = LINS_NCS_NEGATIVE_RESPONSE_REQUIRED;
                            }
                        }
                        #endif /* FILE_REVISION  defined*/
                        
                        else if ((frame[LINS_PDU_D1_IDX] >= 32u) &&
                                 (frame[LINS_PDU_D1_IDX] <= 63u))   /* User defined identification */
                        {
                            if (LINS_TRUE == tempStatus)
                            {
                                /* If user doesn't reassign the status of ld_read_by_id_callout(),
                                 * LD_NEGATIVE_RESPONSE is always returned by ld_read_by_id_callout().
                                 * This indicates to the master that the service by the user defined
                                 * identification is not supported. tempStatus is used to hold the status of
                                 * ld_read_by_id_callout().
                                 */
                                tempStatus = ld_read_by_id_callout_LINS(frame[LINS_PDU_D1_IDX],
                                    frame + LINS_FRAME_DATA_SIZE_3);
                                    
                                if (tempStatus == LD_POSITIVE_RESPONSE)
                                {
                                    /* Fills SRF Buffer with a response to the service. Nad field should contain the current NAD. */
                                    LINS_srfBuffer[LINS_PDU_NAD_IDX] = LINS_nad;
                                    /* PCI is 0, so only the length is required. */
                                    LINS_srfBuffer[LINS_PDU_PCI_IDX] = 6u;
                                    /* RSID for a positive response is always SID + 0x40 */
                                    LINS_srfBuffer[LINS_PDU_SID_IDX] =
                                        LINS_NCS_POS_RESP_READ_BY_ID;
                                        
                                    /* Fills unused data bytes with user defined information */
                                    for (i = 3u; i < LINS_FRAME_LEN; i++)
                                    {
                                        LINS_srfBuffer[i] = frame[i];
                                    }
                                    /* Sets the service response bit that indicates that the response is
                                    * ready to be sent to master node.
                                    */
                                    LINS_status |= LINS_STATUS_SRVC_RSP_RDY;
                                }
                                else
                                {
                                    /* Return Negative response */
                                     tempStatusErr = LINS_NCS_NEGATIVE_RESPONSE_REQUIRED;
                                }
                            }
                            else
                            {
                                /* Return Negative response, serial number is invalid */
                                tempStatusErr = LINS_NCS_NEGATIVE_RESPONSE_REQUIRED;
                            }
                        }
                        /* Message ID identification */
                        else if ((frame[LINS_PDU_D1_IDX] >= LINS_ID_16) &&
                                 (frame[LINS_PDU_D1_IDX] <= LINS_ID_31))
                        {
                            /* LIN 2.0 specification Only */
                        #if (1u == LINS_LIN_2_0)
                            /* If slave serial number matches received one, prepare positive response. */
                            if (LINS_TRUE == tempStatus)
                            {
                                /* Fill SRF Buffer with response to service. NAD field should contain current NAD */
                                LINS_srfBuffer[LINS_PDU_NAD_IDX] = LINS_nad;
                                
                                /* tempStatus is now used to store calculated Message ID index */
                                tempStatus = frame[LINS_PDU_D1_IDX] - LD_MESSAGE_ID_BASE;
                                
                                if (LINS_NUM_FRAMES > tempStatus)
                                {
                                    /* PCI is 0 so only length required */
                                    LINS_srfBuffer[LINS_PDU_PCI_IDX] = 4u;
                                    
                                    /* RSID for positive response is always SID + 0x40 */
                                    LINS_srfBuffer[LINS_PDU_SID_IDX] =
                                        LINS_NCS_POS_RESP_READ_BY_ID;
                                        
                                    /* D1 = Message ID LSB */
                                    LINS_srfBuffer[LINS_PDU_D1_IDX] =
                                        HI8(messageIdTable[tempStatus]);
                                        
                                    /* D2 = Message ID MSB */
                                    LINS_srfBuffer[LINS_PDU_D2_IDX] =
                                        LO8(messageIdTable[tempStatus]);
                                        
                                    /* D3 = PID */
                                    LINS_srfBuffer[LINS_PDU_D3_IDX] =
                                        LINS_volatileConfig[tempStatus];
                                        
                                    /* Message ID response is 3 byte length, so set last two bytes to 0xFF. */
                                    LINS_srfBuffer[LINS_PDU_D4_IDX] = 0xFFu;
                                    LINS_srfBuffer[LINS_PDU_D5_IDX] = 0xFFu;
                                    
                                    /* Set service response bit that indicates that response is
                                     * ready to be sent to master node.
                                     */
                                    LINS_status |= LINS_STATUS_SRVC_RSP_RDY;
                                }
                                else
                                {
                                     tempStatusErr = LINS_NCS_NEGATIVE_RESPONSE_REQUIRED;
                                }
                            }
                            else
                            {
                                 tempStatusErr = LINS_NCS_NEGATIVE_RESPONSE_REQUIRED;
                            }
                        #else
                            /* Return Negative response on erroneous request */
                            tempStatusErr = LINS_NCS_NEGATIVE_RESPONSE_REQUIRED;
                        #endif /* (1u == LINS_LIN_2_0) */
                        }
                        else
                        {
                            /* Return Negative response */
                            tempStatusErr = LINS_NCS_NEGATIVE_RESPONSE_REQUIRED;
                        }

                        if (LINS_NCS_NEGATIVE_RESPONSE_REQUIRED == tempStatusErr)
                        {
                            /* Fill SRF Buffer with negative response to service. Nad field should contain current NAD */
                            LINS_srfBuffer[LINS_PDU_NAD_IDX] = LINS_nad;
                            /* PCI is 0 so only length required */
                            LINS_srfBuffer[LINS_PDU_PCI_IDX] = 3u;
                            /* RSID for negative response is always 0x7F */
                            LINS_srfBuffer[LINS_PDU_SID_IDX] =
                                LINS_NCS_RSID_NEG_REPLY;
                            /* D1 holds the service ID */
                            LINS_srfBuffer[LINS_PDU_D1_ID_IDX] =
                                LINS_NCS_READ_BY_ID;
                            /* D2 contains error code */
                            LINS_srfBuffer[LINS_PDU_D2_IDX] = 0x12u;
                            
                            /* Fill unused data bytes with 0xFFs */
                            for (i = 5u; i < LINS_FRAME_LEN; i++)
                            {
                                LINS_srfBuffer[i] = 0xFFu;
                            }
                            /* Set service response bit that indicates that response is
                             * ready to be sent to master node.
                             */
                            LINS_status |= LINS_STATUS_SRVC_RSP_RDY;
                        }
                    break;
                #endif /* (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB2_SEL) */

                #if (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB3_SEL))
                    case LINS_NCS_COND_CHANGE_NAD:
                    /* Checks the data length. Does not respond if the PCI length does not match this service data length. */
                    if (LINS_PDU_SF_DATA_LEN != (frame[LINS_PDU_PCI_IDX] & LINS_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                        if (LINS_NCS_READ_BY_ID_ID == frame[LINS_PDU_D1_ID_IDX])
                        {
                            /* LIN Product Identification */
                            if ((6u > frame[LINS_PDU_D2_BYTE_IDX]) &&
                                (0u != frame[LINS_PDU_D2_BYTE_IDX]))
                            {
                                switch (frame[LINS_PDU_D2_BYTE_IDX])
                                {
                                    /* tempStatus is used as a temporary variable to store ID byte */
                                    case LINS_CS_BYTE_SUPPLIER_ID1:
                                        tempStatus = LO8(LINS_slaveId.supplierId);
                                    break;
                                    case LINS_CS_BYTE_SUPPLIER_ID2:
                                        tempStatus = HI8(LINS_slaveId.supplierId);
                                    break;
                                    case LINS_CS_BYTE_FUNCTION_ID1:
                                        tempStatus = LO8(LINS_slaveId.functionId);
                                    break;
                                    case LINS_CS_BYTE_FUNCTION_ID2:
                                        tempStatus = HI8(LINS_slaveId.functionId);
                                    break;
                                    case LINS_CS_BYTE_VARIANT:
                                        tempStatus = LINS_slaveId.variant;
                                    break;
                                    default:
                                        /* Never use this state. */
                                    break;
                                }

                                if (0u == ((tempStatus ^ frame[LINS_PDU_D4_INVERT_IDX]) &
                                    frame[LINS_PDU_D3_MASK_IDX]))
                                {
                                    /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                                    LINS_srfBuffer[LINS_PDU_NAD_IDX] = LINS_nad;
                                    
                                    /* Changes NAD to a new NAD. */
                                #if (LINS_SAE_J2602_ENABLED) 
                                    /* Try setting new NAD. Check if NAD is valid by trying to update message IDs */
                                    if ((l_u16) CYRET_BAD_PARAM == l_ifc_ioctl_LINS(L_IOCTL_SET_NAD, frame+LINS_PDU_D5_NEW_NAD_IDX))
                                    {
                                        break; /* The new NAD is invalid for the current frame count. */ 
                                    }
                                #else
                                    LINS_nad = frame[LINS_PDU_D5_NEW_NAD_IDX];
                                #endif /* LINS_SAE_J2602_ENABLED */
                                    
                                    /* PCI is 0, so only the length is required. */
                                    LINS_srfBuffer[LINS_PDU_PCI_IDX] = 1u;
                                    
                                    /* RSID for a positive response is always SID + 0x40 */
                                    LINS_srfBuffer[LINS_PDU_SID_IDX] =
                                        LINS_NCS_POS_RESP_COND_CHANGE_NAD;
                                        
                                    /* Fills unused bytes with 0xFF. */
                                    for (i = 3u; i < LINS_FRAME_LEN; i++)
                                    {
                                        LINS_srfBuffer[i] = 0xFFu;
                                    }
                                    /* Sets a service response bit that indicates that the response is
                                     * ready to be sent to master node.
                                     */
                                    LINS_status |= LINS_STATUS_SRVC_RSP_RDY;
                                }
                                else
                                {
                                    /* Does nothing, ignores an erroneous request. */
                                }
                            }
                            else
                            {
                                /* Does nothing, ignores an erroneous request. */
                            }
                        }
                        else if (LINS_NCS_READ_BY_ID_SERIAL == frame[LINS_PDU_D1_ID_IDX])
                        {
                            if ((5u < frame[LINS_PDU_D2_BYTE_IDX]) &&
                                (0u != frame[LINS_PDU_D2_BYTE_IDX]))
                            {
                                /* Byte = 1 corresponds to first byte (LINS_serialNumber[0]) */
                                if (0u == ((LINS_serialNumber[frame[LINS_PDU_D2_BYTE_IDX] - 1u]
                                ^ frame[LINS_PDU_D4_INVERT_IDX]) & frame[LINS_PDU_D3_MASK_IDX]))
                                {
                                    /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                                    LINS_srfBuffer[LINS_PDU_NAD_IDX] = LINS_nad;
                                    /* Changes the NAD to a new NAD. */
                                   
                                #if (LINS_SAE_J2602_ENABLED)
                                    /* Tries setting a new NAD. Checks if the NAD is valid by trying to update the message IDs. */
                                    if ((l_u16) CYRET_BAD_PARAM == l_ifc_ioctl_LINS(L_IOCTL_SET_NAD, frame+LINS_PDU_D5_NEW_NAD_IDX))
                                    {
                                        break; /* The new NAD is invalid for the current frame count. */ 
                                    }
                                #else
                                    LINS_nad = frame[LINS_PDU_D5_NEW_NAD_IDX];
                                #endif /* LINS_SAE_J2602_ENABLED */
                                    
                                    /* PCI is 0, so only the length is required. */
                                    LINS_srfBuffer[LINS_PDU_PCI_IDX] = 1u;
                                    /* RSID for a positive response is always SID + 0x40. */
                                    LINS_srfBuffer[LINS_PDU_SID_IDX] =
                                        LINS_NCS_POS_RESP_COND_CHANGE_NAD;
                                        
                                    /* Fills unused bytes with 0xFF. */
                                    for (i = 3u; i < LINS_FRAME_LEN; i++)
                                    {
                                        LINS_srfBuffer[i] = 0xFFu;
                                    }
                                    /* Sets a service response bit that indicates that the response is
                                     * ready to be sent to Master node.
                                     */
                                    LINS_status |= LINS_STATUS_SRVC_RSP_RDY;
                                }
                                else
                                {
                                    /* Does nothing, ignores an erroneous request. */
                                }
                            }
                            else
                            {
                                /* Does nothing, ignores an erroneous request. */
                            }
                        }
                        else if ((frame[LINS_PDU_D1_IDX] >= 32u) &&
                                 (frame[LINS_PDU_D1_IDX] <= 63u))  /* User-defined identification */
                        {
                            /* Byte, Invert, and Mask must be stored in variables for user-defined
                             * identification as frame[] should contain user data after execution of
                             * ld_read_by_id_callout();
                             */
                            idByte = frame[LINS_PDU_D2_BYTE_IDX] - 1u;
                            idInvert = frame[LINS_PDU_D4_INVERT_IDX];
                            idMask = frame[LINS_PDU_D3_MASK_IDX];
                            
                            /* If the user doesn't re-assign the status of ld_read_by_id_callout(),
                             * LD_NEGATIVE_RESPONSE is always returned by ld_read_by_id_callout(). This
                             * indicates to the master that the service by the user defined identification is
                             * not supported. tempStatus is used to hold the status of ld_read_by_id_callout().
                             */
                            tempStatus = ld_read_by_id_callout_LINS(frame[LINS_PDU_D1_IDX],
                                frame + LINS_PDU_D1_IDX);
                                
                            if ((tempStatus == LD_NEGATIVE_RESPONSE) || (tempStatus == LD_NO_RESPONSE))
                            {
                                /* Does nothing as there is no response from the user. */
                            }
                            else
                            {
                                if (0u == ((frame[idByte + LINS_PDU_D1_IDX] ^ idInvert) & idMask))
                                {
                                    /* Changes the NAD to a new NAD. */
                                #if (LINS_SAE_J2602_ENABLED)
                                    /* Tries setting a new NAD. Checks if the NAD is valid by trying to update the message IDs. */
                                    if ((l_u16) CYRET_BAD_PARAM == l_ifc_ioctl_LINS(L_IOCTL_SET_NAD, frame+LINS_PDU_D5_NEW_NAD_IDX))
                                    {
                                        break; /* The new NAD is invalid for the current frame count. */ 
                                    }
                                #else
                                    LINS_nad = frame[LINS_PDU_D5_NEW_NAD_IDX];
                                #endif /* LINS_SAE_J2602_ENABLED */
                                    /* Fills SRF Buffer with a response to the service. NAD field should contain the changed NAD. */
                                    LINS_srfBuffer[LINS_PDU_NAD_IDX] = LINS_nad;
                                    /* PCI is 0, so only the length is required. */
                                    LINS_srfBuffer[LINS_PDU_PCI_IDX] = 1u;
                                    /* RSID for a positive response is always SID + 0x40. */
                                    LINS_srfBuffer[LINS_PDU_SID_IDX] =
                                        LINS_NCS_POS_RESP_COND_CHANGE_NAD;
                                    /* Fills unused bytes with 0xFF. */
                                    for (i = 3u; i < LINS_FRAME_LEN; i++)
                                    {
                                        LINS_srfBuffer[i] = 0xFFu;
                                    }
                                    /* Sets a service response bit that indicates that the response is
                                     * ready to be sent to master node.
                                     */
                                    LINS_status |= LINS_STATUS_SRVC_RSP_RDY;
                                }
                                else
                                {
                                    /* Does nothing, ignores an erroneous request */
                                }
                            }
                        }
                        else
                        {
                            /* Does nothing, ignores an erroneous request */
                        }
                    break;
                #endif /* (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB3_SEL) */

                #if (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB4_SEL))
                    case LINS_NCS_DATA_DUMP:
                    /* Checks the data length. Does not respond if the PCI length does not match this service data length. */
                    if (LINS_PDU_SF_DATA_LEN != (frame[LINS_PDU_PCI_IDX] & LINS_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                        /* Not supported. */
                    break;
                #endif /* (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB4_SEL) */

                #if (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB5_SEL))
                    /* Targeted reset for J2602. The node position detection for LIN20 is not supported. */
                    case LINS_NCS_ASSIGN_NAD_SNPD:
                    /* Checks the data length. Does not respond if the PCI length does not match this service data length. */
                    /* Only Save configuaration services B5 and B6 have PCI.length=1u */
                    if (1u != (frame[LINS_PDU_PCI_IDX] & LINS_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                    #if (LINS_SAE_J2602_ENABLED)
                        LINS_ioctlStatus |= LINS_IOCTL_STS_TARGET_RESET;
                        /* Sets the reset status in J2602_Status byte */
                        LINS_j2602Status_Set(LINS_J2602_STS_RESET);
                        
                        /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                        LINS_srfBuffer[LINS_PDU_NAD_IDX] = LINS_nad;
                        /* PCI is 0, so only the length is required. */
                        LINS_srfBuffer[LINS_PDU_PCI_IDX] = 6u;
                        /* RSID for a positive response is always SID + 0x40. */
                        LINS_srfBuffer[LINS_PDU_SID_IDX] =
                            LINS_NCS_POS_RESP_ASSIGN_NAD_SNPD;
                        /* Fills data fields with Supplier and function IDs. */
                        LINS_srfBuffer[LINS_PDU_D1_IDX] =
                            LO8(LINS_slaveId.supplierId);
                        LINS_srfBuffer[LINS_PDU_D2_IDX] =
                            HI8(LINS_slaveId.supplierId);
                        LINS_srfBuffer[LINS_PDU_D3_IDX] =
                            LO8(LINS_slaveId.functionId);
                        LINS_srfBuffer[LINS_PDU_D4_IDX] =
                            HI8(LINS_slaveId.functionId);
                        LINS_srfBuffer[LINS_PDU_D5_IDX] =
                            LINS_slaveId.variant;
                            
                         /* Sets a service response bit that indicates that the response is
                          * ready to be sent to master node.
                          */
                         LINS_status |= LINS_STATUS_SRVC_RSP_RDY;
                    #endif /* LINS_SAE_J2602_ENABLED */
                    
                    break;
                #endif /* (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB5_SEL) */

                #if (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB6_SEL))
                    case LINS_NCS_SAVE_CONFIG:
                    /* Checks the data length. Does not respond if the PCI length does not match this service data length. */
                    /* Only Save configuration services B5 and B6 have PCI.length=1u */
                    if (1u != (frame[LINS_PDU_PCI_IDX] & LINS_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                        /* Sets the save configuration bit in status register. */
                        LINS_ifcStatus |= LINS_IFC_STS_SAVE_CONFIG;
                        
                        /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                        LINS_srfBuffer[LINS_PDU_NAD_IDX] = LINS_nad;
                        /* PCI is 0, so only the length is required. */
                        LINS_srfBuffer[LINS_PDU_PCI_IDX] = 1u;
                        /* RSID for a positive response is always SID + 0x40. */
                        LINS_srfBuffer[LINS_PDU_SID_IDX] =
                            LINS_NCS_POS_RESP_SAVE_CONFIG;
                            
                        /* Fills unused data bytes with 0xFFs. */
                        for (i = 3u; i < LINS_FRAME_LEN; i++)
                        {
                            LINS_srfBuffer[i] = 0xFFu;
                        }
                        /* Set service response bit that indicates that response is ready to be sent to master node. */
                        LINS_status |= LINS_STATUS_SRVC_RSP_RDY;
                    break;
                #endif /* (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB6_SEL) */

                #if (0u != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB7_SEL))
                    case LINS_NCS_ASSIGN_FRAME_ID_RANGE:
                    /* Checks the data length. Does not respond if the PCI length does not match this service data length. */
                    if (LINS_PDU_SF_DATA_LEN != (frame[LINS_PDU_PCI_IDX] & LINS_PDU_PCI_LENGTH_MASK))
                    {
                        break;
                    }
                        /* Zeroes out the temp status. Used as an error counter. */
                        tempStatus = 0u;
                        for (i = 0u; i < LINS_NCS_MAX_FRAME_ID_RANGE; i++)
                        {
                            if ((i + frame[LINS_PDU_D1_START_IDX]) < LINS_NUM_FRAMES)
                            {
                                if ((frame[i + LINS_PDU_D2_PID_IDX] != LINS_INVALID_FRAME_PID) 
                                && ((frame[i + LINS_PDU_D2_PID_IDX] & LINS_PID_PARITY_MASK) 
                                     < LINS_FRAME_PID_MRF))
                                {
                                    /* Unassigned value "0" is used to invalidate this frame for transportation 
                                     * to the bus. Set new received PID value */
                                    LINS_volatileConfig[i + frame[LINS_PDU_D1_START_IDX]] =
                                        frame[i + LINS_PDU_D2_PID_IDX];
                                }
                                else if (frame[i + LINS_PDU_D2_PID_IDX] == 0xFFu)
                                {
                                    /* Does nothing. */
                                }
                                else
                                {
                                    /* Indicates an error by changing the status other than 0, if Frame ID is reserved. */
                                    tempStatus++;
                                }
                            }
                            else
                            {
                                if (frame[i + LINS_PDU_D2_PID_IDX] != 0xFFu)
                                {
                                    tempStatus++;  /* Indicates an error by changing the status other than 0. */
                                }
                            }
                        }

                        if (tempStatus == 0u) /* No errors condition check */
                        {
                            /* Fills SRF Buffer with a response to the service. NAD field should contain the current NAD. */
                            LINS_srfBuffer[LINS_PDU_NAD_IDX] = LINS_nad;
                            /* PCI is 0, so only the length is required. */
                            LINS_srfBuffer[LINS_PDU_PCI_IDX] = 1u;
                            /* RSID for a positive response is always SID + 0x40. */
                            LINS_srfBuffer[LINS_PDU_SID_IDX] =
                                LINS_NCS_POS_RESP_ASSIGN_FRAME_ID_RANGE;
                            /* Fills unused data bytes with 0xFFs. */
                            for (i = 3u; i < LINS_FRAME_LEN; i++)
                            {
                                LINS_srfBuffer[i] = 0xFFu;
                            }
                            /* Sets a service response bit that indicates that the response is
                             * ready to be sent to master node.
                             */
                            LINS_status |= LINS_STATUS_SRVC_RSP_RDY;
                        }
                        else
                        {
                            /* Does nothing, ignores an erroneous request */
                        }
                    break;
                #endif /* (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB7_SEL) */
            #endif /* LINS_CFG_SERVICES_ENABLED */
            
                default:
                    /* Indicates that the requested service is disabled and Frame will be "passed" to TL.
                     * This means the user should process this Frame properly using TL API. */
                    LINS_tlFlags |= LINS_TL_CS_SERVICE_DISABLED;
                    /* Indicates that the detected SID is a diagnostic SID and it should be passed to Transport Layer. */
                    LINS_tlFlags |= LINS_TL_DIAG_FRAME_DETECTED;
                break;
            }
            #if (LINS_SAE_J2602_ENABLED)
            } /* End of SID based configuration services, skipped for PID == LINS_FRAME_PID_MRF_J2602 */
            #endif /* LINS_SAE_J2602_ENABLED */
            if ((0u != (LINS_tlFlags & LINS_TL_CS_SERVICE_DISABLED)) ||
                (0u != (LINS_tlFlags & LINS_TL_DIAG_FRAME_DETECTED)))
            {
                /* SID used for diagnostics */
                if (frame[LINS_PDU_PCI_IDX] <= LINS_PDU_SF_DATA_LEN)
                {
                #if (LINS_COOKED_TL_API)
                    /* Gets one frame of a message if there is message pending. */
                    if (LINS_rxTlStatus == LD_IN_PROGRESS)
                    {
                        /* Makes sure the pointer points to the receive buffer beginning. */
                        if (0u != (LINS_tlFlags & LINS_TL_CS_SERVICE_DISABLED))
                        {
                            LINS_rxTlDataPointer = LINS_rxTlInitDataPointer;
                        }
                        /* Copies data to the user buffer. */
                        for (i = 0u; i < frame[LINS_PDU_PCI_IDX]; i++)
                        {
                            *LINS_rxTlDataPointer = frame[i + 2u];
                            LINS_rxTlDataPointer++;
                        }
                        /* Stores NAD */
                        *LINS_tlNadPointer = frame[LINS_PDU_NAD_IDX];
                        /* Gets the data bytes length. */
                        *LINS_tlLengthPointer = (l_u16) frame[LINS_PDU_PCI_IDX];
                        /* Updates the length pointer properly. */
                        LINS_rxMessageLength = 0u;
                        /* SF message is received, so sets the proper status */
                        LINS_rxTlStatus = LD_COMPLETED;
                    }
                #else /* Raw API */
                    if (LINS_rxBufDepth < (LINS_TL_RX_QUEUE_LEN / 8u))
                    {
                        /* Fills the RX queue from MRF buffer. */
                        for (i = 0u; i < LINS_FRAME_DATA_SIZE_8; i++)
                        {
                            LINS_rawRxQueue[LINS_rxWrIndex] = frame[i];
                            LINS_rxWrIndex++;
                        }
                        /* The Read index should point to the next byte in MRF. */
                        if (LINS_rxWrIndex == LINS_TL_RX_QUEUE_LEN)
                        {
                            LINS_rxWrIndex = 0u;
                        }
                        /* 8 Bytes copied to MRF - increment buffer depth */
                        LINS_rxBufDepth++;
                        /* Specification doesn't require status "queue full"
                         * so unconditionally set status to data available
                         */
                        LINS_rxTlStatus = LD_DATA_AVAILABLE;
                        LINS_rxMessageLength = 0u;
                    }
                #endif /* LINS_COOKED_TL_API */

                    return_value = LINS_RESPONSE_REQUIRED;
                }
                else
                {
                    /* Does nothing. The length is not valid, data should not be trusted. */
                    return_value = LINS_IGNORE_TL_FRAME;
                }

                /* Clears the Service Disabled and Diagnostic Frame detected bits
                 * and process diagnostic frame receiving into user buffer or MRF.
                 */
                LINS_tlFlags &= ((l_u8) ~(LINS_TL_CS_SERVICE_DISABLED | 
                    LINS_TL_DIAG_FRAME_DETECTED));
            }
            else
            {
                /* Does nothing. The length is not valid, data should not be trusted. */
                return_value = LINS_IGNORE_TL_FRAME;
            }
        }
        else if ((frame[LINS_PDU_PCI_IDX] & LINS_PDU_PCI_TYPE_MASK) ==
                 LINS_PDU_PCI_TYPE_FF)                 /* First Frame detected */
        {
            if (frame[LINS_PDU_LEN_IDX] >= LINS_FRAME_DATA_SIZE_7)
            {
            #if (LINS_COOKED_TL_API)
                /* Gets one frame of a message if there is message pending and PCI is valid. */
                LINS_rxPrevPci = LINS_PDU_PCI_TYPE_UNKNOWN;
                
                /* Get First Frame Length with following two operations */
                tmpByte = (frame[LINS_PDU_PCI_IDX] & ((l_u8) ~LINS_PDU_PCI_TYPE_MASK));
                tmpWord = ((l_u16) ((l_u16) tmpByte << 8u)) | ((l_u16) frame[LINS_PDU_LEN_IDX]);

                if ((LINS_rxTlStatus == LD_IN_PROGRESS) && (LINS_TL_BUF_LEN_MAX >= tmpWord))
                {
                    LINS_rxMessageLength = tmpWord;
                    /* Copy Length to current length variable */
                    *LINS_tlLengthPointer = LINS_rxMessageLength;
                    for (i = 3u; i < LINS_FRAME_DATA_SIZE_8; i++)
                    {
                        *LINS_rxTlDataPointer = frame[i];   /* Get Frame Data */
                        LINS_rxTlDataPointer++;
                    }
            #else /* Raw API */
                if (LINS_rxBufDepth < (LINS_TL_RX_QUEUE_LEN / 8u))
                {
                    /* Copy Length to current length variable */
                    LINS_rxMessageLength = (l_u16)((((l_u16) frame[LINS_PDU_PCI_IDX]) &
                        ((l_u16) ((l_u8) ~LINS_PDU_PCI_TYPE_MASK))) << 8u);
                    LINS_rxMessageLength |= ((l_u16) frame[LINS_PDU_LEN_IDX]);
                    
                    /* Fill MRF from frame buffer */
                    for (i = 0u; i < LINS_FRAME_DATA_SIZE_8; i++)
                    {
                        LINS_rawRxQueue[LINS_rxWrIndex] = frame[i];
                        LINS_rxWrIndex++;
                    }
                    /* "read" index should point to next byte in MRF */
                    if (LINS_rxWrIndex == LINS_TL_RX_QUEUE_LEN)
                    {
                        LINS_rxWrIndex = 0u;
                    }
                    /* 8 Bytes copied to MRF - increment buffer depth */
                    LINS_rxBufDepth++;
                    /* Specification doesn't require status "queue full",
                     * so unconditionally set status to data available
                     */
                    LINS_rxTlStatus = LD_DATA_AVAILABLE;
            #endif /* LINS_COOKED_TL_API */

                    /* Updates the length pointer properly. */
                    LINS_rxMessageLength -= LINS_FRAME_DATA_SIZE_5;
                    
                    /* Saves the state of the Frame Counter to monitor future possible errors. */
                    LINS_rxFrameCounter = 0u;
                    
                    /* Saves the PCI type */
                    LINS_rxPrevPci = LINS_PDU_PCI_TYPE_FF;
                    return_value = LINS_RECEIVE_CONTINUES;
                }
                else
                {
                    /* Does nothing. The length is not valid. */
                    return_value = LINS_IGNORE_TL_FRAME;
                }
            }
            else
            {
                /* Does nothing. The length is not valid, data should not be trusted. */
                return_value = LINS_IGNORE_TL_FRAME;
            }
        }
        /* Consecutive Frames detected */
        else if ((frame[LINS_PDU_PCI_IDX] & LINS_PDU_PCI_TYPE_MASK) ==
                 LINS_PDU_PCI_TYPE_CF)            /* Consecutive Frames detected */
        {
            /* Stores the previous PCI to avoid the MISRA warning. */
            tmpPci = LINS_rxPrevPci;
            
            /* Gets one frame of a message if there is message pending and the PCI is valid. */
            if ((tmpPci == LINS_PDU_PCI_TYPE_FF) || (tmpPci == LINS_PDU_PCI_TYPE_CF))
            {
                tmpByte = frame[LINS_PDU_PCI_IDX] & ((l_u8) ~LINS_PDU_PCI_TYPE_MASK);
                
                /* Checks if the frame counter is valid, the counter is always less than 16. */
                if (((LINS_rxFrameCounter + 1u) & 0x0Fu) == tmpByte)
                {
                #if (LINS_COOKED_TL_API)
                    /* Checks if a message is requested from the application. */
                    if (LINS_rxTlStatus == LD_IN_PROGRESS)
                    {
                        for (i = 2u; i < LINS_FRAME_DATA_SIZE_8; i++)
                        {
                            *LINS_rxTlDataPointer = frame[i];    /* Get Frame Data */
                            LINS_rxTlDataPointer++;
                        }
                #else /* Raw API */
                    if (LINS_rxBufDepth < (LINS_TL_RX_QUEUE_LEN / 8u))
                    {
                        /* Fills the MRF from the frame buffer. */
                        for (i = 0u; i < LINS_FRAME_DATA_SIZE_8; i++)
                        {
                            LINS_rawRxQueue[LINS_rxWrIndex] = frame[i];
                            LINS_rxWrIndex++;
                        }
                        /* The Read index should point to the next byte in MRF. */
                        if (LINS_rxWrIndex == LINS_TL_RX_QUEUE_LEN)
                        {
                            LINS_rxWrIndex = 0u;
                        }
                        /* 8 Bytes copied to MRF - increment buffer depth */
                        LINS_rxBufDepth++;
                        /* Specification doesn't require status of queue full
                         * so unconditionally set the status to data available
                         */
                        LINS_rxTlStatus = LD_DATA_AVAILABLE;
                #endif /* LINS_COOKED_TL_API */

                        /* Saves the current Frame Counter. */
                        LINS_rxFrameCounter = frame[LINS_PDU_PCI_IDX] & 
                            ((l_u8) ~LINS_PDU_PCI_TYPE_MASK);
                            
                        /* Saves PCI type */
                        LINS_rxPrevPci = LINS_PDU_PCI_TYPE_CF;
                        
                        /* Updates the length pointer properly. */
                        if (LINS_rxMessageLength > LINS_FRAME_DATA_SIZE_6)
                        {
                            LINS_rxMessageLength -= LINS_FRAME_DATA_SIZE_6;
                            return_value = LINS_RECEIVE_CONTINUES;
                        }
                        else
                        {
                            LINS_rxMessageLength = 0u;
                            return_value = LINS_RESPONSE_REQUIRED;
                        }
                    }
                    else
                    {
                        /* Indicates an error if the frame counter is invalid. */
                    }
                }
                else
                {
                    /* Indicates an an error if frame counter is invalid. */
                    LINS_rxTlStatus = LD_WRONG_SN;
                    return_value = LINS_ERRONEOUS_TL_FRAME;
                }
            }
            else
            {
                /* It is a spurious CF frame */
                return_value = LINS_IGNORE_TL_FRAME;
            }
        }
        else
        {
            /* Does nothing. SID is invalid. */
            return_value = LINS_IGNORE_TL_FRAME;
        }

        if (0u != (LINS_status & LINS_STATUS_SRVC_RSP_RDY))
        {
            /* Changes the node state to the next state. */
            return_value = LINS_RESPONSE_REQUIRED;
        }
    }
    else  /* Alien NAD. Indicate error */
    {
         /* Clears the service response ready status bit. */
        LINS_status &= ((l_u8) ~LINS_STATUS_SRVC_RSP_RDY);
        
    #if (LINS_COOKED_TL_API)
        /* Reception failed */
        if (0u != (LINS_tlFlags & LINS_TL_RX_REQUESTED))
        {
            LINS_rxTlStatus = LD_FAILED;
            LINS_txTlStatus = LD_FAILED;
            LINS_tlFlags &=((l_u8) ~LINS_TL_RX_REQUESTED);
        }
    #endif /* LINS_COOKED_TL_API */
    }
        
    if (LINS_NAD_FUNCTIONAL != LINS_mrfBuffer[LINS_PDU_NAD_IDX])
    {
        if ((return_value == LINS_RESPONSE_REQUIRED) ||
            (return_value == LINS_RECEIVE_CONTINUES))
        {
            LINS_tlFlags |= LINS_TL_N_CR_TIMEOUT_ON;
            LINS_tlTimeoutCnt = 0u;
        }
    }

    if (return_value == LINS_RESPONSE_REQUIRED)
    {
        if (0u == LINS_rxMessageLength)
        {
            /* Stores the previous PCI to avoid the MISRA warning. */
            tmpPci = LINS_rxPrevPci;
            /* Resets the frame counter. */
            LINS_rxFrameCounter = 0u;
            /* Previous PCI is required to be unknown at the beginning of a new message. */
            LINS_rxPrevPci = LINS_PDU_PCI_TYPE_UNKNOWN;

        #if (LINS_COOKED_TL_API)
            if (0u != (LINS_tlFlags & LINS_TL_RX_REQUESTED))
            {
                /* Indicates that a message is received. */
                LINS_rxTlStatus = LD_COMPLETED;
                /* Clears the  RX requested flag as a message was received. */
                LINS_tlFlags &= ((l_u8) ~LINS_TL_RX_REQUESTED);
            }
        #endif /* LINS_COOKED_TL_API */
        }
    }

    return (return_value);
}


#if (LINS_COOKED_TL_API)
    /*******************************************************************************
    * Function Name: ld_send_message
    ********************************************************************************
    *
    * Summary:
    *  The call packs the information specified by the data and length into one or
    *  multiple diagnostic frames. If the call is made in a master node application,
    *  the frames are transmitted to the slave node with the address NAD. If the
    *  call is made in a slave node application, the frames are transmitted to the
    *  master node with the address NAD. The parameter NAD is not used in slave
    *  nodes.
    
    *
    *  The value of the SID (or RSID) shall be the first byte in the data area.
    *
    *  The length must be in the range from 1 to 4095 bytes. The length shall also
    *  include the SID (or RSID) value, i.e. message length plus one.
    *
    *  The call is asynchronous, i.e. is not suspended until the message has been
    *  sent, and the buffer can not be changed by the application as long as calls
    *  to ld_tx_status returns LD_IN_PROGRESS.
    *
    *  The data is transmitted in suitable frames (the master request frame for
    *  master nodes and the slave response frame for slave nodes).
    *
    *  If there is a message in progress, the call returns with no action.
    *
    * Parameters:
    *  iii    - Interface.
    *  length - The size of data to be sent in bytes.
    *  nad    - the address of the slave node to which data is sent.
    *  data   - The array of data to be sent. The value of RSID is the first byte
    *           in the data area.
    *
    *******************************************************************************/
    void ld_send_message_LINS(l_u16 length, l_u8 nad, const l_u8* const ld_data)
    {
        volatile  l_u8 interruptState;

        /* NAD is not used in Slave node but the function definition is the same for Master and Slave. */
        (void) nad;

        if (LINS_txTlStatus != LD_IN_PROGRESS)
        {
        /* Interrupts can be disabled as global variables used by LIN ISR are used below. */
        interruptState = CyEnterCriticalSection();
            LINS_txTlDataPointer = ld_data;
            /* Resets data count */
            LINS_txTlDataCount = 0u;
            /* Sets up the length pointer. The length shouldn't be greater than LINS_TL_BUF_LEN_MAX. */
            LINS_txMessageLength = length;
            /* Indicates that there is a message in progress. */
            LINS_txTlStatus = LD_IN_PROGRESS;
            /* Indicates that a Cooked API request transmits data. */
            LINS_tlFlags |= LINS_TL_TX_REQUESTED;
        /* Restores the interrupt state. */
        CyExitCriticalSection(interruptState);
        }
    }


    /*******************************************************************************
    * Function Name: ld_receive_message
    ********************************************************************************
    *
    * Summary:
    *  The call prepares the LIN diagnostic module to receive one message and store
    *  it in the buffer pointed to by data. During the call, the length shall
    *  specify the maximum length allowed. When reception has completed, the length
    *  changes to the actual length and NAD to the NAD in the message.
    *
    *  SID (or RSID) are the first byte in the data area.
    *
    *  The length are in the range from 1 to 4095 bytes, but never more than the
    *  value originally set in the call. SID (or RSID) is included in the length.
    *
    *  The parameter NAD is not used in slave nodes.
    *
    *  The call is asynchronous, i.e. is not suspended until the message has been
    *  received, and the buffer can not be changed by the application as long as
    *  calls to ld_rx_status returns LD_IN_PROGRESS. If the call is made after the
    *  message transmission has started on the bus (i.e. the SF or FF is already
    *  transmitted), this message is be received. Instead, the function
    *  waits until a next message starts.
    *
    *  The data is received from the succeeding suitable frames (the master request
    *  frame for slave nodes and the slave response frame for master nodes).
    *
    *  The application shall monitor the ld_rx_status and shall not call this
    *  function until the status is LD_COMPLETED. Otherwise, this function may
    *  return inconsistent data in the parameters.
    *
    * Parameters:
    *  iii    - Interface.
    *  length - The size of data to be received in bytes.
    *  nad    - The address of Slave node from which data is received.
    *  data   - The array of data to be received. The value of the SID is the first
    *           byte in the data area.
    *
    *******************************************************************************/
    void ld_receive_message_LINS(l_u16* const length, l_u8* const nad, l_u8* const ld_data)
    {
        l_u8 interruptState;

        if (LINS_rxTlStatus != LD_IN_PROGRESS)
        {
        /* Interrupts can be disabled as global variables used by LIN ISR are used below. */
        interruptState = CyEnterCriticalSection();
        
            /* Sets user status bits */
            LINS_rxTlStatus = LD_IN_PROGRESS;
            /* Sets up the data pointer */
            LINS_rxTlDataPointer = ld_data;
            /* Sets up the initial data pointer that should always point to the user buffer beginning. */
            LINS_rxTlInitDataPointer = ld_data;
            /* Sets up a NAD pointer. */
            LINS_tlNadPointer = nad;
            
            /* Sets up the length pointer. */
            LINS_tlLengthPointer = length;
            LINS_rxMessageLength = *length;
            
            /* Indicates that Cooked API requests receive data */
            LINS_tlFlags |= LINS_TL_RX_REQUESTED;
            
        /* Restores interrupt state */
        CyExitCriticalSection(interruptState);
        }
    }


    /*******************************************************************************
    * Function Name: ld_tx_status
    ********************************************************************************
    *
    * Summary:
    *  The call returns the status of the last made call to ld_send_message.
    *
    * Parameters:
    *  iii - Interface.
    *
    * Return:
    *  LD_IN_PROGRESS - The transmission is not yet completed.
    *
    *  LD_COMPLETED - The transmission has completed successfully (and you can
    *                 issue a new ld_send_message call). This value is also
    *                 returned after initialization of Transport Layer.
    *
    *  LD_FAILED - The transmission ended in an error. The data was only partially
    *              sent. The transport layer is reinitialized before
    *              processing further messages. To find out why transmission has
    *              failed, check the status management function l_read_status.
    *
    *  LD_N_AS_TIMEOUT - The transmission failed because of a N_As timeout,
    *
    *  Note Failed status (e.g. LD_FAILED) could be read only once by this function.
    *       On the next read status will be turned to LD_COMPLETED by the transport
    *       layer reinitialization.
    *
    *******************************************************************************/
    l_u8 ld_tx_status_LINS(void)
    {
        l_u8 status = LINS_txTlStatus;
        
        if (LINS_txTlStatus != LD_IN_PROGRESS)
        {
            LINS_txTlStatus = LD_COMPLETED;
        }
        
        return (status);
    }


    /*******************************************************************************
    * Function Name: ld_rx_status
    ********************************************************************************
    *
    * Summary:
    *  The call returns the status of the last made call to ld_receive_message.
    *
    * Parameters:
    *  iii - Interface.
    *
    * Return:
    *  LD_IN_PROGRESS - The reception is not yet completed.
    *
    *  LD_COMPLETED - The reception has completed successfully and all information
    *                 (length, NAD, data) is available. (You can also issue a new
    *                 ld_receive_message call). This value is also returned after
    *                 initialization of Transport Layer.
    *
    *  LD_FAILED - The reception ended in an error. The data was only partially
    *              received and should not be trusted. Initialize before processing
    *              further transport layer messages. To find out why reception
    *              has failed, check the status management function l_read_status.
    *
    *  LD_N_CR_TIMEOUT - The reception failed because of a N_Cr timeout,
    *
    *  LD_WRONG_SN - The reception failed because of an unexpected sequence
    *                number.
    *
    *  Note Failed status (e.g. LD_WRONG_SN) could be read only once by this 
    *       function. On the next read status will be turned to LD_COMPLETED by
    *       the transport layer reinitialization.
    *
    *******************************************************************************/
    l_u8 ld_rx_status_LINS(void)
    {
        l_u8 status = LINS_rxTlStatus;
        
        if (LINS_rxTlStatus != LD_IN_PROGRESS)
        {
            LINS_rxTlStatus = LD_COMPLETED;
        }

        return(status);
    }
    
#else
    /*******************************************************************************
    * Function Name: ld_put_raw
    ********************************************************************************
    *
    * Summary:
    *  The call queues the transmission of 8 bytes of data in one frame. The data
    *  is sent in the next suitable frame (slave response frame). The data area
    *  is copied in the call, the pointer is be memorized. If no more
    *  queue resources are available, the data may be jettisoned and the
    *  appropriate error status is set.
    * Note:
    *  For J2602 mode on master requests with ID 0x3C and NAD from 0x80 to 0xFF 
    *  or ID 0x3E and NAD from 0x00 to 0xFF, API sends only 7 bytes of ld_data
    *  because Data0 contains J2602 Status byte.
    *
    * Parameters:
    *  iii - Interface.
    *  data - The array of data to be sent.
    *
    *******************************************************************************/
    void ld_put_raw_LINS(const l_u8* const ld_data)
    {
        l_u8 interruptState;
        l_u8 i;

        /* Interrupts can be disabled as global variables used by LIN ISR
         * are used below.
         */
        interruptState = CyEnterCriticalSection();

        /* Copies data only when the buffer is not full. */
        if (LINS_txBufDepth < (LINS_TL_TX_QUEUE_LEN / 8u))
        {
            /* Copies 8 bytes of data to Raw TX queue buffer. */
            for (i = 0u; i < LINS_FRAME_DATA_SIZE_8; i++)
            {
                /* Copies one byte of data to SRF buffer */
                LINS_rawTxQueue[LINS_txWrIndex] = *(ld_data + i);
                LINS_txWrIndex++;
            }

            /* If the buffer end is reached, resets the write index. */
            if (LINS_txWrIndex == LINS_TL_TX_QUEUE_LEN)
            {
                LINS_txWrIndex = 0u;
            }

            /* 8 bytes of data are copied, so increment the buffer depth */
            LINS_txBufDepth++;

            /* Updates the status properly */
            if (LINS_txBufDepth == (LINS_TL_TX_QUEUE_LEN / 8u))
            {
                LINS_txTlStatus = LD_QUEUE_FULL;
            }
            else
            {
                LINS_txTlStatus = LD_QUEUE_AVAILABLE;
            }
        
            LINS_tlFlags |= LINS_TL_TX_REQUESTED;
        }

        /* Restores the interrupt state */
        CyExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: ld_get_raw
    ********************************************************************************
    *
    * Summary:
    *  The call copies the oldest received diagnostic frame data to the memory
    *  specified by data. The data returned is received from the master request
    *  frame If the receive queue is empty, no data is copied.
    *
    * Parameters:
    *  iii  - Interface.
    *  data - The array to which the oldest received diagnostic frame data is
    *         copied.
    *
    *******************************************************************************/
    void ld_get_raw_LINS(l_u8* const ld_data)
    {
        l_u8 interruptState;
        l_u8 i;

        /* Interrupts can be disabled as global variables used by LIN ISR
         * are used below.
         */
        interruptState = CyEnterCriticalSection();

        /* If the queue is empty, does not copy anything. */
        if (LINS_rxBufDepth != 0u)
        {
            /* Copies 8 bytes of data from Raw RX queue buffer. */
            for (i = 0u; i < LINS_FRAME_DATA_SIZE_8; i++)
            {
                *(ld_data + i) = LINS_rawRxQueue[LINS_rxRdIndex];
                LINS_rxRdIndex++;
            }

            /* 8 bytes of data are copied. Decrement buffer depth */
            LINS_rxBufDepth--;

            /* If buffer end is reached, go to start */
            if (LINS_rxRdIndex == LINS_TL_RX_QUEUE_LEN)
            {
                LINS_rxRdIndex = 0u;
            }

            /* Update status properly */
            if (LINS_rxBufDepth == 0u)
            {
                LINS_rxTlStatus = LD_NO_DATA;
            }
            else
            {
                LINS_rxTlStatus = LD_DATA_AVAILABLE;
            }
        }

        /* Restore interrupt state */
        CyExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: ld_raw_tx_status
    ********************************************************************************
    *
    * Summary:
    *  The call returns the status of the raw frame transmission function.
    *
    * Parameters:
    *  iii - Interface.
    *
    * Return:
    *  Status:
    *   LD_QUEUE_EMPTY - The transmit queue is empty. If previous calls to
    *   ld_put_raw, all frames in the queue have been transmitted.
    *
    *   LD_QUEUE_AVAILABLE - The transmit queue contains entries, but is not full.
    *
    *   LD_QUEUE_FULL - The transmit queue is full and can not accept further
    *   frames.
    *
    *   LD_TRANSMIT_ERROR - LIN protocol errors occur during the transfer;
    *   initialize and redo the transfer.
    *
    *******************************************************************************/
    l_u8 ld_raw_tx_status_LINS(void)
    {
        return (LINS_txTlStatus);
    }


    /*******************************************************************************
    * Function Name: LINS_ld_raw_rx_status
    ********************************************************************************
    *
    * Summary:
    *  The call returns the status of the raw frame receive function.
    *
    * Parameters:
    *  iii - Interface.
    *
    * Return:
    *  Status:
    *   LD_NO_DATA - The receive queue is empty.
    *
    *   LD_DATA_AVAILABLE - The receive queue contains data that can be read.
    *
    *   LD_RECEIVE_ERROR - LIN protocol errors occur during the transfer;
    *
    *   initialize and redo the transfer.
    *
    *******************************************************************************/
    l_u8 ld_raw_rx_status_LINS(void)
    {
        return (LINS_rxTlStatus);
    }
#endif /* LINS_COOKED_TL_API */


#endif /* LINS_TL_USED */


/*******************************************************************************
* Function Name: LINS_TimerEnableInt
********************************************************************************
*
* Summary:
*  Enables Systick timer interrupts and configures the SystemTick timer callback
*  slot to the component function that updates the timestamp value.
*
* Return:
*  Status
*    Value                 Description
*     CYRET_SUCCESS         Successful
*     CYRET_INVALID_STATE   Failure - No free callback slot has been found.
*
*******************************************************************************/
cystatus LINS_TimerEnableInt(void)
{
    cystatus retValue = CYRET_INVALID_STATE;
    uint32 i;

    /* If SysTick timer isn't configured */
    if (CySysTickInitVar == 0u)
    {
        /* SysTick timer seems to be not configured.
         * Configure SysTick timer and set callback.
         */
        CySysTickStart();
        (void) CySysTickSetCallback(0u, &l_ifc_aux_LINS);
        retValue = CYRET_SUCCESS;
    }
    else
    {
        /* If SysTick timer is configured, checks if its own callback exists. */
        for (i = 0u; i < CY_SYS_SYST_NUM_OF_CALLBACKS; i++)
        {
            if (CySysTickGetCallback(i) == &l_ifc_aux_LINS)
            {
                retValue = CYRET_SUCCESS;
                break;
            }
        }

        /* Looks for an unused callback slot to fill it with its own callback. */
        if (CYRET_SUCCESS != retValue)
        {
            for (i = 0u; i < CY_SYS_SYST_NUM_OF_CALLBACKS; i++)
            {
                /* Makes sure that the callback slot is not used. */
                if (CySysTickGetCallback(i) == (void *) 0 )
                {
                    (void) CySysTickSetCallback(i, &l_ifc_aux_LINS);
                    retValue = CYRET_SUCCESS;
                    break;
                }
            }
        }
    }
    return (retValue);
}


/*******************************************************************************
* Function Name: LINS_TimerDisableInt
********************************************************************************
*
* Summary:
*  Disables Systick timer interrupts.
*
* Side Effects:
*  Stops the SysTick timer if there are no active callback slots left and
*  disable the SysTick interrupt.
*
*******************************************************************************/
void LINS_TimerDisableInt(void)
{
    uint32 i;
    uint32 nullPointers = 0u;

    /* Find used callback slot */
    for (i = 0u; i < CY_SYS_SYST_NUM_OF_CALLBACKS; i++)
    {
        if (CySysTickGetCallback(i) == &l_ifc_aux_LINS)
        {
            /* Free callback */
            (void)CySysTickSetCallback(i, (void *) 0 );
            nullPointers++;
        }
        else if (CySysTickGetCallback(i) == (void *) 0 )
        {
            /* This callback slot is unused */
            nullPointers++;
        }
        else
        {
            /* This callback slot is used by some function */
        }
    }

    if (nullPointers == CY_SYS_SYST_NUM_OF_CALLBACKS)
    {
        /* If there are no used callback slots - disable SysTick timer */
        CySysTickDisableInterrupt();
        CySysTickStop();
    }
}

/* [] END OF FILE */
