/*
 * SPIT.h
 *
 *  Created on: 2018Äê12ÔÂ18ÈÕ
 *      Author: liu
 */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#ifndef SPIT_H_
#define SPIT_H_

static const eUSCI_SPI_MasterConfig spiMasterConfig =
{
        EUSCI_B_SPI_CLOCKSOURCE_SMCLK,             // SMCLK Clock Source
        3000000,                                   // SMCLK = DCO = 3MHZ
        500000,                                    // SPICLK = 500khz
        EUSCI_B_SPI_MSB_FIRST,                     // MSB First
        EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, // High polarity
        EUSCI_B_SPI_3PIN                           // 3Wire SPI Mode
};
void GPIO_PORT_INIT();
void SPI_send_data(uint8_t);

#endif /* SPIT_H_ */
