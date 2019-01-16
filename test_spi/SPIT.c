/*
 * SPIT.c
 *
 *  Created on: 2018Äê12ÔÂ18ÈÕ
 *      Author: liu
 */
#include "SPIT.h"

void GPIO_PORT_INIT()
{
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
        /* Configuring SPI in 3wire master mode */
        SPI_initMaster(EUSCI_B0_BASE, &spiMasterConfig);
        /* Enable SPI module */
        SPI_enableModule(EUSCI_B0_BASE);
        /* Enabling interrupts */
        SPI_enableInterrupt(EUSCI_B0_BASE, EUSCI_B_SPI_RECEIVE_INTERRUPT);
        Interrupt_enableInterrupt(INT_EUSCIB0);
//        Interrupt_enableSleepOnIsrExit();
}

void SPI_send_data(uint8_t data)
{
    while (!(SPI_getInterruptStatus(EUSCI_B0_BASE,EUSCI_B_SPI_TRANSMIT_INTERRUPT)));

      /* Transmitting data to slave */
      SPI_transmitData(EUSCI_B0_BASE,data);
}


