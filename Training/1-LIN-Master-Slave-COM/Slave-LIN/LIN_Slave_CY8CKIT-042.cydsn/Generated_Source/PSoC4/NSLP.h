/*******************************************************************************
* File Name: NSLP.h  
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

#if !defined(CY_PINS_NSLP_H) /* Pins NSLP_H */
#define CY_PINS_NSLP_H

#include "cytypes.h"
#include "cyfitter.h"
#include "NSLP_aliases.h"


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
} NSLP_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   NSLP_Read(void);
void    NSLP_Write(uint8 value);
uint8   NSLP_ReadDataReg(void);
#if defined(NSLP__PC) || (CY_PSOC4_4200L) 
    void    NSLP_SetDriveMode(uint8 mode);
#endif
void    NSLP_SetInterruptMode(uint16 position, uint16 mode);
uint8   NSLP_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void NSLP_Sleep(void); 
void NSLP_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(NSLP__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define NSLP_DRIVE_MODE_BITS        (3)
    #define NSLP_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - NSLP_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the NSLP_SetDriveMode() function.
         *  @{
         */
        #define NSLP_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define NSLP_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define NSLP_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define NSLP_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define NSLP_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define NSLP_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define NSLP_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define NSLP_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define NSLP_MASK               NSLP__MASK
#define NSLP_SHIFT              NSLP__SHIFT
#define NSLP_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in NSLP_SetInterruptMode() function.
     *  @{
     */
        #define NSLP_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define NSLP_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define NSLP_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define NSLP_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(NSLP__SIO)
    #define NSLP_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(NSLP__PC) && (CY_PSOC4_4200L)
    #define NSLP_USBIO_ENABLE               ((uint32)0x80000000u)
    #define NSLP_USBIO_DISABLE              ((uint32)(~NSLP_USBIO_ENABLE))
    #define NSLP_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define NSLP_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define NSLP_USBIO_ENTER_SLEEP          ((uint32)((1u << NSLP_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << NSLP_USBIO_SUSPEND_DEL_SHIFT)))
    #define NSLP_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << NSLP_USBIO_SUSPEND_SHIFT)))
    #define NSLP_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << NSLP_USBIO_SUSPEND_DEL_SHIFT)))
    #define NSLP_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(NSLP__PC)
    /* Port Configuration */
    #define NSLP_PC                 (* (reg32 *) NSLP__PC)
#endif
/* Pin State */
#define NSLP_PS                     (* (reg32 *) NSLP__PS)
/* Data Register */
#define NSLP_DR                     (* (reg32 *) NSLP__DR)
/* Input Buffer Disable Override */
#define NSLP_INP_DIS                (* (reg32 *) NSLP__PC2)

/* Interrupt configuration Registers */
#define NSLP_INTCFG                 (* (reg32 *) NSLP__INTCFG)
#define NSLP_INTSTAT                (* (reg32 *) NSLP__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define NSLP_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(NSLP__SIO)
    #define NSLP_SIO_REG            (* (reg32 *) NSLP__SIO)
#endif /* (NSLP__SIO_CFG) */

/* USBIO registers */
#if !defined(NSLP__PC) && (CY_PSOC4_4200L)
    #define NSLP_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define NSLP_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define NSLP_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define NSLP_DRIVE_MODE_SHIFT       (0x00u)
#define NSLP_DRIVE_MODE_MASK        (0x07u << NSLP_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins NSLP_H */


/* [] END OF FILE */
