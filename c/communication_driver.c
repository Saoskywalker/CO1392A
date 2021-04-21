#include "General.h"


//======================ȫ�ֱ���=================================
xdata UI08   txd_buf[txd_length];		//��������
xdata UI08   rxd_buf[rece_data_lenth];	//���պ���
xdata UI08   pulse_cnt;			//��������
xdata UI08   txd_byte;			//����λ
xdata UI08   txd_bit;				//����bit
xdata UI08   Rxd_bits;			//����bit
xdata UI08   Rxd_bytes;			//����λ
xdata UI08   TXD_L_timer;                   //���͵͵�ƽʱ��
UI08   communication_width_count;	//���ؼ��ʱ��
xdata UUI08  communication_bit1= {0};		//��־λ


/***********************************************************
*�� �� ����  void  communication_data_init(void)
*����������  ͨѶ��ʼ������
*���������  ��
*��������ֵ����
***********************************************************/
void comm_data_init(void)
{
    UI08 i=0;
    communication_bit1.byte=0;
    pulse_cnt=0;
    txd_byte=0;
    txd_bit=0;
    Rxd_bits=0;
    Rxd_bytes=0;
    communication_width_count=0;
    TXD_L_timer=0;

    for(i=0; i<txd_length; i++)
    {
        txd_buf[i]=0;
        rxd_buf[i]=0;
    }

}

/***********************************************************
*�� �� ����  void  communication_Txd_IRQ(void)
*����������  �ײ㷢�ͺ���
*���������  ��
*��������ֵ����
***********************************************************/
void communication_Txd_IRQ(void)
{

    if(!_txd_en)
    {
        return;
    }

    pulse_cnt++;

    if(_txd_data) //������
    {
        TXD_L_timer=2;
        if(txd_buf[txd_byte]&txd_bit)
        {
            if(pulse_cnt>=COM_ONE)//����1
            {
                pulse_cnt=0;
                txd_bit<<=1;
            }
        }
        else
        {
            if(pulse_cnt>=COM_ZERO)//����0
            {
                pulse_cnt=0;
                txd_bit<<=1;
            }
        }

        if(0x00==txd_bit)
        {
            txd_bit=0x01;

            if(++txd_byte>=txd_length)//�����޶�
            {
                pulse_cnt=0;
                txd_byte=0;
                _txd_end=1;
                _txd_data=0;
                _txd_start=0;
            }
        }
    }
    else if(_txd_start)//��ͷ��
    {
        TXD_L_timer=2+1;
        if(pulse_cnt>=(COM_HEAD+1))
        {
            pulse_cnt=0;
            txd_bit=0x01;
            _txd_start=0;
            _txd_data=1;
            _txd_end=0;
        }
    }
    else if(_txd_end)//���շ�
    {
        TXD_L_timer=2;
        if(pulse_cnt>=COM_ZERO)
        {
            _txd_start=0;
            _txd_data=0;
            _txd_end=0;
            _txd_en=0;
        }
    }
    else
    {
        pulse_cnt=4;
        _txd_start=1;
        _txd_data=0;
        _txd_end=0;
        _txd_en=0;
    }

    if(pulse_cnt<TXD_L_timer)
    {
        Txd_L;
    }
    else
    {
        Txd_H;
    }
}
/***********************************************************
*�� �� ����  void  communication_Rxd_IRQ(void)
*����������  �ײ���պ���
*���������  ��
*��������ֵ����
***********************************************************/
void communication_Rxd_IRQ(void)
{

    if((communication_width_count>=(COM_HEAD-3))&&(communication_width_count<=(COM_HEAD+3)))//30
    {
        _Rxd_start=1;
        Rxd_bits=0x01;
        Rxd_bytes=0;
        rxd_buf[Rxd_bytes]=0;
        return;
    }
    //
    if(!_Rxd_start)
    {
        return;
    }
    //
    if(Rxd_bytes>=rece_data_lenth)
    {
        _Rxd_start=0;
        return;
    }
    //
    if((communication_width_count>=(COM_ONE-2))&&(communication_width_count<=(COM_ONE+2)))//18
    {
        rxd_buf[Rxd_bytes]|=Rxd_bits;
    }
    else if((communication_width_count>=(COM_ZERO-2))&&(communication_width_count<=(COM_ZERO+2)))// 8
    {
        rxd_buf[Rxd_bytes]&=~Rxd_bits;
    }
    else
    {
        _Rxd_start=0;
        return;
    }
    //
    Rxd_bits<<=1;
    if(0==Rxd_bits)
    {
        Rxd_bits=0x01;
        Rxd_bytes++;
        if(Rxd_bytes>=rece_data_lenth)
        {
            _Rxd_OK=1;
            _Rxd_start=0;
        }
        else
        {
            rxd_buf[Rxd_bytes]=0;
        }
    }
}

