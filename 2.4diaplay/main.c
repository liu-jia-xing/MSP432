
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
    //设置J.3,J.2口设置为晶振输入功能
        GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
                GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
        //设置P1.0为输出口
        GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
        //设置外部晶振频率 LFTX为32k，HFTX为48M
        CS_setExternalClockSourceFrequency(32000,48000000);
        //返回电源工作状态
        PCM_setCoreVoltageLevel(PCM_VCORE1);
        //返回给定闪存库的闪存等待状态集的数量。
        FlashCtl_setWaitState(FLASH_BANK0, 1);
        FlashCtl_setWaitState(FLASH_BANK1, 1);
        CS_startHFXT(false);
        CS_initClockSignal(CS_MCLK,CS_HFXTCLK_SELECT , CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK,CS_HFXTCLK_SELECT , CS_CLOCK_DIVIDER_1);
    MAP_CS_setDCOFrequency(8330000);
    TFT_port_init();
    TFT_init();
    LCD_Clear(LIGHTGREEN);
    LCD_PutString24(0,0,"九江学院",LIGHTBLUE,LIGHTGREEN);
    Show_Image(0,240,0,320,gImage_2);
    while(1)
    {

    }
}
void EUSCIB0_IRQHandler(void){
    uint32_t status = SPI_getEnabledInterruptStatus(EUSCI_B0_BASE);

    SPI_clearInterruptFlag(EUSCI_B0_BASE, status);
}
