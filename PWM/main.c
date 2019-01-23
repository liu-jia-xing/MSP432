/***************************************************
 * 程序功能:使用TIMERA定时器进行产生PWM操作
 * 功能:按一下P1.1的按键，PWM波的占空比增加一次，周期为20ms。
 * 时钟：DCO 3M，SMCLK不分频率，定时器3分频，约为1M
 * **************************************************/

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>

Timer_A_PWMConfig pwmconfig =
{
     TIMER_A_CLOCKSOURCE_SMCLK,
     TIMER_A_CLOCKSOURCE_DIVIDER_3,
     20000,
     TIMER_A_CAPTURECOMPARE_REGISTER_1,
     TIMER_A_OUTPUTMODE_RESET_SET,
     1000
};

int main()
{
    WDT_A_holdTimer();
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    Interrupt_enableInterrupt(INT_PORT1);
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,
                GPIO_PRIMARY_MODULE_FUNCTION);
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmconfig);
    Interrupt_enableInterrupt(INT_PORT1);
    Interrupt_enableSleepOnIsrExit();
    Interrupt_enableMaster();
    while (1);
}
void PORT1_IRQHandler(void)
{
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    if (status & GPIO_PIN1)
    {
        if(pwmconfig.dutyCycle == 2000)
            pwmconfig.dutyCycle = 1000;
        else
            pwmconfig.dutyCycle += 100;

        MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmconfig);
    }
}
