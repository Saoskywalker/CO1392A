#include "General.h"

MCU_xdata UI08 Power_Delay_Time = 3; //系统开机延时
MCU_xdata FuncState _Self_Test = DISABLE;

/*************************************************
 // 函数名称    : io_init
 // 功能描述    : IO口初始化
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
/************************************************** ****************************************
 硬件连接：
SW1-P00    SW3-P02    SW2-P04    WIFI_TXD-P05  WIFI_RXD-P06
LED_2-P10  SEG_H-P14  SEG_G-P15  COM1-P16      SEG_F-P17
SEG_E-P20  LED_1-P21  LED_6-P22  LED_7-P23     LED_4-P24    LED_8-P25  SW5-P26    SW4-P27
LED_3-P30  LED_5-P31  SEG_D-P32  SEG_C-P33     SEG_B-P34    SEG_A-P35  COM2-P36   BUZ-P37
LED_R-P40  LED_G-P41  LED_B-P42  DISP_TXD-P44  DISP_RXD-P45
*******************************************************************************************/
void IO_Init(void)
{
    // TK上的IO设置为强推挽输出1

    P0CON = _1011_1111; // P0CON 输入输出寄存器  1输出 0输入
    P0PH = _0000_0000;  //上拉电阻控制寄存器 0不使能上拉电阻 1是能上拉电阻
    P0VO = _0000_0000;  //显示驱动控制寄存器  0关闭显示驱动 1打开显示驱动
    P0 = _1111_0111;    //数据寄存器

    P1CON = _1111_1111; //输入输出寄存器  1输出 0输入
    P1PH = _0000_0000;  //上拉电阻控制寄存器 0不使能上拉电阻 1是能上拉电阻
    P1VO = _0000_0000;  //显示驱动控制寄存器  0关闭显示驱动 1打开显示驱动
    P1 = _0100_1111;    //数据寄存器

    P2CON = _1111_1111; //输入输出寄存器  1输出 0输入
    P2PH = _0000_0000;  //上拉电阻控制寄存器 0不使能上拉电阻 1是能上拉电阻
    P2VO = _0000_0000;  //显示驱动控制寄存器  0关闭显示驱动 1打开显示驱动
    P2 = _1111_1110;    //数据寄存器

    P3CON = _1111_1111; //输入输出寄存器  1输出 0输入
    P3PH = _0000_0000;  //上拉电阻控制寄存器 0不使能上拉电阻 1是能上拉电阻
    P3VO = _0000_0000;  //显示驱动控制寄存器  0关闭显示驱动 1打开显示驱动
    P3 = _0100_0011;    //数据寄存器

    P4CON = _0001_1111; //输入输出寄存器  1输出 0输入
    P4PH = _0000_0000;  //上拉电阻控制寄存器 0不使能上拉电阻 1是能上拉电阻
    P4 = _1111_1000;    //数据寄存器

    P5CON = _0000_1110; //输入输出寄存器  1输出 0输入
    P5PH = _0000_0000;  //上拉电阻控制寄存器 0不使能上拉电阻 1是能上拉电阻
    P5 = _0000_1100;    //数据寄存器

    IOHCON0 = _0000_0000; // P0 P1 高电平电流输出能力
    IOHCON1 = _0000_0000; // P2 P3 高电平电流输出能力
}

void IAP_Reset()
{
    EA = 0;
    IAPKEY = 0X0; //不使能IAP
    // IAPCTL &= ~0x04;//BTLD控制位清0，下次复位后进入APROM
    PCON |= 0X08; //软件复位
    _nop_();      //等待8个_nop_()
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

/*****************************************************
 *函数名称：void BTM_Int(void) interrupt 9
 *函数功能：中断函数 1S定时
 *入口参数：void
 *出口参数：void
 *****************************************************/
void BTM_Int(void) interrupt 9
{
    if (!(BTMCON & 0X40)) //中断标志位判断
    {
        s_bit.word = 0xffff;
    }
}

/**************************************************
 *函数名称：void timer0()interrupt 1
 *函数功能：定时器中断服务函数  125us
 *入口参数：void
 *出口参数：void
 **************************************************/
UI08 sys_time_rj = 0;
void timer0(void) interrupt 1
{
    TL0 = (65536 - (2000 - 1)) % 256; //溢出时间：时钟为Fsys，则1000*（1/Fsys）=125us;  主频8M
    TH0 = (65536 - (2000 - 1)) / 256;
    //
    if (Buzz_Time > 0)
    {
        if (Buzzer_IO_Status == RESET)
        {
            BUZZ_ON;
            Buzzer_IO_Status = SET;
        }
        else
        {
            BUZZ_OFF;
            Buzzer_IO_Status = RESET;
        }
    }

    Communication_INT_Deal();

    // step_time++;//摆叶
    // LED_PWM++;
    // if( LED_PWM>=2)//最大16 led_scan_interval*8
    //{
    //    LED_G_OFF;
    // }
    // exv_run();

    // Get_the_pump_pwm_width();
    //================   1ms时间片   =====================================
    if (++sys_time_rj >= 8)
    {
        sys_time_rj = 0;
        prg_ms1();
        Led_Scan();
    }
}

/*****************************************************
 *oˉêy??3?￡ovoid UartInt(void) interrupt 4
 *oˉêy1|?ü￡oUart?D??oˉêy
 *è??ú2?êy￡ovoid
 *3??ú2?êy￡ovoid
 *****************************************************/
void UartInt(void) interrupt 4
{
    if (READ_TI)
    {
        CLEAR_TI;
        Wifi_UART_Txd_Data();
    }
    if (READ_RI)
    {
        if (SYS_W_Uart.Rxd_byte >= RXD_MAX)
        {
            SYS_W_Uart.Rxd_byte = 0;
        }
        SYS_W_Uart.Rxd_buf[SYS_W_Uart.Rxd_byte++] = UART_SFR; //接收数据
        CLEAR_RI;
        SYS_W_Uart.Rxd_Timerout = 20;
    }
}
