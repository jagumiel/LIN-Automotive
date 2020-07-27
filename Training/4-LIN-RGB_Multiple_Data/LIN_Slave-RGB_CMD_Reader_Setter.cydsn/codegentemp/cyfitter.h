/*******************************************************************************
* File Name: cyfitter.h
* 
* PSoC Creator  4.2
*
* Description:
* 
* This file is automatically generated by PSoC Creator.
*
********************************************************************************
* Copyright (c) 2007-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#ifndef INCLUDED_CYFITTER_H
#define INCLUDED_CYFITTER_H
#include "cydevice_trm.h"

/* LINS */
#define LINS_IntClk__DIVIDER_MASK 0x0000FFFFu
#define LINS_IntClk__ENABLE CYREG_CLK_DIVIDER_A00
#define LINS_IntClk__ENABLE_MASK 0x80000000u
#define LINS_IntClk__MASK 0x80000000u
#define LINS_IntClk__REGISTER CYREG_CLK_DIVIDER_A00
#define LINS_ISR__INTC_CLR_EN_REG CYREG_CM0_ICER
#define LINS_ISR__INTC_CLR_PD_REG CYREG_CM0_ICPR
#define LINS_ISR__INTC_MASK 0x800u
#define LINS_ISR__INTC_NUMBER 11u
#define LINS_ISR__INTC_PRIOR_MASK 0xC0000000u
#define LINS_ISR__INTC_PRIOR_NUM 3u
#define LINS_ISR__INTC_PRIOR_REG CYREG_CM0_IPR2
#define LINS_ISR__INTC_SET_EN_REG CYREG_CM0_ISER
#define LINS_ISR__INTC_SET_PD_REG CYREG_CM0_ISPR
#define LINS_SCB_rx__0__DM__MASK 0x7000u
#define LINS_SCB_rx__0__DM__SHIFT 12u
#define LINS_SCB_rx__0__DR CYREG_PRT0_DR
#define LINS_SCB_rx__0__HSIOM CYREG_HSIOM_PORT_SEL0
#define LINS_SCB_rx__0__HSIOM_GPIO 0u
#define LINS_SCB_rx__0__HSIOM_I2C 14u
#define LINS_SCB_rx__0__HSIOM_I2C_SCL 14u
#define LINS_SCB_rx__0__HSIOM_MASK 0x000F0000u
#define LINS_SCB_rx__0__HSIOM_SHIFT 16u
#define LINS_SCB_rx__0__HSIOM_SPI 15u
#define LINS_SCB_rx__0__HSIOM_SPI_MOSI 15u
#define LINS_SCB_rx__0__HSIOM_UART 9u
#define LINS_SCB_rx__0__HSIOM_UART_RX 9u
#define LINS_SCB_rx__0__INTCFG CYREG_PRT0_INTCFG
#define LINS_SCB_rx__0__INTSTAT CYREG_PRT0_INTSTAT
#define LINS_SCB_rx__0__MASK 0x10u
#define LINS_SCB_rx__0__PA__CFG0 CYREG_UDB_PA0_CFG0
#define LINS_SCB_rx__0__PA__CFG1 CYREG_UDB_PA0_CFG1
#define LINS_SCB_rx__0__PA__CFG10 CYREG_UDB_PA0_CFG10
#define LINS_SCB_rx__0__PA__CFG11 CYREG_UDB_PA0_CFG11
#define LINS_SCB_rx__0__PA__CFG12 CYREG_UDB_PA0_CFG12
#define LINS_SCB_rx__0__PA__CFG13 CYREG_UDB_PA0_CFG13
#define LINS_SCB_rx__0__PA__CFG14 CYREG_UDB_PA0_CFG14
#define LINS_SCB_rx__0__PA__CFG2 CYREG_UDB_PA0_CFG2
#define LINS_SCB_rx__0__PA__CFG3 CYREG_UDB_PA0_CFG3
#define LINS_SCB_rx__0__PA__CFG4 CYREG_UDB_PA0_CFG4
#define LINS_SCB_rx__0__PA__CFG5 CYREG_UDB_PA0_CFG5
#define LINS_SCB_rx__0__PA__CFG6 CYREG_UDB_PA0_CFG6
#define LINS_SCB_rx__0__PA__CFG7 CYREG_UDB_PA0_CFG7
#define LINS_SCB_rx__0__PA__CFG8 CYREG_UDB_PA0_CFG8
#define LINS_SCB_rx__0__PA__CFG9 CYREG_UDB_PA0_CFG9
#define LINS_SCB_rx__0__PC CYREG_PRT0_PC
#define LINS_SCB_rx__0__PC2 CYREG_PRT0_PC2
#define LINS_SCB_rx__0__PORT 0u
#define LINS_SCB_rx__0__PS CYREG_PRT0_PS
#define LINS_SCB_rx__0__SHIFT 4u
#define LINS_SCB_rx__DR CYREG_PRT0_DR
#define LINS_SCB_rx__INTCFG CYREG_PRT0_INTCFG
#define LINS_SCB_rx__INTSTAT CYREG_PRT0_INTSTAT
#define LINS_SCB_rx__MASK 0x10u
#define LINS_SCB_rx__PA__CFG0 CYREG_UDB_PA0_CFG0
#define LINS_SCB_rx__PA__CFG1 CYREG_UDB_PA0_CFG1
#define LINS_SCB_rx__PA__CFG10 CYREG_UDB_PA0_CFG10
#define LINS_SCB_rx__PA__CFG11 CYREG_UDB_PA0_CFG11
#define LINS_SCB_rx__PA__CFG12 CYREG_UDB_PA0_CFG12
#define LINS_SCB_rx__PA__CFG13 CYREG_UDB_PA0_CFG13
#define LINS_SCB_rx__PA__CFG14 CYREG_UDB_PA0_CFG14
#define LINS_SCB_rx__PA__CFG2 CYREG_UDB_PA0_CFG2
#define LINS_SCB_rx__PA__CFG3 CYREG_UDB_PA0_CFG3
#define LINS_SCB_rx__PA__CFG4 CYREG_UDB_PA0_CFG4
#define LINS_SCB_rx__PA__CFG5 CYREG_UDB_PA0_CFG5
#define LINS_SCB_rx__PA__CFG6 CYREG_UDB_PA0_CFG6
#define LINS_SCB_rx__PA__CFG7 CYREG_UDB_PA0_CFG7
#define LINS_SCB_rx__PA__CFG8 CYREG_UDB_PA0_CFG8
#define LINS_SCB_rx__PA__CFG9 CYREG_UDB_PA0_CFG9
#define LINS_SCB_rx__PC CYREG_PRT0_PC
#define LINS_SCB_rx__PC2 CYREG_PRT0_PC2
#define LINS_SCB_rx__PORT 0u
#define LINS_SCB_rx__PS CYREG_PRT0_PS
#define LINS_SCB_rx__SHIFT 4u
#define LINS_SCB_SCB__BIST_CONTROL CYREG_SCB1_BIST_CONTROL
#define LINS_SCB_SCB__BIST_DATA CYREG_SCB1_BIST_DATA
#define LINS_SCB_SCB__CTRL CYREG_SCB1_CTRL
#define LINS_SCB_SCB__EZ_DATA00 CYREG_SCB1_EZ_DATA00
#define LINS_SCB_SCB__EZ_DATA01 CYREG_SCB1_EZ_DATA01
#define LINS_SCB_SCB__EZ_DATA02 CYREG_SCB1_EZ_DATA02
#define LINS_SCB_SCB__EZ_DATA03 CYREG_SCB1_EZ_DATA03
#define LINS_SCB_SCB__EZ_DATA04 CYREG_SCB1_EZ_DATA04
#define LINS_SCB_SCB__EZ_DATA05 CYREG_SCB1_EZ_DATA05
#define LINS_SCB_SCB__EZ_DATA06 CYREG_SCB1_EZ_DATA06
#define LINS_SCB_SCB__EZ_DATA07 CYREG_SCB1_EZ_DATA07
#define LINS_SCB_SCB__EZ_DATA08 CYREG_SCB1_EZ_DATA08
#define LINS_SCB_SCB__EZ_DATA09 CYREG_SCB1_EZ_DATA09
#define LINS_SCB_SCB__EZ_DATA10 CYREG_SCB1_EZ_DATA10
#define LINS_SCB_SCB__EZ_DATA11 CYREG_SCB1_EZ_DATA11
#define LINS_SCB_SCB__EZ_DATA12 CYREG_SCB1_EZ_DATA12
#define LINS_SCB_SCB__EZ_DATA13 CYREG_SCB1_EZ_DATA13
#define LINS_SCB_SCB__EZ_DATA14 CYREG_SCB1_EZ_DATA14
#define LINS_SCB_SCB__EZ_DATA15 CYREG_SCB1_EZ_DATA15
#define LINS_SCB_SCB__EZ_DATA16 CYREG_SCB1_EZ_DATA16
#define LINS_SCB_SCB__EZ_DATA17 CYREG_SCB1_EZ_DATA17
#define LINS_SCB_SCB__EZ_DATA18 CYREG_SCB1_EZ_DATA18
#define LINS_SCB_SCB__EZ_DATA19 CYREG_SCB1_EZ_DATA19
#define LINS_SCB_SCB__EZ_DATA20 CYREG_SCB1_EZ_DATA20
#define LINS_SCB_SCB__EZ_DATA21 CYREG_SCB1_EZ_DATA21
#define LINS_SCB_SCB__EZ_DATA22 CYREG_SCB1_EZ_DATA22
#define LINS_SCB_SCB__EZ_DATA23 CYREG_SCB1_EZ_DATA23
#define LINS_SCB_SCB__EZ_DATA24 CYREG_SCB1_EZ_DATA24
#define LINS_SCB_SCB__EZ_DATA25 CYREG_SCB1_EZ_DATA25
#define LINS_SCB_SCB__EZ_DATA26 CYREG_SCB1_EZ_DATA26
#define LINS_SCB_SCB__EZ_DATA27 CYREG_SCB1_EZ_DATA27
#define LINS_SCB_SCB__EZ_DATA28 CYREG_SCB1_EZ_DATA28
#define LINS_SCB_SCB__EZ_DATA29 CYREG_SCB1_EZ_DATA29
#define LINS_SCB_SCB__EZ_DATA30 CYREG_SCB1_EZ_DATA30
#define LINS_SCB_SCB__EZ_DATA31 CYREG_SCB1_EZ_DATA31
#define LINS_SCB_SCB__I2C_CFG CYREG_SCB1_I2C_CFG
#define LINS_SCB_SCB__I2C_CTRL CYREG_SCB1_I2C_CTRL
#define LINS_SCB_SCB__I2C_M_CMD CYREG_SCB1_I2C_M_CMD
#define LINS_SCB_SCB__I2C_S_CMD CYREG_SCB1_I2C_S_CMD
#define LINS_SCB_SCB__I2C_STATUS CYREG_SCB1_I2C_STATUS
#define LINS_SCB_SCB__INTR_CAUSE CYREG_SCB1_INTR_CAUSE
#define LINS_SCB_SCB__INTR_I2C_EC CYREG_SCB1_INTR_I2C_EC
#define LINS_SCB_SCB__INTR_I2C_EC_MASK CYREG_SCB1_INTR_I2C_EC_MASK
#define LINS_SCB_SCB__INTR_I2C_EC_MASKED CYREG_SCB1_INTR_I2C_EC_MASKED
#define LINS_SCB_SCB__INTR_M CYREG_SCB1_INTR_M
#define LINS_SCB_SCB__INTR_M_MASK CYREG_SCB1_INTR_M_MASK
#define LINS_SCB_SCB__INTR_M_MASKED CYREG_SCB1_INTR_M_MASKED
#define LINS_SCB_SCB__INTR_M_SET CYREG_SCB1_INTR_M_SET
#define LINS_SCB_SCB__INTR_RX CYREG_SCB1_INTR_RX
#define LINS_SCB_SCB__INTR_RX_MASK CYREG_SCB1_INTR_RX_MASK
#define LINS_SCB_SCB__INTR_RX_MASKED CYREG_SCB1_INTR_RX_MASKED
#define LINS_SCB_SCB__INTR_RX_SET CYREG_SCB1_INTR_RX_SET
#define LINS_SCB_SCB__INTR_S CYREG_SCB1_INTR_S
#define LINS_SCB_SCB__INTR_S_MASK CYREG_SCB1_INTR_S_MASK
#define LINS_SCB_SCB__INTR_S_MASKED CYREG_SCB1_INTR_S_MASKED
#define LINS_SCB_SCB__INTR_S_SET CYREG_SCB1_INTR_S_SET
#define LINS_SCB_SCB__INTR_SPI_EC CYREG_SCB1_INTR_SPI_EC
#define LINS_SCB_SCB__INTR_SPI_EC_MASK CYREG_SCB1_INTR_SPI_EC_MASK
#define LINS_SCB_SCB__INTR_SPI_EC_MASKED CYREG_SCB1_INTR_SPI_EC_MASKED
#define LINS_SCB_SCB__INTR_TX CYREG_SCB1_INTR_TX
#define LINS_SCB_SCB__INTR_TX_MASK CYREG_SCB1_INTR_TX_MASK
#define LINS_SCB_SCB__INTR_TX_MASKED CYREG_SCB1_INTR_TX_MASKED
#define LINS_SCB_SCB__INTR_TX_SET CYREG_SCB1_INTR_TX_SET
#define LINS_SCB_SCB__RX_CTRL CYREG_SCB1_RX_CTRL
#define LINS_SCB_SCB__RX_FIFO_CTRL CYREG_SCB1_RX_FIFO_CTRL
#define LINS_SCB_SCB__RX_FIFO_RD CYREG_SCB1_RX_FIFO_RD
#define LINS_SCB_SCB__RX_FIFO_RD_SILENT CYREG_SCB1_RX_FIFO_RD_SILENT
#define LINS_SCB_SCB__RX_FIFO_STATUS CYREG_SCB1_RX_FIFO_STATUS
#define LINS_SCB_SCB__RX_MATCH CYREG_SCB1_RX_MATCH
#define LINS_SCB_SCB__SPI_CTRL CYREG_SCB1_SPI_CTRL
#define LINS_SCB_SCB__SPI_STATUS CYREG_SCB1_SPI_STATUS
#define LINS_SCB_SCB__SS0_POSISTION 0u
#define LINS_SCB_SCB__SS1_POSISTION 1u
#define LINS_SCB_SCB__SS2_POSISTION 2u
#define LINS_SCB_SCB__SS3_POSISTION 3u
#define LINS_SCB_SCB__STATUS CYREG_SCB1_STATUS
#define LINS_SCB_SCB__TX_CTRL CYREG_SCB1_TX_CTRL
#define LINS_SCB_SCB__TX_FIFO_CTRL CYREG_SCB1_TX_FIFO_CTRL
#define LINS_SCB_SCB__TX_FIFO_STATUS CYREG_SCB1_TX_FIFO_STATUS
#define LINS_SCB_SCB__TX_FIFO_WR CYREG_SCB1_TX_FIFO_WR
#define LINS_SCB_SCB__UART_CTRL CYREG_SCB1_UART_CTRL
#define LINS_SCB_SCB__UART_RX_CTRL CYREG_SCB1_UART_RX_CTRL
#define LINS_SCB_SCB__UART_RX_STATUS CYREG_SCB1_UART_RX_STATUS
#define LINS_SCB_SCB__UART_TX_CTRL CYREG_SCB1_UART_TX_CTRL
#define LINS_SCB_tx__0__DM__MASK 0x38000u
#define LINS_SCB_tx__0__DM__SHIFT 15u
#define LINS_SCB_tx__0__DR CYREG_PRT0_DR
#define LINS_SCB_tx__0__HSIOM CYREG_HSIOM_PORT_SEL0
#define LINS_SCB_tx__0__HSIOM_GPIO 0u
#define LINS_SCB_tx__0__HSIOM_I2C 14u
#define LINS_SCB_tx__0__HSIOM_I2C_SDA 14u
#define LINS_SCB_tx__0__HSIOM_MASK 0x00F00000u
#define LINS_SCB_tx__0__HSIOM_SHIFT 20u
#define LINS_SCB_tx__0__HSIOM_SPI 15u
#define LINS_SCB_tx__0__HSIOM_SPI_MISO 15u
#define LINS_SCB_tx__0__HSIOM_UART 9u
#define LINS_SCB_tx__0__HSIOM_UART_TX 9u
#define LINS_SCB_tx__0__INTCFG CYREG_PRT0_INTCFG
#define LINS_SCB_tx__0__INTSTAT CYREG_PRT0_INTSTAT
#define LINS_SCB_tx__0__MASK 0x20u
#define LINS_SCB_tx__0__OUT_SEL CYREG_UDB_PA0_CFG10
#define LINS_SCB_tx__0__OUT_SEL_SHIFT 10u
#define LINS_SCB_tx__0__OUT_SEL_VAL -1u
#define LINS_SCB_tx__0__PA__CFG0 CYREG_UDB_PA0_CFG0
#define LINS_SCB_tx__0__PA__CFG1 CYREG_UDB_PA0_CFG1
#define LINS_SCB_tx__0__PA__CFG10 CYREG_UDB_PA0_CFG10
#define LINS_SCB_tx__0__PA__CFG11 CYREG_UDB_PA0_CFG11
#define LINS_SCB_tx__0__PA__CFG12 CYREG_UDB_PA0_CFG12
#define LINS_SCB_tx__0__PA__CFG13 CYREG_UDB_PA0_CFG13
#define LINS_SCB_tx__0__PA__CFG14 CYREG_UDB_PA0_CFG14
#define LINS_SCB_tx__0__PA__CFG2 CYREG_UDB_PA0_CFG2
#define LINS_SCB_tx__0__PA__CFG3 CYREG_UDB_PA0_CFG3
#define LINS_SCB_tx__0__PA__CFG4 CYREG_UDB_PA0_CFG4
#define LINS_SCB_tx__0__PA__CFG5 CYREG_UDB_PA0_CFG5
#define LINS_SCB_tx__0__PA__CFG6 CYREG_UDB_PA0_CFG6
#define LINS_SCB_tx__0__PA__CFG7 CYREG_UDB_PA0_CFG7
#define LINS_SCB_tx__0__PA__CFG8 CYREG_UDB_PA0_CFG8
#define LINS_SCB_tx__0__PA__CFG9 CYREG_UDB_PA0_CFG9
#define LINS_SCB_tx__0__PC CYREG_PRT0_PC
#define LINS_SCB_tx__0__PC2 CYREG_PRT0_PC2
#define LINS_SCB_tx__0__PORT 0u
#define LINS_SCB_tx__0__PS CYREG_PRT0_PS
#define LINS_SCB_tx__0__SHIFT 5u
#define LINS_SCB_tx__DR CYREG_PRT0_DR
#define LINS_SCB_tx__INTCFG CYREG_PRT0_INTCFG
#define LINS_SCB_tx__INTSTAT CYREG_PRT0_INTSTAT
#define LINS_SCB_tx__MASK 0x20u
#define LINS_SCB_tx__PA__CFG0 CYREG_UDB_PA0_CFG0
#define LINS_SCB_tx__PA__CFG1 CYREG_UDB_PA0_CFG1
#define LINS_SCB_tx__PA__CFG10 CYREG_UDB_PA0_CFG10
#define LINS_SCB_tx__PA__CFG11 CYREG_UDB_PA0_CFG11
#define LINS_SCB_tx__PA__CFG12 CYREG_UDB_PA0_CFG12
#define LINS_SCB_tx__PA__CFG13 CYREG_UDB_PA0_CFG13
#define LINS_SCB_tx__PA__CFG14 CYREG_UDB_PA0_CFG14
#define LINS_SCB_tx__PA__CFG2 CYREG_UDB_PA0_CFG2
#define LINS_SCB_tx__PA__CFG3 CYREG_UDB_PA0_CFG3
#define LINS_SCB_tx__PA__CFG4 CYREG_UDB_PA0_CFG4
#define LINS_SCB_tx__PA__CFG5 CYREG_UDB_PA0_CFG5
#define LINS_SCB_tx__PA__CFG6 CYREG_UDB_PA0_CFG6
#define LINS_SCB_tx__PA__CFG7 CYREG_UDB_PA0_CFG7
#define LINS_SCB_tx__PA__CFG8 CYREG_UDB_PA0_CFG8
#define LINS_SCB_tx__PA__CFG9 CYREG_UDB_PA0_CFG9
#define LINS_SCB_tx__PC CYREG_PRT0_PC
#define LINS_SCB_tx__PC2 CYREG_PRT0_PC2
#define LINS_SCB_tx__PORT 0u
#define LINS_SCB_tx__PS CYREG_PRT0_PS
#define LINS_SCB_tx__SHIFT 5u

/* PWM_B */
#define PWM_B_PWMUDB_genblk1_ctrlreg__16BIT_CONTROL_AUX_CTL_REG CYREG_UDB_W16_ACTL_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__16BIT_CONTROL_CONTROL_REG CYREG_UDB_W16_CTL_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__16BIT_CONTROL_COUNT_REG CYREG_UDB_W16_CTL_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__16BIT_COUNT_CONTROL_REG CYREG_UDB_W16_CTL_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__16BIT_COUNT_COUNT_REG CYREG_UDB_W16_CTL_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__16BIT_MASK_MASK_REG CYREG_UDB_W16_MSK_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__16BIT_MASK_PERIOD_REG CYREG_UDB_W16_MSK_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__16BIT_PERIOD_MASK_REG CYREG_UDB_W16_MSK_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__16BIT_PERIOD_PERIOD_REG CYREG_UDB_W16_MSK_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__7__MASK 0x80u
#define PWM_B_PWMUDB_genblk1_ctrlreg__7__POS 7
#define PWM_B_PWMUDB_genblk1_ctrlreg__CONTROL_AUX_CTL_REG CYREG_UDB_W8_ACTL_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__CONTROL_REG CYREG_UDB_W8_CTL_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__CONTROL_ST_REG CYREG_UDB_CAT16_CTL_ST_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__COUNT_REG CYREG_UDB_W8_CTL_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__COUNT_ST_REG CYREG_UDB_CAT16_CTL_ST_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__MASK 0x80u
#define PWM_B_PWMUDB_genblk1_ctrlreg__MASK_CTL_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__PER_CTL_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_01
#define PWM_B_PWMUDB_genblk1_ctrlreg__PERIOD_REG CYREG_UDB_W8_MSK_01
#define PWM_B_PWMUDB_genblk8_stsreg__0__MASK 0x01u
#define PWM_B_PWMUDB_genblk8_stsreg__0__POS 0
#define PWM_B_PWMUDB_genblk8_stsreg__16BIT_STATUS_AUX_CTL_REG CYREG_UDB_W16_ACTL_01
#define PWM_B_PWMUDB_genblk8_stsreg__16BIT_STATUS_REG CYREG_UDB_W16_ST_01
#define PWM_B_PWMUDB_genblk8_stsreg__2__MASK 0x04u
#define PWM_B_PWMUDB_genblk8_stsreg__2__POS 2
#define PWM_B_PWMUDB_genblk8_stsreg__3__MASK 0x08u
#define PWM_B_PWMUDB_genblk8_stsreg__3__POS 3
#define PWM_B_PWMUDB_genblk8_stsreg__MASK 0x0Du
#define PWM_B_PWMUDB_genblk8_stsreg__MASK_REG CYREG_UDB_W8_MSK_01
#define PWM_B_PWMUDB_genblk8_stsreg__MASK_ST_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_01
#define PWM_B_PWMUDB_genblk8_stsreg__PER_ST_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_01
#define PWM_B_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG CYREG_UDB_W8_ACTL_01
#define PWM_B_PWMUDB_genblk8_stsreg__STATUS_CNT_REG CYREG_UDB_CAT16_CTL_ST_01
#define PWM_B_PWMUDB_genblk8_stsreg__STATUS_CONTROL_REG CYREG_UDB_CAT16_CTL_ST_01
#define PWM_B_PWMUDB_genblk8_stsreg__STATUS_REG CYREG_UDB_W8_ST_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG CYREG_UDB_W16_A0_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG CYREG_UDB_W16_A1_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG CYREG_UDB_W16_D0_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG CYREG_UDB_W16_D1_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__16BIT_DP_AUX_CTL_REG CYREG_UDB_W16_ACTL_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG CYREG_UDB_W16_F0_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG CYREG_UDB_W16_F1_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__A0_A1_REG CYREG_UDB_CAT16_A_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__A0_REG CYREG_UDB_W8_A0_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__A1_REG CYREG_UDB_W8_A1_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__D0_D1_REG CYREG_UDB_CAT16_D_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__D0_REG CYREG_UDB_W8_D0_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__D1_REG CYREG_UDB_W8_D1_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG CYREG_UDB_W8_ACTL_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__F0_F1_REG CYREG_UDB_CAT16_F_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__F0_REG CYREG_UDB_W8_F0_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__F1_REG CYREG_UDB_W8_F1_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__MSK_DP_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_01
#define PWM_B_PWMUDB_sP8_pwmdp_u0__PER_DP_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_01

/* PWM_G */
#define PWM_G_PWMUDB_genblk1_ctrlreg__16BIT_CONTROL_AUX_CTL_REG CYREG_UDB_W16_ACTL_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__16BIT_CONTROL_CONTROL_REG CYREG_UDB_W16_CTL_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__16BIT_CONTROL_COUNT_REG CYREG_UDB_W16_CTL_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__16BIT_COUNT_CONTROL_REG CYREG_UDB_W16_CTL_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__16BIT_COUNT_COUNT_REG CYREG_UDB_W16_CTL_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__16BIT_MASK_MASK_REG CYREG_UDB_W16_MSK_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__16BIT_MASK_PERIOD_REG CYREG_UDB_W16_MSK_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__16BIT_PERIOD_MASK_REG CYREG_UDB_W16_MSK_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__16BIT_PERIOD_PERIOD_REG CYREG_UDB_W16_MSK_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__32BIT_CONTROL_AUX_CTL_REG CYREG_UDB_W32_ACTL_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__32BIT_CONTROL_REG CYREG_UDB_W32_CTL_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__32BIT_COUNT_REG CYREG_UDB_W32_CTL_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__32BIT_PERIOD_REG CYREG_UDB_W32_MSK_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__7__MASK 0x80u
#define PWM_G_PWMUDB_genblk1_ctrlreg__7__POS 7
#define PWM_G_PWMUDB_genblk1_ctrlreg__CONTROL_AUX_CTL_REG CYREG_UDB_W8_ACTL_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__CONTROL_REG CYREG_UDB_W8_CTL_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__CONTROL_ST_REG CYREG_UDB_CAT16_CTL_ST_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__COUNT_REG CYREG_UDB_W8_CTL_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__COUNT_ST_REG CYREG_UDB_CAT16_CTL_ST_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__MASK 0x80u
#define PWM_G_PWMUDB_genblk1_ctrlreg__MASK_CTL_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__PER_CTL_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_00
#define PWM_G_PWMUDB_genblk1_ctrlreg__PERIOD_REG CYREG_UDB_W8_MSK_00
#define PWM_G_PWMUDB_genblk8_stsreg__0__MASK 0x01u
#define PWM_G_PWMUDB_genblk8_stsreg__0__POS 0
#define PWM_G_PWMUDB_genblk8_stsreg__16BIT_STATUS_AUX_CTL_REG CYREG_UDB_W16_ACTL_00
#define PWM_G_PWMUDB_genblk8_stsreg__16BIT_STATUS_REG CYREG_UDB_W16_ST_00
#define PWM_G_PWMUDB_genblk8_stsreg__2__MASK 0x04u
#define PWM_G_PWMUDB_genblk8_stsreg__2__POS 2
#define PWM_G_PWMUDB_genblk8_stsreg__3__MASK 0x08u
#define PWM_G_PWMUDB_genblk8_stsreg__3__POS 3
#define PWM_G_PWMUDB_genblk8_stsreg__32BIT_MASK_REG CYREG_UDB_W32_MSK_00
#define PWM_G_PWMUDB_genblk8_stsreg__32BIT_STATUS_AUX_CTL_REG CYREG_UDB_W32_ACTL_00
#define PWM_G_PWMUDB_genblk8_stsreg__32BIT_STATUS_REG CYREG_UDB_W32_ST_00
#define PWM_G_PWMUDB_genblk8_stsreg__MASK 0x0Du
#define PWM_G_PWMUDB_genblk8_stsreg__MASK_REG CYREG_UDB_W8_MSK_00
#define PWM_G_PWMUDB_genblk8_stsreg__MASK_ST_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_00
#define PWM_G_PWMUDB_genblk8_stsreg__PER_ST_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_00
#define PWM_G_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG CYREG_UDB_W8_ACTL_00
#define PWM_G_PWMUDB_genblk8_stsreg__STATUS_CNT_REG CYREG_UDB_CAT16_CTL_ST_00
#define PWM_G_PWMUDB_genblk8_stsreg__STATUS_CONTROL_REG CYREG_UDB_CAT16_CTL_ST_00
#define PWM_G_PWMUDB_genblk8_stsreg__STATUS_REG CYREG_UDB_W8_ST_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG CYREG_UDB_W16_A0_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG CYREG_UDB_W16_A1_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG CYREG_UDB_W16_D0_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG CYREG_UDB_W16_D1_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__16BIT_DP_AUX_CTL_REG CYREG_UDB_W16_ACTL_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG CYREG_UDB_W16_F0_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG CYREG_UDB_W16_F1_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__32BIT_A0_REG CYREG_UDB_W32_A0_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__32BIT_A1_REG CYREG_UDB_W32_A1_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__32BIT_D0_REG CYREG_UDB_W32_D0_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__32BIT_D1_REG CYREG_UDB_W32_D1_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__32BIT_DP_AUX_CTL_REG CYREG_UDB_W32_ACTL_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__32BIT_F0_REG CYREG_UDB_W32_F0_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__32BIT_F1_REG CYREG_UDB_W32_F1_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__A0_A1_REG CYREG_UDB_CAT16_A_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__A0_REG CYREG_UDB_W8_A0_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__A1_REG CYREG_UDB_W8_A1_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__D0_D1_REG CYREG_UDB_CAT16_D_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__D0_REG CYREG_UDB_W8_D0_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__D1_REG CYREG_UDB_W8_D1_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG CYREG_UDB_W8_ACTL_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__F0_F1_REG CYREG_UDB_CAT16_F_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__F0_REG CYREG_UDB_W8_F0_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__F1_REG CYREG_UDB_W8_F1_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__MSK_DP_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_00
#define PWM_G_PWMUDB_sP8_pwmdp_u0__PER_DP_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_00

/* PWM_R */
#define PWM_R_PWMUDB_genblk1_ctrlreg__16BIT_CONTROL_AUX_CTL_REG CYREG_UDB_W16_ACTL_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__16BIT_CONTROL_CONTROL_REG CYREG_UDB_W16_CTL_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__16BIT_CONTROL_COUNT_REG CYREG_UDB_W16_CTL_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__16BIT_COUNT_CONTROL_REG CYREG_UDB_W16_CTL_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__16BIT_COUNT_COUNT_REG CYREG_UDB_W16_CTL_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__16BIT_MASK_MASK_REG CYREG_UDB_W16_MSK_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__16BIT_MASK_PERIOD_REG CYREG_UDB_W16_MSK_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__16BIT_PERIOD_MASK_REG CYREG_UDB_W16_MSK_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__16BIT_PERIOD_PERIOD_REG CYREG_UDB_W16_MSK_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__7__MASK 0x80u
#define PWM_R_PWMUDB_genblk1_ctrlreg__7__POS 7
#define PWM_R_PWMUDB_genblk1_ctrlreg__CONTROL_AUX_CTL_REG CYREG_UDB_W8_ACTL_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__CONTROL_REG CYREG_UDB_W8_CTL_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__CONTROL_ST_REG CYREG_UDB_CAT16_CTL_ST_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__COUNT_REG CYREG_UDB_W8_CTL_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__COUNT_ST_REG CYREG_UDB_CAT16_CTL_ST_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__MASK 0x80u
#define PWM_R_PWMUDB_genblk1_ctrlreg__MASK_CTL_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__PER_CTL_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_02
#define PWM_R_PWMUDB_genblk1_ctrlreg__PERIOD_REG CYREG_UDB_W8_MSK_02
#define PWM_R_PWMUDB_genblk8_stsreg__0__MASK 0x01u
#define PWM_R_PWMUDB_genblk8_stsreg__0__POS 0
#define PWM_R_PWMUDB_genblk8_stsreg__16BIT_STATUS_AUX_CTL_REG CYREG_UDB_W16_ACTL_02
#define PWM_R_PWMUDB_genblk8_stsreg__16BIT_STATUS_REG CYREG_UDB_W16_ST_02
#define PWM_R_PWMUDB_genblk8_stsreg__2__MASK 0x04u
#define PWM_R_PWMUDB_genblk8_stsreg__2__POS 2
#define PWM_R_PWMUDB_genblk8_stsreg__3__MASK 0x08u
#define PWM_R_PWMUDB_genblk8_stsreg__3__POS 3
#define PWM_R_PWMUDB_genblk8_stsreg__MASK 0x0Du
#define PWM_R_PWMUDB_genblk8_stsreg__MASK_REG CYREG_UDB_W8_MSK_02
#define PWM_R_PWMUDB_genblk8_stsreg__MASK_ST_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_02
#define PWM_R_PWMUDB_genblk8_stsreg__PER_ST_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_02
#define PWM_R_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG CYREG_UDB_W8_ACTL_02
#define PWM_R_PWMUDB_genblk8_stsreg__STATUS_CNT_REG CYREG_UDB_CAT16_CTL_ST_02
#define PWM_R_PWMUDB_genblk8_stsreg__STATUS_CONTROL_REG CYREG_UDB_CAT16_CTL_ST_02
#define PWM_R_PWMUDB_genblk8_stsreg__STATUS_REG CYREG_UDB_W8_ST_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG CYREG_UDB_W16_A0_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG CYREG_UDB_W16_A1_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG CYREG_UDB_W16_D0_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG CYREG_UDB_W16_D1_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__16BIT_DP_AUX_CTL_REG CYREG_UDB_W16_ACTL_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG CYREG_UDB_W16_F0_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG CYREG_UDB_W16_F1_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__A0_A1_REG CYREG_UDB_CAT16_A_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__A0_REG CYREG_UDB_W8_A0_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__A1_REG CYREG_UDB_W8_A1_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__D0_D1_REG CYREG_UDB_CAT16_D_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__D0_REG CYREG_UDB_W8_D0_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__D1_REG CYREG_UDB_W8_D1_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG CYREG_UDB_W8_ACTL_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__F0_F1_REG CYREG_UDB_CAT16_F_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__F0_REG CYREG_UDB_W8_F0_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__F1_REG CYREG_UDB_W8_F1_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__MSK_DP_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_02
#define PWM_R_PWMUDB_sP8_pwmdp_u0__PER_DP_AUX_CTL_REG CYREG_UDB_CAT16_ACTL_MSK_02

/* LedRed */
#define LedRed__0__DM__MASK 0x1C0000u
#define LedRed__0__DM__SHIFT 18u
#define LedRed__0__DR CYREG_PRT1_DR
#define LedRed__0__HSIOM CYREG_HSIOM_PORT_SEL1
#define LedRed__0__HSIOM_MASK 0x0F000000u
#define LedRed__0__HSIOM_SHIFT 24u
#define LedRed__0__INTCFG CYREG_PRT1_INTCFG
#define LedRed__0__INTSTAT CYREG_PRT1_INTSTAT
#define LedRed__0__MASK 0x40u
#define LedRed__0__OUT_SEL CYREG_UDB_PA1_CFG10
#define LedRed__0__OUT_SEL_SHIFT 12u
#define LedRed__0__OUT_SEL_VAL 0u
#define LedRed__0__PA__CFG0 CYREG_UDB_PA1_CFG0
#define LedRed__0__PA__CFG1 CYREG_UDB_PA1_CFG1
#define LedRed__0__PA__CFG10 CYREG_UDB_PA1_CFG10
#define LedRed__0__PA__CFG11 CYREG_UDB_PA1_CFG11
#define LedRed__0__PA__CFG12 CYREG_UDB_PA1_CFG12
#define LedRed__0__PA__CFG13 CYREG_UDB_PA1_CFG13
#define LedRed__0__PA__CFG14 CYREG_UDB_PA1_CFG14
#define LedRed__0__PA__CFG2 CYREG_UDB_PA1_CFG2
#define LedRed__0__PA__CFG3 CYREG_UDB_PA1_CFG3
#define LedRed__0__PA__CFG4 CYREG_UDB_PA1_CFG4
#define LedRed__0__PA__CFG5 CYREG_UDB_PA1_CFG5
#define LedRed__0__PA__CFG6 CYREG_UDB_PA1_CFG6
#define LedRed__0__PA__CFG7 CYREG_UDB_PA1_CFG7
#define LedRed__0__PA__CFG8 CYREG_UDB_PA1_CFG8
#define LedRed__0__PA__CFG9 CYREG_UDB_PA1_CFG9
#define LedRed__0__PC CYREG_PRT1_PC
#define LedRed__0__PC2 CYREG_PRT1_PC2
#define LedRed__0__PORT 1u
#define LedRed__0__PS CYREG_PRT1_PS
#define LedRed__0__SHIFT 6u
#define LedRed__DR CYREG_PRT1_DR
#define LedRed__INTCFG CYREG_PRT1_INTCFG
#define LedRed__INTSTAT CYREG_PRT1_INTSTAT
#define LedRed__MASK 0x40u
#define LedRed__PA__CFG0 CYREG_UDB_PA1_CFG0
#define LedRed__PA__CFG1 CYREG_UDB_PA1_CFG1
#define LedRed__PA__CFG10 CYREG_UDB_PA1_CFG10
#define LedRed__PA__CFG11 CYREG_UDB_PA1_CFG11
#define LedRed__PA__CFG12 CYREG_UDB_PA1_CFG12
#define LedRed__PA__CFG13 CYREG_UDB_PA1_CFG13
#define LedRed__PA__CFG14 CYREG_UDB_PA1_CFG14
#define LedRed__PA__CFG2 CYREG_UDB_PA1_CFG2
#define LedRed__PA__CFG3 CYREG_UDB_PA1_CFG3
#define LedRed__PA__CFG4 CYREG_UDB_PA1_CFG4
#define LedRed__PA__CFG5 CYREG_UDB_PA1_CFG5
#define LedRed__PA__CFG6 CYREG_UDB_PA1_CFG6
#define LedRed__PA__CFG7 CYREG_UDB_PA1_CFG7
#define LedRed__PA__CFG8 CYREG_UDB_PA1_CFG8
#define LedRed__PA__CFG9 CYREG_UDB_PA1_CFG9
#define LedRed__PC CYREG_PRT1_PC
#define LedRed__PC2 CYREG_PRT1_PC2
#define LedRed__PORT 1u
#define LedRed__PS CYREG_PRT1_PS
#define LedRed__SHIFT 6u

/* LedBlue */
#define LedBlue__0__DM__MASK 0xE00u
#define LedBlue__0__DM__SHIFT 9u
#define LedBlue__0__DR CYREG_PRT0_DR
#define LedBlue__0__HSIOM CYREG_HSIOM_PORT_SEL0
#define LedBlue__0__HSIOM_MASK 0x0000F000u
#define LedBlue__0__HSIOM_SHIFT 12u
#define LedBlue__0__INTCFG CYREG_PRT0_INTCFG
#define LedBlue__0__INTSTAT CYREG_PRT0_INTSTAT
#define LedBlue__0__MASK 0x08u
#define LedBlue__0__OUT_SEL CYREG_UDB_PA0_CFG10
#define LedBlue__0__OUT_SEL_SHIFT 6u
#define LedBlue__0__OUT_SEL_VAL 3u
#define LedBlue__0__PA__CFG0 CYREG_UDB_PA0_CFG0
#define LedBlue__0__PA__CFG1 CYREG_UDB_PA0_CFG1
#define LedBlue__0__PA__CFG10 CYREG_UDB_PA0_CFG10
#define LedBlue__0__PA__CFG11 CYREG_UDB_PA0_CFG11
#define LedBlue__0__PA__CFG12 CYREG_UDB_PA0_CFG12
#define LedBlue__0__PA__CFG13 CYREG_UDB_PA0_CFG13
#define LedBlue__0__PA__CFG14 CYREG_UDB_PA0_CFG14
#define LedBlue__0__PA__CFG2 CYREG_UDB_PA0_CFG2
#define LedBlue__0__PA__CFG3 CYREG_UDB_PA0_CFG3
#define LedBlue__0__PA__CFG4 CYREG_UDB_PA0_CFG4
#define LedBlue__0__PA__CFG5 CYREG_UDB_PA0_CFG5
#define LedBlue__0__PA__CFG6 CYREG_UDB_PA0_CFG6
#define LedBlue__0__PA__CFG7 CYREG_UDB_PA0_CFG7
#define LedBlue__0__PA__CFG8 CYREG_UDB_PA0_CFG8
#define LedBlue__0__PA__CFG9 CYREG_UDB_PA0_CFG9
#define LedBlue__0__PC CYREG_PRT0_PC
#define LedBlue__0__PC2 CYREG_PRT0_PC2
#define LedBlue__0__PORT 0u
#define LedBlue__0__PS CYREG_PRT0_PS
#define LedBlue__0__SHIFT 3u
#define LedBlue__DR CYREG_PRT0_DR
#define LedBlue__INTCFG CYREG_PRT0_INTCFG
#define LedBlue__INTSTAT CYREG_PRT0_INTSTAT
#define LedBlue__MASK 0x08u
#define LedBlue__PA__CFG0 CYREG_UDB_PA0_CFG0
#define LedBlue__PA__CFG1 CYREG_UDB_PA0_CFG1
#define LedBlue__PA__CFG10 CYREG_UDB_PA0_CFG10
#define LedBlue__PA__CFG11 CYREG_UDB_PA0_CFG11
#define LedBlue__PA__CFG12 CYREG_UDB_PA0_CFG12
#define LedBlue__PA__CFG13 CYREG_UDB_PA0_CFG13
#define LedBlue__PA__CFG14 CYREG_UDB_PA0_CFG14
#define LedBlue__PA__CFG2 CYREG_UDB_PA0_CFG2
#define LedBlue__PA__CFG3 CYREG_UDB_PA0_CFG3
#define LedBlue__PA__CFG4 CYREG_UDB_PA0_CFG4
#define LedBlue__PA__CFG5 CYREG_UDB_PA0_CFG5
#define LedBlue__PA__CFG6 CYREG_UDB_PA0_CFG6
#define LedBlue__PA__CFG7 CYREG_UDB_PA0_CFG7
#define LedBlue__PA__CFG8 CYREG_UDB_PA0_CFG8
#define LedBlue__PA__CFG9 CYREG_UDB_PA0_CFG9
#define LedBlue__PC CYREG_PRT0_PC
#define LedBlue__PC2 CYREG_PRT0_PC2
#define LedBlue__PORT 0u
#define LedBlue__PS CYREG_PRT0_PS
#define LedBlue__SHIFT 3u

/* LedGreen */
#define LedGreen__0__DM__MASK 0x1C0u
#define LedGreen__0__DM__SHIFT 6u
#define LedGreen__0__DR CYREG_PRT0_DR
#define LedGreen__0__HSIOM CYREG_HSIOM_PORT_SEL0
#define LedGreen__0__HSIOM_MASK 0x00000F00u
#define LedGreen__0__HSIOM_SHIFT 8u
#define LedGreen__0__INTCFG CYREG_PRT0_INTCFG
#define LedGreen__0__INTSTAT CYREG_PRT0_INTSTAT
#define LedGreen__0__MASK 0x04u
#define LedGreen__0__OUT_SEL CYREG_UDB_PA0_CFG10
#define LedGreen__0__OUT_SEL_SHIFT 4u
#define LedGreen__0__OUT_SEL_VAL 0u
#define LedGreen__0__PA__CFG0 CYREG_UDB_PA0_CFG0
#define LedGreen__0__PA__CFG1 CYREG_UDB_PA0_CFG1
#define LedGreen__0__PA__CFG10 CYREG_UDB_PA0_CFG10
#define LedGreen__0__PA__CFG11 CYREG_UDB_PA0_CFG11
#define LedGreen__0__PA__CFG12 CYREG_UDB_PA0_CFG12
#define LedGreen__0__PA__CFG13 CYREG_UDB_PA0_CFG13
#define LedGreen__0__PA__CFG14 CYREG_UDB_PA0_CFG14
#define LedGreen__0__PA__CFG2 CYREG_UDB_PA0_CFG2
#define LedGreen__0__PA__CFG3 CYREG_UDB_PA0_CFG3
#define LedGreen__0__PA__CFG4 CYREG_UDB_PA0_CFG4
#define LedGreen__0__PA__CFG5 CYREG_UDB_PA0_CFG5
#define LedGreen__0__PA__CFG6 CYREG_UDB_PA0_CFG6
#define LedGreen__0__PA__CFG7 CYREG_UDB_PA0_CFG7
#define LedGreen__0__PA__CFG8 CYREG_UDB_PA0_CFG8
#define LedGreen__0__PA__CFG9 CYREG_UDB_PA0_CFG9
#define LedGreen__0__PC CYREG_PRT0_PC
#define LedGreen__0__PC2 CYREG_PRT0_PC2
#define LedGreen__0__PORT 0u
#define LedGreen__0__PS CYREG_PRT0_PS
#define LedGreen__0__SHIFT 2u
#define LedGreen__DR CYREG_PRT0_DR
#define LedGreen__INTCFG CYREG_PRT0_INTCFG
#define LedGreen__INTSTAT CYREG_PRT0_INTSTAT
#define LedGreen__MASK 0x04u
#define LedGreen__PA__CFG0 CYREG_UDB_PA0_CFG0
#define LedGreen__PA__CFG1 CYREG_UDB_PA0_CFG1
#define LedGreen__PA__CFG10 CYREG_UDB_PA0_CFG10
#define LedGreen__PA__CFG11 CYREG_UDB_PA0_CFG11
#define LedGreen__PA__CFG12 CYREG_UDB_PA0_CFG12
#define LedGreen__PA__CFG13 CYREG_UDB_PA0_CFG13
#define LedGreen__PA__CFG14 CYREG_UDB_PA0_CFG14
#define LedGreen__PA__CFG2 CYREG_UDB_PA0_CFG2
#define LedGreen__PA__CFG3 CYREG_UDB_PA0_CFG3
#define LedGreen__PA__CFG4 CYREG_UDB_PA0_CFG4
#define LedGreen__PA__CFG5 CYREG_UDB_PA0_CFG5
#define LedGreen__PA__CFG6 CYREG_UDB_PA0_CFG6
#define LedGreen__PA__CFG7 CYREG_UDB_PA0_CFG7
#define LedGreen__PA__CFG8 CYREG_UDB_PA0_CFG8
#define LedGreen__PA__CFG9 CYREG_UDB_PA0_CFG9
#define LedGreen__PC CYREG_PRT0_PC
#define LedGreen__PC2 CYREG_PRT0_PC2
#define LedGreen__PORT 0u
#define LedGreen__PS CYREG_PRT0_PS
#define LedGreen__SHIFT 2u

/* Clock_50kHz */
#define Clock_50kHz__DIVIDER_MASK 0x0000FFFFu
#define Clock_50kHz__ENABLE CYREG_CLK_DIVIDER_B00
#define Clock_50kHz__ENABLE_MASK 0x80000000u
#define Clock_50kHz__MASK 0x80000000u
#define Clock_50kHz__REGISTER CYREG_CLK_DIVIDER_B00

/* Miscellaneous */
#define CY_PROJECT_NAME "LIN_Slave-RGB_CMD_Reader_Setter"
#define CY_VERSION "PSoC Creator  4.2"
#define CYDEV_BANDGAP_VOLTAGE 1.024
#define CYDEV_BCLK__HFCLK__HZ 48000000U
#define CYDEV_BCLK__HFCLK__KHZ 48000U
#define CYDEV_BCLK__HFCLK__MHZ 48U
#define CYDEV_BCLK__SYSCLK__HZ 48000000U
#define CYDEV_BCLK__SYSCLK__KHZ 48000U
#define CYDEV_BCLK__SYSCLK__MHZ 48U
#define CYDEV_CHIP_DIE_LEOPARD 1u
#define CYDEV_CHIP_DIE_PSOC4A 18u
#define CYDEV_CHIP_DIE_PSOC5LP 2u
#define CYDEV_CHIP_DIE_PSOC5TM 3u
#define CYDEV_CHIP_DIE_TMA4 4u
#define CYDEV_CHIP_DIE_UNKNOWN 0u
#define CYDEV_CHIP_FAMILY_FM0P 5u
#define CYDEV_CHIP_FAMILY_FM3 6u
#define CYDEV_CHIP_FAMILY_FM4 7u
#define CYDEV_CHIP_FAMILY_PSOC3 1u
#define CYDEV_CHIP_FAMILY_PSOC4 2u
#define CYDEV_CHIP_FAMILY_PSOC5 3u
#define CYDEV_CHIP_FAMILY_PSOC6 4u
#define CYDEV_CHIP_FAMILY_UNKNOWN 0u
#define CYDEV_CHIP_FAMILY_USED CYDEV_CHIP_FAMILY_PSOC4
#define CYDEV_CHIP_JTAG_ID 0x04C81193u
#define CYDEV_CHIP_MEMBER_3A 1u
#define CYDEV_CHIP_MEMBER_4A 18u
#define CYDEV_CHIP_MEMBER_4D 13u
#define CYDEV_CHIP_MEMBER_4E 6u
#define CYDEV_CHIP_MEMBER_4F 19u
#define CYDEV_CHIP_MEMBER_4G 4u
#define CYDEV_CHIP_MEMBER_4H 17u
#define CYDEV_CHIP_MEMBER_4I 23u
#define CYDEV_CHIP_MEMBER_4J 14u
#define CYDEV_CHIP_MEMBER_4K 15u
#define CYDEV_CHIP_MEMBER_4L 22u
#define CYDEV_CHIP_MEMBER_4M 21u
#define CYDEV_CHIP_MEMBER_4N 10u
#define CYDEV_CHIP_MEMBER_4O 7u
#define CYDEV_CHIP_MEMBER_4P 20u
#define CYDEV_CHIP_MEMBER_4Q 12u
#define CYDEV_CHIP_MEMBER_4R 8u
#define CYDEV_CHIP_MEMBER_4S 11u
#define CYDEV_CHIP_MEMBER_4T 9u
#define CYDEV_CHIP_MEMBER_4U 5u
#define CYDEV_CHIP_MEMBER_4V 16u
#define CYDEV_CHIP_MEMBER_5A 3u
#define CYDEV_CHIP_MEMBER_5B 2u
#define CYDEV_CHIP_MEMBER_6A 24u
#define CYDEV_CHIP_MEMBER_FM3 28u
#define CYDEV_CHIP_MEMBER_FM4 29u
#define CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE1 25u
#define CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE2 26u
#define CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE3 27u
#define CYDEV_CHIP_MEMBER_UNKNOWN 0u
#define CYDEV_CHIP_MEMBER_USED CYDEV_CHIP_MEMBER_4A
#define CYDEV_CHIP_DIE_EXPECT CYDEV_CHIP_MEMBER_USED
#define CYDEV_CHIP_DIE_ACTUAL CYDEV_CHIP_DIE_EXPECT
#define CYDEV_CHIP_REV_LEOPARD_ES1 0u
#define CYDEV_CHIP_REV_LEOPARD_ES2 1u
#define CYDEV_CHIP_REV_LEOPARD_ES3 3u
#define CYDEV_CHIP_REV_LEOPARD_PRODUCTION 3u
#define CYDEV_CHIP_REV_PSOC4A_ES0 17u
#define CYDEV_CHIP_REV_PSOC4A_PRODUCTION 17u
#define CYDEV_CHIP_REV_PSOC5LP_ES0 0u
#define CYDEV_CHIP_REV_PSOC5LP_PRODUCTION 0u
#define CYDEV_CHIP_REV_PSOC5TM_ES0 0u
#define CYDEV_CHIP_REV_PSOC5TM_ES1 1u
#define CYDEV_CHIP_REV_PSOC5TM_PRODUCTION 1u
#define CYDEV_CHIP_REV_TMA4_ES 17u
#define CYDEV_CHIP_REV_TMA4_ES2 33u
#define CYDEV_CHIP_REV_TMA4_PRODUCTION 17u
#define CYDEV_CHIP_REVISION_3A_ES1 0u
#define CYDEV_CHIP_REVISION_3A_ES2 1u
#define CYDEV_CHIP_REVISION_3A_ES3 3u
#define CYDEV_CHIP_REVISION_3A_PRODUCTION 3u
#define CYDEV_CHIP_REVISION_4A_ES0 17u
#define CYDEV_CHIP_REVISION_4A_PRODUCTION 17u
#define CYDEV_CHIP_REVISION_4D_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4E_CCG2_NO_USBPD 0u
#define CYDEV_CHIP_REVISION_4E_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4F_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4F_PRODUCTION_256DMA 0u
#define CYDEV_CHIP_REVISION_4F_PRODUCTION_256K 0u
#define CYDEV_CHIP_REVISION_4G_ES 17u
#define CYDEV_CHIP_REVISION_4G_ES2 33u
#define CYDEV_CHIP_REVISION_4G_PRODUCTION 17u
#define CYDEV_CHIP_REVISION_4H_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4I_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4J_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4K_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4L_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4M_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4N_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4O_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4P_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4Q_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4R_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4S_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4T_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4U_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4V_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_5A_ES0 0u
#define CYDEV_CHIP_REVISION_5A_ES1 1u
#define CYDEV_CHIP_REVISION_5A_PRODUCTION 1u
#define CYDEV_CHIP_REVISION_5B_ES0 0u
#define CYDEV_CHIP_REVISION_5B_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_6A_ES 17u
#define CYDEV_CHIP_REVISION_6A_NO_UDB 33u
#define CYDEV_CHIP_REVISION_6A_PRODUCTION 33u
#define CYDEV_CHIP_REVISION_FM3_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_FM4_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_PDL_FM0P_TYPE1_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_PDL_FM0P_TYPE2_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_PDL_FM0P_TYPE3_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_USED CYDEV_CHIP_REVISION_4A_PRODUCTION
#define CYDEV_CHIP_REV_EXPECT CYDEV_CHIP_REVISION_USED
#define CYDEV_CONFIG_READ_ACCELERATOR 1
#define CYDEV_CONFIG_UNUSED_IO_AllowButWarn 0
#define CYDEV_CONFIG_UNUSED_IO CYDEV_CONFIG_UNUSED_IO_AllowButWarn
#define CYDEV_CONFIG_UNUSED_IO_AllowWithInfo 1
#define CYDEV_CONFIG_UNUSED_IO_Disallowed 2
#define CYDEV_CONFIGURATION_COMPRESSED 1
#define CYDEV_CONFIGURATION_MODE_COMPRESSED 0
#define CYDEV_CONFIGURATION_MODE CYDEV_CONFIGURATION_MODE_COMPRESSED
#define CYDEV_CONFIGURATION_MODE_DMA 2
#define CYDEV_CONFIGURATION_MODE_UNCOMPRESSED 1
#define CYDEV_DEBUG_PROTECT_KILL 4
#define CYDEV_DEBUG_PROTECT_OPEN 1
#define CYDEV_DEBUG_PROTECT CYDEV_DEBUG_PROTECT_OPEN
#define CYDEV_DEBUG_PROTECT_PROTECTED 2
#define CYDEV_DEBUGGING_DPS_Disable 3
#define CYDEV_DEBUGGING_DPS_SWD 2
#define CYDEV_DEBUGGING_DPS CYDEV_DEBUGGING_DPS_SWD
#define CYDEV_DEBUGGING_ENABLE 1
#define CYDEV_DFT_SELECT_CLK0 1u
#define CYDEV_DFT_SELECT_CLK1 2u
#define CYDEV_HEAP_SIZE 0x80
#define CYDEV_IMO_TRIMMED_BY_USB 0u
#define CYDEV_IMO_TRIMMED_BY_WCO 0u
#define CYDEV_IS_EXPORTING_CODE 0
#define CYDEV_IS_IMPORTING_CODE 0
#define CYDEV_PROJ_TYPE 0
#define CYDEV_PROJ_TYPE_BOOTLOADER 1
#define CYDEV_PROJ_TYPE_LAUNCHER 5
#define CYDEV_PROJ_TYPE_LOADABLE 2
#define CYDEV_PROJ_TYPE_LOADABLEANDBOOTLOADER 4
#define CYDEV_PROJ_TYPE_MULTIAPPBOOTLOADER 3
#define CYDEV_PROJ_TYPE_STANDARD 0
#define CYDEV_STACK_SIZE 0x0800
#define CYDEV_USE_BUNDLED_CMSIS 1
#define CYDEV_VARIABLE_VDDA 1
#define CYDEV_VDDA 3.3
#define CYDEV_VDDA_MV 3300
#define CYDEV_VDDD 3.3
#define CYDEV_VDDD_MV 3300
#define CYDEV_WDT_GENERATE_ISR 1u
#define CYIPBLOCK_M0S8_CTBM_VERSION 0
#define CYIPBLOCK_m0s8cpuss_VERSION 0
#define CYIPBLOCK_m0s8csd_VERSION 0
#define CYIPBLOCK_m0s8gpio2_VERSION 0
#define CYIPBLOCK_m0s8hsiom4a_VERSION 0
#define CYIPBLOCK_m0s8lcd_VERSION 0
#define CYIPBLOCK_m0s8lpcomp_VERSION 0
#define CYIPBLOCK_m0s8pclk_VERSION 0
#define CYIPBLOCK_m0s8sar_VERSION 0
#define CYIPBLOCK_m0s8scb_VERSION 0
#define CYIPBLOCK_m0s8srssv2_VERSION 1
#define CYIPBLOCK_m0s8tcpwm_VERSION 0
#define CYIPBLOCK_m0s8udbif_VERSION 0
#define CYIPBLOCK_S8_GPIO_VERSION 2
#define CYDEV_BOOTLOADER_ENABLE 0

#endif /* INCLUDED_CYFITTER_H */
