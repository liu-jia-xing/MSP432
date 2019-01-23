
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
    //����P1.0Ϊ�����
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    //�����ⲿ����Ƶ�� LFTXΪ32k��HFTXΪ48M
    CS_setExternalClockSourceFrequency(32000,48000000);
    //���ص�Դ����״̬
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    //���ظ�������������ȴ�״̬����������
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
    {   OLED_write_str(3,3,"�Ž�ѧԺ");
    }
}
