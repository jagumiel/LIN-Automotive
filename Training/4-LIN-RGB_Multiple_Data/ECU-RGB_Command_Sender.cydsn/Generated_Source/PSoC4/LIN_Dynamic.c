/*******************************************************************************
* File Name: LIN_Dynamic.c
* Version 4.0
*
* Description:
*  This file contains implementation of LIN Slave component.
*
********************************************************************************
* Copyright 2011-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LIN_Dynamic.h"

/*******************************************************************************
* Function Name: l_flg_tst
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
l_bool l_flg_tst(l_flag_handle fff)
{
    return (l_flg_tst_LIN_1(fff));
}


/*******************************************************************************
* Function Name: l_flg_clr
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
void l_flg_clr(l_flag_handle fff)
{
    l_flg_clr_LIN_1(fff);
}


#if (L_BOOL_RD_INST1 || L_BOOL_RD_INST2)
/*******************************************************************************
* Function Name: l_bool_rd
********************************************************************************
*
* Summary:
*  Reads and returns the current value of the signal for one bit signals.
*  If an invalid signal handle is passed into the function, no action is done. 
*
* Parameters:
*  sss - signal handle of the signal to read.
*
* Return:
*  Returns current value of signal. Returns non-zero value if unknown signal.
*
*******************************************************************************/
l_bool l_bool_rd(l_signal_handle sss) 
{
    return (l_bool_rd_LIN_1(sss));
}
#endif /*(L_BOOL_RD_INST1 || L_BOOL_RD_INST2)*/


#if (L_U8_RD_INST1 || L_U8_RD_INST2)
/*******************************************************************************
* Function Name: l_u8_rd
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
l_u8 l_u8_rd(l_signal_handle sss) 
{
    return (l_u8_rd_LIN_1(sss));
}
#endif /*(L_U8_RD_INST1 || L_U8_RD_INST2)*/


#if (L_U16_RD_INST1 || L_U16_RD_INST2)
/*******************************************************************************
* Function Name: l_u16_rd
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
l_u16 l_u16_rd(l_signal_handle sss) 
{
    return (l_u16_rd_LIN_1(sss));
}
#endif /*(L_U16_RD_INST1 || L_U16_RD_INST2)*/


#if (L_BYTES_RD_INST1 || L_BYTES_RD_INST2)
/*******************************************************************************
* Function Name: l_bytes_rd
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
void l_bytes_rd(l_signal_handle sss, l_u8 start, l_u8 count, l_u8* pData)
{
    l_bytes_rd_LIN_1(sss, start, count, pData);
}
#endif /*(L_BYTES_RD_INST1 || L_BYTES_RD_INST2)*/


#if (L_BOOL_WR_INST1 || L_BOOL_WR_INST2)
/*******************************************************************************
* Function Name: l_bool_wr
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
void l_bool_wr(l_signal_handle sss, l_bool v)
{
    l_bool_wr_LIN_1(sss, v);
}
#endif /*(L_BOOL_WR_INST1 || L_BOOL_WR_INST2)*/


#if (L_U8_WR_INST1 || L_U8_WR_INST2)
/*******************************************************************************
* Function Name: l_u8_wr
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
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u8_wr(l_signal_handle sss, l_u8 v)
{
    l_u8_wr_LIN_1(sss, v);
}
#endif /*(L_U8_WR_INST1 || L_U8_WR_INST2)*/


#if (L_U16_WR_INST1 || L_U16_WR_INST2)
/*******************************************************************************
* Function Name: l_u16_wr
********************************************************************************
*
* Summary:
*  Writes the value v to the signal. If an invalid signal handle is passed
*  into the function, no action is done.
*
* Parameters:
*  sss - signal handle of the signal to write.
*  v   - value of the signal to be set.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void l_u16_wr(l_signal_handle sss, l_u16 v)
{
    l_u16_wr_LIN_1(sss, v);
}
#endif /*(L_U16_WR_INST1 || L_U16_WR_INST2)*/


#if (L_BYTES_WR_INST1 || L_BYTES_WR_INST2)
/*******************************************************************************
* Function Name: l_bytes_wr
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
void l_bytes_wr(l_signal_handle sss, l_u8 start, l_u8 count, const l_u8* const pData)
{
    l_bytes_wr_LIN_1(sss, start, count, pData);
}
#endif /*(L_BYTES_WR_INST1 || L_BYTES_WR_INST2)*/



/*******************************************************************************
* Variables
********************************************************************************/
uint8   LIN_1_initVar = 0u;


/*******************************************************************************
* Function Name: l_sys_init
********************************************************************************
*
* Summary:
*  Initializes the LIN core. If the Automatic Baud Rate Synchronization 
*  parameter is enabled in the Configure dialog, then this function saves the
*  initial SYSCLK-to-UARTCLK divider’s value, calculated by PSoC Creator. 
*  If the parameter is not enabled, this function does nothing and always
*  returns zero.
*
* Parameters:
*  None
*
* Return:
*  Zero     - if the initialization succeeded.
*  Non-zero - if the initialization failed.
*
*******************************************************************************/
l_bool l_sys_init(void)
{
    /* If not already initialized, then initialize hardware and variables */
    if(LIN_1_initVar == 0u)
    {
        #if(1u == LIN_1_AUTO_BAUD_RATE_SYNC)
            /* Save original clock divider */
            LIN_1_initialClockDivider =
                (uint16) (LIN_1_IntClk_GetDividerRegister() + 1u);
        #endif  /* (1u == LIN_1_AUTO_BAUD_RATE_SYNC) */

        #if(LIN_1_IS_MULTI_INSTANCE)
             #if(1u == NOT_DEFINED_AUTO_BAUD_RATE_SYNC)
    		    /* Save original clock divider */
                NOT_DEFINED_initialClockDivider = (uint16) (NOT_DEFINED_IntClk_GetDividerRegister() + 1u);
            #endif  /* (1u == NOT_DEFINED_AUTO_BAUD_RATE_SYNC) */
        #endif /* (LIN_1_IS_MULTI_INSTANCE) */

        LIN_1_initVar = 1u;
    }

    return (LIN_1_FALSE);
}


#if(1u == LIN_1_LIN_1_3)

    /*******************************************************************************
    * Function Name: l_ifc_connect
    ********************************************************************************
    *
    * Summary:
    *  The same as l_ifc_init.This function is not required to be used.
    *  Declared for consistency with LIN v.1.3 only.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Zero     - if the initialization succeeded.
    *  Non-zero - if the initialization failed.
    *
    *******************************************************************************/
    l_bool l_ifc_connect(l_ifc_handle iii)
    {
        l_bool returnValue = LIN_1_FALSE;

        switch(iii)
        {
            #if(1u == LIN_1_LIN_1_3)
                case LIN_1_IFC_HANDLE:
                    returnValue = l_ifc_connect_LIN_1();
                break;
            #endif /* (1u == LIN_1_LIN_1_3) */

            #if(LIN_1_IS_MULTI_INSTANCE)
                #if(1u == NOT_DEFINED_LIN_1_3)
                    /* Go here is interface 2 is specified */
                    case NOT_DEFINED_IFC_HANDLE:
                        returnValue = l_ifc_connect_NOT_DEFINED();
                    break;
                #endif /* (1u == NOT_DEFINED_LIN_1_3) */
            #endif /* (LIN_1_IS_MULTI_INSTANCE) */

            default:
                /* Unknown interface handle */
                returnValue = LIN_1_TRUE;
            break;
        }

        return(returnValue);
    }

    
    /*******************************************************************************
    * Function Name: l_ifc_disconnect
    ********************************************************************************
    *
    * Summary:
    *  Stops the component operation. This function is required for LIN v.1.3 only.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Zero     - if the initialization succeeded.
    *  Non-zero - if the initialization failed.
    *
    *******************************************************************************/
    l_bool l_ifc_disconnect(l_ifc_handle iii)
    {
        l_bool returnValue = LIN_1_FALSE;

        switch(iii)
        {
            #if(1u == LIN_1_LIN_1_3)
                case LIN_1_IFC_HANDLE:
                    returnValue = l_ifc_disconnect_LIN_1();
                break;
            #endif /* (1u == LIN_1_LIN_1_3) */

            #if(LIN_1_IS_MULTI_INSTANCE)
                #if(1u == NOT_DEFINED_LIN_1_3)
                    /* Go here is interface 2 is specified */
                    case NOT_DEFINED_IFC_HANDLE:
                        returnValue = l_ifc_disconnect_NOT_DEFINED();
                    break;
                #endif /* (1u == NOT_DEFINED_LIN_1_3) */
            #endif /* (LIN_1_IS_MULTI_INSTANCE) */

            default:
                /* Unknown interface handle */
                returnValue = LIN_1_TRUE;
            break;
        }

        return(returnValue);
    }

#endif /* ( 1u == LIN_1_LIN_1_3) */


/*******************************************************************************
* Function Name: l_ifc_init
********************************************************************************
*
* Summary:
*  The function initializes the LIN Slave component instance that is specified
*  by the name iii. It sets up internal functions such as the baud rate and
*  starts up digital blocks that are used by the LIN Slave component. This is
*  the first call that must be performed, before using any other interface-
*  related LIN Slave API functions.
*
* Note: This function is defined only for Main Instance in the project
* with multiple LIN instances.
*
* Parameters:
*  iii - is the name of the interface handle.
*
* Return:
*  The 0 is returned if operation succeeded and 1 if an invalid operation
*  parameter was passed to the function.
*
*******************************************************************************/
l_bool l_ifc_init(l_ifc_handle iii)
{
    l_bool returnValue;

    switch(iii)
    {
        case LIN_1_IFC_HANDLE:
            returnValue = l_ifc_init_LIN_1();
        break;

        #if(LIN_1_IS_MULTI_INSTANCE)
            /* Go here is interface 1 is specified */
            case NOT_DEFINED_IFC_HANDLE:
                returnValue = l_ifc_init_NOT_DEFINED();
            break;
        #endif /* (LIN_1_IS_MULTI_INSTANCE) */

        default:
            /* Unknown interface handle */
            returnValue = LIN_1_TRUE;
        break;
    }

    return (returnValue);
}


/*******************************************************************************
* Function Name: l_ifc_wake_up
********************************************************************************
*
* Summary:
*  This function transmits one wakeup signal. The wakeup signal is transmitted
*  directly when this function is called. When you call this API function, the
*  application is blocked until a wakeup signal is transmitted on the LIN bus.
*  The CyDelayUs() function is used as the timing source. The delay is
*  calculated based on the clock configuration entered in PSoC Creator.
*
* Note: This function is defined only for Main Instance in the project
*  with multiple LIN instances.
*
* Parameters:
*  iii - is the name of the interface handle.
*
* Return:
*  None
*
*******************************************************************************/
void l_ifc_wake_up(l_ifc_handle iii)
{
    switch(iii)
    {
        case LIN_1_IFC_HANDLE:
            l_ifc_wake_up_LIN_1();
        break;

        #if(LIN_1_IS_MULTI_INSTANCE)
            /* Go here is interface 1 is specified */
            case NOT_DEFINED_IFC_HANDLE:
                l_ifc_wake_up_NOT_DEFINED();
            break;
        #endif /* (LIN_1_IS_MULTI_INSTANCE) */

        default:
            /* Unknown interface handle - do nothing. */
        break;
    }
}


/*******************************************************************************
* Function Name: l_ifc_ioctl
********************************************************************************
*
* Summary:
*  This function controls functionality that is not covered by the other API
*  calls. It is used for protocol specific parameters or hardware specific
*  functionality. Example of such functionality can be to switch on/off the
*  wake up signal detection.
*
* Parameters:
*  iii - is the name of the interface handle.
*  op - is the operation that should be applied.
*  pv - is the pointer to the optional parameter.
*
* Return:
*  There is no error code value returned for operation selected. This means that
*  you must ensure that the values passed into the function are correct.
*
*  L_IOCTL_READ_STATUS operation
*  The first bit in this byte is the flag that indicates that there has been no
*  signaling on the bus for a certain elapsed time (available when
*  Bus Inactivity Timeout Detection option is enabled). If the elapsed time
*  passes a certain threshold, then this flag is set. Calling this API clears
*  all status bits after they are returned. The second bit is the flag that
*  indicates that a Targeted Reset service request (0xB5) was received
*  (when J2602-1 Compliance is enabled).
*
*  Symbolic Name : LIN_1_IOCTL_STS_BUS_INACTIVITY
*  Value         : 0x0001u
*  Description   : No signal was detected on the bus for a certain elapsed time
*
*  Symbolic Name : LIN_1_IOCTL_STS_TARGET_RESET
*  Value         : 0x0002u
*  Description   : Targeted Reset service request (0xB5) was received.
*
*  L_IOCTL_SET_BAUD_RATE operation
*  The 0 is returned if operation succeeded and 1 if an invalid operation
*  parameter was passed to the function.
*
*  L_IOCTL_SLEEP operation
*  The 0 is returned if operation succeeded and 1 if an invalid operation
*  parameter was passed to the function.
*
*  L_IOCTL_WAKEUP operation
*  The 0 is returned if operation succeeded and 1 if an invalid operation
*  parameter was passed to the function.
*
*  L_IOCTL_SYNC_COUNTS operation
*  Returns current number of sync field timer counts.
*
*  L_IOCTL_SET_SERIAL_NUMBER operation
*  The 0 is returned if operation succeeded and 1 if an invalid operation
*  parameter was passed to the function.
*
*******************************************************************************/
l_u16 l_ifc_ioctl(l_ifc_handle iii, l_ioctl_op op, void* pv)
{
    l_u16 returnValue;

    switch(iii)
    {
        case LIN_1_IFC_HANDLE:
            returnValue = l_ifc_ioctl_LIN_1(op, pv);
        break;

        #if(LIN_1_IS_MULTI_INSTANCE)
            /* Go here is interface 1 is specified */
            case NOT_DEFINED_IFC_HANDLE:
                returnValue = l_ifc_ioctl_NOT_DEFINED(op, pv);
            break;
        #endif /* (LIN_1_IS_MULTI_INSTANCE) */

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
*  iii - is the name of the interface handle.
*
* Return:
*  None
*
*******************************************************************************/
void l_ifc_rx(l_ifc_handle iii)
{
    switch(iii)
    {
        case LIN_1_IFC_HANDLE:
            l_ifc_rx_LIN_1();
        break;

        #if(LIN_1_IS_MULTI_INSTANCE)
            case NOT_DEFINED_IFC_HANDLE:
                l_ifc_rx_NOT_DEFINED();
            break;
        #endif /* (LIN_1_IS_MULTI_INSTANCE) */

        default:
        break;
    }
}


/*******************************************************************************
* Function Name: l_ifc_tx
********************************************************************************
*
* Summary:
*  The LIN Slave component takes care of calling this API routine automatically.
*  Therefore, this API routine must not be called by the application code.
*
* Parameters:
*  iii - is the name of the interface handle.
*
* Return:
*  None
*
*******************************************************************************/
void l_ifc_tx(l_ifc_handle iii)
{
    switch(iii)
    {
        case LIN_1_IFC_HANDLE:
            l_ifc_tx_LIN_1();
        break;

        #if(LIN_1_IS_MULTI_INSTANCE)
            case NOT_DEFINED_IFC_HANDLE:
                l_ifc_tx_NOT_DEFINED();
            break;
        #endif /* (LIN_1_IS_MULTI_INSTANCE) */

        default:
        break;
    }
}


/*******************************************************************************
* Function Name: l_ifc_aux
********************************************************************************
*
* Summary:
*  The LIN Slave component takes care of calling this API routine automatically.
*  Therefore, this API routine must not be called by the application code.
*
* Parameters:
*  iii - is the name of the interface handle.
*
* Return:
*  None
*
*******************************************************************************/
void l_ifc_aux(l_ifc_handle iii)
{
    switch(iii)
    {
        case LIN_1_IFC_HANDLE:
            l_ifc_aux_LIN_1();
        break;

        #if(LIN_1_IS_MULTI_INSTANCE)
            /* Go here is interface 1 is specified */
            case NOT_DEFINED_IFC_HANDLE:
                l_ifc_aux_NOT_DEFINED();
            break;
        #endif /* (LIN_1_IS_MULTI_INSTANCE) */

        default:
        break;
    }
}


/*******************************************************************************
* Function Name: l_ifc_read_status
********************************************************************************
*
* Summary:
*  This function is defined by the LIN specification. This returns the status of
*  the specified LIN interface and then clears all status bits for that
*  interface. See Section 7.2.5.8 of the LIN 2.1 specification.
*
*
* Parameters:
*  iii - is the name of the interface handle.
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
l_u16 l_ifc_read_status(l_ifc_handle iii)
{
    l_u16 returnValue;

    /* Determine which interface is specified */
    switch(iii)
    {
        /* Go here is interface 0 is specified */
        case LIN_1_IFC_HANDLE:
            returnValue = l_ifc_read_status_LIN_1();
        break;

        #if(LIN_1_IS_MULTI_INSTANCE)
            /* Go here is interface 1 is specified */
            case NOT_DEFINED_IFC_HANDLE:
                returnValue = l_ifc_read_status_NOT_DEFINED();
            break;
        #endif /* (LIN_1_IS_MULTI_INSTANCE) */

        default:
            returnValue = (l_u16) CYRET_BAD_PARAM;
        break;
    }

    return (returnValue);
}


/*******************************************************************************
* LIN Transport layer API wrappers
********************************************************************************
* Summary:
*
* Below wrappers are provided only for compatibility with LIN API TL.
* Because LIN API defines only dynamic calls to TL API functions with iii
* interface as parameter, these wrappers redirect the call to the "hidden"
* static function named as ld_function_LIN_1(...).
* It is supposed that customer's code can directly call the static TL function
* without usage of below wrappers.
*
********************************************************************************/

/*******************************************************************************
* Function Name: ld_init
********************************************************************************
*
* Summary:
*  This call will (re)initialize the raw and the cooked layers on the interface
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
void ld_init(l_ifc_handle iii)
{
    switch(iii)
    {
        #if(1u == LIN_1_TL_ENABLED)        
            case LIN_1_IFC_HANDLE:
                ld_init_LIN_1();
            break;
        #endif /* (1u == LIN_1_TL_ENABLED) */
        
        #if(LIN_1_IS_MULTI_INSTANCE)
            #if(1u == NOT_DEFINED_TL_ENABLED)
                /* Go here is interface 1 is specified */
                case NOT_DEFINED_IFC_HANDLE:
                    ld_init_NOT_DEFINED();
                break;
            #endif /* (1u == NOT_DEFINED_TL_ENABLED) */
        #endif /* (LIN_1_IS_MULTI_INSTANCE) */

        default:
        break;
    }
}


/*******************************************************************************
* Function Name: ld_read_configuration
********************************************************************************
*
* Summary:
*  This function is used to read the NAD and PID values from volatile memory.
*  This function can be used to read the current configuration data, and then
*  save this data into non-volatile (flash) memory. The application should save
*  the configuration data to flash when the "Save Configuration" bit is set in
*  the LIN status register (returned by l_ifc_read_status_).
*  The configuration data that is read is a series of bytes. The first byte is
*  the current NAD of the slave. The next bytes are the current PID values for
*  the frames that the slave responds to. The PID values are in the order in
*  which the frames appear in the LDF or NCF file.
*
* Parameters:
*  iii - Interface.
*
* Return:
*  LD_READ_OK - If the service was successful.
*
*  LD_LENGTH_TOO_SHORT - If the configuration size is greater than the length.
*                        It means that the data area does not contain a valid
*                        configuration.
*
*******************************************************************************/
l_u8 ld_read_configuration(l_ifc_handle iii, l_u8* pData, l_u8* const length)
{
    l_u8 returnValue = LIN_1_LD_READ_OK;

    #if((0u == LIN_1_TL_ENABLED) && \
       ((0u == LIN_1_IS_MULTI_INSTANCE) || (1u != NOT_DEFINED_TL_ENABLED)))

        if((0u != iii) || (0u != *pData) || (0u != *length))
        {
            /* Suppress compiler warning. */
        }

    #else

    switch(iii)
    {
        #if(1u == LIN_1_TL_ENABLED)
            case LIN_1_IFC_HANDLE:
                returnValue = ld_read_configuration_LIN_1(pData, length);
            break;
        #endif /* (1u == LIN_1_TL_ENABLED) */

        #if(LIN_1_IS_MULTI_INSTANCE)
            #if(1u == NOT_DEFINED_TL_ENABLED)
                /* Go here is interface 1 is specified */
                case NOT_DEFINED_IFC_HANDLE:
                    returnValue = ld_read_configuration_NOT_DEFINED(pData, length);
                break;
            #endif /* (1u == NOT_DEFINED_TL_ENABLED) */
        #endif /* (LIN_1_IS_MULTI_INSTANCE) */

        default:
        break;
    }

    #endif /* ((0u == LIN_1_TL_ENABLED) || \
              ((0u == LIN_1_IS_MULTI_INSTANCE) && (1u != NOT_DEFINED_TL_ENABLED))) */
    
    return(returnValue);
}


/*******************************************************************************
* Function Name: ld_set_configuration
********************************************************************************
*
* Summary:
*  This call will not transport anything on the bus.
*
*  The function will configure the NAD and the PIDs according to the
*  configuration given by data. The intended usage is to restore a saved
*  configuration or set an initial configuration (e.g. coded by I/O pins).
*
*  The function shall be called after calling l_ifc_init.
*
*  The caller shall set the size of the data area before calling the function.
*
*  The data contains the NAD and the PIDs and occupies one byte each.
*  The structure of the data is: NAD and then all PIDs for the frames.
*  The order of the PIDs are the same as the frame list in the LDF,
*  Section 9.2.2.2, and NCF, Section 8.2.5.
*
* Parameters:
*  iii - Interface.
*
* Return:
*  LD_SET_OK - If the service was successful.
*
*  LD_LENGTH_NOT_CORRECT - If the required size of the configuration is not
*                          equal to the given length.
*
*  LD_DATA_ERROR - The set of configuration could not be set. A an error
*                  occurred while setting the configuration and the read back
*                  configuration settings doesn't match required settings.
*
*  _volatileConfig - new frame PIDs are copied.
*
*******************************************************************************/
l_u8 ld_set_configuration(l_ifc_handle iii, const l_u8* const pData, l_u16 length)
{
    l_u8 returnValue = LIN_1_LD_SET_OK;

    #if((0u == LIN_1_TL_ENABLED) && \
       ((0u == LIN_1_IS_MULTI_INSTANCE) || (1u != NOT_DEFINED_TL_ENABLED)))

        if((0u != iii) || (0u != *pData) || (0u != length))
        {
            /* Suppress compiler warning. */
        }

    #else

        switch(iii)
        {
            #if(1u == LIN_1_TL_ENABLED)
                case LIN_1_IFC_HANDLE:
                    returnValue = ld_set_configuration_LIN_1(pData, length);
                break;
            #endif /* (1u == LIN_1_TL_ENABLED) */

            #if(LIN_1_IS_MULTI_INSTANCE)
                #if(1u == NOT_DEFINED_TL_ENABLED)
                    /* Go here is interface 1 is specified */
                    case NOT_DEFINED_IFC_HANDLE:
                        returnValue = ld_set_configuration_NOT_DEFINED(pData, length);
                    break;
                #endif /* (1u == NOT_DEFINED_TL_ENABLED) */
            #endif /* (NOT_DEFINED_IS_MULTI_INSTANCE) */

            default:
            break;
        }

    #endif /* ((0u == LIN_1_TL_ENABLED) && \
              ((0u == LIN_1_IS_MULTI_INSTANCE) && (1u != NOT_DEFINED_TL_ENABLED))) */

    return(returnValue);
}


/*******************************************************************************
* Function Name: ld_read_by_id_callout
********************************************************************************
*
* Summary:
*  This callout is used when the master node transmits a read by identifier
*  request with an identifier in the user defined area. The slave node
*  application will be called from the driver when such request is received.
*
* Parameters:
*  iii - Interface.
*  id - The id parameter is the identifier in the user defined area (32 to 63),
*  from the read by identifier configuration request.
*  frameData - The data pointer points to a data area with 5 bytes. This area
*  will be used by the application to set up the positive response.
*
* Return:
*  LD_NEGATIVE_RESPONSE - The slave node will respond with a negative response.
*  In this case the data area is not considered
*
*  LD_POSTIVE_RESPONSE - The slave node will setup a positive response using
*  the data provided by the application.
*
*  LD_NO_RESPONSE - The slave node will not answer.
*
*******************************************************************************/
l_u8 ld_read_by_id_callout(l_ifc_handle iii, l_u8 id, l_u8* frameData)
{
    l_u8 returnValue = LD_NEGATIVE_RESPONSE;

    #if( ((1u != LIN_1_TL_ENABLED) || \
          (1u != LIN_1_CS_ENABLED) || \
          ((0u == (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL)) && \
           (0u == (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL)))) \
         && \
         ((0u == LIN_1_IS_MULTI_INSTANCE) || \
          (1u != NOT_DEFINED_TL_ENABLED) || \
          (1u != NOT_DEFINED_CS_ENABLED) || \
          ((0u == (NOT_DEFINED_CS_SEL_SERVICES01 & NOT_DEFINED_NCS_0xB2_SEL)) && \
           (0u == (NOT_DEFINED_CS_SEL_SERVICES01 & NOT_DEFINED_NCS_0xB3_SEL)))) )

        if((0u != iii) || (0u != id) || (0u != *frameData))
        {
            /* Suppress compiler warning. */
        }

    #else

        switch(iii)
        {
            #if(1u == LIN_1_TL_ENABLED)
                #if(1u == LIN_1_CS_ENABLED)
                    #if((0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL)) || \
                        (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL)))
                        case LIN_1_IFC_HANDLE:
                            returnValue = ld_read_by_id_callout_LIN_1(id,frameData);
                        break;
                    #endif /* ((0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB2_SEL)) || \
                              (0u != (LIN_1_CS_SEL_SERVICES01 & LIN_1_NCS_0xB3_SEL))) */
                #endif /* (1u == LIN_1_CS_ENABLED) */
            #endif /* (1u == LIN_1_TL_ENABLED) */
            
            #if(LIN_1_IS_MULTI_INSTANCE)
                /* Go here is interface 2 is specified */
                #if(1u == NOT_DEFINED_TL_ENABLED)
                    #if(1u == NOT_DEFINED_CS_ENABLED)
                        #if((0u != (NOT_DEFINED_CS_SEL_SERVICES01 & NOT_DEFINED_NCS_0xB2_SEL)) || \
                            (0u != (NOT_DEFINED_CS_SEL_SERVICES01 & NOT_DEFINED_NCS_0xB3_SEL)))
                                case NOT_DEFINED_IFC_HANDLE:
                                    returnValue = ld_read_by_id_callout_NOT_DEFINED(id,frameData);
                                break;
                        #endif /* ((0u != (NOT_DEFINED_CS_SEL_SERVICES01 & NOT_DEFINED_NCS_0xB2_SEL)) || \
                            (0u != (NOT_DEFINED_CS_SEL_SERVICES01 & NOT_DEFINED_NCS_0xB3_SEL))) */
                    #endif /* (1u == NOT_DEFINED_CS_ENABLED) */
                #endif /* (1u == NOT_DEFINED_TL_ENABLED) */
            #endif /* (LIN_1_IS_MULTI_INSTANCE) */

            default:
            break;
        }

    #endif /* (1u != LIN_1_TL_ENABLED) */

    return(returnValue);
}


/*******************************************************************************
* Function Name: ld_send_message
********************************************************************************
*
* Summary:
*  The call packs the information specified by data and length into one or
*  multiple diagnostic frames. If the call is made in a master node application
*  the frames are transmitted to the slave node with the address NAD. If the
*  call is made in a slave node application the frames are transmitted to the
*  master node with the address NAD. The parameter NAD is not used in slave
*  nodes.
*
*  The value of the SID (or RSID) shall be the first byte in the data area.
*
*  Length must be in the range of 1 to 4095 bytes. The length shall also include
*  the SID (or RSID) value, i.e. message length plus one.
*
*  The call is asynchronous, i.e. not suspended until the message has been sent,
*  and the buffer may not be changed by the application as long as calls to
*  ld_tx_status returns LD_IN_PROGRESS.
*
*  The data is transmitted in suitable frames (master request frame for master
*  nodes and slave response frame for slave nodes).
*
*  If there is a message in progress, the call will return with no action.
*
* Parameters:
*  iii - Interface.
*  length - Size of data to be sent in bytes.
*  nad - Address of the slave node to which data is sent.
*  data - Array of data to be sent. The value of the RSID is the first byte in
*  the data area.
*
* Return:
*  None
*
*******************************************************************************/
void ld_send_message(l_ifc_handle iii, l_u16 length, l_u8 nad, const l_u8* const ld_data)
{
    /* Below switch statement is defined only when Transport Layer with Cooked
    *  interface is enabled at least in one of two possible component instances
    */

    #if( ((1u != LIN_1_TL_ENABLED) || (1u != LIN_1_TL_API_FORMAT)) && \
         ((0u == LIN_1_IS_MULTI_INSTANCE) || (1u != NOT_DEFINED_TL_ENABLED) || \
          (1u != NOT_DEFINED_TL_API_FORMAT)) )

        if((0u != iii) || (0u != length) || (0u != nad) || (0u != *ld_data))
        {
            /* Suppress compiler warning. */
        }

    #else

        switch(iii)
        {
            #if(1u == LIN_1_TL_ENABLED)
                #if(1u == LIN_1_TL_API_FORMAT)
                    case LIN_1_IFC_HANDLE:
                        ld_send_message_LIN_1(length,nad, ld_data);
                    break;
                    #endif /* (1u == LIN_1_TL_API_FORMAT) */
                #endif /* (1u == LIN_1_TL_ENABLED) */

            #if(LIN_1_IS_MULTI_INSTANCE)
                #if(1u == NOT_DEFINED_TL_ENABLED)
                    #if(1u == NOT_DEFINED_TL_API_FORMAT)
                       /* Go here is interface 1 is specified */
                        case NOT_DEFINED_IFC_HANDLE:
                            ld_send_message_NOT_DEFINED(length,nad,ld_data);
                        break;
                    #endif /* (1u == NOT_DEFINED_TL_API_FORMAT) */
                #endif /* (1u == NOT_DEFINED_TL_ENABLED) */
            #endif /* (LIN_1_IS_MULTI_INSTANCE) */

            default:
            break;
        }

    #endif /* (1u != LIN_1_TL_ENABLED) */
}


/*******************************************************************************
* Function Name: ld_receive_message
********************************************************************************
*
* Summary:
*  The call prepares the LIN diagnostic module to receive one message and store
*  it in the buffer pointed to by data. At the call, length shall specify the
*  maximum length allowed. When the reception has completed, length is changed
*  to the actual length and NAD to the NAD in the message.
*
*  SID (or RSID) will be the first byte in the data area.
*
*  Length will be in the range of 1 to 4095 bytes, but never more than the value
*  originally set in the call. SID (or RSID) is included in the length.
*
*  The parameter NAD is not used in slave nodes.
*
*  The call is asynchronous, i.e. not suspended until the message has been
*  received, and the buffer may not be changed by the application as long as
*  calls to ld_rx_status returns LD_IN_PROGRESS. If the call is made after the
*  message transmission has commenced on the bus (i.e. the SF or FF is already
*  transmitted), this message will not be received. Instead the function will
*  wait until next message commence.
*
*  The data is received from the succeeding suitable frames (master request
*  frame for slave nodes and slave response frame for master nodes).
*
*  The application shall monitor the ld_rx_status and shall not call this
*  function until the status is LD_COMPLETED. Otherwise this function may
*  return inconsistent data in the parameters.
*
* Parameters:
*  iii - Interface.
*  length: Size of data to be received in bytes.
*  nad: Address of the slave node from which data is received.
*  data: Array of data to be received. The value of the SID is the first byte
*  in the data area.
*
* Return:
*  None
*
*******************************************************************************/
void ld_receive_message(l_ifc_handle iii, l_u16* const length, l_u8* const nad, l_u8* const ld_data)
{
    /* Below switch statement is defined only when Transport Layer with Cooked
    *  interface is enabled at least in one of two possible component instances
    */

    #if( ((1u != LIN_1_TL_ENABLED) || (1u != LIN_1_TL_API_FORMAT)) && \
         ((0u == LIN_1_IS_MULTI_INSTANCE) || (1u != NOT_DEFINED_TL_ENABLED) || \
          (1u != NOT_DEFINED_TL_API_FORMAT)) )

        if((0u != iii) || (0u != *length) || (0u != *nad) || (0u != *ld_data))
        {
            /* Suppress compiler warning. */
        }

    #else

        switch(iii)
        {
            #if(1u == LIN_1_TL_ENABLED)
                #if(1u == LIN_1_TL_API_FORMAT)
                    case LIN_1_IFC_HANDLE:
                        ld_receive_message_LIN_1(length,nad, ld_data);
                    break;
                #endif /* (1u == LIN_1_TL_API_FORMAT) */
            #endif /* (1u == LIN_1_TL_ENABLED) */

            #if(LIN_1_IS_MULTI_INSTANCE)
                #if(1u == NOT_DEFINED_TL_ENABLED)
                    #if(1u == NOT_DEFINED_TL_API_FORMAT)
                        /* Go here is interface 2 is specified */
                        case NOT_DEFINED_IFC_HANDLE:
                            ld_receive_message_NOT_DEFINED(length,nad,ld_data);
                        break;
                    #endif /* (1u == NOT_DEFINED_TL_API_FORMAT) */
                #endif /* (1u == NOT_DEFINED_TL_ENABLED) */
            #endif /* (LIN_1_IS_MULTI_INSTANCE) */

            default:
            break;
        }

    #endif /* (1u != LIN_1_TL_ENABLED) */
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
*                 returned after initialization of the Transport Layer.
*
*  LD_FAILED - The transmission ended in an error. The data was only partially
*              sent. The transport layer shall be reinitialized before
*              processing further messages. To find out why a transmission has
*              failed, check the status management function l_read_status.
*
*  LD_N_AS_TIMEOUT - The transmission failed because of a N_As timeout,
*
*******************************************************************************/
l_u8 ld_tx_status(l_ifc_handle iii)
{
    l_u8 returnValue = LD_COMPLETED;

    switch(iii)
    {
        #if(1u == LIN_1_TL_ENABLED)
            #if(1u == LIN_1_TL_API_FORMAT)
                case LIN_1_IFC_HANDLE:
                    returnValue = ld_tx_status_LIN_1();
                break;
                #endif /* (1u == LIN_1_TL_API_FORMAT) */
            #endif /* (1u == LIN_1_TL_ENABLED) */

        #if(LIN_1_IS_MULTI_INSTANCE)
            #if(1u == NOT_DEFINED_TL_ENABLED)
                #if(1u == NOT_DEFINED_TL_API_FORMAT)
                    /* Go here is interface 1 is specified */
                    case NOT_DEFINED_IFC_HANDLE:
                        returnValue = ld_tx_status_NOT_DEFINED();
                    break;
                #endif /* (1u == NOT_DEFINED_TL_API_FORMAT) */
            #endif /* (1u == NOT_DEFINED_TL_ENABLED) */
        #endif /* (LIN_1_IS_MULTI_INSTANCE) */

        default:
        break;
    }

    return(returnValue);
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
*                 initialization of the Tansport Layer.
*
*  LD_FAILED - The reception ended in an error. The data was only partially
*              received and should not be trusted. Initialize before processing
*              further transport layer messages. To find out why a reception
*              has failed, check the status management function l_read_status.
*
*  LD_N_CR_TIMEOUT - The reception failed because of a N_Cr timeout,
*
*  LD_WRONG_SN - The reception failed because of an unexpected sequence
*                number.
*
*******************************************************************************/
l_u8 ld_rx_status(l_ifc_handle iii)
{
    l_u8 returnValue = LD_COMPLETED;

    switch(iii)
    {
        #if(1u == LIN_1_TL_ENABLED)
            #if(1u == LIN_1_TL_API_FORMAT)
                case LIN_1_IFC_HANDLE:
                    returnValue = ld_rx_status_LIN_1();
                break;
            #endif /* (1u == LIN_1_TL_API_FORMAT) */
        #endif /* (1u == LIN_1_TL_ENABLED) */

        #if(LIN_1_IS_MULTI_INSTANCE)
            #if(1u == NOT_DEFINED_TL_ENABLED)
                #if(1u == NOT_DEFINED_TL_API_FORMAT)
                    /* Go here is interface 2 is specified */
                    case NOT_DEFINED_IFC_HANDLE:
                        returnValue = ld_rx_status_NOT_DEFINED();
                    break;
                #endif /* (1u == NOT_DEFINED_TL_API_FORMAT) */
            #endif /* (1u == NOT_DEFINED_TL_ENABLED) */
        #endif /* (LIN_1_IS_MULTI_INSTANCE) */

        default:
        break;
    }

    return(returnValue);
}


/*******************************************************************************
* Function Name: ld_put_raw
********************************************************************************
*
* Summary:
*  The call queues the transmission of 8 bytes of data in one frame. The data
*  is sent in the next suitable frame (slave response frame). The data area
*  will be copied in the call, the pointer will not be memorized. If no more
*  queue resources are available, the data may be jettisoned and the
*  appropriate error status will be set.
*
* Parameters:
*  iii - Interface.
*  data - Array of data to be sent.
*
* Return:
*  None
*
*******************************************************************************/
void ld_put_raw(l_ifc_handle iii, const l_u8* const ld_data)
{

    /* Below switch statement is defined only when Transport Layer with Raw
    *  interface is enabled at least in one of two possible component instances
    */

    #if( ((1u != LIN_1_TL_ENABLED) || (1u == LIN_1_TL_API_FORMAT)) && \
         ((0u == LIN_1_IS_MULTI_INSTANCE) || (1u != NOT_DEFINED_TL_ENABLED) || \
          (1u == NOT_DEFINED_TL_API_FORMAT)) )

        if((0u != iii) || (0u != *ld_data))
        {
            /* Suppress compiler warning. */
        }

    #else

        switch(iii)
        {
            #if(1u == LIN_1_TL_ENABLED)
                #if(1u != LIN_1_TL_API_FORMAT)
                    case LIN_1_IFC_HANDLE:
                        ld_put_raw_LIN_1(ld_data);
                    break;                    
                #endif /* (1u != LIN_1_TL_API_FORMAT) */
            #endif /* (1u == LIN_1_TL_ENABLED) */

            #if(LIN_1_IS_MULTI_INSTANCE)
                #if(1u == NOT_DEFINED_TL_ENABLED)
                    #if(1u != NOT_DEFINED_TL_API_FORMAT)
                        /* Go here is interface 1 is specified */
                        case NOT_DEFINED_IFC_HANDLE:
                            ld_put_raw_NOT_DEFINED(ld_data);
                        break;
                    #endif /* (1u != NOT_DEFINED_TL_API_FORMAT) */
                #endif /* (1u == NOT_DEFINED_TL_ENABLED) */
            #endif /* (LIN_1_IS_MULTI_INSTANCE) */

            default:
            break;
        }

    #endif /* (1u != LIN_1_TL_ENABLED) */
}


/*******************************************************************************
* Function Name: ld_get_raw
********************************************************************************
*
* Summary:
*  The call copies the oldest received diagnostic frame data to the memory
*  specified by data. The data returned is received from master request frame
*  If the receive queue is empty no data will be copied.
*
* Parameters:
*  iii - Interface.
*  data - Array to which the oldest received diagnostic frame data will be
*  copied.
*
* Return:
*  None
*
*  No
*
*******************************************************************************/
void ld_get_raw(l_ifc_handle iii, l_u8* const ld_data)
{
    /* Below switch statement is defined only when Transport Layer with Raw
    *  interface is enabled at least in one of two possible component instances
    */

    #if( ((1u != LIN_1_TL_ENABLED) || (1u == LIN_1_TL_API_FORMAT)) && \
         ((0u == LIN_1_IS_MULTI_INSTANCE) || (1u != NOT_DEFINED_TL_ENABLED) || \
          (1u == NOT_DEFINED_TL_API_FORMAT)) )

        if((0u != iii) || (0u != *ld_data))
        {
            /* Suppress compiler warning. */
        }

    #else

        switch(iii)
        {
            #if(1u == LIN_1_TL_ENABLED)
                #if(1u != LIN_1_TL_API_FORMAT)
                    case LIN_1_IFC_HANDLE:
                        ld_get_raw_LIN_1(ld_data);
                    break;
                #endif /* (1u != LIN_1_TL_API_FORMAT) */
            #endif /* (1u == LIN_1_TL_ENABLED) */

            #if(LIN_1_IS_MULTI_INSTANCE)
                #if(1u == NOT_DEFINED_TL_ENABLED)
                    #if(1u != NOT_DEFINED_TL_API_FORMAT)
                        /* Go here is interface 1 is specified */
                        case NOT_DEFINED_IFC_HANDLE:
                            ld_get_raw_NOT_DEFINED(ld_data);
                        break;
                    #endif /* (1u != NOT_DEFINED_TL_API_FORMAT) */
                #endif /* (1u == NOT_DEFINED_TL_ENABLED) */
            #endif /* (LIN_1_IS_MULTI_INSTANCE) */

            default:
            break;
        }

    #endif /* (1u != LIN_1_TL_ENABLED) */
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
*   LD_QUEUE_EMPTY - The transmit queue is empty. In case previous calls to
*   ld_put_raw, all frames in the queue have been transmitted.
*
*   LD_QUEUE_AVAILABLE - The transmit queue contains entries, but is not full.
*
*   LD_QUEUE_FULL - The transmit queue is full and can not accept further
*   frames.
*
*   LD_TRANSMIT_ERROR - LIN protocol errors occurred during the transfer;
*   initialize and redo the transfer.
*
*******************************************************************************/
l_u8 ld_raw_tx_status(l_ifc_handle iii)
{
    l_u8 returnValue = LD_QUEUE_EMPTY;

    switch(iii)
    {
        #if(1u == LIN_1_TL_ENABLED)
            #if(1u != LIN_1_TL_API_FORMAT)
                case LIN_1_IFC_HANDLE:
                        returnValue = ld_raw_tx_status_LIN_1();
                break;
            #endif /* (1u != LIN_1_TL_API_FORMAT) */
        #endif /* (1u == LIN_1_TL_ENABLED) */

        #if(LIN_1_IS_MULTI_INSTANCE)
            #if(1u == NOT_DEFINED_TL_ENABLED)
                #if(1u != NOT_DEFINED_TL_API_FORMAT)
                    /* Go here is interface 1 is specified */
                    case NOT_DEFINED_IFC_HANDLE:
                        returnValue = ld_raw_tx_status_NOT_DEFINED();
                    break;
                #endif /* (1u != NOT_DEFINED_TL_API_FORMAT) */
            #endif /* (1u == NOT_DEFINED_TL_ENABLED) */
        #endif /* (LIN_1_IS_MULTI_INSTANCE) */

        default:
        break;
    }

    return(returnValue);
}


/*******************************************************************************
* Function Name: _ld_raw_rx_status
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
*   LD_RECEIVE_ERROR - LIN protocol errors occurred during the transfer;
*
*   initialize and redo the transfer.
*
*******************************************************************************/
l_u8 ld_raw_rx_status(l_ifc_handle iii)
{
    l_u8 returnValue = LD_NO_DATA;

    switch(iii)
    {
        #if(1u == LIN_1_TL_ENABLED)
            #if(1u != LIN_1_TL_API_FORMAT)
                case LIN_1_IFC_HANDLE:
                        returnValue = ld_raw_rx_status_LIN_1();
                break;
            #endif /* (1u != LIN_1_TL_API_FORMAT) */
        #endif /* (1u == LIN_1_TL_ENABLED) */

        #if(LIN_1_IS_MULTI_INSTANCE)
            #if(1u == NOT_DEFINED_TL_ENABLED)
                #if(1u != NOT_DEFINED_TL_API_FORMAT)
                    /* Go here is interface 1 is specified */
                    case NOT_DEFINED_IFC_HANDLE:
                        returnValue = ld_raw_rx_status_NOT_DEFINED();
                    break;
                #endif /* (1u != NOT_DEFINED_TL_API_FORMAT) */
            #endif /* (1u == NOT_DEFINED_TL_ENABLED) */
        #endif /* (LIN_1_IS_MULTI_INSTANCE) */

        default:
        break;
    }

    return(returnValue);
}

/* [] END OF FILE */
