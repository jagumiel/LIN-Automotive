/*******************************************************************************
* File Name: LIN_Dynamic.h
* Version 4.0
*
* Description:
*  This file contains implementation of dynamic part of LIN API.
*
********************************************************************************
* Copyright 2011-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_LIN_LIN_DYNAMIC_H)
#define CY_LIN_LIN_DYNAMIC_H

/* IsMultiInstanse = 0u for single instance
*  IsMultiInstanse = 1u for 1st and 2nd instance */
#define LIN_1_IS_MULTI_INSTANCE           (0u)

#include "LIN_1.h"

#if(LIN_1_IS_MULTI_INSTANCE)
        #if !defined(CY_LIN_NOT_DEFINED_H)
            #include "NOT_DEFINED.h"
        #endif /* (!defined(CY_LIN_NOT_DEFINED_H)) */
#endif /* (_IS_MULTI_INSTANCE) */

#if(1u == LIN_1_AUTO_BAUD_RATE_SYNC)

    /* Initial clock divider */
    extern l_u16 LIN_1_initialClockDivider;

#endif  /* (1u == LIN_1_AUTO_BAUD_RATE_SYNC) */

#if(LIN_1_IS_MULTI_INSTANCE)

     #if(1u == NOT_DEFINED_AUTO_BAUD_RATE_SYNC)

        extern l_u16 NOT_DEFINED_initialClockDivider;

    #endif  /* (1u == NOT_DEFINED_AUTO_BAUD_RATE_SYNC) */
#endif /* (LIN_1_IS_MULTI_INSTANCE) */

/* Interface handle constant  for  */
#define LIN_1_IFC_HANDLE                   (0u)

#if(LIN_1_IS_MULTI_INSTANCE)
    #define NOT_DEFINED_IFC_HANDLE               (1u)
#else
    #define NOT_DEFINED_TL_ENABLED               (0u)
    #define NOT_DEFINED_CS_ENABLED               (0u)
    #define NOT_DEFINED_TL_API_FORMAT            (0u)
    #define NOT_DEFINED_CS_SEL_SERVICES01        (0u)
    #define NOT_DEFINED_CS_SEL_SERVICES01        (0u)
    #define NOT_DEFINED_CS_SEL_SERVICES01        (0u)

    #define NOT_DEFINED_NCS_0xB0_SEL             (0x01u)
    #define NOT_DEFINED_NCS_0xB1_SEL             (0x02u)
    #define NOT_DEFINED_NCS_0xB2_SEL             (0x04u)
    #define NOT_DEFINED_NCS_0xB3_SEL             (0x08u)
    #define NOT_DEFINED_NCS_0xB4_SEL             (0x10u)
    #define NOT_DEFINED_NCS_0xB5_SEL             (0x20u)
    #define NOT_DEFINED_NCS_0xB6_SEL             (0x40u)
    #define NOT_DEFINED_NCS_0xB7_SEL             (0x80u)

#endif /* (_IS_MULTI_INSTANCE) */

l_bool l_ifc_init(l_ifc_handle iii);
void l_ifc_wake_up(l_ifc_handle iii);
l_u16 l_ifc_ioctl(l_ifc_handle iii, l_ioctl_op op, void* pv);
void l_ifc_rx(l_ifc_handle iii);
void l_ifc_tx(l_ifc_handle iii);
void l_ifc_aux(l_ifc_handle iii);
l_u16 l_ifc_read_status(l_ifc_handle iii);


#if(1u == LIN_1_LIN_1_3)
    l_bool l_ifc_connect(l_ifc_handle iii);
    l_bool l_ifc_disconnect(l_ifc_handle iii);
#else
    #if(LIN_1_IS_MULTI_INSTANCE)
        #if(1u == NOT_DEFINED_LIN_1_3)
            l_bool l_ifc_connect(l_ifc_handle iii);
            l_bool l_ifc_disconnect(l_ifc_handle iii);
        #endif /* ( 1u == NOT_DEFINED_LIN_1_3) */
    #endif /*(LIN_1_IS_MULTI_INSTANCE)*/
#endif /* ( 1u == LIN_1_LIN_1_3) */

void        ld_init(l_ifc_handle iii);
l_u8 ld_read_by_id_callout(l_ifc_handle iii, l_u8 id, l_u8* frameData);

/* Transport Layer Functions: Node Configuration Functions */
l_u8        ld_read_configuration(l_ifc_handle iii, l_u8* pData, l_u8* const length);
l_u8        ld_set_configuration(l_ifc_handle iii, const l_u8* const pData, l_u16 length);
void        ld_send_message(l_ifc_handle iii, l_u16 length, l_u8 nad, const l_u8* const ld_data);
void        ld_receive_message(l_ifc_handle iii, l_u16* const length, l_u8* const nad, l_u8* const ld_data);
l_u8        ld_tx_status(l_ifc_handle iii);
l_u8        ld_rx_status(l_ifc_handle iii);

void        ld_put_raw(l_ifc_handle iii, const l_u8* const ld_data);
void        ld_get_raw(l_ifc_handle iii, l_u8* const ld_data);
l_u8        ld_raw_tx_status(l_ifc_handle iii);
l_u8        ld_raw_rx_status(l_ifc_handle iii);


#if (L_BOOL_RD_INST1 || L_BOOL_RD_INST2)
    l_bool l_bool_rd(l_signal_handle sss);
#endif /*(L_BOOL_RD_INST1 || L_BOOL_RD_INST2)*/

#if (L_U8_RD_INST1 || L_U8_RD_INST2)
    l_u8 l_u8_rd(l_signal_handle sss);
#endif /*(L_U8_RD_INST1 || L_U8_RD_INST2)*/

#if (L_U16_RD_INST1 || L_U16_RD_INST2)
    l_u16 l_u16_rd(l_signal_handle sss);
#endif /*(L_U16_RD_INST1 || L_U16_RD_INST2)*/

#if (L_BYTES_RD_INST1 || L_BYTES_RD_INST2)
    void l_bytes_rd(l_signal_handle sss, l_u8 start, l_u8 count, l_u8* pData);
#endif /*(L_BYTES_RD_INST1 || L_BYTES_RD_INST2)*/

#if (L_BOOL_WR_INST1 || L_BOOL_WR_INST2)
    void l_bool_wr(l_signal_handle sss, l_bool v);
#endif /*(L_BOOL_WR_INST1 || L_BOOL_WR_INST2)*/

#if (L_U8_WR_INST1 || L_U8_WR_INST2)
    void l_u8_wr(l_signal_handle sss, l_u8 v);
#endif /*(L_U8_WR_INST1 || L_U8_WR_INST2)*/

#if (L_U16_WR_INST1 || L_U16_WR_INST2)
    void l_u16_wr(l_signal_handle sss, l_u16 v);
#endif /*(L_U16_WR_INST1 || L_U16_WR_INST2)*/

#if (L_BYTES_WR_INST1 || L_BYTES_WR_INST2)
    void l_bytes_wr(l_signal_handle sss, l_u8 start, l_u8 count, const l_u8* const pData);
#endif /*L_BYTES_WR_INST1 || L_BYTES_WR_INST2*/

void l_flg_clr(l_flag_handle fff);
l_bool l_flg_tst(l_flag_handle fff);


#endif /* CY_LIN_LIN_DYNAMIC_H */

/* [] END OF FILE */

