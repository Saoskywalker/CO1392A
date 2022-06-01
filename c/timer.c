#include "General.h"

MCU_xdata UI08 Minute_Count = 0; //����count

MCU_xdata UI08 ms10_cont = 0;  // 10ms����
MCU_xdata UI08 ms20_cont = 0;  // 20ms����
MCU_xdata UI08 ms100_cont = 0; // 100ms����
MCU_xdata UI08 ms200_cont = 0; // 200ms����
MCU_xdata UI08 ms500_cont = 0; // 500ms����

MCU_xdata UUI08 ms_bit = {0};     // 1msʱ��Ƭ
MCU_xdata UUI08 ms5_bit = {0};    // 5msʱ��Ƭ
MCU_xdata UUI08 ms10_bit = {0};   // 10msʱ��Ƭ
MCU_xdata UUI08 ms100_bit = {0};  // 100msʱ��Ƭ
MCU_xdata UUI08 ms500_bit = {0};  // 500msʱ��Ƭ
MCU_xdata UUI16 s_bit = {0};      // 1000msʱ��Ƭ
MCU_xdata UUI08 minute_bit = {0}; // 1minʱ��Ƭ

unsigned int xdata PWMRD_40 _at_ 0x1040;
unsigned int xdata PWMRD_41 _at_ 0x1042;
unsigned int xdata PWMRD_42 _at_ 0x1044;
unsigned int xdata PWMRD_43 _at_ 0x1046;
unsigned int xdata PWMRD_50 _at_ 0x1048;
unsigned int xdata PWMRD_51 _at_ 0x104A;
unsigned int xdata PWMRD_52 _at_ 0x104C;
unsigned int xdata PWMRD_53 _at_ 0x104E;

/*****************************************************
 *�������ƣ�void BTM_Init(void)
 *�������ܣ�BTM��ʼ��(���32k����)
 *��ڲ�����void
 *���ڲ�����void
 *****************************************************/
void BTM_Init(void)
{
    //	BTMCON = 0x00;  //������
    //	BTMCON = 0x80;  //ÿ15.6ms����һ���ж�
    //	BTMCON = 0x81;	//ÿ31.3ms����һ���ж�
    //	BTMCON = 0x82;	//ÿ62.5ms����һ���ж�
    //	BTMCON = 0x83;	//ÿ125ms����һ���ж�
    //	BTMCON = 0x84;	//ÿ0.25s����һ���ж�
    //	BTMCON = 0x85;	//ÿ0.5s����һ���ж�
    BTMCON = 0x86; //ÿ1s����һ���ж�
                   //	BTMCON = 0x87;	//ÿ2s����һ���ж�
                   //      BTMCON = 0x88;	//ÿ4s����һ���ж�
                   //      BTMCON = 0x89;	//ÿ8s����һ���ж�
                   //      BTMCON = 0x8A;	//ÿ16s����һ���ж�
                   //      BTMCON = 0x8B;	//ÿ32s����һ���ж�
    IE1 |= 0x04;   //����BTM�ж�
}

/*********************************************************
������: void TimerInit(void)
��  ��: T16N3��ʱ����ʼ��
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
void TimerInit(void)
{
    //��ʱ�� 32M
    TMCON |= bit0; //------111 ;Timer0 ѡ��ʱ��Fsys
    // T0����
    TMOD |= 0x01;                     // 0000 0001;Timer0���ù�����ʽ1
    TL0 = (65536 - (2000 - 1)) % 256; //���ʱ�䣺ʱ��ΪFsys����2000*��1/Fsys��=125us;
    TH0 = (65536 - (2000 - 1)) / 256;
    TR0 = 0;
    ET0 = 1; //��ʱ��0����
    TR0 = 1; //�򿪶�ʱ��0

    // BTM_Init();
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

        PWMRD_40 = 0x8000 | _duty_buf; //����PWM�ڣ�����ռ�ձ�
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

        PWMRD_41 = 0x8000 | _duty_buf; //����PWM�ڣ�����ռ�ձ�
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

        PWMRD_42 = 0x8000 | _duty_buf; //����PWM�ڣ�����ռ�ձ�
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

      PWMRD_43 = 0x8000 | _duty_buf;     //����PWM�ڣ�����ռ�ձ�
    }
} */
/*************************************************
 // ��������    : PWM_Init
 // ��������    :
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void PWM_Init(void)
{
    UI16 value_buf = PWM_CYCLE_VALUE;
    //����������Ϊ50us��ռ�ձ�Ϊ50%��PWM����
    PWMMOD = 0x00;                      //����ģʽ�����ض���
    PWMCON = (UI08)PWM_CYCLE_VALUE;     //�������õ�8λ,1mS
    PWMCFG = (0xB0 | (value_buf >> 8)); // 7:����  5-4��ʱ��Դѡ�� 8����  3-0���������ø�4λ

    ////////////////�ϵ�Ĭ�ϳ�ɫ->��ɫ->��ɫ
    BLUE_LED_Duty(0);
    GREEN_LED_Duty(15);
    RED_LED_Duty(100);
}
/*************************************************
 // ��������    : prg_ms1
 // ��������    :
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_ms1(void) //�˺��������жϳ�����ִ��
{
    // prg_ms_uart();
    //_SYS_Inspect_ms=1;
    SYS_Inspect_ms_general();
    //
    if (Buzz_Time > 0)
    {
        Buzz_Time--;
        if (Buzz_Time == 0)
        {
            BUZZ_OFF;
        }
    }

    if (M_Key_last < 0xffff)
    {
        M_Key_last++;
    }

    ms_bit.byte = 0xff;
    ms10_cont++;
    if (ms10_cont >= 10)
    {
        ms10_bit.byte = 0xff;
        ms10_cont = 0;
        ms100_cont++;
        ms200_cont++;
    }
    if (ms200_cont >= 20)
    {
        ms200_cont = 0;
    }

    if (ms100_cont >= 10)
    {
        ms100_bit.byte = 0xff;
        ms100_cont = 0;

        ms500_cont++;
        if (ms500_cont >= 5)
        {
            ms500_cont = 0;
            ms500_bit.byte = 0xff;
            _txd_tick = 1;
        }
    }
}
/*************************************************
 // ��������    : Timer_Deal
 // ��������    :
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Timer_Deal(void) //�˺���������������ִ��
{
    if (!_Timer_second)
        return;
    _Timer_second = 0;
    Minute_Count++;

    if (Minute_Count >= 60)
    {
        Minute_Count = 0;
        minute_bit.byte = 0xff;
    }
}
