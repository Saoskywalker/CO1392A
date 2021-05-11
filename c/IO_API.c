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
      DCPUMP_para,
      UVC_para;

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
    &DCPUMP_para,
    &UVC_para
};

/* ��IO�ڶ���*/
/*
SW1->P37  |  LED1->SEGG->P30  |  LED10->P23  |  LED3-1->P43  |  DISP_RXD->P02  |  SWINGA->P05   |   UV_LED->P04     |   DEBUG_D_TXD->P13            
SW2->P36  |  LED2->SEGB->P10  |  LED11->P24  |  LED3-2->P42  |  DISP_TXD->P01  |  SWINGB->P06   |   UV_CHECK->P03   |   DEBUG_C_RXD->P11
SW3->P35  |  LED4->SEGC->P12  |  LED12->P25  |               |  WIFI_RXD->P20  |  SWINGC->P52
SW4->P34  |  LED5->SEGD->P14  |  COM4->P41   |               |  WIFI_TXD->P21  |  SWINGD->P53
SW5->P33  |  LED6->SEGA->P15
SW6->P00  |  LED7->SEGH->P17
SW7->P27  |  LED8->SEGF->P22
SW8->P26  |  LED9->SEGE->P16
COM1->P31 |  COM3->P4.0
COM2->P32 |                           
*/
void IO_init(void)
{
    //TK�ϵ�IO����Ϊǿ�������1

    //0Ϊ���� 1Ϊ���
    //0��ʹ���������� 1������������

    // P0CON |= (1<<0);//SW6 ���
    // P0CON |= (1<<1);//DISP_TXD ���
    // P0CON &= (~(1<<2));//DISP_RXD ����
    // P0CON &= (~(1<<3));//UV_V_CHECK ����
    // P0CON |= (1<<4);//UV�ƣ����
    // P0CON |= (1<<5);//SWINGA�����
    // P0CON |= (1<<6);//SWINGB�����

    P0CON = _0111_0011;
    P0PH  = _0000_0000;  //0��ʹ���������� 1������������

    // P1CON |= (1<<0);//SEGB ���
    // P1CON &= (~(1<<1));//DEBUG_C_RXD ����
    // P1CON |= (1<<2);//SEGC ���
    // P1CON |= (1<<3);//DEBUG_D_TXD ���
    // P1CON |= (1<<4);//SEGD�����
    // P1CON |= (1<<5);//SEGA�����
    // P1CON |= (1<<6);//SEGE�����
    // P1CON |= (1<<7);//SEGH�����

    P1CON = _1111_1101;
    P1PH  = _0000_0000;

    // P2CON &= (~(1<<0));//WIFI_RXD ����
    // P2CON |= (1<<1);//WIFI_TXD ���
    // P2CON |= (1<<2);//SEGF ���
    // P2CON |= (1<<3);//LED3 ���
    // P2CON |= (1<<4);//LED4�����
    // P2CON |= (1<<5);//LED5�����
    // P2CON |= (1<<6);//SW8�����
    // P2CON |= (1<<7);//SW7�����

    P2CON = _1111_1110;
    P2PH  = _0000_0000;

    // P3CON |= (1<<0);//SEGG ���
    // P3CON |= (1<<1);//COM1 ���
    // P3CON |= (1<<2);//COM2 ���
    // P3CON |= (1<<3);//SW5 ���
    // P3CON |= (1<<4);//SW4�����
    // P3CON |= (1<<5);//SW3�����
    // P3CON |= (1<<6);//SW2�����
    // P3CON |= (1<<7);//SW1�����

    P3CON = _1111_1111;
    P3PH  = _0000_0000;

    // P4CON |= (1<<0);//COM3 ���
    // P4CON |= (1<<1);//COM4 ���
    // P4CON |= (1<<2);//LED2 �������
    // P4CON |= (1<<3);//LED1 ������
    // P4CON &= (~(1<<4));//�������ͷ������
    P4CON = _0000_1111;
    P4PH = _0001_0000;
    
    // P5CON |= (1<<0);//COM3 ���
    // P5CON |= (1<<1);//COM4 ���
    // P5CON |= (1<<2);//SWINGC ���
    // P5CON |= (1<<3);//SWINGD ���
    P5CON = _0000_1111;
    P5PH  = _0000_0000;

    P0 = _1110_1111;
    P1 = _1111_1111;
    P2 = _1111_1111;
    P3 = _1111_1111;
    P4 = _1111_1111;
    P5 = _1111_1111;

    // DCPUMP_OFF;
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
    for(i=0; i<DO_total; i++)
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
    {
        return;
    }

    DI_read_time=0;

    if(Tcomp_coil.AD_value<150)//�Ͽ�
    {
        b=1;
    }
    else if(Tcomp_coil.AD_value>350)//�պ�
    {
        b=0;
    }

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
    {
        return;
    }
    _s_for_IO=0;

    num=(_EC_Fast_Test)?(60):(1);
    for(i=0; i<DO_total; i++)
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
//����        :	DO_output
//����        : IO �������
//��ڲ���    :	��
//���ڲ���    :	��
//��	��: 	GCE XXX
//��	��: 	GCE XXX
**************************************************/
void DO_output(void)
{
    if((power_status==OFF)&&(!_Self_Test))
    {
        UVC_OFF;
        return;
    }
  //
    if(UVC_para.OUT==ON) 
    {
        UVC_ON;
    } 
    else 
    {
        UVC_OFF;
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
    DO_output();
}






/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
