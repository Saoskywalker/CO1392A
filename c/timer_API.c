#include "General.h"


xdata UUI08   SYS_ms1_bit;//1msʱ��Ƭ
xdata UUI08   SYS_ms10_bit;//10msʱ��Ƭ
xdata UUI08   SYS_ms20_bit;//20msʱ��Ƭ
xdata UUI08   SYS_ms100_bit;//100msʱ��Ƭ
xdata UUI08   SYS_ms500_bit;//500msʱ��Ƭ
xdata UUI08   SYS_S_bit;//1000msʱ��Ƭ

xdata UI08   ms10_cont;//10ms����
xdata UI08   ms20_cont;//20ms����
xdata UI08   ms100_cont;//100ms����
xdata UI08   ms500_cont;//500ms����
xdata UI08   sys_time_rj;//1ms����
/**************************************************
*�������ƣ�void timer_data_init(void)
*�������ܣ�������ʼ��
*��ڲ�����void
*���ڲ�����void
**************************************************/
void timer_data_init(void)
{
  ms10_cont=0;
  ms20_cont=0;
  ms100_cont=0;
  ms500_cont=0;
  sys_time_rj=0;

  SYS_ms1_bit.byte=0;
  SYS_ms10_bit.byte=0;
  SYS_ms20_bit.byte=0;
  SYS_ms100_bit.byte=0;
  SYS_ms500_bit.byte=0;
  SYS_S_bit.byte=0;
}
 /**************************************************
*�������ƣ�void TimerInit(void)
*�������ܣ���ʱ����ʼ��
*��ڲ�����void
*���ڲ�����void
**************************************************/
void TimerInit(void)
{
	//��ʱ�� 8M
        TMCON |= bit0;    //------111 ;Timer0 ѡ��ʱ��Fsys
	//T0����
	TMOD |= 0x01;                 //0000 0001;Timer0���ù�����ʽ1
	TL0 = (65536 - (1000-1))%256;    //���ʱ�䣺ʱ��ΪFsys����2000*��1/Fsys��=125us;
	TH0 = (65536 - (1000-1))/256;
	TR0 = 0;
	ET0 = 1;//��ʱ��0����
	TR0 = 1;//�򿪶�ʱ��0
	//T2����
	TXINX = _0000_0010;   //ѡ��ʱ��T2
	TXCON = _0000_0000;   //T2��ʱ����ʽ
	TXMOD = _0000_0000;   //Tiemr2 ѡ��ʱ��Fsys/12
	THX = (65536-33434)/256;//������ֵ   //ʾ����ʵ�������5ms  �ֶ�����Ϊ 33434
	TLX = (65536-33434)%256;
	RCAPXH = (65536-33434)/256;//��װ�ؼ����� ���ʱ�䣺ʱ��ΪFsys����33334*��1/Fsys*1/12��=50ms;
	RCAPXL = (65536-33434)%256;
        ET2 = 1;//�򿪶�ʱ���ж�
	TRX = 1;//�򿪶�ʱ��3

}
 /**************************************************
*�������ƣ�void TimerInit(void)
*�������ܣ���ʱ����ʼ��
*��ڲ�����void
*���ڲ�����void
**************************************************/
void prg_ms1(void)//�˺��������жϳ�����ִ��
{
	DI_read_time++;
	if(DE_rec_time>0)
        {DE_rec_time--;}
	excit_time++;
        self_step_time++;
        Sleep_scan_time++;
	//
        ms10_cont++;
	if(ms10_cont>=10)
	{
		SYS_ms10_bit.byte=0xff;
		ms10_cont=0;
		ms20_cont++;
		ms100_cont++;

		if(Read_key_delay>0)
		{Read_key_delay--;}

	}
	if(ms20_cont>=2)
	{
		ms20_cont=0;
		SYS_ms20_bit.byte=0xff;
	}

	if(ms100_cont>=10)
	{
	    SYS_ms100_bit.byte=0xff;
	    ms100_cont=0;
	    ms500_cont++;
	    if(ms500_cont>=5)
    	    {
	    	ms500_cont=0;
		SYS_ms500_bit.byte=0xff;
            }
	}


        if(Key_last<0xffff)
        { Key_last++; }

        SYS_ms1_bit.byte=0xff;


	_SYS_Inspect_ms=1;
    SYS_Inspect_ms_general();
}
/**************************************************
*�������ƣ�void timer0()interrupt 1
*�������ܣ���ʱ���жϷ�����  125us
*��ڲ�����void
*���ڲ�����void
**************************************************/
void timer0(void)interrupt 1
{
	TL0 = (65536 - (1000-1))%256;    //���ʱ�䣺ʱ��ΪFsys����1000*��1/Fsys��=125us;  ��Ƶ8M
	TH0 = (65536 - (1000-1))/256;
       //
        communication_width_count++;
         if(!Rxd_IO)
	 {
            if(_Rxd_IO_)
            {
		 _Rxd_IO_=0;
		 communication_Rxd_IRQ();
		 communication_width_count=0;
            }
        }
	else
	{_Rxd_IO_=1;}
	step_time++;//��Ҷ
        exv_run();
	communication_Txd_IRQ();
	Get_the_pump_pwm_width();
//================   1msʱ��Ƭ   =====================================
      	if(++sys_time_rj >= 8)
      	{
              sys_time_rj = 0;
              prg_ms1();
	      Led_and_Key_scan();
      	}
}
/*************************************************
 // ��������    : timer2
 // ��������    : ��ʱ40ms
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
static xdata UI16 sec_cont=0;//1S��ʱ
void Timer2Int(void) interrupt 5
{
        TFX = 0;        //�������
	//
	if(++sec_cont>=20)
	{
	     sec_cont=0;
	     SYS_S_bit.byte=0xff;
	}
}
/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
