#include "General.h"

MCU_xdata UUI08 Sys_eep_bit = {0}; // EEP�����־λ

MCU_xdata UI32 M_Timer_Run = 0;   //��ʱʣ��ʱ��
MCU_xdata UI08 M_Timer_Set = 1;   //��ʱ�趨ʱ��
MCU_xdata UI08 M_Timer_Buf = 1;   //��ʱ�趨ʱ��buf
MCU_xdata UI08 M_Timer_Point = 0; //��ʱ point
// MCU_xdata UI16  M_Power_Delay_Time=300;//�ϵ���ʱ
MCU_xdata UI16 M_Power_Delay_Time2 = 300; //�ϵ���ʱ(�����Լ�)

MCU_xdata UI08 M_Timer_Setting_Time = 0; //��ʱ�趨(ȷ��ʱ��)
MCU_xdata UI08 M_Dsp_Time = 0;           //��ʾʱ��
MCU_xdata UI08 Hum_dsp_com = 60;         //����ʪ��ֵ��ʾ
MCU_xdata UI08 Hum_dsp_state = 60;         //����ʪ��״̬��ʾ

MCU_xdata UI08 M_test_seq = 0;    //�Լ�
MCU_xdata UI08 M_test_cont1 = 0;  //�Լ�
MCU_xdata UI08 M_test_cont2 = 0;  //�Լ�
MCU_xdata UUI08 M_sys_bit = {0};  //ϵͳ��־λ
MCU_xdata UUI16 M_sys_bit1 = {0}; //ϵͳ��־λ1
MCU_xdata SYS_ERR Sys_Err;        //����

//
MCU_xdata ONOFF_STATUS SYS_Power_Status = OFF; //���ػ�״̬

MCU_xdata SYS_MODE SYS_Mode = mode_SYS_HUM;     //ϵͳģʽ
MCU_xdata SYS_MODE SYS_Mode_Buf = mode_SYS_HUM; //ϵͳģʽ
MCU_xdata UI08 Set_SYS_Mode_Timer = 0;          //ģʽ�趨ʱ��
UI08 G_Disp_SA_Time = 0; //SA������ʾʱ��

MCU_xdata DYR_TYPE SYS_DYR_Tyde = DYR_STRONG;     //��������
MCU_xdata DYR_TYPE SYS_DYR_Tyde_Buf = DYR_STRONG; //��������
MCU_xdata UI08 Set_SYS_DYR_Tyde_timer = 0;

MCU_xdata HUM_TYPE SYS_HUN_Tyde = USER_DEFINE_HUM;     //��ʪ����
MCU_xdata HUM_TYPE SYS_HUN_Tyde_Buf = USER_DEFINE_HUM; //��ʪ����
MCU_xdata UI08 SET_SYS_HUN_Tyde_Timer = 0;             //ʪ���趨(ȷ��ʱ��)

MCU_xdata UI08 SYS_Hum_Set = 0;     //ʪ���趨
MCU_xdata UI08 SYS_Hum_Set_Buf = 0; //ʪ���趨buf
MCU_xdata UI08 Set_SYS_Hum_timer = 0;

MCU_xdata FANSPEED_TYPE SYS_Fan_Tyde = SILENCE_FAN;     //���з���
MCU_xdata FANSPEED_TYPE SYS_Fan_Tyde_Buf = SILENCE_FAN; //���з���
MCU_xdata FANSPEED_TYPE Fan_Speed_Out = OFF_FAN;        //ʵ�����з���
MCU_xdata FANSPEED_TYPE Fan_Speed_Out_Buf = OFF_FAN;    //ʵ�����з���
MCU_xdata FuncState Set_Fan_State = Enable;             //�Ƿ��������
MCU_xdata UI08 Set_SYS_Fan_Tyde_timer = 0;
MCU_xdata UI16 FAN_ON_timer = 0;
MCU_xdata UI16 FAN_OFF_timer = 0;

MCU_xdata SWING_MODE SYS_Swing_Tyde = STOP_SWING;     //��Ҷ����
MCU_xdata SWING_MODE SYS_Swing_Tyde_Buf = STOP_SWING; //��Ҷ����
MCU_xdata SWING_MODE Swing_Tyde_Out = STOP_SWING;     //ʵ�ʰ�Ҷ��ʽ
MCU_xdata SWING_MODE Swing_Tyde_Out_Buf = STOP_SWING; //ʵ�ʰ�Ҷ��ʽ
MCU_xdata UI08 Set_SYS_Swing_Tyde_Timer = 0;
MCU_xdata FuncState Set_Swing_State = Disable; //�Ƿ��������ð�Ҷ

MCU_xdata AIR_CLEAR_MODE SYS_Air_Clear_Tyde = AUTO_AIR_CLEAR;     //������������
MCU_xdata AIR_CLEAR_MODE SYS_Air_Clear_Tyde_Buf = AUTO_AIR_CLEAR; //������������
MCU_xdata UI08 Set_SYS_Air_Clear_Tyde_Timer = 0;

MCU_xdata FuncState LED_Disp_Status = ENABLE; //�ƹ��Ƿ��

MCU_xdata FuncState Buzzer_Status = ENABLE; //�������Ƿ�ʹ��

MCU_xdata UI16 SYS_filter_time = 0;     //����ʱ��
MCU_xdata UI16 SYS_filter_time_buf = 0; //����ʱ��

UI16 Buzz_Time = 0;               //������ʱ��
MCU_xdata UI08 Buzz_Cnt = 0;      //�����������
MCU_xdata UI08 Dump_Buzz_Cnt = 0; //�㵹�����������
MCU_xdata PinStatus Buzzer_IO_Status = RESET;

/*************************************************
 // ��������    : SYSData_Rest
 // ��������    : ������ʼ��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void SYSData_Rest(void)
{
        _Fast_Test = 0;

        M_Timer_Run = 0;
        M_Timer_Set = 1;
        M_Timer_Buf = 1;

        Pump_Status = DISABLE;

        SYS_Mode = mode_SYS_HUM;     //ϵͳģʽ
        SYS_Mode_Buf = mode_SYS_HUM; //ϵͳģʽ
        Set_SYS_Mode_Timer = 0;      //ģʽ�趨ʱ��

        SYS_DYR_Tyde = DYR_AUTO;     //��������
        SYS_DYR_Tyde_Buf = DYR_AUTO; //��������
        Set_SYS_DYR_Tyde_timer = 0;

        SYS_HUN_Tyde = CONTINUOUS_HUM;     //��ʪ����
        SYS_HUN_Tyde_Buf = CONTINUOUS_HUM; //��ʪ����

        SYS_Hum_Set = 0;            //ʪ���趨
        SYS_Hum_Set_Buf = 0;        //ʪ���趨buf
        SET_SYS_HUN_Tyde_Timer = 0; //ʪ���趨(ȷ��ʱ��)

        SYS_Fan_Tyde = SILENCE_FAN;     //���з���
        SYS_Fan_Tyde_Buf = SILENCE_FAN; //���з���
        Set_Fan_State = ENABLE;
        Set_SYS_Fan_Tyde_timer = 0;

        SYS_Swing_Tyde = STOP_SWING;     //��Ҷ����
        SYS_Swing_Tyde_Buf = STOP_SWING; //��Ҷ����
        Set_Swing_State = Disable;
        Set_SYS_Swing_Tyde_Timer = 0;

        SYS_Air_Clear_Tyde = AUTO_AIR_CLEAR;     //������������
        SYS_Air_Clear_Tyde_Buf = AUTO_AIR_CLEAR; //������������
        Set_SYS_Air_Clear_Tyde_Timer = 0;

        Buzzer_Status = ENABLE;

        LED_Disp_Status = ENABLE;

        M_Defrost_status = 0;

        SYS_filter_time = 0;

        Run_Reg.def_cont = 0;
        Run_Reg.def_time = 0;
        Run_Reg.HI_stay_time = 0;
        Run_Reg.LO_stay_time = 0;
        //   M_write_delay_time=2;
        wifi_net_led_timer = WIFI_NET_TIMER;
}
/*************************************************
 // ��������    : SYS_data_init
 // ��������    : ������ʼ��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void SYS_data_init(void)
{
        Adc_data_init();
        Run_reg_init();
        // Wifi_UART_Init();
        io_data_init();
        SYSData_Rest();

        M_sys_bit.byte = 0;  //ϵͳ��־λ
        M_sys_bit1.word = 0; //ϵͳ��־λ1

        //
        Buzz_Time = BUZZ_long_time;
        Buzz_Cnt = 0;
        Dump_Buzz_Cnt = 0;

        Sys_Err.Water_Full = DISABLE;
        Sys_Err.temp_room_err = DISABLE;
        Sys_Err.temp_coil_err = DISABLE;
        Sys_Err.hum_Sensor_err = DISABLE;
        Sys_Err.comm_err = DISABLE;
        Sys_Err.pm25_Sensor_err = DISABLE; //�۳�����
        Sys_Err.pan_motor_err = DISABLE;   //�������

        Temp_room.status = AI_NORMAL;
        Temp_coil.status = AI_NORMAL;
        Hum_para.status = AI_NORMAL;

        Hum_para.value = 60;
        Temp_room.value = 25 + 9;
        Temp_coil.value = 25 + 9;

        Sys_eep_bit.byte = 0;

        _SYS_UVC_Status = 0;
}
/*************************************************
 // ��������    : prg_s_general
 // ��������    : 100mSʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
UI08 cont_200ms = 0;
void prg_ms100_general(void)
{
        if (!_100mS_For_SYS)
        {
                return;
        }

        if (++cont_200ms < 4)
        {
                return;
        }
        cont_200ms = 0;

        _Self_Test_wifi_TXD_en = 1;

        if ((M__Self_Test) && (Power_Delay_Time == 0))
        {
                if (M_test_cont1 < 0xff)
                {
                        M_test_cont1++;
                }
                if ((M_test_cont2 < 0xff) && (M_test_cont1 >= 8))
                {
                        M_test_cont2++;
                }
        }
}
/*************************************************
 // ��������    : prg_s_general
 // ��������    : Sʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_s_general(void)
{
        static MCU_xdata UI08 M_hum_update_time1 = 0, M_hum_update_time2 = 0;    //ʪ�ȸ���ʱ��

        if (!_1S_For_For_SYS)
        {
                return;
        }

        M_hum_update_time1++;
        if ((M_hum_update_time1 >= 3) || (Power_Delay_Time > 0))
        {
                M_hum_update_time1 = 0;
                Hum_dsp_com = Hum_para.value;
        }
        M_hum_update_time2++;
        if (M_hum_update_time2 >= 60 || Power_Delay_Time > 0)
        {
                M_hum_update_time2 = 0;
                Hum_dsp_state = Hum_para.value;
        }

        if (Power_Delay_Time > 0)
        {
                Power_Delay_Time--;
        }

        if (Buzz_Cnt > 0)
        {
                Buzz_Cnt--;
                Buzz_Time = BUZZ_long_time;
        }
        else if (Key_ERR_Buzz_Cnt > 0)
        {
                Key_ERR_Buzz_Cnt--;
                Buzz_Time = BUZZ_short_time;
        }
        else if (Dump_Buzz_Cnt > 0)
        {
                Dump_Buzz_Cnt--;
                Buzz_Time = BUZZ_long_time;
        }

        //����ʱ��, ��������
        // if (M_sleep_Time > 0)
        // {
        //         M_sleep_Time--;
        //         if (M_sleep_Time == 0)
        //         {
        //                 _Sleep_status = 1;
        //         }
        // }

        if (communication_timer > 0)
        {
                communication_timer--;
                if (communication_timer == 0)
                {
                        Sys_Err.comm_err = ENABLE;
                }
        }
}
/*************************************************
//����        :	F_C
//����        : Fת��ΪC
//��ڲ���    :	��
//���ڲ���    :	��
//��	��: 	GCE XXX
//��	��: 	GCE XXX

ԭʼ��ʽ ��������
(X-32)*5/9+0.5
�Ż���ʽ
((X-32)*10+9)/18
**************************************************/
/*
UI08 F_C(UI08 cfdata)
{
        UI08 a=0;
      if(cfdata>32)
      {a=((cfdata-32)*10+9)/18;}
      else
      {a=((32-cfdata)*10+9)/18;}

        return 	a;
}
*/
/*************************************************
//����        :	C_F
//����        : Cת��ΪF
//��ڲ���    :	��
//���ڲ���    :	��
//��	��: 	GCE XXX
//��	��: 	GCE XXX
ԭ��ʽΪ :  F=C*9/5+32
**************************************************/
/*
UI08 C_F(UI08 cfdata)
{
        UI08 a=0;
        a=((cfdata-9)*18+5)/10+32;
        if (a>89)
        {a=89;}
       return a;
}
*/
/*************************************************
 // ��������    : delay
 // ��������    : ��ʱ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
/*
void delay(UI16 i)
{
        while(i)
        {i--;}
}
*/
/*************************************************
 // ��������    : Turn_On
 // ��������    : ��������
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Turn_On(void)
{
        if ((SYS_Power_Status) || (Sys_Err.Water_Full))
        {
                return;
        }

        //
        SYS_Power_Status = ON;
        M_Timer_Run = 0;
        M_Timer_Setting_Time = 0;
        _Timer_set_ok = 0;
        Set_SYS_Hum_timer = 0;
        M_Dsp_Time = 10;

}
/*************************************************
 // ��������    : Turn_Off
 // ��������    : �ػ�����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Turn_Off(void)
{
        if (SYS_Power_Status == OFF)
        {
                return;
        }
        SYS_Power_Status = OFF;

        //
        M_Timer_Run = 0;
        M_Timer_Setting_Time = 0;
        _Timer_set_ok = 0;
        Set_SYS_Hum_timer = 0;
        M_Dsp_Time = 0;

        G_Comp_Test_EN = 0; //�˳�ѹ��������
}

/*************************************************
 // ��������    : WdtInit
 // ��������    : ���Ź���ʼ��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void WdtInit(void)
{
}
/*************************************************
 // ��������    : Sys_Initial
 // ��������    : //ϵͳ��ʼ��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Sys_Initial(void)
{
        EA = 0;
        IO_Init();
        PWM_Init();
        SYS_data_init();
        TimerInit();
        TouchKeyInit();
        tuya_init();
        WdtInit();
        EA = 1;
}
/*************************************************
 // ��������    : general_deal
 // ��������    :
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void general_deal(void)
{
        prg_ms100_general();
        prg_s_general();
        Timer_Deal();
}
