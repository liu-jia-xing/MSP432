/*************************
 * 函数功能：配置时钟
 *
 * ***********************/
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
/* Statics */
static volatile uint32_t aclk, mclk, smclk, hsmclk, bclk;

int main(void)
{
    /* Halting WDT */
    MAP_WDT_A_holdTimer();

    /* Initializing the clock source as follows:
     *      MCLK = MODOSC/4 = 6MHz
     *      ACLK = REFO/2 = 16kHz
     *      HSMCLK = DCO/2 = 1.5Mhz
     *      SMCLK = DCO/4 = 750kHz
     *      BCLK  = REFO = 32kHz
     */
    MAP_CS_initClockSignal(CS_MCLK, CS_MODOSC_SELECT, CS_CLOCK_DIVIDER_4);
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_2);
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);
    MAP_CS_initClockSignal(CS_BCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /*
     *  Getting all of the frequency values of the CLK sources using the
     * convenience functions */
    aclk = CS_getACLK();
    mclk = CS_getMCLK();
    smclk = CS_getSMCLK();
    hsmclk = CS_getHSMCLK();
    bclk = CS_getBCLK();

    /*
     * Staying in active mode so that the user can see the values that are put
     * in the above registers through the debugger
     */
    while (1)
    {

    }
}

