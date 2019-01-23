
/* DriverLib Defines */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Defines */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "OLED.h"
void Clock_init()
{

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

    /* Initializing MCLK to HFXT (effectively 48MHz) */
    CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
}
int main(void)
{
    MAP_WDT_A_holdTimer();
    Clock_init();
    IIC_init();
    oled_init();
    OLED_Clear();
    while (1)
    {   OLED_write_str(3,3,"九江学院");
    }
}
