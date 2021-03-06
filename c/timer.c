#include "General.h"

UUI16 G_Time_Bit = {0}; // 时间片变量

unsigned int xdata PWMRD_40 _at_ 0x1040;
unsigned int xdata PWMRD_41 _at_ 0x1042;
unsigned int xdata PWMRD_42 _at_ 0x1044;
unsigned int xdata PWMRD_43 _at_ 0x1046;
unsigned int xdata PWMRD_50 _at_ 0x1048;
unsigned int xdata PWMRD_51 _at_ 0x104A;
unsigned int xdata PWMRD_52 _at_ 0x104C;
unsigned int xdata PWMRD_53 _at_ 0x104E;

/*****************************************************
 *函数名称：void BTM_Init(void)
 *函数功能：BTM初始化(外接32k晶振)
 *入口参数：void
 *出口参数：void
 *****************************************************/
void BTM_Init(void)
{
    //	BTMCON = 0x00;  //不开启
    //	BTMCON = 0x80;  //每15.6ms产生一个中断
    //	BTMCON = 0x81;	//每31.3ms产生一个中断
    //	BTMCON = 0x82;	//每62.5ms产生一个中断
    //	BTMCON = 0x83;	//每125ms产生一个中断
    //	BTMCON = 0x84;	//每0.25s产生一个中断
    //	BTMCON = 0x85;	//每0.5s产生一个中断
    BTMCON = 0x86; //每1s产生一个中断
                   //	BTMCON = 0x87;	//每2s产生一个中断
                   //      BTMCON = 0x88;	//每4s产生一个中断
                   //      BTMCON = 0x89;	//每8s产生一个中断
                   //      BTMCON = 0x8A;	//每16s产生一个中断
                   //      BTMCON = 0x8B;	//每32s产生一个中断
    IE1 |= 0x04;   //开启BTM中断
}

/*********************************************************
函数名: void TimerInit(void)
描  述: T16N3定时器初始化
输入值: 无
输出值: 无
返回值: 无
**********************************************************/
void TimerInit(void)
{
    //主时钟 32M
/*     
    TMCON |= bit0; //------111 ;Timer0 选择时钟Fsys
    // T0设置
    TMOD |= 0x01;                     // 0000 0001;Timer0设置工作方式1
    TL0 = (65536 - (2000 - 1)) % 256; //溢出时间：时钟为Fsys，则2000*（1/Fsys）=125us;
    TH0 = (65536 - (2000 - 1)) / 256;
    TR0 = 0;
    ET0 = 1; //定时器0允许
    TR0 = 1; //打开定时器0
 */
    // BTM_Init();

    // T3设置
    TXINX = 0x03; //选择定时器T3
    TXMOD = 0x80;
    TXCON = 0x00;                  //设置为16位重载寄存
    RCAPXH = (65536 - 1999) / 256; //溢出时间：时钟为Fsys，则32000*（1/Fsys）=2ms;
    RCAPXL = (65536 - 1999) % 256;
    TRX = 0;
    IE1 |= 0x40; //定时器3中断允许
    IP1 |= 0X40; //timer3中断优先级为高
    TRX = 1; //打开定时器3
}

#define PWM_CYCLE_VALUE 4000

void PWM40_Duty(UI08 _buf)
{
    static UI08 duty_old0;
    UI16 _duty_buf;
    // if(duty_old0!=_buf)
    {
        duty_old0 = _buf;

        if (_buf >= 100)
        {
            _duty_buf = 99;
        }
        else
        {
            _duty_buf = _buf;
        }

        _duty_buf = (PWM_CYCLE_VALUE / 100 * _duty_buf) & 0x0fff;

        PWMRD_40 = 0x8000 | _duty_buf; //开启PWM口，设置占空比
    }
}

void PWM41_Duty(UI08 _buf)
{
    static UI08 duty_old1;
    UI16 _duty_buf;
    if (duty_old1 != _buf)
    {
        duty_old1 = _buf;

        if (_buf >= 100)
        {
            _duty_buf = 99;
        }
        else
        {
            _duty_buf = _buf;
        }

        _duty_buf = (PWM_CYCLE_VALUE / 100 * _duty_buf) & 0x0fff;

        PWMRD_41 = 0x8000 | _duty_buf; //开启PWM口，设置占空比
    }
}

void PWM42_Duty(UI08 _buf)
{
    static UI08 duty_old2;
    UI16 _duty_buf;
    // if(duty_old2!=_buf)
    {
        duty_old2 = _buf;

        if (_buf >= 100)
        {
            _duty_buf = 99;
        }
        else
        {
            _duty_buf = _buf;
        }

        _duty_buf = (PWM_CYCLE_VALUE / 100 * _duty_buf) & 0x0fff;

        PWMRD_42 = 0x8000 | _duty_buf; //开启PWM口，设置占空比
    }
}
/*
void PWM43_Duty(UI08 _buf)
{
    static  UI08  duty_old3;
    UI16  _duty_buf;
    //if(duty_old3!=_buf)
    {
      duty_old3=_buf;

      if(_buf>=100)
      {_duty_buf=99;}
      else
      {_duty_buf=_buf;}

      _duty_buf=(PWM_CYCLE_VALUE/100*_duty_buf)&0x0fff;

      PWMRD_43 = 0x8000 | _duty_buf;     //开启PWM口，设置占空比
    }
} */
/*************************************************
 // 函数名称    : PWM_Init
 // 功能描述    :
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void PWM_Init(void)
{
    UI16 value_buf = PWM_CYCLE_VALUE;
    //设置了周期为50us，占空比为50%的PWM波形
    PWMMOD = 0x00;                      //独立模式，边沿对齐
    PWMCON = (UI08)PWM_CYCLE_VALUE;     //周期设置低8位,1mS
    PWMCFG = (0xB0 | (value_buf >> 8)); // 7:开关  5-4：时钟源选择 8分屏  3-0：周期设置高4位

    ////////////////上电默认橙色->绿色->蓝色
    BLUE_LED_Duty(0);
    GREEN_LED_Duty(15);
    RED_LED_Duty(100);
}
/*************************************************
 // 函数名称    : prg_ms1
 // 功能描述    :
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void prg_ms1(void) //此函数放在中断程序中执行
{
    static UI08 ms5_cont = 0;
    static UI08 ms10_cont = 0;  // 10ms计数
    static UI08 ms100_cont = 0; // 100ms计数
    static UI08 ms250_cont = 0; // 200ms计数
    static UI08 ms500_cont = 0; // 500ms计数

    if (Buzz_Time > 0)
    {
        Buzz_Time--;
        if (Buzz_Time == 0)
        {
            BUZZ_OFF;
        }
    }

    if (++ms5_cont >= 5)
    {
        ms5_cont = 0;
        _5mS_For_SYS = 1;
    }
    ms10_cont++;
    if (ms10_cont >= 10)
    {
        _10mS_For_SYS = 1;
        ms10_cont = 0;
        ms100_cont++;
        ms250_cont++;
    }
    if (ms250_cont >= 25)
    {
        ms250_cont = 0;
        _250mS_For_SYS = 1;
    }

    if (ms100_cont >= 10)
    {
       _100mS_For_SYS = 1;
        ms100_cont = 0;

        ms500_cont++;
        if (ms500_cont >= 5)
        {
            ms500_cont = 0;
            _500mS_For_SYS = 1;
            _txd_tick = 1;
        }
    }
}
/*************************************************
 // 函数名称    : Timer_Deal
 // 功能描述    :
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Timer_Deal(void) //此函数放在主程序中执行
{
    static UI08 Minute_Count = 0; //分钟count

    if (!_1S_For_For_SYS)
        return;

    Minute_Count++;

    if (Minute_Count >= 60)
    {
        Minute_Count = 0;
        _1Minute_For_SYS = 1;
    }
}
