/*******************************************************************************
* File Name: LedGreen.h  
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

#if !defined(CY_PINS_LedGreen_H) /* Pins LedGreen_H */
#define CY_PINS_LedGreen_H

#include "cytypes.h"
#include "cyfitter.h"
#include "LedGreen_aliases.h"


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
} LedGreen_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   LedGreen_Read(void);
void    LedGreen_Write(uint8 value);
uint8   LedGreen_ReadDataReg(void);
#if defined(LedGreen__PC) || (CY_PSOC4_4200L) 
    void    LedGreen_SetDriveMode(uint8 mode);
#endif
void    LedGreen_SetInterruptMode(uint16 position, uint16 mode);
uint8   LedGreen_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void LedGreen_Sleep(void); 
void LedGreen_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(LedGreen__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define LedGreen_DRIVE_MODE_BITS        (3)
    #define LedGreen_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - LedGreen_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the LedGreen_SetDriveMode() function.
         *  @{
         */
        #define LedGreen_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define LedGreen_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define LedGreen_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define LedGreen_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define LedGreen_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define LedGreen_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define LedGreen_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define LedGreen_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define LedGreen_MASK               LedGreen__MASK
#define LedGreen_SHIFT              LedGreen__SHIFT
#define LedGreen_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in LedGreen_SetInterruptMode() function.
     *  @{
     */
        #define LedGreen_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define LedGreen_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define LedGreen_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define LedGreen_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(LedGreen__SIO)
    #define LedGreen_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(LedGreen__PC) && (CY_PSOC4_4200L)
    #define LedGreen_USBIO_ENABLE               ((uint32)0x80000000u)
    #define LedGreen_USBIO_DISABLE              ((uint32)(~LedGreen_USBIO_ENABLE))
    #define LedGreen_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define LedGreen_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define LedGreen_USBIO_ENTER_SLEEP          ((uint32)((1u << LedGreen_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << LedGreen_USBIO_SUSPEND_DEL_SHIFT)))
    #define LedGreen_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << LedGreen_USBIO_SUSPEND_SHIFT)))
    #define LedGreen_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << LedGreen_USBIO_SUSPEND_DEL_SHIFT)))
    #define LedGreen_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(LedGreen__PC)
    /* Port Configuration */
    #define LedGreen_PC                 (* (reg32 *) LedGreen__PC)
#endif
/* Pin State */
#define LedGreen_PS                     (* (reg32 *) LedGreen__PS)
/* Data Register */
#define LedGreen_DR                     (* (reg32 *) LedGreen__DR)
/* Input Buffer Disable Override */
#define LedGreen_INP_DIS                (* (reg32 *) LedGreen__PC2)

/* Interrupt configuration Registers */
#define LedGreen_INTCFG                 (* (reg32 *) LedGreen__INTCFG)
#define LedGreen_INTSTAT                (* (reg32 *) LedGreen__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define LedGreen_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(LedGreen__SIO)
    #define LedGreen_SIO_REG            (* (reg32 *) LedGreen__SIO)
#endif /* (LedGreen__SIO_CFG) */

/* USBIO registers */
#if !defined(LedGreen__PC) && (CY_PSOC4_4200L)
    #define LedGreen_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define LedGreen_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define LedGreen_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define LedGreen_DRIVE_MODE_SHIFT       (0x00u)
#define LedGreen_DRIVE_MODE_MASK        (0x07u << LedGreen_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins LedGreen_H */


/* [] END OF FILE */
