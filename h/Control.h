#ifndef __Control_H
#define __Control_H

#include "DataType.h"

//=================��Ҫ���ñ���==================================================
#define Comp_Protect_Time 180 //ѹ��������ʱ��
//===========��˪����
#define defrost_para1 (10 * 60)  //ѹ������תX���Ӻ�ʼ��ʱ
#define defrost_para2 135        // 0�ȶ�ӦADֵ  (10/(10+27.9))*1024-1=269
#define defrost_para3 157        // 5�ȶ�ӦADֵ  (10/(10+22.47))*1024-1=314
#define defrost_para4 (10 * 60)  //��ʱ������X�����жϹ���
#define defrost_para5 (19 * 60)  //��ʱ������Y�����жϹ���
#define defrost_para6 (20 * 60)  //��˪��ʼʱ���ʱ
#define defrost_para7 (10)       //��˪ʱ�����¶��ж�  10��
#define defrost_para8 (7 * 60)   //��˪ʱ��1
#define defrost_para9 (7 * 60)   //��˪ʱ��2
#define defrost_para10 (4)       //��X�γ�˪����˪ʱ���ӳ�
#define defrost_para11 (15 * 60) //��X�γ�˪����˪ʱ���ӳ���Y����
#define defrost_para12 (15 + 9)  //ͭ�ܹ���ʱ���������ж� <15
//==================����ӿ�===============================================================
typedef struct
{
  UI16 HI_stay_time;           //ʪ�ȳ������е�ʱ��
  UI16 LO_stay_time;           //��ʪ�����е�ʱ��
  UI16 def_start_time;         //��˪��ʱ
  UI08 def_start_count_enable; //��˪��ʱʹ��
  UI16 def_time;               //��˪��ʱ
  UI08 def_cont;               //��˪����
  UI16 temp_room_point;        //��˪�¶ȵ�
  DEF_TYPE def_type;           //��˪��ʽ(A/B)
  bool_f sensor_err;           // sensor����
} RUN_REG;
extern MCU_xdata RUN_REG Run_Reg;

typedef struct
{
  UI16 on_timer;     //ˮ�ô�ʱ��
  UI16 off_timer;    //�ر�ʱ��
  UI08 on_count;     //�򿪴���
  UI08 mode;         //״̬
  UI08 SW_ON_Timer;  //��ˮ�������ʱ��
  UI08 SW_OFF_Timer; //��ˮ���ر�ʱ��
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
extern MCU_xdata FuncState Pump_water_pipe_Status; //��ˮˮ���Ƿ����

extern MCU_xdata FuncState Fan_set_enable;
extern MCU_xdata UI16 Fan_Speed_delay;

extern MCU_xdata UI08 Feel_Mode;
// extern MCU_xdata UI08 Dump_SW;//��̨�㵹����״̬
// extern MCU_xdata UI08 Dump_Status;//��̨�㵹���

extern MCU_xdata bool_f sys_reg__comp_protect_stause_condition_a; //��ýй©����2������aѹ��������
extern MCU_xdata bool_f sys_reg__comp_protect_stause_condition_b; //��ýй©����2������bѹ��������
extern MCU_xdata UI16 sys_reg_comp_38_count_condition_a;          //��ýй©����2������aʱ�����
extern MCU_xdata UI16 sys_reg_comp_38_count_condition_b;          //��ýй©����2������bʱ�����
extern MCU_xdata UI16 g_ec_count_timer;
extern MCU_xdata bool_f sys_reg__ec_err;

extern MCU_xdata sEC_struct sEC_SYS;
#define _EC_err sEC_SYS.EC_protect_byte.bit_.b0       // EC����
#define _EC_Fast_Test sEC_SYS.EC_protect_byte.bit_.b1 //���

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
