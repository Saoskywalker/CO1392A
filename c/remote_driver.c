/**
  ******************************************************************************
  * @file remote.c

  ******************************************************************************

  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "General.h"			//ϵͳ ͨ�õ����ļ�

xdata UI08 width=0;//��ʱ����
xdata UI08 rec_time;//�ж�ң����������ɱ���
xdata UI32 Rec_IO_buf=0;//�洢��һ��IOֵ
xdata UI08 _rec_enb=0;//ң����ʹ�ܱ�־
xdata UI08 _Rec_Ok=0;//ң����������ɱ�־
xdata UI08 rec_data[total_byte]={0};//ң������������
xdata UI08 rec_bits=0;//ң��������bit����
xdata UI08 rec_bytes=0;//ң��������byte����


/*************************************************
//����        :	prg_10ms_remote
//����        :	ÿ10MS�ж�һ�Σ���������Ƿ����
//��ڲ���    :	��
//���ڲ���    :	��
//��	��:
//��	��: 	GCE XXX
************************************************/
void prg_10ms_remote(void)
{
   if(!_10ms_for_remote)
   { return; }
   _10ms_for_remote=0;

   if(rec_time != 0)//30ms������ȷ����ң�������
   {
	  if(--rec_time==0)
	  {
	  	_Rec_Ok=1;
	  }
   }
}

/*************************************************
//����        :	Remote_Req
//����        :	����ʱ���������
//��ڲ���    :	��
//���ڲ���    :	��
//��	��:
//��	��: 	GCE XXX
************************************************/
void  Remote_Req(void)//125usɨ��
{
    if((width > HEAD-15)&&(width <= HEAD+15))//ͷ��ʶ�� HEAD: 11.125~14.875 ms
    {
      	_rec_enb=1;
      	rec_bits=0X01;
      	rec_bytes=0;
        rec_data[rec_bytes]=0;
      	return;
    }

    if(!_rec_enb)
    {
    	_rec_enb=0;
        return;
    }

    if(rec_bytes>=total_byte)//��ֹ����������� total_byte:7
    {
    	_rec_enb=0;
        return;
    }
    rec_time=3;
    if((width>=ONE_1-4)&&(width<=ONE_1+4))//ONE_1: 1.75~2.75 ms
    {
        rec_data[rec_bytes]|=rec_bits;
    }
    else if((width>=ZERO_0-3)&&(width<=ZERO_0+3))//ZERO_0: 0.75~1.5ms
    {
        //rec_data[rec_bytes]&=~rec_bits;//����ǰ����
    }
    else
    {
        _rec_enb=0;
    }

    rec_bits=rec_bits<<1;
    if(rec_bits==0)
    {
    	rec_bytes++;
        rec_bits=0x01;
        if(rec_bytes<total_byte)
        {
            rec_data[rec_bytes]=0;
        }
    }

}


/*************************************************
//����        :	remote_IRQ
//����        :	0.0125MS��ʱ�жϵ���
//��ڲ���    :	��
//���ڲ���    :	��
//��	��:
//��	��: 	GCE XXX
************************************************/
void remote_IRQ(void)
{
    width++;
    if(REC_IO_VALUE==0)//�½���
    {
	    if(Rec_IO_buf)
        {
            Remote_Req();
            width=0;
	    }
	    Rec_IO_buf=0;
    }
    else
    {
        Rec_IO_buf=1;
    }
}
/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
