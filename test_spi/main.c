#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "SPIT.h"
#include "TFT.h"
#include <stdint.h>
#include <stdbool.h>

/*
P1.6 Data Out (SIMO)
P1.5 Serial Clock Out (CLK)
P5.0 CS
P5.1 DC
P5.2 RESET
 */

int main(void){
     WDT_A_holdTimer();
    GPIO_PORT_INIT();

    MAP_CS_initClockSignal(CS_SMCLK,CS_MODOSC_SELECT , CS_CLOCK_DIVIDER_1);
    MAP_CS_setDCOFrequency(8330000);
    TFT_port_init();
    TFT_init();
    LCD_Clear(LIGHTGREEN);
    LCD_PutString24(0,0,"¾Å½­Ñ§Ôº",LIGHTBLUE,LIGHTGREEN);
    Show_Image(0,240,0,320,gImage_2);
    while(1)
    {

    }
}

void EUSCIB0_IRQHandler(void){
    uint32_t status = SPI_getEnabledInterruptStatus(EUSCI_B0_BASE);

    SPI_clearInterruptFlag(EUSCI_B0_BASE, status);
}
