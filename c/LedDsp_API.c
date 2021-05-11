/********************************************************************************
  // �ļ�����:   Judgeinput_device.c
  // ��������:   ģ�鱾����������ļ�
*********************************************************************************/

#include "General.h"

xdata UUI08   Display_data_buf[COM_total];   //�������ʾ��������
xdata UUI08   LED_data_buf;                  //LED��ʾ��������

xdata UUI08   Display_out_buf[COM_total];    //�������ʾ�������
xdata UUI08   LED_out_buf;                       //LED��ʾ�������

xdata UI08   Screen_Data[SCREEN_MAX];//931D�������ʾ
xdata UI08   Dsp_Time;//��ʾʱ��

//�Լ����
xdata UI08    test_cont1;//�Լ�
xdata UI08    test_cont2;//�Լ�
xdata UI08    test_seq;//�Լ�
xdata UI08    test_key_data;//�Լ�
xdata UI08 Reda_EEP;//�������¶�����  ���Ƿ���ң��ѡ��Ļ���һ��

code  UI08   BCD_tab[10]=   /*LED��ʾ���룬���ڲ��*/
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
 // ��������    : LED_data_intit
 // ��������    : ���ݳ�ʼ��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
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
 // ��������    : prg_ms100_DSP
 // ��������    :
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
xdata UI08   cont2=0;//�Լ�
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
 // ��������    : prg_ms500_DSP
 // ��������    : 500msȡ�� LED��˸
 // ��ڲ���    : ��
 // ���ڲ���    : ��
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
 // ��������    : dig1_2_dsp
 // ��������    : ��ʾ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void dig1_2_dsp(UI08 data_buf)
{
    dig2_num=BCD_tab[data_buf%10];
    dig1_num=BCD_tab[data_buf/10];
}

/*************************************************
 // ��������    : Err_dsp
 // ��������    : ������ʾ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Err_dsp(void)
{
    if(_temp_room_err)     //�¶ȸ�Ӧ����·���߿�·  E1
    {
        dig1_num=DATA_E;      
        dig2_num=DATA_1;
    }
    else if((_temp_coil_err)&&(_Tcoil_Enable))//ͭ�ܸ�Ӧ����·���߿�·  E2
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
 // ��������    : TempSet_dsp
 // ��������    : �趨�¶���ʾ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
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
 // ��������    : TempRoom_dsp
 // ��������    : //������ʾ����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
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
    //������ģʽ�����¶�(ֱ�Ӳ���7F)�л�������ģʽ,�˳������¶Ƚ���,�ָ��������˳�������ʾ
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
        //��ʾ ��ʵ���¶����
        if(Troom_dsp_com==Temp_room.value)
        {
            Troom_dsp_com_timer=30;
            _heat_temp_compensation_DSP_EN=0;//�˳�������ʾ
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
 // ��������    : disp_All
 // ��������    : �ϵ�ȫ��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void disp_All(void)
{
    UI08 i;
    for(i=0; i<COM_total; i++)
    {
        Display_data_buf[i].byte=0xFF;//����ܺ�LED��ȫ��
    }
    LED_SwingOn;
    LED_MuteOff;
    LED_Sleep_off;
    light_down.byte=0;
    LED_data_buf.byte=0x02;//���
}
/*************************************************
 // ��������    : clear_all
 // ��������    : ����ʾ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
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
 // ��������    : LedDsp_Test
 // ��������    : �Լ�
 // ��ڲ���    : ��
 // ���ڲ���    : ��
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
        //LEDȫ��
        // _led_swing_disp_buf=1;
        light_down.byte=0;
        LED_out_buf.byte=0Xff;
        LED_out_buf.bit_.b0=0;//˫ɫLEDֻ����ɫ
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

        //����ˮ�÷���
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

    //Ϊ�˼���û��UVC�ƵĻ���
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
 // ��������    : LedDsp_content
 // ��������    : //��ʾ����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
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

    //ѹ����ǿ�Ʋ�����ʾ
    if(Comp_Test_Disp_En_Timer>0)
    {
        SYS_Inspect_Disp();
        return;
    }
    //������  ��ʾ���¡�����
    if((_EC_Fast_Test)&&(Disp_Temp_Timer<7))
    {
        disp_temp();
        return;
    }
    /**********************************************************************************/
    if(_Water_Full)  //��ˮָʾ
    {
        if(water_light_cont<=2)//��0.3s����0.7��
        {
            LED_water;
        }
        return;
    }
    //������ʾ��
    if(Sys_filter_time>=FITER_TIMER)
    {
        LED_filter;
    }
    //================  ��ҳָʾ��
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
        if((fan_set!=fan_speed)&&(Power_Status==ON)&&(0==Fan_Delay_Timer))//�����趨���е����
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
                (Power_para.on_time<2)||//�ϵ�󣬰���Դ�����������ٵ�����������/����
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
    if(Timer_Setting_Time>0)         //ʱ����ʾ
    {
        if(_Flash_500ms)              //��ʱʱ���������ʱ ��˸��ʾ
        {
            dig1_2_dsp(Timer_buff);
        }
        light_down3=0;
    }
    else if((Power_Status==OFF)&&(Timer_run>0)&&(Temp_Setting_Time==0)&&(Dsp_Time==0))//����״̬ ��ʱ������ʱʱ�������ʾ
    {
        dig1_2_dsp(Timer_buff);
        light_down3=1;
    }
    else if((Power_Status==OFF)&&(Timer_run>0)&&(Dsp_Time==0)&&(Temp_Setting_Time==0)&&(Mode_Buf==FAN))//����״̬ ��ʱ������ʱʱ�������ʾ
    {
        dig1_2_dsp(Timer_buff);
        light_down3=1;
    }
    /**********************************************************************************/
    else
    {
        //�¶���ʾ
        light_down3=0;

        if((Temp_Setting_Time>0)||((Dsp_Time>0)&&(Power_Status==OFF)))//��ʾ�趨�¶�
        {
            if((_Flash_500ms)&&((Mode_Buf==COOL)||(Mode_Buf==HEAT)))
            {
                TempSet_dsp();
            }
        }
        else if(_high_temp_protect_flag)//���±�����ʾ
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

    /*****************************off cycle ��ʾ************************************************/
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
 // ��������    : Verify_dat
 // ��������    : �ۼӺ�  ȡ����1
 // ��ڲ���    : *ptr,len
 // ���ڲ���    : verify
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
 // ��������    : LedDsp_Outdata
 // ��������    : ˢ����ʾ�������е�����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
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
 // ��������    : LED_display
 // ��������    :
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void LED_display(void)
{
    prg_ms100_DSP();
    prg_ms500_DSP();
    LedDsp_content();
    Get_LED_data();
}


