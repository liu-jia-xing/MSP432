#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
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
void Clock_init()
{

    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
    //设置P1.0为输出口
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    //设置外部晶振频率 LFTX为32k，HFTX为48M
    CS_setExternalClockSourceFrequency(32000,48000000);
    //返回电源工作状态
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    //返回给定闪存库的闪存等待状态集的数量。
    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);
    CS_startHFXT(false);

    /* Initializing MCLK to HFXT (effectively 48MHz) */
    CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
}

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

void W_data(uint8_t addr,uint8_t dat)
{
    IIC_Start();    //启动
    IIC_send_byte(IIC_WADDR);  //发送从设备地址
    IIC_send_byte(addr);   //发出芯片内地址
    IIC_send_byte(dat);    //发送数据
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

void Write_IIC_Command(uint8_t dat)
{
    IIC_Start();    //启动
    IIC_send_byte(IIC_WADDR);  //发送从设备地址
    IIC_send_byte(0x00);   //发出芯片内地址
    IIC_send_byte(dat);    //发送数据
    IIC_Stop(); //停止
}

void Write_IIC_Data(uint8_t dat)
{
    IIC_Start();    //启动
    IIC_send_byte(IIC_WADDR);  //发送从设备地址
    IIC_send_byte(0x40);   //发出芯片内地址
    IIC_send_byte(dat);    //发送数据
    IIC_Stop(); //停止
}

void Initial_LY096BG30()
{
    Write_IIC_Command(0xAE);   //display off
    Write_IIC_Command(0x20);    //Set Memory Addressing Mode
    Write_IIC_Command(0x10);    //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    Write_IIC_Command(0xb0);    //Set Page Start Address for Page Addressing Mode,0-7
    Write_IIC_Command(0xc8);    //Set COM Output Scan Direction
    Write_IIC_Command(0x00);//---set low column address
    Write_IIC_Command(0x10);//---set high column address
    Write_IIC_Command(0x40);//--set start line address
    Write_IIC_Command(0x81);//--set contrast control register
    Write_IIC_Command(0x7f);
    Write_IIC_Command(0xa1);//--set segment re-map 0 to 127
    Write_IIC_Command(0xa6);//--set normal display
    Write_IIC_Command(0xa8);//--set multiplex ratio(1 to 64)
    Write_IIC_Command(0x3F);//
    Write_IIC_Command(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    Write_IIC_Command(0xd3);//-set display offset
    Write_IIC_Command(0x00);//-not offset
    Write_IIC_Command(0xd5);//--set display clock divide ratio/oscillator frequency
    Write_IIC_Command(0xf0);//--set divide ratio
    Write_IIC_Command(0xd9);//--set pre-charge period
    Write_IIC_Command(0x22); //
    Write_IIC_Command(0xda);//--set com pins hardware configuration
    Write_IIC_Command(0x12);
    Write_IIC_Command(0xdb);//--set vcomh
    Write_IIC_Command(0x20);//0x20,0.77xVcc
    Write_IIC_Command(0x8d);//--set DC-DC enable
    Write_IIC_Command(0x14);//
    Write_IIC_Command(0xaf);//--turn on oled panel
}
void OLED_Clear(void)
{
    uint8_t i,n;
    for(i=0;i<8;i++)
    {
        Write_IIC_Command(0xb0+i);    //设置页地址（0~7）
        Write_IIC_Command(0x00);      //设置显示位置—列低地址
        Write_IIC_Command(0x10);      //设置显示位置—列高地址
        for(n=0;n<128;n++)Write_IIC_Data(0x00);
    } //更新显示
}
int main(void)
{
    /* Halting the Watchdog */
        MAP_WDT_A_holdTimer();
       Clock_init();
       IIC_init();
        Initial_LY096BG30();
        OLED_Clear();
        while(1)
        {

        }
}

