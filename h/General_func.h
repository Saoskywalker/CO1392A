#ifndef __General_func_H
#define __General_func_H

#include "DataType.h"

extern MCU_xdata UUI08 Sys_eep_bit;
//#define  _check_EEP_EN   Sys_eep_bit.bit_.b0//检验使能
//#define  _Write_EEP_EN   Sys_eep_bit.bit_.b1//
//#define  _MS100_EEP_EN   Sys_eep_bit.bit_.b2//

extern MCU_xdata UI32 M_Timer_Run;   //定时剩余时间
extern MCU_xdata UI08 M_Timer_Set;   //定时设定时间
extern MCU_xdata UI08 M_Timer_Buf;   //定时设定时间buf
extern MCU_xdata UI08 M_Timer_Point; //定时 point
// extern MCU_xdata UI16  M_Power_Delay_Time;//上电延时
extern MCU_xdata UI16 M_Power_Delay_Time2; //上电延时(进入自检)

// extern MCU_xdata UI08  M_Hum_Set;//湿度设定
// extern MCU_xdata UI08  M_Hum_Set_buf;//湿度设定buf
// extern MCU_xdata UI08  M_Dry_Status;//干衣状态
// extern MCU_xdata UI08  M_Swing_Status;//摆叶状态
// extern MCU_xdata UI08  M_continuous;  //连续模式

// extern MCU_xdata UI08 M_Hum_Setting_Time;//湿度设定(确认时间)
extern MCU_xdata UI08 M_Timer_Setting_Time; //定时设定(确认时间)
extern MCU_xdata UI08 M_Dsp_Time;           //显示时间
extern MCU_xdata UI08 Hum_dsp_com;          //显示湿度
extern MCU_xdata UI08 M_hum_update_time;    //湿度更新时间

extern MCU_xdata UI08 M_test_seq;   //自检
extern MCU_xdata UI08 M_test_cont1; //自检
extern MCU_xdata UI08 M_test_cont2; //自检
extern MCU_xdata SYS_ERR Sys_Err;   //故障
extern MCU_xdata UUI08 M_sys_bit;

extern MCU_xdata ONOFF_STATUS SYS_Power_Status; //开关机状态

extern MCU_xdata SYS_MODE SYS_Mode;       //系统模式
extern MCU_xdata SYS_MODE SYS_Mode_Buf;   //系统模式
extern MCU_xdata UI08 Set_SYS_Mode_Timer; //模式设定时间

extern MCU_xdata DYR_TYPE SYS_DYR_Tyde;     //干衣类型
extern MCU_xdata DYR_TYPE SYS_DYR_Tyde_Buf; //干衣类型
extern MCU_xdata UI08 Set_SYS_DYR_Tyde_timer;

extern MCU_xdata HUM_TYPE SYS_HUN_Tyde;       //除湿类型
extern MCU_xdata HUM_TYPE SYS_HUN_Tyde_Buf;   //除湿类型
extern MCU_xdata UI08 SET_SYS_HUN_Tyde_Timer; //湿度设定(确认时间)

extern MCU_xdata UI08 SYS_Hum_Set;     //湿度设定
extern MCU_xdata UI08 SYS_Hum_Set_Buf; //湿度设定buf
extern MCU_xdata UI08 Set_SYS_Hum_timer;

extern MCU_xdata FANSPEED_TYPE SYS_Fan_Tyde;      //运行风速
extern MCU_xdata FANSPEED_TYPE SYS_Fan_Tyde_Buf;  //运行风速
extern MCU_xdata FANSPEED_TYPE Fan_Speed_Out;     //实际运行风速
extern MCU_xdata FANSPEED_TYPE Fan_Speed_Out_Buf; //实际运行风速
extern MCU_xdata FuncState Set_Fan_State;         //是否允许风速
extern MCU_xdata UI08 Set_SYS_Fan_Tyde_timer;
extern MCU_xdata UI16 FAN_ON_timer;
extern MCU_xdata UI16 FAN_OFF_timer;

extern MCU_xdata SWING_MODE SYS_Swing_Tyde;     //摆叶类型
extern MCU_xdata SWING_MODE SYS_Swing_Tyde_Buf; //摆叶类型
extern MCU_xdata SWING_MODE Swing_Tyde_Out;     //实际摆叶方式
extern MCU_xdata SWING_MODE Swing_Tyde_Out_Buf; //实际摆叶方式
extern MCU_xdata UI08 Set_SYS_Swing_Tyde_Timer;
extern MCU_xdata FuncState Set_Swing_State; //是否允许设置摆叶

extern MCU_xdata AIR_CLEAR_MODE SYS_Air_Clear_Tyde;     //空气净化类型
extern MCU_xdata AIR_CLEAR_MODE SYS_Air_Clear_Tyde_Buf; //空气净化类型
extern MCU_xdata UI08 Set_SYS_Air_Clear_Tyde_Timer;

extern MCU_xdata FuncState LED_Disp_Status;

extern MCU_xdata UI16 SYS_filter_time;     //滤网时间
extern MCU_xdata UI16 SYS_filter_time_buf; //滤网时间

extern MCU_xdata FuncState Buzzer_Status;
extern UI16 Buzz_Time;          //蜂鸣器时间
extern MCU_xdata UI08 Buzz_Cnt; //蜂鸣器响次数
extern MCU_xdata UI08 Dump_Buzz_Cnt;
extern MCU_xdata PinStatus Buzzer_IO_Status;

#define M__Flash_500ms M_sys_bit.bit_.b0        // 500ms闪烁
#define M__Fan_set_enable M_sys_bit.bit_.b1     //风机设定使能
#define M_Defrost_status M_sys_bit.bit_.b2      //除霜
#define M__Self_Test M_sys_bit.bit_.b3          //自检
#define touch_version_read_ok M_sys_bit.bit_.b4 //触摸芯片读取版本OK?
#define M_ADC_check M_sys_bit.bit_.b5           // ADC处理  接收到数据后处理

extern MCU_xdata UUI16 M_sys_bit1;
#define _Fast_Test M_sys_bit1.bit_.b0      //快测
#define _Timer_set_ok M_sys_bit1.bit_.b3   //定时设定
#define _SYS_UVC_Status M_sys_bit1.bit_.b4 // UVC灯

extern void Turn_On(void);
extern void Turn_Off(void);
extern void prg_s_general(void);
extern void prg_ms200_test(void);
extern void Sys_Initial(void); //	系统 初始化程序
// extern void delay(UI16 i);
// extern UI08 F_C(UI08 cfdata);
// extern UI08  C_F(UI08 cfdata);
// extern void Sys_data_write(void);
// extern void Sys_data_read(void);
// extern void Mach_type_judge(void);

extern void SYSData_Rest(void);
extern void general_deal(void);
extern void SYS_data_init(void);

#endif
