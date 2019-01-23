/*
 * IIC.c
 *
 *  Created on: 2019年1月23日
 *      Author: L9379
 */
#include "IIC.h"


 void IIC_init()
 {
     SCL_OUT;
     SDA_OUT;
     SCL_H;
     SDA_H;
 }

 void IIC_Start()
 {   SDA_OUT;
     SDA_H;
     delay_us(5);
     SCL_H;
     delay_us(5);
     SDA_L;
     delay_us(5);
     SCL_L;
     delay_us(5);
 }
 void IIC_Stop()
 {
     SDA_OUT;
     SDA_L;
     delay_us(5);
     SCL_H;
     delay_us(5);
     SDA_H;
     delay_us(5);
 }


 uint8_t IIC_send_byte(uint8_t txd)
 {   SDA_OUT;
     uint8_t i,b;
     for(i=0;i<8;i++)
     {
         if(txd&0x80)
             SDA_H;
         else
             SDA_L;
         txd<<=1;
         delay_us(5);
         SCL_H;
         delay_us(5);
         SCL_L;
         delay_us(5);
     }
     SDA_H;
     delay_us(5);
     SCL_H;
     while(SDA_GET)
     {
         b++;
         if(b>200)
         {
             SCL_L;
             delay_us(5);
             return 0;
         }
     }
     SCL_L;
     delay_us(5);
     return 1;
 }
 uint8_t IIC_read_byte()
 {
     uint8_t i=0;
     uint8_t dat =0;
     SDA_H;
     delay_us(5);
     SDA_IN;
     for(i=0;i<8;i++)
     {
         SCL_H;
         delay_us(5);
         dat<<=1;
         dat |=SDA_GET;
         delay_us(5);
         SCL_L;
         delay_us(5);
     }
     return dat;
 }
void i2c_write(uint8_t com,uint8_t date)
{   IIC_Start();    //启动
    IIC_send_byte(IIC_WADDR);  //发送从设备地址
    IIC_send_byte(com);   //发出芯片内地址
    IIC_send_byte(date);    //发送数据
    IIC_Stop(); //停止
}

uint8_t R_data(uint8_t addr)
{
    uint8_t dat;
    IIC_Start();
    IIC_send_byte(IIC_WADDR);//发送发送从设备地址 写操作
    IIC_send_byte(addr);//发送芯片内地址
    IIC_Start();
    IIC_send_byte(IIC_WADDR+1);//发送发送从设备地址 读操作
    dat = IIC_read_byte();//获取数据
    IIC_Stop();
    return dat;
}


