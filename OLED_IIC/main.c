/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/******************************************************************************
 *  MSP432P4 I2C - EUSCI_B0 I2C Master TX/RX bytes to MSP432P4 Slave-Repeated Start
 *
 *  For use with the i2c_master_rw_repeated_start_singlebyte_slave project
 *
 *  Description: This demo connects two MSP432's via the I2C bus. This is the
 *  MASTER CODE.The master transmits 2B to the slave, then receives a byte.
 *  The master continuously transmits two bytes to the slave followed by a
 *  repeated start and a single-byte read from the slave.
 *  This is a common operation for bytes from I2C slave devices such as
 *  external EEPROMs.
 *  The transaction for the I2C that is written looks as follows:
 *  ________________________________________________________
 *  |  Start    | WData | WData | Start    | RData  |       |
 *  | 0x48 Addr | 0x042 | 0x041 |0x48 Addr |  0x52  | Stop  |
 *  |    W      |       |       |    R     |        |       |
 *  |___________|_______|_______|__________|________|_______|
 *
 *  ACLK = n/a, MCLK = HSMCLK = SMCLK = BRCLK = default DCO = ~3.0MHz
 *
 *                                /|\  /|\
 *                MSP432P411      10k  10k      MSP432P411
 *                   slave         |    |         master
 *             -----------------   |    |   -----------------
 *            |     P1.6/UCB0SDA|<-|----+->|P1.6/UCB0SDA     |
 *            |                 |  |       |                 |
 *            |                 |  |       |                 |
 *            |     P1.7/UCB0SCL|<-+------>|P1.7/UCB0SCL     |
 *            |                 |          |             P1.0|-- Error
 *            |                 |          |             P2.1|-- Activity
 *
 *****************************************************************************/
/* DriverLib Defines */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Defines */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "OLED.h"
/* Slave Address for I2C Slave */
#define SLAVE_ADDRESS       0x78
#define NUM_OF_REC_BYTES    1

/* Variables */
const uint8_t TXData[] = {0x41,0x42};
static uint8_t RXData;

 /* I2C Master Configuration Parameter */
// Baud rate selectable (100KBPS, 400KBPS)
const eUSCI_I2C_MasterConfig i2cConfig =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        3000000,                                // SMCLK = 3MHz (default)
        EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
        2,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};

int main(void)
{
    /* Disabling the Watchdog  */
    MAP_WDT_A_holdTimer();

    /* Select Port 1 for I2C - Set Pin 6, 7 to input Primary Module Function,
     *   (UCB0SIMO/UCB0SDA, UCB0SOMI/UCB0SCL).
     */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN6 + GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Initializing I2C Master to SMCLK at 100khz with no autostop */
    MAP_I2C_initMaster(EUSCI_B0_BASE, &i2cConfig);
    /* Specify slave address */
    MAP_I2C_setSlaveAddress(EUSCI_B0_BASE, SLAVE_ADDRESS>>1);
    I2C_setMode(EUSCI_B0_BASE,EUSCI_B_I2C_TRANSMIT_MODE);
    /* Enable I2C Module to start operations */
    MAP_I2C_enableModule(EUSCI_B0_BASE);
    oled_init();
    OLED_Clear();
    while (1)
    {
        MAP_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, 0x41);  // Start + 1Byte
        MAP_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, 0x32); // Poll for TXINT,Send 1Byte
        MAP_I2C_masterSendMultiByteStop(EUSCI_B0_BASE);
    }
}

