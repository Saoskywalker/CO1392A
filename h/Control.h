//**********************************************************
//文件名称: 	Control.c
//文件描述: 	系统逻辑控制函数(威技冷气机)
//项目名称: 	COR748
//构	建: 	GCE 唐永强		2011/07/15
//修	改: 	GCE XXX 		2011/07/15
//*********************************************************/

#ifndef __Control_H
#define __Control_H


#define DCPUMP_ON     P45=1
#define DCPUMP_OFF    P45=0

//=================跟外部程序接口用到的RAM宏定义=======================
#define mode_set 		Mode_Set
#define _defrost 		_Defrost_status
#define fan_set         	Fan_Set
#define outfan_speed 		Outfan_Speed
#define temp_set_f 		Temp_Set_F
#define _fan_set_enb 		_Fan_set_enable
#define power_status 		Power_Status
#define temp_room_status   	Temp_room.status
#define temp_coil_status   	Temp_coil.status
#define Troom_value     Temp_room.value
#define Tcoil_value     Temp_coil.value
//=================需要配置变量==================================================
#define Comp_Protect_Time  180  //压缩机保护时间
//=========防冻结参数
#define defreeze_Troom1   72//22C
#define defreeze_Tcoil1   43//6C
#define defreeze_Troom2   64//18C
#define defreeze_Tcoil2   36//2C
#define defreeze2_runtime_const 15*60
//==========防冷风参数
#define coolair_point1 86//30C(30+15)*4
#define coolair_point2 77//25C(25+15)*4
#define coolair_point3 68//20C(20+15)*4
//==========过载保护参数
#define overload_point1 131//55C(55+15)*4
#define overload_point2 140//60C(60+15)*4
#define overload_point3 149//65C(65+15)*4
//===========除霜参数
#define defrost_para1   20*60   //条件1 压缩机运行时间
#define defrost_para2   18    //条件1 温度点 10C
#define defrost_para3   20*60   //条件2 压缩机运行时间
#define defrost_para4   81//27C(27+15)*4   //条件2 温度点
#define defrost_para5   2*60   //条件2 计时
#define defrost_para6   15*60   //条件3 压缩机运行时间
#define defrost_para7   68//20C(20+15)*4   //条件3 温度点
#define defrost_para8   3*60       //除霜时间

//=========除湿压缩机开停时间参数
#define comp_on_target_time_define  360,180,180     //6,3,3
#define comp_off_target_time_define 180,360,540     //3,6,9

//===================PUMP相关定义=================================
#define  	SPEED_BUF_MAX  6		//数组带下
#define  	INFAN_IO       (P45)		//IO口
#define 	PUMP_PG        (15*10)		//有反馈的打水泵,满水恢复后继续打水15S(15*10=15S)
#define 	PUMP_NO_PG     (32*10)		//无反馈的打水泵,满水恢复后继续打水35S(35*10=35S)  ==>实测35S
//==================对外接口===============================================================
extern  xdata UUI08   Control_bit;
extern  xdata UI08    Room_Temp;
extern  xdata UI08   R_overload_status;
extern xdata UI16  	      off_cycle_Tmin;         	//off cycle参数T
extern xdata UI16  	      off_cycle_Tmin_Dsip_timer;	//off cycle参数T显示时间
extern xdata UI08  	      PowerKey_off_cycle_Time;	//参数允许设置时间
extern xdata UI08             off_cycle_mode;
extern xdata UI16             off_cycle_timer;

#define   _comp_firston_in_heat          Control_bit.bit_.b0
#define   _high_temp_protect_flag        Control_bit.bit_.b1
#define   _heat_temp_compensation        Control_bit.bit_.b2
#define   _heat_temp_compensation_DSP_EN Control_bit.bit_.b3

extern xdata UI08   _Flash_for_high_temp;

#define WATER_FULL_TIME    Judge_water_Full_delay_time

extern  xdata UUI08 DcPump_bit;
#define  _water_pipe_Status      DcPump_bit.bit_.b0  //排水管 0/1 ==> 未插入/插入
#define  _Pump_Water_PG_Status   DcPump_bit.bit_.b1  //判断是否为带反馈的打水马达(带反馈的打水猛一点,打水时间就短一点)
#define  _water_pipe_Status_old  DcPump_bit.bit_.b2  //判断是否为带反馈的打水马达(带反馈的打水猛一点,打水时间就短一点)


extern xdata sEC_struct sEC_SYS;
#define _EC_err        sEC_SYS.EC_protect_byte.bit_.b0
#define _EC_protect    sEC_SYS.EC_protect_byte.bit_.b1
#define _EC_Fast_Test  sEC_SYS.EC_protect_byte.bit_.b2

#define ABSOLUTE_VALUE(a,b) ((a>b)?(a-b):(b-a))


extern void prg_s_control(void);
extern void Sys_Control(void);
void control_data_intit(void);
extern void   off_cycle_mode_control(void);
extern void Clear_off_cycle_mode_Data(void);
extern void Get_the_pump_pwm_width(void);

#endif

