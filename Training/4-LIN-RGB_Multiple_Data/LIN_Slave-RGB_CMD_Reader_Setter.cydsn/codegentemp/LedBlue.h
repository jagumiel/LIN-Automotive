/*******************************************************************************
* File Name: LedBlue.h  
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

#if !defined(CY_PINS_LedBlue_H) /* Pins LedBlue_H */
#define CY_PINS_LedBlue_H

#include "cytypes.h"
#include "cyfitter.h"
#include "LedBlue_aliases.h"


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
} LedBlue_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   LedBlue_Read(void);
void    LedBlue_Write(uint8 value);
uint8   LedBlue_ReadDataReg(void);
#if defined(LedBlue__PC) || (CY_PSOC4_4200L) 
    void    LedBlue_SetDriveMode(uint8 mode);
#endif
void    LedBlue_SetInterruptMode(uint16 position, uint16 mode);
uint8   LedBlue_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void LedBlue_Sleep(void); 
void LedBlue_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(LedBlue__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define LedBlue_DRIVE_MODE_BITS        (3)
    #define LedBlue_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - LedBlue_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the LedBlue_SetDriveMode() function.
         *  @{
         */
        #define LedBlue_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define LedBlue_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define LedBlue_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define LedBlue_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define LedBlue_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define LedBlue_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define LedBlue_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define LedBlue_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define LedBlue_MASK               LedBlue__MASK
#define LedBlue_SHIFT              LedBlue__SHIFT
#define LedBlue_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in LedBlue_SetInterruptMode() function.
     *  @{
     */
        #define LedBlue_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define LedBlue_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define LedBlue_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define LedBlue_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(LedBlue__SIO)
    #define LedBlue_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(LedBlue__PC) && (CY_PSOC4_4200L)
    #define LedBlue_USBIO_ENABLE               ((uint32)0x80000000u)
    #define LedBlue_USBIO_DISABLE              ((uint32)(~LedBlue_USBIO_ENABLE))
    #define LedBlue_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define LedBlue_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define LedBlue_USBIO_ENTER_SLEEP          ((uint32)((1u << LedBlue_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << LedBlue_USBIO_SUSPEND_DEL_SHIFT)))
    #define LedBlue_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << LedBlue_USBIO_SUSPEND_SHIFT)))
    #define LedBlue_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << LedBlue_USBIO_SUSPEND_DEL_SHIFT)))
    #define LedBlue_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(LedBlue__PC)
    /* Port Configuration */
    #define LedBlue_PC                 (* (reg32 *) LedBlue__PC)
#endif
/* Pin State */
#define LedBlue_PS                     (* (reg32 *) LedBlue__PS)
/* Data Register */
#define LedBlue_DR                     (* (reg32 *) LedBlue__DR)
/* Input Buffer Disable Override */
#define LedBlue_INP_DIS                (* (reg32 *) LedBlue__PC2)

/* Interrupt configuration Registers */
#define LedBlue_INTCFG                 (* (reg32 *) LedBlue__INTCFG)
#define LedBlue_INTSTAT                (* (reg32 *) LedBlue__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define LedBlue_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(LedBlue__SIO)
    #define LedBlue_SIO_REG            (* (reg32 *) LedBlue__SIO)
#endif /* (LedBlue__SIO_CFG) */

/* USBIO registers */
#if !defined(LedBlue__PC) && (CY_PSOC4_4200L)
    #define LedBlue_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define LedBlue_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define LedBlue_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define LedBlue_DRIVE_MODE_SHIFT       (0x00u)
#define LedBlue_DRIVE_MODE_MASK        (0x07u << LedBlue_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins LedBlue_H */


/* [] END OF FILE */
