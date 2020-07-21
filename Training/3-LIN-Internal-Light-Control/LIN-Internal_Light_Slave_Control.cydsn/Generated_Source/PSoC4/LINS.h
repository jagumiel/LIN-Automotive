/******************************************************************************
* File Name: LINS.h
* Version 5.0
*
* Description:
*  This file provides constants and parameter values for the LIN Slave Component.
*
*******************************************************************************
* Copyright 2011-2019, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
******************************************************************************/


#if !defined(CY_LIN_LINS_H)
#define CY_LIN_LINS_H

#include "cyfitter.h"
#include "CyLib.h"
#include "cytypes.h"

#include "LINS_SCB.h"
#include "LINS_SCB_SPI_UART.h"
#include "LINS_IntClk.h"
#include "LINS_ISR.h"

#if !defined LIN_API_TYPES
#define LIN_API_TYPES
/******************************************************************************
*                        LIN Core Types 
******************************************************************************/
    typedef unsigned char  l_bool;
    typedef unsigned char  l_u8;
    typedef unsigned short l_u16;
    typedef unsigned char  l_irqmask;
    typedef unsigned char  l_ioctl_op;
    typedef unsigned char  l_signal_handle;
    typedef unsigned char  l_ifc_handle;
    typedef unsigned char  l_flag_handle;
#endif /*!defined LIN_API_TYPES */

/******************************************************************************
*                 Conditional Compilation Parameters
******************************************************************************/

/* General */
#define LINS_RESPONSE_ERROR_SIGNAL  (1u)
#define LINS_ISO_17987              (1u)
#define LINS_SAE_J2602              (0u)
#define LINS_LIN_2_2                (1u)
#define LINS_LIN_2_0                (0u)
#define LINS_LIN_1_3                (0u)

#define LINS_SAE_J2602_ENABLED      (0u != LINS_SAE_J2602)

#define LINS_INACTIVITY_ENABLED     (1u)
#define LINS_INACTIVITY_THRESHOLD   (7000u)

/*  LDF/NCF file version for ISO_17987 */
#if (0 != LINS_ISO_17987)
    #define LINS_FILE_REVISION_major    (0u)
    #define LINS_FILE_REVISION_minor    (0u)
    #define LINS_FILE_REVISION_sub      (0u)
    #define LINS_FILE_REVISION_source   (0u)
    #define LINS_FILE_REVISION_source_LDF (1u)
    #define LINS_FILE_REVISION_source_NCF (2u)
    #define LINS_FILE_REVISION_DEFINED  ((LINS_FILE_REVISION_source ==  LINS_FILE_REVISION_source_LDF) || (LINS_FILE_REVISION_source == LINS_FILE_REVISION_source_NCF))
#endif

/* Break threshold detection value assuming 16x oversampling */
#define LINS_BREAK_THRESHOLD        (11u)

/* Baud rate */
#define LINS_AUTO_BAUD_RATE_SYNC    (1u)
#define LINS_BAUD_RATE              (19200u)
#if (0u == LINS_AUTO_BAUD_RATE_SYNC)    /* Auto Baud Rate Sync Disabled */
/* BREAK to SYNC timeout is 21 bits from break detection +1 ms CySysTick */
    #define LINS_BREAK_TO_SYNC_TIMEOUT  (l_u16)((21000u/LINS_BAUD_RATE)+1u) 
#endif  /* (1u == LINS_AUTO_BAUD_RATE_SYNC) */

/* Transport Layer */
#define LINS_TL_ENABLED             (1u)
#define LINS_TL_INITIAL_NAD         (1u)
#define LINS_TL_API_FORMAT          (1u)
#define LINS_TL_BUF_LEN_MAX         (512u)
#define LINS_TL_TX_QUEUE_LEN        (32u)
#define LINS_TL_RX_QUEUE_LEN        (32u)

#define LINS_TL_USED                (1u == LINS_TL_ENABLED)
#define LINS_COOKED_TL_API          (1u == LINS_TL_API_FORMAT)
#define LINS_RAW_TL_API             (0u == LINS_TL_API_FORMAT)

/* Config. Services */
#define LINS_CS_ENABLED             (1u)
#define LINS_CS_SUPPLIER_ID         (32766u)
#define LINS_CS_FUNCTION_ID         (65534u)
#define LINS_CS_VARIANT             (0u)
#define LINS_CS_SEL_SERVICES01      (132u)

#define LINS_CFG_SERVICES_ENABLED   (1u == LINS_CS_ENABLED)


#define LINS_ABS(x)(( (x) < 0 ) ? -(x) : (x) )


/* General frames and signals information */
#define LINS_NUM_FRAMES            (0x02u)
#define LINS_NUM_ET_FRAMES         (0x00u)
#define LINS_NUM_SIGNALS           (0x05u)
#define LINS_NUM_UNIQUE_SIGNALS    (0x05u)
#define LINS_RESPONSE_ERROR_FRAME_INDEX    (0x01u)


#define LINS_NUM_SERVICE_FRAMES     (0x02u)     /* MRF and SRF */

#define LINS_SIG_FRAME_FLAGS_SIZE   (((LINS_NUM_FRAMES         + \
                                                   LINS_NUM_SIGNALS        + \
                                                   LINS_NUM_UNIQUE_SIGNALS + \
                                                   LINS_NUM_SERVICE_FRAMES) / 8u) + 1u)

#define LINS_ET_FRAMES_FLAGS_SIZE   (((LINS_NUM_ET_FRAMES) / 8u) + 1u)

#define LINS_INACTIVITY_THRESHOLD_IN_100_MS  (LINS_INACTIVITY_THRESHOLD / 100u)
#define LINS_INACTIVITY_THRESHOLD_IN_1_MS    (LINS_INACTIVITY_THRESHOLD )

#if (LINS_TL_USED && LINS_CFG_SERVICES_ENABLED)
/* Bit masks for selected services */
    #define LINS_NCS_0xB0_SEL   (0x01u)
    #define LINS_NCS_0xB1_SEL   (0x02u)
    #define LINS_NCS_0xB2_SEL   (0x04u)
    #define LINS_NCS_0xB3_SEL   (0x08u)
    #define LINS_NCS_0xB4_SEL   (0x10u)
    #define LINS_NCS_0xB5_SEL   (0x20u)
    #define LINS_NCS_0xB6_SEL   (0x40u)
    #define LINS_NCS_0xB7_SEL   (0x80u)
/* Macro for simple check of selected services */
    #define LINS_NCS_0xB0_ENABLED (0 != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB0_SEL))
    #define LINS_NCS_0xB1_ENABLED (0 != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB1_SEL))
    #define LINS_NCS_0xB2_ENABLED (0 != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB2_SEL))
    #define LINS_NCS_0xB3_ENABLED (0 != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB3_SEL))
    #define LINS_NCS_0xB4_ENABLED (0 != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB4_SEL))
    #define LINS_NCS_0xB5_ENABLED (0 != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB5_SEL))
    #define LINS_NCS_0xB6_ENABLED (0 != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB6_SEL))
    #define LINS_NCS_0xB7_ENABLED (0 != (LINS_CS_SEL_SERVICES01 & LINS_NCS_0xB7_SEL))
#endif /* (LINS_TL_USED && LINS_CFG_SERVICES_ENABLED) */


/******************************************************************************
*     Data Types Definitions
******************************************************************************/

/* Detailed PID information */
typedef struct
{
    l_u8   param;
    volatile l_u8 *dataPtr;
    /* Unconditional PID associated if this is the Event Frame  */
    volatile l_u8 associatedPID;

} LINS_PID_INFO_TABLE;

/* LIN Slave Configuration */
typedef struct
{
    #if (LINS_TL_USED)
        /* Initial NAD */
        l_u8 initialNad;
    #endif /* LINS_TL_USED */

    /* PID Table */
    l_u8 pidTable[LINS_NUM_FRAMES];
 } LINS_SLAVE_CONFIG;
 
typedef struct
{   
/* Used as the second parameter for l_ifc_ioctl_LINS() */
    l_u8 index;
    l_u8 PID;
} LINS_NEW_PID; 

typedef struct
{   
/* Used as the second parameter for l_ifc_ioctl_LINS() */
    l_u16 messageID;
    l_u8 PID;
} LINS_NEW_PID_BY_MSG_ID; 

#if LINS_CFG_SERVICES_ENABLED
    /* LIN Slave Identification */
    typedef struct
    {
        l_u16 supplierId;
        l_u16 functionId;
        l_u8  variant;
    } LINS_SLAVE_ID;
#endif /* LINS_CFG_SERVICES_ENABLED */

/* Sleep Mode API Support */
typedef struct
{
    l_u8 enableState;
    l_u8 control;
    l_u8 statusMask;

    #if (1u == LINS_INACTIVITY_ENABLED)
        l_u8 inactivityDiv0;
        l_u8 inactivityDiv1;
    #endif  /* (1u == LINS_INACTIVITY_ENABLED) */

} LINS_BACKUP_STRUCT;


/******************************************************************************
*  Function Prototypes
******************************************************************************/

cystatus LINS_TimerEnableInt(void);
void     LINS_TimerDisableInt(void);
l_bool l_sys_init(void);
l_bool LINS_Start(void);
void   LINS_Stop(void);
#if (LINS_SAE_J2602_ENABLED)
    void LINS_j2602Status_Reset(void);
#endif  /* LINS_SAE_J2602_ENABLED */

/* Core API: Signal Interaction  Functions */
l_u8 l_u8_rd_InSig(void);
l_u8 l_u8_rd_InArraySig(void);
l_u8 l_u8_rd_LINS(l_signal_handle sss);

void l_bool_wr_Response_Error(l_bool v);
void l_bool_wr_LINS(l_signal_handle sss, l_bool v);
void l_u8_wr_OutSig(l_u8 v);
void l_u8_wr_OutArraySig(l_u8 v);
void l_u8_wr_LINS(l_signal_handle sss, l_u8 v);

/* Signals API for current instance definition */
#define  L_BOOL_RD_INST1     (0x00u)
#define  L_U8_RD_INST1       (0x01u)
#define  L_U16_RD_INST1      (0x00u)
#define  L_BYTES_RD_INST1    (0x00u)
#define  L_BOOL_WR_INST1     (0x01u)
#define  L_U8_WR_INST1       (0x01u)
#define  L_U16_WR_INST1      (0x00u)
#define  L_BYTES_WR_INST1    (0x00u)

/* Signals API for second instance definition */
#define  L_BOOL_RD_INST2     (0x00u)
#define  L_U8_RD_INST2       (0x00u)
#define  L_U16_RD_INST2      (0x00u)
#define  L_BYTES_RD_INST2    (0x00u)
#define  L_BOOL_WR_INST2     (0x00u)
#define  L_U8_WR_INST2       (0x00u)
#define  L_U16_WR_INST2      (0x00u)
#define  L_BYTES_WR_INST2    (0x00u)

/* Core API Functions: Notification */
l_bool l_flg_tst_MRF(void);
l_bool l_flg_tst_SRF(void);
l_bool l_flg_tst_InSig(void);
l_bool l_flg_tst_InArraySig(void);
l_bool l_flg_tst_OutSig(void);
l_bool l_flg_tst_OutArraySig(void);
l_bool l_flg_tst_InFrame(void);
l_bool l_flg_tst_OutFrame(void);
l_bool l_flg_tst_LINS(l_flag_handle fff);

void l_flg_clr_MRF(void);
void l_flg_clr_SRF(void);
void   l_flg_clr_InSig(void);
void   l_flg_clr_InArraySig(void);
void   l_flg_clr_OutSig(void);
void   l_flg_clr_OutArraySig(void);
void   l_flg_clr_InFrame(void);
void   l_flg_clr_OutFrame(void);
void l_flg_clr_LINS(l_flag_handle fff);


l_bool l_ifc_init_LINS(void);
void   l_ifc_wake_up_LINS(void);
l_u16  l_ifc_ioctl_LINS(l_ioctl_op op, void* pv);
void   l_ifc_rx_LINS(void);
void   l_ifc_tx_LINS(void);
void   l_ifc_aux_LINS(void);
l_u16  l_ifc_read_status_LINS(void);

#if (1u == LINS_LIN_1_3)
    l_bool l_ifc_connect_LINS(void);
    l_bool l_ifc_disconnect_LINS(void);
#endif /* (1u == LINS_LIN_1_3) */

l_u8 ld_read_by_id_callout_LINS(l_u8 id, l_u8* frameData);

/* Transport Layer Functions: Initialization */
void ld_init_LINS(void);

/* Transport Layer Functions: Node Configuration Functions */
l_u8 ld_read_configuration_LINS(l_u8* pData, l_u8* const length);
l_u8 ld_set_configuration_LINS(const l_u8* const pData, l_u16 length);

/* Transport Layer Functions: Cooked Transport Layer API */
void ld_send_message_LINS(l_u16 length, l_u8 nad, const l_u8* const ld_data);
void ld_receive_message_LINS(l_u16* const length, l_u8* const nad, l_u8* const ld_data);
l_u8 ld_tx_status_LINS(void);
l_u8 ld_rx_status_LINS(void);

/* Transport Layer Functions: Raw Transport Layer API */
void ld_put_raw_LINS(const l_u8* const ld_data);
void ld_get_raw_LINS(l_u8* const ld_data);
l_u8 ld_raw_tx_status_LINS(void);
l_u8 ld_raw_rx_status_LINS(void);

/* ISR Prototypes */
CY_ISR_PROTO(LINS_UART_ISR);

/* Swap U16 macro */
#define LINS_SWAP_U8_TO_U16(x,y)    (l_u16) ((l_u16) (((l_u16) (x)) << 8u) | (l_u16) (y))


/******************************************************************************
*   API Constants
******************************************************************************/
/* Constants for l_u8_rd_InSig */
#define LINS_InSig_InFrame_SIG_BYTE_OFFSET    (0x00u)
#define LINS_InSig_InFrame_SIG_MASK_0         (0x7Fu)

/* Constants for l_u8_rd_InArraySig */
#define LINS_InArraySig_InFrame_SIG_BYTE_OFFSET    (0x01u)

/* Constants for l_u8_wr_OutSig() */
#define LINS_OutSig_OutFrame_SIG_BYTE_OFFSET    (0x00u)
#define LINS_OutSig_OutFrame_SIG_MASK_0         (0x7Fu)

/* Constants for l_bool_wr_Response_Error() */
#define LINS_Response_Error_OutFrame_SIG_BYTE_OFFSET    (0x00u)
#define LINS_Response_Error_OutFrame_SIG_BIT_OFFSET     (0x07u)
#define LINS_Response_Error_OutFrame_SIG_MASK_0         (0x80u)

/* Constants for l_u8_wr_OutArraySig() */
#define LINS_OutArraySig_OutFrame_SIG_BYTE_OFFSET    (0x01u)


/* l_flg_tst_MRF() and l_flg_clr_MRF() */
#define LINS_MRF_FRAME_FLAG_BYTE_OFFSET_0           (0x00u)
#define LINS_MRF_FRAME_FLAG_MASK_0                  (0x01u)

/* l_flg_tst_SRF() and l_flg_clr_SRF() */
#define LINS_SRF_FRAME_FLAG_BYTE_OFFSET_0           (0x00u)
#define LINS_SRF_FRAME_FLAG_MASK_0                  (0x02u)


/* l_flg_tst_InSig() and l_flg_clr_InSig() */
#define LINS_InSig_FRAME_FLAG_BYTE_OFFSET_0    (0x00u)
#define LINS_InSig_FRAME_FLAG_MASK_0           (0x04u)

/* l_flg_tst_InArraySig() and l_flg_clr_InArraySig() */
#define LINS_InArraySig_FRAME_FLAG_BYTE_OFFSET_0    (0x00u)
#define LINS_InArraySig_FRAME_FLAG_MASK_0           (0x08u)

/* l_flg_tst_OutSig() and l_flg_clr_OutSig() */
#define LINS_OutSig_FRAME_FLAG_BYTE_OFFSET_0    (0x00u)
#define LINS_OutSig_FRAME_FLAG_MASK_0           (0x10u)

/* l_flg_tst_OutArraySig() and l_flg_clr_OutArraySig() */
#define LINS_OutArraySig_FRAME_FLAG_BYTE_OFFSET_0    (0x00u)
#define LINS_OutArraySig_FRAME_FLAG_MASK_0           (0x20u)

/* l_flg_tst_InFrame() and l_flg_clr_InFrame() */
#define LINS_InFrame_FRAME_FLAG_BYTE_OFFSET_0    (0x00u)
#define LINS_InFrame_FRAME_FLAG_MASK_0           (0x40u)

/* l_flg_tst_OutFrame() and l_flg_clr_OutFrame() */
#define LINS_OutFrame_FRAME_FLAG_BYTE_OFFSET_0    (0x00u)
#define LINS_OutFrame_FRAME_FLAG_MASK_0           (0x80u)



#define InSig_SIGNAL_HANDLE        (0x00u)
#define InArraySig_SIGNAL_HANDLE        (0x01u)
#define OutSig_SIGNAL_HANDLE        (0x02u)
#define Response_Error_SIGNAL_HANDLE        (0x03u)
#define OutArraySig_SIGNAL_HANDLE        (0x04u)
#define MRF_SIGNAL_HANDLE        (0x05u)
#define SRF_SIGNAL_HANDLE        (0x06u)


#define MRF_FLAG_HANDLE        (0x00u)
#define SRF_FLAG_HANDLE        (0x01u)
#define InSig_FLAG_HANDLE        (0x02u)
#define InArraySig_FLAG_HANDLE        (0x03u)
#define OutSig_FLAG_HANDLE        (0x04u)
#define OutArraySig_FLAG_HANDLE        (0x05u)
#define InFrame_FLAG_HANDLE        (0x06u)
#define OutFrame_FLAG_HANDLE        (0x07u)


#define LINS_ISR_AUX_SET_FLAGS_InFrame_FLAG_BYTE_OFFSET_0        (0x00u)
#define LINS_ISR_AUX_SET_FLAGS_InFrame_FLAG_MASK_0        (0x4Cu)
#define LINS_ISR_AUX_SET_FLAGS_OutFrame_FLAG_BYTE_OFFSET_0        (0x00u)
#define LINS_ISR_AUX_SET_FLAGS_OutFrame_FLAG_MASK_0        (0xB0u)




/* 8 bits with 16x oversampling rate */
#define LINS_EXPECTED_TIME_COUNTS       (128u)
#if (1u == LINS_AUTO_BAUD_RATE_SYNC)    /* Auto Baud Rate Sync Enabled */
    #define LINS_TIME_COUNTS_MAX_DEVIATION  (0x13u)
#else                                                /* Auto Baud Rate Sync Disabled */
    #define LINS_TIME_COUNTS_MAX_DEVIATION  (0x03u)
#endif    
#define LINS_BR_BASE_SHIFT              (7u)

/* Bus inactivity block reconfiguration */
#if (1u == LINS_INACTIVITY_ENABLED)
    /* Bus inactivity block configuration to issue interrupt */
    #define LINS_INACT_100MS_IN_S       (10u)
    #define LINS_INACT_OVERSAMPLE_RATE  (16u)
    #define LINS_INACT_DIVIDE_FACTOR    (256u)

    #define LINS_INACT_TIME_FACTOR      ((l_u16) (LINS_INACT_OVERSAMPLE_RATE * \
                                (LINS_BAUD_RATE / LINS_INACT_100MS_IN_S)))

    /* Divider 1 for specified interrupt rate */
    #define LINS_INACT_DIV1             (HI8(LINS_INACT_TIME_FACTOR))

    /* Divider 0 for specified interrupt rate */
    #define LINS_INACT_DIV0             (LO8(LINS_INACT_TIME_FACTOR))
#endif /* (1u == LINS_INACTIVITY_ENABLED) */

#define LINS_FALSE                      (0x00u)
#define LINS_TRUE                       (0x01u)
#define LINS_ALL_IRQ_ENABLE             (0x03u)
#define LINS_FRAME_PID_MRF              (0x3Cu)
#define LINS_FRAME_PID_SRF              (0x7Du)
#define LINS_FRAME_PID_BROADCAST_MIN    (0x38u)
#define LINS_FRAME_PID_BROADCAST_MAX    (0x3Bu)
#define LINS_FRAME_PID_UNASSIGNED       (0x40u)
#define LINS_MESSAGE_ID_UNASSIGNED    (0xFFFFu)
#define LINS_FRAME_SYNC_BYTE            (0x55u)
#define LINS_ID_16                      (16u)
#define LINS_ID_31                      (31u)

#if LINS_SAE_J2602_ENABLED
    #define LINS_FRAME_PID_MRF_J2602    (0xFEu)
#endif /* LINS_SAE_J2602_ENABLED */

/* Numbers and priorities of bLIN and UART interrupts */
#define LINS_BLIN_ISR_NUMBER            (LINS_BLIN_ISR__INTC_NUMBER)
#define LINS_BLIN_ISR_PRIORITY          (LINS_BLIN_ISR__INTC_PRIOR_NUM)
#define LINS_UART_ISR_NUMBER            (LINS_UART_ISR__INTC_NUMBER)
#define LINS_UART_ISR_PRIORITY          (LINS_UART_ISR__INTC_PRIOR_NUM)

/* Wakeup signal length in us */
#define LINS_WAKE_UP_SIGNAL_LENGTH      (300u)
/* Wakeup fake symbol length in ms */
#define LINS_WAKEUP_TX_DELAY            (11u)

#define LINS_INVALID_FRAME_PID          (0xFFu)

/* Used to clear UART Rx FIFO */
#define LINS_UART_RX_FIFO_CLEAR         (0x01u)


/******************************************************************************
*                       UART State Machine                                    *
******************************************************************************/

/* Sync Field detection(UDB) / IDLE state (SCB) */
#define LINS_UART_ISR_STATE_0_IDLE      (0x00u)

/* Receive PID was detected. Analyze recieved PID and determine action */
#define LINS_UART_ISR_STATE_1_PID       (0x01u)

/* Transmit data and checksum byte to the LIN master */
#define LINS_UART_ISR_STATE_2_TX        (0x02u)

/* Read data from the LIN master */
#define LINS_UART_ISR_STATE_3_RX        (0x03u)

/* Checksum verification */
#define LINS_UART_ISR_STATE_4_CHS       (0x04u)

/* PID Parity Error Mask */
#define LINS_PID_PARITY_MASK            (0x3Fu)


/******************************************************************************
*                     LINS_PID_INFO_TABLE                         *
******************************************************************************/
#define LINS_FRAME_DATA_SIZE_1          (0x01u)
#define LINS_FRAME_DATA_SIZE_2          (0x02u)
#define LINS_FRAME_DATA_SIZE_3          (0x03u)
#define LINS_FRAME_DATA_SIZE_4          (0x04u)
#define LINS_FRAME_DATA_SIZE_5          (0x05u)
#define LINS_FRAME_DATA_SIZE_6          (0x06u)
#define LINS_FRAME_DATA_SIZE_7          (0x07u)
#define LINS_FRAME_DATA_SIZE_8          (0x08u)

/* LIN slave direction = subscribe */
#define LINS_FRAME_DIR_SUBSCRIBE        (0x00u)

/* LIN slave direction = publish */
#define LINS_FRAME_DIR_PUBLISH          (0x10u)

/* LIN frame type = event-triggered */
#define LINS_FRAME_TYPE_EVENT           (0x20u)

/* LIN frame type = unconditional */
#define LINS_FRAME_TYPE_UNCOND          (0x00u)

#define LINS_FRAME_DATA_SIZE_MASK       (0x0Fu)
#define LINS_FRAME_DIR_MASK             (0xEFu)
#define LINS_FRAME_TYPE_MASK            (0xDEu)


/******************************************************************************
*                                Interface Status
*                          LINS_ifcStatus
******************************************************************************/
/* Error in response */
#define LINS_IFC_STS_ERROR_IN_RESPONSE  (0x0001u)

/* Successful frame transfer */
#define LINS_IFC_STS_SUCCESSFUL_TRANSFER (0x0002u)

/* Overrun */
#define LINS_IFC_STS_OVERRUN            (0x0004u)

/* Go to sleep */
#define LINS_IFC_STS_GO_TO_SLEEP        (0x0008u)

/* Bus activity */
#define LINS_IFC_STS_BUS_ACTIVITY       (0x0010u)

/* Event-triggered frame collision */
#define LINS_IFC_STS_EVTRIG_COLLISION   (0x0020u)

/* Save configuration */
#define LINS_IFC_STS_SAVE_CONFIG        (0x0040u)

/* Last frame PID mask */
#define LINS_IFC_STS_PID_MASK           (0xFF00u)

/* Status mask */
#define LINS_IFC_STS_MASK               (0xFFFFu)


/******************************************************************************
*                          Internal LIN Slave Status
*                           LINS_status
******************************************************************************/

/* This bit indicates that there is a response for ACRH service
*  and it is ready to be sent to Master node.
*/
#define LINS_STATUS_SRVC_RSP_RDY        (0x80u)
#define LINS_STATUS_ERROR_MIRROR        (0x01u)
#define LINS_STATUS_NOT_CHECKED         (0x04u)
#define LINS_STATUS_RESPONSE_ACTIVE     (0x10u)
/* Indicates that a response pending frame (NRC 0x78 code) was issued. */
#define LINS_STATUS_RESPONSE_PENDING    (0x20u)


#if LINS_SAE_J2602_ENABLED
    /***************************************************************************
    *  SAE J2602-1 Status codes [ERR2:ERR0] for
    * LINS_j2602Status[7:5]
    * used with: 
    *     LINS_j2602Status_Set()
    *     LINS_j2602Status_Get()
    *     LINS_j2602Status_Clear()
    ***************************************************************************/

    /* Data error */
    #define LINS_J2602_STS_RESET            (1u)
    
    /* Data error */
    #define LINS_J2602_STS_DATA_ERR         (4u)

    /* Checksum error  */
    #define LINS_J2602_STS_CHECKSUM_ERR     (5u)

    /* Framing error  */
    #define LINS_J2602_STS_FRAMING_ERR      (6u)

    /* Parity error  */
    #define LINS_J2602_STS_PARITY_ERR       (7u)
    
    /* APINFO mask  */
    #define LINS_J2602_STS_APINFO_MASK   (0x1Fu)    
    
    /***************************************************************************
    * Internal AUX ISR shadow status  
    *  used for 
    * LINS_auxStatus; 
    ***************************************************************************/
    /* Previous MRF requires J2602_Status in SRF (J2602-1 5.7.2.4)*/
    #define LINS_AUXSTS_MRF_REQUIRES_J2602STATUS  (1u)
    /* Previous MRF ID==0x3E (PID==FE) */
    #define LINS_AUXSTS_MRF_WAS_3E                (2u)
    
    
#endif /* LINS_SAE_J2602_ENABLED */


/******************************************************************************
*                       LINS_EndFrame(l_u16 status)
******************************************************************************/
#define LINS_HANDLING_RESET_FSM_ERR     (0x01u)
#define LINS_HANDLING_DONT_SAVE_PID     (0x10u)
#define LINS_HANDLING_SKIP_OVERRUN      (0x20u)
#define LINS_HANDLING_PID_ERR           (0x40u)


/******************************************************************************
*                           LINS_fsmFlags
******************************************************************************/

/* Break signal detected  */
#define LINS_FSM_BREAK_FLAG             (0x01u)

/* Sleep API was called */
#define LINS_FSM_SLEEP_API_FLAG         (0x02u)

/* UART receives at least 1 byte of data */
#define LINS_FSM_DATA_RECEIVE           (0x04u)

/* UART enable flag */
#define LINS_FSM_UART_ENABLE_FLAG       (0x08u)

/* Overrun flag is used for status word */
#define LINS_FSM_OVERRUN                (0x40u)

/* Framing error */
#define LINS_FSM_FRAMING_ERROR_FLAG     (0x80u)


/******************************************************************************
*                           Transport Layer
******************************************************************************/
#if LINS_CFG_SERVICES_ENABLED
    /* Wildcard ID understandable for every slave node */
    #define LINS_CS_SUPPLIER_ID_WILDCARD    (0x7FFFu)
    #define LINS_CS_FUNCTION_ID_WILDCARD    (0xFFFFu)

    #define LINS_CS_BYTE_SUPPLIER_ID1       (0x01u)
    #define LINS_CS_BYTE_SUPPLIER_ID2       (0x02u)
    #define LINS_CS_BYTE_FUNCTION_ID1       (0x03u)
    #define LINS_CS_BYTE_FUNCTION_ID2       (0x04u)
    #define LINS_CS_BYTE_VARIANT            (0x05u)
#endif /* LINS_CFG_SERVICES_ENABLED */

/* Specifies the value for the TL timeouts  */
#define LINS_TL_N_AS_TIMEOUT_VALUE      (1000u)
#define LINS_TL_N_CR_TIMEOUT_VALUE      (1000u)
#define LINS_TL_P2_TIMEOUT_VALUE        (500u)

/* Specifies the Frame buffer length for Transport Layer */
#define LINS_FRAME_BUFF_LEN             (8u)

/* Specifies the Frame length for Transport Layer */
#define LINS_FRAME_LEN                  (8u)

/* Packet Data Unit (PDU) Offsets */
#define LINS_PDU_NAD_IDX                (0u)
#define LINS_PDU_PCI_IDX                (1u)
#define LINS_PDU_SID_IDX                (2u)
#define LINS_PDU_LEN_IDX                (2u)
#define LINS_PDU_D1_IDX                 (3u)
#define LINS_PDU_D1_START_IDX           (3u)
#define LINS_PDU_D1_ID_IDX              (3u)
#define LINS_PDU_D2_IDX                 (4u)
#define LINS_PDU_D2_PID_IDX             (4u)
#define LINS_PDU_D2_BYTE_IDX            (4u)
#define LINS_PDU_D3_IDX                 (5u)
#define LINS_PDU_D3_MASK_IDX            (5u)
#define LINS_PDU_D4_IDX                 (6u)
#define LINS_PDU_D4_INVERT_IDX          (6u)
#define LINS_PDU_D5_IDX                 (7u)
#define LINS_PDU_D5_NEW_NAD_IDX         (7u)

/* Single Frame data length  */
#define LINS_PDU_SF_DATA_LEN            (6u)

/* Protocol Control Information (PCI) Types */
#define LINS_PDU_PCI_TYPE_SF            (0x00u)     /* Single Frame */
#define LINS_PDU_PCI_TYPE_FF            (0x10u)     /* First Frame */
#define LINS_PDU_PCI_TYPE_CF            (0x20u)     /* Consecutive Frame */
#define LINS_PDU_PCI_TYPE_UNKNOWN       (0xFFu)
#define LINS_PDU_PCI_TYPE_MASK          (0xF0u)
#define LINS_PDU_PCI_LENGTH_MASK        (0x0Fu)

#define LINS_NAD_J2602_BASE             (0x60u)
#define LINS_NAD_MULTIPLE2_MASK         (0x01u)
#define LINS_NAD_MULTIPLE4_MASK         (0x03u)
#define LINS_NAD_MULTIPLE8_MASK         (0x07u)
#define LINS_NAD_DNN_MASK               (0x0Fu)
#define LINS_NAD_UNINITIALIZED          (0x6Fu)
#define LINS_NAD_FUNCTIONAL             (0x7Eu)

/* Wildcard NAD */
#define LINS_NAD_BROADCAST              (0x7Fu)

/* Go to sleep command ID */
#define LINS_NAD_GO_TO_SLEEP            (0x00u)

/* Max and min Config Services IDs */
#define LINS_SID_CONF_MIN               (0xB0u)
#define LINS_SID_CONF_MAX               (0xB7u)

/* Node Configuration Services */
#define LINS_NCS_ASSIGN_NAD             (0xB0u)
#define LINS_NCS_ASSIGN_FRAME_ID        (0xB1u) /* Used only in LIN 2.0 */
#define LINS_NCS_READ_BY_ID             (0xB2u)
#define LINS_NCS_COND_CHANGE_NAD        (0xB3u)
#define LINS_NCS_DATA_DUMP              (0xB4u) /* Not supported */
#define LINS_NCS_ASSIGN_NAD_SNPD        (0xB5u) /* Not supported */
#define LINS_NCS_SAVE_CONFIG            (0xB6u)
#define LINS_NCS_ASSIGN_FRAME_ID_RANGE  (0xB7u)

/* Used with tempStatusErr to Return Negative response for Config Service */
#define LINS_NCS_NEGATIVE_RESPONSE_REQUIRED      (0x01u)

/* Positive responses for Node Configuration Services requests */
#define LINS_NCS_POS_RESP_ASSIGN_NAD             (0xF0u)
#define LINS_NCS_POS_RESP_ASSIGN_FRAME_ID        (0xF1u) /* Used only in LIN 2.0 */
#define LINS_NCS_POS_RESP_READ_BY_ID             (0xF2u)
#define LINS_NCS_POS_RESP_COND_CHANGE_NAD        (0xF3u)
#define LINS_NCS_POS_RESP_DATA_DUMP              (0xF4u) /* Not supported */
#define LINS_NCS_POS_RESP_ASSIGN_NAD_SNPD        (0xF5u) /* Not supported */
#define LINS_NCS_POS_RESP_SAVE_CONFIG            (0xF6u)
#define LINS_NCS_POS_RESP_ASSIGN_FRAME_ID_RANGE  (0xF7u)

/* Other LIN TL constants */
#define LINS_NCS_READ_BY_ID_ID          (0x00u)
#define LINS_NCS_READ_BY_ID_SERIAL      (0x01u)
#define LINS_NCS_READ_BY_ID_FILE_REVISION (0x03u)
#define LINS_NCS_RSID_NEG_REPLY         (0x7Fu)
#define LINS_NCS_MAX_FRAME_ID_RANGE     (0x04u)

/* ld_read_by_id_callout() return constants */
#define LD_NEGATIVE_RESPONSE                        (0x00u)
#define LD_NO_RESPONSE                              (0x01u)
#define LD_POSITIVE_RESPONSE                        (0x02u)

#define LD_INVALID_MESSAGE_INDEX                    (0xFFu)
#define LD_MESSAGE_ID_BASE                          (0x10u)


/******************************************************************************
*       LINS_txTlStatus and LINS_rxTlStatus
******************************************************************************/

/* Reception or transmission is not yet completed */
#define LD_IN_PROGRESS                              (0x01u)

/* Reception or transmission completed successfully */
#define LD_COMPLETED                                (0x02u)

/* Reception or transmission ended in an error */
#define LD_FAILED                                   (0x03u)

/* Transmission failed because of a N_As timeout */
#define LD_N_AS_TIMEOUT                             (0x04u)

/* Reception failed because of a N_Cr timeout */
#define LD_N_CR_TIMEOUT                             (0x05u)

/* Reception failed because of an unexpected sequence number */
#define LD_WRONG_SN                                 (0x06u)

/* The transmit queue is empty */
#define LD_QUEUE_EMPTY                              (0x07u)

/* Transmit queue contains entires */
#define LD_QUEUE_AVAILABLE                          (0x08u)

/* Transmit queue is full and cannott accept further frames. */
#define LD_QUEUE_FULL                               (0x09u)

/* LIN protocol errors occurred during transfer */
#define LD_TRANSMIT_ERROR                           (0x0Au)

/* Receive queue is empty */
#define LD_NO_DATA                                  (0x0Bu)

/* Receive queue contains data that can be read */
#define LD_DATA_AVAILABLE                           (0x0Cu)

/* LIN protocol errors occurred during transfer */
#define LD_RECEIVE_ERROR                            (0x0Du)


/******************************************************************************
*                           LINS_tlFlags
******************************************************************************/

/* The requested service is disabled and diagnostic frame will
*  be "passed" to Transport Layer.
*/
#define LINS_TL_CS_SERVICE_DISABLED     (0x01u)

/* SID isn't ACRH but a simple diagnostic SID */
#define LINS_TL_DIAG_FRAME_DETECTED     (0x02u)

/* The last PID that occurred is SRF PID */
#define LINS_TL_TX_DIRECTION            (0x04u)

/* The last PID that occurred is MRF PID */
#define LINS_TL_RX_DIRECTION            (0x08u)

/* Indicates that Cooked API requested transmit data */
#define LINS_TL_TX_REQUESTED            (0x10u)

/* Indicates that Cooked API requested receive data */
#define LINS_TL_RX_REQUESTED            (0x20u)

/* Indicates that the N_AS timeout monitoring is in progress */
#define LINS_TL_N_AS_TIMEOUT_ON         (0x40u)

/* Indicates that the N_CR timeout monitoring is in progress */
#define LINS_TL_N_CR_TIMEOUT_ON         (0x80u)


/******************************************************************************
*            ld_read_configuration() and ld_set_configuration()
******************************************************************************/

/* Read configuration is complete */
#define LINS_LD_READ_OK                 (0x01u)

/* Read configuration failed. The configuration size is greater than the length. */
#define LINS_LD_LENGTH_TOO_SHORT        (0x02u)

/* Set configuration completed. */
#define LINS_LD_SET_OK                  (0x01u)

/* Set configuration failed.
*  The required configuration size is not equal to the given length.
*/
#define LINS_LD_LENGTH_NOT_CORRECT      (0x02u)

/* Set configuration is fail. Set conf could not be made. */
#define LINS_LD_DATA_ERROR              (0x04u)


/******************************************************************************
*            CheckNodeState() Parameters
******************************************************************************/

#define LINS_MRF_ALIEN_NAD              (0x01u)
#define LINS_MRF_OWN_NAD                (0x02u)
#define LINS_MRF_FUNC_NAD               (0x04u)
#define LINS_RX_TIMEOUT                 (0x08u)
#define LINS_TX_TIMEOUT                 (0x10u)
#define LINS_RX_ERROR                   (0x20u)
#define LINS_TX_ERROR                   (0x40u)
#define LINS_SRF                        (0x80u)


/******************************************************************************
*            CheckNodeState() Return Values
******************************************************************************/

#define LINS_NO_RESPONSE_REQUIRED       (0x00u)
#define LINS_RESPONSE_REQUIRED          (0x01u)
#define LINS_RECEIVE_CONTINUES          (0x02u)
#define LINS_TRANSMISSION_CONTINUES     (0x04u)
#define LINS_IGNORE_TL_FRAME            (0x08u)
#define LINS_ERRONEOUS_TL_FRAME         (0x10u)


/*******************************************************************************
*                         LINS_nodeState
*           Slave Node states as defined in part 5.5 of LIN2.2a Spec.
*******************************************************************************/

#define LINS_IDLE                       (0x00u)
#define LINS_RX_PHY_REQUEST             (0x01u)
#define LINS_TX_PHY_RESPONSE            (0x02u)


/******************************************************************************
*           l_ifc_ioctl() Parameters
******************************************************************************/
#define L_IOCTL_READ_STATUS                         (0x00u)
#define L_IOCTL_SET_BAUD_RATE                       (0x01u)
#define L_IOCTL_SLEEP                               (0x02u)
#define L_IOCTL_WAKEUP                              (0x03u)
#define L_IOCTL_SYNC_COUNTS                         (0x04u)
#define L_IOCTL_SET_SERIAL_NUMBER                   (0x05u)
#define L_IOCTL_GET_NAD                             (0x06u)
#define L_IOCTL_SET_NAD                             (0x07u)
#define L_IOCTL_GET_FRAME_PID                       (0x08u)
#define L_IOCTL_SET_FRAME_PID                       (0x09u)
#define L_IOCTL_SET_DNN                             (0x0Au)
#define L_IOCTL_SET_FRAME_PID_BY_MESSAGE_ID         (0x0Bu)


/******************************************************************************
*           Variable LINS_ioctlStatus flags
******************************************************************************/
/* Bus inactivity */
#define LINS_IOCTL_STS_BUS_INACTIVITY   (0x0001u)

/* Target reset (0xB5) */
#define LINS_IOCTL_STS_TARGET_RESET     (0x0002u)

/* Status Register */
#define LINS_STATUS_BREAK_DETECTED      (0x01u)
#define LINS_STATUS_EDGE_DETECTED       (0x02u)
#define LINS_STATUS_SYNC_COMPLETED      (0x08u)
#if (1u == LINS_INACTIVITY_ENABLED)
    #define LINS_STATUS_INACTIVITY_INT  (0x04u)
#endif  /* (1u == LINS_INACTIVITY_ENABLED) */

/* Control Register */
#define LINS_CONTROL_ENABLE             (0x01u)
#define LINS_CONTROL_TX_DIS             (0x02u)
#define LINS_CONTROL_RX_DIS             (0x04u)

/* Status Mask Register */
#define LINS_INT_MASK_BREAK             (0x01u)
#define LINS_INT_MASK_EDGE              (0x02u)
#define LINS_INT_MASK_SYNC              (0x08u)

#if (1u == LINS_INACTIVITY_ENABLED)
    #define LINS_INT_MASK_INACTIVITY    (0x04u)
#else
    #define LINS_INT_MASK_INACTIVITY    (0x00u)
#endif  /* (1u == LINS_INACTIVITY_ENABLED) */

#define LINS_STATUS_AUX_CONTROL_INT_EN  (0x10u)

#endif /* CY_LIN_LINS_H */

/* [] END OF FILE */
