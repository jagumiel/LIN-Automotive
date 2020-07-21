/*******************************************************************************
* File Name: RedLed.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_RedLed_H) /* Pins RedLed_H */
#define CY_PINS_RedLed_H

#include "cytypes.h"
#include "cyfitter.h"
#include "RedLed_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} RedLed_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   RedLed_Read(void);
void    RedLed_Write(uint8 value);
uint8   RedLed_ReadDataReg(void);
#if defined(RedLed__PC) || (CY_PSOC4_4200L) 
    void    RedLed_SetDriveMode(uint8 mode);
#endif
void    RedLed_SetInterruptMode(uint16 position, uint16 mode);
uint8   RedLed_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void RedLed_Sleep(void); 
void RedLed_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(RedLed__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define RedLed_DRIVE_MODE_BITS        (3)
    #define RedLed_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - RedLed_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the RedLed_SetDriveMode() function.
         *  @{
         */
        #define RedLed_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define RedLed_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define RedLed_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define RedLed_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define RedLed_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define RedLed_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define RedLed_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define RedLed_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define RedLed_MASK               RedLed__MASK
#define RedLed_SHIFT              RedLed__SHIFT
#define RedLed_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in RedLed_SetInterruptMode() function.
     *  @{
     */
        #define RedLed_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define RedLed_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define RedLed_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define RedLed_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(RedLed__SIO)
    #define RedLed_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(RedLed__PC) && (CY_PSOC4_4200L)
    #define RedLed_USBIO_ENABLE               ((uint32)0x80000000u)
    #define RedLed_USBIO_DISABLE              ((uint32)(~RedLed_USBIO_ENABLE))
    #define RedLed_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define RedLed_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define RedLed_USBIO_ENTER_SLEEP          ((uint32)((1u << RedLed_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << RedLed_USBIO_SUSPEND_DEL_SHIFT)))
    #define RedLed_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << RedLed_USBIO_SUSPEND_SHIFT)))
    #define RedLed_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << RedLed_USBIO_SUSPEND_DEL_SHIFT)))
    #define RedLed_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(RedLed__PC)
    /* Port Configuration */
    #define RedLed_PC                 (* (reg32 *) RedLed__PC)
#endif
/* Pin State */
#define RedLed_PS                     (* (reg32 *) RedLed__PS)
/* Data Register */
#define RedLed_DR                     (* (reg32 *) RedLed__DR)
/* Input Buffer Disable Override */
#define RedLed_INP_DIS                (* (reg32 *) RedLed__PC2)

/* Interrupt configuration Registers */
#define RedLed_INTCFG                 (* (reg32 *) RedLed__INTCFG)
#define RedLed_INTSTAT                (* (reg32 *) RedLed__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define RedLed_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(RedLed__SIO)
    #define RedLed_SIO_REG            (* (reg32 *) RedLed__SIO)
#endif /* (RedLed__SIO_CFG) */

/* USBIO registers */
#if !defined(RedLed__PC) && (CY_PSOC4_4200L)
    #define RedLed_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define RedLed_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define RedLed_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define RedLed_DRIVE_MODE_SHIFT       (0x00u)
#define RedLed_DRIVE_MODE_MASK        (0x07u << RedLed_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins RedLed_H */


/* [] END OF FILE */
