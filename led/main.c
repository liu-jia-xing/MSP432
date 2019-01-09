#include "msp432p401r.h"
void delay(unsigned int i){
while(i--);

}
/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	P1DIR=0xff;
	while(1){
	P1OUT|=BIT0;
	delay(60000);
	P1OUT =0;
	delay(60000);}
}
