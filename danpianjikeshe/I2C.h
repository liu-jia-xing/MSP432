/*
 * I2C.h
 *
 *  Created on: 2018��11��26��
 *      Author: liu
 */

#ifndef I2C_H_
#define I2C_H_
#include<msp430f169.h>
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define uchar unsigned char
#define uint unsigned int
void i2c_write(unsigned char naddr,unsigned char data);
unsigned char  i2c_read(unsigned char naddr);
void I2C_init(unsigned char);
#endif /* I2C_H_ */
