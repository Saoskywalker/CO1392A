/********************************************************************************
  // 文件名称:   Judgeinput_device.c
  // 功能描述:   模块本身操作定义文件
*********************************************************************************/

#include "General.h"

xdata UUI08   Display_data_buf[COM_total];   //数码管显示操作缓存
xdata UUI08   LED_data_buf;                  //LED显示操作缓存

xdata UUI08   Display_out_buf[COM_total];    //数码管显示输出缓存
xdata UUI08   LED_out_buf;                       //LED显示输出缓存

xdata UI08   Screen_Data[SCREEN_MAX];//931D数码管显示
xdata UI08   Dsp_Time;//显示时间

//自检变量
xdata UI08    test_cont1;//自检
xdata UI08    test_cont2;//自检
xdata UI08    test_seq;//自检
xdata UI08    test_key_data;//自检
xdata UI08 Reda_EEP;//机型重新读处理  看是否与遥控选择的机型一致

code  UI08   BCD_tab[10]=   /*LED显示编码，用于查表*/
{
    (DATA_0), /*0*/
    (DATA_1), /*1*/
    (DATA_2), /*2*/
    (DATA_3), /*3*/
    (DATA_4), /*4*/
    (DATA_5), /*5*/
    (DATA_6), /*6*/
    (DATA_7), /*7*/
    (DATA_8), /*8*/
    (DATA_9), /*9*/
};

/*************************************************
 // 函数名称    : LED_data_intit
 // 功能描述    : 数据初始化
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void LED_data_intit(void)
{
    UI08 i;
    for(i=0; i<COM_total; i++)
    {
        Display_data_buf[i].byte=0;
        Display_out_buf[i].byte=0;
    }

    LED_data_buf.byte = 0;
    LED_out_buf.byte = 0;

    for(i=0; i<SCREEN_MAX; i++)
    {
        Screen_Data[i]=0;
    }

    Dsp_Time=0;
    Reda_EEP=0;

    test_cont1=0;
    test_cont2=0;
    test_seq=0;
    test_key_data=0;
}
/*************************************************
 // 函数名称    : prg_ms100_DSP
 // 功能描述    :
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
xdata UI08   cont2=0;//自检
void prg_ms100_DSP(void)
{
    if(!_ms100_disp)
    {
        return;
    }
    _ms100_disp=0;

    water_light_cont++;

    if(water_light_cont&bit1)
    {
        test_cont1++;
        if(++cont2>=2)
        {
            cont2=0;
            test_cont2++;
        }

    }

    if(water_light_cont>=10)
    {
        water_light_cont=0;
    }

    //off cycle
    if(off_cycle_Tmin_Dsip_timer>0)
    {
        off_cycle_Tmin_Dsip_timer--;
    }
}
/*************************************************
 // 函数名称    : prg_ms500_DSP
 // 功能描述    : 500ms取反 LED闪烁
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void prg_ms500_DSP(void)
{
    if(!_led_ms500_tick)
    {
        return;
    }
    _led_ms500_tick=0;
    _Flash_500ms^=1;

    _Flash_for_high_temp++;
    if(_Flash_for_high_temp>=6)
    {
        _Flash_for_high_temp=0;
    }
}
/*************************************************
 // 函数名称    : dig1_2_dsp
 // 功能描述    : 显示
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void dig1_2_dsp(UI08 data_buf)
{
    dig2_num=BCD_tab[data_buf%10];
    dig1_num=BCD_tab[data_buf/10];
}

/*************************************************
 // 函数名称    : Err_dsp
 // 功能描述    : 故障显示
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Err_dsp(void)
{
    if(_temp_room_err)     //温度感应器短路或者开路  E1
    {
        dig1_num=DATA_E;      
        dig2_num=DATA_1;
    }
    else if((_temp_coil_err)&&(_Tcoil_Enable))//铜管感应器短路或者开路  E2
    {
        dig1_num=DATA_E;
        dig2_num=DATA_2;
    }
    else if(_INFAN_ERR)
    {
        dig1_num=DATA_E;
        dig2_num=DATA_3;
    }
    else if(_OUTFAN_ERR )
    {
        dig1_num=DATA_E;
        dig2_num=DATA_4;
    }
    else if(communication_ERR)
    {
        dig1_num=DATA_E;
        dig2_num=DATA_5;
    }

    if(_sys_err==TRUE)
    {
        //dig3_num=0x00;
    }

    if(_sys_err)
    {
        light_down2=1;
    }
    else
    {
        light_down2=0;
    }
}
/*************************************************
 // 函数名称    : TempSet_dsp
 // 功能描述    : 设定温度显示
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void TempSet_dsp(void)
{
    UI08 a;
    UI08 temp_buf;
    if(Mode_Buf!=Mode_Set)
    {
        if(Mode_Buf==HEAT)
        {
            temp_buf=Temp_Heat_F;
        }
        else
        {
            temp_buf=Temp_Cool_F;
        }
        if(CF_Status==_C)
        {
            a=F_C(temp_buf);
        }
        else
        {
            a=temp_buf;
        }
    }
    else
    {
        if(CF_Status==_C)
        {
            a=Temp_Set_C;
        }
        else
        {
            a=Temp_Set_F;
        }
    }
    dig1_2_dsp(a);
}
/*************************************************
 // 函数名称    : TempRoom_dsp
 // 功能描述    : //室温显示函数
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void TempRoom_dsp(void)
{
    UI08 T_max,T_min;
    UI08 dsp_temp;
    if(CF_Status==_C)
    {
        T_max=122;
        T_min=16;
    }
    else
    {
        T_max=99;
        T_min=32;
    }
    //
    if(Troom_dsp<T_min)//
    {
        dig1_num=DATA_L;
        dig2_num=DATA_o;
        return;
    }
    else if(Troom_dsp>T_max)
    {
        dig1_num=DATA_H;
        dig2_num=DATA_I;
        return;
    }

    //if(Mode_Set==COOL)
    //{
    //     dsp_temp=compensate_temp(Troom_dsp);
    //}
    //else
    //{
    dsp_temp=Troom_dsp;
    //}
    //在制热模式补偿温度(直接补偿7F)切换至其他模式,退出补偿温度渐变,恢复正常后，退出补偿显示
    if((!_comp_firston_in_heat)&&(_heat_temp_compensation_DSP_EN))
    {
        if(Troom_dsp_com_timer>=30)
        {
            Troom_dsp_com_timer=0;
            if(Troom_dsp_com>dsp_temp)
            {
                Troom_dsp_com--;
            }
            else if(Troom_dsp_com<dsp_temp)
            {
                Troom_dsp_com++;
            }
        }
        //显示 跟实际温度相等
        if(Troom_dsp_com==Temp_room.value)
        {
            Troom_dsp_com_timer=30;
            _heat_temp_compensation_DSP_EN=0;//退出补偿显示
        }
        dsp_temp=Troom_dsp_com;
    }


    if(CF_Status==_C)
    {
        dsp_temp=F_C(dsp_temp);
    }

    //
    if(Troom_dsp<32)
    {
        dig1_2_dsp(dsp_temp);
        dig1_num=DATA_g;  //-
    }
    else
    {
        dig1_2_dsp(dsp_temp);
    }

}

/*************************************************
 // 函数名称    : disp_All
 // 功能描述    : 上电全显
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void disp_All(void)
{
    UI08 i;
    for(i=0; i<COM_total; i++)
    {
        Display_data_buf[i].byte=0xFF;//数码管和LED灯全亮
    }
    LED_SwingOn;
    LED_MuteOff;
    LED_Sleep_off;
    light_down.byte=0;
    LED_data_buf.byte=0x02;//红灯
}
/*************************************************
 // 函数名称    : clear_all
 // 功能描述    : 清显示
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void  clear_all(void)
{
    UI08 i;

    // _led_swing_disp_buf=0;
    for(i=0; i<COM_total; i++)
    {
        Display_data_buf[i].byte=0;
    }
    // LED_out_buf.byte=0x00;
    LED_data_buf.byte=0x00;
}
/*************************************************
 // 函数名称    : LedDsp_Test
 // 功能描述    : 自检
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void LedDsp_Test(void)
{
    switch(test_seq)
    {
    case 0:
    {
        switch(test_cont2)
        {
        case 0:
        case 1:
            dig1_num=0;
            // _led_swing_disp_buf=0;
            // LED_SwingOff;
            LED_water;
            break;
        case 2:
            // LED_water;
            LED_SwingOn;
            dig1_num|=BIT_A;
            break;
        case 3:
            LED_hfan;
            dig1_num|=BIT_B;
            break;
        case 4:
            LED_mfan;
            dig1_num|=BIT_C;
            break;
        case 5:
            LED_lfan;
            dig1_num|=BIT_D;
            break;
        case 6:
            LED_MuteOn;
            dig1_num|=BIT_E;
            break;
        case 7:
            LED_Sleep_on;
            dig1_num|=BIT_F;
            break;
        case 8:
            LED_fan;
            dig1_num|=BIT_G;
            break;
        case 9:
            LED_hum;
            break;
        case 10:
            LED_CoolAir;
            LED_HEAT_off;
            break;
        case 11:
            LED_HEAT;
            LED_CoolAir_off;
            break;
        case 12:
            LED_timer;
            break;
        case 13:
            LED_UvcOn;
            break;

        default:
            test_cont2=0;
            test_cont1=0;
            // _led_swing_disp_buf=0;
            clear_all();
            break;
        }
        dig2_num=dig1_num;
    }
    break;

    case 3:
    {
        //LED全亮
        // _led_swing_disp_buf=1;
        light_down.byte=0;
        LED_out_buf.byte=0Xff;
        LED_out_buf.bit_.b0=0;//双色LED只亮白色
        //
        if(Reda_EEP==0)
        {
            SYS_Mach_type_judge();
            Reda_EEP=1;
        }
        if((Sys_EEP_SYTP>=TYPE_MAX)||(Sys_Version_buf!=Sys_EEP_SYTP)||(Sys_EEP_SYTP==0))
        {
            dig1_num=DATA_E;
            dig2_num=DATA_E;
        }
        else if(communication_ERR)
        {
            dig1_num=DATA_E;
            dig2_num=DATA_5;
        }
        else
        {
            dig1_2_dsp(Sys_EEP_SYTP);
        }
    }
    break;
    case 2:
    {
        LED_out_buf.byte=0X00;

        //测试水泵反馈
        if(INFAN_IO)
        {
            DCPUMP_ON;
        }
        else
        {
            DCPUMP_OFF;
        }

        dig1_num=DATA_r;
        dig2_num=BCD_tab[Soft_Version];
        // _led_swing_disp_buf=1;
    }
    break;
    }
    //////////////////////////////////////////////////////////////////////////////////////
    if(Key_Number!=0x00)
    {
        Buz_mode(BUZZ_TYPE7);
        switch(Key_Number)
        {
        case uvc_key:
            test_key_data|=bit0;
            break;
        case timer_key:
            test_key_data|=bit1;
            break;
        case func_key:
            test_key_data|=bit2;
            break;
        case down_key:
            test_key_data|=bit3;
            break;
        case up_key:
            test_key_data|=bit4;
            break;
        case fan_key:
            test_key_data|=bit5;
            break;
        case SWING_key:
            test_key_data|=bit6;
            break;
        case power_key:
            test_key_data|=bit7;
            break;
        }
        Key_Number=0;
    }

    //为了兼容没有UVC灯的机型
    if (test_key_data & 0x01)
    {
        switch(test_key_data)
        {
        case  0x7f:
            test_seq=3;
            break;
        case  0xff:
            test_seq=2;
            break;
        }   
    }
    else
    {
        switch(test_key_data)
        {
        case  (0x7f >> 1):
            test_seq=3;
            break;
        case  (0xff >> 1):
            test_seq=2;
            break;
        } 
    }
}

/*************************************************
 // 函数名称    : LedDsp_content
 // 功能描述    : //显示处理
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void LedDsp_content(void)
{
    FANSPEED_TYPE a;
    /**********************************************************************************/
    if(Power_Delay_Time>0)
    {
        disp_All();
        return;
    }
    if(_Self_Test)
    {
        LedDsp_Test();
        return;
    }
    //===========
    if(   (_Sys_lowpower)
            &&(Power_Status==ON)
            && ((Mode_Set==COOL)||(Mode_Set==HEAT))
            &&(Power_Delay_Time==0)
            &&(Temp_Setting_Time==0)
            &&(Timer_Setting_Time==0)
            &&(Dsp_Time==0))
    {

        if( (light_down_cont==0)&&(Way4_para.OUT==OFF)&&(R_overload_status==0) )
        {
            light_down4=1;
        }
        else
        {
            light_down4=0;
        }
    }
    else
    {
        light_down4=0;
    }
    //

    //===========
    clear_all();
    /**********************************************************************************/
    if(Sys_EEP_SYTP==0xff)
    {
        dig1_num=DATA_F;
        dig2_num=DATA_F;
        Power_Status=OFF;
        return;
    }

    //压缩机强制测试显示
    if(Comp_Test_Disp_En_Timer>0)
    {
        SYS_Inspect_Disp();
        return;
    }
    //进入快测  显示室温、管温
    if((_EC_Fast_Test)&&(Disp_Temp_Timer<7))
    {
        disp_temp();
        return;
    }
    /**********************************************************************************/
    if(_Water_Full)  //满水指示
    {
        if(water_light_cont<=2)//亮0.3s，灭0.7秒
        {
            LED_water;
        }
        return;
    }
    //滤网提示灯
    if(Sys_filter_time>=FITER_TIMER)
    {
        LED_filter;
    }
    //================  摆页指示灯
    if(Power_Status==ON)
    {
        if(_led_swing_enable)
        {
            // _led_swing_disp_buf=1;
            LED_SwingOn;
        }

        if (UVC_para.BUF==ON)
        {
            LED_UvcOn;
        }   
        else
        {
            LED_UvcOff;
        }
    }
    else if((Dsp_Time>0)&&(Power_Status==OFF))
    {
        if(_led_swing_enable)
        {
            // _led_swing_disp_buf=1;
            LED_SwingOn;
        }   
    }
    
    /**********************************************************************************/
    if((Power_Status==ON)||(Timer_Setting_Time>0)||(Dsp_Time>0)||(Temp_Setting_Time>0))
    {
#if 1
        if((fan_set!=fan_speed)&&(Power_Status==ON)&&(0==Fan_Delay_Timer))//不按设定运行的情况
        {
            a=fan_speed;
        }
        else
        {
            a=Fan_Buf;
        }

#else
        if( (Fan_Delay_disp>0)||
                (Fan_Delay_Timer>0)||
                (Power_para.on_time<2)||//上电后，按电源键开机，风速灯立即点亮。/朱大秉
                ((Dsp_Time>0)&&(Power_Status==OFF)))

        {
            a=Fan_Buf;
        }
        else
        {
            a=fan_speed;
        }
#endif

        if(a==HIGH)
        {
            LED_hfan;
        }
        else if(a==MID)
        {
            LED_mfan;
        }
        else if(a==LOW)
        {
            LED_lfan;
        }


        if(Mode_Buf==COOL)
        {

            LED_CoolAir;
        }
        else if(Mode_Buf==FAN)
        {
            LED_fan;
        }
        else if(Mode_Buf==DRY)
        {
            LED_hum;
        }
        else if(Mode_Buf==HEAT)
        {
            LED_HEAT;
        }
    }

    /**********************************************************************************/
    if(Timer_Setting_Time>0)         //时间显示
    {
        if(_Flash_500ms)              //定时时间参数设置时 闪烁显示
        {
            dig1_2_dsp(Timer_buff);
        }
        light_down3=0;
    }
    else if((Power_Status==OFF)&&(Timer_run>0)&&(Temp_Setting_Time==0)&&(Dsp_Time==0))//待机状态 定时开机定时时间参数显示
    {
        dig1_2_dsp(Timer_buff);
        light_down3=1;
    }
    else if((Power_Status==OFF)&&(Timer_run>0)&&(Dsp_Time==0)&&(Temp_Setting_Time==0)&&(Mode_Buf==FAN))//待机状态 定时开机定时时间参数显示
    {
        dig1_2_dsp(Timer_buff);
        light_down3=1;
    }
    /**********************************************************************************/
    else
    {
        //温度显示
        light_down3=0;

        if((Temp_Setting_Time>0)||((Dsp_Time>0)&&(Power_Status==OFF)))//显示设定温度
        {
            if((_Flash_500ms)&&((Mode_Buf==COOL)||(Mode_Buf==HEAT)))
            {
                TempSet_dsp();
            }
        }
        else if(_high_temp_protect_flag)//高温报警显示
        {
            if(_Flash_for_high_temp<4)
            {
                TempRoom_dsp();
            }
        }
        else if(Power_Status==ON)
        {
            TempRoom_dsp();
        }
    }

    /*****************************off cycle 显示************************************************/
    if(off_cycle_Tmin_Dsip_timer>0)
    {
        dig1_2_dsp((UI08)(off_cycle_Tmin/60));
    }
    /**********************************************************************************/
    if(_DISP_En)
    {
        Err_dsp();
    }
    if(!_sys_err)
    {
        _DISP_En=0;
    }
    /**********************************************************************************/
    if((power_status==OFF)
            &&(0==Timer_run)
            &&(0==Temp_Setting_Time)
            &&(0==Timer_Setting_Time)
            &&(0==Dsp_Time)
            &&(!_DISP_En))
    {
        light_down1=1;
        return;
    }
    light_down1=0;


    if(_sys_err)
    {
        light_down2=1;
        return;
    }
    light_down2=0;

    if(Timer_Setting_Time>0)
    {
        if(_Flash_500ms)
        {
            LED_timer;
        }
    }
    else if(Timer_run>0)
    {
        LED_timer;
    }

}
/*************************************************
 // 函数名称    : Verify_dat
 // 功能描述    : 累加和  取反加1
 // 入口参数    : *ptr,len
 // 出口参数    : verify
***************************************************/
//UI08 Verify_dat(UI08 *ptr,UI08 len)
//{
//    UI08 verify=0;
//    UI08 i=0;
//    for(i=0; i<len; i++)
//    {
//        verify+=*ptr;
//        ptr++;
//    }
//    verify=(~verify)+1;
//    return verify;
//}
/*************************************************
 // 函数名称    : LedDsp_Outdata
 // 功能描述    : 刷新显示缓冲区中的数据
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Get_LED_data(void)
{
    UI08 i=0;
    for(i=0;i<COM_total;i++)
    {
        Display_out_buf[i].byte=Display_data_buf[i].byte;
    }
    LED_out_buf.byte = LED_data_buf.byte;
}
/*************************************************
 // 函数名称    : LED_display
 // 功能描述    :
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void LED_display(void)
{
    prg_ms100_DSP();
    prg_ms500_DSP();
    LedDsp_content();
    Get_LED_data();
}


