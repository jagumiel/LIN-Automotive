/*******************************************************************************
* File Name: LedRed.h  
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

#if !defined(CY_PINS_LedRed_H) /* Pins LedRed_H */
#define CY_PINS_LedRed_H

#include "cytypes.h"
#include "cyfitter.h"
#include "LedRed_aliases.h"


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
} LedRed_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   LedRed_Read(void);
void    LedRed_Write(uint8 value);
uint8   LedRed_ReadDataReg(void);
#if defined(LedRed__PC) || (CY_PSOC4_4200L) 
    void    LedRed_SetDriveMode(uint8 mode);
#endif
void    LedRed_SetInterruptMode(uint16 position, uint16 mode);
uint8   LedRed_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void LedRed_Sleep(void); 
void LedRed_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(LedRed__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define LedRed_DRIVE_MODE_BITS        (3)
    #define LedRed_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - LedRed_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the LedRed_SetDriveMode() function.
         *  @{
         */
        #define LedRed_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define LedRed_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define LedRed_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define LedRed_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define LedRed_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define LedRed_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define LedRed_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define LedRed_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define LedRed_MASK               LedRed__MASK
#define LedRed_SHIFT              LedRed__SHIFT
#define LedRed_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in LedRed_SetInterruptMode() function.
     *  @{
     */
        #define LedRed_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define LedRed_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define LedRed_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define LedRed_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(LedRed__SIO)
    #define LedRed_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(LedRed__PC) && (CY_PSOC4_4200L)
    #define LedRed_USBIO_ENABLE               ((uint32)0x80000000u)
    #define LedRed_USBIO_DISABLE              ((uint32)(~LedRed_USBIO_ENABLE))
    #define LedRed_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define LedRed_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define LedRed_USBIO_ENTER_SLEEP          ((uint32)((1u << LedRed_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << LedRed_USBIO_SUSPEND_DEL_SHIFT)))
    #define LedRed_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << LedRed_USBIO_SUSPEND_SHIFT)))
    #define LedRed_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << LedRed_USBIO_SUSPEND_DEL_SHIFT)))
    #define LedRed_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(LedRed__PC)
    /* Port Configuration */
    #define LedRed_PC                 (* (reg32 *) LedRed__PC)
#endif
/* Pin State */
#define LedRed_PS                     (* (reg32 *) LedRed__PS)
/* Data Register */
#define LedRed_DR                     (* (reg32 *) LedRed__DR)
/* Input Buffer Disable Override */
#define LedRed_INP_DIS                (* (reg32 *) LedRed__PC2)

/* Interrupt configuration Registers */
#define LedRed_INTCFG                 (* (reg32 *) LedRed__INTCFG)
#define LedRed_INTSTAT                (* (reg32 *) LedRed__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define LedRed_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(LedRed__SIO)
    #define LedRed_SIO_REG            (* (reg32 *) LedRed__SIO)
#endif /* (LedRed__SIO_CFG) */

/* USBIO registers */
#if !defined(LedRed__PC) && (CY_PSOC4_4200L)
    #define LedRed_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define LedRed_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define LedRed_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define LedRed_DRIVE_MODE_SHIFT       (0x00u)
#define LedRed_DRIVE_MODE_MASK        (0x07u << LedRed_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins LedRed_H */


/* [] END OF FILE */
