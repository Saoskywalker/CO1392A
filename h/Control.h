#ifndef __Control_H
#define __Control_H

#include "DataType.h"

//=================需要配置变量==================================================
#define Comp_Protect_Time 180 //压缩机保护时间
//===========除霜参数
#define defrost_para1 (10 * 60)  //压缩机运转X分钟后开始计时
#define defrost_para2 135        // 0度对应AD值  (10/(10+27.9))*1024-1=269
#define defrost_para3 157        // 5度对应AD值  (10/(10+22.47))*1024-1=314
#define defrost_para4 (10 * 60)  //计时过程中X分钟判断管温
#define defrost_para5 (19 * 60)  //计时过程中Y分钟判断管温
#define defrost_para6 (20 * 60)  //除霜开始时间计时
#define defrost_para7 (10)       //除霜时环境温度判断  10°
#define defrost_para8 (7 * 60)   //除霜时间1
#define defrost_para9 (7 * 60)   //除霜时间2
#define defrost_para10 (4)       //第X次除霜，除霜时间延长
#define defrost_para11 (15 * 60) //第X次除霜，除霜时间延长至Y分钟
#define defrost_para12 (15 + 9)  //铜管故障时，以室温判断 <15
//==================对外接口===============================================================
typedef struct
{
  UI16 HI_stay_time;           //湿度超高运行的时间
  UI16 LO_stay_time;           //低湿度运行的时间
  UI16 def_start_time;         //除霜计时
  UI08 def_start_count_enable; //除霜计时使能
  UI16 def_time;               //除霜计时
  UI08 def_cont;               //除霜次数
  UI16 temp_room_point;        //除霜温度点
  DEF_TYPE def_type;           //除霜方式(A/B)
  bool_f sensor_err;           // sensor故障
} RUN_REG;
extern MCU_xdata RUN_REG Run_Reg;

typedef struct
{
  UI16 on_timer;     //水泵打开时间
  UI16 off_timer;    //关闭时间
  UI08 on_count;     //打开次数
  UI08 mode;         //状态
  UI08 SW_ON_Timer;  //排水马达运行时间
  UI08 SW_OFF_Timer; //排水马达关闭时间
  UI08 off_delay;
} Pump_str;
extern MCU_xdata Pump_str Pump_type;

#define Pump_mode_0 0
#define Pump_mode_1 1
#define Pump_mode_2 2
#define Pump_mode_3 3

#define Temp_room_para_C Temp_room.C_value
#define Temp_coil_para_C Temp_coil.C_value

extern MCU_xdata UI16 fan_force_runtime;
extern MCU_xdata UI16 SYS_RUN_timer;

extern MCU_xdata FuncState test_factory;
extern MCU_xdata FuncState Child_Lock_status;
extern MCU_xdata UI08 Child_Lock_Disp_Count;
extern MCU_xdata UI08 Child_Lock_1s_Count;
extern MCU_xdata UI16 Child_Lock_Disp_timer;

extern MCU_xdata UI08 Key_ERR_Buzz_Cnt;

extern MCU_xdata FuncState Pump_Status;
extern MCU_xdata FuncState Pump_water_pipe_Status; //抽水水管是否介入

extern MCU_xdata FuncState Fan_set_enable;
extern MCU_xdata UI16 Fan_Speed_delay;

extern MCU_xdata UI08 Feel_Mode;
// extern MCU_xdata UI08 Dump_SW;//机台倾倒开关状态
// extern MCU_xdata UI08 Dump_Status;//机台倾倒情况

extern MCU_xdata bool_f sys_reg__comp_protect_stause_condition_a; //冷媒泄漏条件2中条件a压缩机保护
extern MCU_xdata bool_f sys_reg__comp_protect_stause_condition_b; //冷媒泄漏条件2中条件b压缩机保护
extern MCU_xdata UI16 sys_reg_comp_38_count_condition_a;          //冷媒泄漏条件2中条件a时间计数
extern MCU_xdata UI16 sys_reg_comp_38_count_condition_b;          //冷媒泄漏条件2中条件b时间计数
extern MCU_xdata UI16 g_ec_count_timer;
extern MCU_xdata bool_f sys_reg__ec_err;

extern MCU_xdata sEC_struct sEC_SYS;
#define _EC_err sEC_SYS.EC_protect_byte.bit_.b0       // EC故障
#define _EC_Fast_Test sEC_SYS.EC_protect_byte.bit_.b1 //快测

///
///
#define ABSOLUTE_VALUE(a, b) ((a > b) ? (a - b) : (b - a))

extern void Pump_S_general(void);
extern void prg_s_control(void);
extern void prg_minute_control(void);
extern void prg_hour_conl(void);
extern void Sys_Control(void);
extern void prg_200ms_control(void);
extern void load_set_self_test(void);
extern void Run_reg_init(void);
extern void BUZZ_CONTROL(void);
extern void Run_reg_init(void);
#endif

//////////////////////////////////////////////////////////
//
//      eeeeeeeeee       n           nn      ddddddddd
//     ee               nnn         nn      dd       dd
//    ee               nn   n      nn      dd        dd
//   eeeeeeeeee       nn     n    nn      dd         dd
//  ee               nn       n  nn      dd         dd
// ee               nn         nnn      dd         dd
// eeeeeeeeee       nn          nn      ddddddddddd
//
// Designed by Benkye.Zhang 2010.10.28
///////////////////////////////////////////////////////
