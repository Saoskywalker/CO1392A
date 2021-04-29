#ifndef __KEYFUNC_API_H
#define __KEYFUNC_API_H

#define COOL_C 0
#define COOL_F 2
#define HEAT_C 1
#define HEAT_F 3

#define      Set_Time          6

extern const UI08 TempSet_range[4][2];

//按键定义
#define power_key   6
#define fan_key     8
#define down_key    4
#define up_key      5
#define timer_key   2
#define lamp_key    10   
#define func_key    3
#define SWING_key   7
#define uvc_key     1

#define self_test_key 10
#define _c_f        (24)    //!同时按下上下键，摄氏度和华氏度转换
#define EC_Fast_key    30
#define filter_key     40
#define OFF_CYCLE_KEY  50


extern UI08 Disp_Delay;     //!显示延时
extern xdata UUI08  key_bit;
#define    _Key_Ok           key_bit.bit_.b0

void key_decode(void);
extern void Turn_On(void);
extern void Turn_Off(void);
extern void prg_s_key(void);
extern void key_deal(void);
extern void set_heat_temp_chack(void);
void set_cool_temp_chack(void);
#endif

