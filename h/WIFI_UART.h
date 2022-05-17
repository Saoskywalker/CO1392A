#ifndef _WIFI_UART_H
#define _WIFI_UART_H

#include "DataType.h"

#define SET_POWER '1'
#define SET_MODE '3'
#define SET_HUM '5'
#define SET_FAN '7'

#define SET_LAMP '1'
#define SET_BUZZER '1'
#define SET_CHILD_LOCK '1'
#define SET_PUMP '1'

#define SET_TIME '2'

#define Star_Process_OK_Num 4
#define Wifi_net_NUM_L 7
#define Wifi_net_NUM_S 1

#define TXD_MAX 512
#define RXD_MAX 512

#define PIID_1 bit0
#define PIID_2 bit1
#define PIID_3 bit2
#define PIID_4 bit3
#define PIID_5 bit4
#define PIID_6 bit5
#define PIID_7 bit6
#define PIID_8 bit7
#define PIID_9 bit8
#define PIID_10 bit9
#define PIID_11 bit10

#define key_power_bit bit0
#define key_Child_Lock_bit bit1
#define key_dry_cloth_bit bit2
//#define  key_dry_in_bit            bit3
#define key_Water_Full_bit bit4
#define key_Fan_set_bit bit5

#define get_prop_num 7 //不能大于16

// SIID1
#define get_properties_SIID1_PIID_EN \
   (PIID_1 |                         \
    PIID_2 |                         \
    PIID_3 |                         \
    PIID_4)
// SIID2
#define get_properties_SIID2_PIID_EN \
   (PIID_1 |                         \
    PIID_2 |                         \
    PIID_3 |                         \
    PIID_4 |                         \
    PIID_5 |                         \
    PIID_6 |                         \
    PIID_7)
// SIID3
#define get_properties_SIID3_PIID_EN \
   (PIID_1 |                         \
    PIID_2 |                         \
    PIID_3 |                         \
    PIID_4 |                         \
    PIID_5 |                         \
    PIID_6 |                         \
    PIID_7 |                         \
    PIID_8 |                         \
    PIID_9 |                         \
    PIID_10)
// SIID4
#define get_properties_SIID4_PIID_EN \
   (PIID_1 |                         \
    PIID_2)
// SIID5
#define get_properties_SIID5_PIID_EN \
   (PIID_1 |                         \
    PIID_2 |                         \
    PIID_3 |                         \
    PIID_4 |                         \
    PIID_5 |                         \
    PIID_6 |                         \
    PIID_7)
// SIID6
#define get_properties_SIID6_PIID_EN \
   (PIID_1)
// SIID7
#define get_properties_SIID7_PIID_EN \
   (PIID_1 |                         \
    PIID_2 |                         \
    PIID_3 |                         \
    PIID_4 |                         \
    PIID_5 |                         \
    PIID_6 |                         \
    PIID_7)
// SIID8
#define get_properties_SIID8_PIID_EN \
   (PIID_1 |                         \
    PIID_2)
// SIID9
#define get_properties_SIID9_PIID_EN \
   (PIID_1 |                         \
    PIID_2)

typedef struct
{
   UI08 Txd_buf[TXD_MAX];
   UI16 Txd_byte;  //
   UI16 Txd_Lenth; //
   FuncState Txd_OK;

   UI08 Rxd_buf[RXD_MAX];
   UI16 Rxd_byte;
   UI08 Rxd_Timerout;
   FuncState Rxd_OK;
} Wifi_UART_str;

extern MCU_xdata Wifi_UART_str SYS_W_Uart;

typedef struct
{
   // ONOFF_STATUS     props_on_off_rec;
   // FunctionalState  props_force_dhum_rec;
   // FunctionalState  props_dry_clothes_rec;
   // SETMODE          props_humid_st_rec;
   // FANSPEED_TYPE    props_fan_st_rec;
   // FunctionalState  props_buzzer_rec;
   // FunctionalState  props_led_rec;
   // FunctionalState  props_child_lock_rec;
   UI08 props_humidity_rec;
   UI08 props_temp_rec;
   UI08 props_temp_coil_rec;
   // ONOFF_STATUS     props_compressor_status_rec;
   // FANSPEED_TYPE    props_fan_speed_rec;
   // FunctionalState  props_tank_full_rec;
   // UI08             props_defrost_status_rec;
   // UI08             props_alarm_rec;
} Wifi_props_str;

typedef enum
{
   e_uap = 0,      //等待快连中
   e_cloud = 1,    //成功连接到路由器
   e_offline = 2,  //掉线重连中
   e_updating = 3, //升级中
   e_unprov = 4    //配网功能关闭
} net_Status_e;
extern MCU_xdata net_Status_e Wifi_net_Status;
extern MCU_xdata net_Status_e Wifi_net_Status_buf;
extern MCU_xdata UI08 Wifi_net_delay;
extern MCU_xdata Wifi_props_str SYS_W_props;
extern MCU_xdata UI08 Star_Process_Num; //初始化WIFI模块参数步骤变量
extern MCU_xdata FuncState Sec_Txd_Star_Process;
extern MCU_xdata FuncState Wifi_model_load;
extern MCU_xdata FANSPEED_TYPE Wifi_Fan_Speed; //风速变化时  上报app(做检测风速变化用)
//
extern MCU_xdata UUI32 Txd_props_bit[];

// Txd_props_bit[0]
#define _props_manufacturer Txd_props_bit[0].bit_.b0  //制造商
#define _props_model Txd_props_bit[0].bit_.b1         //模块
#define _props_serial_number Txd_props_bit[0].bit_.b2 //序列号
#define _props_revision Txd_props_bit[0].bit_.b3      //固件软件版本
// Txd_props_bit[1]
#define _props_on_off Txd_props_bit[1].bit_.b0
#define _props_fault Txd_props_bit[1].bit_.b1
#define _props_mode Txd_props_bit[1].bit_.b2
#define _props_status Txd_props_bit[1].bit_.b3
#define _props_hum_set Txd_props_bit[1].bit_.b4
//#define     _props_tank_levell        Txd_props_bit[1].bit_.b5
#define _props_fan_speed Txd_props_bit[1].bit_.b6

// Txd_props_bit[2];
#define _props_humidity Txd_props_bit[2].bit_.b0
#define _props_temp Txd_props_bit[2].bit_.b6

// Txd_props_bit[3];
#define _props_buzzer Txd_props_bit[3].bit_.b0

// Txd_props_bit[4];

#define _props_led Txd_props_bit[4].bit_.b0

// Txd_props_bit[5];
#define _props_child_lock Txd_props_bit[5].bit_.b0

// Txd_props_bit[6];
#define _props_temp_coil Txd_props_bit[6].bit_.b0
#define _props_compressor_status Txd_props_bit[6].bit_.b1
#define _props_tank_full Txd_props_bit[6].bit_.b2
#define _props_defrost_status Txd_props_bit[6].bit_.b3
#define _props_pump_sw_status Txd_props_bit[6].bit_.b4

// Txd_props_bit[7];
#define _props_pump Txd_props_bit[7].bit_.b0
#define _props_pump_pipe_Status Txd_props_bit[7].bit_.b1

// Txd_props_bit[8];
#define _props_time_run Txd_props_bit[8].bit_.b0
#define _props_run_set Txd_props_bit[8].bit_.b1

extern MCU_xdata UUI32 Set_props_err;            //等待发送的错误
#define SET_POWER_Err Set_props_err.bit_.b0      //开关机设定错误
#define SET_MODE_Err Set_props_err.bit_.b1       //模式设定错误
#define SET_HUM_Err Set_props_err.bit_.b2        //湿度设定错误
#define SET_FAN_Err Set_props_err.bit_.b3        //水箱设定错误
#define SET_LAMP_Err Set_props_err.bit_.b4       //灯光设定错误
#define SET_BUZZER_Err Set_props_err.bit_.b5     //蜂鸣器设定错误
#define SET_CHILD_LOCK_Err Set_props_err.bit_.b6 //童锁设定错误
#define SET_PUMP_Err Set_props_err.bit_.b7       //水泵设定错误
#define SET_TIME_Err Set_props_err.bit_.b8       //定时设定错误

extern MCU_xdata UUI32 Txd_event_bit;             //等待发送的事件
#define _Txd_event_alarm_E0 Txd_event_bit.bit_.b0 //水箱移除
#define _Txd_event_alarm_E1 Txd_event_bit.bit_.b1 //室温传感器故障
#define _Txd_event_alarm_E2 Txd_event_bit.bit_.b2 //管温传感器故障
#define _Txd_event_alarm_E3 Txd_event_bit.bit_.b3 //湿度传感器故障
#define _Txd_event_alarm_E5 Txd_event_bit.bit_.b4 //与电源板通信故障
#define _Txd_event_alarm_E8 Txd_event_bit.bit_.b5 //冷媒报警条件1 计数满3次报警
#define _Txd_event_alarm_FC Txd_event_bit.bit_.b6 //滤网提醒
#define _Txd_event_alarm_F9 Txd_event_bit.bit_.b7 //倾倒提醒

extern MCU_xdata UUI32 Txd_event_bit_buf;                 //等待发送的事件 buf
#define _Txd_event_alarm_E0_buf Txd_event_bit_buf.bit_.b0 //水箱移除
#define _Txd_event_alarm_E1_buf Txd_event_bit_buf.bit_.b1 //室温传感器故障
#define _Txd_event_alarm_E2_buf Txd_event_bit_buf.bit_.b2 //管温传感器故障
#define _Txd_event_alarm_E3_buf Txd_event_bit_buf.bit_.b3 //湿度传感器故障
#define _Txd_event_alarm_E5_buf Txd_event_bit_buf.bit_.b4 //与电源板通信故障
#define _Txd_event_alarm_E8_buf Txd_event_bit_buf.bit_.b5 //冷媒报警条件1 计数满3次报警
#define _Txd_event_alarm_FC_buf Txd_event_bit_buf.bit_.b6 //滤网提醒
#define _Txd_event_alarm_F9_buf Txd_event_bit_buf.bit_.b7 //倾倒提醒

extern MCU_xdata UUI32 Txd_event_ok_bit; //当前发送的事件
#define _Txd_event_ok_alarm_E0 Txd_event_ok_bit.bit_.b0
#define _Txd_event_ok_alarm_E1 Txd_event_ok_bit.bit_.b1
#define _Txd_event_ok_alarm_E2 Txd_event_ok_bit.bit_.b2
#define _Txd_event_ok_alarm_E3 Txd_event_ok_bit.bit_.b3
#define _Txd_event_ok_alarm_E5 Txd_event_ok_bit.bit_.b4 // 与电源板通信故障
#define _Txd_event_ok_alarm_E8 Txd_event_ok_bit.bit_.b5 // 冷媒报警条件1 计数满3次报警
#define _Txd_event_ok_alarm_FC Txd_event_ok_bit.bit_.b6 // 滤网提醒

extern MCU_xdata UUI32 Txd_COM_bit; //公共指令

#define _Txd_COM_net Txd_COM_bit.bit_.b7 //查询网络连接状态
#define _Txd_COM_get_down Txd_COM_bit.bit_.b31

extern MCU_xdata UUI32 Txd_COM_ok_bit; //公共指令

#define _Txd_COM_ok_net Txd_COM_ok_bit.bit_.b7 //查询网络连接状态
#define _Txd_COM_ok_get_down Txd_COM_ok_bit.bit_.b31

//
extern MCU_xdata UUI16 T_result_bit;
#define _wifi_T_result_SET_POWER T_result_bit.bit_.b0
#define _wifi_T_result_SET_MODE T_result_bit.bit_.b1
#define _wifi_T_result_SET_HUM T_result_bit.bit_.b2
#define _wifi_T_result_SET_LAMP T_result_bit.bit_.b3
#define _wifi_T_result_SET_BUZZER T_result_bit.bit_.b4
#define _wifi_T_result_SET_Child_Lock T_result_bit.bit_.b5
#define _wifi_T_result_SET_PUMP T_result_bit.bit_.b6
#define _wifi_T_result_SET_TIMER T_result_bit.bit_.b7
#define _wifi_T_result_SET_FAN T_result_bit.bit_.b8

extern MCU_xdata UUI16 R_result_bit;
#define _wifi_R_result_SET_POWER R_result_bit.bit_.b0
#define _wifi_R_result_SET_MODE R_result_bit.bit_.b1
#define _wifi_R_result_SET_HUM R_result_bit.bit_.b2
#define _wifi_R_result_SET_LAMP R_result_bit.bit_.b3
#define _wifi_R_result_SET_BUZZER R_result_bit.bit_.b4
#define _wifi_R_result_SET_Child_Lock R_result_bit.bit_.b5
#define _wifi_R_result_SET_PUMP R_result_bit.bit_.b6
#define _wifi_R_result_SET_TIMER R_result_bit.bit_.b7
#define _wifi_R_result_SET_FAN R_result_bit.bit_.b8

extern MCU_xdata UUI16 Get_Props_bit;
#define _wifi_Get_Props_En Get_Props_bit.bit_.b0
#define _wifi_Get_Props_load Get_Props_bit.bit_.b1

extern MCU_xdata UUI16 wifi_com_bit;
#define _wifi_com_props_error wifi_com_bit.bit_.b0 //
#define _wifi_com_factory wifi_com_bit.bit_.b1
#define _wifi_com_Rxd_down_error wifi_com_bit.bit_.b2
#define _wifi_com_Rxd_get_prop_error wifi_com_bit.bit_.b3 //网络查询状态
#define _wifi_com_Txd_result_ok wifi_com_bit.bit_.b4      // get_down
#define _wifi_com_mcu_version_req wifi_com_bit.bit_.b5
#define _wifi_com_MIIO_model_req wifi_com_bit.bit_.b6
#define _wifi_com_MIIO_reboot wifi_com_bit.bit_.b7  // 重启
#define _Self_Test_wifi_err wifi_com_bit.bit_.b8    // 自检故障
#define _Self_Test_wifi_TXD_en wifi_com_bit.bit_.b9 // 自检数据发送使能

extern MCU_xdata UUI16 wifi_com_ok_bit;
#define _wifi_com_ok_props_error wifi_com_ok_bit.bit_.b0 //不可设定
#define _wifi_com_ok_factory wifi_com_ok_bit.bit_.b1
#define _wifi_com_ok_Rxd_down_error wifi_com_ok_bit.bit_.b2
#define _wifi_com_ok_Rxd_get_prop_error wifi_com_ok_bit.bit_.b3 //网络查询状态
#define _wifi_com_ok_Txd_result_ok wifi_com_ok_bit.bit_.b4      // get_down
#define _wifi_com_ok_mcu_version_req wifi_com_ok_bit.bit_.b5
#define _wifi_com_ok_MIIO_model_req wifi_com_ok_bit.bit_.b6
#define _wifi_com_ok_MIIO_reboot wifi_com_ok_bit.bit_.b7 // 重启

extern UI08 chack_down_deal(UI08 type_buf);

extern void Rest_Wifi(void);
extern void Wifi_UART_Txd_Data(void);
extern void Wifi_UART_Init(void);
extern void Wifi_UART_Deal(void);
extern void prg_Wifi_UART_ms(void);
extern void Txd_data(const UI08 *point, UI16 lenth);

#endif
