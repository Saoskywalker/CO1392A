//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: main.c
//	作者		: Yanni
//	模块功能	: SC91F8311触控DEMO
//  最后更正日期:
// 	版本		: 2015-12-25:
//              :
//*************************************************************
#include "General.h"
//#include "SOC_DebugTouchKey.h"
//*****************全局变量区***************************************************
void main(void)
{
    Sys_Initial();
    //SOCAPI_DeBugTouchKey_Init();//动态库调试
    //上电全显
    while(Power_Delay_Time>0)
    {
        //显示温度更新
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
    //EEP 机型读取
    SYS_Mach_type_judge();
    Sys_data_read_eep();
    //显示温度更新
    Troom_update_time=3;
    _temp_update_enb=1;
    Troom_dsp_com=Temp_room.value;

    IAPPageErase(SYS_DATA_ADDR,IapROM);
    EEP_OffSet_DATA_ADDR=0;//擦除后 偏移量从0开始
    _Write_EEP_EN = 1;
    Sys_data_write();
    //强制测试入口
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
