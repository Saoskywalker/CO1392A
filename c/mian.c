//************************************************************
//  Copyright (c) ��������Ԫ΢�������޹�˾
//	�ļ�����	: main.c
//	����		: Yanni
//	ģ�鹦��	: SC91F8311����DEMO
//  ����������:
// 	�汾		: 2015-12-25:
//              :
//*************************************************************
#include "General.h"
//#include "SOC_DebugTouchKey.h"
//*****************ȫ�ֱ�����***************************************************
void main(void)
{
    Sys_Initial();
    //SOCAPI_DeBugTouchKey_Init();//��̬�����
    //�ϵ�ȫ��
    while(Power_Delay_Time>0)
    {
        //��ʾ�¶ȸ���
        Troom_update_time=3;
        _temp_update_enb=1;
        Troom_dsp_com=Temp_room.value;
        //
#if ((defined DEBUG_EN) && !DEBUG_EN)
        WDTCON|=0x10;//WDT_Clear
#endif
        Temp_Deal();
        key_deal();
        Remote_Deal();
        LED_display();
        SYS_Sleep_deal();
        general_deal();
        SYS_Inspect_Deal();
        communication_Deal();
        exv_control();
    }
    //EEP ���Ͷ�ȡ
    SYS_Mach_type_judge();
    Sys_data_read_eep();
    //��ʾ�¶ȸ���
    Troom_update_time=3;
    _temp_update_enb=1;
    Troom_dsp_com=Temp_room.value;

    IAPPageErase(SYS_DATA_ADDR,IapROM);
    EEP_OffSet_DATA_ADDR=0;//������ ƫ������0��ʼ
    _Write_EEP_EN = 1;
    Sys_data_write();
    //ǿ�Ʋ������
    PowerKey_Delay_Time=200;
    while(PowerKey_Delay_Time>0)
    {
        WDTCON|=0x10;//WDT_Clear
        Temp_Deal();
        key_deal();
        SYS_Inspect_Deal();
        exv_control();
        LED_display();
    }

    while (1)
    {
#if ((defined DEBUG_EN) && !DEBUG_EN)
        WDTCON|=0x10;//WDT_Clear
#endif
        key_deal();
        Temp_Deal();
        LED_display();
        Remote_Deal();
        general_deal();
        EEP_deal();
        IO_operate();
        Sys_Control();
        SYS_Inspect_Deal();
        communication_Deal();
        exv_control();
        SYS_Sleep_deal();
    }
}
