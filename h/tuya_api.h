#ifndef _TUYA_API_H
#define _TUYA_API_H
#include "DataType.h"

#define WIFI_COMM_ERR_TIME (60 * 3) //��WIFIͨ�Ź���ʱ��

//����ʪ�ȵ��ϴ�ֵ
#define HUM_SET_30_UPLOAD 0X00
#define HUM_SET_40_UPLOAD 0X01
#define HUM_SET_50_UPLOAD 0X02
#define HUM_SET_60_UPLOAD 0X03
#define HUM_SET_70_UPLOAD 0X04
//�����ϱ�ֵ
#define TEMP_HUM_SEN_ERR_UPLOAD (1 << 0) // E1,��ʪ�ȴ���������
#define COIL_SEN_ERR_UPLOAD (1 << 1)     // E2,ͭ�ܴ���������
#define MOTOR_ERR_UPLOAD (1 << 2)        // E4,������
#define MCU_COMMUN_ERR_UPLOAD (1 << 3)   // E5,MCUͨ�Ź���
#define DUST_SEN_ERR_UPLOAD (1 << 4)     // E9,�۳�����������

typedef struct
{
    ONOFF_STATUS power_status;     //���ػ�״̬
    UI08 hum_set;                  //ʪ������
    FANSPEED_TYPE fan_speed;       //����
    HUM_TYPE hum_mode;             //��ʪģʽ
    UI08 hum_value;                //��ǰʪ��
    FuncState led_disp_status;     //�ƹ�״̬
    UI16 pm2_5_value;              // PM2.5����
    FuncState child_lock_status;   //ͯ��״̬
    UI08 timer_set_value;          //��ʱ����
    UI08 err_code;                 //���ϴ���
    UI08 filter_status;            //����״̬
    FuncState water_full;          //��ˮ״̬
    UI08 defrost_status;           //��˪״̬
    UI08 hepa_filter;              //��ǰhepa����״̬
    DYR_TYPE dry_mode;             //����ģʽ
    UI08 swing_mode;               //��ǰ�ڷ�
    AIR_CLEAR_MODE air_clear_mode; //����ģʽ
    UI08 sound_mode;               //��ǰ��ʾ��
    FuncState uvc_mode;            // UVCģʽ
    SYS_MODE sys_mode;             //ϵͳģʽ
} WIFI_UPDATE;

extern xdata UUI16 flag_wifi;
#define _Flash_250ms flag_wifi.bit_.b0
#define _Flash_1500ms flag_wifi.bit_.b1
#define WiFi_LED_Locate_buf flag_wifi.bit_.b2 //wifiָʾ��״̬
#define _wifi_reset_En flag_wifi.bit_.b3 //wifiģ�鸴λ/����ģʽ�л�
#define _Wifi_Online_Status flag_wifi.bit_.b4
#define _Wifi_Comm_Err flag_wifi.bit_.b5
#define _Wifi_factory_test flag_wifi.bit_.b6
#define _Self_Test_wifi_TXD_en flag_wifi.bit_.b7 // wifi�ӿ��Լ�
#define _WIFI_10S_check_status flag_wifi.bit_.b8 // 10S������ѯһ��wifi ״̬
#define _all_status_updata_en flag_wifi.bit_.b9  //�ϱ���̨��Ϣ
#define _Self_Test_wifi_err flag_wifi.bit_.b10   // wifi���Լ�
#define _props_run_set flag_wifi.bit_.b11        //��ʱ�����ϱ�
#define _wifi_WakeUp_En flag_wifi.bit_.b12       //�����˯��

extern xdata UI08 Wifi_value_compare_delay; // app��������ʱ�Ա�����
extern xdata WIFI_UPDATE Wifi_Updata;
extern xdata UI08 M_wifi_rssi_dsp;       // wifi �ź�ǿ��
extern xdata UI08 all_data_update_point; //ȫ�������ϱ�ʱ�ļ���ֵ
extern xdata FANSPEED_TYPE Updata_speed;
extern xdata UI08 upload_filter_status;
extern xdata UI08 upload_defrost_status;
extern xdata UI08 upload_hepa_filter;
extern xdata UI08 Wifi_Rssi_DSP_delay_time;

extern void tuya_init(void);
extern void tuya_deal(void);
extern void wifi_rssi_Dsp(void);

#endif
