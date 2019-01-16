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
    WDT_A_holdTimer();
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    CS_initClockSignal(CS_SMCLK, CS_MODOSC_SELECT, CS_CLOCK_DIVIDER_1);
    Timer_A_configureUpMode(TIMER_A0_BASE,&upmode);
    Interrupt_enableInterrupt(INT_TA0_N);
    Interrupt_enableMaster();
    Timer_A_startCounter(TIMER_A0_BASE,TIMER_A_UP_MODE );
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
