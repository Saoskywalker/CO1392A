#include "General.h"

MCU_xdata FuncState Set_timer_status = DISABLE;    //�����趨
MCU_const UI08 Timer_SET[6] = {1, 2, 4, 8, 12, 0}; //��ʱ�趨
MCU_xdata UI08 S_Self_Test_delay_Time = 3;	  //���ذ��������Լ�ȴ�ʱ��
static MCU_const UI08 Hum_SET[] = {0, 40, 50, 60, 70};   //ʪ���趨
/* Private defines -----------------------------------------------------------*/

MCU_xdata UI32 exKeyValueFlag = 0; //��ǰ�ְ�����־
MCU_const UI32 TK_Number[4] = {
    TK_23, //����1
    TK_24, //����2
    TK_26, //����3
    TK_28, //����4
};

/*************************************************
 // ��������    : prg_ms10_key
 // ��������    : 10msʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_ms10_key(void)
{
    if (!_10mS_For_SYS)
    {
        return;
    }

    if (M_Power_Delay_Time2 > 0)
    {
        M_Power_Delay_Time2--;
    }
}

/*************************************************
 // ��������    : prg_ms10_key
 // ��������    : 10msʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_mS100_key(void)
{
    if (!_100mS_For_SYS)
    {
        return;
    }
    //
    if (Set_SYS_Mode_Timer > 0)
    {
        Set_SYS_Mode_Timer--;
        if (Set_SYS_Mode_Timer == 0)
        {
            SYS_Mode = SYS_Mode_Buf;
        }
    }
    //
    if (Set_SYS_DYR_Tyde_timer > 0)
    {
        Set_SYS_DYR_Tyde_timer--;
        if (Set_SYS_DYR_Tyde_timer == 0)
        {
            SYS_DYR_Tyde = SYS_DYR_Tyde_Buf;
        }
    }
    //
    if (SET_SYS_HUN_Tyde_Timer > 0)
    {
        SET_SYS_HUN_Tyde_Timer--;
        if (SET_SYS_HUN_Tyde_Timer == 0)
        {
            SYS_HUN_Tyde = SYS_HUN_Tyde_Buf;
        }
    }
    //
    if (Set_SYS_Hum_timer > 0)
    {
        Set_SYS_Hum_timer--;
        if (Set_SYS_Hum_timer == 0)
        {
            SYS_Hum_Set = SYS_Hum_Set_Buf;
        }
    }
    //
    if (Set_SYS_Fan_Tyde_timer > 0)
    {
        Set_SYS_Fan_Tyde_timer--;
        if (Set_SYS_Fan_Tyde_timer == 0)
        {
            SYS_Fan_Tyde = SYS_Fan_Tyde_Buf;
        }
    }
    //
    if (Set_SYS_Swing_Tyde_Timer > 0)
    {
        Set_SYS_Swing_Tyde_Timer--;
        if (Set_SYS_Swing_Tyde_Timer == 0)
        {
            SYS_Swing_Tyde = SYS_Swing_Tyde_Buf;
        }
    }
    //
    if (Set_SYS_Air_Clear_Tyde_Timer > 0)
    {
        Set_SYS_Air_Clear_Tyde_Timer--;
        if (Set_SYS_Air_Clear_Tyde_Timer == 0)
        {
            SYS_Air_Clear_Tyde = SYS_Air_Clear_Tyde_Buf;
        }
    }
}
/*************************************************
 // ��������    : prg_s_key
 // ��������    : Sʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_s_key(void)
{
    if (!_1S_For_For_SYS)
    {
        return;
    }

    if (Disp_Delay > 0)
    {
        Disp_Delay--;
    }

    if (Disp_set_Delay > 0)
    {
        Disp_set_Delay--;
    }

    if (M_Timer_Setting_Time > 0)
    {
        M_Timer_Setting_Time--;
        if ((M_Timer_Setting_Time == 0) && (_Timer_set_ok == 1))
        {

            M_Timer_Set = M_Timer_Buf;
            M_Timer_Run = M_Timer_Set;
            M_Timer_Run = M_Timer_Run * 3600;
            // Set_timer_status=DISABLE;
            M_Timer_Setting_Time = 0;
        }
    }

    if (M_Timer_Run > 0)
    {
        M_Timer_Run--;
        if (M_Timer_Run == 0)
        {
            if (SYS_Power_Status == ON)
            {
                Turn_Off();
            }
            else
            {
                Turn_On();
                fan_force_runtime = 180;
            }
        }
    }

    if (M_Timer_Run > 0)
    {
        if ((M_Timer_Run % 3600) == 0)
        {
            M_Timer_Buf = (UI08)(M_Timer_Run / 3600);
        }
    }

    if (M_Dsp_Time > 0)
    {
        M_Dsp_Time--;
    }

    if(S_Self_Test_delay_Time > 0)
    {
       S_Self_Test_delay_Time--;
    }
}
/*************************************************
 // ��������    : Rest_Key_Buzzer
 // ��������    : ������
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Rest_Key_Buzzer(void)
{
    Key_ERR_Buzz_Cnt = 0;
    Buzz_Time = 1; //�����Ը�0����ֹ������IOһֱ���1
}

/*********************************************
// �������� : Enter_Test_Judge
// ����˵�� : �ж��Ƿ�����Լ�
 * ******************************************/
UI08 Enter_Test_Judge(UI08 key)
{
	static UI16 keep_cnt = 0;

	if (S_Self_Test_delay_Time > 0) // 3s
	{
		if (key == Dry_Clothes_key)
		{
			keep_cnt++;
			S_Self_Test_delay_Time = 2;
		}
	}

	if (keep_cnt >= 300)
	{
		keep_cnt = 0;
		S_Self_Test_delay_Time = 0;
		return 1;
	}

	return 0;
}

/*************************************************
 // ��������    : get_key_number
 // ��������    : ��ֵ��ȡ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
MCU_xdata UUI08 M_Key_flag = {0};
MCU_xdata UI08 G_Key_Number = 0; //��ֵ
void get_key_number(void)
{
    static UI16 WIFI_Self_Test_delay_time = 500;
    static MCU_xdata UI16 M_Key_last = 0;   //��������ʱ��
    static MCU_xdata UI08 shake_count = 0;  //������ʱ
    static MCU_xdata UI08 key_new = 0;      //�¼�ֵ
    static MCU_xdata UI08 key_old = 0;      // old ��ֵ
    static MCU_xdata UI16 Read_key_delay = 0; //��ϰ����󣬵ȴ�Nʱ�����ʶ�𰴼�����������ʱʶ�𵽰���

    UI08 i = 0;
    UI08 key_total_count = 0;
    UI08 number = 0;
    UI08 key_data = 0; //����data
                       //
    if (!_10mS_For_SYS)
    {
        return;
    }

    if (M_Key_last < 0xffff)
    {
        M_Key_last++;
    }

    if (SYS_Power_Status == ON)
        WIFI_Self_Test_delay_time = 500;
    if (WIFI_Self_Test_delay_time)
        WIFI_Self_Test_delay_time--;

    if (Read_key_delay > 0)
    {
        Read_key_delay--;
        return;
    }

    if (SOCAPI_TouchKeyStatus & 0x80) //��Ҫ����2:  ������ɨ��һ�ֱ�־���Ƿ����TouchKeyScan()һ��Ҫ���ݴ˱�־λ�����
    {
        SOCAPI_TouchKeyStatus &= 0x7f;   //��Ҫ����3: �����־λ�� ��Ҫ�ⲿ�����
        exKeyValueFlag = TouchKeyScan(); //�������ݴ�����
        TouchKeyRestart();               //������һ��ת��
    }

    for (i = 0; i < 4; i++)
    {
        if (TK_Number[i] & exKeyValueFlag)
        {
            key_data |= (0x01 << i);
        }
    }

    _SYS_Inspect_Key_OK = 1;
    SYS_Inspect_Key_Data = key_data;

    for (i = 0; i < 8; i++)
    {
        if (key_data & (UI08)(0x01 << i))
        {
            key_total_count++;
            number = i + 1;
        }
    }

    if (key_total_count == 0)
    {
        if (shake_count == 3)
        {
            G_Key_Number = key_old;
            _KEY_OK = 1;
        }

        shake_count = 0;
        M_Key_last = 0;
        return;
    }
    else if (key_total_count == 1)
    {
        key_new = number;
    }
    else if (key_total_count == 2)
    {
        if (key_data == timer_and_hum)
        {
            key_new = fast_test_key;
        }
        else if (key_data == hum_and_clothes)
        {
            key_new = Child_key;
        }
        else
        {
            key_new = 0;
        }
    }
    else
    {
        key_new = 0;
        shake_count = 0;
        M_Key_last = 0;
        return;
    }
    /////////////////////////////////////////////////////////////////////////////////
    if (key_new != key_old)
    {
        key_old = key_new;
        shake_count = 0;
        return;
    }

    if (shake_count < 3)
    {
        shake_count++;
    }
    //���
    /////////////////////////////////////////////////////////////////////////////////
    if ((M_Key_last >= 500) && (M_Key_last <= 600) && (key_new == fast_test_key))
    {
        G_Key_Number = fast_test_key;
        M_Key_last = 0xffff;
        Read_key_delay = 100;
        _KEY_OK = 1;
    }

    //ͯ����Ч    �����O���I"��"�����O���I"3��
    if ((M_Key_last >= 300) && (M_Key_last <= 400) && (key_new == Child_key))
    {
        G_Key_Number = Child_key;
        shake_count = 0xff;   //�ɿ��̰���ִ��
        M_Key_last = 0xffff;  //��ס
        Read_key_delay = 100; //��ϰ�������ʱ��ȡ����
        _KEY_OK = 1;          //������Ч
    }

    // UVC ��   �L�������r�O���I��5��
    // if ((M_Key_last >= 500) && (M_Key_last <= 600) && (key_new == set_timer_key))
    // {
    //     G_Key_Number = UVC_key;
    //     shake_count = 0xff;
    //     M_Key_last = 0xffff;
    //     Read_key_delay = 100; //��ϰ�������ʱ��ȡ����
    //     _KEY_OK = 1;
    // }

    // PCB�����Լ������  �L�������¡�3��
    if (Enter_Test_Judge(key_new))
    {
        G_Key_Number = SELF_TEST_KEY;
        shake_count = 0xff;
        M_Key_last = 0xffff;
        Read_key_delay = 100; //��ϰ�������ʱ��ȡ����
        _KEY_OK = 1;
    }

    //�L��"�Դ�I"5��
    // if ((M_Key_last >= 500) && (M_Key_last <= 600) && (key_new == power_key))
    // {
    //     if ((Child_Lock_status == ENABLE) || (Sys_Err.Water_Full == ENABLE))
    //     {
    //         return;
    //     }
    //     M_Key_last = 0xffff;
    //     shake_count = 0xff;
    //     Read_key_delay = 100; //��ϰ�������ʱ��ȡ����
    //     Buzz_Time = BUZZ_long_time;
    //     _wifi_reset_En = 1; //wifiģ�鸴λ/����ģʽ�л�
    //     _KEY_OK = 1;
    // }

    /*
    ����wifiģ���������ж�
    ���C��B��ǰ5���,�L��5��l��һ��Wifiģ����ģʽָ��
    �B�Ӯa�z�~̖�ɹ���,Wifiָʾ���c��,,�p�˟��@ʾ��̖����.�������ģ�K�~̖��Ϣ�K�C�_�֏�Ĭ�J�O��.
    �˳��l��:1.���Դ�I�_�C����
            2.�M��a�zģʽ�r�g���^3���.
    ���Sģʽ�r,��̖���60,���c��Wifiָʾ��.
    */
    if ((WIFI_Self_Test_delay_time) && (key_new == dry_key) && (SYS_Power_Status == OFF))
    {
        WIFI_Self_Test_delay_time = 500;
        if (M_Key_last >= 500)
        {
            WIFI_Self_Test_delay_time = 0;
            M_Key_last = 0xffff;
            shake_count = 0xff;
            Read_key_delay = 100; //��ϰ�������ʱ��ȡ����
            Buzz_Time = BUZZ_long_time;
            SYSData_Rest();         //д��ϵͳĬ�ϲ���
            _Wifi_factory_test = 1; // wifi����ʹ��
        }
    }
}

/*************************************************
 // ��������    : key_decode
 // ��������    : ��������
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void key_decode(void)
{
    static unsigned char SYS_Hum_Point = 255; //ʪ���趨��
    unsigned char i = 0, j = 0;
    UI08 key_num = 0;

    key_num = Get_Key_Data();

    if (Power_Delay_Time > 0)
    {
        key_num = 0x00; //�ڼ䰴����Ч
        return;
    }
    if (M__Self_Test)
    {
        return;
    }

    if (!_KEY_OK)
    {
        return;
    }
    _KEY_OK = 0;

    Disp_Delay = 10;

    if ((Child_Lock_status == ENABLE) && (key_num != Child_key))
    {
        if (Child_Lock_Disp_Count == 0)
        {
            Key_ERR_Buzz_Cnt = 1;
            Child_Lock_Disp_Count = Child_Lock_Disp_NUM;
            Child_Lock_Disp_timer = 50;
            Child_Lock_1s_Count = 0;
        }
        return;
    }

    if (key_num != SELF_TEST_KEY)
    {
        if ((Sys_Err.Water_Full == ENABLE) ||
            (SYS_Power_Status == OFF && M_Timer_Run == 0 && key_num != power_key &&
             key_num != set_timer_key && key_num != Child_key && key_num != fast_test_key))
        {
            Key_ERR_Buzz_Cnt = 3;
            return;
        }
    }

    switch (key_num)
    {
    case power_key:
    {
        Buzz_Time = BUZZ_long_time;
        if (_Fast_Test)
        {
            _Fast_Test = DISABLE;
            break;
        }
        if (SYS_Power_Status == ON)
        {
            Turn_Off();
            Disp_Delay = 0;
            Disp_set_Delay = 0;
        }
        else
        {
            Turn_On();
            fan_force_runtime = 180;
        }

        Fan_Speed_delay = 0;
    }
    break;
    case set_timer_key:
    {
        Disp_set_Delay = 10;
        Buzz_Time = BUZZ_short_time;
        if (M_Timer_Setting_Time > 0)
        {
            M_Timer_Setting_Time = 5;
            _Timer_set_ok = 1;
            if (M_Timer_Run > 0)
            {
                M_Timer_Setting_Time = 0;
                _Timer_set_ok = 0;
                M_Timer_Run = 0;
            }
            else
            {

                if (++M_Timer_Point >= sizeof(Timer_SET))
                {
                    M_Timer_Point = 0;
                }
                //
                M_Timer_Buf = Timer_SET[M_Timer_Point];
                //
                if (M_Timer_Buf == 0)
                {
                    M_Timer_Setting_Time = 0;
                    _Timer_set_ok = 0;
                    M_Timer_Set = 1;
                    M_Timer_Buf = 1;
                    M_Timer_Point = 0;
                    M_Timer_Run = 0;
                }
            }
        }
        else
        {
            if ((M_Timer_Run > 0) && (SYS_Power_Status == OFF))
            {
                M_Timer_Setting_Time = 0;
                _Timer_set_ok = 0;
                M_Timer_Run = 0;
                Disp_Delay = 0;
                Disp_set_Delay = 0;
            }
            else
            {
                M_Timer_Setting_Time = 5;
                if (M_Timer_Run)
                {
                    _Timer_set_ok = 0;
                } //������ڶ�ʱ  �����¸�ֵ��ʱ
                else
                {
                    _Timer_set_ok = 1;
                }
            }
        }

        if (Set_SYS_Hum_timer > 0)
        {
            Set_SYS_Hum_timer = 0;
            SYS_Hum_Set = SYS_Hum_Set_Buf;
        }
    }
    break;

    case dry_key: //��ʪ
    {
        if (SYS_Mode_Buf != mode_SYS_HUM) //����ģʽת����
        {
            SYS_Mode_Buf = mode_SYS_HUM;
            Set_SYS_Mode_Timer = 20;
            Buzz_Time = BUZZ_short_time;
        }
        if (!Set_SYS_Hum_timer) //�Ȼ���
        {
            Set_SYS_Hum_timer = 50;
            Buzz_Time = BUZZ_short_time;
            return;
        }
        Set_SYS_Hum_timer = 50;
        Buzz_Time = BUZZ_short_time;
        SET_SYS_HUN_Tyde_Timer = 20;

        j = SYS_Hum_Set_Buf - SYS_Hum_Set_Buf % 10; //��ֹWIFI�·���ʪ���趨�ݶȺͲ�����Ĳ�ͬ
        if (SYS_HUN_Tyde_Buf == CONTINUOUS_HUM) //SYS_Hum_Pointͬ��, ����ң�ش���ûͬ��
        {
            SYS_Hum_Point = 0;
        }
        else
        {
            for (i = 0; i < sizeof(Hum_SET); i++)
            {
                if (Hum_SET[i] == j)
                {
                    SYS_Hum_Point = i;
                    break;
                }
            }
        }

        if (++SYS_Hum_Point >= sizeof(Hum_SET))
        {
            SYS_Hum_Point = 0;
            SYS_HUN_Tyde_Buf = CONTINUOUS_HUM;
        }
        else
        {
            SYS_Hum_Set_Buf = Hum_SET[SYS_Hum_Point];
            SYS_HUN_Tyde_Buf = USER_DEFINE_HUM;
        }
    }
    break;

    case Dry_Clothes_key: //�������
    {
        Buzz_Time = BUZZ_short_time;
        if (SYS_Mode_Buf != mode_DRY_Clothes)
        {
            Set_SYS_DYR_Tyde_timer = 20;
            SYS_Mode_Buf = mode_DRY_Clothes;
            Set_SYS_Mode_Timer = 20;
            SYS_DYR_Tyde_Buf = DYR_STRONG;
            SYS_DYR_Tyde = DYR_STRONG;
        }
    }
    break;
    case Child_key:
    {
        Rest_Key_Buzzer();
        if (Child_Lock_status == DISABLE)
        {
            Child_Lock_status = ENABLE;
            Child_Lock_Disp_Count = Child_Lock_Disp_NUM;
            Child_Lock_Disp_timer = 50;
        }
        else
        {
            Child_Lock_status = DISABLE;
            Child_Lock_Disp_Count = 1;
            Child_Lock_Disp_timer = 50;
        }
        Child_Lock_1s_Count = 0;
        Buzz_Time = BUZZ_short_time;
        //             M_write_delay_time=2;
    }
    break;
    case UVC_key:
    {
        if (SYS_Power_Status == OFF)
        {
            return;
        }
        _SYS_UVC_Status ^= 1;
        Disp_set_Delay = 10;
        Buzz_Time = BUZZ_short_time;
    }
    break;
    case LAMP_key:
    {
        if (LED_Disp_Status == ENABLE)
        {
            LED_Disp_Status = DISABLE;
            Disp_Delay = 0;
            Disp_set_Delay = 0;
        }
        else
        {
            LED_Disp_Status = ENABLE;
            Disp_set_Delay = 10;
        }
        Buzz_Time = BUZZ_short_time;
    }
    break;
    case fast_test_key: //���
    {
        _Fast_Test = ENABLE;
        Buzz_Time = BUZZ_short_time;
        G_Disp_Machine_Temp_Time = 10;
    };
    break;
    case SELF_TEST_KEY:
    {
        //Ϊ��ֹ�û��󴥷�, ����PCB�����Լ�ʱ, ͭ�ܺ�������Ҫ�ε�, ˮ������
        if (Sys_Err.temp_room_err && Sys_Err.temp_coil_err && Sys_Err.Water_Full == DISABLE)
        {
            Buzz_Time = BUZZ_long_time;
            M__Self_Test = 1;
            //�Լ�д��ϵͳĬ�ϲ���
            SYSData_Rest();
        }
    }
    break;
    default: break;
    }
}

/*************************************************
 // ��������    : Key_Deal
 // ��������    : ��������
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Key_Deal(void)
{
    prg_ms10_key();
    prg_mS100_key();
    prg_s_key();
    get_key_number();

    if (M__Self_Test)
        return;
    
    key_decode();
}

/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
