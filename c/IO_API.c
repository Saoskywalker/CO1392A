#include "General.h"

//=================================����==========================================
xdata  UI08 DcPump_count=0;
xdata  UI32 DcPump_buf=0;

xdata UI08             water_delaytime=0;             //��ˮ����ʱ����Buzz������
xdata UI08             DI_read_time=0;
xdata SHORTCUT_STATUS  _DcPump_Full_status_buf=DI_SHORT;

/////////////////////////////////////////////////////////////
//IO���

xdata DO_PARA  Comp_para,
               Way4_para,
               Pump_para,
               Hfan_para,
               Mfan_para,
               Lfan_para,
               OutHfan_para,
               OutLfan_para,
               Power_para,//���ؿ�����ʱ���У���Ҷ����
               DCPUMP_para;

xdata DO_PARA *DO_struct_addr[DO_total]=
{
 &Comp_para,
 &Way4_para,
 &Pump_para,
 &Hfan_para,
 &Mfan_para,
 &Lfan_para,
 &OutHfan_para,
 &OutLfan_para,
 &Power_para,//���ؿ�����ʱ���У���Ҷ����
 &DCPUMP_para
};
/* Private functions ---------------------------------------------------------*/
/*
PA0 LED3     PA1 LED10    PA2 ����     PA3 ����     PA4 R      PA5 ����   PA6 ����   PA7 ����
PB0 SWING4   PB1 SWING3   PB2 SWING2   PB3 SWING1   PB4 ����   PB5 D      PB6 C      PB7 ����
PC0 LED2     PC1 LED1     PC2 LUX_AD   PC3 SAD      PC4 SCL    PC5 TXD    PC6 RXD    PC7 ����
PE0 LED9     PE1 LED8     PE2 LED7     PE3 LED6     PE4 LED5   PE5 LED4
*/
void IO_init(void)
{
	//TK�ϵ�IO����Ϊǿ�������1
	P0CON = 0xFF;  //0Ϊ���� 1Ϊ���
	P0PH  = 0x00;  //0��ʹ���������� 1������������
	//
	P1CON = 0xFF;
	P1PH  = 0x00;
	//
	P2CON = 0xFF;
	P2PH  = 0x00;
	//
	P3CON = 0xFF;
	P3PH  = 0x00;
	//
	P4CON = 0xEB;
	P4PH  = 0x00;
	//
	P5CON = 0xFF;
	P5PH  = 0x00;

	P0 = 0xFF;
	P1 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;
	P4 = 0xF7;
	P5 = 0xFF;

        DCPUMP_OFF;
        //IOHCON0= 0xFF;
	//IOHCON1= 0xFF;
}
/*************************************************
 // ��������    : void prg_ms100_IO(void)
 // ��������    : 100����ʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void IO_data_Init(void)
{
    DO_PARA *ptr;
    UI08 i;
    for(i=0;i<DO_total;i++)
    {
	ptr=DO_struct_addr[i];
        (*ptr).BUF=OFF;
        (*ptr).OUT=OFF;
        (*ptr).off_time=0;
        (*ptr).on_time=0;
    }
    //
    Comp_para.off_time=180;
}

/*************************************************
 // ��������    : void prg_ms100_IO(void)
 // ��������    : 100����ʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_ms100_IO(void)
{
   if(!_ms100_for_IO)
   {
       return;
   }
   _ms100_for_IO=0;


   if(_DcPump_Full_status_buf==DI_SHORT)
   {
	_water_pipe_Status=1;
   }
   else
   {
        _water_pipe_Status=0;
   }

}
/*************************************************
 // ��������    : void DI_read(void)
 // ��������    : ��IO��״̬
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void DI_read(void)
{
  UI32 b;

  if(DI_read_time<DI_read_interval)
  {return;}

  DI_read_time=0;

  if(Tcomp_coil.AD_value<150)//�Ͽ�
  {b=1;}
  else if(Tcomp_coil.AD_value>350)//�պ�
  {b=0;}

  if(b!=DcPump_buf)
  {
  	DcPump_buf=b;
	DcPump_count=0;
  }
  else
  {
      	DcPump_count++;
	if(DcPump_count>=3)
	{
		_first_read_IO=1;//����һ���ȶ���IO��
		DcPump_count=0;
		if(DcPump_buf)
		{
			_DcPump_Full_status_buf=DI_CUT;
		}
		else
		{
			_DcPump_Full_status_buf=DI_SHORT;
		}
	  }
	}
}
/*************************************************
 // ��������    : void prg_s_IO(void)
 // ��������    : S��ʱ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_s_IO(void)
{
    UI08 i;
    DO_PARA *ptr;
    UI08 num=0;

    if(!_s_for_IO)
    {return;}
    _s_for_IO=0;

	num=(_EC_Fast_Test)?(60):(1);
	for(i=0;i<DO_total;i++)
	{
		ptr=DO_struct_addr[i];
		if((*ptr).OUT ==1)
		{
			(*ptr).off_time=0;
			if((*ptr).on_time<0xffff-num)
			{
                (*ptr).on_time+=num;
			}
		}
		else
		{
			(*ptr).on_time=0;
			if((*ptr).off_time<0xffff-num)
			{
                (*ptr).off_time+=num;
			}
		}
	}
}

/*************************************************
 // ��������    : void IO_operate(void)
 // ��������    :
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void IO_operate(void)
{
  prg_ms100_IO();
  prg_s_IO();
  DI_read();
}






/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
