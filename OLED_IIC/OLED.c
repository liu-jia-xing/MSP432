/*
 * OLED.c
 *
 *  Created on: 2018年11月26日
 *      Author: liu
 */
#include "OLED.h"

void oled_init()
{     //  I2C_init(0xaf);
        i2c_write(0x00,0xAE);//--turn off oled panel
        i2c_write(0x00,0x00);//---set low column address
        i2c_write(0x00,0x10);//---set high column address
        i2c_write(0x00,0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
        i2c_write(0x00,0x81);//--set contrast control register
        i2c_write(0x00,0xCF); // Set SEG Output Current Brightness
        i2c_write(0x00,0xA1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
        i2c_write(0x00,0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
        i2c_write(0x00,0xA6);//--set normal display
        i2c_write(0x00,0xA8);//--set multiplex ratio(1 to 64)
        i2c_write(0x00,0x3f);//--1/64 duty
        i2c_write(0x00,0xD3);//-set display offset   Shift Mapping RAM Counter (0x00~0x3F)
        i2c_write(0x00,0x00);//-not offset
        i2c_write(0x00,0xd5);//--set display clock divide ratio/oscillator frequency
        i2c_write(0x00,0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
        i2c_write(0x00,0xD9);//--set pre-charge period
        i2c_write(0x00,0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
        i2c_write(0x00,0xDA);//--set com pins hardware configuration
        i2c_write(0x00,0x12);
        i2c_write(0x00,0xDB);//--set vcomh
        i2c_write(0x00,0x40);//Set VCOM Deselect Level
        i2c_write(0x00,0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
        i2c_write(0x00,0x02);//
        i2c_write(0x00,0x8D);//--set Charge Pump enable/disable
        i2c_write(0x00,0x14);//--set(0x10) disable
        i2c_write(0x00,0xA4);// Disable Entire Display On (0xa4/0xa5)
        i2c_write(0x00,0xA6);// Disable Inverse Display On (0xa6/a7)
        i2c_write(0x00,0xAF);//--turn on oled panel
        i2c_write(0x00,0xAF); /*display ON*/
}
//清屏
void OLED_Clear(void)
{
    uint8_t i,n;
    for(i=0;i<8;i++)
    {
        i2c_write(0x00,0xb0+i);    //设置页地址（0~7）
        i2c_write(0x00,0x00);      //设置显示位置—列低地址
        i2c_write(0x00,0x10);      //设置显示位置—列高地址
        for(n=0;n<128;n++)i2c_write(0x40,0x00);
    } //更新显示
}
//设置显示位置
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
    i2c_write(0x00,0xb0+y);
    i2c_write(0x00,((x&0xf0)>>4)|0x10);
    i2c_write(0x00,(x&0x0f)|0x01);
}

void OLED_write_str(uint8_t x,uint8_t y,uint8_t *str)
{
   // uint8_t i,j;
    while(*str)
    {   if( *str < 0x80)
    {
        OLED_ShowChar(x,y,*str);
        x+=8;str++;
    }
    else
    {   OLED_write_char(x,y,str);
        x+=8;str++;}
    }
}
void OLED_write_char(uint8_t x,uint8_t y,uint8_t str[2])
{   uint8_t a,b;
    OLED_Set_Pos(x,y);
    for(a=0;a<72;a++)
    {
        if((Fonts16x16[a].Index[0]==str[0])&&(Fonts16x16[a].Index[1]==str[1]))
        {
            for(b=0;b<16;b++)
                i2c_write(0x40,Fonts16x16[a].Msk[b]);
               // i2c_write(0x40,Fonts16x16[a].Msk[b]);     //上八列
            OLED_Set_Pos(x,y+1);
            for(b=0;b<16;b++)                   //下八列
                i2c_write(0x40,Fonts16x16[a].Msk[b+16]);
            break;
        }
    }
}
/*函数功能:写单个汉字
 * 传参内容:横坐标:x,纵坐标:y,字:font
 * 16*16字体
 * */


//显示一个字符号串
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr)
{
    uint8_t j=0;
    while (chr[j]!='\0')
    {       OLED_ShowChar(x,y,chr[j]);
            x+=8;
        if(x>120){x=0;y+=2;}
            j++;
    }
}
//显示单个字符
void OLED_ShowChar(uint8_t x,uint8_t  y,uint8_t chr)
{
    uint8_t c=0,i=0;
        c=chr-' ';//
        if(x>Max_Column-1){x=0;y=y+2;}
        if(SIZE==16)
            {
            OLED_Set_Pos(x,y);
            for(i=0;i<8;i++)
            i2c_write(0x40,F8X16[c*16+i]);
            OLED_Set_Pos(x,y+1);
            for(i=0;i<8;i++)
            i2c_write(0x40,F8X16[c*16+i+8]);
            }
            else {
                OLED_Set_Pos(x,y+1);
                for(i=0;i<6;i++)
                i2c_write(0x40,F6x8[c][i]);
            }
}
//m^n
unsigned long oled_pow(uint8_t  m,uint8_t n)
{
    unsigned long result=1;
    while(n--)result*=m;
    return result;
}

void OLED_ShowNum(uint8_t x,uint8_t y,unsigned long num,uint8_t len,uint8_t size)
{
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size/2)*t,y,' ');
                continue;
            }else enshow=1;

        }
        OLED_ShowChar(x+(size/2)*t,y,temp+'0');
    }
}

