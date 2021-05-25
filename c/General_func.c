//**********************************************************
//�ļ�����: 	General_func.c
//�ļ�����: 	��Ƭ���ײ�Ӳ�������Ӻ���
//��Ŀ����:
//��	��:
//��	��:
//*********************************************************/
#include "General.h"
//ϵͳģʽ���ñ���
xdata MODE_TYPE       Mode_Set;//ģʽ�趨
xdata MODE_TYPE       Mode_Buf;//ģʽ�趨buf
xdata UI08            Mode_Delay_Timer;//ģʽ�趨��ʱ
/////////////////////////////////////////////////////////////
xdata FANSPEED_TYPE    Fan_Set;//�����趨
xdata FANSPEED_TYPE    Fan_Buf;//�����趨buf
xdata FANSPEED_TYPE    Outfan_Speed;//��������
xdata FANSPEED_TYPE    fan_speed;//ʵ�����з���
xdata UI08             Fan_Delay_Timer;//�����趨��ʱ
xdata UI08             Fan_Delay_disp;//�趨��ʾ
/////////////////////////////////////////////////////////////
//�����¶�
xdata UI08  Temp_Set_C;//�¶��趨 C
xdata UI08  Temp_Set_F;//�¶��趨 F
xdata UI08  Temp_Cool_F;//�����趨�¶�
xdata UI08  Temp_Heat_F;//�����趨�¶�
xdata UI08  Temp_Setting_Time;//�¶�Ԥ�趨ʱ��
/////////////////////////////////////////////////////////////
//���ö�ʱʱ��
xdata UI32        Timer_run;            //��ʱʱ�� ��Ϊ��λ
xdata UI08        Timer_buff;           //00-24Сʱ ʱ���趨��ʱ����
xdata UI08        Timer_Setting_Time;   //�趨�������˸��ʾʱ��/���޸Ĳ���ʱ��
xdata UI08        Timer_set;            //00-24Сʱ ʱ���趨
xdata TIMER_TYPE  Timer_Type;           // 12H/24H

xdata UI08   Key_Number;    //��ֵ
xdata UI16   Key_last;     //��������ʱ�����
/////////////////////////////////////////////////////////////
//ϵͳ����
xdata ONOFF_STATUS     Power_Status;         //���ػ�����
xdata ONOFF_STATUS     rec_buf_power2;       //���ػ�
xdata UI08             write_delay_time;     //дEEP
xdata UI08             Power_Delay_Time;      //ϵͳ������ʱ

xdata UI16             compensate_delaytime; //�¶Ƚ���ʱ��
xdata UI16             light_down_cont;//���ͣ����ʾ����ʱ��  2016/3/29
xdata UUI08            SYS_bit;   //ϵͳbit
xdata UUI08            SYS_bit1;  //ϵͳbit1
xdata UUI08            SYS_bit2;  //ϵͳbit2
xdata UUI08            SYS_bit3;  //ϵͳbit3
xdata UUI08            SYS_bit4;  //ϵͳbit4
xdata UUI08            SYS_bit5;  //ϵͳbit5
xdata UUI08            SYS_bit6;  //ϵͳbit6
xdata UI08             sleep_Time;//˯����ʱʱ��
xdata UI08             DE_rec_time;//rec��ȡ
xdata ONOFF_STATUS     LAMP_Status;//�ƹ�
xdata UI08             minute_cont;//Min����
xdata UI16             Sys_filter_time;	     //���������תʱ��
/////////////////////////////////////////////////////////////
xdata FAN_TYPE         sys_fan_type;//����/���ٷ�
xdata CF_TYPE          CF_Status;         // C/F
xdata FORM_TYPE        Sys_Tpye;          //ȷ������������ģʽ����
xdata FAN_ORDER_TYPE   Sys_fan_order; //�������
xdata UUI08            light_down;//����
xdata UI08             water_light_cont;//��ˮ
xdata UI08 Sys_EEP_SYTP;//����
xdata UI08 Sys_Version_buf;//�������
xdata UI08 Buzz_Cnt;//�������������
xdata  UI08 EEP_data[EEP_MAX]= {0};
xdata  UI08 EEP_data_last[EEP_MAX]= {0};
xdata UI16 EEP_OffSet_DATA_ADDR = 0;//дEEPƫ�Ƶĵ�ַ(0x00~511,һҳΪ512)
//================================================
//8����λһ��
//================================================

//ʹ�� bit  1                  0
//bit0     ��ů               ����
//bit1     _F                 _C
//bit2     Auto_Restart       NO_Auto_Restart
//bit3     Hr12               Hr24
//bit4     �й���             �޹���
//bit5     ���¶Ȳ���         ���¶Ȳ���
//bit6     ��ͷ�             ��߷�
//bit7     �����ٷ�           �ڶ��ٷ�
//bit8     �е͹���Ҫ��       �޵͹���Ҫ��
code  UI16 mach_type_tab[TYPE_MAX]=
{
    bit0|bit4|bit7|bit8,                //���� C N0RESTART hr24  �޲��� ��߷� �����ٷ� /*Ĭ��*/
    //Off cycle    ���ͺ�
    bit4|bit5|bit7|bit8,                     //���� C N0RESTART ��߷� �����ٷ�   NA        /*01*/
    bit4|bit5|bit6|bit7|bit8,                //���� C N0RESTART ��ͷ� �����ٷ�   NA        /*02*/
    bit1|bit2|bit4|bit5|bit7,                //���� F  RESTART ��߷� �����ٷ�    10        /*03*/
    bit1|bit2|bit4|bit5|bit6|bit7,           //���� F  RESTART ��ͷ� �����ٷ�    10        /*04*/
    bit1|bit2|bit4|bit5|bit7,                //���� F  RESTART ��߷� �����ٷ�    15        /*05*/
    bit1|bit2|bit4|bit5|bit6|bit7,           //���� F  RESTART ��ͷ� �����ٷ�    15        /*06*/
    bit1|bit2|bit4|bit5|bit7,                //���� F  RESTART ��߷� �����ٷ�    30        /*07*/
    bit1|bit2|bit4|bit5|bit6|bit7,           //���� F  RESTART ��ͷ� �����ٷ�    30        /*08*/
    bit0|bit4|bit5|bit7|bit8,                //��ů C  N0RESTART ��߷� �����ٷ�  NA        /*09*/
    bit0|bit4|bit5|bit6|bit7|bit8,           //��ů C  N0RESTART ��ͷ� �����ٷ�  NA        /*10*/
    bit0|bit2|bit1|bit4|bit5|bit7,           //��ů F  RESTART ��߷� �����ٷ�    10        /*11*/
    bit0|bit2|bit1|bit4|bit5|bit6|bit7,      //��ů F  RESTART ��ͷ� �����ٷ�    10        /*12*/
    bit0|bit2|bit1|bit4|bit5|bit7,           //��ů F  RESTART ��߷� �����ٷ�    15        /*13*/
    bit0|bit2|bit1|bit4|bit5|bit6|bit7,      //��ů F  RESTART ��ͷ� �����ٷ�    15        /*14*/
    bit0|bit2|bit1|bit4|bit5|bit7,           //��ů F  RESTART ��߷� �����ٷ�    30        /*15*/
    bit0|bit2|bit1|bit4|bit5|bit6|bit7,      //��ů F  RESTART ��ͷ� �����ٷ�    30        /*16*/
    bit4|bit5|bit8,                          //���� C  N0RESTART ��߷� �ڶ��ٷ�  NA        /*17*/
    bit1|bit2|bit4|bit5,                     //���� F  RESTART ��߷� �ڶ��ٷ�    10        /*18*/
    bit0|bit4|bit5|bit8,                     //��ů C  N0RESTART ��߷� �ڶ��ٷ�  NA        /*19*/
    bit0|bit2|bit1|bit4|bit5,                //��ů F  RESTART ��߷� �ڶ��ٷ�    10        /*20*/
};

code   UI08 Off_cycle_Timer_tab[TYPE_MAX]=
{
    0,
    0,        /*01*/
    0,        /*02*/
    10,        /*03*/
    10,        /*04*/
    15,        /*05*/
    15,        /*06*/
    30,        /*07*/
    30,        /*08*/
    0,        /*09*/
    0,        /*10*/
    10,       /*11*/
    10,        /*12*/
    15,        /*13*/
    15,        /*14*/
    30,        /*15*/
    30,        /*16*/
    0,        /*17*/
    10,        /*18*/
    0,        /*19*/
    10,        /*20*/
};

xdata ONOFF_STATUS     SYS_UVC_Status = OFF;

/***********************************************************
*�� �� ����  void SysData_init(void)
*����������  ������ʼ��
*���������  ��
*��������ֵ����
***********************************************************/
void SysData_init(void)			//ϵͳ�Ĵ��� ��ʼ������
{
    Fan_Set=LOW;
    Fan_Buf=LOW;
    fan_speed=FANOFF;
    Outfan_Speed=LOW;
    Fan_Delay_Timer=0;
    Fan_Delay_disp=0;
    Mode_Buf=COOL;
    Mode_Set=COOL;
    Mode_Delay_Timer=0;
    CF_Status=_C;
    Temp_Set_C=24;
    Temp_Set_F=75;
    Temp_Cool_F=75;
    Temp_Heat_F=72;
    Temp_Setting_Time=0;
    Buzz_Cnt=0XFF;
    _Water_Full_status=0;
    Power_Status=OFF;
    rec_buf_power2=OFF;
    write_delay_time=0;
    Power_Delay_Time=3;      //ϵͳ������ʱ
    compensate_delaytime=0;
    light_down_cont=0;//���ͣ����ʾ����ʱ��  2016/3/29
    SYS_bit.byte=0;
    SYS_bit1.byte=0;
    SYS_bit2.byte=0;
    SYS_bit3.byte=0;
    SYS_bit4.byte=0;
    SYS_bit5.byte=0;
    SYS_bit6.byte=0;
    sleep_Time=15;
    DE_rec_time=0;
    LAMP_Status=ON;
    Timer_run=0;
    Timer_buff=8;
    Timer_set=8;            //00-24Сʱ ʱ���趨
    Timer_Type=Hr24;
    Timer_Setting_Time=0;
    Key_Number=0;
    Key_last=0;
    key_new=0;
    M_Key_last=0;
    Read_key_delay=0;
    SYS_Inspect_Key_Data=0;
    sys_fan_type=SPEED3;
    light_down.byte=0;

    minute_cont=0;
    Sys_filter_time=0;

    Sys_Tpye=HEATCOOL;
    Sys_fan_order=HIGH_MID_LOW;
    water_light_cont=0;
    Sys_EEP_SYTP=0xff;
    Sys_Version_buf=0xff;
    //
    ADC_Data_Init();
    control_data_intit();
    LED_data_intit();
    comm_data_init();
    communication_data_init();
    SYS_Inspect_Data_Init();
    EXV_Data_init();
    IO_data_Init();
//    REC_data_init();
    timer_data_init();

    Buzz_Cnt=20;
    Buz_mode(BUZZ_TYPE8);
}


/***********************************************************
*�� �� ����  void SYS_type(void)
*����������  ���Ͳ���
*���������  ��
*��������ֵ����
***********************************************************/
void SYS_type(void)
{
    if(Sys_EEP_SYTP>=TYPE_MAX)
    {
        return;
    }

    if(mach_type_tab[Sys_EEP_SYTP]& bit0)//����/��ů����
    {
        Sys_Tpye=HEATCOOL;
    }
    else
    {
        Sys_Tpye=ONLYCOOL;
    }
    if(mach_type_tab[Sys_EEP_SYTP] & bit1)
    {
#ifndef WAP23A
        CF_Status=_F;
#else
        CF_Status=_C;
#endif
    }
    else
    {
        CF_Status=_C;
    }
    if(mach_type_tab[Sys_EEP_SYTP] & bit2)
    {
        _Auto_Restart=ENABLE;
    }
    else
    {
        _Auto_Restart=DISABLE;
    }
    if(mach_type_tab[Sys_EEP_SYTP]&bit3)                   //ʱ���ʽ
    {
        Timer_Type=Hr12;
    }
    else
    {
        Timer_Type=Hr24;
    }
    if(mach_type_tab[Sys_EEP_SYTP]&bit4)                  //���޹���
    {
        _Tcoil_Enable=ENABLE;
    }
    else
    {
        _Tcoil_Enable=DISABLE;
    }
    if(mach_type_tab[Sys_EEP_SYTP]&bit5)                  //�¶Ȳ���
    {
        _compensate_enable=1;
    }
    else
    {
        _compensate_enable=0;
    }
    if(mach_type_tab[Sys_EEP_SYTP]&bit6)
    {
        _outfan_low_enable=1;
    }
    else
    {
        _outfan_low_enable=0;
    }
    if(mach_type_tab[Sys_EEP_SYTP]&bit7)
    {
        sys_fan_type=SPEED3;
    }
    else
    {
        sys_fan_type=SPEED2;
    }
    if(mach_type_tab[Sys_EEP_SYTP]&bit8)//2016
    {
        _Sys_lowpower=ENABLE;
    }
    else
    {
        _Sys_lowpower=DISABLE;
    }
    /*
    if(mach_type_tab[Sys_EEP_SYTP]&bit9)
    {
       CF_EN=ENABLE;
    }
    else
    {
       CF_EN=DISABLE;
    }*/
}


/***********************************************************
*�� �� ����  void SYS_Mach_type_judge(void)
*����������  ���Ͳ���
*���������  ��
*��������ֵ����
***********************************************************/
void SYS_Mach_type_judge(void)
{
    UI08 buf=0;
#if 0
    Sys_EEP_SYTP=4;
    SYS_type();
    return;
#endif
    buf=IAPRead(TYPE_DATA_ADDR,IapROM);
    // buf = 13;

    if((buf==0x00)||(buf==0xff))
    {
        Sys_EEP_SYTP=0xff;
        return;   
    }
    else if(buf>TYPE_MAX)
    {
        Sys_EEP_SYTP=0;
        off_cycle_Tmin=Off_cycle_Timer_tab[0];//����
    }
    else
    {
        Sys_EEP_SYTP=buf;
        off_cycle_Tmin=Off_cycle_Timer_tab[Sys_EEP_SYTP];
    }
    off_cycle_Tmin=off_cycle_Tmin*60;//��ֵ


    SYS_type();
}

/***********************************************************
*�� �� ����  void prg_s_general(void)
*����������  Sʱ��Ƭ
*���������  ��
*��������ֵ����
***********************************************************/
void prg_s_general(void)
{
    if(!_s_general)
    {
        return;
    }
    _s_general=0;

    if(Power_Delay_Time>0)
    {
        Power_Delay_Time--;
    }

    Troom_update_time++;
    if(Troom_update_time>=3)
    {
        Troom_update_time=0;
        //����ģʽ��-ѹ����&����򿪺���ʾ�����¶�
        if((_heat_temp_compensation)&&(_comp_firston_in_heat))//���Ȳ���7F
        {
            Troom_dsp=Temp_room.value-7;
            Troom_dsp_com=Troom_dsp;
            Troom_dsp_com_timer=0;//����ʱ������
            _heat_temp_compensation_DSP_EN=1;
        }
        else
        {
            Troom_dsp=Temp_room.value;
        }
    }
    //
    if(Troom_dsp_com_timer<0xff)
    {
        Troom_dsp_com_timer++;
    }
    //
    if(write_delay_time>0)
    {
        write_delay_time--;
        if(write_delay_time==0)
        {
            _Write_EEP_EN=1;
        }
    }

    if((compensate_delaytime>0)&&(Comp_para.OUT==ON))
    {
        compensate_delaytime--;
    }

    if(Comp_para.OUT==OFF)
    {
        _compensate_80_87=1;
        compensate_delaytime=0;
    }


    if(communication_timer>0)
    {
        communication_timer--;
        communication_ERR=0;
    }
    else
    {
        communication_ERR=1;
    }



    if(sleep_Time>0)
    {
        sleep_Time--;
        if(0==sleep_Time)
        {
            _Sleep_status=1;
        }
    }


    //off cycle ��ʱ
    if(PowerKey_off_cycle_Time>0)
    {
        PowerKey_off_cycle_Time--;
    }

}
/***********************************************************
*�� �� ����  void F_C(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/
UI08 F_C(UI08 cfdata)
{
    UI08 a=0;
    if(cfdata>32)
    {
        a=((cfdata-32)*10+9)/18;
    }
    else
    {
        a=((32-cfdata)*10+9)/18;
    }
    return 	a;
}
/***********************************************************
*�� �� ����  void C_F(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/
UI08 C_F(UI08 cfdata)
{
    UI08 a=0;
    a=(cfdata*18+5)/10+32;
    if (a>89)
    {
        a=89;
    }
    return a;
}
/***********************************************************
*�� �� ����  void delay(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/
void delay(UI16 i)
{
    while(i)
    {
        i--;
    }
}


/***********************************************************
*�� �� ����  void prg_minute(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/

void prg_minute(void)
{
    if(++minute_cont<60)
    {
        return;
    }
    minute_cont=0;
    prg_minute_control();
}
/***********************************************************
*�� �� ����  void prg_minute_control(void)
*����������
*���������  ��
*��������ֵ����
***********************************************************/
void prg_minute_control(void)
{
    Sys_filter_deal();
}
/*********************************************************
������: void filter_time_deal(void)
��  ��: ������ʱ����
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
void Sys_filter_deal(void)
{
    UI16 num=0;
    //����
    //if(++filter_Minute_Count<60)//һ���ӽ���һ����
    //{return;}
    //filter_Minute_Count=0;
    //
    if(_EC_Fast_Test==ENABLE)//�����ʱ  15000 ==> 25����
    {
        num=600;
    }
    else
    {
        num=1;
    }
    //
    if(fan_speed!=FANOFF)
    {
        if(Sys_filter_time<0Xffff-num)
        {
            Sys_filter_time=Sys_filter_time+num;
        }
        //
        if(Sys_filter_time%60==0)//ÿһСʱ��¼һ������ʱ��
        {
            write_delay_time=2;
        }
    }
    //
    if(Sys_filter_time>=15000)//������ϴʱ�� 250h*60=15000����
    {
        write_delay_time=2;
    }
}
/***********************************************************
*�� �� ����  void Sys_data_read_eep(void)
*����������  EEP
*���������  ��
*��������ֵ����
***********************************************************/
void Sys_data_read_eep(void)
{
    UI08 buf=0;
    UI16 buf16=0;
    UI16 i;
    UI08 crc=0;
    UI08 check_EEP_data=0;//����Ƿ�洢��EEP����
    UI16 check_offset=0;//EEP�洢��ƫ����

    for (i=0;i<SECTION_SIZE;i=i+EEP_MAX)
    {
        check_EEP_data = IAPRead(SYS_DATA_ADDR+i,IapROM);
        if(check_EEP_data != 0xAA)//����һ���ֽ�
        {
            if(i == 0 && check_EEP_data != 0xFF && check_EEP_data != 0x00)//û�д������
            {
                IAPPageErase(SYS_DATA_ADDR,IapROM);
                EEP_OffSet_DATA_ADDR = 0;
                break;
            }
            else
            {
                check_offset = i - EEP_MAX;
                break;
            }
        }
    }

    for(i=0; i<EEP_MAX; i++)
    {
        EEP_data[i]=IAPRead(SYS_DATA_ADDR+i+check_offset,IapROM);
    }
    crc=crc_check(&EEP_data[0],EEP_MAX-1);//CRCУ��
    //У��
    if((0xaa!=EEP_data[0])||(crc!=EEP_data[EEP_MAX-1]))
    {
        _Write_EEP_EN=1;
        return;
    }
    //
    if(EEP_data[9]<=30)//���� off_cycle_Tmin
    {
        off_cycle_Tmin=EEP_data[9];
        off_cycle_Tmin=off_cycle_Tmin*60;
    }

    buf16=EEP_data[10];//����ʱ�� �߰�λ
    buf16=buf16<<8;
    buf=EEP_data[11];//����ʱ�� �Ͱ�λ
    buf16=buf16+buf;
    if(buf16!=0xffff)
    {
        Sys_filter_time=buf16;
    }

    if(!_Auto_Restart)
    {
        return;
    }
    //���ػ�
    if(EEP_data[1]==0)
    {
        Power_Status=OFF;
        Comp_para.off_time=180;
        return;
    }
    else
    {
        Power_Status=ON;
        Comp_para.off_time=0;
    }
    //ģʽ
    if(EEP_data[2]>(Sys_Tpye+2) )
    {
        Mode_Set=COOL;
    }
    else
    {
        Mode_Set=(MODE_TYPE)EEP_data[2];
    }
    Mode_Buf=Mode_Set;
    //
    if(Mode_Buf==COOL)
    {
        light_down_cont=cool_light_down_time;
    }
    //����  EEP_data[3]>FANOFF
    if((EEP_data[3]>3)||((sys_fan_type==SPEED2)&&(EEP_data[3]==1)))
    {
        Fan_Set=LOW;
    }
    else
    {
        Fan_Set=(FANSPEED_TYPE)(EEP_data[3]);
    }
    Fan_Buf=Fan_Set;
    //�����趨�¶�
    if(((EEP_data[4]<TempSet_range[3][0])
            ||(EEP_data[4]>TempSet_range[3][1])))
    {
        Temp_Heat_F=72;
    }
    else
    {
        Temp_Heat_F=EEP_data[4];
    }
    //�����趨�¶�
    if(   ((EEP_data[5]<TempSet_range[2][0])
            ||(EEP_data[5]>TempSet_range[2][1])))
    {
        Temp_Cool_F=75;
    }
    else
    {
        Temp_Cool_F=EEP_data[5];
    }

    if(Mode_Set==HEAT)
    {
        Temp_Set_F=Temp_Heat_F;
    }
    else
    {
        Temp_Set_F=Temp_Cool_F;
    }
    Temp_Set_C=F_C(Temp_Set_F);
    // C/F
    if(0==EEP_data[6])
    {
        CF_Status=_C;
    }
    else
    {
        CF_Status=_F;
    }
    //��ҳ
    if(0==EEP_data[7])
    {
        _led_swing_enable=0;
    }
    else
    {
        _led_swing_enable=1;
    }
    //�ƹ�
    if(0==EEP_data[8])
    {
        LAMP_Status=OFF;
    }
    else
    {
        LAMP_Status=ON;
    }

    if(EEP_data[12] == ON)
    {
        SYS_UVC_Status = ON;
    }
    else
    {
        SYS_UVC_Status = OFF;   
    }
}
/*********************************************************
������: void Sys_data_write(void)
��  ��: EEP д
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
void Sys_data_write(void)
{
    UI08 i;
    UI08 crc=0;
    UI08 same_flag = 0;
    //
    EEP_data[0]=0xaa;
    EEP_data[1]=(UI08)(Power_Status);
    EEP_data[2]=(UI08)(Mode_Set);
    EEP_data[3]=(UI08)(Fan_Set);
    EEP_data[4]=Temp_Heat_F;
    EEP_data[5]=Temp_Cool_F;
    EEP_data[6]=(UI08)(CF_Status);
    EEP_data[7]=(UI08)(_led_swing_enable);
    EEP_data[8]=(UI08)(LAMP_Status);
    EEP_data[9]=(UI08)(off_cycle_Tmin/60);
    EEP_data[10]=Sys_filter_time/256;
    EEP_data[11]=Sys_filter_time%256;
    EEP_data[12]=SYS_UVC_Status;
    EEP_data[EEP_MAX-1]=crc_check(&EEP_data[0],EEP_MAX-1);//CRCУ��

    for (i=0;i<EEP_MAX;i++)
    {
        if (EEP_data[i] != EEP_data_last[i])
        {
            same_flag = 1;//���ϴ����ݲ�ͬ
            break;    
        }
    }

    if (same_flag == 0)//����һ����ͬ
    {
        return;
    }
    
    if(EEP_OffSet_DATA_ADDR >= (SECTION_SIZE - EEP_MAX))//31*16=496byte(һҳ512byte)
    {
        IAPPageErase(SYS_DATA_ADDR,IapROM);
        EEP_OffSet_DATA_ADDR = 0;
    }
    
    //д��
    for(i=0; i<EEP_MAX; i++)
    {
        IAPWrite(SYS_DATA_ADDR+i+EEP_OffSet_DATA_ADDR,EEP_data[i],IapROM);
        EEP_data_last[i] = EEP_data[i];
    }

    EEP_OffSet_DATA_ADDR=EEP_OffSet_DATA_ADDR+EEP_MAX;   //ƫ�Ƶ���һ��

    _check_EEP_EN=1;
}

/*********************************************************
������: void Sys_data_check(void)
��  ��: EEP check
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
void Sys_data_check(void)
{
    UI16 i;
    UI08 crc=0;
    UI08 check_EEP_data=0;//����Ƿ�洢��EEP����
    UI16 check_offset=0;//EEP�洢��ƫ����
    if(!_check_EEP_EN)
    {
        return;
    }
    _check_EEP_EN=0;

    for (i=0;i<SECTION_SIZE;i=i+EEP_MAX)
    {
        check_EEP_data = IAPRead(SYS_DATA_ADDR+i,IapROM);
        if(check_EEP_data != 0xAA)//����һ���ֽ�
        {
            if(i == 0 && check_EEP_data != 0xFF && check_EEP_data != 0x00)//û�д������
            {
                IAPPageErase(SYS_DATA_ADDR,IapROM);
                EEP_OffSet_DATA_ADDR = 0;
                break;
            }
            else
            {
                check_offset = i - EEP_MAX;
                break;
            }
        }
    }

    //���¶�����У��
    for(i=0; i<EEP_MAX; i++)
    {
        EEP_data[i]=IAPRead(SYS_DATA_ADDR+i+check_offset,IapROM);
    }

    crc=crc_check(&EEP_data[0],EEP_MAX-1);//CRCУ��
    //У��   || д�ķ�Χ����  �������¿�ʼ
    if((0xaa!=EEP_data[0])||(crc!=EEP_data[EEP_MAX-1]))
    {
        for(i=0; i<EEP_MAX; i++)
        {
            EEP_data_last[i] = 0;
        }
    }
}
/*********************************************************
������: void EEP_deal(void)
��  ��: EEP����
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
void EEP_deal(void)
{
    if(!_MS100_EEP_EN)
    {
        return;
    }

    _MS100_EEP_EN=0;

    if(_check_EEP_EN)
    {
        Sys_data_check();
    }
    Sys_data_write();
}

/**************************************************
*�������ƣ�void WdtInit
*�������ܣ����Ź���ʼ��
*��ڲ�����void
*���ڲ�����void
**************************************************/
void WdtInit(void)
{
    //1--1 -- 00   ?��Code Optionѡ��ENWDT
    WDTCON  = 0x10;//���Ź���ͨ����¼ʱ������(ʾ����ʵ��850ms��λ)
}
/*********************************************************
������: void Sys_Initial(void)
��  ��: ϵͳ��ʼ��
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
void Sys_Initial(void)	//	ϵͳ ��ʼ������
{
    EA = 0;						//�ر��ж�
    IO_init();//�˿ڳ�ʼ��
    delay(6000);
    SysData_init();
    TimerInit();
    TouchKeyInit();
#if ((defined DEBUG_EN) && !DEBUG_EN)
    WdtInit();
#endif
    EA = 1;						//�����ж�
}
/*********************************************************
������: void general_deal(void)
��  ��:
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
void general_deal(void)
{
    prg_s_general();
    // EEP_deal();
}



