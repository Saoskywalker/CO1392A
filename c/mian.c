//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: main.c
//	作者		: Yanni
//	模块功能	: SC91F8311触控DEMO
//  最后更正日期:
// 	版本		: 2015-12-25:
//              :
//*************************************************************
// #define TOUCH_DYN_DEBUG     //!动态调试触摸，需添加SC95F8X1X_HighSensitiveTKDynamicDebug_S_V0.0.2.LIB
#include "General.h"

#ifdef TOUCH_DYN_DEBUG
#include "SOC_DebugTouchKey.h"
#endif

//*****************全局变量区***************************************************
void main(void)
{
    Sys_Initial();
#ifdef TOUCH_DYN_DEBUG
    SOCAPI_DeBugTouchKey_Init(); //动态库调试
#endif
    //上电全显
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
    // EEP 机型读取
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
