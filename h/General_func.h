#ifndef __General_func_H
#define __General_func_H

#include "DataType.h"

extern MCU_xdata UUI08 Sys_eep_bit;
//#define  _check_EEP_EN   Sys_eep_bit.bit_.b0//����ʹ��
//#define  _Write_EEP_EN   Sys_eep_bit.bit_.b1//
//#define  _MS100_EEP_EN   Sys_eep_bit.bit_.b2//

extern MCU_xdata UI32 M_Timer_Run;   //��ʱʣ��ʱ��
extern MCU_xdata UI08 M_Timer_Set;   //��ʱ�趨ʱ��
extern MCU_xdata UI08 M_Timer_Buf;   //��ʱ�趨ʱ��buf
extern MCU_xdata UI08 M_Timer_Point; //��ʱ point
// extern MCU_xdata UI16  M_Power_Delay_Time;//�ϵ���ʱ
extern MCU_xdata UI16 M_Power_Delay_Time2; //�ϵ���ʱ(�����Լ�)

// extern MCU_xdata UI08  M_Hum_Set;//ʪ���趨
// extern MCU_xdata UI08  M_Hum_Set_buf;//ʪ���趨buf
// extern MCU_xdata UI08  M_Dry_Status;//����״̬
// extern MCU_xdata UI08  M_Swing_Status;//��Ҷ״̬
// extern MCU_xdata UI08  M_continuous;  //����ģʽ

// extern MCU_xdata UI08 M_Hum_Setting_Time;//ʪ���趨(ȷ��ʱ��)
extern MCU_xdata UI08 M_Timer_Setting_Time; //��ʱ�趨(ȷ��ʱ��)
extern MCU_xdata UI08 M_Dsp_Time;           //��ʾʱ��
extern MCU_xdata UI08 Hum_dsp_com;          //��ʾʪ��
extern MCU_xdata UI08 M_hum_update_time;    //ʪ�ȸ���ʱ��

extern MCU_xdata UI08 M_test_seq;   //�Լ�
extern MCU_xdata UI08 M_test_cont1; //�Լ�
extern MCU_xdata UI08 M_test_cont2; //�Լ�
extern MCU_xdata SYS_ERR Sys_Err;   //����
extern MCU_xdata UUI08 M_sys_bit;

extern MCU_xdata ONOFF_STATUS SYS_Power_Status; //���ػ�״̬

extern MCU_xdata SYS_MODE SYS_Mode;       //ϵͳģʽ
extern MCU_xdata SYS_MODE SYS_Mode_Buf;   //ϵͳģʽ
extern MCU_xdata UI08 Set_SYS_Mode_Timer; //ģʽ�趨ʱ��

extern MCU_xdata DYR_TYPE SYS_DYR_Tyde;     //��������
extern MCU_xdata DYR_TYPE SYS_DYR_Tyde_Buf; //��������
extern MCU_xdata UI08 Set_SYS_DYR_Tyde_timer;

extern MCU_xdata HUM_TYPE SYS_HUN_Tyde;       //��ʪ����
extern MCU_xdata HUM_TYPE SYS_HUN_Tyde_Buf;   //��ʪ����
extern MCU_xdata UI08 SET_SYS_HUN_Tyde_Timer; //ʪ���趨(ȷ��ʱ��)

extern MCU_xdata UI08 SYS_Hum_Set;     //ʪ���趨
extern MCU_xdata UI08 SYS_Hum_Set_Buf; //ʪ���趨buf
extern MCU_xdata UI08 Set_SYS_Hum_timer;

extern MCU_xdata FANSPEED_TYPE SYS_Fan_Tyde;      //���з���
extern MCU_xdata FANSPEED_TYPE SYS_Fan_Tyde_Buf;  //���з���
extern MCU_xdata FANSPEED_TYPE Fan_Speed_Out;     //ʵ�����з���
extern MCU_xdata FANSPEED_TYPE Fan_Speed_Out_Buf; //ʵ�����з���
extern MCU_xdata FuncState Set_Fan_State;         //�Ƿ��������
extern MCU_xdata UI08 Set_SYS_Fan_Tyde_timer;
extern MCU_xdata UI16 FAN_ON_timer;
extern MCU_xdata UI16 FAN_OFF_timer;

extern MCU_xdata SWING_MODE SYS_Swing_Tyde;     //��Ҷ����
extern MCU_xdata SWING_MODE SYS_Swing_Tyde_Buf; //��Ҷ����
extern MCU_xdata SWING_MODE Swing_Tyde_Out;     //ʵ�ʰ�Ҷ��ʽ
extern MCU_xdata SWING_MODE Swing_Tyde_Out_Buf; //ʵ�ʰ�Ҷ��ʽ
extern MCU_xdata UI08 Set_SYS_Swing_Tyde_Timer;
extern MCU_xdata FuncState Set_Swing_State; //�Ƿ��������ð�Ҷ

extern MCU_xdata AIR_CLEAR_MODE SYS_Air_Clear_Tyde;     //������������
extern MCU_xdata AIR_CLEAR_MODE SYS_Air_Clear_Tyde_Buf; //������������
extern MCU_xdata UI08 Set_SYS_Air_Clear_Tyde_Timer;

extern MCU_xdata FuncState LED_Disp_Status;

extern MCU_xdata UI16 SYS_filter_time;     //����ʱ��
extern MCU_xdata UI16 SYS_filter_time_buf; //����ʱ��

extern MCU_xdata FuncState Buzzer_Status;
extern UI16 Buzz_Time;          //������ʱ��
extern MCU_xdata UI08 Buzz_Cnt; //�����������
extern MCU_xdata UI08 Dump_Buzz_Cnt;
extern MCU_xdata PinStatus Buzzer_IO_Status;

#define M__Flash_500ms M_sys_bit.bit_.b0        // 500ms��˸
#define M__Fan_set_enable M_sys_bit.bit_.b1     //����趨ʹ��
#define M_Defrost_status M_sys_bit.bit_.b2      //��˪
#define M__Self_Test M_sys_bit.bit_.b3          //�Լ�
#define touch_version_read_ok M_sys_bit.bit_.b4 //����оƬ��ȡ�汾OK?
#define M_ADC_check M_sys_bit.bit_.b5           // ADC����  ���յ����ݺ���

extern MCU_xdata UUI16 M_sys_bit1;
#define _Fast_Test M_sys_bit1.bit_.b0      //���
#define _Timer_set_ok M_sys_bit1.bit_.b3   //��ʱ�趨
#define _SYS_UVC_Status M_sys_bit1.bit_.b4 // UVC��

extern void Turn_On(void);
extern void Turn_Off(void);
extern void prg_s_general(void);
extern void prg_ms200_test(void);
extern void Sys_Initial(void); //	ϵͳ ��ʼ������
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
