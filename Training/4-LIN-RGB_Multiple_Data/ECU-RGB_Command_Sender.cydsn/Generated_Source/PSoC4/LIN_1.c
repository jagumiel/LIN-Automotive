/*******************************************************************************
* File Name: LIN_1.c
* Version 4.0
*
* Description:
*  This file contains the implementation of the LIN Slave component.
*
********************************************************************************
* Copyright 2011-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LIN_1.h"
#include "LIN_Dynamic.h"
#include "cyapicallbacks.h"


/*******************************************************************************
*  Place your includes, defines and variable declarations here
*******************************************************************************/
/* `#START LIN_1_DECLARATIONS` */

/* `#END` */

/* Internal APIs */
static l_u8    LIN_1_FindPidIndex(l_u8 pid);
static void    LIN_1_EndFrame(l_u8 status);
static void    LIN_1_SetAssociatedFlags(l_u8 pIndex);

/* SAE J2602 is disabled */
#if(0u == LIN_1_SAE_J2602)

    static l_bool  LIN_1_GetEtFlagValue(l_u8 pidIndex);
    static void    LIN_1_ClearEtFlagValue(l_u8 pidIndex);

#endif /* (0u == LIN_1_SAE_J2602) */

#if(1u == LIN_1_TL_ENABLED)

    static l_u8 LIN_1_TransmitTLFrame(l_u8 stimulus);
    static void LIN_1_ClrRxBuffer(l_u8 stimulus);
    static void LIN_1_ClrTxBuffer(l_u8 stimulus);
    static void LIN_1_CheckNodeState(l_u8 stimulus);
    static l_u8 LIN_1_ProcessMrf(l_u8 frame[]);

    #if(1u == LIN_1_CS_ENABLED)

        #if((0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB0_SEL)) || \
            (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL)))

            static l_bool LIN_1_LinProductId(volatile const l_u8 frameData[]);

        #endif /* ((0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB0_SEL)) ||
                  (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL)))  */

        #if(0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB1_SEL))
            #if(1u == LIN_1_LIN_2_0)

                static l_u8 LIN_1_MessageId(volatile const l_u8* frameData);

            #endif /* (1u == LIN_1_LIN_2_0) */
        #endif /* (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB1_SEL)) */

    #endif /* (1u == LIN_1_CS_ENABLED) */

    /* RAM copy of the slave NAD */
    static volatile l_u8 LIN_1_nad;

#endif /* (1u == LIN_1_TL_ENABLED) */

#if(1u == LIN_1_TL_ENABLED)

    #if(1u == LIN_1_LIN_2_0)
        #if(0u != (LIN_1_CS_SEL_SERVICES01 & (LIN_1_NCS_0xB2_SEL | \
                LIN_1_NCS_0xB1_SEL)))

        /* Message ID Table */
static l_u16 const CYCODE messageIdTable[4] = {0x003Bu, 0x000Au, 0x000Eu, 0x0001u};

        #endif /* (0u != (LIN_1_CS_SEL_SERVICES01 & (LIN_1_NCS_0xB2_SEL |
               * LIN_1_NCS_0xB1_SEL)))
               */

    #endif /* (1u == LIN_1_LIN_2_0) */

#endif /* (1u == LIN_1_TL_ENABLED) */

#if(1u == LIN_1_INACTIVITY_ENABLED)

    /* Free-running timer */
    static uint32 LIN_1_periodCounter;

#endif /* (1u == LIN_1_INACTIVITY_ENABLED) */


static volatile l_u8  LIN_1_status;       /* Internal Status                  */
static volatile l_u8  LIN_1_syncCounts;   /* Sync Field Timer Counts          */
static volatile l_u8  LIN_1_auxStatus;    /* Internal AUX ISR shadow status   */
static volatile l_u16 LIN_1_ioctlStatus;  /* Status used by l_ifc_ioctl()     */
static volatile l_u16 LIN_1_ifcStatus;    /* Interface communication status   */
static volatile l_u8  LIN_1_uartFsmState; /* Current state of the UART ISR    */
static volatile l_u8  LIN_1_fsmFlags;

/* Initial clock divider */
l_u16 LIN_1_initialClockDivider;

#if(1u == LIN_1_SAE_J2602)

    /* J2602 status variable */
    static l_u8 LIN_1_j2602Status;

#endif  /* (1u == LIN_1_SAE_J2602) */

/* Notification API statuses */
static l_u8 LIN_1_statusFlagArray[LIN_1_SIG_FRAME_FLAGS_SIZE];

/* SAE J2602 is disabled */
#if(0u == LIN_1_SAE_J2602)

    #if(0u != LIN_1_NUM_ET_FRAMES)
        static l_u8 LIN_1_etFrameFlags[LIN_1_ET_FRAMES_FLAGS_SIZE];
    #endif /* (0u != LIN_1_NUM_ET_FRAMES) */

#endif /* (0u == LIN_1_SAE_J2602) */

/* RAM copy of the slave configuration data */
static volatile l_u8 LIN_1_volatileConfig[LIN_1_NUM_FRAMES];


/****************************************************
*   Transport Layer API
*****************************************************/
#if(1u == LIN_1_TL_ENABLED)

    #if(1u == LIN_1_CS_ENABLED)

        #if(0u != (LIN_1_CS_SEL_SERVICES01 & (LIN_1_NCS_0xB0_SEL | LIN_1_NCS_0xB1_SEL \
                    | LIN_1_NCS_0xB2_SEL | LIN_1_NCS_0xB3_SEL)))

            /* LIN Slave Identification */
            static const LIN_1_SLAVE_ID CYCODE LIN_1_slaveId =
            {
                LIN_1_CS_SUPPLIER_ID,
                LIN_1_CS_FUNCTION_ID,
                LIN_1_CS_VARIANT
            };

        #endif /*(0u != (LIN_1_CS_SEL_SERVICES01 & (LIN_1_NCS_0xB0_SEL
               * | LIN_1_NCS_0xB1_SEL | LIN_1_NCS_0xB2_SEL |
               * LIN_1_NCS_0xB3_SEL))
               */

        /* Serial Number */
        static l_u8* LIN_1_serialNumber;

    #endif /* (1u == LIN_1_CS_ENABLED) */

    /* MRF buffer */
    static l_u8 LIN_1_mrfBuffer[LIN_1_FRAME_BUFF_LEN];

    /* SRF buffer */
    static volatile l_u8 LIN_1_srfBuffer[LIN_1_FRAME_BUFF_LEN];

    /* Transport Layer Rx and Tx Statuses */
    static volatile l_u8 LIN_1_txTlStatus;
    static volatile l_u8 LIN_1_rxTlStatus;

    /* Flags that are used for Transport Layer */
    static volatile l_u8 LIN_1_tlFlags;
    static l_u8 LIN_1_nodeState = LIN_1_IDLE;

    /* Internal variable used to store the PCI of the previously transmitted frame */
    static l_u8 LIN_1_txPrevPci;

    /* Internal variable used to store the PCI of the previously received frame */
    static l_u8 LIN_1_rxPrevPci;

    static volatile l_u16  LIN_1_txMessageLength = 0u;
    static volatile l_u16  LIN_1_rxMessageLength = 0u;
    static volatile l_u8   LIN_1_txFrameCounter = 0u;
    static volatile l_u8   LIN_1_rxFrameCounter = 0u;
    static volatile uint32 LIN_1_tlTimeoutCnt   = 0u;

    #if(1u == LIN_1_TL_API_FORMAT)

        static const    l_u8*  LIN_1_txTlDataPointer     = NULL;
        static volatile l_u16  LIN_1_txTlDataCount;
        static volatile l_u8*  LIN_1_rxTlDataPointer     = NULL;
        static volatile l_u8*  LIN_1_rxTlInitDataPointer = NULL;
        static volatile l_u8*  LIN_1_tlNadPointer      = NULL;
        static volatile l_u16* LIN_1_tlLengthPointer   = NULL;

    #else

        /* Internal variables for buffer indexing */
        static l_u8 LIN_1_txBufDepth;
        static l_u8 LIN_1_rxBufDepth;

        /* The Master Request Frame (MRF) buffer */
        static volatile l_u8 LIN_1_rawRxQueue[LIN_1_TL_RX_QUEUE_LEN];

        /* The Slave Response Frame (SRF) buffer */
        static volatile l_u8 LIN_1_rawTxQueue[LIN_1_TL_TX_QUEUE_LEN];

        /* TX buffer indexes */
        static volatile l_u16 LIN_1_txWrIndex;
        static volatile l_u16 LIN_1_txRdIndex;

        /* RX buffer indexes */
        static volatile l_u16 LIN_1_rxWrIndex;
        static volatile l_u16 LIN_1_rxRdIndex;

    #endif /* (1u == LIN_1_TL_API_FORMAT) */

#endif /* (1u == LIN_1_TL_ENABLED) */


/* LIN slave configuration data */
static LIN_1_SLAVE_CONFIG const CYCODE LIN_1_LinSlaveConfig = {
    LIN_1_TL_INITIAL_NAD,

    /* Frame PID Table */
    {0xFBu, 0xCAu, 0x8Eu, 0xC1u}
};


/* Frames declaration with initial signals values */
static volatile l_u8  LIN_1_Buffer_Frame1[8u] = {0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u};
static volatile l_u8  LIN_1_Buffer_Frame2[8u] = {0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u};
static volatile l_u8  LIN_1_Buffer_Frame3[8u] = {0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u};
static volatile l_u8  LIN_1_Buffer_Frame4[8u] = {0xFEu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu};



/*******************************************************************************
*  Place your code here
*******************************************************************************/
/* `#START LIN_1_CODE` */

/* `#END` */


/*******************************************************************************
* Function Name: LIN_1_Start
********************************************************************************
*
* Summary:
*  Starts the component operation. This function is not required to use.
*
* Parameters:
*  None
*
* Return:
*  Zero     - If the initialization succeeds.
*  Non-zero - If the initialization fails.
*
*******************************************************************************/
l_bool LIN_1_Start(void)
{
    l_bool returnValue;

    /* Call the initialization function
    *  which do nothing so can be called safely from any instance of API */

    (void) l_sys_init();

    /* Start LIN components */
    returnValue = l_ifc_init_LIN_1();

    return (returnValue);
}


/*******************************************************************************
* Function Name: LIN_1_Stop
********************************************************************************
*
* Summary:
*  Stops the component operation. This function is not required to use.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LIN_1_Stop(void)
{
    /* Stop UART */
    LIN_1_SCB_Stop();

    /* Clear bit corresponding to current instance in SysTimerEnabledFlag
    *  and stop SySTick timer only if both flags are cleared */

    #if(1u == LIN_1_INACTIVITY_ENABLED)

        /* Stop SysTick timer if it was enabled before */
        LIN_1_TimerDisableInt();

    #endif /* (1u == LIN_1_INACTIVITY_ENABLED) */

    /* Disable interrupts from this instance  */
    LIN_1_ISR_Disable();
}


/* Signal interaction and notification API */






/*******************************************************************************
* Function Name: l_bytes_rd_LIN_1
********************************************************************************
*
* Summary:
*  Reads and returns the current values of the selected bytes in the signal.
*  The sum of the "start" and "count" parameters must never be greater than the
*  length of the byte array. Note that when the sum of "start" and "count" is
*  greater than the length of the signal byte array then an accidental data is
*  read.
*
*  If an invalid signal handle is passed into the function, no action is done.
*  Assume that a byte array is 8 bytes long, numbered 0 to 7. Reading bytes from
*  2 to 6 from user selected array requires start to be 2 (skipping byte 0 and 1)
*  and count to be 5. In this case byte 2 is written to user_selected_array[0]
*  and all consecutive bytes are written into user_selected_array in ascending
*  order.
*
* Parameters:
*  sss   - Signal handle of the signal to read.
*  start - First byte to read from.
*  count - Number of bytes to read.
*  data  - Pointer to array, in which the data read from the signal is stored.
*
* Return:
*  None
*
*******************************************************************************/
void l_bytes_rd_LIN_1(l_signal_handle sss, l_u8 start, l_u8 count, l_u8* pData)
{
    switch(sss)
    {
        case Klappe_Status_SIGNAL_HANDLE:
            l_bytes_rd_Klappe_Status(start, count, pData);
        break;

        case SSA_Status_SIGNAL_HANDLE:
            l_bytes_rd_SSA_Status(start, count, pData);
        break;

        default:
        break;
    }
}


/*******************************************************************************
* Function Name: l_bytes_rd_Klappe_Status
********************************************************************************
*
* Summary:
*  Reads and returns the current values of the selected bytes in the signal.
*  The sum of the "start" and "count" parameters must never be greater than the
*  length of the byte array. Note that when the sum of "start" and "count" is
*  greater than the length of the signal byte array then an accidental data is
*  read.
*
*  If an invalid signal handle is passed into the function, no action is done.
*  Assume that a byte array is 8 bytes long, numbered 0 to 7. Reading bytes from
*  2 to 6 from user selected array requires start to be 2 (skipping byte 0 and 1)
*  and count to be 5. In this case byte 2 is written to user_selected_array[0]
*  and all consecutive bytes are written into user_selected_array in ascending
*  order.
*
* Parameters:
*  start - First byte to read from.
*  count - Number of bytes to read.
*  data  - Pointer to array, in which the data read from the signal is stored.
*
* Return:
*  None
*
*******************************************************************************/
void l_bytes_rd_Klappe_Status(l_u8 start, l_u8 count, l_u8 pData[]) 
{
    l_u8 i;
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    for(i = 0u; i < count; i++)
    {
        pData[i] = LIN_1_Buffer_Frame1[LIN_1_Klappe_Status_Frame1_SIG_BYTE_OFFSET + start + i];
    }

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}

/*******************************************************************************
* Function Name: l_bytes_rd_SSA_Status
********************************************************************************
*
* Summary:
*  Reads and returns the current values of the selected bytes in the signal.
*  The sum of the "start" and "count" parameters must never be greater than the
*  length of the byte array. Note that when the sum of "start" and "count" is
*  greater than the length of the signal byte array then an accidental data is
*  read.
*
*  If an invalid signal handle is passed into the function, no action is done.
*  Assume that a byte array is 8 bytes long, numbered 0 to 7. Reading bytes from
*  2 to 6 from user selected array requires start to be 2 (skipping byte 0 and 1)
*  and count to be 5. In this case byte 2 is written to user_selected_array[0]
*  and all consecutive bytes are written into user_selected_array in ascending
*  order.
*
* Parameters:
*  start - First byte to read from.
*  count - Number of bytes to read.
*  data  - Pointer to array, in which the data read from the signal is stored.
*
* Return:
*  None
*
*******************************************************************************/
void l_bytes_rd_SSA_Status(l_u8 start, l_u8 count, l_u8 pData[]) 
{
    l_u8 i;
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    for(i = 0u; i < count; i++)
    {
        pData[i] = LIN_1_Buffer_Frame2[LIN_1_SSA_Status_Frame2_SIG_BYTE_OFFSET + start + i];
    }

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}






/*******************************************************************************
* Function Name: l_bool_wr_LIN_1
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
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_bool_wr_LIN_1(l_signal_handle sss, l_bool v)
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
* Return:
*  None
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
        LIN_1_Buffer_Frame4[LIN_1_Response_Error_Frame4_SIG_BYTE_OFFSET] |= 
                            LIN_1_Response_Error_Frame4_SIG_MASK_0;
    }
    else
    {
        LIN_1_Buffer_Frame4[LIN_1_Response_Error_Frame4_SIG_BYTE_OFFSET] &= 
                            (l_u8) ~LIN_1_Response_Error_Frame4_SIG_MASK_0;
    }

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}









/*******************************************************************************
* Function Name: l_bytes_wr_LIN_1
********************************************************************************
*
* Summary:
*  Writes the current value of the selected bytes to the signal specified by
*  the name "sss". The sum of "start" and "count" must never be greater than
*  the length of the byte array, although the device driver may choose not to
*  enforce this in runtime. Note that when the sum of "start" and "count" is
*  greater than the length of the signal byte array then an accidental memory
*  area is to be affected.
*
*  If an invalid signal handle is passed into the function, no action is done.
*  Assume that a byte array signal is 8 bytes long, numbered 0 to 7. Writing
*  byte 3 and 4 of this array requires "start" to be 3 (skipping byte 0, 1 and
*  2) and "count" to be 2. In this case byte 3 of the byte array signal is
*  written from user_selected_array[0] and byte 4 is written from
*  user_selected_array[1].
*
* Parameters:
*  sss   - signal handle of the signal to write.
*  start - first byte to write to.
*  count - number of bytes to write.
*  data  - pointer to array, in which the data to transmit to LIN master is
*          located.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_bytes_wr_LIN_1(l_signal_handle sss, l_u8 start, l_u8 count, const l_u8* const pData)
{
    switch(sss)
    {
        case Bedienung_SIGNAL_HANDLE:
            l_bytes_wr_Bedienung(start, count, pData);
            break;

        default:
            break;
    }
}


/*******************************************************************************
* Function Name: l_bytes_wr_Bedienung
********************************************************************************
*
* Summary:
*  Writes the current value of the selected bytes to the signal specified by
*  the name "sss". The sum of "start" and "count" must never be greater than
*  the length of the byte array, although the device driver may choose not to
*  enforce this in runtime. Note that when the sum of "start" and "count" is
*  greater than the length of the signal byte array then an accidental memory
*  area is to be affected.
*
*  If an invalid signal handle is passed into the function, no action is done.
*  Assume that a byte array signal is 8 bytes long, numbered 0 to 7. Writing
*  byte 3 and 4 of this array requires "start" to be 3 (skipping byte 0, 1 and
*  2) and "count" to be 2. In this case byte 3 of the byte array signal is
*  written from user_selected_array[0] and byte 4 is written from
*  user_selected_array[1].
*
* Parameters:
*  start - first byte to write to.
*  count - number of bytes to write.
*  data  - pointer to array, in which the data to transmit to LIN master is
*          located.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_bytes_wr_Bedienung(l_u8 start, l_u8 count, const l_u8* const pData)
{
    l_u8 i;
    uint8 interruptState;

    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

    for(i = 0u; i < count; i++)
    {
        LIN_1_Buffer_Frame3[LIN_1_Bedienung_Frame3_SIG_BYTE_OFFSET + start + i] = pData[i];
    }

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);
}






/*******************************************************************************
* Function Name: l_flg_tst_LIN_1
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
l_bool l_flg_tst_LIN_1(l_flag_handle fff)
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

        case Klappe_Status_FLAG_HANDLE:
            returnValue = l_flg_tst_Klappe_Status();
        break;

        case SSA_Status_FLAG_HANDLE:
            returnValue = l_flg_tst_SSA_Status();
        break;

        case Bedienung_FLAG_HANDLE:
            returnValue = l_flg_tst_Bedienung();
        break;

        case Frame1_FLAG_HANDLE:
            returnValue = l_flg_tst_Frame1();
        break;

        case Frame2_FLAG_HANDLE:
            returnValue = l_flg_tst_Frame2();
        break;

        case Frame3_FLAG_HANDLE:
            returnValue = l_flg_tst_Frame3();
        break;

        case Frame4_FLAG_HANDLE:
            returnValue = l_flg_tst_Frame4();
        break;

        default:
            returnValue = LIN_1_TRUE;
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
* Parameters:
*  None
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

    if(0u != (LIN_1_statusFlagArray[LIN_1_MRF_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_MRF_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }
    else
    {
        result = LIN_1_FALSE;
    }
    return(result);
}


/*******************************************************************************
* Function Name: l_flg_tst_SRF
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the Slave Response Frame
*  (SRF), i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Parameters:
*  None
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

    if(0u != (LIN_1_statusFlagArray[LIN_1_SRF_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_SRF_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }
    else
    {
        result = LIN_1_FALSE;
    }
    return(result);
}

/*******************************************************************************
* Function Name: l_flg_tst_Klappe_Status
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Parameters:
*  None
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_Klappe_Status(void)
{
    l_bool result = LIN_1_FALSE;

    if(0u != (LIN_1_statusFlagArray[LIN_1_Klappe_Status_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_Klappe_Status_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return(result);
}

/*******************************************************************************
* Function Name: l_flg_tst_SSA_Status
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Parameters:
*  None
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_SSA_Status(void)
{
    l_bool result = LIN_1_FALSE;

    if(0u != (LIN_1_statusFlagArray[LIN_1_SSA_Status_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_SSA_Status_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return(result);
}

/*******************************************************************************
* Function Name: l_flg_tst_Bedienung
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Parameters:
*  None
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_Bedienung(void)
{
    l_bool result = LIN_1_FALSE;

    if(0u != (LIN_1_statusFlagArray[LIN_1_Bedienung_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_Bedienung_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return(result);
}

/*******************************************************************************
* Function Name: l_flg_tst_Frame1
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Parameters:
*  None
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_Frame1(void)
{
    l_bool result = LIN_1_FALSE;

    if(0u != (LIN_1_statusFlagArray[LIN_1_Frame1_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_Frame1_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return(result);
}

/*******************************************************************************
* Function Name: l_flg_tst_Frame2
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Parameters:
*  None
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_Frame2(void)
{
    l_bool result = LIN_1_FALSE;

    if(0u != (LIN_1_statusFlagArray[LIN_1_Frame2_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_Frame2_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return(result);
}

/*******************************************************************************
* Function Name: l_flg_tst_Frame3
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Parameters:
*  None
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_Frame3(void)
{
    l_bool result = LIN_1_FALSE;

    if(0u != (LIN_1_statusFlagArray[LIN_1_Frame3_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_Frame3_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return(result);
}

/*******************************************************************************
* Function Name: l_flg_tst_Frame4
********************************************************************************
*
* Summary:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff, i.e. returns zero if the flag is cleared, non-zero otherwise.
*
* Parameters:
*  None
*
* Return:
*  Returns a C boolean indicating the current state of the flag specified by
*  the name fff:
*  False - if the flag is cleared;
*  True - if the flag is not cleared.
*
*******************************************************************************/
l_bool l_flg_tst_Frame4(void)
{
    l_bool result = LIN_1_FALSE;

    if(0u != (LIN_1_statusFlagArray[LIN_1_Frame4_FRAME_FLAG_BYTE_OFFSET_0] &
    LIN_1_Frame4_FRAME_FLAG_MASK_0))
    {
        result = LIN_1_TRUE;
    }

    return(result);
}






/*******************************************************************************
* Function Name: l_flg_clr_LIN_1
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Parameters:
*  fff - is the name of the flag handle.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_LIN_1(l_flag_handle fff)
{
    switch(fff)
    {
        case MRF_FLAG_HANDLE:
            l_flg_clr_MRF();
        break;

        case SRF_FLAG_HANDLE:
            l_flg_clr_SRF();        break;

        case Klappe_Status_FLAG_HANDLE:
            l_flg_clr_Klappe_Status();
        break;

        case SSA_Status_FLAG_HANDLE:
            l_flg_clr_SSA_Status();
        break;

        case Bedienung_FLAG_HANDLE:
            l_flg_clr_Bedienung();
        break;

        case Frame1_FLAG_HANDLE:
            l_flg_clr_Frame1();
        break;

        case Frame2_FLAG_HANDLE:
            l_flg_clr_Frame2();
        break;

        case Frame3_FLAG_HANDLE:
            l_flg_clr_Frame3();
        break;

        case Frame4_FLAG_HANDLE:
            l_flg_clr_Frame4();
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
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_MRF(void)
{
    LIN_1_statusFlagArray[LIN_1_MRF_FRAME_FLAG_BYTE_OFFSET_0] &=
        ((l_u8) ~LIN_1_MRF_FRAME_FLAG_MASK_0);
}


/*******************************************************************************
* Function Name: l_flg_clr_SRF
********************************************************************************
*
* Summary:
*  Sets the current value of the Slave Response Frame (SRF) flag.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_SRF(void)
{
    LIN_1_statusFlagArray[LIN_1_SRF_FRAME_FLAG_BYTE_OFFSET_0] &=
    ((l_u8) ~LIN_1_SRF_FRAME_FLAG_MASK_0);
}

/*******************************************************************************
* Function Name: l_flg_clr_Klappe_Status
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_Klappe_Status(void)
{
    LIN_1_statusFlagArray[LIN_1_Klappe_Status_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_Klappe_Status_FRAME_FLAG_MASK_0);
}

/*******************************************************************************
* Function Name: l_flg_clr_SSA_Status
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_SSA_Status(void)
{
    LIN_1_statusFlagArray[LIN_1_SSA_Status_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_SSA_Status_FRAME_FLAG_MASK_0);
}

/*******************************************************************************
* Function Name: l_flg_clr_Bedienung
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_Bedienung(void)
{
    LIN_1_statusFlagArray[LIN_1_Bedienung_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_Bedienung_FRAME_FLAG_MASK_0);
}

/*******************************************************************************
* Function Name: l_flg_clr_Frame1
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_Frame1(void)
{
    LIN_1_statusFlagArray[LIN_1_Frame1_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_Frame1_FRAME_FLAG_MASK_0);
}

/*******************************************************************************
* Function Name: l_flg_clr_Frame2
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_Frame2(void)
{
    LIN_1_statusFlagArray[LIN_1_Frame2_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_Frame2_FRAME_FLAG_MASK_0);
}

/*******************************************************************************
* Function Name: l_flg_clr_Frame3
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_Frame3(void)
{
    LIN_1_statusFlagArray[LIN_1_Frame3_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_Frame3_FRAME_FLAG_MASK_0);
}

/*******************************************************************************
* Function Name: l_flg_clr_Frame4
********************************************************************************
*
* Summary:
*  Sets the current value of the flag specified by the name fff to zero.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_flg_clr_Frame4(void)
{
    LIN_1_statusFlagArray[LIN_1_Frame4_FRAME_FLAG_BYTE_OFFSET_0] &=
       ((l_u8) ~LIN_1_Frame4_FRAME_FLAG_MASK_0);
}






#if(1u == LIN_1_LIN_1_3)

    /*******************************************************************************
    * Function Name: l_ifc_disconnect
    ********************************************************************************
    *
    * Summary:
    *  Stops the component operation.
    *  This function is implementeed because it is mandatory for LIN v.1.3 API.
    *
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Zero     - If the initialization succeeds.
    *  Non-zero - If the initialization fails.
    *
    *******************************************************************************/
    l_bool l_ifc_disconnect_LIN_1(void)
    {
        l_u8 return_value = LIN_1_FALSE;
        LIN_1_Stop();

        return (return_value);
    }


    /*******************************************************************************
    * Function Name: l_ifc_connect
    ********************************************************************************
    *
    * Summary:
    *  Inilizes LIN interface.
    *  This function is implementeed because it is mandatory for LIN v.1.3 API.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Zero     - If the initialization succeeds.
    *  Non-zero - If the initialization fails.
    *
    *******************************************************************************/
    l_bool l_ifc_connect_LIN_1(void)
    {
        return (l_ifc_init_LIN_1());
    }

#endif /* (1u == LIN_1_LIN_1_3) */


/*******************************************************************************
* Function Name: l_ifc_init_LIN_1
********************************************************************************
*
* Summary:
*  The function initializes the LIN Slave component instance that is specified
*  by name iii. It sets up internal functions such as the baud rate and
*  starts up digital blocks used by the LIN Slave component. This is
*  the first call that must be performed before using any other interface-
*  related LIN Slave API functions.
*
* Parameters:
*  None
*
* Return:
*  0 is returned if operation succeeds and 1 if an invalid operation
*  parameter is passed to the function.
*
*******************************************************************************/
l_bool l_ifc_init_LIN_1(void)
{
    l_u8 interruptState;
    l_u8 i;
    l_bool returnValue = LIN_1_FALSE;

    interruptState = CyEnterCriticalSection();

    /* Set internal clock divider with effect on end of cycle.
    *  The divider's value is decremented to be compatible with the
    *  SetFractionalDividerRegister() parameters */

    #if(1u == LIN_1_AUTO_BAUD_RATE_SYNC)

        LIN_1_IntClk_Stop();

        LIN_1_IntClk_SetFractionalDividerRegister((LIN_1_initialClockDivider - 1u), 0u);

        LIN_1_IntClk_Start();

    #endif  /* (1u == LIN_1_AUTO_BAUD_RATE_SYNC) */

    /* Initialize UART */
    LIN_1_SCB_Init();

    /* Set Break Detection Threshold in counts */

    LIN_1_SCB_UART_RX_CTRL_REG |= (uint32)((uint32)(LIN_1_BREAK_THRESHOLD_VALUE - 1u) <<
                   LIN_1_SCB_UART_RX_CTRL_BREAK_WIDTH_POS);

    /* Place SCB UART in LIN compatibility mode  */

    LIN_1_SCB_UART_RX_CTRL_REG |= (uint32)(LIN_1_SCB_UART_RX_CTRL_LIN_MODE);

    /* Configure BREAK DETECT, BAUD DETECT and FIFO NOT EMPTY as interrupt sources */
    LIN_1_SCB_INTR_RX_MASK_REG |=  (LIN_1_SCB_INTR_RX_BREAK_DETECT |
            LIN_1_SCB_INTR_RX_BAUD_DETECT | LIN_1_SCB_INTR_RX_NOT_EMPTY);

    /* Copy PIDs from NVRAM to VRAM */

    for(i = 0u; i < LIN_1_NUM_FRAMES; i++)
    {
        LIN_1_volatileConfig[i] = LIN_1_LinSlaveConfig.pidTable[i];
    }

    #if(1u == LIN_1_TL_ENABLED)

        LIN_1_nad = LIN_1_LinSlaveConfig.initialNad;

    #endif /* (1u == LIN_1_TL_ENABLED) */

    /*  Clear interface status */
    LIN_1_ifcStatus &= (l_u16) ~LIN_1_IFC_STS_MASK;

    #if(1u == LIN_1_INACTIVITY_ENABLED)

        /* Clear period timer counter */
        LIN_1_periodCounter = 0x00u;

    #endif /* (1u == LIN_1_INACTIVITY_ENABLED) */


    /* Clear ioctl status register */
    LIN_1_ioctlStatus = 0x00u;

    LIN_1_SCB_Enable();

    CyExitCriticalSection(interruptState);

    /* Bus inactivity block configuration */
    #if(1u == LIN_1_INACTIVITY_ENABLED)

        /* Start SysTick timer with default period = 1mS */
        if(CYRET_SUCCESS != LIN_1_TimerEnableInt())
        {
            /* Report error as all SysTick callback slots are busy */
            returnValue = LIN_1_TRUE;
        }
    #endif  /* (1u == LIN_1_INACTIVITY_ENABLED) */

    /* Clear pending SCB interrupts */
    LIN_1_ISR_ClearPending();

    /* Enable SCB interrupt */
    LIN_1_ISR_StartEx(&LIN_1_UART_ISR);

    return (returnValue);
}


/*******************************************************************************
* Function Name: l_ifc_wake_up
********************************************************************************
*
* Summary:
*  This function transmits one wakeup signal. The wakeup signal is transmitted
*  directly when this function is called. When you call this API function, the
*  application is blocked until the wakeup signal is transmitted to the LIN bus.
*  The CyDelayUs() function is used as the timing source. The delay is
*  calculated based on the clock configuration entered in PSoC Creator.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void l_ifc_wake_up_LIN_1(void)
{
    #if(0u == LIN_1_LIN_1_3)

        /* For LIN 2.x: Force TXD low by transmission of N dominant bits
        *  (N depends on baudrate)
        *  To calculate how much of bit-interval takes 300uS wake-up pulse:
        *  1) calculate bit period: 1000000(uS) divide by BAUD_RATE(bps)
        *  2) divide WAKE_UP_SIGNAL_LENGTH by result of 1)
        *  3) add 1 to compensate rounding error
        *  4) shift 0xFF constant by number of bit obtained in 3)
        *  form dominant level pulse with duration proportional to baud rate
        */

        LIN_1_SCB_SpiUartWriteTxData((uint8) (0xffu << ((LIN_1_WAKE_UP_SIGNAL_LENGTH /
            (1000000u /LIN_1_BAUD_RATE)) + 1u)));

        /* Wait until symbol transmission ends, Tdelay > 11mS ,
        *  11 bit-times at min. baudrate 1000bps,for  8N1.5 symbol*/
        CyDelay(LIN_1_WAKEUP_TX_DELAY);

    #else

        /* For LIN 1.3: Simply send 0x80 as LIN 1.3 specifies */
        LIN_1_SCB_SpiUartWriteTxData((uint8) 0x80);

    #endif /* (0u == LIN_1_LIN_1_3) */
}


/*******************************************************************************
* Function Name: l_ifc_ioctl
********************************************************************************
*
* Summary:
*  This function controls functionality that is not covered by the other API
*  calls. It is used for protocol specific parameters or hardware specific
*  functionality.
*
* Parameters:
*  op - The operation that should be applied.
*  pv - The pointer to the optional parameter.
*
* Return:
*  There is no error code value returned for the operation selected. This means
*  that you must ensure that the values passed into the function are correct.
*
*  L_IOCTL_READ_STATUS operation:
*  The first bit in this byte is the flag that indicates that there has been no
*  signalling on the bus for a certain elapsed time (available when the
*  Bus Inactivity Timeout Detection option is enabled). If the elapsed time
*  passes a certain threshold, this flag is set. Calling this API clears
*  all the status bits after they are returned. The second bit is the flag that
*  indicates that a Targeted Reset service request (0xB5) is received
*  (when J2602-1 Compliance is enabled).
*
*  Symbolic Name : LIN_1_IOCTL_STS_BUS_INACTIVITY
*  Value         : 0x0001u
*  Description   : No signal has been detected on the bus for a certain elapsed
*                  time
*
*  Symbolic Name : LIN_1_IOCTL_STS_TARGET_RESET
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
*  Returns the current number of sync field timer counts.
*
*  L_IOCTL_SET_SERIAL_NUMBER operation:
*  CYRET_SUCCESS is returned if operation succeeds and CYRET_BAD_PARAM if
*  an invalid operation parameter is passed to the function.
*
*******************************************************************************/
l_u16 l_ifc_ioctl_LIN_1(l_ioctl_op op, void* pv)
{
    l_u16 returnValue = (l_u16) CYRET_SUCCESS;
    l_u8 interruptState;

    switch (op)
    {

        /***********************************************************************
        *                           Read Status
        ***********************************************************************/
        case L_IOCTL_READ_STATUS:

            /* Return status */
            returnValue = LIN_1_ioctlStatus;

            /* Clear status */
            #if(0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB5_SEL))
                #if(1u == LIN_1_SAE_J2602)

                    LIN_1_ioctlStatus &= (l_u16)~LIN_1_IOCTL_STS_TARGET_RESET;

                #endif /* (1u == LIN_1_SAE_J2602) */
            #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB5_SEL) */

        break;


        /***********************************************************************
        *                           Set Baud Rate
        ***********************************************************************/
        case L_IOCTL_SET_BAUD_RATE:

            interruptState = CyEnterCriticalSection();

            /* Set new internal clock divider with effect on end of cycle.
            *  The divider's value is decremented to be compatible with the
            *  SetFractionalDividerRegister() parameters */

            LIN_1_IntClk_Stop();

            LIN_1_IntClk_SetFractionalDividerRegister(((*((l_u16*)pv)) - 1u), 0u);

            LIN_1_initialClockDivider =*(((l_u16*)pv));

            LIN_1_IntClk_Start();

            CyExitCriticalSection(interruptState);

        break;


        /***********************************************************************
        *                   Prepare for the low power modes
        ***********************************************************************/

        case L_IOCTL_SLEEP:

            LIN_1_SCB_Stop();

            /* Clear SCB FIFOs*/
            LIN_1_SCB_CLEAR_TX_FIFO;
            LIN_1_SCB_CLEAR_RX_FIFO;

            /* Disable interrupt from this instance */
            LIN_1_ISR_Disable();

             /* Clear pending interrupts */
            LIN_1_ISR_ClearPending();
            LIN_1_SCB_ClearRxInterruptSource(LIN_1_SCB_INTR_RX_ALL);

        break;


        /***********************************************************************
        *             Restore after wakeup from low power modes
        ***********************************************************************/

        case L_IOCTL_WAKEUP:

            (void)l_ifc_init_LIN_1();

        break;

        #if(1u == LIN_1_AUTO_BAUD_RATE_SYNC)

            case L_IOCTL_SYNC_COUNTS:

                /* Returns current number of sync field timer counts */
                returnValue = (l_u16) LIN_1_syncCounts;

            break;

        #endif  /* (1u == LIN_1_AUTO_BAUD_RATE_SYNC) */


        #if(1u == LIN_1_TL_ENABLED)
            
            #if(1u == LIN_1_CS_ENABLED)

                case L_IOCTL_SET_SERIAL_NUMBER:
                    LIN_1_serialNumber = (l_u8*) pv;
                break;

            #endif /* (1u == LIN_1_CS_ENABLED) */
            
        #endif /* (1u == LIN_1_TL_ENABLED) */

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
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void l_ifc_rx_LIN_1(void)
{
    l_u8 i;
    l_u8 interruptState;

    #if(1u == LIN_1_AUTO_BAUD_RATE_SYNC)
        uint32 tmp;
        static uint32 LIN_1_correctedClockDivider;
    #endif  /* (1u == LIN_1_AUTO_BAUD_RATE_SYNC) */

    static l_u16 LIN_1_interimChecksum;   /* Holds interim checksum value     */
    static l_u8 LIN_1_framePid;           /* PID of current frame         */
    static l_u8 LIN_1_frameSize;          /* Size of frame being processed    */
    static l_u8 LIN_1_bytesTransferred;   /* Number of transferred bytes       */
    static l_u8 LIN_1_tmpData;            /* Used to store transmitted byte   */
    static l_u8 LIN_1_pidIndex;           /* Index in pidInfoTable            */
    static l_u8 LIN_1_tmpRxFrameData[8u]; /* RXed data before checksum checked */
    static volatile l_u8* LIN_1_frameData = NULL;  /* Pointer to frame data, points to byte to be sent.*/
    static l_u8 LIN_1_frameErrorAfterPID;

    /*******************************************************************************
    * The parity lookup table.  Given a six bit identifier as an index, the indexed
    * value provides the correct value with the parity bit set.
    *******************************************************************************/
    static const l_u8 CYCODE LIN_1_parityTable[] =
    {
        0x80u, 0xC1u, 0x42u, 0x03u, 0xC4u, 0x85u, 0x06u, 0x47u, 0x08u, 0x49u, 0xCAu,
        0x8Bu, 0x4Cu, 0x0Du, 0x8Eu, 0xCFu, 0x50u, 0x11u, 0x92u, 0xD3u, 0x14u, 0x55u,
        0xD6u, 0x97u, 0xD8u, 0x99u, 0x1Au, 0x5Bu, 0x9Cu, 0xDDu, 0x5Eu, 0x1Fu, 0x20u,
        0x61u, 0xE2u, 0xA3u, 0x64u, 0x25u, 0xA6u, 0xE7u, 0xA8u, 0xE9u, 0x6Au, 0x2Bu,
        0xECu, 0xADu, 0x2Eu, 0x6Fu, 0xF0u, 0xB1u, 0x32u, 0x73u, 0xB4u, 0xF5u, 0x76u,
        0x37u, 0x78u, 0x39u, 0xBAu, 0xFBu, 0x3Cu, 0x7Du, 0xFEu, 0xBFu
    };

    /* PID information table */
    static LIN_1_PID_INFO_TABLE const CYCODE LIN_1_pidInfoTable[LIN_1_NUM_FRAMES] =
{
    {(LIN_1_FRAME_DIR_SUBSCRIBE | LIN_1_FRAME_DATA_SIZE_8 | LIN_1_FRAME_TYPE_UNCOND),
     LIN_1_Buffer_Frame1}, 

    {(LIN_1_FRAME_DIR_SUBSCRIBE | LIN_1_FRAME_DATA_SIZE_8 | LIN_1_FRAME_TYPE_UNCOND),
     LIN_1_Buffer_Frame2}, 

    {(LIN_1_FRAME_DIR_PUBLISH | LIN_1_FRAME_DATA_SIZE_8 | LIN_1_FRAME_TYPE_UNCOND),
     LIN_1_Buffer_Frame3}, 

    {(LIN_1_FRAME_DIR_PUBLISH | LIN_1_FRAME_DATA_SIZE_8 | LIN_1_FRAME_TYPE_UNCOND),
     LIN_1_Buffer_Frame4}
};


   /* Set bus activity interface status bit */
    LIN_1_ifcStatus |= LIN_1_IFC_STS_BUS_ACTIVITY;

    #if(1u == LIN_1_INACTIVITY_ENABLED)

        /* Clear period timer counter */
        LIN_1_periodCounter = 0x00u;

        /* Clear bus inactivity ioctl status bit */
        LIN_1_ioctlStatus &=(l_u16)(~(l_u16)LIN_1_IOCTL_STS_BUS_INACTIVITY);

    #endif /* (1u == LIN_1_INACTIVITY_ENABLED) */

    /* Check for RX UART framing error and overflow  */

    if(0u != (LIN_1_SCB_INTR_RX_REG & (LIN_1_SCB_INTR_RX_FRAME_ERROR |
            LIN_1_SCB_INTR_RX_OVERFLOW)))
    {

        LIN_1_SCB_ClearRxInterruptSource(
        LIN_1_SCB_INTR_RX_ERR | LIN_1_SCB_INTR_RX_OVERFLOW );

         /* Set framing error  */

        LIN_1_fsmFlags |= LIN_1_FSM_FRAMING_ERROR_FLAG;
    }

    /***************************************************************************
    *                       Break Field Detected
    ***************************************************************************/

    if(0u !=(LIN_1_SCB_INTR_RX_REG & LIN_1_SCB_INTR_RX_BREAK_DETECT))
    {
        /* Reset pending flag*/
        LIN_1_SCB_INTR_RX_REG = LIN_1_SCB_INTR_RX_BREAK_DETECT;

            /* Framing error or data transfer is aborted */
            if(0u  != (LIN_1_FSM_DATA_RECEIVE & LIN_1_fsmFlags))
            {
                /* Set response error */
                LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;

                #if(1u == LIN_1_SAE_J2602)

                    /* Set ERR2 bit */
                    LIN_1_j2602Status |= LIN_1_J2602_STS_DATA_ERR;

                #endif  /* (1u == LIN_1_SAE_J2602) */

               
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_Frame4[LIN_1_Response_Error_Frame4_SIG_BYTE_OFFSET] |= 
                            LIN_1_Response_Error_Frame4_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);


            }   /* No response error, continue */

        /* Auto Baud Rate Sync Enabled */
        #if(1u == LIN_1_AUTO_BAUD_RATE_SYNC)

            LIN_1_IntClk_Stop();

            /* Restore initial clock divider */
            LIN_1_IntClk_SetFractionalDividerRegister((uint16) 
                (LIN_1_initialClockDivider - 1u), 0u);

            LIN_1_IntClk_Start();

        #endif  /* (1u == LIN_1_AUTO_BAUD_RATE_SYNC) */

        LIN_1_fsmFlags &= ((l_u8) ~(LIN_1_FSM_DATA_RECEIVE |
                                LIN_1_FSM_FRAMING_ERROR_FLAG));

        /* Set UART ISR FSM to IDLE state */
        LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_0_IDLE;

        /* Reset UART state machine */
        LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);

        return;
    }

    /***********************************************************************
    *                       Sync Field Complete                            *
    ***********************************************************************/
    if(0u != (LIN_1_SCB_INTR_RX_REG & LIN_1_SCB_INTR_RX_BAUD_DETECT))
    {
        /* Clear SYNC source first */
        LIN_1_SCB_INTR_RX_REG = LIN_1_SCB_INTR_RX_BAUD_DETECT;

        /* Auto Baud Rate Sync Enabled */
        #if(1u == LIN_1_AUTO_BAUD_RATE_SYNC)

            LIN_1_syncCounts=(l_u8)(LIN_1_SCB_UART_RX_STATUS_REG &
                LIN_1_SCB_UART_RX_STATUS_BR_COUNTER_MASK);

            /* Check if correction is necessary */
            if(LIN_1_syncCounts != LIN_1_EXPECTED_TIME_COUNTS)
            {
                /* Clock max deviation < 14%.
                +0.5% from Master
                So if deviation > (14.5*128/100)=18,56 (0x13) - drop the packet */

                if((LIN_1_ABS((int32)LIN_1_syncCounts -
                    (int32)LIN_1_EXPECTED_TIME_COUNTS)) <
                        (int32)LIN_1_TIME_COUNTS_MAX_DEVIATION)
                {
                    /* As BR_COUNTER uses 0x80 as the base to measure the frequency,
                    *  divide its constant by 0x80 and round off the result checking the last thrown bit */

                    tmp =((uint32)LIN_1_initialClockDivider * (uint32)LIN_1_syncCounts);

                    LIN_1_correctedClockDivider = (uint32) (tmp >> LIN_1_BR_BASE_SHIFT);

                    LIN_1_correctedClockDivider += (l_u16)
                        (( 0u != (tmp & (l_u8)(1u <<(LIN_1_BR_BASE_SHIFT-1u)))) ? 1u : 0u);

                    LIN_1_IntClk_Stop();

                    LIN_1_IntClk_SetFractionalDividerRegister((uint16)
                        (LIN_1_correctedClockDivider - 1u), 0u);

                    LIN_1_IntClk_Start();
                }
                else /* clocks differ > +/-14% so reset state machine */
                {
                    /* Reset UART state machine */
                    LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                }
            }

        #endif /* (1u == LIN_1_AUTO_BAUD_RATE_SYNC) */

         /* This sets the UART ISR FSM to state 1 (PID awaiting) */
        LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_1_PID;

         /* Set UART enabled flag */
        LIN_1_fsmFlags |= LIN_1_FSM_UART_ENABLE_FLAG;

         /* Clear any pending UART interrupt and RX FIFO */
        LIN_1_SCB_SpiUartClearRxBuffer();

        /* Clear all UART pending interrupts */
        LIN_1_SCB_INTR_RX_REG = LIN_1_SCB_INTR_RX_ALL;
    }

    /* Return if there is nothing to process in RX/TX state machine */
    if(0u == (LIN_1_SCB_INTR_RX_REG & LIN_1_SCB_INTR_RX_NOT_EMPTY))
    {
        return;
    }

    switch(LIN_1_uartFsmState)
    {
        /***********************************************************************
        *                       IDLE state
        * State description:
        *  - Receives a sporadic byte not predicted by the BREAK/SYNC sequence,
        *    so does not set Response Error
        ***********************************************************************/

        case LIN_1_UART_ISR_STATE_0_IDLE:

            LIN_1_SCB_SpiUartClearRxBuffer();

            LIN_1_SCB_INTR_RX_REG = (uint32)LIN_1_SCB_INTR_RX_NOT_EMPTY;

            /* Reset UART state machine */
            LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);

            break;

        /***********************************************************************
        *                       PID Field Byte Receive
        * State description:
        *  - Receives protected identifier (PID)
        *  - Checks PID parity
        *  - Set flags
        *  - Determine next state (RX or TX)
        ***********************************************************************/
        case LIN_1_UART_ISR_STATE_1_PID:

            if(0u != (LIN_1_FSM_FRAMING_ERROR_FLAG & LIN_1_fsmFlags))
            {
                /* Reset UART State Machine */
                LIN_1_EndFrame(LIN_1_HANDLING_DONT_SAVE_PID);
                break;
            }
            /* Save PID */
            LIN_1_framePid = (l_u8)LIN_1_SCB_SpiUartReadRxData();

            /* Reset number of transferred bytes */
            LIN_1_bytesTransferred = 0u;

            /* Clear checksum byte */
            LIN_1_interimChecksum = 0u;

            /* Clear Frame Error after PID flag -such a condition must be processed separately
            *as it can be Frame Error caused by the Break field of the next frame */
            LIN_1_frameErrorAfterPID = 0u;

            /* Verify PID parity */
            if(LIN_1_parityTable[LIN_1_framePid & LIN_1_PID_PARITY_MASK] !=
                LIN_1_framePid)
            {
                /* Set J2602 response error  */

                #if(1u == LIN_1_SAE_J2602)

                    /* Set ERR2, ERR1 and ERR0 bits */
                    LIN_1_j2602Status |= LIN_1_J2602_STS_PARITY_ERR;

                    LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;

                    
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_Frame4[LIN_1_Response_Error_Frame4_SIG_BYTE_OFFSET] |= 
                            LIN_1_Response_Error_Frame4_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);


                #endif /* (1u == LIN_1_SAE_J2602) */

                /* Reset UART State Machine */
                LIN_1_EndFrame(LIN_1_HANDLING_DONT_SAVE_PID);
            }
            else    /* PID parity is correct */
            {
                /* Check if MRF or SRF frame */
                if((LIN_1_FRAME_PID_MRF == LIN_1_framePid) ||
                (LIN_1_FRAME_PID_SRF == LIN_1_framePid))
                {
                    /*  Transport Layer section. MRF and SRF detection */
                    #if(1u == LIN_1_TL_ENABLED)

                        #if(1u == LIN_1_SAE_J2602)

                            if(LIN_1_FRAME_PID_MRF_J2602 == LIN_1_framePid)
                            {
                                /* Process Master Request */

                                /* Nothing to be done for Transport Layer */

                                /* Set response error */
                                LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;

                                /* Set ERR2, ERR1 and ERR0 bits */
                                LIN_1_j2602Status |= LIN_1_J2602_STS_PARITY_ERR;

                                
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_Frame4[LIN_1_Response_Error_Frame4_SIG_BYTE_OFFSET] |= 
                            LIN_1_Response_Error_Frame4_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);


                                /* Check for framing error */
                                if(0u == (LIN_1_fsmFlags & LIN_1_FSM_FRAMING_ERROR_FLAG))
                                {
                                    /* Save last processed on bus PID to status variable */
                                    LIN_1_ifcStatus &= ((l_u16) ~LIN_1_IFC_STS_PID_MASK);
                                    LIN_1_ifcStatus |= ((l_u16) (((l_u16)LIN_1_framePid) << 8u));
                                }

                                /* Reset UART State Machine */
                                LIN_1_EndFrame(LIN_1_HANDLING_DONT_SAVE_PID);
                            }

                        #endif  /* (1u == LIN_1_SAE_J2602) */

                        if(LIN_1_FRAME_PID_MRF == LIN_1_framePid)
                        {
                            /* Indicate that slave is required to receive data */
                            LIN_1_tlFlags |= LIN_1_TL_RX_DIRECTION;

                            /*******************************************************
                            *               Cooked & RAW API
                            *******************************************************/

                            /* If MRF PID is detected, pass pointer to start of
                            * Frame Buffer and size of data to RX state to handle data receiving
                            */

                            /* Frame equals 8 bytes */
                            LIN_1_frameSize = LIN_1_FRAME_DATA_SIZE_8;

                            /* Set frame data pointer to start of frame buffer */
                            LIN_1_frameData = LIN_1_mrfBuffer;

                            /* Switch to subscribe data state */
                            LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_3_RX;
                        }

                        if(LIN_1_FRAME_PID_SRF == LIN_1_framePid)
                        {
                            if(0u != (LIN_1_status & LIN_1_STATUS_SRVC_RSP_RDY))
                            {
                                /* Frame is always equal to 8 bytes for TL */
                                LIN_1_frameSize = LIN_1_FRAME_DATA_SIZE_8;

                                /* Set frame data pointer to frame buffer start */
                                LIN_1_frameData = LIN_1_srfBuffer;

                                /* Send first byte to LIN master */
                                LIN_1_tmpData = *LIN_1_frameData;
                                LIN_1_frameData++;
                                LIN_1_SCB_SpiUartWriteTxData((uint32)LIN_1_tmpData);
                                LIN_1_bytesTransferred = 1u;

                                /* Switch to publish data state. */
                                LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_2_TX;

                               /* One or more data bytes have been received */
                               LIN_1_fsmFlags |= LIN_1_FSM_DATA_RECEIVE;

                                /* Indicate to transport layer that slave transmits the frame */
                                LIN_1_tlFlags |= LIN_1_TL_TX_DIRECTION;
                            }
                            else
                            {
                                /* This part of code will handle LIN Transport Layer.
                                 * Sends one frame of a message if applications prepered
                                 * the message in TX message buffer or TX queue and LIN Slave node
                                 * state machine has appropriate state (is ready for physical response
                                 * transmission
                                 */
                                if((0u != (LIN_1_tlFlags & LIN_1_TL_TX_REQUESTED)) &&
                                (LIN_1_nodeState == LIN_1_TX_PHY_RESPONSE))
                                {
                                    #if(0u == LIN_1_TL_API_FORMAT)

                                        if(0u != LIN_1_txBufDepth)
                                        {
                                            /* Fill SRF from the frame buffer */
                                            for(i = 0u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                                            {
                                                LIN_1_srfBuffer[i] =
                                                    LIN_1_rawTxQueue[LIN_1_txRdIndex];

                                                /* Update the index to TX queue */
                                                LIN_1_txRdIndex++;
                                            }

                                            /* Read index should point to the next byte in MRF */
                                            if(LIN_1_TL_TX_QUEUE_LEN == LIN_1_txRdIndex)
                                            {
                                                LIN_1_txRdIndex = 0u;
                                            }

                                            /* 8 bytes were read from the SRF so decrement the buffer depth */
                                            LIN_1_txBufDepth--;

                                            /* Update status properly */
                                            if(0u == LIN_1_txBufDepth)
                                            {
                                                LIN_1_txTlStatus = LD_QUEUE_EMPTY;
                                            }
                                            else
                                            {
                                                LIN_1_txTlStatus = LD_QUEUE_AVAILABLE;
                                            }

                                            if(LIN_1_PDU_PCI_TYPE_UNKNOWN == LIN_1_txPrevPci)
                                            {
                                                if(LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] <=
                                                   LIN_1_PDU_SF_DATA_LEN)
                                                {
                                                    LIN_1_txMessageLength |=
                                                    ((l_u16) LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX]);
                                                }
                                                else
                                                {
                                                    /* Get FF/SF from queue */
                                                    LIN_1_txMessageLength = (l_u16)((((l_u16)
                                                        LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX]) &
                                                        ((l_u16) ((l_u8) ~LIN_1_PDU_PCI_TYPE_MASK))) << 8u);

                                                    LIN_1_txMessageLength |=
                                                    ((l_u16) LIN_1_srfBuffer[LIN_1_PDU_LEN_IDX]);
                                                }
                                            }
                                        }

                                    #endif /* (0u == LIN_1_TL_API_FORMAT) */

                                        /* This part of code handles PDU packing for Cooked API */
                                        /* Check length it shows if message is already sent */

                                    if(LIN_1_txMessageLength == 0u)
                                    {
                                        LIN_1_tlFlags &=(l_u8)~LIN_1_TL_TX_REQUESTED;
                                        /* Reset UART State Machine */
                                        LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                                    }
                                        /* Process message sending */
                                    else
                                    {
                                            /* Analyze length to find type of frame message to be sent */
                                        if(LIN_1_txMessageLength > LIN_1_FRAME_DATA_SIZE_6)
                                        {
                                            /* Process the FF Frame */
                                            if(LIN_1_txPrevPci == LIN_1_PDU_PCI_TYPE_UNKNOWN)
                                            {
                                                /* Fill Frame PCI field */
                                                /* Save the previous PCI */
                                                LIN_1_txPrevPci = LIN_1_PDU_PCI_TYPE_FF;

                                                LIN_1_tlFlags |= LIN_1_TL_N_AS_TIMEOUT_ON;
                                                LIN_1_tlTimeoutCnt = 0u;

                                                #if(1u == LIN_1_TL_API_FORMAT)

                                                    /* Fill Frame NAD field */
                                                    LIN_1_srfBuffer[0u] = LIN_1_nad;

                                                    LIN_1_srfBuffer[1u] = (LIN_1_PDU_PCI_TYPE_FF |
                                                        (HI8(LIN_1_txMessageLength)));

                                                    /* Fill Frame LEN field */
                                                    LIN_1_srfBuffer[2u] =
                                                        LO8(LIN_1_txMessageLength);

                                                    /* Fill Frame Data fields */
                                                    for(i = 3u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                                                    {
                                                        LIN_1_srfBuffer[i] =
                                                            LIN_1_txTlDataPointer[(i +
                                                                LIN_1_txTlDataCount) - 3u];
                                                    }

                                                    /* Update user buffer pointer */
                                                    LIN_1_txTlDataCount +=
                                                        LIN_1_FRAME_DATA_SIZE_5;

                                                    LIN_1_txMessageLength -=
                                                        LIN_1_FRAME_DATA_SIZE_5;

                                                #endif /* (1u == LIN_1_TL_API_FORMAT) */

                                            }

                                            /* Process the CF Frame */
                                            else
                                            {
                                                #if(1u == LIN_1_TL_API_FORMAT)

                                                     /* Fill Frame PCI field */
                                                    LIN_1_srfBuffer[1u] = (LIN_1_PDU_PCI_TYPE_CF |
                                                        LIN_1_txFrameCounter);

                                                    /* Fill Frame Data fields */
                                                    for(i = 2u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                                                    {
                                                        LIN_1_srfBuffer[i] =
                                                            LIN_1_txTlDataPointer[(i +
                                                                LIN_1_txTlDataCount) - 2u];
                                                    }

                                                    /* Update the user buffer pointer */
                                                    LIN_1_txTlDataCount +=
                                                            LIN_1_FRAME_DATA_SIZE_6;

                                                #endif /* (1u == LIN_1_TL_API_FORMAT) */

                                                /* Save the previous PCI */
                                                LIN_1_txPrevPci = LIN_1_PDU_PCI_TYPE_CF;

                                                /* Update length pointer properly */
                                                LIN_1_txMessageLength -=
                                                    LIN_1_FRAME_DATA_SIZE_6;

                                                LIN_1_tlFlags |= LIN_1_TL_N_AS_TIMEOUT_ON;
                                                LIN_1_tlTimeoutCnt = 0u;
                                            }
                                        }
                                        /* Process SF Frame or last CF Frame */
                                        else
                                        {
                                            /* Check if the Previous frame is in "Unknown" state, which indicates that
                                            * the current frame is SF, otherwise it is the last CF frame.
                                            * Fill the Frame PCI field properly.
                                            */

                                            LIN_1_tlFlags |= LIN_1_TL_N_AS_TIMEOUT_ON;
                                            LIN_1_tlTimeoutCnt = 0u;

                                            if(LIN_1_PDU_PCI_TYPE_UNKNOWN == LIN_1_txPrevPci)
                                            {
                                                #if(1u == LIN_1_TL_API_FORMAT)

                                                    /* Fill Frame NAD field */
                                                    LIN_1_srfBuffer[0u] = LIN_1_nad;

                                                    LIN_1_srfBuffer[1u] = (l_u8)
                                                        LIN_1_txMessageLength;

                                                #endif /* (1u == LIN_1_TL_API_FORMAT) */

                                                /* Save the previous PCI */
                                                LIN_1_txPrevPci = LIN_1_PDU_PCI_TYPE_SF;
                                            }
                                            else
                                            {
                                                #if(1u == LIN_1_TL_API_FORMAT)

                                                    /* Fill Frame NAD field */
                                                    LIN_1_srfBuffer[0u] = LIN_1_nad;

                                                    LIN_1_srfBuffer[1u] =
                                                        (LIN_1_PDU_PCI_TYPE_CF |
                                                            LIN_1_txFrameCounter);

                                                #endif /* (1u == LIN_1_TL_API_FORMAT) */

                                                    /* Save previous PCI */
                                                LIN_1_txPrevPci = LIN_1_PDU_PCI_TYPE_CF;
                                            }

                                            #if(1u == LIN_1_TL_API_FORMAT)

                                                /* Fill Frame Data fields */
                                                for(i = 2u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                                                {
                                                    if(LIN_1_txMessageLength >= ((l_u8) (i - 1u)))
                                                    {
                                                        LIN_1_srfBuffer[i] =
                                                            LIN_1_txTlDataPointer[(i +
                                                                LIN_1_txTlDataCount) - 2u];
                                                    }
                                                    else
                                                    {
                                                        /* Fill unused data bytes with FFs */
                                                        LIN_1_srfBuffer[i] = 0xFFu;
                                                    }
                                                }

                                                /* Update the user buffer pointer */
                                                LIN_1_txTlDataCount += LIN_1_FRAME_DATA_SIZE_6;

                                            #endif /* (1u == LIN_1_TL_API_FORMAT) */

                                            /* Update length pointer properly */
                                            if(LIN_1_txMessageLength > LIN_1_FRAME_DATA_SIZE_6)
                                            {
                                                LIN_1_txMessageLength -= LIN_1_FRAME_DATA_SIZE_6;
                                            }
                                            else
                                            {
                                                LIN_1_txMessageLength = 0u;
                                            }
                                        }

                                        /* Update the frame counter */
                                        if(LIN_1_txFrameCounter != 15u)
                                        {
                                            LIN_1_txFrameCounter++;
                                        }
                                        else
                                        {
                                            /* If frame counter is larger then 15 then reset it */
                                            LIN_1_txFrameCounter = 0u;
                                        }

                                        /* Frame equals 8 bytes */
                                        LIN_1_frameSize = LIN_1_FRAME_DATA_SIZE_8;

                                        /* Set frame data pointer to a start of a frame buffer */
                                        LIN_1_frameData = LIN_1_srfBuffer;

                                        /* Send first byte to the LIN master */
                                        LIN_1_tmpData = *LIN_1_frameData;
                                        LIN_1_frameData++;
                                        LIN_1_SCB_SpiUartWriteTxData((uint32)LIN_1_tmpData);
                                        LIN_1_bytesTransferred = 1u;

                                        /* Switch to the publish data state. */
                                        LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_2_TX;

                                        /* One or more data bytes have been received */
                                       LIN_1_fsmFlags |= LIN_1_FSM_DATA_RECEIVE;
                                        /* Indicate to transport layer that slave is transmitting the frame */
                                        LIN_1_tlFlags |= LIN_1_TL_TX_DIRECTION;
                                    }
                                }
                                else
                                {
                                    /* Reset UART State Machine */
                                    LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                                }
                            }
                        } /* if SRF */

                    #else

                        /* These are invalid PIDs when TL is disabled - reset UART state machine */

                        /* Set response error */

                        #if(1u == LIN_1_SAE_J2602)

                            /* Set ERR2, ERR1 and ERR0 bits */
                            LIN_1_j2602Status |= LIN_1_J2602_STS_PARITY_ERR;

                            LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;

                            
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_Frame4[LIN_1_Response_Error_Frame4_SIG_BYTE_OFFSET] |= 
                            LIN_1_Response_Error_Frame4_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);


                        #endif /* (1u == LIN_1_SAE_J2602) */

                        /* Reset UART State Machine */
                        LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);

                    #endif /* (1u == LIN_1_TL_ENABLED) */
                }
                else    /* Not MRF and SRF */
                {
                    /* Get PID index in LIN_1_pidInfoTable */
                    LIN_1_pidIndex = LIN_1_FindPidIndex(LIN_1_framePid);

                    if(LIN_1_INVALID_FRAME_PID != LIN_1_pidIndex)
                    {
                        /* Valid ID */
                        #if(0u == LIN_1_LIN_1_3)

                            /* Start enhanced checksum calculation  */
                            LIN_1_interimChecksum = LIN_1_framePid;

                        #endif /* (0u == LIN_1_LIN_1_3) */

                        /* Get size of frame */
                        LIN_1_frameSize = LIN_1_pidInfoTable[LIN_1_pidIndex].param &
                                                        LIN_1_FRAME_DATA_SIZE_MASK;

                        /* TX response (publish action) is requested by Master */
                        if(0u != (LIN_1_FRAME_DIR_PUBLISH &
                                 LIN_1_pidInfoTable[LIN_1_pidIndex].param))
                        {
                            /* SAE J2602 is disabled */
                            #if(0u == LIN_1_SAE_J2602)

                                /* This frame is event-triggered */
                                if(0u  != (LIN_1_FRAME_TYPE_EVENT &
                                           LIN_1_pidInfoTable[LIN_1_pidIndex].param))
                                {
                                    /* Check if we need to process event-triggered frame */
                                    if(0u == LIN_1_GetEtFlagValue(LIN_1_pidIndex))
                                    {
                                       /* Reset UART State Machine */
                                        LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                                       return;
                                    }
                                }

                            #endif /* (0u == LIN_1_SAE_J2602) */

                            /* Get pointer to frame data */
                            LIN_1_frameData =
                                LIN_1_pidInfoTable[LIN_1_pidIndex].dataPtr;

                            /* Send first byte to LIN master */
                            LIN_1_tmpData = *LIN_1_frameData;

                            /* SAE J2602 is disabled */
                            #if(0u == LIN_1_SAE_J2602)

                                /* This frame is event-triggered */
                                if(0u  != (LIN_1_FRAME_TYPE_EVENT &
                                           LIN_1_pidInfoTable[LIN_1_pidIndex].param))
                                {
                                /* Set firth byte equal to associated unconditional frame's PID */
                                    LIN_1_tmpData = LIN_1_parityTable[
                                        LIN_1_pidInfoTable[LIN_1_pidIndex].associatedPID];
                                }
                                else
                                {
                                    LIN_1_fsmFlags |= LIN_1_FSM_DATA_RECEIVE;
                                }
                            #endif /* (0u == LIN_1_SAE_J2602) */

                            LIN_1_frameData++;
                            LIN_1_SCB_SpiUartWriteTxData((uint32)LIN_1_tmpData);
                            LIN_1_bytesTransferred = 1u;

                            /* Switch to publish data state. */
                            LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_2_TX;

                            /* Set DATA RECEIVE flag */
                            LIN_1_fsmFlags |= LIN_1_FSM_DATA_RECEIVE;
                        }
                        else    /* RX response (subscribe action) is requested by Master */
                        {
                            LIN_1_bytesTransferred = 0u;

                            /* Get pointer to temp RX frame data buffer */
                            LIN_1_frameData = LIN_1_tmpRxFrameData;

                            /* Switch to subscribe data state. */
                            LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_3_RX;
                        }
                    }
                    else    /* Invalid ID */
                    {
                        /* Set response error */

                        #if(1u == LIN_1_SAE_J2602)

                            /* Set data error bit */
                            LIN_1_j2602Status |= LIN_1_J2602_STS_DATA_ERR;

                            LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;

                        #endif /* (1u == LIN_1_SAE_J2602) */

                        /* Reset UART State Machine */
                        LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
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
        case LIN_1_UART_ISR_STATE_2_TX:

            /* Set response active flag */
            LIN_1_status |= LIN_1_STATUS_RESPONSE_ACTIVE;

            /* Previously transmitted and read back bytes are not equal */
            if((LIN_1_tmpData != (l_u8)LIN_1_SCB_SpiUartReadRxData()) ||
               (0u != (LIN_1_FSM_FRAMING_ERROR_FLAG & LIN_1_fsmFlags)))
            {
                /* Mismatch Error */

                #if(1u == LIN_1_SAE_J2602)

                    /* Set ERR2 bit */
                    LIN_1_j2602Status |= LIN_1_J2602_STS_DATA_ERR;

                    /* Readback error - set response error flag */
                    LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;

                    
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_Frame4[LIN_1_Response_Error_Frame4_SIG_BYTE_OFFSET] |= 
                            LIN_1_Response_Error_Frame4_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);


                #else

                    /* Skip event-triggered frame */
                    if(0u  == (LIN_1_FRAME_TYPE_EVENT &
                               LIN_1_pidInfoTable[LIN_1_pidIndex].param))
                    {
                        /* Readback error - set response error flag */
                        LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;

                        
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_Frame4[LIN_1_Response_Error_Frame4_SIG_BYTE_OFFSET] |= 
                            LIN_1_Response_Error_Frame4_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

                    }

                #endif  /* (1u == LIN_1_SAE_J2602) */

                /* Check for framing error */
                if(0u == (LIN_1_fsmFlags & LIN_1_FSM_FRAMING_ERROR_FLAG))
                {
                    /* Save last processed on bus PID to status variable */
                    LIN_1_ifcStatus &= ((l_u16) ~LIN_1_IFC_STS_PID_MASK);
                    LIN_1_ifcStatus |= ((l_u16) (((l_u16)LIN_1_framePid) << 8u));
                }

                /* End frame with response error */
                LIN_1_EndFrame(LIN_1_HANDLING_DONT_SAVE_PID);
            }
            else    /* If readback is successful, continue transmitting */
            {
                /* Add transmitted byte to interim checksum */
                LIN_1_interimChecksum += LIN_1_tmpData;
                if(LIN_1_interimChecksum >= 256u)
                {
                    LIN_1_interimChecksum -= 255u;
                }

                /* Check to see if all data bytes have been sent */
                if(LIN_1_frameSize > LIN_1_bytesTransferred)
                {
                    /* Send out the next byte of the buffer */
                    LIN_1_tmpData = *LIN_1_frameData;
                    LIN_1_frameData++;
                    LIN_1_SCB_SpiUartWriteTxData((uint32)LIN_1_tmpData);
                    LIN_1_bytesTransferred++;
                }
                else    /* All data bytes are sent - compute and transmit checksum */
                {
                    /* Compute and send out checksum byte */
                    LIN_1_SCB_SpiUartWriteTxData((((l_u8)LIN_1_interimChecksum) ^ (uint32)0xFFu));

                    LIN_1_bytesTransferred = 0u;

                    /* Switch to checksum state */
                    LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_4_CHS;
                }
            }
        break;


        /***********************************************************************
        *                       RX response (Subscribe)
        * State description:
        *  - Receives data from LIN Master.
        *  - Received data saved to the temporary buffer.
        ***********************************************************************/
        case LIN_1_UART_ISR_STATE_3_RX:
            /* Set Response error only if error appears during receive */
            if(0u  != (LIN_1_FSM_FRAMING_ERROR_FLAG & LIN_1_fsmFlags))
            {
                /* This is a workaround to suppress Response Error
                 * when only Break header+PID are received          */
                if((0u == LIN_1_bytesTransferred) && (0u == LIN_1_frameErrorAfterPID))
                {
                    /* Read erroneous byte from fifo */
                    (void)LIN_1_SCB_SpiUartReadRxData();

                    /* Bypass first Framing error after PID */
                    LIN_1_frameErrorAfterPID = 1u;
                }
                else
                {
                    /* Set response error */
                    LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;

                    #if(1u == LIN_1_SAE_J2602)

                        /* Set framing error bits */
                       LIN_1_j2602Status |= LIN_1_J2602_STS_FRAMING_ERR;

                    #endif /* (1u == LIN_1_SAE_J2602) */

                   
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_Frame4[LIN_1_Response_Error_Frame4_SIG_BYTE_OFFSET] |= 
                            LIN_1_Response_Error_Frame4_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);


                   /* Clear FE  flag and DATA_RECEIVE flags anyway */
                   LIN_1_fsmFlags &= (l_u8)(~LIN_1_FSM_DATA_RECEIVE);

                    /* Finish frame processing */
                    LIN_1_EndFrame(LIN_1_HANDLING_DONT_SAVE_PID);
                }
               /* Clear FE  flag and DATA_RECEIVE flags anyway */
               LIN_1_fsmFlags &= (l_u8)(~LIN_1_FSM_FRAMING_ERROR_FLAG);
            }
            else
            {
                /* Save received byte */
                LIN_1_tmpData = (l_u8)LIN_1_SCB_SpiUartReadRxData();

                *LIN_1_frameData = LIN_1_tmpData;
                LIN_1_frameData++;
                LIN_1_bytesTransferred++;

                /* Set response active flag */
                LIN_1_status |= LIN_1_STATUS_RESPONSE_ACTIVE;

                /* One or more data bytes have been received */
                LIN_1_fsmFlags |= LIN_1_FSM_DATA_RECEIVE;

                /* Add received byte to interim checksum */
                LIN_1_interimChecksum += LIN_1_tmpData;
                if(LIN_1_interimChecksum >= 256u)
                {
                    LIN_1_interimChecksum -= 255u;
                }

                /* Check to see if data section has not finished */
                if(LIN_1_frameSize > LIN_1_bytesTransferred)
                {
                    /* There is data to be received */
                }
                else
                {
                    /* There is no data to be received */

                    LIN_1_bytesTransferred = 0u;

                    /* Switch to checksum state */
                    LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_4_CHS;
                }
            }
        break;


        /***********************************************************************
        *                              Checksum
        ***********************************************************************/
        case LIN_1_UART_ISR_STATE_4_CHS:

            /* Previously transmitted and read back bytes are not equal */

            if(((((l_u8) LIN_1_interimChecksum) ^ 0xFFu) != 
                  (l_u8) LIN_1_SCB_SpiUartReadRxData()) ||
                  (0u != (LIN_1_fsmFlags & LIN_1_FSM_FRAMING_ERROR_FLAG)))
            {
                /* Mismatch or Checksum Error */

                /* Set response error */
                LIN_1_ifcStatus |= LIN_1_IFC_STS_ERROR_IN_RESPONSE;

                #if(1u == LIN_1_SAE_J2602)

                    /* Set ERR2 and ERR0 bits */
                    LIN_1_j2602Status |= LIN_1_J2602_STS_CHECKSUM_ERR;

                #endif /* (1u == LIN_1_SAE_J2602) */

                
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_Frame4[LIN_1_Response_Error_Frame4_SIG_BYTE_OFFSET] |= 
                            LIN_1_Response_Error_Frame4_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);


                /* Check for framing error */
                if(0u == (LIN_1_fsmFlags & LIN_1_FSM_FRAMING_ERROR_FLAG))
                {
                    /* Save the last processed on bus PID to the status variable */
                    LIN_1_ifcStatus &= ((l_u16) ~LIN_1_IFC_STS_PID_MASK);
                    LIN_1_ifcStatus |= ((l_u16) (((l_u16)LIN_1_framePid) << 8u));
                }

                /* Reset UART state machine with checksum or mismatch error */
                LIN_1_EndFrame(LIN_1_HANDLING_DONT_SAVE_PID);
            }
            else
            {
                /*  Clear all error bits in interface status */
                #if(1u == LIN_1_SAE_J2602)

                    LIN_1_j2602Status &= LIN_1_J2602_CLEAR_ERR_BITS_MASK;

                #endif  /* (1u == LIN_1_SAE_J2602) */

                /* Clear framing error and data receive flags */
                LIN_1_fsmFlags &=
                   ((l_u8) ~(LIN_1_FSM_FRAMING_ERROR_FLAG | LIN_1_FSM_DATA_RECEIVE));

                /* Set successful transfer interface flag */
                LIN_1_ifcStatus |= LIN_1_IFC_STS_SUCCESSFUL_TRANSFER;

                /* Save the last processed on bus PID to the status variable */
                LIN_1_ifcStatus &= ((l_u16) ~LIN_1_IFC_STS_PID_MASK);
                LIN_1_ifcStatus |= ((l_u16) (((l_u16)LIN_1_framePid) << 8u));

                /* Set overrun interface flag */
                if(0u != (LIN_1_FSM_OVERRUN & LIN_1_fsmFlags))
                {
                    LIN_1_ifcStatus |= LIN_1_IFC_STS_OVERRUN;
                }

                /* Set Overrun flag */
                LIN_1_fsmFlags |= LIN_1_FSM_OVERRUN;

                /* Clear response error signal if frame contains RESPONSE ERROR signal */
                #if(1u == LIN_1_RESPONSE_ERROR_SIGNAL)

                    if(LIN_1_RESPONSE_ERROR_FRAME_INDEX == LIN_1_pidIndex)
                    {
                        
    /* Save current global interrupt enable and disable it */
    interruptState = CyEnterCriticalSection();

        LIN_1_Buffer_Frame4[LIN_1_Response_Error_Frame4_SIG_BYTE_OFFSET] &= 
                            (l_u8) ~LIN_1_Response_Error_Frame4_SIG_MASK_0;

    /* Restore global interrupt enable state */
    CyExitCriticalSection(interruptState);

                    }

                #endif /* (1u == LIN_1_RESPONSE_ERROR_SIGNAL) */

                #if(1u == LIN_1_TL_ENABLED)

                    if(!((LIN_1_FRAME_PID_MRF == LIN_1_framePid) ||
                         (LIN_1_FRAME_PID_SRF == LIN_1_framePid)))
                    {
                        /* SAE J2602 is disabled */
                        #if(0u == LIN_1_SAE_J2602)

                            /* This frame is event-triggered */
                            if(0u != (LIN_1_FRAME_TYPE_EVENT &
                                      LIN_1_pidInfoTable[LIN_1_pidIndex].param))
                            {
                                /* Clear event-triggered flags */
                                LIN_1_ClearEtFlagValue(LIN_1_pidIndex);

                                /* Reset UART State Machine */
                               LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                            }

                        #endif /* (0u == LIN_1_SAE_J2602) */

                        /* Set associated with current frame flags */
                        LIN_1_SetAssociatedFlags(LIN_1_pidIndex);
                    }

                #else

                    /* SAE J2602 is disabled */
                    #if(0u == LIN_1_SAE_J2602)

                        /* This frame is event-triggered */
                        if(0u != (LIN_1_FRAME_TYPE_EVENT &
                                  LIN_1_pidInfoTable[LIN_1_pidIndex].param))
                        {
                            /* Clear event-triggered flags */
                            LIN_1_ClearEtFlagValue(LIN_1_pidIndex);

                            /* Reset UART State Machine */
                           LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                        }

                    #endif /* (0u == LIN_1_SAE_J2602) */

                    /* Set flags associated with current frame */
                    LIN_1_SetAssociatedFlags(LIN_1_pidIndex);

                #endif  /* (1u == LIN_1_TL_ENABLED) */

                #if(1u == LIN_1_TL_ENABLED)

                    /* Check to see if received data is a "master request frame" */
                    if(LIN_1_FRAME_PID_MRF == LIN_1_framePid)
                    {
                        if(LIN_1_NAD_GO_TO_SLEEP == LIN_1_mrfBuffer[LIN_1_PDU_NAD_IDX])
                        {
                            LIN_1_ifcStatus |= LIN_1_IFC_STS_GO_TO_SLEEP;
                        }
                        else if((LIN_1_nad == LIN_1_mrfBuffer[LIN_1_PDU_NAD_IDX]) ||
                                (LIN_1_NAD_BROADCAST == 
                                    LIN_1_mrfBuffer[LIN_1_PDU_NAD_IDX]))
                        {
                            LIN_1_CheckNodeState(LIN_1_MRF_OWN_NAD);
                        }
                        else if( LIN_1_NAD_FUNCTIONAL == 
                            LIN_1_mrfBuffer[LIN_1_PDU_NAD_IDX])
                        {
                            LIN_1_CheckNodeState(LIN_1_MRF_FUNC_NAD);
                        }
                        else
                        {
                            LIN_1_CheckNodeState(LIN_1_MRF_ALIEN_NAD);
                        }

                        /* Clear TL RX direction flag */
                        LIN_1_tlFlags &= ((l_u8) ~LIN_1_TL_RX_DIRECTION);

                        /* Reset UART state machine */
                        LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                    }
                    else if(LIN_1_FRAME_PID_SRF == LIN_1_framePid)
                    {
                        LIN_1_CheckNodeState(LIN_1_SRF);

                        /* Reset UART state machine */
                        LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                    }
                    else
                    {
                        /* RX response (subscribe action) is requested by Master */
                        if(0u == (LIN_1_FRAME_DIR_PUBLISH &
                                 LIN_1_pidInfoTable[LIN_1_pidIndex].param))
                        {
                            interruptState = CyEnterCriticalSection();

                            /* Copy received data from temporary buffer to frame buffer */
                            for(i = 0u; i < LIN_1_frameSize; i++)
                            {
                                *(LIN_1_pidInfoTable[LIN_1_pidIndex].dataPtr + i) =
                                    LIN_1_tmpRxFrameData[i];
                            }

                            CyExitCriticalSection(interruptState);
                        }

                        /* Reset UART state machine */
                        LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                    }
                #else
                    /* RX response (subscribe action) is requested by Master */
                    if(0u == (LIN_1_FRAME_DIR_PUBLISH &
                              LIN_1_pidInfoTable[LIN_1_pidIndex].param))
                    {
                        interruptState = CyEnterCriticalSection();

                        /* Copy received data from temporary buffer to frame buffer */
                        for(i = 0u; i < LIN_1_frameSize; i++)
                        {
                            *(LIN_1_pidInfoTable[LIN_1_pidIndex].dataPtr + i) =
                                LIN_1_tmpRxFrameData[i];
                        }
                        CyExitCriticalSection(interruptState);
                    }

                    /* Reset UART state machine */
                    LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
                #endif  /* (1u == LIN_1_TL_ENABLED) */
            }
        break;
        default:
            /* Reset UART state machine */
            LIN_1_EndFrame(LIN_1_HANDLING_RESET_FSM_ERR);
        break;
    }

    LIN_1_SCB_INTR_RX_REG = LIN_1_SCB_INTR_RX_NOT_EMPTY;


    /***************************************************************************
    *  Place your UART ISR code here
    ***************************************************************************/
    /* `#START LIN_1_UART_ISR_CODE` */

    /* `#END` */

    #ifdef LIN_1_L_IFC_RX_CALLBACK
        l_ifc_rx_LIN_1_Callback();
    #endif /* LIN_1_L_IFC_RX_CALLBACK */
}


/*******************************************************************************
* Function Name: l_ifc_tx
********************************************************************************
*
* Summary:
*  The LIN Slave component calls this API routine automatically.
*  Therefore, this API routine must not be called by the application code.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void l_ifc_tx_LIN_1(void)
{
    l_ifc_rx_LIN_1();
}


/*******************************************************************************
* Function Name: l_ifc_aux
********************************************************************************
*
* Summary:
*  The LIN Slave component calls this API routine automatically.
*  Therefore, this API routine must not be called by the application code.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void l_ifc_aux_LIN_1(void)
{
    /***************************************************************************
    *                       Bus Inactivity Interrupt Detected
    ***************************************************************************/
    #if(1u == LIN_1_INACTIVITY_ENABLED)

            #if(1u == LIN_1_TL_ENABLED)              /* TL enabled */

                /* If timeout is enabled, proceed timeout manage */
                if(0u !=(LIN_1_tlFlags & LIN_1_TL_N_AS_TIMEOUT_ON))
                {
                    /* Increment timeout */
                    LIN_1_tlTimeoutCnt++;

                    if((LIN_1_TL_N_AS_TIMEOUT_VALUE) <= LIN_1_tlTimeoutCnt)
                    {
                        LIN_1_CheckNodeState(LIN_1_TX_TIMEOUT);
                    }
                }
                else if(0u !=(LIN_1_tlFlags & LIN_1_TL_N_CR_TIMEOUT_ON))
                {
                    /* Increment timeout */
                    LIN_1_tlTimeoutCnt++;

                    if((LIN_1_TL_N_CR_TIMEOUT_VALUE) <= LIN_1_tlTimeoutCnt)
                    {
                        LIN_1_CheckNodeState(LIN_1_RX_TIMEOUT);
                    }
                }
                else
                {
                    /* Reset timeout counter */
                    LIN_1_tlTimeoutCnt = 0u;
                }

            #endif /* (1u == LIN_1_TL_ENABLED) */

            if((LIN_1_INACTIVITY_THRESHOLD_IN_1_MS) <= LIN_1_periodCounter)
            {
                /* Inactivity threshold achieved */

                /* Set bus inactivity ioctl status bit */
                LIN_1_ioctlStatus |= LIN_1_IOCTL_STS_BUS_INACTIVITY;
            }
            else
            {
                LIN_1_periodCounter++;
            }

    #endif  /* (1u == LIN_1_INACTIVITY_ENABLED) */


    /***************************************************************************
    *  Place your BASE ISR code here
    ***************************************************************************/
    /* `#START LIN_1_BASE_ISR_CODE` */

    /* `#END` */

    #ifdef LIN_1_L_IFC_AUX_CALLBACK
        
        l_ifc_aux_LIN_1_Callback();
        
    #endif /* LIN_1_L_IFC_AUX_CALLBACK */
}


/*******************************************************************************
* Function Name: l_ifc_read_status_LIN_1
********************************************************************************
*
* Summary:
*  This function is defined by the LIN specification. This returns the status of
*  the specified LIN interface and clears all status bits for that
*  interface. See Section 7.2.5.8 of the LIN 2.1 specification.
*
* Parameters:
*  None
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
*******************************************************************************/
l_u16 l_ifc_read_status_LIN_1(void)
{
    l_u16 returnValue;
    l_u8 interruptState;

    interruptState = CyEnterCriticalSection();

    /* Copy global status variable to local temp variable */
    returnValue = LIN_1_ifcStatus;

    /* Clear status variable */
    LIN_1_ifcStatus &= (l_u16) ~LIN_1_IFC_STS_MASK;

    CyExitCriticalSection(interruptState);
    
    /* Return status in temp variable */
    return (returnValue);
}


/*******************************************************************************
* Function Name: LIN_1_FindPidIndex
********************************************************************************
*
* Summary:
*  Returns the PID index in LIN_1_volatileConfig.
*
* Parameters:
*  l_u8 pid - PID of the frame whose index is required.
*
* Return:
*  The index if PID in LIN_1_volatileConfig.
*  0xFFu - If PID is not found.
*
*******************************************************************************/
static l_u8 LIN_1_FindPidIndex(l_u8 pid)
{
    l_u8 i;
    l_u8 returnValue = LIN_1_INVALID_FRAME_PID;

    for(i = 0u; i < LIN_1_NUM_FRAMES; i++)
    {
        if(pid == LIN_1_volatileConfig[i])
        {
            returnValue = i;

            /* Break the loop */
            break;
        }
    }

    return (returnValue);
}


/*******************************************************************************
* Function Name: LIN_1_EndFrame
********************************************************************************
*
* Summary:
*  Finishes the frame transmission.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void LIN_1_EndFrame(l_u8 status)
{
    l_u8 interruptState;

    /* Clear data received flag */
    LIN_1_fsmFlags &= ((l_u8) ~LIN_1_FSM_DATA_RECEIVE);

    #if(1u == LIN_1_TL_ENABLED)

        /* Clear TL TX direction flag */
        LIN_1_tlFlags &= ((l_u8) ~LIN_1_TL_TX_DIRECTION);

    #endif /* (1u == LIN_1_TL_ENABLED) */

    if(status == LIN_1_HANDLING_DONT_SAVE_PID)
    {
        /* Clear data received flag */
        LIN_1_fsmFlags &= ((l_u8) ~LIN_1_FSM_DATA_RECEIVE);

        if(0u != (LIN_1_fsmFlags & LIN_1_FSM_OVERRUN))
        {
            /* Set overrun */
            LIN_1_ifcStatus |= LIN_1_IFC_STS_OVERRUN;
        }
    }
    if(status ==LIN_1_HANDLING_SKIP_OVERRUN)
    {
        /* Set Overrun flag */
        LIN_1_fsmFlags |= LIN_1_FSM_OVERRUN;

        #if(1u == LIN_1_TL_ENABLED)

            if(0u != (LIN_1_tlFlags & LIN_1_TL_TX_DIRECTION))
            {
                /* Transport Layer Functions: Cooked Transport Layer API */
                #if(1u == LIN_1_TL_API_FORMAT)

                    /* Set TL TX error status */
                    LIN_1_txTlStatus = LD_FAILED;

                #else   /* Transport Layer Functions: Raw Transport Layer API */

                    /* Set TL TX error status */
                    LIN_1_txTlStatus = LD_TRANSMIT_ERROR;

                #endif /* (1u == LIN_1_TL_API_FORMAT) */

                /* Clear TL flags register */
                LIN_1_tlFlags = 0u;
            }

            if(0u != (LIN_1_tlFlags & LIN_1_TL_RX_DIRECTION))
            {
                /* Transport Layer Functions: Cooked Transport Layer API */
                #if(1u == LIN_1_TL_API_FORMAT)

                    /* Set TL RX error status */
                    LIN_1_rxTlStatus = LD_FAILED;

                #else   /* Transport Layer Functions: Raw Transport Layer API */

                    /* Set TL RX error status */
                    LIN_1_rxTlStatus = LD_RECEIVE_ERROR;

                #endif /* (1u == LIN_1_TL_API_FORMAT) */

                /* Clear TL flags register */
                LIN_1_tlFlags = 0u;
            }

        #endif /* (1u == LIN_1_TL_ENABLED) */
    }
    else if((status == LIN_1_HANDLING_RESET_FSM_ERR) || (status == LIN_1_HANDLING_DONT_SAVE_PID))
    {
        /* Clear UART enable flag */
        LIN_1_fsmFlags &= ((l_u8) ~(LIN_1_FSM_UART_ENABLE_FLAG |
                        LIN_1_FSM_FRAMING_ERROR_FLAG));

        interruptState = CyEnterCriticalSection();

        /* Clear RX and TX FIFOs after Frame or Overrun Error */

        LIN_1_SCB_SpiUartClearRxBuffer();
        LIN_1_SCB_SpiUartClearTxBuffer();

        LIN_1_SCB_INTR_RX_REG = LIN_1_SCB_INTR_RX_NOT_EMPTY;

        /* Set IDLE FSM State */
        LIN_1_uartFsmState = LIN_1_UART_ISR_STATE_0_IDLE;

        CyExitCriticalSection(interruptState);
    }
    else
    {
        /* Do nothing */
    }
}


/*******************************************************************************
* Function Name: LIN_1_SetAssociatedFlags
********************************************************************************
*
* Summary:
*  This function sets appropriate flags. Used by ISR.
*
* Parameters:
*  The PID index in LIN_1_LinSlaveConfig.
*
* Return:
*  None
*
*******************************************************************************/
static void LIN_1_SetAssociatedFlags(l_u8 pIndex)
{
    switch(pIndex)
    {
        case 0u:
            /* Mark frame "Frame1" (Type:Unconditional,Associated With:None) and associated signals processed. */ 
            LIN_1_statusFlagArray[LIN_1_ISR_AUX_SET_FLAGS_Frame1_FLAG_BYTE_OFFSET_0] |=
                LIN_1_ISR_AUX_SET_FLAGS_Frame1_FLAG_MASK_0;
        break;

        case 1u:
            /* Mark frame "Frame2" (Type:Unconditional,Associated With:None) and associated signals processed. */ 
            LIN_1_statusFlagArray[LIN_1_ISR_AUX_SET_FLAGS_Frame2_FLAG_BYTE_OFFSET_0] |=
                LIN_1_ISR_AUX_SET_FLAGS_Frame2_FLAG_MASK_0;
        break;

        case 2u:
            /* Mark frame "Frame3" (Type:Unconditional,Associated With:None) and associated signals processed. */ 
            LIN_1_statusFlagArray[LIN_1_ISR_AUX_SET_FLAGS_Frame3_FLAG_BYTE_OFFSET_0] |=
                LIN_1_ISR_AUX_SET_FLAGS_Frame3_FLAG_MASK_0;
        break;

        case 3u:
            /* Mark frame "Frame4" (Type:Unconditional,Associated With:None) and associated signals processed. */ 
            LIN_1_statusFlagArray[LIN_1_ISR_AUX_SET_FLAGS_Frame4_FLAG_BYTE_OFFSET_1] |=
                LIN_1_ISR_AUX_SET_FLAGS_Frame4_FLAG_MASK_1;
        break;


        default:
        break;    
    }
}


/* SAE J2602 is disabled */
#if(0u == LIN_1_SAE_J2602)


    /*******************************************************************************
    * Function Name: LIN_1_GetEtFlagValue
    ********************************************************************************
    *
    * Summary:
    *  This function gets the value of an appropriate event-triggered frame flag.
    *
    * Parameters:
    *  The PID index in LIN_1_LinSlaveConfig.
    *
    * Return:
    *  Current flag value.
    *
    *******************************************************************************/
    static l_bool LIN_1_GetEtFlagValue(l_u8 pidIndex)
{
    l_bool returnValue;

    switch(pidIndex)
    {
        default:
             returnValue = LIN_1_FALSE;
        break;    
    }

    return (returnValue);
}



    /*******************************************************************************
    * Function Name: LIN_1_ClearEtFlagValue
    ********************************************************************************
    *
    * Summary:
    *  This function clears the value of an appropriate event-triggered frame flag.
    *
    * Parameters:
    *  The PID index in LIN_1_LinSlaveConfig.
    *
    * Return:
    *  None
    *
    *  No
    *
    *******************************************************************************/
    static void LIN_1_ClearEtFlagValue(l_u8 pidIndex)
{
    switch(pidIndex)
    {
        default:
        break;    
    }
}



#endif /* (0u == LIN_1_SAE_J2602) */

#if(1u == LIN_1_TL_ENABLED)

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
    * Return:
    *  None
    *
    *******************************************************************************/
    void ld_init_LIN_1(void)
    {
        l_u8 interruptState;

        #if(1u == LIN_1_TL_API_FORMAT)

            while(0u != (LIN_1_tlFlags & LIN_1_TL_TX_DIRECTION))
            {
                /* Wait until current message is processed */
            }

            /* Save interrupt state and disable interrupts */
            interruptState = CyEnterCriticalSection();

            /* Initialize Tx and Rx status variables correctly */
            LIN_1_txTlStatus = LD_COMPLETED;
            LIN_1_rxTlStatus = LD_COMPLETED;

            LIN_1_tlLengthPointer = NULL;

            /* Reset frame counters */
            LIN_1_rxMessageLength = 0u;
            LIN_1_txMessageLength = 0u;

            /* Reset frame counters */
            LIN_1_txFrameCounter = 0u;
            LIN_1_rxFrameCounter = 0u;

            LIN_1_rxTlDataPointer = NULL;
            LIN_1_rxTlInitDataPointer = NULL;

            /* Previous PCI requires to be unknown after initialization */
            LIN_1_rxPrevPci = LIN_1_PDU_PCI_TYPE_UNKNOWN;
            LIN_1_txPrevPci = LIN_1_PDU_PCI_TYPE_UNKNOWN;

        #else

            /* Save interrupt state and disable interrupts */
            interruptState = CyEnterCriticalSection();

            /* Reset buffers depth to 0, it indicates that buffers are empty */
            LIN_1_txBufDepth = 0u;
            LIN_1_rxBufDepth = 0u;

            /* Raw API buffers initialization */

            LIN_1_txWrIndex = 0u;
            LIN_1_txRdIndex = 0u;

            LIN_1_rxWrIndex = 0u;
            LIN_1_rxRdIndex = 0u;

            LIN_1_txTlStatus = LD_QUEUE_EMPTY;
            LIN_1_rxTlStatus = LD_NO_DATA;

        #endif /* (1u == LIN_1_TL_API_FORMAT) */

        /* Set initial NAD as current active NAD before initializing TL */
        LIN_1_nad = LIN_1_LinSlaveConfig.initialNad;

        LIN_1_tlFlags = 0u;

        /* Change node state to next state */
       LIN_1_nodeState = LIN_1_IDLE;

        /* Enable interrupts */
        CyExitCriticalSection(interruptState);
    }

    
    /*******************************************************************************
    * Function Name: ld_read_configuration
    ********************************************************************************
    *
    * Summary:
    *  This function reads the NAD and PID values from the volatile memory.
    *  This function reads the current configuration data, and
    *  saves this data into the non-volatile (flash) memory. The application should
    *  save the configuration data to the flash when the "Save Configuration" bit
    *  is set in the LIN status register (returned by 
    *  l_ifc_read_status_LIN_1).
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
    l_u8 ld_read_configuration_LIN_1(l_u8* pData, l_u8* const length)
    {
        l_u8 i;
        l_u8 result = LIN_1_LD_READ_OK;

        if(*length < (LIN_1_NUM_FRAMES + 1u))
        {
            /* Return with no action when requested length is smaller
            *  than configuration data length.
            */
            result = LIN_1_LD_LENGTH_TOO_SHORT;
        }
        else
        {
            /* Copy over configured NAD */
            pData[0u] = LIN_1_nad;

            /* Copy data from PID array to data array */
            for(i = 0u; i < LIN_1_NUM_FRAMES; i++)
            {
                pData[i + 1u] = LIN_1_volatileConfig[i];
            }

            /* Set length parameter to actual length of configuration data */
            *length = LIN_1_NUM_FRAMES + 1u;
        }

        /* Return status */
        return (result);
    }

    /*******************************************************************************
    * Function Name: ld_set_configuration
    ********************************************************************************
    *
    * Summary:
    *  This call does not transport anything to the bus.?
    *
    *  The function configures NAD and PIDs accordingly to the
    *  configuration given by data. The intended usage is to restore the saved
    *  configuration or set initial configuration (e.g. coded by I/O pins).
    *
    *  The function is called after calling l_ifc_init.
    *
    *  The caller shall set the size of the data area before calling the function.
    *
    *  The data contains NAD and PIDs each occupying one byte.?
    *  The data structure is: NAD and all PIDs for the frames. ?
    *  The PIDs order is the same as the frame list in LDF,?
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
    *  LIN_1_volatileConfig - New frame PIDs are copied.
    *
    *******************************************************************************/
    l_u8 ld_set_configuration_LIN_1(const l_u8* const pData, l_u16 length)
    {
        l_u8 i;
        l_u8 result = LIN_1_LD_SET_OK;

        if(length != (LIN_1_NUM_FRAMES + 1u))
        {
            /* Return error if length isn't correct */
            result = LIN_1_LD_LENGTH_NOT_CORRECT;
        }
        else
        {
            /* Copy NAD to volatile memory */
            LIN_1_nad = pData[0u];

            /* Data read back */
            if(LIN_1_nad != pData[0u])
            {
                /* Indicate data error if NAD is not set correctly */
                result = LIN_1_LD_DATA_ERROR;
            }

            /* Copy Frame PIDs to volatile memory */
            for(i = 0u; i < LIN_1_NUM_FRAMES; i++)
            {
                LIN_1_volatileConfig[i] = pData[i + 1u];

                /* Data read back */
                if(LIN_1_volatileConfig[i] != pData[i + 1u])
                {
                    /* Indicate data error if NAD is not set correctly */
                    result = LIN_1_LD_DATA_ERROR;
                }
            }
        }

        /* Return success code if copy is completed */
        return(result);
    }

    #if(1u == LIN_1_CS_ENABLED)

        #if((0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB0_SEL)) || \
            (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL)))

            /*******************************************************************************
            * Function Name: LIN_1_LinProductId
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
            *  1 - If the LIN product IDs match.?
            *
            *******************************************************************************/
            static l_bool LIN_1_LinProductId(volatile const l_u8 frameData[])
            {
                l_u8 i = 1u;

                if((frameData[0u] != LO8(LIN_1_slaveId.supplierId)) &&
                    (frameData[0u] != LO8(LIN_1_CS_SUPPLIER_ID_WILDCARD)))
                {
                    i = 0u;        /* Zero out 'i' if data isn't for this slave */
                }

                if((frameData[1u] != HI8(LIN_1_slaveId.supplierId)) &&
                    (frameData[1u] != HI8(LIN_1_CS_SUPPLIER_ID_WILDCARD)))
                {
                    i = 0u;        /* Zero out 'i' if data isn't for this slave */
                }

                if((frameData[2u] != LO8(LIN_1_slaveId.functionId)) &&
                    (frameData[2u] != LO8(LIN_1_CS_FUNCTION_ID_WILDCARD)))
                {
                    i = 0u;        /* Zero out 'i' if data isn't for this slave */
                }

                if((frameData[3u] != HI8(LIN_1_slaveId.functionId)) &&
                    (frameData[3u] != HI8(LIN_1_CS_FUNCTION_ID_WILDCARD)))
                {
                    i = 0u;        /* Zero out 'i' if data isn't for this slave */
                }

                return(i);
            }

        #endif /*((0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB0_SEL)) ||
               * (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL)))
               */

        #if(0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB1_SEL))

            #if(1u == LIN_1_LIN_2_0)

                /*******************************************************************************
                * Function Name: LIN_1_MessageId
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
                static l_u8 LIN_1_MessageId(volatile const l_u8* frameData)
                {
                    l_u8 i = 0u;
                    l_u8 result = LD_INVALID_MESSAGE_INDEX;

                    while((i < LIN_1_NUM_FRAMES) && (result == LD_INVALID_MESSAGE_INDEX))
                    {
                        /* If LSB of message ID from table is equal to that received,
                        * Compare MSB and in case of success set result to message index.
                        */
                        if(frameData[0u] == LO8(messageIdTable[i]))
                        {
                            if(frameData[1u] == HI8(messageIdTable[i]))
                            {
                                result = i;
                            }
                        }

                        i++;
                    }

                    return(result);
                }

            #endif /* (1u == LIN_1_LIN_2_0) */

        #endif /*(0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB1_SEL)) */

        #if((0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL)) || \
            (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL)))

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
            l_u8 ld_read_by_id_callout_LIN_1(l_u8 id, l_u8* frameData)
            {
                l_u8 result = LD_NEGATIVE_RESPONSE;

                /* To remove unreferenced local variable warning */
                id = id;
                frameData = frameData;

                /* User code required to handle user defined identification (Optional) */
                /* `#START LD_READ_BY_ID_CALLOUT_USER_SECTION` */

                /* `#END` */

                #ifdef LIN_1_LD_READ_BY_ID_CALLOUT_CALLBACK
                    
                    ld_read_by_id_callout_LIN_1_Callback();
                    
                #endif /* LIN_1_LD_READ_BY_ID_CALLOUT_CALLBACK */

                return(result);
            }

        #endif /* ((0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL)) ||
               * (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL)))
               */

    #endif /* (1u == LIN_1_CS_ENABLED) */

    /*******************************************************************************
    * Function Name: CheckNodeState
    ********************************************************************************
    *
    * Summary:
    *  This function implements LIN Slave Node state machine as defined in LIN2.2.a,
    *  item 5.5 SLAVE NODE TRANSMISSION HANDLER.
    *  Its states are:
    *       IDLE - initial state;
    *       RX_PHY_REQUEST - processing of incoming segmented message;
    *       TX_PHY_RESPONSE -processing of outcoming RESPONSE.
    *
    * Parameters:
    *  stimulus - The event passed from LIN Protocol Layer:
    *     MRF_ALIEN_NAD  - frame with NAD not equal to this node NAD arrives;
    *     MRF_OWN_NAD    - frame with NAD equal to this node NAD arrives;
    *     MRF_FUNC_NAD   - frame with functional NAD equal arrives;
    *     SRF            - SRF header arrives;
    *     RX_TIMEOUT     - N_CR timeout was expired;
    *     TX_TIMEOUT     - N_AS timeout was expired;
    * Return:
    *  None
    *
    *******************************************************************************/
    static void LIN_1_CheckNodeState(l_u8 stimulus)
    {
        l_u8 return_value;

        switch(LIN_1_nodeState)
        {
            case LIN_1_IDLE:

                switch(stimulus)
                {
                    case LIN_1_MRF_ALIEN_NAD:
                    case LIN_1_SRF:
                    case LIN_1_RX_TIMEOUT:
                    case LIN_1_TX_TIMEOUT:

                        LIN_1_tlFlags &= (l_u8) (~(LIN_1_TL_N_CR_TIMEOUT_ON |
                                                    LIN_1_TL_N_AS_TIMEOUT_ON ));

                        break;

                    case LIN_1_MRF_FUNC_NAD:

                        (void)LIN_1_ProcessMrf(LIN_1_mrfBuffer);

                        LIN_1_ClrTxBuffer(stimulus);

                        break;
                    case LIN_1_MRF_OWN_NAD:

                         /* Receive incoming TL frame (FF or SF) */
                        LIN_1_ClrTxBuffer(stimulus);

                        return_value = LIN_1_ProcessMrf(LIN_1_mrfBuffer);

                        if(LIN_1_RESPONSE_REQUIRED == return_value)
                        {
                            /* Change node state to TX state */
                            LIN_1_nodeState = LIN_1_TX_PHY_RESPONSE;
                        }
                        else if(LIN_1_RECEIVE_CONTINUES == return_value)
                        {
                            /* Change node state to RX state */
                            LIN_1_nodeState = LIN_1_RX_PHY_REQUEST;
                        }
                        else
                        {
                            /* do_nothing; */
                        }

                        break;

                    default:
                        /* do_nothing; */
                        break;
                }
                break;

            case LIN_1_RX_PHY_REQUEST:

                switch(stimulus)
                {
                    case LIN_1_MRF_ALIEN_NAD:
                    case LIN_1_SRF:
                    case LIN_1_RX_TIMEOUT:

                    case LIN_1_TX_TIMEOUT:

                        LIN_1_ClrTxBuffer(stimulus);

                        LIN_1_ClrRxBuffer(stimulus);

                        LIN_1_nodeState = LIN_1_IDLE;

                    break;

                    case LIN_1_MRF_FUNC_NAD:
                         /* Miss functional request */

                    break;

                    case LIN_1_MRF_OWN_NAD:

                         /* Receive incoming TL frame (CF) */
                        return_value = LIN_1_ProcessMrf(LIN_1_mrfBuffer);

                        if(LIN_1_RESPONSE_REQUIRED == return_value)
                        {
                            /* Change node state to TX state */
                            LIN_1_nodeState = LIN_1_TX_PHY_RESPONSE;
                        }
                        else if(LIN_1_RECEIVE_CONTINUES == return_value)
                        {
                            /* Change node state to RX state */
                            LIN_1_nodeState = LIN_1_RX_PHY_REQUEST;
                        }
                        else
                        {
                            LIN_1_ClrTxBuffer(stimulus);

                            LIN_1_ClrRxBuffer(stimulus);

                            LIN_1_nodeState = LIN_1_IDLE;
                            /* do_nothing; */
                        }

                        break;
                   default:
                        /* do_nothing; */
                   break;
                }
                break;

            case LIN_1_TX_PHY_RESPONSE:

                switch(stimulus)
                {
                    case LIN_1_MRF_ALIEN_NAD:
                    case LIN_1_RX_TIMEOUT:
                    case LIN_1_TX_TIMEOUT:

                    LIN_1_ClrTxBuffer(stimulus);

                    LIN_1_nodeState = LIN_1_IDLE;
                    break;

                case LIN_1_MRF_FUNC_NAD:
                        /* do nothing , miss Functional Request here */
                    break;
                case LIN_1_SRF:

                        /* Transmit RESPONSE message */
                        return_value = LIN_1_TransmitTLFrame(stimulus);

                        if(LIN_1_TRANSMISSION_CONTINUES != return_value)
                        {
                            LIN_1_ClrTxBuffer(stimulus);

                            /* Change node state to idle state */
                            LIN_1_nodeState = LIN_1_IDLE;
                        }

                    break;

                case LIN_1_MRF_OWN_NAD:

                        /* MRF arrives during the RESPONSE transmission , drop
                        transmitted message and receive new diagnostic message */

                        LIN_1_ClrTxBuffer(stimulus);

                        return_value = LIN_1_ProcessMrf(LIN_1_mrfBuffer);

                        if(LIN_1_RESPONSE_REQUIRED == return_value)
                        {
                            /* Change node state to TX state */
                            LIN_1_nodeState = LIN_1_TX_PHY_RESPONSE;
                        }
                        else if(LIN_1_RECEIVE_CONTINUES == return_value)
                        {
                            /* Change node state to RX state */
                            LIN_1_nodeState = LIN_1_RX_PHY_REQUEST;
                        }
                        else
                        {
                            LIN_1_ClrRxBuffer(stimulus);

                            LIN_1_ClrTxBuffer(stimulus);

                            LIN_1_nodeState = LIN_1_IDLE;
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


    /*******************************************************************************
    * Function Name: LIN_1_TransmitTLFrame
    ********************************************************************************
    *
    * Summary:
    *  Transmits frame of segmented message.
    *
    * Parameters:
    *  stimulus - The event passed from LIN Protocol Layer:
    *     MRF_ALIEN_NAD  - frame with NAD not equal to this node NAD arrives;
    *     MRF_OWN_NAD    - frame with NAD equal to this node NAD arrives;
    *     MRF_FUNC_NAD   - frame with functional NAD equal arrives;
    *     SRF            - SRF header arrives;
    *     RX_TIMEOUT     - N_CR timeout was expired;
    *     TX_TIMEOUT     - N_AS timeout was expired;
    * Return:
    *  NO_RESPONSE_REQUIRED      - The message transmission is finished.
    *  TRANSMISSION_CONTINUES    - There is more segments to be transmitted.
    *
    *******************************************************************************/
    static l_u8 LIN_1_TransmitTLFrame(l_u8 stimulus)
    {
        l_u8 return_value = LIN_1_NO_RESPONSE_REQUIRED;

        /* To avoid 'unused variable' warning */
         stimulus = stimulus;

        /* Clear CR Timeout after answering on SRF */
        LIN_1_tlFlags &= ((l_u8) ~LIN_1_TL_N_CR_TIMEOUT_ON);

        if(0u == (LIN_1_status & LIN_1_STATUS_SRVC_RSP_RDY))
        {
            if((0u != LIN_1_txMessageLength) &&
               (0u != (LIN_1_tlFlags & LIN_1_TL_TX_REQUESTED)))
            {
                return_value = LIN_1_TRANSMISSION_CONTINUES;
            }
        }

        return(return_value);
    }


    /*******************************************************************************
    * Function Name: LIN_1_ClrRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  This function iinitializes receive part of Transport Layer variables .
    *
    * Parameters:
    *  stimulus - The event passed from LIN Protocol Layer:
    *     MRF_ALIEN_NAD  - frame with NAD not equal to this node NAD arrives;
    *     MRF_OWN_NAD    - frame with NAD equal to this node NAD arrives;
    *     MRF_FUNC_NAD   - frame with functional NAD equal arrives;
    *     SRF            - SRF header arrives;
    *     RX_TIMEOUT     - N_Cr timeout was expired;
    *     TX_TIMEOUT     - N_As timeout was expired;
    * Return:
    *  None
    *
    *******************************************************************************/
    static void LIN_1_ClrRxBuffer(l_u8 stimulus)
    {
        /* Previous PCI requires to be unknown after the initialization */
        LIN_1_rxPrevPci = LIN_1_PDU_PCI_TYPE_UNKNOWN;
        /* Reset the frame counters */
        LIN_1_rxMessageLength = 0u;
        
        #if(1u == LIN_1_TL_API_FORMAT)
            if (LIN_1_tlLengthPointer != NULL)
            {
                /* Clear length of erroneous frame */
                *LIN_1_tlLengthPointer = 0u;
            }
        #endif /* (1u == LIN_1_TL_API_FORMAT) */
        
        /* Reset the frame counters */
        LIN_1_rxFrameCounter = 0u;

        #if(1u == LIN_1_TL_API_FORMAT)

            if(stimulus == LIN_1_RX_TIMEOUT)
            {
                /* Set error status as the timeout occurred */
                LIN_1_rxTlStatus = LD_N_CR_TIMEOUT;
            }
            else if(stimulus == LIN_1_MRF_ALIEN_NAD)
            {
                LIN_1_rxTlStatus = LD_FAILED;
            }  
            else
            {
                if((LIN_1_rxTlStatus != LD_FAILED)   &&
                   (LIN_1_rxTlStatus != LD_WRONG_SN) &&
                   (LIN_1_rxTlStatus != LD_N_CR_TIMEOUT))
                {
                    /* Initialize Rx status variable properly */
                    LIN_1_rxTlStatus = LD_COMPLETED;
                }
            }

            LIN_1_rxTlDataPointer = NULL;
            LIN_1_rxTlInitDataPointer = NULL;

        #else

            /* To avoid 'unused variable' warning */
            stimulus = stimulus;

            /* Reset buffers depth to 0, it will indicate the buffers are empty */
            LIN_1_rxBufDepth = 0u;

            /* Raw API buffers initialization */
            LIN_1_rxWrIndex = 0u;
            LIN_1_rxRdIndex = 0u;

            LIN_1_rxTlStatus = LD_NO_DATA;

        #endif /* (1u == LIN_1_TL_API_FORMAT) */

        /* Clear Service Response ready status bit */
        LIN_1_status &= ((l_u8) ~LIN_1_STATUS_SRVC_RSP_RDY);

        LIN_1_tlFlags &= (l_u8) (~(LIN_1_TL_RX_REQUESTED |
            LIN_1_TL_N_CR_TIMEOUT_ON ));

        LIN_1_tlTimeoutCnt = 0u;
    }


    /*******************************************************************************
    * Function Name: LIN_1_ClrTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  This function iinitializes transmit part of Transport Layer variables.
    *
    * Parameters:
    *  stimulus - The event passed from LIN Protocol Layer:
    *     MRF_ALIEN_NAD  - frame with NAD not equal to this node NAD arrives;
    *     MRF_OWN_NAD    - frame with NAD equal to this node NAD arrives;
    *     MRF_FUNC_NAD   - frame with functional NAD equal arrives;
    *     SRF            - SRF header arrives;
    *     RX_TIMEOUT     - N_Cr timeout was expired;
    *     TX_TIMEOUT     - N_As timeout was expired;
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    static void LIN_1_ClrTxBuffer(l_u8 stimulus)
    {
        /* Reset the frame counters */
        LIN_1_txMessageLength = 0u;
        /* Reset the frame counters */
        LIN_1_txFrameCounter = 0u;
        /* Previous PCI requires to be unknown after the initialization */
        LIN_1_txPrevPci = LIN_1_PDU_PCI_TYPE_UNKNOWN;

        #if(1u == LIN_1_TL_API_FORMAT)

            if(stimulus == LIN_1_TX_TIMEOUT)
            {
                /* Set error status as the timeout occurred */
                LIN_1_txTlStatus = LD_N_AS_TIMEOUT;
            }
            else if(stimulus == LIN_1_MRF_ALIEN_NAD)
            {
                LIN_1_txTlStatus = LD_FAILED;
            }
            else
            {
                if((LIN_1_txTlStatus != LD_FAILED)   &&
                   (LIN_1_txTlStatus != LD_N_AS_TIMEOUT))
                {
                    /* Initialize Tx status variable properly */
                    LIN_1_txTlStatus = LD_COMPLETED;
                }
            }

        #else

            /* To avoid 'unused variable' warning */
            stimulus = stimulus;

            /* Reset buffers depth to 0, it will indicate the buffers are empty */
            LIN_1_txBufDepth = 0u;

            /* Raw API buffers initialization */

            LIN_1_txWrIndex = 0u;
            LIN_1_txRdIndex = 0u;

            LIN_1_txTlStatus = LD_QUEUE_EMPTY;

        #endif /* (1u == LIN_1_TL_API_FORMAT) */

        /* Clear Service Response ready status bit */
        LIN_1_status &= ((l_u8) ~LIN_1_STATUS_SRVC_RSP_RDY);

        LIN_1_tlFlags &= (l_u8) (~(LIN_1_TL_TX_REQUESTED |
            LIN_1_TL_N_AS_TIMEOUT_ON |  LIN_1_TL_N_CR_TIMEOUT_ON));

        LIN_1_tlTimeoutCnt = 0u;
    }


    /*******************************************************************************
    * Function Name: LIN_1_ProcessMrf
    ********************************************************************************
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
    *  NO_RESPONSE_REQUIRED      - No response on received message is awaited.
    *  RESPONSE_REQUIRED         - Response on received message is awaited.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    static l_u8 LIN_1_ProcessMrf(l_u8 frame[])
    {
        l_u8 i;
        l_u8 return_value = LIN_1_NO_RESPONSE_REQUIRED;
        l_u8 tempStatus = 0u;

        l_u8 tmpPci;

        #if(1u == LIN_1_TL_API_FORMAT)    /* Cooked API */
            l_u16 tmpWord;
        #endif /* (1u == LIN_1_TL_API_FORMAT) */

        l_u8 tmpByte;

        #if(1u == LIN_1_CS_ENABLED)

            l_u8 tempStatusErr = LIN_1_NO_RESPONSE_REQUIRED;

            #if(0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL))

                l_u8 idByte;
                l_u8 idMask;
                l_u8 idInvert;

            #endif /* (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL)) */

            /* To avoid 'unused variable' warning when relevant services are disabled*/
            tempStatusErr = tempStatusErr;

        #endif /*(1u == LIN_1_CS_ENABLED)*/

        /* To avoid 'unused variable' warning when relevant services are disabled*/
        tempStatus = tempStatus;

        if(LIN_1_NAD_GO_TO_SLEEP == frame[LIN_1_PDU_NAD_IDX])
        {
            LIN_1_ifcStatus |= LIN_1_IFC_STS_GO_TO_SLEEP;
        }
        else if((LIN_1_nad == frame[LIN_1_PDU_NAD_IDX]) ||
                    (LIN_1_NAD_BROADCAST == frame[LIN_1_PDU_NAD_IDX]) ||
                        (LIN_1_NAD_FUNCTIONAL == frame[LIN_1_PDU_NAD_IDX]))
        {
            /* Single Frame is detected */
            if(0u == (frame[LIN_1_PDU_PCI_IDX] & LIN_1_PDU_PCI_TYPE_MASK))
            {
                /* SID used for node configuration */
                switch(frame[LIN_1_PDU_SID_IDX])
                {
                    #if(1u == LIN_1_CS_ENABLED)

                        #if(0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB0_SEL))

                            case LIN_1_NCS_ASSIGN_NAD:

                                if(1u == LIN_1_LinProductId(&frame[3u]))  /* Check LIN Product ID */
                                {
                                    /* Save received NAD */
                                    LIN_1_nad = frame[LIN_1_PDU_D5_NEW_NAD_IDX];

                                    /* Fill SRF Buffer with response to service */
                                    /* NAD field should contain initial NAD */
                                    LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] =
                                        LIN_1_LinSlaveConfig.initialNad;

                                    /* PCI is 0 so only length required */
                                    LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 1u;

                                    /* RSID for positive response is always SID + 0x40 */
                                    LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                        LIN_1_NCS_POS_RESP_ASSIGN_NAD;

                                    /* Fill unused data bytes with 0xFFs */
                                    for(i = 3u; i < LIN_1_FRAME_LEN; i++)
                                    {
                                        LIN_1_srfBuffer[i] = 0xFFu;
                                    }

                                    /* Set service response bit that indicates that response is
                                    * ready to be sent to master node.
                                    */
                                    LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                }
                                else
                                {
                                    /* No response as Supplier ID and Function ID are invalid */
                                }

                            break;

                        #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB0_SEL) */

                        #if(0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB1_SEL))

                            case LIN_1_NCS_ASSIGN_FRAME_ID:  /* Obsolete, not implemented for LIN lin 2.1 */
                                                                        /* LIN 2.0 specification Only */
                                #if(1u == LIN_1_LIN_2_0)

                                    /* tempStatus is used in Supplier ID verification */
                                    tempStatus = 1u;

                                    if((frame[LIN_1_PDU_D1_IDX] !=
                                        LO8(LIN_1_slaveId.supplierId)) &&
                                            (frame[LIN_1_PDU_D1_IDX] !=
                                                LO8(LIN_1_CS_SUPPLIER_ID_WILDCARD)))
                                    {
                                        tempStatus = 0u;  /* Zero out tempStatus if data isn't for this slave */
                                    }

                                    if((frame[LIN_1_PDU_D2_IDX] !=
                                        HI8(LIN_1_slaveId.supplierId)) &&
                                            (frame[LIN_1_PDU_D2_IDX] !=
                                                HI8(LIN_1_CS_SUPPLIER_ID_WILDCARD)))
                                    {
                                        tempStatus = 0u;   /* Zero out tempStatus if data isn't for this slave */
                                    }

                                    /* If tempStatus is not 0 then verification passed successfully */
                                    if(tempStatus != 0u)
                                    {
                                        /* Now tempStatus is used to hold Message ID Index */
                                        tempStatus = LIN_1_MessageId(frame + 5u);

                                        /* If Message ID index is valid, process request and
                                        * prepare positive answer
                                        */
                                        if(tempStatus != LD_INVALID_MESSAGE_INDEX)
                                        {
                                            /* Set PID in position that valid index points at */
                                            LIN_1_volatileConfig[tempStatus] =
                                                frame[LIN_1_PDU_D5_IDX];

                                            /* Fill SRF Buffer with response to service */
                                            /* Nad field should contain current NAD */
                                            LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] =
                                                LIN_1_nad;

                                            /* PCI is 0 so only length required */
                                            LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 1u;

                                            /* RSID for positive response is always SID + 0x40 */
                                            LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                                LIN_1_NCS_POS_RESP_ASSIGN_FRAME_ID;

                                            /* Fill unused data bytes with 0xFFs */
                                            for(i = 3u; i < LIN_1_FRAME_LEN; i++)
                                            {
                                                LIN_1_srfBuffer[i] = 0xFFu;
                                            }

                                            /* Set service response bit that indicates that response is
                                            * ready to be sent to master node.
                                            */
                                            LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                        }
                                    }

                                #else

                                    /* Do nothing, ignore obsolete request */

                                #endif /* (1u == LIN_1_LIN_2_0) */
                            break;

                        #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB1_SEL) */

                        #if(0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL))

                            case LIN_1_NCS_READ_BY_ID:

                                /* tempStatus is used to hold status of LIN_1_LinProductId() */
                                tempStatus = LIN_1_LinProductId(&frame[4u]);

                                /* LIN Product Identification (only identifier is supported) */
                                if(LIN_1_NCS_READ_BY_ID_ID == frame[LIN_1_PDU_D1_IDX])
                                {
                                    if(1u == tempStatus)
                                    {
                                        /* Fill SRF Buffer with response to service */
                                        /* NAD field should contain current NAD */
                                        LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;

                                        /* PCI is 0 so only length required */
                                        LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 6u;

                                        /* RSID for positive response is always SID + 0x40 */
                                        LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                            LIN_1_NCS_POS_RESP_READ_BY_ID;

                                        /* Fill data fields with Supplier and function IDs */
                                        LIN_1_srfBuffer[LIN_1_PDU_D1_IDX] =
                                           LO8(LIN_1_slaveId.supplierId);
                                        LIN_1_srfBuffer[LIN_1_PDU_D2_IDX] =
                                            HI8(LIN_1_slaveId.supplierId);
                                        LIN_1_srfBuffer[LIN_1_PDU_D3_IDX] =
                                            LO8(LIN_1_slaveId.functionId);
                                        LIN_1_srfBuffer[LIN_1_PDU_D4_IDX] =
                                            HI8(LIN_1_slaveId.functionId);
                                        LIN_1_srfBuffer[LIN_1_PDU_D5_IDX] =
                                            LIN_1_slaveId.variant;

                                        /* Set service response bit that indicates that response is
                                        * ready to be sent to master node.
                                        */
                                        LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                    }
                                    else
                                    {
                                        /* No action */
                                    }
                                }
                                else if(LIN_1_NCS_READ_BY_ID_SERIAL == frame[LIN_1_PDU_D1_IDX])
                                {
                                    /* Serial number identification*/

                                    /* If slave serial number matches received one,
                                    * then prepare positive response.
                                    */
                                    if(1u == tempStatus)
                                    {
                                        /* Fill SRF Buffer with response to service */
                                        /* NAD field should contain current NAD */
                                        LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;

                                        /* PCI is 0 so only length required */
                                        LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 5u;

                                        /* RSID for positive response is always SID + 0x40 */
                                        LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                            LIN_1_NCS_POS_RESP_READ_BY_ID;

                                        /* Fill unused data bytes with serial number ID */
                                        for(i = 3u; i < (LIN_1_FRAME_LEN - 1u); i++)
                                        {
                                            LIN_1_srfBuffer[i] = LIN_1_serialNumber[i - 3u];
                                        }

                                        /* Serial number is 4 byte length, set to 0xFF last unused byte */
                                        LIN_1_srfBuffer[LIN_1_PDU_D5_IDX] = 0xFFu;

                                        /* Set service response bit that indicates that response is
                                        * ready to be sent to master node.
                                        */
                                        LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                    }
                                    else
                                    {
                                         /* Return Negative response, serial number is invalid */
                                         tempStatusErr=1u;
                                    }
                                }

                                else if((frame[LIN_1_PDU_D1_IDX] >= 32u) &&
                                        (frame[LIN_1_PDU_D1_IDX] <= 63u))   /* User defined identification */
                                {
                                    if(1u == tempStatus)
                                    {
                                        /* If user doesn't reassign the status of ld_read_by_id_callout(),
                                        * LD_NEGATIVE_RESPONSE is always returned by ld_read_by_id_callout().
                                        * This indicates to the master that the service by the user defined
                                        * identification is not supported. tempStatus is used to hold the status of
                                        * ld_read_by_id_callout().
                                        */
                                        tempStatus = 
                                            ld_read_by_id_callout_LIN_1(frame[LIN_1_PDU_D1_IDX],
                                                frame + LIN_1_FRAME_DATA_SIZE_3);

                                        if(tempStatus == LD_POSITIVE_RESPONSE)
                                        {
                                            /* Fill SRF Buffer with response to service */
                                            /* Nad field should contain current NAD */
                                            LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] =
                                                LIN_1_nad;

                                            /* PCI is 0 so only length required */
                                            LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 6u;

                                            /* RSID for positive response is always SID + 0x40 */
                                            LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                                LIN_1_NCS_POS_RESP_READ_BY_ID;

                                            /* Fill unused data bytes with user defined information */
                                            for(i = 3u; i < LIN_1_FRAME_LEN; i++)
                                            {
                                                LIN_1_srfBuffer[i] = frame[i];
                                            }

                                            /* Set service response bit that indicates that response is
                                            * ready to be sent to master node.
                                            */
                                            LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                        }
                                        else
                                        {
                                            /* Return Negative response */
                                             tempStatusErr=1u;
                                        }
                                    }
                                    else
                                    {
                                        /* Return Negative response, serial number is invalid */
                                         tempStatusErr=1u;
                                    }
                                }
                                /* Message ID identification*/
                                else if((frame[LIN_1_PDU_D1_IDX] >= LIN_1_ID_16) &&
                                    (frame[LIN_1_PDU_D1_IDX] <= LIN_1_ID_31))
                                {
                                    /* LIN 2.0 specification Only */
                                    #if(1u == LIN_1_LIN_2_0)

                                        /* If slave serial number matches received one,
                                        * prepare positive response.
                                        */
                                        if(1u == tempStatus)
                                        {
                                            /* Fill SRF Buffer with response to service */
                                            /* NAD field should contain current NAD */
                                            LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] =
                                                LIN_1_nad;

                                            /* tempStatus is now used to store calculated Message ID index */
                                            tempStatus = frame[LIN_1_PDU_D1_IDX] - LD_MESSAGE_ID_BASE;

                                            if(LIN_1_NUM_FRAMES > tempStatus)
                                            {
                                                /* PCI is 0 so only length required */
                                                LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 4u;

                                                /* RSID for positive response is always SID + 0x40 */
                                                LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                                    LIN_1_NCS_POS_RESP_READ_BY_ID;

                                                /* D1 = Message ID LSB */
                                                LIN_1_srfBuffer[LIN_1_PDU_D1_IDX] =
                                                    HI8(messageIdTable[tempStatus]);

                                                /* D2 = Message ID MSB */
                                                LIN_1_srfBuffer[LIN_1_PDU_D2_IDX] =
                                                    LO8(messageIdTable[tempStatus]);

                                                /* D3 = PID */
                                                LIN_1_srfBuffer[LIN_1_PDU_D3_IDX] =
                                                    LIN_1_volatileConfig[tempStatus];

                                                /* Message ID response is 3 byte length, so set last two bytes
                                                * to 0xFF.
                                                */
                                                LIN_1_srfBuffer[LIN_1_PDU_D4_IDX] = 0xFFu;
                                                LIN_1_srfBuffer[LIN_1_PDU_D5_IDX] = 0xFFu;

                                                /* Set service response bit that indicates that response is
                                                * ready to be sent to master node.
                                                */
                                                LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                            }
                                            else
                                            {
                                                 tempStatusErr=1u;
                                            }
                                        }
                                        else
                                        {
                                             tempStatusErr=1u;
                                        }

                                    #else

                                        /* Return Negative response on erroneous request */
                                        tempStatusErr=1u;

                                    #endif /* (1u == LIN_1_LIN_2_0) */
                                }
                                else
                                {
                                    /* Return Negative response */
                                    tempStatusErr=1u;
                                }

                                if(1u == tempStatusErr)
                                {
                                    /* Fill SRF Buffer with negative response to service */
                                    /* Nad field should contain current NAD */
                                    LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;

                                    /* PCI is 0 so only length required */
                                    LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 3u;

                                    /* RSID for positive response is always SID + 0x40 */
                                    LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                        LIN_1_NCS_RSID_NEG_REPLY;

                                    /* D1 holds the service ID */
                                    LIN_1_srfBuffer[LIN_1_PDU_D1_ID_IDX] =
                                        LIN_1_NCS_READ_BY_ID;

                                    /* D2 contains error code */
                                    LIN_1_srfBuffer[LIN_1_PDU_D2_IDX] = 0x12u;

                                    /* Fill unused data bytes with 0xFFs */
                                    for(i = 5u; i < LIN_1_FRAME_LEN; i++)
                                    {
                                        LIN_1_srfBuffer[i] = 0xFFu;
                                    }

                                    /* Set service response bit that indicates that response is
                                    * ready to be sent to master node.
                                    */
                                    LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                }
                                else
                                {
                                    /* Do nothing */
                                }
                            break;

                        #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL) */

                        #if(0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL))

                            case LIN_1_NCS_COND_CHANGE_NAD:

                                if(LIN_1_NCS_READ_BY_ID_ID == frame[LIN_1_PDU_D1_ID_IDX])
                                {
                                    /* LIN Product Identification */
                                    if((6u > frame[LIN_1_PDU_D2_BYTE_IDX]) &&
                                        (0u != frame[LIN_1_PDU_D2_BYTE_IDX]))
                                    {
                                        switch (frame[LIN_1_PDU_D2_BYTE_IDX])
                                        {
                                            /* tempStatus is used as a temporary variable to store ID byte */

                                            case LIN_1_CS_BYTE_SUPPLIER_ID1:

                                                tempStatus = LO8(LIN_1_slaveId.supplierId);

                                            break;

                                            case LIN_1_CS_BYTE_SUPPLIER_ID2:

                                                tempStatus = HI8(LIN_1_slaveId.supplierId);

                                            break;

                                            case LIN_1_CS_BYTE_FUNCTION_ID1:

                                                tempStatus = LO8(LIN_1_slaveId.functionId);

                                            break;

                                            case LIN_1_CS_BYTE_FUNCTION_ID2:

                                                tempStatus = HI8(LIN_1_slaveId.functionId);

                                            break;

                                            case LIN_1_CS_BYTE_VARIANT:

                                                tempStatus = LIN_1_slaveId.variant;

                                            break;

                                            default:

                                                /* This state should never be used */

                                            break;
                                        }

                                        if(0u == ((tempStatus ^ frame[LIN_1_PDU_D4_INVERT_IDX]) &
                                            frame[LIN_1_PDU_D3_MASK_IDX]))
                                        {
                                            /* Fill SRF Buffer with response to service */
                                            /* NAD field should contain current NAD */
                                            LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] =
                                                LIN_1_nad;

                                            /* Change NAD to new NAD */
                                            LIN_1_nad = frame[LIN_1_PDU_D5_NEW_NAD_IDX];


                                            /* PCI is 0 so only length required */
                                            LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 1u;

                                            /* RSID for positive response is always SID + 0x40 */
                                            LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                                LIN_1_NCS_POS_RESP_COND_CHANGE_NAD;

                                            /* Fill unused bytes with 0xFF */
                                            for(i = 3u; i < LIN_1_FRAME_LEN; i++)
                                            {
                                                LIN_1_srfBuffer[i] = 0xFFu;
                                            }

                                            /* Set service response bit that indicates that response is
                                            * ready to be sent to master node.
                                            */
                                            LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                        }
                                        else
                                        {
                                            /* Do nothing, ignore erroneous request */
                                        }
                                    }
                                    else
                                    {
                                        /* Do nothing, ignore erroneous request */
                                    }
                                }
                                else if(LIN_1_NCS_READ_BY_ID_SERIAL == frame[LIN_1_PDU_D1_ID_IDX])
                                {
                                    if((5u < frame[LIN_1_PDU_D2_BYTE_IDX]) &&
                                        (0u != frame[LIN_1_PDU_D2_BYTE_IDX]))
                                    {
                                        /* Byte = 1 corresponds to first byte (LIN_1_serialNumber[0]) */
                                        if(0u == ((
                                        LIN_1_serialNumber[frame[LIN_1_PDU_D2_BYTE_IDX] - 1u] ^
                                            frame[LIN_1_PDU_D4_INVERT_IDX]) &
                                                frame[LIN_1_PDU_D3_MASK_IDX]))
                                        {

                                            /* Fill SRF Buffer with response to service */
                                            /* NAD field should contain current NAD */
                                            LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] =
                                                LIN_1_nad;

                                            /* Change NAD to new NAD */
                                            LIN_1_nad = frame[LIN_1_PDU_D5_NEW_NAD_IDX];


                                            /* PCI is 0 so only length required */
                                            LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 1u;

                                            /* RSID for positive response is always SID + 0x40 */
                                            LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                                LIN_1_NCS_POS_RESP_COND_CHANGE_NAD;

                                            /* Fill unused bytes with 0xFF */
                                            for(i = 3u; i < LIN_1_FRAME_LEN; i++)
                                            {
                                                LIN_1_srfBuffer[i] = 0xFFu;
                                            }

                                            /* Set service response bit that indicates that response is
                                            * ready to be sent to master node.
                                            */
                                            LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                        }
                                        else
                                        {
                                            /* Do nothing, ignore erroneous request */
                                        }
                                    }
                                    else
                                    {
                                        /* Do nothing, ignore erroneous request */
                                    }
                                }
                                else if((frame[LIN_1_PDU_D1_IDX] >= 32u) &&
                                        (frame[LIN_1_PDU_D1_IDX] <= 63u))   /* User defined identification */
                                {
                                    /* Need to store Byte, Invert, and Mask in variables for user defined
                                    * identification as frame[] should contain user data after execution of
                                    * ld_read_by_id_callout();
                                    */
                                    idByte = frame[LIN_1_PDU_D2_BYTE_IDX] - 1u;
                                    idInvert = frame[LIN_1_PDU_D4_INVERT_IDX];
                                    idMask = frame[LIN_1_PDU_D3_MASK_IDX];

                                    /* If user doesn't reassign the status of ld_read_by_id_callout(),
                                    * LD_NEGATIVE_RESPONSE is always returned by ld_read_by_id_callout(). This
                                    * indicates to the master that the service by the user defined identification is
                                    * not supported. tempStatus is used to hold the status of ld_read_by_id_callout().
                                    */
                                    tempStatus = 
                                        ld_read_by_id_callout_LIN_1(frame[LIN_1_PDU_D1_IDX],
                                            frame + LIN_1_PDU_D1_IDX);

                                    if((tempStatus == LD_NEGATIVE_RESPONSE) || (tempStatus == LD_NO_RESPONSE))
                                    {
                                        /* Do nothing as there is no response from user */
                                    }
                                    else
                                    {
                                        if(0u == ((frame[idByte + LIN_1_PDU_D1_IDX] ^ idInvert) & idMask))
                                        {
                                            /* Change NAD to new NAD */
                                            LIN_1_nad = frame[LIN_1_PDU_D5_NEW_NAD_IDX];

                                            /* Fill SRF Buffer with response for a service */
                                            /* NAD field should contain changed NAD */
                                            LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] =
                                                LIN_1_nad;

                                            /* PCI is 0 so only length required */
                                            LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 1u;

                                            /* RSID for positive response is always SID + 0x40 */
                                            LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                                LIN_1_NCS_POS_RESP_COND_CHANGE_NAD;

                                            /* Fill unused bytes with 0xFF */
                                            for(i = 3u; i < LIN_1_FRAME_LEN; i++)
                                            {
                                                LIN_1_srfBuffer[i] = 0xFFu;
                                            }

                                            /* Set service response bit that indicates that response is
                                            * ready to be sent to master node.
                                            */
                                            LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                        }
                                        else
                                        {
                                            /* Do nothing, ignore erroneous request */
                                        }
                                    }
                                }
                                else
                                {
                                    /* Do nothing, ignore erroneous request */
                                }
                            break;

                        #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL) */

                        #if(0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB4_SEL))

                            case LIN_1_NCS_DATA_DUMP:

                                /* Not Supported */

                            break;

                        #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB4_SEL) */

                        #if(0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB5_SEL))

                            /* LIN Slave Node Position Detection */
                            case LIN_1_NCS_ASSIGN_NAD_SNPD:

                                #if(1u == LIN_1_SAE_J2602)

                                    LIN_1_ioctlStatus |= LIN_1_IOCTL_STS_TARGET_RESET;

                                #endif /* (1u == LIN_1_SAE_J2602) */

                            break;

                        #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB5_SEL) */

                        #if(0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB6_SEL))

                            case LIN_1_NCS_SAVE_CONFIG:

                                /* Set save configuration bit in status register */
                                LIN_1_ifcStatus |= LIN_1_IFC_STS_SAVE_CONFIG;

                                /* Fill SRF Buffer with response to service */
                                /* NAD field should contain current NAD */
                                LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] = LIN_1_nad;

                                /* PCI is 0 so only length required */
                                LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 1u;

                                /* RSID for positive response is always SID + 0x40 */
                                LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                    LIN_1_NCS_POS_RESP_SAVE_CONFIG;

                                /* Fill unused data bytes with 0xFFs */
                                for(i = 3u; i < LIN_1_FRAME_LEN; i++)
                                {
                                    LIN_1_srfBuffer[i] = 0xFFu;
                                }

                                /* Set service response bit that indicates that response is
                                * ready to be sent to master node.
                                */
                                LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;

                            break;

                        #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB6_SEL) */

                        #if(0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB7_SEL))

                            case LIN_1_NCS_ASSIGN_FRAME_ID_RANGE:

                                /* Zero out temp status. It is used as error counter. */
                                tempStatus = 0u;

                                for(i = 0u; i < LIN_1_NCS_MAX_FRAME_ID_RANGE; i++)
                                {
                                    if((i + frame[LIN_1_PDU_D1_START_IDX]) < LIN_1_NUM_FRAMES)
                                    {
                                        if((frame[i + LIN_1_PDU_D2_PID_IDX] !=
                                            LIN_1_INVALID_FRAME_PID) &&
                                                ((frame[i + LIN_1_PDU_D2_PID_IDX] &
                                                    LIN_1_PID_PARITY_MASK) < LIN_1_FRAME_PID_MRF))
                                        {
                                            /* Unassigned value "0" is used to invalidate
                                            *  this frame for transportation to the bus.?
                                            */
                                            /* Set new received PID value */
                                            LIN_1_volatileConfig[i +
                                                frame[LIN_1_PDU_D1_START_IDX]] =
                                                    frame[i + LIN_1_PDU_D2_PID_IDX];
                                        }
                                        else if(frame[i + LIN_1_PDU_D2_PID_IDX] == 0xFFu)
                                        {
                                            /* Do nothing. */
                                        }
                                        else
                                        {
                                            /* Indicate error by changing status other than 0
                                            * if Frame ID is reserved.
                                            */
                                            tempStatus++;
                                        }
                                    }
                                    else
                                    {
                                        if(frame[i + LIN_1_PDU_D2_PID_IDX] != 0xFFu)
                                        {
                                            tempStatus++;  /* Indicate error by changing status other than 0 */
                                        }
                                    }
                                }

                                if(tempStatus == 0u) /* No errors condition check */
                                {
                                    /* Fill SRF Buffer with response to service */
                                    /* NAD field should contain current NAD */
                                    LIN_1_srfBuffer[LIN_1_PDU_NAD_IDX] =
                                        LIN_1_nad;

                                    /* PCI is 0 so only length required */
                                    LIN_1_srfBuffer[LIN_1_PDU_PCI_IDX] = 1u;

                                    /* RSID for positive response is always SID + 0x40 */
                                    LIN_1_srfBuffer[LIN_1_PDU_SID_IDX] =
                                        LIN_1_NCS_POS_RESP_ASSIGN_FRAME_ID_RANGE;

                                    /* Fill unused data bytes with 0xFFs */
                                    for(i = 3u; i < LIN_1_FRAME_LEN; i++)
                                    {
                                        LIN_1_srfBuffer[i] = 0xFFu;
                                    }

                                    /* Set service response bit that indicates that response is
                                    * ready to be sent to master node.
                                    */
                                    LIN_1_status |= LIN_1_STATUS_SRVC_RSP_RDY;
                                }
                                else
                                {
                                    /* Do nothing, ignore erroneous request */
                                }

                            break;

                        #endif /* (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB7_SEL) */

                    #endif /* (1u == LIN_1_CS_ENABLED) */

                    default:

                       /* This indicates that the requested service is disabled and Frame
                       * will be "passed" to TL. This means the user should process this Frame
                       * properly using TL API.
                       */
                       LIN_1_tlFlags |= LIN_1_TL_CS_SERVICE_DISABLED;

                       /* Indicates that the detected SID is a diagnostic SID and it should be
                       * passed to Transport Layer.
                       */
                      LIN_1_tlFlags |= LIN_1_TL_DIAG_FRAME_DETECTED;

                    break;
                }

                if((0u != (LIN_1_tlFlags & LIN_1_TL_CS_SERVICE_DISABLED)) ||
                   (0u != (LIN_1_tlFlags & LIN_1_TL_DIAG_FRAME_DETECTED)))
                {
                    /* SID used for diagnostics */
                    if(frame[LIN_1_PDU_PCI_IDX] <= LIN_1_PDU_SF_DATA_LEN)
                    {
                        #if(1u == LIN_1_TL_API_FORMAT) /* Cooked API */

                            /* Get one frame of message if there is message pending */
                            if(LIN_1_rxTlStatus == LD_IN_PROGRESS)
                            {
                                /* Make sure pointer points
                                * receive buffer beginning.
                                */
                                if(0u != (LIN_1_tlFlags & LIN_1_TL_CS_SERVICE_DISABLED))
                                {
                                    LIN_1_rxTlDataPointer = LIN_1_rxTlInitDataPointer;
                                }
                                else
                                {
                                    /* Do nothing */
                                }

                                /* Copy data to user buffer */
                                for(i = 0u; i < frame[LIN_1_PDU_PCI_IDX]; i++)
                                {
                                    *LIN_1_rxTlDataPointer = frame[i + 2u];
                                    LIN_1_rxTlDataPointer++;
                                }

                                /* Store NAD */
                                *LIN_1_tlNadPointer = frame[LIN_1_PDU_NAD_IDX];

                                /* Get data bytes length */
                                *LIN_1_tlLengthPointer = (l_u16) frame[LIN_1_PDU_PCI_IDX];

                                /* Update length pointer properly */
                                LIN_1_rxMessageLength = 0u;

                                /* SF message is received, so set proper status */
                                LIN_1_rxTlStatus = LD_COMPLETED;
                            }

                        #else /* Raw API */

                            if(LIN_1_rxBufDepth < (LIN_1_TL_RX_QUEUE_LEN / 8u))
                            {
                                /* Fill RX queue from MRF buffer */
                                for(i = 0u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                                {
                                    LIN_1_rawRxQueue[LIN_1_rxWrIndex] = frame[i];
                                    LIN_1_rxWrIndex++;
                                }

                                /* "read" index should point to next byte in MRF */
                                if(LIN_1_rxWrIndex == LIN_1_TL_RX_QUEUE_LEN)
                                {
                                    LIN_1_rxWrIndex = 0u;
                                }

                                /* 8 Bytes copied to MRF - increment buffer depth */
                                LIN_1_rxBufDepth++;

                                /* Specification doesn't require status "queue full"
                                * so unconditionally set status to data available
                                */
                                LIN_1_rxTlStatus = LD_DATA_AVAILABLE;

                                LIN_1_rxMessageLength = 0u;

                                return_value = LIN_1_RESPONSE_REQUIRED;
                            }

                        #endif /* (1u == LIN_1_TL_API_FORMAT) */

                        return_value = LIN_1_RESPONSE_REQUIRED;
                    }
                    else
                    {
                        /* Do nothing. Length is not valid, data should not be trusted. */
                        return_value = LIN_1_IGNORE_TL_FRAME;
                    }

                    /* Clear Service Disabled and Diagnostic Frame detected bits
                    * and process diagnostic frame receiving into user buffer or MRF.
                    */
                    LIN_1_tlFlags &= ((l_u8) ~(LIN_1_TL_CS_SERVICE_DISABLED | 
                        LIN_1_TL_DIAG_FRAME_DETECTED));
                }
                else
                {
                    /* Do nothing. Length is not valid, data should not be trusted. */
                    return_value = LIN_1_IGNORE_TL_FRAME;
                }
            }
            else if((frame[LIN_1_PDU_PCI_IDX] & LIN_1_PDU_PCI_TYPE_MASK) ==
                LIN_1_PDU_PCI_TYPE_FF)                                 /* First Frame detected */
            {
                if(frame[LIN_1_PDU_LEN_IDX] >= LIN_1_FRAME_DATA_SIZE_7)
                {
                    #if(1u == LIN_1_TL_API_FORMAT)    /* Cooked API */

                        /* Get one frame of message if there is message pending
                        * and PCI is valid
                        */
                        LIN_1_rxPrevPci = LIN_1_PDU_PCI_TYPE_UNKNOWN;

                            /* Store result of condition check operand in temporary
                            * variable. This improves the code readability and removes the
                            * MISRA warning about uneven typecast.
                            */
                            tmpByte = (frame[LIN_1_PDU_PCI_IDX] &
                                ((l_u8) ~LIN_1_PDU_PCI_TYPE_MASK));

                            /* The following line allows avoiding violation of MISRA Rule 10.3:
                            * "A composite expression of 'essentially unsigned' type (unsigned char)
                            * is being cast to a wider unsigned type, 'unsigned int'."
                            */
                            tmpWord = (l_u16) tmpByte;

                            tmpWord = ((l_u16) (tmpWord << 8u)) | ((l_u16) frame[LIN_1_PDU_LEN_IDX]);

                    if((LIN_1_rxTlStatus == LD_IN_PROGRESS) &&
                        (LIN_1_TL_BUF_LEN_MAX >= tmpWord))
                        {
                            /* Get First Frame Length with following two operations */

                            LIN_1_rxMessageLength = (l_u16)((((l_u16)
                                frame[LIN_1_PDU_PCI_IDX]) &
                                    ((l_u16) ((l_u8) ~LIN_1_PDU_PCI_TYPE_MASK))) << 8u);

                            LIN_1_rxMessageLength |= ((l_u16) frame[LIN_1_PDU_LEN_IDX]);

                            /* Copy Length to current length variable */
                            *LIN_1_tlLengthPointer = LIN_1_rxMessageLength;

                            for(i = 3u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                            {
                                *LIN_1_rxTlDataPointer = frame[i];   /* Get Frame Data */
                                LIN_1_rxTlDataPointer++;
                            }

                    #else /* Raw API */

                        if(LIN_1_rxBufDepth < (LIN_1_TL_RX_QUEUE_LEN / 8u))
                        {
                            /* Copy Length to current length variable */
                            /* Get First Frame Length with following two operations */
                            LIN_1_rxMessageLength = (l_u16)((((l_u16)
                                frame[LIN_1_PDU_PCI_IDX]) &
                                    ((l_u16) ((l_u8) ~LIN_1_PDU_PCI_TYPE_MASK)))<< 8u);

                            LIN_1_rxMessageLength |=
                                    ((l_u16) frame[LIN_1_PDU_LEN_IDX]);

                            /* Fill MRF from frame buffer */
                            for(i = 0u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                            {
                                LIN_1_rawRxQueue[LIN_1_rxWrIndex] = frame[i];
                                LIN_1_rxWrIndex++;
                            }

                            /* "read" index should point to next byte in MRF */
                            if(LIN_1_rxWrIndex == LIN_1_TL_RX_QUEUE_LEN)
                            {
                                LIN_1_rxWrIndex = 0u;
                            }

                            /* 8 Bytes copied to MRF - increment buffer depth */
                            LIN_1_rxBufDepth++;

                            /* Specification doesn't require status "queue full",
                            * so unconditionally set status to data available
                            */
                            LIN_1_rxTlStatus = LD_DATA_AVAILABLE;

                    #endif /* (1u == LIN_1_TL_API_FORMAT) */

                            /* Update length pointer properly */
                            LIN_1_rxMessageLength -= LIN_1_FRAME_DATA_SIZE_5;

                            /* Save the state of the Frame Counter for monitor future possible errors */
                            LIN_1_rxFrameCounter = 0u;

                            /* Save the PCI type */
                            LIN_1_rxPrevPci = LIN_1_PDU_PCI_TYPE_FF;

                            return_value = LIN_1_RECEIVE_CONTINUES;
                        }
                        else
                        {
                            /* Do nothing Length is invalid */
                            return_value = LIN_1_IGNORE_TL_FRAME;
                        }
                }
                else
                {
                    /* Do nothing. Length is not valid, data should not be trusted. */
                    return_value = LIN_1_IGNORE_TL_FRAME;
                }
            }
            /* Consecutive Frames detected */
            else if((frame[LIN_1_PDU_PCI_IDX] & LIN_1_PDU_PCI_TYPE_MASK) ==
                LIN_1_PDU_PCI_TYPE_CF)                                 /* Consecutive Frames detected */
            {
                /* Store previous PCI to avoid MISRA warning */
                tmpPci = LIN_1_rxPrevPci;

                /* Get one frame of a message if there is a message pending and the
                * PCI is valid.
                */
                if((tmpPci == LIN_1_PDU_PCI_TYPE_FF) ||
                        (tmpPci == LIN_1_PDU_PCI_TYPE_CF))
                {
                    /* Store the result of condition check operand in temporary
                    * variable. This will improve code readability and will remove
                    * MISRA warning of uneven typecast.
                    */
                    tmpByte = frame[LIN_1_PDU_PCI_IDX] & ((l_u8) ~LIN_1_PDU_PCI_TYPE_MASK);

                    /* Check if frame counter is valid , counter is always less then 16 */
                    if(((LIN_1_rxFrameCounter + 1u) & 0x0Fu) == tmpByte)
                    {

                        #if(1u == LIN_1_TL_API_FORMAT) /* Cooked API */

                            /* Check if message is requested from application */
                            if(LIN_1_rxTlStatus == LD_IN_PROGRESS)
                            {
                                for(i = 2u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                                {
                                    *LIN_1_rxTlDataPointer = frame[i];    /* Get Frame Data */
                                    LIN_1_rxTlDataPointer++;
                                }

                        #else /* Raw API */

                            if(LIN_1_rxBufDepth < (LIN_1_TL_RX_QUEUE_LEN / 8u))
                            {
                                /* Fill the MRF from a frame buffer */
                                for(i = 0u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                                {
                                    LIN_1_rawRxQueue[LIN_1_rxWrIndex] = frame[i];
                                    LIN_1_rxWrIndex++;
                                }

                                /* Read index should point to the next byte in MRF */
                                if(LIN_1_rxWrIndex == LIN_1_TL_RX_QUEUE_LEN)
                                {
                                    LIN_1_rxWrIndex = 0u;
                                }

                                /* 8 Bytes copied to MRF - increment buffer depth */
                                LIN_1_rxBufDepth++;

                                /* Specification doesn't require status of queue full
                                * so unconditionally set the status to data available
                                */
                                LIN_1_rxTlStatus = LD_DATA_AVAILABLE;

                        #endif /* (1u == LIN_1_TL_API_FORMAT) */

                            /* Save current Frame Counter */
                            LIN_1_rxFrameCounter =
                                frame[LIN_1_PDU_PCI_IDX] & ((l_u8) ~LIN_1_PDU_PCI_TYPE_MASK);

                            /* Save PCI type */
                            LIN_1_rxPrevPci = LIN_1_PDU_PCI_TYPE_CF;

                            /* Update length pointer properly */
                            if(LIN_1_rxMessageLength > LIN_1_FRAME_DATA_SIZE_6)
                            {
                                LIN_1_rxMessageLength -= LIN_1_FRAME_DATA_SIZE_6;

                                return_value = LIN_1_RECEIVE_CONTINUES;
                            }
                            else
                            {
                                LIN_1_rxMessageLength = 0u;
                                return_value = LIN_1_RESPONSE_REQUIRED;
                            }

                            }
                            else
                            {
                                /* Indicate error if Frame Counter is invalid. */
                            }
                        }
                        else
                        {
                            /* Indicate an error if Frame Counter is invalid. */
                            LIN_1_rxTlStatus = LD_WRONG_SN;
                            return_value = LIN_1_ERRONEOUS_TL_FRAME;
                        }
                    }
                    else
                    {
                        /* It is a spurious CF frame */
                        return_value = LIN_1_IGNORE_TL_FRAME;
                    }
                }
                else
                {
                    /* Do nothing SID is invalid. */
                    return_value = LIN_1_IGNORE_TL_FRAME;
                }

                if(0u != (LIN_1_status & LIN_1_STATUS_SRVC_RSP_RDY))
                {
                    /* Change node state to next state */
                    return_value = LIN_1_RESPONSE_REQUIRED;
                }
            }

            else  /* Alien NAD. Indicate error */
            {
                 /* Clear Service Response ready status bit */
                LIN_1_status &= ((l_u8) ~LIN_1_STATUS_SRVC_RSP_RDY);

                #if(1u == LIN_1_TL_API_FORMAT) /* Cooked API */
                    /* Reception failed */
                    if(0u != (LIN_1_tlFlags & LIN_1_TL_RX_REQUESTED))
                    {
                        LIN_1_rxTlStatus = LD_FAILED;

                        LIN_1_txTlStatus = LD_FAILED;

                        LIN_1_tlFlags &=((l_u8) ~LIN_1_TL_RX_REQUESTED);
                    }

                #endif /* (1u == LIN_1_TL_API_FORMAT) */
            }
            
        if(LIN_1_NAD_FUNCTIONAL != LIN_1_mrfBuffer[LIN_1_PDU_NAD_IDX])
        {
            if((return_value == LIN_1_RESPONSE_REQUIRED) ||
                (return_value == LIN_1_RECEIVE_CONTINUES))
            {
                LIN_1_tlFlags |= LIN_1_TL_N_CR_TIMEOUT_ON;
                LIN_1_tlTimeoutCnt = 0u;
            }
        }

        if(return_value == LIN_1_RESPONSE_REQUIRED)
        {
            if(0u == LIN_1_rxMessageLength)
            {

                /* Store previous PCI to avoid MISRA warning */
                tmpPci = LIN_1_rxPrevPci;

                /* Reset the frame counter */
                LIN_1_rxFrameCounter = 0u;

                /* Previous PCI is required to be unknown at the beginning of a new message */
                LIN_1_rxPrevPci = LIN_1_PDU_PCI_TYPE_UNKNOWN;

                #if(1u == LIN_1_TL_API_FORMAT)

                    if( 0u != (LIN_1_tlFlags & LIN_1_TL_RX_REQUESTED))
                    {
                        /* Indicate that message is received */
                        LIN_1_rxTlStatus = LD_COMPLETED;

                        /* Clear RX requested flag as the message was received */
                        LIN_1_tlFlags &= ((l_u8) ~LIN_1_TL_RX_REQUESTED);
                    }
                #endif /* (1u == LIN_1_TL_API_FORMAT) */
            }
        }
      return(return_value);
    }

    #if(1u == LIN_1_TL_API_FORMAT)

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
        *  the length must be in the range from 1 to 4095 bytes. The length shall also
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
        *  iii - Interface.
        *  length - The size of data to be sent in bytes.
        *  nad - the address of the slave node to which data is sent.
        *  data - The array of data to be sent. The value of RSID is the first byte
        *  in the data area.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void ld_send_message_LIN_1(l_u16 length, l_u8 nad, const l_u8* const ld_data)
        {
            volatile  l_u8 interruptState;

            /* NAD isn't used in slave node */
            nad = nad;

            if(LIN_1_txTlStatus != LD_IN_PROGRESS)
            {
                /* Interrupts can be disabled as global variables used by LIN ISR
                * are used below.?
                */
                interruptState = CyEnterCriticalSection();

                LIN_1_txTlDataPointer = ld_data;

                /* Reset data count */
                LIN_1_txTlDataCount = 0u;

                /* Set up length pointer. Length shouldn't be greater than?
                * LIN_1_TL_BUF_LEN_MAX.
                */
                LIN_1_txMessageLength = length;

                /* Indicates that there is message in progress */
                LIN_1_txTlStatus = LD_IN_PROGRESS;

                /* Indicates that Cooked API request transmit data */
                LIN_1_tlFlags |= LIN_1_TL_TX_REQUESTED;

                /* Restore interrupt state */
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
        *  iii - Interface.
        *  length: The size of data to be received in bytes.
        *  nad: The address of the slave node from which data is received.
        *  data: The array of data to be received. The value of the SID is the first
        *  byte in the data area.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void ld_receive_message_LIN_1(l_u16* const length,
            l_u8* const nad, l_u8* const ld_data)
        {
            l_u8 interruptState;

            if(LIN_1_rxTlStatus != LD_IN_PROGRESS)
            {
                /* Interrupts can be disabled as global variables used by LIN ISR
                * are used below.
                */
                interruptState = CyEnterCriticalSection();

                /* Set user status bits */
                LIN_1_rxTlStatus = LD_IN_PROGRESS;

                /* Set up data pointer */
                LIN_1_rxTlDataPointer = ld_data;

                /* Set up initial data pointer that should
                * always point to user buffer beginning.?
                */
                LIN_1_rxTlInitDataPointer = ld_data;

                /* Set up NAD pointer */
                LIN_1_tlNadPointer = nad;

                /* Set up length pointer */
                LIN_1_tlLengthPointer = length;

                LIN_1_rxMessageLength = *length;

                /* Indicates that Cooked API requests receive data */
                LIN_1_tlFlags |= LIN_1_TL_RX_REQUESTED;

                /* Restore interrupt state */
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
        l_u8 ld_tx_status_LIN_1(void)
        {
            l_u8 status = LIN_1_txTlStatus;
            
            if(LIN_1_txTlStatus != LD_IN_PROGRESS)
            {
                LIN_1_txTlStatus = LD_COMPLETED;
            }
            
            return(status);
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
        l_u8 ld_rx_status_LIN_1(void)
        {
            l_u8 status = LIN_1_rxTlStatus;
            
            if(LIN_1_rxTlStatus != LD_IN_PROGRESS)
            {
                LIN_1_rxTlStatus = LD_COMPLETED;
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
        *
        * Parameters:
        *  iii - Interface.
        *  data - The array of data to be sent.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void ld_put_raw_LIN_1(const l_u8* const ld_data)
        {
            l_u8 interruptState;
            l_u8 i;

            /* Interrupts can be disabled as global variables used by LIN ISR
            * are used below.
            */
            interruptState = CyEnterCriticalSection();

            /* Copy data only when buffer is not full */
            if(LIN_1_txBufDepth < (LIN_1_TL_TX_QUEUE_LEN / 8u))
            {
                /* Copy 8 bytes of data to Raw TX queue buffer */
                for(i = 0u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                {
                    /* Copy one byte of data to SRF buffer */
                    LIN_1_rawTxQueue[LIN_1_txWrIndex] = *(ld_data + i);
                    LIN_1_txWrIndex++;
                }

                /* If buffer end is reached, reset write index */
                if(LIN_1_txWrIndex == LIN_1_TL_TX_QUEUE_LEN)
                {
                    LIN_1_txWrIndex = 0u;
                }

                /* 8 bytes of data were copied, so increment a buffer depth */
                LIN_1_txBufDepth++;

                /* Update status properly */
                if(LIN_1_txBufDepth == (LIN_1_TL_TX_QUEUE_LEN / 8u))
                {
                    LIN_1_txTlStatus = LD_QUEUE_FULL;
                }
                else
                {
                    LIN_1_txTlStatus = LD_QUEUE_AVAILABLE;
                }
            
                LIN_1_tlFlags |= LIN_1_TL_TX_REQUESTED;
            }

            /* Restore interrupt state */
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
        *  iii - Interface.
        *  data - The array to which the oldest received diagnostic frame data is
        *  copied.
        *
        * Return:
        *  None
        *
        *  No
        *
        *******************************************************************************/
        void ld_get_raw_LIN_1(l_u8* const ld_data)
        {
            l_u8 interruptState;
            l_u8 i;

            /* Interrupts can be disabled as global variables used by LIN ISR
            * are used below.
            */
            interruptState = CyEnterCriticalSection();

            /* If queue is empty, do not copy anything */
            if(LIN_1_rxBufDepth != 0u)
            {
                /* Copy 8 bytes of data from Raw RX queue buffer */
                for(i = 0u; i < LIN_1_FRAME_DATA_SIZE_8; i++)
                {
                    *(ld_data + i) = LIN_1_rawRxQueue[LIN_1_rxRdIndex];
                    LIN_1_rxRdIndex++;
                }

                /* 8 bytes of data were copied so decrement a buffer depth */
                LIN_1_rxBufDepth--;

                /* If buffer end is reached, go to start */
                if(LIN_1_rxRdIndex == LIN_1_TL_RX_QUEUE_LEN)
                {
                    LIN_1_rxRdIndex = 0u;
                }

                /* Update status properly */
                if(LIN_1_rxBufDepth == 0u)
                {
                    LIN_1_rxTlStatus = LD_NO_DATA;
                }
                else
                {
                    LIN_1_rxTlStatus = LD_DATA_AVAILABLE;
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
        l_u8 ld_raw_tx_status_LIN_1(void)
        {
            return(LIN_1_txTlStatus);
        }


        /*******************************************************************************
        * Function Name: LIN_1_ld_raw_rx_status
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
        l_u8 ld_raw_rx_status_LIN_1(void)
        {
            return(LIN_1_rxTlStatus);
        }

    #endif /* (1u == LIN_1_TL_API_FORMAT) */

#endif /* (1u == LIN_1_TL_ENABLED) */


/*******************************************************************************
* Function Name: LIN_1_TimerEnableInt
********************************************************************************
*
* Summary:
*  Enables Systick timer interrupts and configures the SystemTick timer callback
*  slot to the component function that updates the timestamp value.
*
*
* Parameters:
*  None
*
* Return:
*  Status
*     Value               Description
*    CYRET_SUCCESS       Successful
*    CYRET_INVALID_STATE Failure - No free callback slot has been found.
*
* Side Effects:
*  None
*
*******************************************************************************/
cystatus LIN_1_TimerEnableInt(void)
{
    cystatus retValue = CYRET_INVALID_STATE;

    uint32 i;

    /* If SysTick timer isn't configured */
    if(CySysTickInitVar == 0u)
    {
        /* SysTick timer seems to be not configured.
         * Configure SysTick timer and set callback.
         */
        CySysTickStart();
        (void)CySysTickSetCallback(0u, &l_ifc_aux_LIN_1);
        retValue = CYRET_SUCCESS;
    }
    else
    {
        /* If SysTick timer has been configured, check if own callback exists */
        for(i = 0u; i < CY_SYS_SYST_NUM_OF_CALLBACKS; i++)
        {
            if(CySysTickGetCallback(i) == &l_ifc_aux_LIN_1)
            {
                retValue = CYRET_SUCCESS;
                break;
            }
        }

        /* Look for unused callback slot to fill it with own callback */
        if(CYRET_SUCCESS != retValue)
        {
            for(i = 0u; i < CY_SYS_SYST_NUM_OF_CALLBACKS; i++)
            {
                /* Make sure that callback slot is not used */
                if(CySysTickGetCallback(i) == (void *) 0 )
                {
                    (void)CySysTickSetCallback(i, &l_ifc_aux_LIN_1);
                    retValue = CYRET_SUCCESS;
                    break;
                }
            }
        }
    }
    return (retValue);
}


/*******************************************************************************
* Function Name: LIN_1_TimerDisableInt
********************************************************************************
*
* Summary:
*  Disables Systick timer interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Stops the SysTick timer if there are no active callback slots left and
*  disable the SysTick interrupt.
*
*******************************************************************************/
void LIN_1_TimerDisableInt(void)
{
    uint32 i;
    uint32 nullPointers = 0u;

    /* Find used callback slot */
    for(i = 0u; i < CY_SYS_SYST_NUM_OF_CALLBACKS; i++)
    {
        if(CySysTickGetCallback(i) == &l_ifc_aux_LIN_1)
        {
            /* Free callback */
            (void)CySysTickSetCallback(i, (void *) 0 );
            nullPointers++;
        }
        else if(CySysTickGetCallback(i) == (void *) 0 )
        {
            /* This callback slot is unused */
            nullPointers++;
        }
        else
        {
            /* This callback slot is used by some function */
        }
    }

    if(nullPointers == CY_SYS_SYST_NUM_OF_CALLBACKS)
    {
        /* If there are no used callback slots - disable SysTick timer */
        CySysTickDisableInterrupt();
        CySysTickStop();
    }
}

/* [] END OF FILE */
