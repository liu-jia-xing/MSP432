/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

int main(void)
{
    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();

    /* Configuring GPIO as an output */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    
    //![Simple FPU Config]
    /* Enabling FPU for DCO Frequency calculation */
    MAP_FPU_enableModule();
    
    /* Setting the DCO Frequency to a non-standard 8.33MHz */
    MAP_CS_setDCOFrequency(8330000);
    //![Simple FPU Config]
    
    /* Configuring SysTick to trigger at 4165000 (MCLK is 8.33MHz so this will
     * make it toggle every ~0.5s) */
    MAP_SysTick_enableModule();
    MAP_SysTick_setPeriod(4165000);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_SysTick_enableInterrupt();
    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();
    while (1)
    {
       PCM_gotoLPM0();
    }
}

void SysTick_Handler(void)
{
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

