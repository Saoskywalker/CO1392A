#include "General.h"


xdata UUI08   SYS_ms1_bit;//1ms时间片
xdata UUI08   SYS_ms10_bit;//10ms时间片
xdata UUI08   SYS_ms20_bit;//20ms时间片
xdata UUI08   SYS_ms100_bit;//100ms时间片
xdata UUI08   SYS_ms500_bit;//500ms时间片
xdata UUI08   SYS_S_bit;//1000ms时间片

xdata UI08   ms10_cont;//10ms计数
xdata UI08   ms20_cont;//20ms计数
xdata UI08   ms100_cont;//100ms计数
xdata UI08   ms500_cont;//500ms计数
xdata UI08   sys_time_rj;//1ms计数
/**************************************************
*函数名称：void timer_data_init(void)
*函数功能：变量初始化
*入口参数：void
*出口参数：void
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
*函数名称：void TimerInit(void)
*函数功能：定时器初始化
*入口参数：void
*出口参数：void
**************************************************/
void TimerInit(void)
{
	//主时钟 8M
        TMCON |= bit0;    //------111 ;Timer0 选择时钟Fsys
	//T0设置
	TMOD |= 0x01;                 //0000 0001;Timer0设置工作方式1
	TL0 = (65536 - (1000-1))%256;    //溢出时间：时钟为Fsys，则2000*（1/Fsys）=125us;
	TH0 = (65536 - (1000-1))/256;
	TR0 = 0;
	ET0 = 1;//定时器0允许
	TR0 = 1;//打开定时器0
	//T2设置
	TXINX = _0000_0010;   //选择定时器T2
	TXCON = _0000_0000;   //T2定时器方式
	TXMOD = _0000_0000;   //Tiemr2 选择时钟Fsys/12
	THX = (65536-33434)/256;//计数器值   //示波器实测有误差5ms  手动补偿为 33434
	TLX = (65536-33434)%256;
	RCAPXH = (65536-33434)/256;//重装载计数器 溢出时间：时钟为Fsys，则33334*（1/Fsys*1/12）=50ms;
	RCAPXL = (65536-33434)%256;
        ET2 = 1;//打开定时器中断
	TRX = 1;//打开定时器3

}
 /**************************************************
*函数名称：void TimerInit(void)
*函数功能：定时器初始化
*入口参数：void
*出口参数：void
**************************************************/
void prg_ms1(void)//此函数放在中断程序中执行
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
*函数名称：void timer0()interrupt 1
*函数功能：定时器中断服务函数  125us
*入口参数：void
*出口参数：void
**************************************************/
void timer0(void)interrupt 1
{
	TL0 = (65536 - (1000-1))%256;    //溢出时间：时钟为Fsys，则1000*（1/Fsys）=125us;  主频8M
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
	step_time++;//摆叶
        exv_run();
	communication_Txd_IRQ();
	Get_the_pump_pwm_width();
//================   1ms时间片   =====================================
      	if(++sys_time_rj >= 8)
      	{
              sys_time_rj = 0;
              prg_ms1();
	      Led_and_Key_scan();
      	}
}
/*************************************************
 // 函数名称    : timer2
 // 功能描述    : 定时40ms
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
static xdata UI16 sec_cont=0;//1S定时
void Timer2Int(void) interrupt 5
{
        TFX = 0;        //溢出清零
	//
	if(++sec_cont>=20)
	{
	     sec_cont=0;
	     SYS_S_bit.byte=0xff;
	}
}
/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
