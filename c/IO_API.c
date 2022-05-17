#include "General.h"

MCU_xdata DO_PARA Comp_para; //ѹ����
MCU_xdata DO_PARA UVC_para;	 // UVC

MCU_xdata DI_PARA water_full_para; //ˮ��ˮλ���
// MCU_xdata DI_PARA     pump_sw_para;//��ˮ��ˮλ���

/*************************************************
 // ��������    : io_data_init
 // ��������    : ���ݳ�ʼ��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void io_data_init(void)
{
	Comp_para.on_time = 0;
	Comp_para.off_time = 180;
	Comp_para.BUF = OFF;
	Comp_para.OUT = OFF;

	UVC_para.on_time = 0;
	UVC_para.off_time = 0;
	UVC_para.BUF = OFF;
	UVC_para.OUT = OFF;

	water_full_para.Status = DI_SHORT;
	water_full_para.short_time = 0;
	water_full_para.cut_time = 0;
}

void prg_100mS_IO(void)
{
	if (!_ms100_for_IO)
	{
		return;
	}
	_ms100_for_IO = 0;

	if (water_full_para.Status)
	{
		if (water_full_para.cut_time < 0xffff)
		{
			water_full_para.cut_time++;
		}
		water_full_para.short_time = 0;
	}
	else
	{
		if (water_full_para.short_time < 0xffff)
		{
			water_full_para.short_time++;
		}
		water_full_para.cut_time = 0;
	}
}
/*************************************************
 // ��������    : prg_s_IO
 // ��������    : Sʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_s_IO(void)
{
	UI08 buf = 0;
	if (!_s_for_IO)
	{
		return;
	}
	_s_for_IO = 0;

	if (_Fast_Test) //???fast_f
	{
		buf = 60;
	}
	else
	{
		buf = 1;
	}

	if (Comp_para.OUT)
	{
		Comp_para.off_time = 0;
		if (Comp_para.on_time < (0xffff - buf))
		{
			Comp_para.on_time = Comp_para.on_time + buf;
		}
		else
		{
			Comp_para.on_time = 0xffff;
		}
	}
	else
	{
		Comp_para.on_time = 0;
		if (Comp_para.off_time < (0xffff - buf))
		{
			Comp_para.off_time = Comp_para.off_time + buf;
		}
		else
		{
			Comp_para.off_time = 0xffff;
		}
	}

	if (Fan_Speed_Out != OFF_FAN)
	{
		FAN_OFF_timer = 0;
		if (FAN_ON_timer < (0xffff - buf))
		{
			FAN_ON_timer = FAN_ON_timer + buf;
		}
		else
		{
			FAN_ON_timer = 0xffff;
		}
	}
	else
	{
		FAN_ON_timer = 0;
		if (FAN_OFF_timer < (0xffff - buf))
		{
			FAN_OFF_timer = FAN_OFF_timer + buf;
		}
		else
		{
			FAN_OFF_timer = 0xffff;
		}
	}
}
/*************************************************
 // ��������    : IO_operate
 // ��������    : IO����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void IO_operate(void)
{
	prg_100mS_IO();
	prg_s_IO();
}
/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
