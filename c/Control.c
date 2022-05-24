#include "General.h"

//����ýй©����
MCU_xdata bool_f sys_reg__ec_err = FALSE;                          // EC����
MCU_xdata bool_f sys_reg__ec_60 = FALSE;                           // EC����60����
MCU_xdata bool_f sys_reg__comp_protect_stause_condition_a = FALSE; //��ýй©����2������aѹ��������
MCU_xdata bool_f sys_reg__comp_protect_stause_condition_b = FALSE; //��ýй©����2������bѹ��������
MCU_xdata UI16 sys_reg_comp_38_count_condition_a = 0;              //��ýй©����2������aʱ�����
MCU_xdata UI16 sys_reg_comp_38_count_condition_b = 0;              //��ýй©����2������bʱ�����

MCU_xdata UI08 g_ec_count = 0;       // EC��ʱ����
MCU_xdata UI16 g_ec_count_timer = 0; // EC��ʱ����
MCU_xdata Pump_str Pump_type;        // PUMP����
MCU_xdata RUN_REG Run_Reg;           // run����
/* Private function prototypes -----------------------------------------------*/
//=========�����߼���Ҫ��ȫ�ֱ���
/* Private functions ---------------------------------------------------------*/
MCU_xdata UI16 SYS_RUN_timer = 0;                //ϵͳ����ʱ��
MCU_xdata UI16 SYS_UP_Feel_Disp_timer = 180;     //�ϵ�ʱ��
MCU_xdata FuncState test_factory = DISABLE;      //��������
MCU_xdata FuncState Child_Lock_status = DISABLE; //ͯ��
MCU_xdata UI08 Child_Lock_Disp_Count = 0;        //ͯ����ʾ
MCU_xdata UI08 Child_Lock_1s_Count = 0;          //ͯ����ʱ
MCU_xdata UI16 Child_Lock_Disp_timer = 0;        //ͯ����ʾʱ��

MCU_xdata UI08 Key_ERR_Buzz_Cnt = 0; //������

MCU_xdata FuncState Pump_Status = DISABLE;           //�Զ�ˮ�ó�ˮ�����Ƿ��
MCU_xdata FuncState Pump_water_pipe_Status = ENABLE; //��ˮˮ���Ƿ����

MCU_xdata UI08 Feel_Mode = 1;       //ʪ�ȸ�֪����
MCU_xdata UI08 Dump_SW_H_count = 0; //��̨�㵹����״̬�ۼƱ���
MCU_xdata UI08 Dump_SW_L_count = 0; //��̨�㵹����״̬�ۼƱ���
MCU_xdata UI08 Dump_SW = 0;         //��̨�㵹����״̬
// MCU_xdata UI08 Dump_Status=0;//��̨�㵹���
MCU_xdata FuncState Fan_set_enable = ENABLE; //����趨ʹ��
MCU_xdata UI16 Fan_Speed_delay = 0;          //�����ʱ
MCU_xdata UI08 filter_Minute_Count = 0;      //������ʱ

//
MCU_xdata UI08 hum_err_updata_delay_time = 180;  //ʪ�ȴ����������ϱ���ʱ
MCU_xdata UI08 temp_err_updata_delay_time = 180; //�¶ȴ����������ϱ���ʱ

MCU_xdata UI16 fan_force_runtime = 180;

//////
MCU_xdata sEC_struct sEC_SYS;
MCU_const UI08 HUM_Feel_Code[][2] = {
    {0, 40},   // 35
    {39, 70},  // 55
    {69, 100}, // 85
};

void Ec_1s_Control(void);
void Comp_Protect_1S_Control_Condition_A(void);
void Comp_Protect_1S_Control_Condition_B(void);
void Pump_S_general(void);
/*************************************************
 // ��������    : Run_reg_init
 // ��������    : ������ʼ��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Run_reg_init(void)
{
    UI08 i = 0;
    Pump_type.on_timer = 0;
    Pump_type.off_timer = 0;
    Pump_type.on_count = 0;
    Pump_type.mode = 0;
    Pump_type.SW_ON_Timer = 0;
    Pump_type.SW_OFF_Timer = 0;
    Pump_type.off_delay = 0;

    Run_Reg.HI_stay_time = 0;
    Run_Reg.LO_stay_time = 0;
    Run_Reg.def_start_time = 0;
    Run_Reg.def_start_count_enable = 0;
    Run_Reg.def_time = 0;
    Run_Reg.def_cont = 0;
    Run_Reg.temp_room_point = 0;
    Run_Reg.def_type = TYPE_A;
    Run_Reg.sensor_err = FALSE;
}
/*************************************************
 // ��������    : Pump_100ms_general
 // ��������    : 100msʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Pump_100ms_general(void)
{
#if 0
   if(pump_sw_para.Status == DI_SHORT)
   {
       Pump_type.SW_ON_Timer=0;
       if(Pump_type.SW_OFF_Timer<0xff)
       {
          Pump_type.SW_OFF_Timer++;
       }

   }
   else
   {
       Pump_type.SW_OFF_Timer=0;
       if(Pump_type.SW_ON_Timer<0xff)
       {
          Pump_type.SW_ON_Timer++;
       }
   }
#endif
}

/*************************************************
 // ��������    : prg_100ms_control
 // ��������    : 100msʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_100ms_control(void)
{
    if (!_ms100_for_conl)
    {
        return;
    }
    _ms100_for_conl = 0;

    Pump_100ms_general();

    if (Dump_SW)
    {
        if (Dump_SW_L_count < 0xff)
        {
            Dump_SW_L_count++;
        }
        Dump_SW_H_count = 0;
    }
    else
    {
        if (Dump_SW_H_count < 0xff)
        {
            Dump_SW_H_count++;
        }
        Dump_SW_L_count = 0;
    }
}

/*************************************************
 // ��������    : prg_s_control
 // ��������    : Sʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_s_control(void)
{
    UI16 filter_en_timer = 0;

    if (!_s_for_conl)
    {
        return;
    }
    _s_for_conl = 0;

    if (hum_err_updata_delay_time) //ʪ�ȴ����������ϱ���ʱ
    {
        hum_err_updata_delay_time--;
    }
    if (temp_err_updata_delay_time) //�¶ȴ����������ϱ���ʱ
    {
        temp_err_updata_delay_time--;
    }

    if (Fan_Speed_delay > 0)
    {
        Fan_Speed_delay--;
    }

    if ((SYS_RUN_timer < 0xffff) && (SYS_Power_Status == ON))
    {
        SYS_RUN_timer++;
    }
    else
    {
        SYS_RUN_timer = 0;
    }

    if (SYS_UP_Feel_Disp_timer < 0xffff)
    {
        SYS_UP_Feel_Disp_timer++;
    }

    if ((Run_Reg.def_start_count_enable) && (M_Defrost_status == FALSE))
    {
        Run_Reg.def_start_time++;
    }
    //
    if (M_Defrost_status)
    {
        Run_Reg.def_time++;
    }
    else
    {
        Run_Reg.def_time = 0;
    }

    if ((fan_force_runtime > 0) && (Power_Delay_Time == 0))
    {
        fan_force_runtime--;
    }
    //
    Ec_1s_Control();
    Comp_Protect_1S_Control_Condition_A();
    Comp_Protect_1S_Control_Condition_B();
    //
    if (_Fast_Test == ENABLE)
    {
        filter_Minute_Count = 60;
        filter_en_timer = 250;
    }
    else
    {
        filter_en_timer = 15000;
    }
    //
    //����
    if (++filter_Minute_Count < 60)
    {
        return;
    }
    filter_Minute_Count = 0;
    //
    if (Fan_Speed_Out != OFF_FAN)
    {
        if (SYS_filter_time <= 0Xffff)
        {
            SYS_filter_time++;
        }
        //
        if (SYS_filter_time % 60 == 0) //ÿһСʱ��¼һ������ʱ��
        {
            SYS_filter_time_buf = SYS_filter_time;
        }
    }
    //
    if (SYS_filter_time >= filter_en_timer) //������ϴʱ��
    {
        SYS_filter_time = 0;
        SYS_filter_time_buf = SYS_filter_time;
    }
}
/*************************************************
 // ��������    : prg_minute_control
 // ��������    : minʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_minute_control(void)
{
    UI08 buf = 0;
    if (!_Control_minute)
    {
        return;
    }
    _Control_minute = 0;
    /*if((Hum_para.value>=90)&&(Comp_para.OUT))
    {
      if(Run_Reg.HI_stay_time<0xffff)
        Run_Reg.HI_stay_time++;
    }*/

    if (_Fast_Test) //����ж�
    {
        buf = 60;
    }
    else
    {
        buf = 1;
    }
    //ʪ�ȴ�������·�ж�����
    if ((Hum_para.AD_value >= 975) // ADֵ�ж�
        && (Comp_para.OUT))        //ѹ��������
    {
        if (Run_Reg.HI_stay_time < (0xffff - buf))
        {
            Run_Reg.HI_stay_time = Run_Reg.HI_stay_time + buf;
        }
        else
        {
            Run_Reg.HI_stay_time = 0xffff;
        }
    }
    //ʪ�ȴ�������·�ж�����
    if ((Hum_para.AD_value <= 56)    // ADֵ�ж�
        && (SYS_Power_Status == ON)) //�жϻ�̨�Ƿ񿪻�
    {
        if (Run_Reg.LO_stay_time < (0xffff - buf))
        {
            Run_Reg.LO_stay_time = Run_Reg.LO_stay_time + buf;
        }
        else
        {
            Run_Reg.LO_stay_time = 0xffff;
        }
    }
}
/*************************************************
 // ��������    : Fan_Run_Check
 // ��������    : ���
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Fan_Run_Check(void)
{
    if (SYS_Power_Status == ON)
    {
        // M_Fan_Speed=(M_Fan_Speed==FANOFF)?(fan_set):(M_Fan_Speed);
        Fan_Speed_Out_Buf = HIGH_FAN; //�û�̨���ٲ����趨  �ܸ���
        Comp_para.BUF = OFF;
    }
}
/*************************************************
 // ��������    : Ec_Protect_Check
 // ��������    : EC����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Ec_Protect_Check(void)
{
    UI08 g_dt = 0; //

    if (sys_reg__ec_err)
    {
        Fan_Run_Check();
        return;
    }
    //
    if (Temp_room_para_C > Temp_coil_para_C)
    {
        g_dt = Temp_room_para_C - Temp_coil_para_C;
    }
    else
    {
        g_dt = Temp_coil_para_C - Temp_room_para_C;
    }
    //
    if ((g_dt > 5) || (Comp_para.BUF == OFF))
    {
        g_ec_count = 0;
        g_ec_count_timer = 0;
        sys_reg__ec_60 = FALSE;
        return;
    }
    //
    if (Comp_para.on_time > 1200) // 1200
    {
        if (g_dt <= 5)
        {
            if (g_ec_count_timer == 0)
            {
                g_ec_count_timer = (20 + 60); // ��������Ӽ��
                g_ec_count_timer = g_ec_count_timer * 60;
            }
        }
    }
    //
    if ((g_ec_count_timer <= 3600) && (g_ec_count_timer > 0))
    {
        sys_reg__ec_60 = TRUE;
        Fan_Run_Check();
    }
    else
    {
        sys_reg__ec_60 = FALSE;
    }
}
/*************************************************
 // ��������    : Ec_Protect_Deal
 // ��������    : EC����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Ec_Protect_Deal(void)
{
    Ec_Protect_Check();
}
/*************************************************
 // ��������    : Ec_1s_Control
 // ��������    : Sʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Ec_1s_Control(void)
{
    if (0 == g_ec_count_timer)
    {
        return;
    }
    if (_EC_Fast_Test)
    {
        g_ec_count_timer = (g_ec_count_timer > 60) ? (g_ec_count_timer - 60) : (0);
    }
    else
    {
        g_ec_count_timer--;
    }

    if (g_ec_count_timer == 0)
    {
        g_ec_count++;
    }

    if (g_ec_count >= 3)
    {
        sys_reg__ec_err = TRUE;
    }
}
/*************************************************
 // ��������    : Comp_Protect_Condition_A
 // ��������    :
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Comp_Protect_Condition_A(void)
{
    if (sys_reg__comp_protect_stause_condition_a == FALSE)
    {
        return;
    }
    Fan_Run_Check();
}
/*************************************************
 // ��������    : Comp_Protect_1S_Control_Condition_A
 // ��������    : Sʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Comp_Protect_1S_Control_Condition_A(void)
{
    UI08 l_comp_protect_dou = 0;
    if (sys_reg_comp_38_count_condition_a == 0xffff)
    {
        return;
    }
    l_comp_protect_dou = (_EC_Fast_Test) ? (60) : (1);

    sys_reg_comp_38_count_condition_a += l_comp_protect_dou;

    if (sys_reg__comp_protect_stause_condition_a)
    {
        if (sys_reg_comp_38_count_condition_a > 90 * 60) // 40����  ��Ϊ90
        {
            sys_reg__comp_protect_stause_condition_a = FALSE;
            sys_reg_comp_38_count_condition_a = 0;
        }
        return;
    }

    if ((Temp_room_para_C >= (27 + 15)) && (Comp_para.OUT)) // 30�ȸ�Ϊ27��
    {
        if (sys_reg_comp_38_count_condition_a >= 360 * 60) // 180���Ӹ�Ϊ360����
        {
            if ((Temp_room_para_C > Temp_coil_para_C) && ((Temp_room_para_C - Temp_coil_para_C) >= 25))
            {
                sys_reg__comp_protect_stause_condition_a = TRUE;
                sys_reg_comp_38_count_condition_a = 0;
            }
            else
            {
                sys_reg_comp_38_count_condition_a = 0xffff;
            }
        }
    }
    else
    {
        sys_reg_comp_38_count_condition_a = (sys_reg_comp_38_count_condition_a > l_comp_protect_dou) ? (0xffff) : (0);
    }
}
/*************************************************
 // ��������    : Comp_Protect_Condition_B
 // ��������    : Sʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Comp_Protect_Condition_B(void)
{
    if (sys_reg__comp_protect_stause_condition_b == FALSE)
    {
        return;
    }
    Fan_Run_Check();
}
/*************************************************
 // ��������    : Comp_Protect_1S_Control_Condition_B
 // ��������    : Sʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Comp_Protect_1S_Control_Condition_B(void)
{
    UI08 l_comp_protect_dou = 0;
    if (sys_reg_comp_38_count_condition_b == 0xffff)
    {
        return;
    }
    l_comp_protect_dou = (_EC_Fast_Test) ? (60) : (1);

    sys_reg_comp_38_count_condition_b += l_comp_protect_dou;

    if (sys_reg__comp_protect_stause_condition_b)
    {
        if (sys_reg_comp_38_count_condition_b > 90 * 60)
        {
            sys_reg__comp_protect_stause_condition_b = FALSE;
            sys_reg_comp_38_count_condition_b = 0;
        }
        return;
    }

    if ((Temp_room_para_C >= (38 + 15)) && (Comp_para.OUT))
    {
        if (sys_reg_comp_38_count_condition_b >= 360 * 60)
        {
            sys_reg__comp_protect_stause_condition_b = TRUE;
            sys_reg_comp_38_count_condition_b = 0;
        }
    }
    else
    {
        sys_reg_comp_38_count_condition_b = (sys_reg_comp_38_count_condition_b > l_comp_protect_dou) ? (0xffff) : (0);
    }
}
/*************************************************
 // ��������    : defost_enb
 // ��������    : �����߼�
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void defost_enb(void)
{
    UI08 a;
    M_Defrost_status = TRUE;
    Run_Reg.def_start_time = 0;
    Run_Reg.def_start_count_enable = FALSE;
    if (Run_Reg.def_type == TYPE_A)
    {
        a = 5;
    }
    else
    {
        a = 4;
    }
    if (++Run_Reg.def_cont > a)
    {
        Run_Reg.def_cont = 1;
    }
}
/*************************************************
 // ��������    : defost_enb
 // ��������    : �����߼�
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void def_start_jude(bool_f condition1, bool_f condition2)
{
    // condition1 ����30���Ӽ�ʱ����
    // condition2 30���Ӽ�ʱ��;��������
    UI16 x, a;
    if (condition1)
    {
        Run_Reg.def_start_count_enable = TRUE;
    }
    if (condition2)
    {
        Run_Reg.def_start_count_enable = FALSE;
        Run_Reg.def_start_time = 0;
    }
    if (Run_Reg.def_start_time < 2)
    {
        if (Temp_room.C_value > 15) //�¶�̧�� 9   9Ϊ0��
        {
            Run_Reg.temp_room_point = Temp_room.C_value - 15;
        }
        else
        {
            Run_Reg.temp_room_point = 0;
        }
    } // 30���ӿ�ʼ��ʱʱ��׽����
    //
    if ((Run_Reg.temp_room_point < defrost_para7) || (Run_Reg.sensor_err))
    {
        Run_Reg.def_type = TYPE_B;
        a = 19 * 60;
    }
    else
    {
        Run_Reg.def_type = TYPE_A;
        a = 24 * 60;
    }
    //
    if ((Run_Reg.def_start_time == defrost_para4) || (Run_Reg.def_start_time == a))
    {
        if (!condition1)
        {
            Run_Reg.def_start_count_enable = FALSE;
            Run_Reg.def_start_time = 0;
        }
    }
    //
    // x=defrost_para6;
    x = a + 60; // 30����
    if (_Fast_Test)
    {
        x /= 60;
    }
    if (Run_Reg.def_start_time >= x)
    {
        defost_enb();
    }
}
/*************************************************
 // ��������    : defrost_logic
 // ��������    : �����߼�
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void defrost_logic(void)
{
    UI16 def_end_time, x;
    bool_f condition1, condition2;
    if (Sys_Err.Water_Full)
    {
        Run_Reg.def_cont = 0;
    }
    if ((SYS_Power_Status == OFF) || (Sys_Err.Water_Full))
    {
        Run_Reg.def_start_time = 0;
        Run_Reg.def_time = 0;
        M_Defrost_status = FALSE;
        M__Fan_set_enable = 1;
        return;
    }
    //===================
    if (M_Defrost_status == FALSE) //�ϵ���һ��ѹ��������10�����ڲ����г�˪�ж�
    {
        M__Fan_set_enable = 1;
        /*x=defrost_para1;
        if(Sys_Reg._Fast_Test)
           x/=60;
        if(Sys_Reg.Power_first_on==FALSE)*/
        x = 1;

        //��˪��ʼ�����ж�
        if (Sys_Err.temp_coil_err)
        {
            condition1 = (bool_f)(Comp_para.OUT); //ͭ�ܹ���ѹ��������ʱֱ�ӽ����˪�г�2013-3-7
                                                  // &&(Temp_room.value<defrost_para12);//Troom<15

            condition2 = (bool_f)(Comp_para.OUT == OFF); // Troom>=16
            def_start_jude(condition1, condition2);
        }
        //�޹��ϳ�˪
        else
        {
            condition1 = (bool_f)((Comp_para.on_time >= x) && (Temp_coil.AD_value <= defrost_para2)); // coil<0?
            condition2 = (bool_f)((Temp_coil.AD_value >= defrost_para3) || (Comp_para.OUT == OFF));   // coil>=20
            def_start_jude(condition1, condition2);
        }
    }
    else
    {
        //��˪����
        Comp_para.BUF = OFF;
        Fan_Speed_Out_Buf = HIGH_FAN;
        M__Fan_set_enable = 0;
        //��˪���������ж�
        if (Run_Reg.def_type == TYPE_A)
        {
            x = 5;
            def_end_time = defrost_para8;
        }
        else
        {
            x = 4;
            def_end_time = defrost_para9;
        }
        if (Run_Reg.def_cont == x)
        {
            def_end_time = defrost_para11;
        }
        if (_Fast_Test)
        {
            def_end_time /= 60;
        }
        if (Run_Reg.def_time >= def_end_time)
        {
            M_Defrost_status = FALSE;
        }
    }
}

/*************************************************
 // ��������    : Water_Dump_protect_deal
 // ��������    : ��ˮ/��̨�㵹/ͨ�Ź���
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Water_protect_deal(void)
{
    if (water_full_para.cut_time >= 30)
    {
        if (Sys_Err.Water_Full == DISABLE)
        {
            Buzz_Cnt = 5;
        }
        Sys_Err.Water_Full = ENABLE;
        fan_force_runtime = 0;
        Comp_para.BUF = OFF;
        Fan_Speed_Out_Buf = OFF_FAN;

        SYS_RUN_timer = 0;
    }
    else if (water_full_para.short_time >= 10)
    {
        if (Sys_Err.Water_Full == ENABLE)
        {
            fan_force_runtime = 180;
        }
        Sys_Err.Water_Full = DISABLE;
        Buzz_Cnt = 0;
        Fan_Speed_delay = 0;
    }
}
/*************************************************
 // ��������    : Sys_err_deal
 // ��������    : ���ϴ���
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
#define HUM_ERR_TIMER 480
void Sys_err_deal(void)
{
    if ((SYS_Power_Status) && (!M_Defrost_status))
    {
        if (Sys_Err.temp_room_err)
        {
            Comp_para.BUF = ON;
        }

    } //���´���������·��ʪ�ȴ���������·ʱ��ѹ����������������

    if (Temp_coil.status != AI_NORMAL) //����·
    {
        Sys_Err.temp_coil_err = ENABLE;
    }
    else
    {
        Sys_Err.temp_coil_err = DISABLE;
    }

    if ((Sys_Err.temp_room_err) || (Sys_Err.temp_coil_err))
    {
        Run_Reg.sensor_err = TRUE; //��˪���
    }
    else
    {
        Run_Reg.sensor_err = FALSE;
    }
}
/*************************************************
 // ��������    : protect_logic
 // ��������    : �����߼�
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void protect_logic(void)
{
    Sys_err_deal();
    defrost_logic();
    Ec_Protect_Deal();
    Comp_Protect_Condition_A();
    Comp_Protect_Condition_B();
    Water_protect_deal();
}

/*************************************************
 // ��������    : load_set
 // ��������    : �������
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void load_set(void)
{
    if (Comp_para.BUF == ON)
    {
        if ((Comp_para.off_time < Comp_Protect_Time) && (Comp_para.OUT == OFF) && (!_Fast_Test))
        {
            Comp_para.OUT = OFF;
        }
        else
        {
            if (Comp_para.OUT == OFF)
            {
                Comp_para.off_time = 0;
            }
            Comp_para.OUT = ON;
        }
    }
    else
    {
        if ((Comp_para.on_time < Comp_Protect_Time) && (Comp_para.OUT == ON) && (SYS_Power_Status) && (water_full_para.Status == DI_SHORT)) // 20140807
        {
            Comp_para.OUT = ON;
        }
        else
        {
            if (Comp_para.OUT == ON)
            {
                Comp_para.on_time = 0;
            }
            Comp_para.OUT = OFF;
        }
    }

    Fan_Speed_Out = Fan_Speed_Out_Buf;
    if (Fan_Speed_Out == OFF_FAN) //����ر�ʱ����������������ʱ��
    {
        FAN_ON_timer = 0;
    }

    UVC_para.OUT = UVC_para.BUF;
    if (Fan_Speed_Out != OFF_FAN)
    {
        Swing_Tyde_Out = Swing_Tyde_Out_Buf;
    }
    else
    {
        Swing_Tyde_Out = STOP_SWING;
    }
}

/*************************************************
 // ��������    : HUM_Mode
 // ��������    : ��ʪģʽ ������ģʽ�����L�R�_�Ե����\�D
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void HUM_Mode(void)
{
    if (SYS_HUN_Tyde == CONTINUOUS_HUM) //������ת
    {
        Comp_para.BUF = ON;
    }
    else if (Hum_para.value >= SYS_Hum_Set + 5)
    {
        Comp_para.BUF = ON;
    }
    else if (Hum_para.value + 5 <= SYS_Hum_Set)
    {
        Comp_para.BUF = OFF;
    }

    //���
    if ((Comp_para.OUT == ON)       //ѹ������תʱ
        || (fan_force_runtime > 0)) //ÿ�ΰ���Դ����������ʱ������������ˮ���������֮����ǿ�ư��趨������ת3����
    {
        Fan_Speed_Out_Buf = LOW_FAN;
    }
    else
    {
        Fan_Speed_Out_Buf = OFF_FAN;
    }

    Set_Swing_State = Enable;
    Swing_Tyde_Out_Buf = SYS_Swing_Tyde;
    Set_Fan_State = ENABLE;
}

/*************************************************
 // ��������    : DRY_Clothes_Mode
 // ��������    : ����ģʽ
 // ��ڲ���    : ��
 // ���ڲ���    : ��

***************************************************/
void DRY_Clothes_Mode(void)
{
    Comp_para.BUF = ON;
    Set_Fan_State = DISABLE;
    Fan_Speed_Out_Buf = HIGH_FAN;
}
/*************************************************
 // ��������    : void UVC_DC_MOTOR_Control(void)
 // ��������    : UVC ����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void UVC_Control(void)
{
    if (_SYS_UVC_Status == ON)
    {
        if (Fan_Speed_Out_Buf != OFF_FAN) //�ֶ�������ʱ��ָʾ�Ʊ��ֲ��䵫�������������ͣ
        {
            UVC_para.BUF = ON;
        }
        else
        {
            UVC_para.BUF = OFF;
        }
    }
    else
    {
        UVC_para.BUF = OFF;
    }
}
/*************************************************
 // ��������    : Sys_Control
 // ��������    :
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Sys_Control(void)
{
    prg_100ms_control();
    prg_s_control();
    prg_minute_control();
    //
    if (M__Self_Test)
    {
        return;
    }
    //
    if (SYS_Power_Status == OFF)
    {
        Fan_Speed_Out_Buf = OFF_FAN;
        Comp_para.BUF = OFF;
    }
    else if (SYS_Mode == mode_DRY_Clothes)
    {
        DRY_Clothes_Mode();
    }
    else if (SYS_Mode == mode_SYS_HUM)
    {
        HUM_Mode();
    }
    protect_logic();
    UVC_Control(); //
    load_set();
}
