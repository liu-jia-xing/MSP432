/***************************************************
 * 程序功能:使用TIMEA定时器进行定时操作
 * 功能:每过一秒，使P1.0口的灯反转一次（增计数模式）
 * 时钟：MODSC 25M，SMCLK不分频率，定时器24分频，约为1M
 * **************************************************/

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>

const Timer_A_UpModeConfig upmode =
{
     TIMER_A_CLOCKSOURCE_SMCLK,
     TIMER_A_CLOCKSOURCE_DIVIDER_24,
     10000,
     TIMER_A_TAIE_INTERRUPT_ENABLE,
     TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
     TIMER_A_DO_CLEAR
};
uint16_t counter=0;
/**
 * main.c
 */
void main(void)
{
    WDT_A_holdTimer();                            //关闭看门狗
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);   //设置p1.0口为输出
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);    //默认为低电平
    CS_initClockSignal(CS_SMCLK, CS_MODOSC_SELECT, CS_CLOCK_DIVIDER_1); //时钟选择
    Timer_A_configureUpMode(TIMER_A0_BASE,&upmode);             //TimeA模式选择
    Interrupt_enableInterrupt(INT_TA0_N);                       //中断函数允许位
    Interrupt_enableMaster();                                   //总中断
    Timer_A_startCounter(TIMER_A0_BASE,TIMER_A_UP_MODE );       //计数器工作
    while(1);
}

void TA0_N_IRQHandler(void)
{   counter++;
    if(counter==100){
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        counter=0;
    }
    Timer_A_clearInterruptFlag(TIMER_A0_BASE);
}
