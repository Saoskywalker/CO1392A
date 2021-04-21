#include "General.h"

xdata UUI08  Sleep_bit= {0}; //bit
xdata UI08 Sleep_scan_time=0;//ɨ��ʱ��
xdata UI08 Sleep_delay_time=10;//˯����ʱ
xdata UI08 Scl_Low_Count=0;//low��ʱ
xdata UI08 Scl_High_Count=0;//high��ʱ
xdata UI08 IICEN_Count=0;//��ʱ

/***********************************************************
*�� �� ���� void prg_s_Sleep()
*����������
*���������  ��
*��������ֵ����
***********************************************************/
void  prg_s_Sleep(void)
{
    if(!_s_for_Sleep)
    {
        return;
    }

    _s_for_Sleep=0;

    if(Sleep_delay_time>0)
    {
        Sleep_delay_time--;
    }
}
/*******************************************************************************
����ʱ�������������Ƿ��������ң��������
*******************************************************************************/
void  Read_Status(void)
{
    if( (_IICSleep_status)
            ||(Sleep_scan_time<1))
    {
        return;
    }

    Sleep_scan_time=0;

    SCL_input;
    if(Scl)
    {
        if(Scl_High_Count<0xff)
        {
            Scl_High_Count++;
        }
        Scl_Low_Count=0;
        if((Scl_High_Count>=3)&&(IICEN_Count>0))
        {
            _Read_Sleep_OK=1;
            IICEN_Count=0;
        }
    }
    else
    {
        if(Scl_Low_Count<0xff)
        {
            Scl_Low_Count++;
        }
        Scl_High_Count=0;
        if((Scl_Low_Count>=3)&&(IICEN_Count<0xff))
        {
            IICEN_Count++;
        }
    }
}
/*******************************************************************************
ȷ��IIC�����Ƿ񷢸������������
*******************************************************************************/
void  Verify_Status(void)
{
    if(Sys_EEP_SYTP==0xff)
    {
        _IICSleep_status=1;
        Sleep_delay_time=10;
        IICEN_Count=0;
        _Read_Sleep_OK=0;
        Scl_Low_Count=0;
        Scl_High_Count=0;
    }

    if((Power_Status==ON)|| //����
            (Dsp_Time>0)||//�������ò���
            (Timer_run>0)||// ��ʱ����
            (Power_Delay_Time>0)||// �״��ϵ�3��
            (_temp_room_err)||//���¹���
            (_temp_coil_err)||//���¹���
            (Temp_Setting_Time>0))  //�¶�����
    {
        _IICSleep_status=1;
        Sleep_delay_time=10;
        IICEN_Count=0;
        _Read_Sleep_OK=0;
        Scl_Low_Count=0;
        Scl_High_Count=0;
    }
    else
    {
        if((IICEN_Count==0)&&(Sleep_delay_time==0))
        {
            _IICSleep_status=0;
        }

        if((_Read_Sleep_OK)&&(!_IICSleep_status))
        {
            _IICSleep_status=1;
            IICEN_Count=0;
            Sleep_delay_time=10;
            _Read_Sleep_OK=0;
        }

        if(!_IICSleep_status)
        {
            SCL_input;
            SDA_input;
        }
    }
}
/**************************************************
*�������ƣ�void SYS_Sleep_deal(void)
*�������ܣ�
*��ڲ�����void
*���ڲ�����void
**************************************************/
void SYS_Sleep_deal(void)
{
    if(_Self_Test==ENABLE)
    {
        return;
    }

    prg_s_Sleep();
    Read_Status();
    Verify_Status();
}
























