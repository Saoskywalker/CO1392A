#include "General.h"

//======================ȫ�ֱ���=================================
const UI16 comp_on_target_time[3]= {comp_on_target_time_define};
const UI16 comp_off_target_time[3]= {comp_off_target_time_define};
//======================ȫ�ֱ���=================================
xdata UI08   R_Dry_Status;		//��ʪ����
xdata UI08   R_Freeze_Status;		//����������
xdata UI08   R_Fan_Ontime;		//�����ʱ��
xdata UI16   R_Heat_run_time;		//��������ʱ��
xdata UI16   R_T_coil_first;		//���²�����־
xdata UI08   R_Tcoil_L_para4C_time;	//��˪����2��ʱ
xdata UI16   R_defreeze2_runtime;	//��˪ʱ��
xdata UI16   R_def_run_time;		//����������ʱ��
xdata UI08   R_coolair_status;	//��ֹ�������
xdata UI08   Coil_Temp;		//����
xdata UI08   R_overload_status;	//��������
xdata UI08   Room_Temp;		//����
xdata UUI08  Control_bit;		//��־λ
//===================PUMP������ر���============================
xdata UI08    	PUMP_IO_value;                //PUMP������״̬
xdata UI16    	PUMP_pwm_width;               //PUMP_PWM����
xdata UI08    	SPEED_buf_point;              //��������
xdata UI16    	SPEED_width_BUF[SPEED_BUF_MAX];//�ٶȻ�������
xdata UI08 	Feedback_Duration;		//��������ʱ��
xdata UI08 	Low_Duration;			//�͵�ƽ����ʱ��
//===================PUMP���б���============================
xdata UI08  	Pump_Water_Step;           	//��ˮ�߼�
xdata UI16  	Pump_Off_delay_time;       	//��ˮ��ˮλ�ָ������󣬴�ˮ��������ˮ��ʱ��
xdata UI16  	Water_Full_Time;           	//������ˮʱ��
xdata UI08  	Judge_water_Full_delay_time; 	//��ˮ�����ӳ��ж�ʱ��(��ˮ��δ���밴����3S,����İ�25S)
xdata UUI08 	DcPump_bit;              	//��־λ
//===================���±���============================
xdata UI16   High_Temp_protect_time;	//���±����ӳټ��ʱ��
xdata UI08   protect_type;		//���±�������
xdata UI08   _Flash_for_high_temp;
//===================off_cycle============================
xdata UI16   off_cycle_Tmin;         	//off cycle����T
xdata UI16   off_cycle_Tmin_Dsip_timer;	//off cycle����T��ʾʱ��
xdata UI08   PowerKey_off_cycle_Time;	//������������ʱ��
xdata UI08   off_cycle_mode;//�׶�
xdata UI16   off_cycle_timer;//off_cycle��ʱ

xdata sEC_struct sEC_SYS;

//======================ȫ�ֱ���=================================
void EC_s_control(void);//ÿ��ִ��һ��
/***********************************************************
*�� �� ����  void control_data_intit(void)
*����������  �����߼�������ʼ��
*���������  ��
*��������ֵ����
***********************************************************/
void control_data_intit(void)
{
    UI08 i=0;
    R_Dry_Status=0;
    R_Freeze_Status=0;
    R_Fan_Ontime=0;
    R_Heat_run_time=0;
    R_T_coil_first=77;
    R_Tcoil_L_para4C_time=0;
    R_defreeze2_runtime=0;
    R_def_run_time=0;
    R_coolair_status=0;
    Coil_Temp=77;

    R_overload_status=0;
    Control_bit.byte=0;
    Room_Temp=77;
    High_Temp_protect_time=3600;
    protect_type=0;
    _Flash_for_high_temp=0;
    PUMP_IO_value=0;
    PUMP_pwm_width=0;
    SPEED_buf_point=0;
    Feedback_Duration=0;
    Low_Duration=0;
    Pump_Water_Step=0;
    Pump_Off_delay_time=0;
    Water_Full_Time=0;
    Judge_water_Full_delay_time=0;
    DcPump_bit.byte=0xff;

    for(i=0; i<SPEED_BUF_MAX; i++)
    {
        SPEED_width_BUF[i]=0;
    }

    off_cycle_mode=0;
    off_cycle_timer=0;
    off_cycle_Tmin=0;         	//off cycle����T
    off_cycle_Tmin_Dsip_timer=0;	//off cycle����T��ʾʱ��
    PowerKey_off_cycle_Time=60;	//������������ʱ��

    sEC_SYS.troom_c=0;
    sEC_SYS.tcoil_c=0;
    sEC_SYS.EC_count=0;
    sEC_SYS.timer_key_count=0;
    sEC_SYS.EC_protect_type=0;
    sEC_SYS.EC_protect_byte.byte=0;
    sEC_SYS.EC_count_timer=0;
    sEC_SYS.EC_comp_timer=0;

}
/***********************************************************
*�� �� ����  void prg_s_control(void)
*����������  ��ʱ��Ƭ
*���������  ��
*��������ֵ����
***********************************************************/
void prg_s_control(void)
{
    if(!_s_for_conl)
    {
        return;
    }
    _s_for_conl=0;

    prg_minute();

    if( (Lfan_para.OUT) || (Mfan_para.OUT) ||(Hfan_para.OUT) )
    {
        if(R_Fan_Ontime<0xff)
        {
            R_Fan_Ontime++;
        }
    }
    else
    {
        R_Fan_Ontime=0;
    }
    //==========
    if( (mode_set==HEAT) && (!_defrost) &&(Comp_para.OUT) )
    {
        if(R_Heat_run_time<0xffff)
        {
            R_Heat_run_time++;
        }
        if(R_Heat_run_time==defrost_para1)
        {
            R_T_coil_first=Coil_Temp;
        }
    }
    else if(_defrost)
    {
        R_Heat_run_time=1;
    }
    else
    {
        R_Heat_run_time=0;
    }
    //==========
    if(  (mode_set==HEAT)
            &&(R_Heat_run_time>defrost_para3)
            &&(Coil_Temp<defrost_para4  ))    //27C
    {
        if(R_Tcoil_L_para4C_time<0xff)
        {
            R_Tcoil_L_para4C_time++;
        }
    }
    else
        R_Tcoil_L_para4C_time=0;
    //
    //===========
    if((R_Freeze_Status==2)&&(Comp_para.OUT))
    {
        if(R_defreeze2_runtime<0xffff)
            R_defreeze2_runtime++;
    }
    else
        R_defreeze2_runtime=0;
    //===========
    if(_defrost)
    {
        R_def_run_time++;
        if(R_def_run_time>=defrost_para8)
        {
            _defrost=0;
            R_def_run_time=0;
        }
    }
    else
        R_def_run_time=0;

    EC_s_control();//ÿ��ִ��һ��

}
/***********************************************************
*�� �� ����  void prg_ms100_control(void)
*����������  100msʱ��Ƭ
*���������  ��
*��������ֵ����
***********************************************************/
void prg_ms100_control(void)
{
    if(!_ms100_for_control)
    {
        return;
    }
    _ms100_for_control=0;

    //��ˮ����ӳٹر�ʱ��
    if(Pump_Off_delay_time<0xffff)
    {
        Pump_Off_delay_time++;
    }
    //������ˮʱ��
    if(_Water_Full_status==DI_CUT)
    {
        if(Water_Full_Time<0xff)
        {
            Water_Full_Time++;
        }
    }
    else
    {
        Water_Full_Time=0;
    }


}


/***********************************************************
*�� �� ����  void off_cycle_mode_control(void)
*����������  ��ʱ��Ƭ
*���������  ��
*��������ֵ����
***********************************************************/
void   off_cycle_mode_control(void)
{
    switch (off_cycle_mode)
    {
    case 0:
    {
        fan_speed=LOW;
        if(off_cycle_timer>=off_cycle_Tmin)//���º󣬷����������off_cycle_Tmin����
        {
            off_cycle_mode=1;
            off_cycle_timer=0;
        }
    }
    break;
    case 1:
    {
        fan_speed=FANOFF;
        if(off_cycle_timer>=14*60)
        {
            off_cycle_mode=2;
            off_cycle_timer=0;
        }
    }
    break;
    case 2:
    {
        fan_speed=LOW;
        if(off_cycle_timer>=1*60)
        {
            off_cycle_mode=1;
            off_cycle_timer=0;
        }
    }
    break;
    }
}

/***********************************************************
*�� �� ����  void Clear_off_cycle_mode_Data(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/
void Clear_off_cycle_mode_Data(void)
{
    off_cycle_mode=0;
    off_cycle_timer=0;
}
/***********************************************************
*�� �� ����  void all_load_off(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/
void all_load_off(void)
{
    Comp_para.BUF=OFF;
    fan_speed=FANOFF;
    outfan_speed=FANOFF;
    Way4_para.BUF=OFF;
    Pump_para.BUF=OFF;
    DCPUMP_para.OUT=OFF;
    UVC_para.BUF=OFF;
}
/***********************************************************
*�� �� ����  void normal_cool(void)
*����������  ����ģʽ
*���������  ��
*��������ֵ����
***********************************************************/
void normal_cool(void)
{
    UI08 Ts=0;

    Ts=temp_set_f;
    if(Room_Temp>=Ts+2)//Tr>=Ts+1
    {
        Comp_para.BUF=ON;
    }

    if(Room_Temp+2<=Ts)//Tr<=Ts-1
    {
        Comp_para.BUF=OFF;
    }

    if(Comp_para.on_time>=2)
    {

        Pump_para.BUF=ON;
    }
    else
    {
        Pump_para.BUF=OFF;
    }

    Way4_para.BUF=OFF;

    //fan_speed=fan_set;
    if(Comp_para.OUT==ON)
    {
        off_cycle_timer=0;
        off_cycle_mode=0;
    }

    if((_Sys_lowpower==DISABLE)&&(Comp_para.OUT==OFF))
    // if(Comp_para.OUT==OFF)
    {
        off_cycle_mode_control();
    }
    else
    {
        fan_speed=fan_set;
    }
    _fan_set_enb=1;

    if(!_outfan_low_enable)
    {
        outfan_speed=HIGH;
    }
    else if(fan_speed==LOW)
    {
        outfan_speed=LOW;
    }
    else
    {
        outfan_speed=HIGH;
    }
}
/***********************************************************
*�� �� ����  void normal_dry(void)
*����������  ��ʪ
*���������  ��
*��������ֵ����
***********************************************************/
void normal_dry(void)
{
    switch (R_Dry_Status)
    {
    case 0:
    {
        if(Room_Temp<=77)//25C
        {
            R_Dry_Status=1;
        }
    }
    break;
    case 1:
    {
        if(Room_Temp<=72)//22C
        {
            R_Dry_Status=2;
        }
        else if(Room_Temp>=79)//26C
        {
            R_Dry_Status=0;
        }
    }
    break;
    case 2:
    {
        if(Room_Temp<=66)//19C
        {
            R_Dry_Status=3;
        }
        else if(Room_Temp>=73)//23C
        {
            R_Dry_Status=1;
        }
    }
    break;
    case 3:
    {
        if(Room_Temp>=68)//20C
        {
            R_Dry_Status=2;
        }
    }
    break;
    default:
    {
        ;
    }
    break;
    }
    if(0==R_Dry_Status)
    {
        Comp_para.BUF=ON;
    }
    else
    {
        if((Comp_para.on_time>=comp_on_target_time[R_Dry_Status-1])&&(Comp_para.OUT==ON))
        {
            Comp_para.BUF=OFF;
        }
        if((Comp_para.off_time>=comp_off_target_time[R_Dry_Status-1])&&(Comp_para.OUT==OFF))
        {

            Comp_para.BUF=ON;
        }
    }

    //
    if(Comp_para.on_time>=2)
    {
        Pump_para.BUF=ON;
    }
    else
    {
        Pump_para.BUF=OFF;
    }
    ////2014-07-09
    Way4_para.BUF=OFF;
    //
    outfan_speed=HIGH;
#if 0
    fan_speed=LOW;
    _fan_set_enb=DISABLE;

#endif

    //
    //==================================

#if 1
    if(R_Dry_Status!=0)
    {

        fan_speed=LOW;

        _fan_set_enb=0;
    }
    else
    {
        fan_speed=fan_set;
        _fan_set_enb=1;
    }
#endif

}
/***********************************************************
*�� �� ����  void normal_fan(void)
*����������  �ͷ�
*���������  ��
*��������ֵ����
***********************************************************/
void normal_fan(void)
{
    Comp_para.BUF=OFF;
    Pump_para.BUF=OFF;
    Way4_para.BUF=OFF;
    outfan_speed=FANOFF;
    fan_speed=fan_set;
    _fan_set_enb=1;
}
/***********************************************************
*�� �� ����  void normal_heat(void)
*����������  ů��
*���������  ��
*��������ֵ����
***********************************************************/
void normal_heat(void)
{
    UI16 Ts=0;
    if(_temp_update_enb)
    {
        Ts=temp_set_f;
        if(Room_Temp+2<=Ts)//Tr<=Ts-1
        {
            Comp_para.BUF=ON;
        }
        else if(Room_Temp>=Ts+2)//Tr>=Ts+1
        {
            Comp_para.BUF=OFF;
        }
    }

    Pump_para.BUF=OFF;

    if(!_Sys_lowpower)
    {
        Way4_para.BUF=ON;
    }
    else if(_comp_should_on)
    {
        Way4_para.BUF=ON;
    }
    else if(Comp_para.off_time>=300)
    {
        Way4_para.BUF=OFF;
    }
    //
    outfan_speed=HIGH;
    //

    if( (R_Heat_run_time>=15)
            ||(R_overload_status>0)
            ||(Room_Temp>=77) )  //25C
    {
        fan_speed=fan_set;
    }
    else
    {
        fan_speed=FANOFF;
    }


}
/***********************************************************
*�� �� ����  void temp_update_judge(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/
void temp_update_judge(void)
{
    Coil_Temp=Tcoil_value;
    Room_Temp=Troom_dsp;//ʹ����ʾ�¶����߼�
    //
    if((mode_set!=HEAT)||(!_compensate_enable))
    {
        _temp_update_enb=1;
        _comp_firston_in_heat=0;
        _heat_temp_compensation=0;
        return;
    }
    if((fan_speed!=FANOFF)&&(Comp_para.OUT==ON)&&(!_Mode_Change))
    {
        _comp_firston_in_heat=1;
    }
    //
    if(Troom_value>=32)
    {
        _heat_temp_compensation=1;   //����7F;
    }
    else
    {
        _heat_temp_compensation=0;
    }

    //Room_Temp=Troom_value-7;//ֱ�Ӳ�����ʾ
    //
    if((Comp_para.OUT==ON)&&(Comp_para.on_time<25))
    {
        _temp_update_enb=0;
    }
    else if((R_Heat_run_time)&&(R_Heat_run_time<15))
    {
        _temp_update_enb=0;
    }
    else
    {
        _temp_update_enb=1;
    }
}

/*************************************************
 // ��������    : void UVC_DC_MOTOR_Control(void)
 // ��������    : UVC ����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void UVC_DC_MOTOR_Control(void)
{
   if(SYS_UVC_Status==ON)
   {UVC_para.BUF=ON;}
   else
   {UVC_para.BUF=OFF;}
   //�ػ������ϡ�ˮ�� �ر�
}

/***********************************************************
*�� �� ����  void normal_logic(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/
void normal_logic(void)
{
    temp_update_judge();
    if(_Mode_Change)
    {
        return;
    }

    if((Mode_Set!=COOL)||(Power_Status==OFF))
    {
        off_cycle_timer=0;
        off_cycle_mode=0;
    }

    if(Power_Status==OFF)
    {
        all_load_off();
        _fan_set_enb=1;

        R_Heat_run_time=0;
        R_overload_status=0;

    }

    if(Power_para.on_time<2)//����2��󿪸��أ������������
    {
        return;
    }

   

    UVC_DC_MOTOR_Control();
    if(Mode_Set==COOL)
    {
        normal_cool();
    }
    else if(Mode_Set==DRY)
    {
        normal_dry();
    }
    else if(Mode_Set==FAN)
    {
        normal_fan();
    }
    else if(Mode_Set==HEAT)
    {
        normal_heat();
    }
}
//============================�����߼�============================
/***********************************************************
*�� �� ����  void defreeze_logic(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/
void defreeze_logic(void)
{
    UI16 freeze_temp=0;
    UI16 freeze_point1=0;
    UI16 freeze_point2=0;
    if(power_status==OFF)
    {
        return;
    }

    if((mode_set!=COOL)&&(mode_set!=DRY))
    {
        return;
    }
    if (_Tcoil_Enable)
    {
        freeze_temp=Coil_Temp;
        freeze_point1=defreeze_Tcoil1;
        freeze_point2=defreeze_Tcoil2;
    }
    else
    {
        freeze_temp=Room_Temp;
        freeze_point1=defreeze_Troom1;
        freeze_point2=defreeze_Troom2;
    }

    switch (R_Freeze_Status)
    {
    case 0:                           //����point1
    {
        if(freeze_temp<=freeze_point1)
        {
            R_Freeze_Status=1;
        }
    }
    break;
    case 1:                           //����point1 ����point1
    {
        if(_Tcoil_Enable)//�޹��»����޴�����
        {
            outfan_speed=LOW;
        }
        if(freeze_temp>=freeze_point1+2)
        {
            R_Freeze_Status=0;
        }
        else if(freeze_temp<=freeze_point2)
        {
            R_Freeze_Status=2;
        }
    }
    break;
    case 2:                           //����point2
    {
        //Pump_para.BUF=OFF;
        if(_Tcoil_Enable)//�޹��»����޴�����
        {
            outfan_speed=LOW;
        }
        if(freeze_temp>=freeze_point2+2)
        {
            R_Freeze_Status=1;
        }
        if(R_defreeze2_runtime>=defreeze2_runtime_const)
        {
            Comp_para.BUF=OFF;
            outfan_speed=FANOFF;
        }
    }
    break;
    default:
    {
        R_Freeze_Status=0;
    }
    break;

    }
}
/***********************************************************
*�� �� ����  void avoid_coolair_logic(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/
void avoid_coolair_logic(void)
{
    if(power_status==OFF)
    {
        R_coolair_status=0;
        return;
    }

    if(mode_set!=HEAT)
    {
        R_coolair_status=0;
        return;
    }

    if((Comp_para.OUT==OFF)||(R_Heat_run_time<=15))
    {
        _fan_set_enb=1;
    }
    else
    {
        _fan_set_enb=0;
    }



    //if(!_Sys_lowpower)
    //{
    if(Room_Temp>=78)
    {
        _fan_set_enb=1;
        return;
    }
    else if((Room_Temp>76)&&(R_coolair_status>0))
    {
        return;//���ڷ����״̬ʱ���¶Ȼز
    }
    //}



    switch(R_coolair_status)
    {
    case 0:
    {
        if(Coil_Temp<coolair_point1)
        {
            R_coolair_status=1;
        }
    }
    break;
    case 1:
    {
        if(fan_speed!=FANOFF)
        {
            fan_speed=LOW;
        }

        if(Coil_Temp>=coolair_point1+1)
        {
            R_coolair_status=0;
        }
        else if(Coil_Temp<=coolair_point3)
        {
            R_coolair_status=2;
        }
    }
    break;
    case 2:
    {
        fan_speed=FANOFF;
        if(Coil_Temp>=coolair_point3+1)
        {
            R_coolair_status=1;
        }
    }
    break;
    default:
    {
        R_coolair_status=0;
    }
    break;
    }


    if(R_coolair_status==0)
    {
        _fan_set_enb=1;
    }

    /* else
     {
        _fan_set_enb=DISABLE;
     }*/



}
/***********************************************************
*�� �� ����  void defrost_logic(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/
void defrost_logic(void)
{
    UI08 condition1=0;
    UI08 condition2=0;
    UI08 condition3=0;
    if((mode_set!=HEAT)||(power_status==OFF))
    {
        _defrost=0;
        return;
    }
    if(!_defrost)
    {
        condition1=(R_Heat_run_time>=defrost_para1)&&(Coil_Temp+defrost_para2<R_T_coil_first);   //10C
        condition2=(R_Tcoil_L_para4C_time>defrost_para5);
        condition3=(R_Heat_run_time>=defrost_para6)&&(Coil_Temp<defrost_para7);

        if(condition1||condition2||condition3)
        {
            _defrost=1;
        }
    }
    else
    {
        Comp_para.BUF=ON;
        Way4_para.BUF=OFF;
        outfan_speed=HIGH;
        fan_speed=FANOFF;
    }
}


/*************************************************
 // ��������    : Water_Full_check
 // ��������    : ��ˮ״̬�ж�
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Water_Full_check(void)
{
    #if 0
//������ˮ�ܻ���,���߰γ���ˮ��(����ͨ��ˮ3S����)
//����ˮ�ܵĻ���(����ͨ��ˮ25S����)
    if(_water_pipe_Status==0)//������ˮ�ܻ���
    {
        _water_pipe_Status_old=0;
        WATER_FULL_TIME=30;
    }// 100ms*30=3S
    else//����ˮ�ܵĻ���
    {
        if((_water_pipe_Status_old==0)&&(_Water_Full))//��ˮ�ڼ����²���ˮ�ܣ����¼�ʱ��ˮ��ʱ
        {
            _Water_Full=0;
            Buzz_Cnt=0xff;
            Water_Full_Time=0;
        }
        _water_pipe_Status_old=1;
        WATER_FULL_TIME=250;
    }
    #endif
//============================================================
//======   ��ˮ״̬�ж�
//============================================================
    if(_Water_Full_status==DI_SHORT)
    {
        //.���Mˮ������,�z�y���Mˮ�_�P�]��,������PUMP�^�m��ˮ15�� ��ֹͣ.
        //if(_Water_Full)
        //{
        //    Pump_Off_delay_time=0xffff;
        //}
        _Water_Full=0;
        Buzz_Cnt=0xff;
    }
    else//����ˮ��
    {
        //if((Water_Full_Time+30)>=WATER_FULL_TIME)//_Water_Full_status=DI_CUT, ��Դ���Ѿ���ʱ3S
        //{
            if(!_Water_Full)
            {
                Buzz_Cnt=0;   //������diʮ��
            }
            _Water_Full=1;
        //}
    }
}


/*************************************************
 // ��������    : DcPump_deal
 // ��������    : ��ˮ����߼�
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void DcPump_deal(void)
{
    //==========================================================================================
    //===== ������ˮ�ܻ���,���߰γ���ˮ��,�����д�ˮ����߼�   =================================
    //��һ�r��,�z�y�����ٽ��^�Ĺ̶����γ�(�Żɿ��ؔ��_),PUMPֹͣ�\�D. ====>(!_water_pipe_Status)
    //���Mˮ������,�z�y�����ٽ��^�γ����ٴβ���̶����t�ٴ��M�벽�Ea ===>((_Water_Full)&&(!_water_pipe_Status))
    //����ˮ��ȷ��Ҳ�����д�ˮ�߼�����ע:�ڲ���ˮ������·���ˮ��,��ˮ���������ת25S
    //���û�н����ˮ״̬,����̨ͣ������)
    //====================================================================================
    if((!_water_pipe_Status)||(_Water_Full))// 1.û����ˮ��  2.��ˮ������û��ˮ��
    {
        Pump_Water_Step=0;
        DCPUMP_para.BUF=OFF;
        Pump_Off_delay_time=0;
        return;//�����д�ˮ�߼�
    }
    //��ˮ����߼�
    switch(Pump_Water_Step)
    {
    //��ʼ״̬
    case 0:
    {
        DCPUMP_para.BUF=OFF;
        //�ȴ�������ˮ
        if(_Water_Full_status==DI_CUT)//������ˮ(�ڵ�Դ���Ѽ�ʱ3S)
        {
            Pump_Water_Step=1;
        }
    };
    break;
    //��ʼ��ˮ
    case 1:
    {
        DCPUMP_para.BUF=ON;
        if(_Water_Full_status==DI_SHORT)//ˮλ����������
        {
            Pump_Off_delay_time=0;
            Pump_Water_Step=2;
        }
    };
    break;
    //ˮ�����ػָ�����,��ˮ��������ˮʱ��
    case 2:
    {
        DCPUMP_para.BUF=ON;
        if(Pump_Off_delay_time>=PUMP_PG)//�������Ĵ�ˮ���15S/����������35S
        {
            if(_Pump_Water_PG_Status)//�������Ĵ�ˮ���(��ˮ�Ƚ���,������15S����)
            {
                Pump_Water_Step=0;
            }
            else
            {
                if(Pump_Off_delay_time>=PUMP_NO_PG)//�޷����ʹ�ˮ���(��ˮû��ô��,��35S)
                {
                    Pump_Water_Step=0;
                }
            }
        }
    }
    break;
    //�쳣
    default:
    {
        Pump_Water_Step=0;
        DCPUMP_para.BUF=OFF;
        Pump_Off_delay_time=0;
    }
    break;
    }
}


/*********************************************************
������: void Get_the_pump_pwm_width(void)
��  ��: ��ȡPUMP����������
����ֵ: ��
����ֵ: ��
**********************************************************/
void Get_the_pump_pwm_width(void)
{
    //PUMP������
    UI16  width=0;

    PUMP_pwm_width++;//��ʱ

    if((INFAN_IO)&&(PUMP_IO_value==0))//�����ش���
    {
        width=PUMP_pwm_width;//��������ʱ��
        PUMP_pwm_width=0;

        SPEED_buf_point++;//װ������
        if(SPEED_buf_point>=SPEED_BUF_MAX)
        {
            SPEED_buf_point=0;
        }
        SPEED_width_BUF[SPEED_buf_point]=width;
        PUMP_IO_value=1;
    }


    //ͬ��IO��״̬
    if(INFAN_IO==0)
    {
        PUMP_IO_value=0;
    }

}

/*********************************************************
������: void Get_PUMP_model(void)
��  ��: ��ȡPUMP����
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
void Get_PUMP_model(void)
{
    UI32 buf=0;
    UI08 i=0;

    if(DCPUMP_para.on_time==0)//PUMP����ʱ���
    {
        return;
    }

    if(!_ms100_for_PUMP)
    {
        return;
    }
    _ms100_for_PUMP=0;

    for(i=0; i<SPEED_BUF_MAX; i++) //ȡƽ��ֵ
    {
        buf+=SPEED_width_BUF[i];
    }
    buf=buf/SPEED_BUF_MAX;

    if(buf>10)//�з���
    {
        Low_Duration=0;
        Feedback_Duration++;
        if(Feedback_Duration>50)//����5���з���
        {
            Feedback_Duration=0;
            _Pump_Water_PG_Status=1;
        }
    }
    else
    {
        Feedback_Duration=0;
        Low_Duration++;
        if(Low_Duration>50)
        {
            Low_Duration=0;
            _Pump_Water_PG_Status=0;
        }
    }
    for(i=0; i<SPEED_BUF_MAX; i++)
    {
        SPEED_width_BUF[i]=0;

    }

}

/***********************************************************
*�� �� ����  void overload_protect(void)
*����������  ����
*���������  ��
*��������ֵ����
***********************************************************/
void overload_protect(void)
{
    if(power_status==OFF)
    {
        return;
    }
    if(mode_set!=HEAT)
    {
        R_overload_status=0;
        return;
    }

    if(((Room_Temp>80)||(Comp_para.OUT==OFF))
            &&(R_overload_status!=3) )
    {
        return;
    }
    switch(R_overload_status)
    {
    case 0:
    {
        if(Coil_Temp>=overload_point1+1)
        {
            R_overload_status=1;
        }
    }
    break;
    case 1:
    {
        outfan_speed=LOW;
        if(Coil_Temp>=overload_point2+1)
        {
            R_overload_status=2;
        }
        else if(Coil_Temp<=overload_point1-1)
        {
            R_overload_status=0;
        }
    }
    break;
    case 2:
    {
        outfan_speed=FANOFF;

        if(Coil_Temp>=overload_point3+1)
        {
            R_overload_status=3;
        }
        else if(Coil_Temp<=overload_point2-1)
        {
            R_overload_status=1;
        }
    }
    break;
    case 3:
    {
        Comp_para.BUF=OFF;
        outfan_speed=FANOFF;
        if(Coil_Temp<=overload_point3-1)
        {
            R_overload_status=2;
        }
    }
    break;
    default:
    {
        R_overload_status=0;
    }
    break;
    }

}

/***********************************************************
*�� �� ����  void Sys_err_deal(void)
*����������  ���ϴ���
*���������  ��
*��������ֵ����
***********************************************************/
void Sys_err_deal(void)
{
    Get_PUMP_model();
    DcPump_deal();
    Water_Full_check();

    if(temp_room_status!=AI_NORMAL)
    {
        _temp_room_err=1;
    }
    else
    {
        _temp_room_err=0;
    }
    if(temp_coil_status!=AI_NORMAL)
    {
        _temp_coil_err=1;
    }
    else
    {
        _temp_coil_err=0;
    }

    if( (temp_room_status!=AI_NORMAL)||
            (temp_coil_status!=AI_NORMAL)||
            (_Water_Full)||
            (_INFAN_ERR)||
            (_OUTFAN_ERR)||
            (communication_ERR))
    {
        if((Power_Status==ON)&&(!_DISP_En2))
        {
            _DISP_En=1;
            _DISP_En2=1;
        }
        //else  if((Power_Status==ON)&&(DISP_En2==TRUE)&&(DISP_En==FALSE))
        //{
        //   DISP_En2=FALSE;
        //}
        Turn_Off();
        all_load_off();
        _sys_err=1;
    }
    else
    {
        _sys_err=0;
    }
}
/***********************************************************
*�� �� ����  void Fan_Run_check(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/
void Fan_Run_check(void)
{
    if(Power_Status==ON)
    {
        fan_speed=(fan_speed==FANOFF)?(fan_set):(fan_speed);
        Comp_para.BUF=OFF;

        if(Comp_para.off_time>=2)//15
        {
            Way4_para.BUF=OFF;
        }

        Pump_para.BUF=OFF;
    }
}


/***********************************************************
*�� �� ����  void EC_s_control(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/
void EC_s_control(void)//ÿ��ִ��һ��
{
    UI08 num=0;

    if(_EC_Fast_Test)
    {
        num=60;
    }
    else
    {
        num=1;
    }
    //
    if(off_cycle_timer<(0xffff-num))
    {
        off_cycle_timer=off_cycle_timer+num;
    }
    //
    if(sEC_SYS.EC_count_timer>num)
    {
        sEC_SYS.EC_count_timer=sEC_SYS.EC_count_timer-num;
    }
    else
    {
        sEC_SYS.EC_count_timer=0;
    }

    if(Comp_para.OUT==ON)
    {
        if(sEC_SYS.EC_comp_timer<(0Xffff-num))
        {
            sEC_SYS.EC_comp_timer=sEC_SYS.EC_comp_timer+num;
        }
        else
        {
            sEC_SYS.EC_comp_timer=0xffff;
        }
    }
    else
    {
        sEC_SYS.EC_comp_timer=0;
    }
    if(High_Temp_protect_time>num)
    {
        High_Temp_protect_time=High_Temp_protect_time-num;
    }
    else
    {
        High_Temp_protect_time=0;
    }
}


/***********************************************************
*�� �� ����  void EC_protect_deal(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/
void EC_protect_deal(void) //���ڿ����߼��б����߼�֮��
{
    UI08 dt=0;

    if(Temp_room.value>32)
    {
        sEC_SYS.troom_c=F_C(Temp_room.value);
    }
    else
    {
        sEC_SYS.troom_c=0;
    }

    if(Temp_coil.value>32)
    {
        sEC_SYS.tcoil_c=F_C(Temp_coil.value);
    }
    else
    {
        sEC_SYS.tcoil_c=0;
    }

    if(_EC_err)
    {
        Fan_Run_check();
        return;
    }

    dt=ABSOLUTE_VALUE(sEC_SYS.troom_c,sEC_SYS.tcoil_c);

    if(  (dt>5)||
            (Comp_para.BUF==OFF)||
            (_defrost) )
    {
        sEC_SYS.EC_count=0;
        sEC_SYS.EC_count_timer=0;
        sEC_SYS.EC_protect_type=0;
        sEC_SYS.EC_comp_timer=0;
        return;
    }

    switch(sEC_SYS.EC_protect_type)
    {
    case 0:
    {
        if(sEC_SYS.EC_comp_timer>(20*60))//20����
        {
            sEC_SYS.EC_protect_type=1;
            sEC_SYS.EC_count_timer=20*60;
        }
    }
    break;
    case 1:
    {
        if(sEC_SYS.EC_count_timer==0)
        {
            sEC_SYS.EC_protect_type=2;
            sEC_SYS.EC_count_timer=60*60;
        }
    }
    break;
    case 2:
    {
        Fan_Run_check();
        if(sEC_SYS.EC_count_timer==0)
        {
            sEC_SYS.EC_protect_type=0;
            sEC_SYS.EC_count++;
            if(sEC_SYS.EC_count>=3)
            {
                _EC_err=1;
            }
        }
    }
    }
}
/***********************************************************
*�� �� ����  void High_Temp_protect_logic(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/
void High_Temp_protect_logic(void)
{
    UI08 temp_buf=0;
    UI08 target_temp1=0;
    UI08 target_temp2=0;


    if(power_status==OFF)
    {
        _high_temp_protect_flag=0;
        High_Temp_protect_time=3600;
        return;
    }

    if(Mode_Set==FAN)
    {
        _high_temp_protect_flag=0;
        return;
    }

    if(High_Temp_protect_time>0)
    {
        return;
    }

    if(CF_Status==_C)
    {
        temp_buf=F_C(Temp_room.value);
        target_temp1=42;
        target_temp2=43;
    }
    else
    {
        temp_buf=Temp_room.value;
        target_temp1=108;
        target_temp2=109;
    }

    switch(protect_type)
    {
    case 0:
    {
        if(temp_buf>=target_temp2)//����43
        {
            protect_type=1;
        }
        _high_temp_protect_flag=0;
    }
    break;
    case 1:
    {
        if(temp_buf<=target_temp1)//С��42
        {
            protect_type=0;
        }
        _high_temp_protect_flag=1;
        all_load_off();
    }
    break;
    }
}
/***********************************************************
*�� �� ����  void protect_logic(void)
*����������  �����߼�
*���������  ��
*��������ֵ����
***********************************************************/
void protect_logic(void)
{
    defreeze_logic();
    avoid_coolair_logic();
    defrost_logic();
    overload_protect();
    EC_protect_deal();
    High_Temp_protect_logic();

    Sys_err_deal();
}
/***********************************************************
*�� �� ����  void protect_logic(void)
*����������  �������
*���������  ��
*��������ֵ����
***********************************************************/
void load_set(void)
{
    if(Power_Status)
    {
        Power_para.OUT=ON;
    }
    else
    {
        Power_para.OUT=OFF;
    }

    if(_Mode_Change)
    {
        _comp_should_on=0;
        Comp_para.OUT=OFF;
        _Mode_Change=0;
    }
    else if(Comp_para.BUF==ON)
    {
        if((Comp_para.off_time<Comp_Protect_Time)&&(!_comp_should_on))
        {
            _comp_should_on=0;
        }
        else
        {
            _comp_should_on=1;
        }
    }
    else
    {
        if(Comp_para.OUT==ON)
        {
            Comp_para.off_time=0x00;
            Comp_para.on_time=0x00;
        }
        Comp_para.OUT=OFF;
        _comp_should_on=0;
    }

    //==
    if(_comp_should_on)
    {
        if(!_Sys_lowpower)
        {
            Comp_para.OUT=ON;
        }
        else if(mode_set==COOL)
        {
            Comp_para.OUT=ON;
        }
        else if(mode_set==HEAT)
        {
            if(Way4_para.on_time>2)
            {
                Comp_para.OUT=ON;
            }
        }
        else
        {
            Comp_para.OUT=ON;   //��ʪ�ͷ�ģʽ
        }
    }


    //==========================
    //
    //2016/3/29
    if(Comp_para.OUT==ON)
    {
        if(mode_set==COOL)
        {
            light_down_cont=cool_light_down_time;
        }
        else if((mode_set==HEAT)&&(fan_speed!=FANOFF))
        {
            light_down_cont=heat_light_down_time;
        }
        else
        {
            light_down_cont=0;
        }
    }
/*
    if(   (light_down_cont==0)
            &&(R_overload_status==0)
            &&(Comp_para.OUT==OFF)
            &&(_Sys_lowpower)
            &&((mode_set==COOL)||(mode_set==HEAT)) )
    {
        fan_speed=FANOFF;
    }
*/

/*
    if( (light_down_cont>0)
//		&&(fan_speed!=FANOFF)
            &&(Comp_para.OUT==OFF)
            &&(_Sys_lowpower)
            &&(mode_set==HEAT))
    {
        fan_speed=LOW;
        Lfan_para.OUT=ON;
        Mfan_para.OUT=OFF;
        Hfan_para.OUT=OFF;
    }
    else */
    if(fan_speed==FANOFF)
    {
        Lfan_para.OUT=OFF;
        Mfan_para.OUT=OFF;
        Hfan_para.OUT=OFF;
    }
    else if(fan_speed==LOW)
    {
        Lfan_para.OUT=ON;
        Mfan_para.OUT=OFF;
        Hfan_para.OUT=OFF;
    }
    else if(fan_speed==MID)
    {
        Lfan_para.OUT=OFF;
        Mfan_para.OUT=ON;
        Hfan_para.OUT=OFF;
    }
    else
    {
        Lfan_para.OUT=OFF;
        Mfan_para.OUT=OFF;
        Hfan_para.OUT=ON;
    }


    //==========================
    if((outfan_speed==FANOFF)||(Comp_para.OUT==OFF))
    {
        OutLfan_para.OUT=OFF;
        OutHfan_para.OUT=OFF;
    }
    else if(outfan_speed==LOW)
    {
        OutLfan_para.OUT=ON;
        OutHfan_para.OUT=OFF;
    }
    else if(outfan_speed==HIGH)
    {
        OutLfan_para.OUT=OFF;
        OutHfan_para.OUT=ON;
    }


    //
    Way4_para.OUT=Way4_para.BUF;

    Pump_para.OUT=Pump_para.BUF;
    DCPUMP_para.OUT=DCPUMP_para.BUF;

    if((_led_swing_enable)&&(fan_speed!=FANOFF))
    {
        _led_swing_out=1;
    }
    else
    {
        _led_swing_out=0;
    }

    if(DCPUMP_para.OUT==ON)
    {
        DCPUMP_ON;
    }
    else
    {
        DCPUMP_OFF;
    }

    //TODO �����ʱ  UVC�Ű��趨����  
    if(fan_speed==FANOFF)
    {
        UVC_para.OUT=OFF;
    }
    else//�����ʱ  UVC�Ű��趨����
    {
        UVC_para.OUT=UVC_para.BUF;
    }
}
/***********************************************************
*�� �� ����  void Sys_Control(void)
*����������  �������
*���������  ��
*��������ֵ����
***********************************************************/
void Sys_Control(void)
{

    if((Sys_EEP_SYTP==0xff)&&(!_Self_Test))
    {
        return;
    }

    if(_Self_Test)
    {
        return;
    }

    prg_ms100_control();
    prg_s_control();

    normal_logic();
    protect_logic();
    load_set();
}





