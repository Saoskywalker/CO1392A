#include "General.h"


//======================ȫ�ֱ���=================================
xdata UI08   Buz_list;		//BUZZģʽ
xdata UI08   communication_timer;	//ͨ�Ź���ʱ��
xdata UI08   Txd_count;		//���ʹ���
xdata UI08   Txd_timer;		//���ͼ��ʱ��
xdata UUI08  COM_bit0;		//��־λ
/***********************************************************
*�� �� ����  void  communication_data_init(void)
*����������  ͨѶ��ʼ������
*���������  ��
*��������ֵ����
***********************************************************/
void   communication_data_init(void)
{
    Buz_list=0;
    COM_bit0.byte=0;
    communication_timer=180;
    Txd_count=0;
    Txd_timer=0;
}


/***********************************************************
*�� �� ����  void  sleep_deal(void)
*����������  ˯�ߴ�����
*���������  ��
*��������ֵ����
***********************************************************/
void  sleep_deal(void)
{
    if((Power_Status)||(_sys_err )||(Dsp_Time>0)||(_Self_Test))
    {
        sleep_Time=10;
        _Sleep_status=0;
        _Sleep_status_buf=0;
    }
    else
    {
        communication_timer=180;
    }
}


/***********************************************************
*�� �� ����  void  Buz_mode(UI08  mode_buf)
*����������  BUZZģʽ
*���������  mode_buf��������ģʽ
*��������ֵ����
***********************************************************/
void  Buz_mode(UI08  mode_buf)
{
    UI08 buf=0;
    buf=(Buz_list&0xf0)>>4;
    buf++;
    Buz_list=(mode_buf&0x0f)+(buf<<4);
}


/***********************************************************
*�� �� ����  UI08 crc_check(UI08 *buffer ,UI08 length)
*����������  8λCRC����
*���������  *buffer:У������
             length:���ݳ���
*��������ֵ: ����ֵ
***********************************************************/
UI08 crc_check(UI08 *buffer,UI08 length)
{
    UI08 crc = 0;
    UI08 loop=0;
    UI08 loop_1=0;
    for(loop = 0; loop < length; loop++)
    {
        crc ^= buffer[loop];
        for(loop_1 = 0; loop_1 < 8; loop_1++)
        {
            if((crc & 1) == 1)
            {
                crc = crc >> 1;
                crc ^= 0x8C;
            }
            else
            {
                crc = crc >> 1;
            }
        }
    }
    return crc;
}


/***********************************************************
*�� �� ����  void rxd_data_protocl(void)
*����������  ��������ת��
*���������  ��
*��������ֵ: ��
***********************************************************/
void rxd_data_protocl(void)
{
//   UUI16  buf;
    UUI08  UUI08_buf;

    if(_Self_Test)
    {
        communication_timer=2;
    }
    else
    {
        communication_timer=180;
    }
    Temp_room.AD_value=rxd_buf[1]&0x03;
    Temp_room.AD_value=Temp_room.AD_value<<8;
    Temp_room.AD_value=Temp_room.AD_value+rxd_buf[2];
    Temp_room.AD_value=Temp_room.AD_value>>1;

    Temp_coil.AD_value=(rxd_buf[1]&0x0c)>>2;
    Temp_coil.AD_value=Temp_coil.AD_value<<8;
    Temp_coil.AD_value=Temp_coil.AD_value+rxd_buf[3];
    Temp_coil.AD_value=Temp_coil.AD_value>>1;

    Tcomp_coil.AD_value=(rxd_buf[1]&0x30)>>4;
    Tcomp_coil.AD_value=Tcomp_coil.AD_value<<8;
    Tcomp_coil.AD_value=Tcomp_coil.AD_value+rxd_buf[4];
    Tcomp_coil.AD_value=Tcomp_coil.AD_value>>1;

    if(rxd_buf[5]&bit7)
    {
        _Water_Full_status=1;
    }
    else
    {
        _Water_Full_status=0;
    }

    UUI08_buf.byte=rxd_buf[5];
    _Sleep_status_buf=UUI08_buf.bit_.b6;
    _pump_sw_status =UUI08_buf.bit_.b5;
    _INFAN_ERR=UUI08_buf.bit_.b4;
    _OUTFAN_ERR=UUI08_buf.bit_.b3;

    _INFAN_ERR=0;
    _OUTFAN_ERR=0;

}


/***********************************************************
*�� �� ����  void txd_data_protocol(void)
*����������  ��������
*���������  ��
*��������ֵ: ��
***********************************************************/
void txd_data_protocol(void)
{
    UUI08  UUI08_buf;

    if(!_txd_ms100_tick)
    {
        return;
    }
    _txd_ms100_tick=0;

    Txd_timer++;
    if(Txd_timer<2)
    {
        return;
    }
    Txd_timer=0;

    if( (_Sleep_status_buf)&&(_Sleep_status) )
    {
        return;
    }


    txd_buf[0]=0xaa;

    Txd_count++;
    if(Txd_count>=5)//������10�Σ�1s/��
    {
        Txd_count=0;
        if(Buzz_Cnt<10)
        {
            Buzz_Cnt++;
            Buz_mode(BUZZ_TYPE8);
        }
    }
    txd_buf[1]=Buz_list;

    if((Power_Status)||(_Self_Test==ENABLE))
    {
        UUI08_buf.byte=0;
        if(Lfan_para.OUT==ON)
        {
            UUI08_buf.bit_.b0=1;
        }
        else if(Mfan_para.OUT==ON)
        {
            UUI08_buf.bit_.b1=1;
        }
        else if(Hfan_para.OUT==ON)
        {
            UUI08_buf.bit_.b2=1;
        }

        if(_Self_Test)
        {
            UUI08_buf.bit_.b4=1;
        }

        if(Comp_para.OUT==ON)
        {
            UUI08_buf.bit_.b5=1;
        }

        if(Pump_para.OUT==ON)
        {
            UUI08_buf.bit_.b6=1;
        }

        if(Way4_para.OUT==ON)
        {
            UUI08_buf.bit_.b7=1;
        }

        txd_buf[2]=UUI08_buf.byte;

        txd_buf[3]=0;


        UUI08_buf.byte=0;

        if(OutLfan_para.OUT==ON)
        {
            UUI08_buf.bit_.b0=1;
        }
        else if(OutHfan_para.OUT==ON)
        {
            UUI08_buf.bit_.b2=1;
        }

        UUI08_buf.bit_.b7=1;

        txd_buf[4]=UUI08_buf.byte;
        txd_buf[5]=0;
        txd_buf[6]=0;


    }
    else
    {
        txd_buf[2]=0;
        txd_buf[3]=0;
        txd_buf[4]=0;
        txd_buf[5]=0;
        txd_buf[6]=0;
    }

    if(_Sleep_status)
    {
        txd_buf[4]|=bit6;//����Դ�巢˯��״̬
    }

    txd_buf[txd_length-1]=crc_check(txd_buf,(txd_length-1));

    _txd_data=0;
    _txd_end=0;
    pulse_cnt=0;
    _txd_start=1;
    _txd_en=1;


}


/***********************************************************
*�� �� ����  void communication_txd(void)
*����������  ͨ�ŷ���
*���������  ��
*��������ֵ: ��
***********************************************************/
void communication_txd(void)
{
    txd_data_protocol();
}


/***********************************************************
*�� �� ����  void communication_rxd(void)
*����������  ͨ�Ž���
*���������  ��
*��������ֵ: ��
***********************************************************/
void communication_rxd(void)
{
    UI08 sum=0;
    if(!_Rxd_OK)
    {
        return;
    }
    _Rxd_OK=0;
    if(rxd_buf[0]!=0xaa)//ͷ��
    {
        return;
    }
    sum=crc_check(rxd_buf,rece_data_lenth-1);//У��
    rxd_buf[0]=0;
    if(rxd_buf[rece_data_lenth-1]!=sum)
    {
        return;
    }
    rxd_data_protocl();//��������
}


/***********************************************************
*�� �� ����  void communication_Deal(void)
*����������  ͨ�Ŵ���
*���������  ��
*��������ֵ: ��
***********************************************************/
void communication_Deal(void)
{
    sleep_deal();
    communication_rxd();
    communication_txd();
}


