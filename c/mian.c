//************************************************************
//  Copyright (c) ��������Ԫ΢�������޹�˾
//	�ļ�����	: main.c
//	����		: Yanni
//	ģ�鹦��	: SC91F8311����DEMO
//  ����������:
// 	�汾		: 2015-12-25:
//              :
//*************************************************************
// #define TOUCH_DYN_DEBUG     //!��̬���Դ����������SC95F8X1X_HighSensitiveTKDynamicDebug_S_V0.0.2.LIB
#include "General.h"

#ifdef TOUCH_DYN_DEBUG
#include "SOC_DebugTouchKey.h"
#endif

//*****************ȫ�ֱ�����***************************************************
void main(void)
{
    Sys_Initial();
#ifdef TOUCH_DYN_DEBUG
    SOCAPI_DeBugTouchKey_Init(); //��̬�����
#endif
    //�ϵ�ȫ��
    while (Power_Delay_Time > 0)
    {
        if (_10mS_For_SYS)
        {
            WDTCON |= 0x10; // WDT_Clear
            Temp_Deal();
            Key_Deal();
            LED_display();
            general_deal();
            tuya_deal();
            communication_Deal();
            CLEAR_SYS_TIME_BIT();
        }
    }
    // EEP ���Ͷ�ȡ
    // SYS_Mach_type_judge();
    Sys_data_read_eep();

    while (1)
    {
        if (_10mS_For_SYS)
        {
            WDTCON |= 0x10; // WDT_Clear
            Key_Deal();
            Temp_Deal();
            LED_display();
            general_deal();
            EEP_deal();
            IO_operate();
            Sys_Control();
            SYS_Inspect_Deal();
            tuya_deal();
            communication_Deal();
            CLEAR_SYS_TIME_BIT();
        }
    }
}
