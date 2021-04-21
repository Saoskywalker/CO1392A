#include "General.h"

xdata  rec_T  rec_buf;
xdata  UI08  old_timer;
xdata UI08   first_rec=1;
xdata UI08 Screen_rec_data[SCREEN_REC_LEN];
xdata UI08    rec_data[SCREEN_REC_LEN];

/***********************************************************
*函 数 名：  void REC_data_init(void)
*功能描述：  变量初始化
*输入参数：  无
*函数返回值：无
***********************************************************/
void REC_data_init(void)
{

    UI08 i;
    for(i=0; i<SCREEN_REC_LEN; i++)
    {
        rec_data[i]=0;
        Screen_rec_data[i]=0;
    }

    rec_buf.mode=COOL;
    rec_buf.fan=FANOFF;
    rec_buf.temp=25;
    rec_buf.cf=_F;
    rec_buf.sleep=0;
    rec_buf.power=OFF;
    rec_buf.timer_f=77;
    rec_buf.timer_time=0;


    old_timer=0;
    first_rec=1;
}
/***********************************************************
*函 数 名：  void set_heat_temp_chack(void)
*功能描述：
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
*功能描述：
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


/***********************************************************
*函 数 名：  void cor258_inf_load(void)
*功能描述：
*输入参数：  无
*函数返回值：无
***********************************************************/
void cor258_inf_load(void)
{
    UI08  temp_buff_F=0;
    UI08  temp_buff_C=0;
    if((_Water_Full)||(_Self_Test==ENABLE))
    {
        return;
    }
    Buz_mode(BUZZ_TYPE7);

    if((rec_buf.power!=rec_buf_power2)||(first_rec))
    {
        Timer_Setting_Time=0;
        Temp_Setting_Time=0;
        rec_buf_power2=rec_buf.power;
        if(rec_buf.power==ON)
        {
            Turn_On();
            _DISP_En=1;
            _DISP_En2=0;
        }
        else
        {
            Turn_Off();
            _DISP_En=0;
            _DISP_En2=1;
        }
        Buz_mode(BUZZ_TYPE8);
    }

    if(_sys_err)
    {
        return;
    }

    if(rec_buf.cf!=CF_Status)
    {
        CF_Status= rec_buf.cf;
        Timer_Setting_Time=0;
    }

    if(rec_buf.mode!=Mode_Buf)
    {
        Mode_Buf=rec_buf.mode;
        Timer_Setting_Time=0;
        Temp_Setting_Time=0;
        if(first_rec)
        {
            Mode_Set=Mode_Buf;
        }
        else
        {
            Mode_Delay_Timer=20;
        }
    }

    if((rec_buf.fan!=Fan_Buf)&&(_Fan_set_enable==ENABLE))
    {
        Fan_Buf=rec_buf.fan;
        Timer_Setting_Time=0;
        Temp_Setting_Time=0;
        if(first_rec)
        {
            Fan_Set=Fan_Buf;
        }
        else
        {
            Fan_Delay_Timer=2;
        }
        Fan_Delay_disp=10;
    }

    if(Mode_Buf==HEAT)
    {

        temp_buff_F=Temp_Heat_F;
    }
    else
    {
        temp_buff_F=Temp_Cool_F;
    }
    temp_buff_C=F_C(temp_buff_F);

    if(CF_Status==_C)
    {
        if(rec_buf.temp!=temp_buff_C)
        {
            Temp_Set_C=rec_buf.temp;
            Temp_Set_F=C_F(Temp_Set_C);

            if(Mode_Buf==HEAT)
            {
                set_heat_temp_chack();
            }
            else
            {
                set_cool_temp_chack();
            }
            if(Mode_Buf==HEAT)
                Temp_Heat_F=Temp_Set_F;
            else
                Temp_Cool_F=Temp_Set_F;
            if((Power_Status==ON)&&((Mode_Buf==HEAT)||(Mode_Buf==COOL)))
                Temp_Setting_Time=10;
            Timer_Setting_Time=0;
            write_delay_time=2;
        }
    }
    else
    {
        if(rec_buf.temp!=temp_buff_F)
        {
            Temp_Set_F=rec_buf.temp;
            Temp_Set_C=F_C(Temp_Set_F);
            if(Mode_Buf==HEAT)
            {
                set_heat_temp_chack();
            }
            else
            {
                set_cool_temp_chack();
            }
            if(Mode_Buf==HEAT)
            {
                Temp_Heat_F=Temp_Set_F;
            }
            else
            {
                Temp_Cool_F=Temp_Set_F;
            }
            if((Power_Status==ON)&&((Mode_Buf==HEAT)||(Mode_Buf==COOL)))
            {
                Temp_Setting_Time=10;
            }
            Timer_Setting_Time=0;
            write_delay_time=2;
        }
    }
    //
    if(rec_buf.timer_f)
    {
        Timer_set=rec_buf.timer_time;
        Timer_buff=Timer_set;
        Timer_run=Timer_set;
        Timer_run=Timer_run*3600;
        if(old_timer!=rec_buf.timer_time)
        {
            old_timer=rec_buf.timer_time;
            Timer_Setting_Time=6;
            Temp_Setting_Time=0;
        }
    }
    else
    {
        Timer_run=0;
        Timer_Setting_Time=0;
    }
    if(first_rec)
    {
        first_rec=0;
    }
}

/***********************************************************
*函 数 名：  void rec_cor258(void)
*功能描述：
*输入参数：  无
*函数返回值：无
***********************************************************/
void rec_cor258(void)
{
    switch(rec_data[1]>>4)
    {
    case 1:
    {
        rec_buf.mode=COOL;
    }
    break;
    case 4:
    {
        rec_buf.mode=DRY;
    }
    break;
    case 8:
    {
        rec_buf.mode=FAN;
    }
    break;
    case 2:
    {
        if(Sys_Tpye==HEATCOOL)
        {
            rec_buf.mode=HEAT;
        }
        else
        {
            return;
        }
    };
    break;
    default:
    {
        ;
    }
    break;
    }

    switch(rec_data[1]&0x0f)
    {
    case 2:
    {
        rec_buf.fan=LOW;
    }
    break;
    case 4:
    {
        if(sys_fan_type==SPEED3)
        {
            rec_buf.fan=MID;
        }
    }
    break;
    case 8:
    {
        rec_buf.fan=HIGH;
    }
    break;
    default:
    {
        ;
    }
    break;
    }

    if(rec_data[2]&bit5)
    {
        if(rec_buf.cf!=_F)
        {
            write_delay_time=1;
            rec_buf.cf=_F;
        }
    }
    else
    {
        if(rec_buf.cf!=_C)
        {
            write_delay_time=1;
            rec_buf.cf=_C;
        }
    }

    if(rec_buf.cf==_F)
    {
        rec_buf.temp=rec_data[3];
    }
    else
    {
        rec_buf.temp=rec_data[3]+16;
    }

    if(rec_data[2]&bit7)
    {
        rec_buf.power=ON;
    }
    else
    {
        rec_buf.power=OFF;
    }

    if(rec_data[2]&bit6)
    {
        rec_buf.timer_time=rec_data[2]&0x1f;
        if(!rec_buf.timer_f)
        {
            old_timer=rec_buf.timer_time+1;
        }
        rec_buf.timer_f=1;
    }
    else
    {
        rec_buf.timer_f=0;
    }

    cor258_inf_load();
}

/***********************************************************
*函 数 名：  void rec_cor279(void)
*功能描述：
*输入参数：  无
*函数返回值：无
***********************************************************/
void rec_cor279(void)
{
    switch(rec_data[2])
    {
    case 0x02:
    {
        Key_Number=power_key;
    }
    break;
    case 0x09:
    {
        Key_Number=func_key ;
    }
    break;
    case 0x11:
    {
        Key_Number=down_key;
    }
    break;
    case 0x1a:
    {
        Key_Number=fan_key ;
    };
    break;
    case 0x0a:
    {
        Key_Number=up_key ;
    }
    break;
    case 0x19:
    {
        Key_Number=timer_key ;
    }
    break;
    case   0x12://LAMP
    {
        Key_Number=lamp_key;
    }
    break;
    case  0x01://SWING_key
    {
        Key_Number=SWING_key;
    }
    break;
    default:
    {
        Key_Number=0xff;
    }
    break;
    }
    rec_data[2]=0xff;
    //_Key_Ok=1;
}

/***********************************************************
*函 数 名：  void remote_selftest(void)
*功能描述：
*输入参数：  无
*函数返回值：无
***********************************************************/
void remote_selftest(void)
{
//   UUI32 data_buf;
    if(_Self_Test==ENABLE)
    {
        return;
    }

    _Self_Test=ENABLE;

    Sys_Version_buf=rec_data[4];

    communication_timer=2;//预防断线情况  可迅速在自检中测出来
    //页擦除
    IAPPageErase(TYPE_DATA_ADDR,IapROM);
    IAPWrite(TYPE_DATA_ADDR,rec_data[4],IapROM);
    Sys_Version_buf=rec_data[4];
    //
    off_cycle_Tmin=Off_cycle_Timer_tab[Sys_Version_buf];
    off_cycle_Tmin=off_cycle_Tmin*60;


    _Auto_Restart=ENABLE;
    _Tcoil_Enable=1;
    Power_Status=OFF;
    Mode_Set=COOL;
    Fan_Set=LOW;
    Temp_Heat_F=72;
    Temp_Cool_F=75;
    _Write_EEP_EN=1;
    Sys_data_write();
    DE_rec_time=0;
    _EC_Fast_Test=0;
    write_delay_time=1;
    Sys_filter_time=0;


    if(mach_type_tab[Sys_Version_buf] & bit1)
    {
        CF_Status=_F;
    }
    else
    {
        CF_Status=_C;
    }
    Sys_data_write();
    test_cont1=0xff;
    test_cont2=0xff;
    test_seq=0;
    test_key_data=0;
    Buz_mode(BUZZ_TYPE8);
    Power_Delay_Time=0;

}

/***********************************************************
*函 数 名： void Screen_rec_deal()
*功能描述：
*输入参数：  无
*函数返回值：无
***********************************************************/
void Screen_rec_deal()
{
    UI08 i=0;
    UI08 verify=0;

    if( (!_REC_IIC)
            ||(!_IICSleep_status) )
    {
        return;
    }
    _REC_IIC=0;

    if(Power_Delay_Time>2)
    {
        return;
    }
    //read_Ndata(0xB0,0x07,Screen_rec_data,SCREEN_REC_LEN);
    read_Ndata(0xB0);
    DE_rec_time=IIC_DELAY;
    if(Screen_rec_data[0]==0)
    {
        return;
    }
    verify=Verify_dat(Screen_rec_data,VERIFY_REC_LEN);

    if(verify==Screen_rec_data[VERIFY_REC_LEN])
    {
        for(i=0; i<7; i++)
        {
            rec_data[i]=Screen_rec_data[i];
            Screen_rec_data[i]=0;
        }
        if((rec_data[0]==0x20)&&(rec_data[1]==0xdf)&&(rec_data[3]==(UI08)(~rec_data[2])))
        {
            rec_cor279();
        }
        //
        else if((rec_data[0]==0x48)&&(Power_Delay_Time==0))
        {
            rec_cor258();
        }
        else if((rec_data[0]==0x84)&&(rec_data[5]==0xAA)&&(rec_data[6]==0x55))
        {
            remote_selftest();
        }
    }

}


/***********************************************************
*函 数 名： void rec_decode()
*功能描述：
*输入参数：  无
*函数返回值：无
***********************************************************/
void rec_decode(void)
{
    if(_Self_Test==ENABLE)
    {
        return;
    }
    Screen_rec_deal();
}



