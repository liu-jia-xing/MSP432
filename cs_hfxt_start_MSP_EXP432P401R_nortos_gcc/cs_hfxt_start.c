
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#define CPU_F ((double)48000000)   //外部高频晶振8MHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define SCL_H   GPIO_setOutputHighOnPin(GPIO_PORT_P8,GPIO_PIN5)
#define SCL_L   GPIO_setOutputLowOnPin(GPIO_PORT_P8,GPIO_PIN5)

uint16_t counter=0;
const Timer_A_UpModeConfig upmode =
{
     TIMER_A_CLOCKSOURCE_SMCLK,
     TIMER_A_CLOCKSOURCE_DIVIDER_48,
     10000,
     TIMER_A_TAIE_INTERRUPT_ENABLE,
     TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
     TIMER_A_DO_CLEAR
};

int main(void)
{
    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();
    /*开启48M晶振*/
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

    /* Initializing MCLK to HFXT (effectively 48MHz) */
    CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
    Timer_A_configureUpMode(TIMER_A0_BASE,&upmode);             //TimeA模式选择
    Interrupt_enableInterrupt(INT_TA0_N);                       //中断函数允许位
    Interrupt_enableMaster();                                   //总中断
    Timer_A_startCounter(TIMER_A0_BASE,TIMER_A_UP_MODE );       //计数器工作
    GPIO_setAsOutputPin(GPIO_PORT_P8,GPIO_PIN5);
    while (1)
    {
        SCL_H;
        delay_ms(1000);
        SCL_L;
        delay_ms(1000);
    }
}

void TA0_N_IRQHandler(void)
{   counter++;
    if(counter==100){
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        counter=0;
    }
    Timer_A_clearInterruptFlag(TIMER_A0_BASE);
}
