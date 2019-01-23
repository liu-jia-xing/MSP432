/*
 * IIC.h
 *
 *  Created on: 2019年1月23日
 *      Author: L9379
 */

#ifndef IIC_H_
#define IIC_H_
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Defines */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#define CPU_F ((double)48000000)   //外部高频晶振8MHZ
#define delay_us(x) __delay_cycles((long)(48*(x)))
#define delay_ms(x) __delay_cycles((long)(48000*(x)))
#define SCL_OUT GPIO_setAsOutputPin(GPIO_PORT_P8,GPIO_PIN5)
#define SDA_OUT GPIO_setAsOutputPin(GPIO_PORT_P8,GPIO_PIN6)
#define SDA_IN  GPIO_setAsInputPin(GPIO_PORT_P8,GPIO_PIN6)
#define SDA_GET GPIO_getInputPinValue(GPIO_PORT_P8,GPIO_PIN6)
#define SCL_H   GPIO_setOutputHighOnPin(GPIO_PORT_P8,GPIO_PIN5)
#define SCL_L   GPIO_setOutputLowOnPin(GPIO_PORT_P8,GPIO_PIN5)
#define SDA_H   GPIO_setOutputHighOnPin(GPIO_PORT_P8,GPIO_PIN6)
#define SDA_L    GPIO_setOutputLowOnPin(GPIO_PORT_P8,GPIO_PIN6)

#define IIC_WADDR 0x78
 void IIC_init();
 void IIC_Start();
 void IIC_Stop();
 uint8_t IIC_send_byte(uint8_t txd);
 uint8_t IIC_read_byte();
 void i2c_write(uint8_t com,uint8_t date);
 void i2c_write(uint8_t com,uint8_t date);
#endif /* IIC_H_ */
