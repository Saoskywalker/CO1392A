#ifndef __GENERAL_FUNC_H
#define __GENERAL_FUNC_H


#define   TYPE_MAX      21     //存储数据延时
#define   EEP_MAX       16     //存储数据大小
extern code  UI16 mach_type_tab[TYPE_MAX];
extern code  UI08 Off_cycle_Timer_tab[TYPE_MAX];
#define   Comp_Protect_Time  180  //压缩机保护时间  180
#define   cool_light_down_time 3600//
#define   heat_light_down_time 300

#define   TEMP_SET_TIME     10

//系统模式设置变量
extern xdata MODE_TYPE       Mode_Set;
extern xdata MODE_TYPE       Mode_Buf;
extern xdata UI08           Mode_Delay_Timer;
/////////////////////////////////////////////////////////////
extern xdata FANSPEED_TYPE    Fan_Set;
extern xdata FANSPEED_TYPE    Fan_Buf;
extern xdata FANSPEED_TYPE    Outfan_Speed;
extern xdata FANSPEED_TYPE    fan_speed;
extern xdata UI08             Fan_Delay_Timer;
extern xdata UI08             Fan_Delay_disp;
/////////////////////////////////////////////////////////////
//设置温度
extern xdata UI08  Temp_Set_C;
extern xdata UI08  Temp_Set_F;
extern xdata UI08  Temp_Cool_F;
extern xdata UI08  Temp_Heat_F;
extern xdata UI08  Temp_Setting_Time;
/////////////////////////////////////////////////////////////
//设置定时时间
extern xdata UI32        Timer_run;            //定时时间 秒为单位
extern xdata UI08        Timer_buff;           //00-24小时 时间设定临时变量
extern xdata UI08        Timer_Setting_Time;   //设定数码管闪烁显示时间/可修改参数时间
extern xdata UI08        Timer_set;            //00-24小时 时间设定
extern xdata TIMER_TYPE  Timer_Type;
/////////////////////////////////////////////////////////////
extern xdata UI08   Key_Number;
extern xdata UI16   Key_last;     //按键持续时间变量
/////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//系统公用
extern xdata ONOFF_STATUS     Power_Status;
extern xdata ONOFF_STATUS     rec_buf_power2;
extern xdata UI08             write_delay_time;
extern xdata UI08             Power_Delay_Time;      //系统开机延时

extern xdata UI16             compensate_delaytime;
extern xdata UI16             light_down_cont;//风机停后显示半亮时间  2016/3/29

extern xdata UI08             sleep_Time;
extern xdata UI08             DE_rec_time;
extern xdata ONOFF_STATUS     LAMP_Status;
extern xdata UI08             minute_cont;
extern xdata UI16             Sys_filter_time;	        //室内马达运转时间
extern xdata ONOFF_STATUS     SYS_UVC_Status;     //UVC状态

extern xdata UUI08            SYS_bit;
#define     _temp_room_err   SYS_bit.bit_.b0        //室温错误标志
#define     _temp_coil_err   SYS_bit.bit_.b1        //铜管温度错误标志
#define     _sys_err         SYS_bit.bit_.b2        //系统错误标志
#define     _Flash_500ms     SYS_bit.bit_.b3        //500ms闪烁标志
#define     _DISP_En         SYS_bit.bit_.b4
#define     _DISP_En2        SYS_bit.bit_.b5
#define     _DISP_LED_filter SYS_bit.bit_.b6
#define     _Self_Test_EEp   SYS_bit.bit_.b7


extern xdata UUI08            SYS_bit1;
#define     _Defrost_status  SYS_bit1.bit_.b0
#define     _Write_EEP_EN    SYS_bit1.bit_.b1       //允许写EEPROM标志
#define     _check_EEP_EN    SYS_bit1.bit_.b2
#define     _Troom_dsp_com_EN  SYS_bit1.bit_.b3
#define     _pump_sw_status  SYS_bit1.bit_.b4
#define     _pump_sw_buf     SYS_bit1.bit_.b5
#define     _pump_sw_out     SYS_bit1.bit_.b6
#define     _first_ad        SYS_bit1.bit_.b7

extern xdata UUI08            SYS_bit2;
#define     _Water_Full         SYS_bit2.bit_.b0
#define     _Water_Full_status   SYS_bit2.bit_.b1
#define     _outfan_low_enable  SYS_bit2.bit_.b2
#define     _compensate_enable  SYS_bit2.bit_.b3
#define     _Tcoil_Enable       SYS_bit2.bit_.b4
#define     _Sys_lowpower       SYS_bit2.bit_.b5
#define     _Auto_Restart       SYS_bit2.bit_.b6
#define     _temp_update_enb    SYS_bit2.bit_.b7

extern xdata UUI08            SYS_bit3;
#define     _comp_should_on     SYS_bit3.bit_.b0
#define     _Mode_Change        SYS_bit3.bit_.b1
#define     _Timer_set_Flag     SYS_bit3.bit_.b2
#define     _Self_Test          SYS_bit3.bit_.b3
#define     _Fan_set_enable     SYS_bit3.bit_.b4
#define     _get_key_enable     SYS_bit3.bit_.b5
#define     _led_swing_enable   SYS_bit3.bit_.b6
#define     _led_swing_disp_buf SYS_bit3.bit_.b7

extern xdata UUI08            SYS_bit4;
//#define     _led_swing_disp     SYS_bit4.bit.b0
#define     _led_swing_out      SYS_bit4.bit_.b1
#define     _IICSleep_status    SYS_bit4.bit_.b2
#define     _ADC_END_OK         SYS_bit4.bit_.b3
#define     _ad_start           SYS_bit4.bit_.b4
#define     _SYS_Inspect_Key_OK SYS_bit4.bit_.b5
#define     _READ_KEY_OK        SYS_bit4.bit_.b6

extern xdata UUI08            SYS_bit5;
#define   Cool_compensate_status   SYS_bit5.bit_.b0
#define   Trom_HE_88               SYS_bit5.bit_.b1
#define   Trom_HE_79               SYS_bit5.bit_.b3
#define   _compensate_disable      SYS_bit5.bit_.b4
#define   _compensate_80_87        SYS_bit5.bit_.b5
#define   _test_LUX_ok             SYS_bit5.bit_.b6
#define   _first_read_IO           SYS_bit5.bit_.b7

extern xdata UUI08            SYS_bit6;
#define   _long_key_en           SYS_bit6.bit_.b0

/////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////





extern xdata FAN_TYPE         sys_fan_type;
extern xdata CF_TYPE          CF_Status;         //

extern xdata FORM_TYPE        Sys_Tpye;          //确定多少种运行模式变量
extern xdata FAN_ORDER_TYPE   Sys_fan_order;

extern xdata UUI08            light_down;
extern xdata UI08             water_light_cont;


#define light_down1 light_down.bit_.b0//待机显示--
#define light_down2 light_down.bit_.b1//故障
#define light_down3 light_down.bit_.b2//定时关
#define light_down4 light_down.bit_.b3//压机关


/////////////////////////////////////////////////////////////

#define   DISP_TIMER    10     //定时、温度设置等待多久，自动确认
#define   INIT_TIMER    8      //定时开关机初始时间
#define   FAN_DELAY     2      //风速切换延时
#define   MODE_DELAY    2      //模式切换延时
#define   EEP_DELAY     2      //存储数据延时





extern xdata UI08 Sys_EEP_SYTP;//机型
extern xdata UI08 Sys_Version_buf;
extern xdata UI08 Buzz_Cnt;




#define   Soft_Version 0




extern void WdtInit(void);
extern void Sys_Initial(void);
extern void SysData_init(void);
extern void SYS_Mach_type_judge(void);
extern UI08 F_C(UI08 cfdata);
extern UI08 C_F(UI08 cfdata);
extern void delay(UI16 i);
extern void EEP_deal(void);
extern void prg_s_general(void);
extern void Sys_data_read_eep(void);
extern void Sys_data_write(void);
extern void general_deal(void);
extern void Sys_filter_deal(void);
extern void prg_minute_control(void);
extern void prg_minute(void);













#endif

