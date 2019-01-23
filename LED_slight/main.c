#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

int main(void)
{
     volatile uint32_t i;

    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    // Set P1.0 to output direction

    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
    while(1)
    {
        for(i=100000; i>0; i--);
        GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
  //       Toggle P1.0 output

        // Delay
        for(i=100000; i>0; i--);
        GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
    }
}
