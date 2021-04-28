/********************************************************************************
  // 文件名称:   Judgeinput_device.c
  // 功能描述:   模块本身操作定义文件
*********************************************************************************/

#include "General.h"


void key_deal(void);
void prg_s_key(void);
UI08 Disp_Delay=0;//显示delay

const UI08 TempSet_range[4][2]=
{
    16,32,//cool C
    16,25,//heat C
    61,89,//cool F
    61,77 //heat F
};




/********************************************************************************
按键设置时，相应显示时间,如未开机设置风速，风速指示灯显示时间
********************************************************************************/
void prg_s_key(void)
{
    UI08 num=0;

    if(!_s_key)
    {
        return;
    }

    _s_key=0;

    if(Mode_Delay_Timer>0)
    {
        Mode_Delay_Timer--;
        if(Mode_Delay_Timer==0)
        {
            if(((Mode_Buf==HEAT)&&(Mode_Set!=HEAT))||((Mode_Buf!=HEAT)&&(Mode_Set==HEAT)))
            {
                _Mode_Change=1;
            }
            Mode_Set= Mode_Buf;

            if(Mode_Set==HEAT)
            {
                Temp_Set_F=Temp_Heat_F;
            }
            else
            {
                Temp_Set_F=Temp_Cool_F;
            }

            Temp_Set_C=F_C(Temp_Set_F);
            write_delay_time=EEP_DELAY;
            Temp_Setting_Time=0;
            Timer_Setting_Time=0;
            //2016/3/29
            if(Power_Status==ON)
            {
                // Dsp_Time=0;
                Fan_Delay_disp=0;
            }
            if(Mode_Set==COOL)
            {
                light_down_cont=cool_light_down_time;
            }
            //else if(Mode_Set==HEAT)
            //{
            //          light_down_cont=heat_light_down_time;
            //}
            else
            {
                light_down_cont=0;
            }

            Clear_off_cycle_mode_Data();
        }
    }
    //
    if(light_down_cont>0)
    {

        if(_EC_Fast_Test)
        {
            num=60;
        }
        else
        {
            num=1;
        }

        if(light_down_cont>=num)
        {
            light_down_cont=light_down_cont-num;
        }
        else
        {
            light_down_cont=0;
        }
    }



    /**************************************************************************/
    if(Dsp_Time>0)            //湿度设置显示等待时间
    {
        Dsp_Time--;
    }
    /**************************************************************************/
    if(Fan_Delay_Timer>0)              //风速设置确定等待时间
    {
        Fan_Delay_Timer--;
        if(Fan_Delay_Timer==0)
        {
            Fan_Set=Fan_Buf;
            write_delay_time=EEP_DELAY;
        }
    }
    if(Fan_Delay_disp>0)
    {
        Fan_Delay_disp--;
    }
    /**************************************************************************/
    if(Temp_Setting_Time>0)             //温度设置确定等待时间
    {
        Temp_Setting_Time--;
    }
    /**************************************************************************/
    if(Timer_Setting_Time>0)          //定时时间设置显示等待时间
    {
        Timer_Setting_Time--;
        if((Timer_Setting_Time==0)&&(_Timer_set_Flag==1))
        {
            Timer_set=Timer_buff;

            Timer_run=Timer_buff;
            Timer_run=Timer_run*3600;
        }
    }

    /**************************************************************************/
    if(Timer_run>0)
    {
        if(Timer_run%3600==0)//时间显示更新。当记满一个小时
        {
            Timer_buff=Timer_run/3600;
        }
        Timer_run--;
        if(Timer_run==0)     //定时开机 定时关机 时间计算
        {
            if(Power_Status==ON)
            {
                Turn_Off();
            }
            else
            {
                Turn_On();
            }
        }
    }

    if(Disp_Delay)
    {
        Disp_Delay--;
    }

}
/***********************************************************
*函 数 名：  void Turn_On(void)
*功能描述：  开机
*输入参数：  无
*函数返回值：无
***********************************************************/
void Turn_On(void)
{
    if((Power_Status)||(_Water_Full))
    {
        return;
    }
    Power_Status=ON;
    rec_buf_power2=ON;
    Timer_run=0;
    Timer_Setting_Time=0;
    Temp_Setting_Time=0;
    write_delay_time=EEP_DELAY;
    //2016/3/29
    if(Mode_Set==COOL)
    {
        light_down_cont=cool_light_down_time;
    }

    if(_EC_Fast_Test)
    {
        _EC_Fast_Test=0;
        Comp_para.off_time=0;
    }

    Clear_off_cycle_mode_Data();

}
/***********************************************************
*函 数 名：  void Turn_Off(void)
*功能描述：  关机
*输入参数：  无
*函数返回值：无
***********************************************************/
void Turn_Off(void)
{
    if(Power_Status==OFF)
    {
        return;
    }
    Power_Status=OFF;
    rec_buf_power2=OFF;
    Timer_run=0;
    Timer_Setting_Time=0;
    Temp_Setting_Time=0;
    Dsp_Time=0;
    write_delay_time=EEP_DELAY;
    light_down_cont=0;
    Power_para.on_time=0;
    Power_para.off_time=0;
    Disp_Delay=0;

    if(_EC_Fast_Test)
    {
        _EC_Fast_Test=0;
        Comp_para.off_time=0;
    }
}

/***********************************************************
*函 数 名：  void Timer_check(void)
*功能描述：  定时确认
*输入参数：  无
*函数返回值：无
***********************************************************/
void Timer_check(void)
{
    if((Timer_Setting_Time>0)&&(_Timer_set_Flag))
    {
        Timer_set=Timer_buff;
        Timer_run=Timer_buff;
        Timer_run=Timer_run*3600;
        Timer_Setting_Time=0;
        Dsp_Time=0;
        _Timer_set_Flag=0;
        Buz_mode(BUZZ_TYPE7);
    }
}

/***********************************************************
*函 数 名：  void key_decode(void)
*功能描述：  按键处理
*输入参数：  无
*函数返回值：无
***********************************************************/
void key_decode(void)
{
    UI08 a=0;
    UI08 ptr;

    if((_sys_err)&&(Key_Number!=power_key))
    {
        return;
    }

    Disp_Delay=10;

    if(_Water_Full)
    {
        return;
    }
    switch(Key_Number)
    {
    case power_key://power
    {
        Buz_mode(BUZZ_TYPE8);
        if(Power_Status==ON)
        {
            Turn_Off();
        }
        else
        {
            Turn_On();
        }

        if((!_DISP_En)&&(Power_Status==ON))
        {
            _DISP_En=1;
            _DISP_En2=0;
        }
        else
        {
            _DISP_En=0;
            _DISP_En2=1;
        }
    }
    break;
    case timer_key: //time
    {

        if(sEC_SYS.timer_key_count<0xff)
        {
            sEC_SYS.timer_key_count++;
        }

        Dsp_Time=5;
        Temp_Setting_Time=0;
        if(Timer_Setting_Time>0)
        {
            Timer_run=0;
            Timer_Setting_Time=0;
            Dsp_Time=0;
            _Timer_set_Flag=0;
            Disp_Delay=0;
        }
        else if((Timer_run>0)&&(Timer_Setting_Time==0))
        {
            Timer_Setting_Time=5;
            _Timer_set_Flag=0;
        }
        else
        {
            Timer_Setting_Time=5;
            Timer_buff=Timer_set;
            _Timer_set_Flag=1;
        }
        Buz_mode(BUZZ_TYPE7);
    }
    break;
    case fan_key:
    {
        Timer_check();
        if(!_Fan_set_enable)
        {
            return;
        }
        if((Dsp_Time==0)&&(Power_Status==OFF))
        {
            Dsp_Time=DISP_TIMER;
            Buz_mode(BUZZ_TYPE7);
            break;
        }
        Temp_Setting_Time=0;
        Timer_Setting_Time=0;
        Buz_mode(BUZZ_TYPE7);
        Dsp_Time=DISP_TIMER;
        Sys_fan_order=HIGH_MID_LOW;

        if(sys_fan_type==SPEED3)
        {
            if(Sys_fan_order==LOW_MID_HIGH)
            {
                if(Fan_Buf==HIGH)
                {
                    Fan_Buf=LOW;
                }
                else
                {
                    Fan_Buf--;
                }
            }
            else
            {
                if(Fan_Buf>=LOW)
                {
                    Fan_Buf=HIGH;
                }
                else
                {
                    Fan_Buf++;
                }
            }
        }
        else if(sys_fan_type==SPEED2)
        {
            if(Fan_Buf==LOW)
            {
                Fan_Buf=HIGH;
            }
            else
            {
                Fan_Buf=LOW;
            }
        }
        Fan_Delay_Timer=FAN_DELAY;
        Fan_Delay_disp=DISP_TIMER;//FAN_DELAY+1;
    };
    break;
    case func_key://mode
    {
        Timer_check();
        if((Dsp_Time==0)&&(Power_Status==OFF))
        {
            Dsp_Time=DISP_TIMER;
            Buz_mode(BUZZ_TYPE7);
            break;
        }
        Temp_Setting_Time=0;
        Timer_Setting_Time=0;
        Buz_mode(BUZZ_TYPE7);
        Dsp_Time=DISP_TIMER;

        Mode_Buf++;
        if(Sys_Tpye==ONLYCOOL)
        {
            if(Mode_Buf>FAN)
            {
                Mode_Buf=COOL;
            }
        }
        else
        {
            if(Mode_Buf>HEAT)
            {
                Mode_Buf=COOL;
            }
        }


        if(Power_Status==OFF)
        {
            Mode_Delay_Timer=1;
        }
        else
        {
            Mode_Delay_Timer=MODE_DELAY;
        }
    }
    break;
    case up_key: //up
    case down_key: //down
    {
        if(Timer_Setting_Time>0)
        {
            Dsp_Time=5;
            Timer_Setting_Time=5;
            Temp_Setting_Time=0;
            if(Timer_Type==Hr24)
            {
                a=24;
            }
            else
            {
                a=12;
            }
            if(Key_Number==up_key)
            {
                Timer_buff++;
                if(Timer_buff>a)
                {
                    Timer_buff=0;
                }
            }
            else
            {
                if(Timer_buff==0)
                {
                    Timer_buff=a;
                }
                else
                {
                    Timer_buff--;
                }
            }
            _Timer_set_Flag=1;
            Buz_mode(BUZZ_TYPE7);
        }
        else if((Temp_Setting_Time>0)||((Dsp_Time>0)&&(Power_Status==OFF)))
        {
            if(Mode_Delay_Timer>0)
            {
                break;
            }

            if(Mode_Buf==FAN)
            {
                return;
            }
            if(Mode_Set==HEAT)
            {
                if(CF_Status==_C)
                {
                    a=HEAT_C;
                    ptr=Temp_Set_C;
                }
                else
                {
                    a=HEAT_F;
                    ptr=Temp_Set_F;
                }
            }
            else
            {
                if(CF_Status==_C)
                {
                    a=(UI08)(COOL_C);
                    ptr=Temp_Set_C;
                }
                else
                {
                    a=(UI08)(COOL_F);
                    ptr=Temp_Set_F;
                }
            }
            if(Key_Number==up_key)
            {
                if(ptr<TempSet_range[a][1])
                {
                    ptr++;
                }
            }
            else
            {
                if(ptr>TempSet_range[a][0])
                {
                    ptr--;
                }
            }

            if(CF_Status==_C)
            {
                Temp_Set_C=ptr;
                Temp_Set_F=C_F(Temp_Set_C);
                if(Mode_Set==HEAT)
                {
                    set_heat_temp_chack();
                }
                else
                {
                    set_cool_temp_chack();
                }
            }
            else
            {
                Temp_Set_F=ptr;
                Temp_Set_C=F_C(Temp_Set_F);

                if(Mode_Set==HEAT)
                {
                    set_heat_temp_chack();
                }
                else
                {
                    set_cool_temp_chack();
                }
            }


            if(Mode_Set==HEAT)
            {
                Temp_Heat_F=Temp_Set_F;
            }
            else
            {
                Temp_Cool_F=Temp_Set_F;
            }

            Buz_mode(BUZZ_TYPE7);
            Dsp_Time=DISP_TIMER;
            Temp_Setting_Time=DISP_TIMER;
            write_delay_time=EEP_DELAY;
        }
        else
        {
            if(Mode_Delay_Timer>0)
            {
                break;
            }
            if((Mode_Buf==FAN)||(Mode_Buf==DRY))
                //if(Mode_Buf==FAN)
            {
                return;
            }
            Temp_Setting_Time=DISP_TIMER;
            Dsp_Time=DISP_TIMER;
            Buz_mode(BUZZ_TYPE7);

        }
    }
    break;
    case SWING_key://_c_f
    {
        Timer_check();
        //if((Power_Status==OFF))
        //{
        //      break;
        // }
        Dsp_Time=DISP_TIMER;
        if(_led_swing_enable)
        {
            _led_swing_enable=0;
        }
        else
        {
            _led_swing_enable=1;
        }
        Buz_mode(BUZZ_TYPE7);
        write_delay_time=EEP_DELAY;
    }
    break;
    case _c_f://_c_f
    {
        Timer_check();
        if((Dsp_Time==0)&&(Power_Status==OFF))
        {
            break;
        }
        Dsp_Time=DISP_TIMER;
        CF_Status^=1;
        Buz_mode(BUZZ_TYPE7);
        write_delay_time=EEP_DELAY;
    }
    break;
    case  EC_Fast_key:
    {
        if((_EC_Fast_Test)||(sEC_SYS.timer_key_count>=2))
        {
            return;
        }

        _EC_Fast_Test=1;
        Disp_Temp_Timer=0;
        Buz_mode(BUZZ_TYPE8);
        minute_cont=0;//清分钟的计时,不然缩时会有<60S的误差

    }
    break;

    case   lamp_key://LAMP
    {
        Timer_check();
        if(LAMP_Status==ON)
        {
            Disp_Delay=0;
            LAMP_Status=OFF;
        }
        else
        {
            LAMP_Status=ON;
        }
        Buz_mode(BUZZ_TYPE7);
        write_delay_time=EEP_DELAY;
    }
    break;

    case filter_key:
    {
        if((Power_Status==OFF))
        {
            return;
        }
        if(Sys_filter_time>=FITER_TIMER)
        {
            Sys_filter_time=0;
            write_delay_time=1;
            Buz_mode(BUZZ_TYPE7);
        }
    }
    break;
    case OFF_CYCLE_KEY:
    {
        if(_Sys_lowpower)
        {
            return;
        }
        Buz_mode(BUZZ_TYPE7);
        off_cycle_Tmin_Dsip_timer=30;//显示时间

        off_cycle_Tmin=off_cycle_Tmin/60;
        if(off_cycle_Tmin==10)//切换参数
        {
            off_cycle_Tmin=15;
        }
        else if(off_cycle_Tmin==15)
        {
            off_cycle_Tmin=30;
        }
        else if(off_cycle_Tmin==30)
        {
            off_cycle_Tmin=10;
        }
        off_cycle_Tmin=off_cycle_Tmin*60;//赋值
        write_delay_time=2;
        Dsp_Time=10;
    }
    break;

    }

}
/***********************************************************
*函 数 名：  void key_deal(void)
*功能描述：  按键处理
*输入参数：  无
*函数返回值：无
***********************************************************/
void key_deal(void)
{
    prg_s_key();
    if(!_ms20_for_key_deal)
    {
        return;
    }
    _ms20_for_key_deal=0;
    //
    get_key_number();
    //
    if(_Self_Test)
    {
        return;
    }
    //
    if(Key_Number!=0)
    {
        key_decode();
        Key_Number=0;
    }
}

/***********************************************************
*函 数 名：  void set_heat_temp_chack(void)
*功能描述：  检查制热模式下温度设置值
*输入参数：  无
*函数返回值：无
***********************************************************/
void set_heat_temp_chack(void)
{
    if(Temp_Set_C>TempSet_range[HEAT_C][1])
    {
        Temp_Set_C=TempSet_range[HEAT_C][1];
    }
    else if(Temp_Set_C<TempSet_range[HEAT_C][0])
    {
        Temp_Set_C=TempSet_range[HEAT_C][0];
    }

    if(Temp_Set_F>TempSet_range[HEAT_F][1])
    {
        Temp_Set_F=TempSet_range[HEAT_F][1];
    }
    else if(Temp_Set_F<TempSet_range[HEAT_F][0])
    {
        Temp_Set_F=TempSet_range[HEAT_F][0];
    }
}
/***********************************************************
*函 数 名：  void set_cool_temp_chack(void)
*功能描述：  检查制冷模式下温度设置值
*输入参数：  无
*函数返回值：无
***********************************************************/
void set_cool_temp_chack(void)
{
    if(Temp_Set_C>TempSet_range[COOL_C][1])
    {
        Temp_Set_C=TempSet_range[COOL_C][1];
    }
    else if(Temp_Set_C<TempSet_range[COOL_C][0])
    {
        Temp_Set_C=TempSet_range[COOL_C][0];
    }

    if(Temp_Set_F>TempSet_range[COOL_F][1])
    {
        Temp_Set_F=TempSet_range[COOL_F][1];
    }
    else if(Temp_Set_F<TempSet_range[COOL_F][0])
    {
        Temp_Set_F=TempSet_range[COOL_F][0];
    }
}

