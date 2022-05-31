#ifndef _TUYA_API_H
#define _TUYA_API_H
#include "DataType.h"

#define WIFI_COMM_ERR_TIME (60 * 3) //与WIFI通信故障时间

//设置湿度的上传值
#define HUM_SET_30_UPLOAD 0X00
#define HUM_SET_40_UPLOAD 0X01
#define HUM_SET_50_UPLOAD 0X02
#define HUM_SET_60_UPLOAD 0X03
#define HUM_SET_70_UPLOAD 0X04
//故障上报值
#define TEMP_HUM_SEN_ERR_UPLOAD (1 << 0) // E1,温湿度传感器故障
#define COIL_SEN_ERR_UPLOAD (1 << 1)     // E2,铜管传感器故障
#define MOTOR_ERR_UPLOAD (1 << 2)        // E4,马达故障
#define MCU_COMMUN_ERR_UPLOAD (1 << 3)   // E5,MCU通信故障
#define DUST_SEN_ERR_UPLOAD (1 << 4)     // E9,粉尘传感器故障

typedef struct
{
    ONOFF_STATUS power_status;     //开关机状态
    UI08 hum_set;                  //湿度设置
    FANSPEED_TYPE fan_speed;       //风速
    HUM_TYPE hum_mode;             //除湿模式
    UI08 hum_value;                //当前湿度
    FuncState led_disp_status;     //灯光状态
    UI16 pm2_5_value;              // PM2.5数据
    FuncState child_lock_status;   //童锁状态
    UI08 timer_set_value;          //定时设置
    UI08 err_code;                 //故障代码
    UI08 filter_status;            //滤网状态
    FuncState water_full;          //满水状态
    UI08 defrost_status;           //除霜状态
    UI08 hepa_filter;              //当前hepa滤网状态
    DYR_TYPE dry_mode;             //干衣模式
    UI08 swing_mode;               //当前摆风
    AIR_CLEAR_MODE air_clear_mode; //净化模式
    UI08 sound_mode;               //当前提示音
    FuncState uvc_mode;            // UVC模式
    SYS_MODE sys_mode;             //系统模式
} WIFI_UPDATE;

extern xdata UUI16 flag_wifi;
#define _Flash_250ms flag_wifi.bit_.b0
#define _Flash_1500ms flag_wifi.bit_.b1
#define WiFi_LED_Locate_buf flag_wifi.bit_.b2 //wifi指示灯状态
#define _wifi_reset_En flag_wifi.bit_.b3 //wifi模块复位/配网模式切换
#define _Wifi_Online_Status flag_wifi.bit_.b4
#define _Wifi_Comm_Err flag_wifi.bit_.b5
#define _Wifi_factory_test flag_wifi.bit_.b6
#define _Self_Test_wifi_TXD_en flag_wifi.bit_.b7 // wifi接口自检
#define _WIFI_10S_check_status flag_wifi.bit_.b8 // 10S主动查询一次wifi 状态
#define _all_status_updata_en flag_wifi.bit_.b9  //上报机台信息
#define _Self_Test_wifi_err flag_wifi.bit_.b10   // wifi口自检
#define _props_run_set flag_wifi.bit_.b11        //定时主动上报
#define _wifi_WakeUp_En flag_wifi.bit_.b12       //激活唤醒睡眠

extern xdata UI08 Wifi_value_compare_delay; // app操作后延时对比数据
extern xdata WIFI_UPDATE Wifi_Updata;
extern xdata UI08 M_wifi_rssi_dsp;       // wifi 信号强度
extern xdata UI08 all_data_update_point; //全部数据上报时的计数值
extern xdata FANSPEED_TYPE Updata_speed;
extern xdata UI08 upload_filter_status;
extern xdata UI08 upload_defrost_status;
extern xdata UI08 upload_hepa_filter;
extern xdata UI08 Wifi_Rssi_DSP_delay_time;

extern void tuya_init(void);
extern void tuya_deal(void);
extern void wifi_rssi_Dsp(void);

#endif
