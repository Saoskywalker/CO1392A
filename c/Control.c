#include "General.h"

//======================全局变量=================================
const UI16 comp_on_target_time[3]= {comp_on_target_time_define};
const UI16 comp_off_target_time[3]= {comp_off_target_time_define};
//======================全局变量=================================
xdata UI08   R_Dry_Status;		//除湿索引
xdata UI08   R_Freeze_Status;		//防冻结索引
xdata UI08   R_Fan_Ontime;		//风机打开时间
xdata UI16   R_Heat_run_time;		//制热运行时间
xdata UI16   R_T_coil_first;		//管温采样标志
xdata UI08   R_Tcoil_L_para4C_time;	//除霜条件2计时
xdata UI16   R_defreeze2_runtime;	//除霜时间
xdata UI16   R_def_run_time;		//防冻结运行时间
xdata UI08   R_coolair_status;	//防止冷风索引
xdata UI08   Coil_Temp;		//管温
xdata UI08   R_overload_status;	//过载索引
xdata UI08   Room_Temp;		//室温
xdata UUI08  Control_bit;		//标志位
//===================PUMP反馈相关变量============================
xdata UI08    	PUMP_IO_value;                //PUMP反馈口状态
xdata UI16    	PUMP_pwm_width;               //PUMP_PWM脉宽
xdata UI08    	SPEED_buf_point;              //数组索引
xdata UI16    	SPEED_width_BUF[SPEED_BUF_MAX];//速度缓存数组
xdata UI08 	Feedback_Duration;		//反馈持续时间
xdata UI08 	Low_Duration;			//低电平持续时间
//===================PUMP运行变量============================
xdata UI08  	Pump_Water_Step;           	//打水逻辑
xdata UI16  	Pump_Off_delay_time;       	//打水至水位恢复正常后，打水马达继续打水的时间
xdata UI16  	Water_Full_Time;           	//发生满水时间
xdata UI08  	Judge_water_Full_delay_time; 	//满水故障延迟判断时间(排水管未插入按正常3S,插入的按25S)
xdata UUI08 	DcPump_bit;              	//标志位
//===================高温保护============================
xdata UI16   High_Temp_protect_time;	//高温保护延迟检测时间
xdata UI08   protect_type;		//高温保护索引
xdata UI08   _Flash_for_high_temp;
//===================off_cycle============================
xdata UI16   off_cycle_Tmin;         	//off cycle参数T
xdata UI16   off_cycle_Tmin_Dsip_timer;	//off cycle参数T显示时间
xdata UI08   PowerKey_off_cycle_Time;	//参数允许设置时间
xdata UI08   off_cycle_mode;//阶段
xdata UI16   off_cycle_timer;//off_cycle计时

xdata sEC_struct sEC_SYS;

//======================全局变量=================================
void EC_s_control(void);//每秒执行一次
/***********************************************************
*函 数 名：  void control_data_intit(void)
*功能描述：  控制逻辑变量初始化
*输入参数：  无
*函数返回值：无
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
    off_cycle_Tmin=0;         	//off cycle参数T
    off_cycle_Tmin_Dsip_timer=0;	//off cycle参数T显示时间
    PowerKey_off_cycle_Time=60;	//参数允许设置时间

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
*函 数 名：  void prg_s_control(void)
*功能描述：  秒时间片
*输入参数：  无
*函数返回值：无
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

    EC_s_control();//每秒执行一次

}
/***********************************************************
*函 数 名：  void prg_ms100_control(void)
*功能描述：  100ms时间片
*输入参数：  无
*函数返回值：无
***********************************************************/
void prg_ms100_control(void)
{
    if(!_ms100_for_control)
    {
        return;
    }
    _ms100_for_control=0;

    //打水马达延迟关闭时间
    if(Pump_Off_delay_time<0xffff)
    {
        Pump_Off_delay_time++;
    }
    //发生满水时间
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
*函 数 名：  void off_cycle_mode_control(void)
*功能描述：  秒时间片
*输入参数：  无
*函数返回值：无
***********************************************************/
void   off_cycle_mode_control(void)
{
    switch (off_cycle_mode)
    {
    case 0:
    {
        fan_speed=LOW;
        if(off_cycle_timer>=off_cycle_Tmin)//达温后，风机低速运行off_cycle_Tmin分钟
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
*函 数 名：  void Clear_off_cycle_mode_Data(void)
*功能描述：
*输入参数：  无
*函数返回值：无
***********************************************************/
void Clear_off_cycle_mode_Data(void)
{
    off_cycle_mode=0;
    off_cycle_timer=0;
}
/***********************************************************
*函 数 名：  void all_load_off(void)
*功能描述：
*输入参数：  无
*函数返回值：无
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
*函 数 名：  void normal_cool(void)
*功能描述：  冷气模式
*输入参数：  无
*函数返回值：无
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
*函 数 名：  void normal_dry(void)
*功能描述：  除湿
*输入参数：  无
*函数返回值：无
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
*函 数 名：  void normal_fan(void)
*功能描述：  送风
*输入参数：  无
*函数返回值：无
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
*函 数 名：  void normal_heat(void)
*功能描述：  暖气
*输入参数：  无
*函数返回值：无
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
*函 数 名：  void temp_update_judge(void)
*功能描述：
*输入参数：  无
*函数返回值：无
***********************************************************/
void temp_update_judge(void)
{
    Coil_Temp=Tcoil_value;
    Room_Temp=Troom_dsp;//使用显示温度做逻辑
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
        _heat_temp_compensation=1;   //补偿7F;
    }
    else
    {
        _heat_temp_compensation=0;
    }

    //Room_Temp=Troom_value-7;//直接补偿显示
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
 // 函数名称    : void UVC_DC_MOTOR_Control(void)
 // 功能描述    : UVC 控制
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void UVC_DC_MOTOR_Control(void)
{
   if(SYS_UVC_Status==ON)
   {UVC_para.BUF=ON;}
   else
   {UVC_para.BUF=OFF;}
   //关机、故障、水满 关闭
}

/***********************************************************
*函 数 名：  void normal_logic(void)
*功能描述：
*输入参数：  无
*函数返回值：无
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

    if(Power_para.on_time<2)//开机2秒后开负载，步进电机除外
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
//============================保护逻辑============================
/***********************************************************
*函 数 名：  void defreeze_logic(void)
*功能描述：
*输入参数：  无
*函数返回值：无
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
    case 0:                           //高于point1
    {
        if(freeze_temp<=freeze_point1)
        {
            R_Freeze_Status=1;
        }
    }
    break;
    case 1:                           //低于point1 高于point1
    {
        if(_Tcoil_Enable)//无管温机型无此条件
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
    case 2:                           //低于point2
    {
        //Pump_para.BUF=OFF;
        if(_Tcoil_Enable)//无管温机型无此条件
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
*函 数 名：  void avoid_coolair_logic(void)
*功能描述：
*输入参数：  无
*函数返回值：无
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
        return;//处于放冷风状态时的温度回差。
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
*函 数 名：  void defrost_logic(void)
*功能描述：
*输入参数：  无
*函数返回值：无
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
 // 函数名称    : Water_Full_check
 // 功能描述    : 满水状态判定
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Water_Full_check(void)
{
    #if 0
//不带排水管机型,或者拔出排水管(按普通满水3S处理)
//带排水管的机型(按普通满水25S处理)
    if(_water_pipe_Status==0)//不带排水管机型
    {
        _water_pipe_Status_old=0;
        WATER_FULL_TIME=30;
    }// 100ms*30=3S
    else//带排水管的机型
    {
        if((_water_pipe_Status_old==0)&&(_Water_Full))//满水期间重新插入水管，重新计时满水超时
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
//======   满水状态判断
//============================================================
    if(_Water_Full_status==DI_SHORT)
    {
        //.若滿水警報後,檢測到滿水開關閉合,不執行PUMP繼續打水15秒 後停止.
        //if(_Water_Full)
        //{
        //    Pump_Off_delay_time=0xffff;
        //}
        _Water_Full=0;
        Buzz_Cnt=0xff;
    }
    else//发生水满
    {
        //if((Water_Full_Time+30)>=WATER_FULL_TIME)//_Water_Full_status=DI_CUT, 电源板已经计时3S
        //{
            if(!_Water_Full)
            {
                Buzz_Cnt=0;   //蜂鸣器di十声
            }
            _Water_Full=1;
        //}
    }
}


/*************************************************
 // 函数名称    : DcPump_deal
 // 功能描述    : 打水马达逻辑
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void DcPump_deal(void)
{
    //==========================================================================================
    //===== 不带排水管机型,或者拔出排水管,不进行打水马达逻辑   =================================
    //任一時刻,檢測到快速接頭從固定座拔出(磁簧开关斷開),PUMP停止運轉. ====>(!_water_pipe_Status)
    //若滿水警報後,檢測到快速接頭拔出後再次插入固定座則再次進入步驟a ===>((_Water_Full)&&(!_water_pipe_Status))
    //发生水满确认也不进行打水逻辑处理。注:在插入水管情况下发生水满,打水马达连续运转25S
    //如果没有解除满水状态,将机台停机处理)
    //====================================================================================
    if((!_water_pipe_Status)||(_Water_Full))// 1.没插排水管  2.满水发生且没插水管
    {
        Pump_Water_Step=0;
        DCPUMP_para.BUF=OFF;
        Pump_Off_delay_time=0;
        return;//不进行打水逻辑
    }
    //打水马达逻辑
    switch(Pump_Water_Step)
    {
    //初始状态
    case 0:
    {
        DCPUMP_para.BUF=OFF;
        //等待发生满水
        if(_Water_Full_status==DI_CUT)//发生满水(在电源板已计时3S)
        {
            Pump_Water_Step=1;
        }
    };
    break;
    //开始打水
    case 1:
    {
        DCPUMP_para.BUF=ON;
        if(_Water_Full_status==DI_SHORT)//水位开关已正常
        {
            Pump_Off_delay_time=0;
            Pump_Water_Step=2;
        }
    };
    break;
    //水满开关恢复正常,打水马达继续打水时间
    case 2:
    {
        DCPUMP_para.BUF=ON;
        if(Pump_Off_delay_time>=PUMP_PG)//带反馈的打水马达15S/不带反馈的35S
        {
            if(_Pump_Water_PG_Status)//带反馈的打水马达(打水比较猛,再运行15S即可)
            {
                Pump_Water_Step=0;
            }
            else
            {
                if(Pump_Off_delay_time>=PUMP_NO_PG)//无反馈型打水马达(打水没那么猛,需35S)
                {
                    Pump_Water_Step=0;
                }
            }
        }
    }
    break;
    //异常
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
函数名: void Get_the_pump_pwm_width(void)
描  述: 获取PUMP反馈的脉宽
输入值: 无
返回值: 无
**********************************************************/
void Get_the_pump_pwm_width(void)
{
    //PUMP反馈口
    UI16  width=0;

    PUMP_pwm_width++;//计时

    if((INFAN_IO)&&(PUMP_IO_value==0))//上升沿触发
    {
        width=PUMP_pwm_width;//拷贝脉宽时间
        PUMP_pwm_width=0;

        SPEED_buf_point++;//装载数据
        if(SPEED_buf_point>=SPEED_BUF_MAX)
        {
            SPEED_buf_point=0;
        }
        SPEED_width_BUF[SPEED_buf_point]=width;
        PUMP_IO_value=1;
    }


    //同步IO口状态
    if(INFAN_IO==0)
    {
        PUMP_IO_value=0;
    }

}

/*********************************************************
函数名: void Get_PUMP_model(void)
描  述: 获取PUMP机型
输入值: 无
输出值: 无
返回值: 无
**********************************************************/
void Get_PUMP_model(void)
{
    UI32 buf=0;
    UI08 i=0;

    if(DCPUMP_para.on_time==0)//PUMP运行时检测
    {
        return;
    }

    if(!_ms100_for_PUMP)
    {
        return;
    }
    _ms100_for_PUMP=0;

    for(i=0; i<SPEED_BUF_MAX; i++) //取平均值
    {
        buf+=SPEED_width_BUF[i];
    }
    buf=buf/SPEED_BUF_MAX;

    if(buf>10)//有反馈
    {
        Low_Duration=0;
        Feedback_Duration++;
        if(Feedback_Duration>50)//连续5秒有反馈
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
*函 数 名：  void overload_protect(void)
*功能描述：  过载
*输入参数：  无
*函数返回值：无
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
*函 数 名：  void Sys_err_deal(void)
*功能描述：  故障处理
*输入参数：  无
*函数返回值：无
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
*函 数 名：  void Fan_Run_check(void)
*功能描述：
*输入参数：  无
*函数返回值：无
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
*函 数 名：  void EC_s_control(void)
*功能描述：
*输入参数：  无
*函数返回值：无
***********************************************************/
void EC_s_control(void)//每秒执行一次
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
*函 数 名：  void EC_protect_deal(void)
*功能描述：
*输入参数：  无
*函数返回值：无
***********************************************************/
void EC_protect_deal(void) //放在控制逻辑中保护逻辑之后
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
        if(sEC_SYS.EC_comp_timer>(20*60))//20分钟
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
*函 数 名：  void High_Temp_protect_logic(void)
*功能描述：
*输入参数：  无
*函数返回值：无
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
        if(temp_buf>=target_temp2)//大于43
        {
            protect_type=1;
        }
        _high_temp_protect_flag=0;
    }
    break;
    case 1:
    {
        if(temp_buf<=target_temp1)//小于42
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
*函 数 名：  void protect_logic(void)
*功能描述：  保护逻辑
*输入参数：  无
*函数返回值：无
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
*函 数 名：  void protect_logic(void)
*功能描述：  负载输出
*输入参数：  无
*函数返回值：无
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
            Comp_para.OUT=ON;   //除湿送风模式
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

    //TODO 风机打开时  UVC才按设定运行  
    if(fan_speed==FANOFF)
    {
        UVC_para.OUT=OFF;
    }
    else//风机打开时  UVC才按设定运行
    {
        UVC_para.OUT=UVC_para.BUF;
    }
}
/***********************************************************
*函 数 名：  void Sys_Control(void)
*功能描述：  负载输出
*输入参数：  无
*函数返回值：无
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





