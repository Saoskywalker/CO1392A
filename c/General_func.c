#include "General.h"

MCU_xdata UUI08 Sys_eep_bit = {0}; // EEP处理标志位

MCU_xdata UI32 M_Timer_Run = 0;   //定时剩余时间
MCU_xdata UI08 M_Timer_Set = 1;   //定时设定时间
MCU_xdata UI08 M_Timer_Buf = 1;   //定时设定时间buf
MCU_xdata UI08 M_Timer_Point = 0; //定时 point
// MCU_xdata UI16  M_Power_Delay_Time=300;//上电延时
MCU_xdata UI16 M_Power_Delay_Time2 = 300; //上电延时(进入自检)

MCU_xdata UI08 M_Timer_Setting_Time = 0; //定时设定(确认时间)
MCU_xdata UI08 M_Dsp_Time = 0;           //显示时间
MCU_xdata UI08 Hum_dsp_com = 60;         //环境湿度值显示
MCU_xdata UI08 Hum_dsp_state = 60;         //环境湿度状态显示

MCU_xdata UI08 M_test_seq = 0;    //自检
MCU_xdata UI08 M_test_cont1 = 0;  //自检
MCU_xdata UI08 M_test_cont2 = 0;  //自检
MCU_xdata UUI08 M_sys_bit = {0};  //系统标志位
MCU_xdata UUI16 M_sys_bit1 = {0}; //系统标志位1
MCU_xdata SYS_ERR Sys_Err;        //故障

//
MCU_xdata ONOFF_STATUS SYS_Power_Status = OFF; //开关机状态

MCU_xdata SYS_MODE SYS_Mode = mode_SYS_HUM;     //系统模式
MCU_xdata SYS_MODE SYS_Mode_Buf = mode_SYS_HUM; //系统模式
MCU_xdata UI08 Set_SYS_Mode_Timer = 0;          //模式设定时间
UI08 G_Disp_SA_Time = 0; //SA保护显示时间

MCU_xdata DYR_TYPE SYS_DYR_Tyde = DYR_STRONG;     //干衣类型
MCU_xdata DYR_TYPE SYS_DYR_Tyde_Buf = DYR_STRONG; //干衣类型
MCU_xdata UI08 Set_SYS_DYR_Tyde_timer = 0;

MCU_xdata HUM_TYPE SYS_HUN_Tyde = USER_DEFINE_HUM;     //除湿类型
MCU_xdata HUM_TYPE SYS_HUN_Tyde_Buf = USER_DEFINE_HUM; //除湿类型
MCU_xdata UI08 SET_SYS_HUN_Tyde_Timer = 0;             //湿度设定(确认时间)

MCU_xdata UI08 SYS_Hum_Set = 0;     //湿度设定
MCU_xdata UI08 SYS_Hum_Set_Buf = 0; //湿度设定buf
MCU_xdata UI08 Set_SYS_Hum_timer = 0;

MCU_xdata FANSPEED_TYPE SYS_Fan_Tyde = SILENCE_FAN;     //运行风速
MCU_xdata FANSPEED_TYPE SYS_Fan_Tyde_Buf = SILENCE_FAN; //运行风速
MCU_xdata FANSPEED_TYPE Fan_Speed_Out = OFF_FAN;        //实际运行风速
MCU_xdata FANSPEED_TYPE Fan_Speed_Out_Buf = OFF_FAN;    //实际运行风速
MCU_xdata FuncState Set_Fan_State = Enable;             //是否允许风速
MCU_xdata UI08 Set_SYS_Fan_Tyde_timer = 0;
MCU_xdata UI16 FAN_ON_timer = 0;
MCU_xdata UI16 FAN_OFF_timer = 0;

MCU_xdata SWING_MODE SYS_Swing_Tyde = STOP_SWING;     //摆叶类型
MCU_xdata SWING_MODE SYS_Swing_Tyde_Buf = STOP_SWING; //摆叶类型
MCU_xdata SWING_MODE Swing_Tyde_Out = STOP_SWING;     //实际摆叶方式
MCU_xdata SWING_MODE Swing_Tyde_Out_Buf = STOP_SWING; //实际摆叶方式
MCU_xdata UI08 Set_SYS_Swing_Tyde_Timer = 0;
MCU_xdata FuncState Set_Swing_State = Disable; //是否允许设置摆叶

MCU_xdata AIR_CLEAR_MODE SYS_Air_Clear_Tyde = AUTO_AIR_CLEAR;     //空气净化类型
MCU_xdata AIR_CLEAR_MODE SYS_Air_Clear_Tyde_Buf = AUTO_AIR_CLEAR; //空气净化类型
MCU_xdata UI08 Set_SYS_Air_Clear_Tyde_Timer = 0;

MCU_xdata FuncState LED_Disp_Status = ENABLE; //灯光是否打开

MCU_xdata FuncState Buzzer_Status = ENABLE; //蜂鸣器是否使能

MCU_xdata UI16 SYS_filter_time = 0;     //滤网时间
MCU_xdata UI16 SYS_filter_time_buf = 0; //滤网时间

UI16 Buzz_Time = 0;               //蜂鸣器时间
MCU_xdata UI08 Buzz_Cnt = 0;      //蜂鸣器响次数
MCU_xdata UI08 Dump_Buzz_Cnt = 0; //倾倒蜂鸣器响次数
MCU_xdata PinStatus Buzzer_IO_Status = RESET;

/*************************************************
 // 函数名称    : SYSData_Rest
 // 功能描述    : 变量初始化
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void SYSData_Rest(void)
{
        _Fast_Test = 0;

        M_Timer_Run = 0;
        M_Timer_Set = 1;
        M_Timer_Buf = 1;

        Pump_Status = DISABLE;

        SYS_Mode = mode_SYS_HUM;     //系统模式
        SYS_Mode_Buf = mode_SYS_HUM; //系统模式
        Set_SYS_Mode_Timer = 0;      //模式设定时间

        SYS_DYR_Tyde = DYR_AUTO;     //干衣类型
        SYS_DYR_Tyde_Buf = DYR_AUTO; //干衣类型
        Set_SYS_DYR_Tyde_timer = 0;

        SYS_HUN_Tyde = CONTINUOUS_HUM;     //除湿类型
        SYS_HUN_Tyde_Buf = CONTINUOUS_HUM; //除湿类型

        SYS_Hum_Set = 0;            //湿度设定
        SYS_Hum_Set_Buf = 0;        //湿度设定buf
        SET_SYS_HUN_Tyde_Timer = 0; //湿度设定(确认时间)

        SYS_Fan_Tyde = SILENCE_FAN;     //运行风速
        SYS_Fan_Tyde_Buf = SILENCE_FAN; //运行风速
        Set_Fan_State = ENABLE;
        Set_SYS_Fan_Tyde_timer = 0;

        SYS_Swing_Tyde = STOP_SWING;     //摆叶类型
        SYS_Swing_Tyde_Buf = STOP_SWING; //摆叶类型
        Set_Swing_State = Disable;
        Set_SYS_Swing_Tyde_Timer = 0;

        SYS_Air_Clear_Tyde = AUTO_AIR_CLEAR;     //空气净化类型
        SYS_Air_Clear_Tyde_Buf = AUTO_AIR_CLEAR; //空气净化类型
        Set_SYS_Air_Clear_Tyde_Timer = 0;

        Buzzer_Status = ENABLE;

        LED_Disp_Status = ENABLE;

        M_Defrost_status = 0;

        SYS_filter_time = 0;

        Run_Reg.def_cont = 0;
        Run_Reg.def_time = 0;
        Run_Reg.HI_stay_time = 0;
        Run_Reg.LO_stay_time = 0;
        //   M_write_delay_time=2;
        wifi_net_led_timer = WIFI_NET_TIMER;
}
/*************************************************
 // 函数名称    : SYS_data_init
 // 功能描述    : 变量初始化
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void SYS_data_init(void)
{
        Adc_data_init();
        Run_reg_init();
        // Wifi_UART_Init();
        io_data_init();
        SYSData_Rest();

        M_sys_bit.byte = 0;  //系统标志位
        M_sys_bit1.word = 0; //系统标志位1

        //
        Buzz_Time = BUZZ_long_time;
        Buzz_Cnt = 0;
        Dump_Buzz_Cnt = 0;

        Sys_Err.Water_Full = DISABLE;
        Sys_Err.temp_room_err = DISABLE;
        Sys_Err.temp_coil_err = DISABLE;
        Sys_Err.hum_Sensor_err = DISABLE;
        Sys_Err.comm_err = DISABLE;
        Sys_Err.pm25_Sensor_err = DISABLE; //粉尘故障
        Sys_Err.pan_motor_err = DISABLE;   //风机故障

        Temp_room.status = AI_NORMAL;
        Temp_coil.status = AI_NORMAL;
        Hum_para.status = AI_NORMAL;

        Hum_para.value = 60;
        Temp_room.value = 25 + 9;
        Temp_coil.value = 25 + 9;

        Sys_eep_bit.byte = 0;

        _SYS_UVC_Status = 0;
}
/*************************************************
 // 函数名称    : prg_s_general
 // 功能描述    : 100mS时间片
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
UI08 cont_200ms = 0;
void prg_ms100_general(void)
{
        if (!_100mS_For_SYS)
        {
                return;
        }

        if (++cont_200ms < 4)
        {
                return;
        }
        cont_200ms = 0;

        _Self_Test_wifi_TXD_en = 1;

        if ((M__Self_Test) && (Power_Delay_Time == 0))
        {
                if (M_test_cont1 < 0xff)
                {
                        M_test_cont1++;
                }
                if ((M_test_cont2 < 0xff) && (M_test_cont1 >= 8))
                {
                        M_test_cont2++;
                }
        }
}
/*************************************************
 // 函数名称    : prg_s_general
 // 功能描述    : S时间片
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void prg_s_general(void)
{
        static MCU_xdata UI08 M_hum_update_time1 = 0, M_hum_update_time2 = 0;    //湿度更新时间

        if (!_1S_For_For_SYS)
        {
                return;
        }

        M_hum_update_time1++;
        if ((M_hum_update_time1 >= 3) || (Power_Delay_Time > 0))
        {
                M_hum_update_time1 = 0;
                Hum_dsp_com = Hum_para.value;
        }
        M_hum_update_time2++;
        if (M_hum_update_time2 >= 60 || Power_Delay_Time > 0)
        {
                M_hum_update_time2 = 0;
                Hum_dsp_state = Hum_para.value;
        }

        if (Power_Delay_Time > 0)
        {
                Power_Delay_Time--;
        }

        if (Buzz_Cnt > 0)
        {
                Buzz_Cnt--;
                Buzz_Time = BUZZ_long_time;
        }
        else if (Key_ERR_Buzz_Cnt > 0)
        {
                Key_ERR_Buzz_Cnt--;
                Buzz_Time = BUZZ_short_time;
        }
        else if (Dump_Buzz_Cnt > 0)
        {
                Dump_Buzz_Cnt--;
                Buzz_Time = BUZZ_long_time;
        }

        //倒计时后, 进入休眠
        // if (M_sleep_Time > 0)
        // {
        //         M_sleep_Time--;
        //         if (M_sleep_Time == 0)
        //         {
        //                 _Sleep_status = 1;
        //         }
        // }

        if (communication_timer > 0)
        {
                communication_timer--;
                if (communication_timer == 0)
                {
                        Sys_Err.comm_err = ENABLE;
                }
        }
}
/*************************************************
//名称        :	F_C
//功能        : F转换为C
//入口参数    :	无
//出口参数    :	无
//构	建: 	GCE XXX
//修	改: 	GCE XXX

原始公式 四舍五入
(X-32)*5/9+0.5
优化公式
((X-32)*10+9)/18
**************************************************/
/*
UI08 F_C(UI08 cfdata)
{
        UI08 a=0;
      if(cfdata>32)
      {a=((cfdata-32)*10+9)/18;}
      else
      {a=((32-cfdata)*10+9)/18;}

        return 	a;
}
*/
/*************************************************
//名称        :	C_F
//功能        : C转换为F
//入口参数    :	无
//出口参数    :	无
//构	建: 	GCE XXX
//修	改: 	GCE XXX
原公式为 :  F=C*9/5+32
**************************************************/
/*
UI08 C_F(UI08 cfdata)
{
        UI08 a=0;
        a=((cfdata-9)*18+5)/10+32;
        if (a>89)
        {a=89;}
       return a;
}
*/
/*************************************************
 // 函数名称    : delay
 // 功能描述    : 延时
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
/*
void delay(UI16 i)
{
        while(i)
        {i--;}
}
*/
/*************************************************
 // 函数名称    : Turn_On
 // 功能描述    : 开机函数
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Turn_On(void)
{
        if ((SYS_Power_Status) || (Sys_Err.Water_Full))
        {
                return;
        }

        //
        SYS_Power_Status = ON;
        M_Timer_Run = 0;
        M_Timer_Setting_Time = 0;
        _Timer_set_ok = 0;
        Set_SYS_Hum_timer = 0;
        M_Dsp_Time = 10;

}
/*************************************************
 // 函数名称    : Turn_Off
 // 功能描述    : 关机函数
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Turn_Off(void)
{
        if (SYS_Power_Status == OFF)
        {
                return;
        }
        SYS_Power_Status = OFF;

        //
        M_Timer_Run = 0;
        M_Timer_Setting_Time = 0;
        _Timer_set_ok = 0;
        Set_SYS_Hum_timer = 0;
        M_Dsp_Time = 0;

        G_Comp_Test_EN = 0; //退出压缩机测试
}

/*************************************************
 // 函数名称    : WdtInit
 // 功能描述    : 看门狗初始化
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void WdtInit(void)
{
}
/*************************************************
 // 函数名称    : Sys_Initial
 // 功能描述    : //系统初始化
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Sys_Initial(void)
{
        EA = 0;
        IO_Init();
        PWM_Init();
        SYS_data_init();
        TimerInit();
        TouchKeyInit();
        tuya_init();
        WdtInit();
        EA = 1;
}
/*************************************************
 // 函数名称    : general_deal
 // 功能描述    :
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void general_deal(void)
{
        prg_ms100_general();
        prg_s_general();
        Timer_Deal();
}
