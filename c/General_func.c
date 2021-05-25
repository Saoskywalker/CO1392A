//**********************************************************
//文件名称: 	General_func.c
//文件描述: 	单片机底层硬件配置子函数
//项目名称:
//构	建:
//修	改:
//*********************************************************/
#include "General.h"
//系统模式设置变量
xdata MODE_TYPE       Mode_Set;//模式设定
xdata MODE_TYPE       Mode_Buf;//模式设定buf
xdata UI08            Mode_Delay_Timer;//模式设定延时
/////////////////////////////////////////////////////////////
xdata FANSPEED_TYPE    Fan_Set;//风速设定
xdata FANSPEED_TYPE    Fan_Buf;//风速设定buf
xdata FANSPEED_TYPE    Outfan_Speed;//外风机风速
xdata FANSPEED_TYPE    fan_speed;//实际运行风速
xdata UI08             Fan_Delay_Timer;//风速设定延时
xdata UI08             Fan_Delay_disp;//设定显示
/////////////////////////////////////////////////////////////
//设置温度
xdata UI08  Temp_Set_C;//温度设定 C
xdata UI08  Temp_Set_F;//温度设定 F
xdata UI08  Temp_Cool_F;//制冷设定温度
xdata UI08  Temp_Heat_F;//制热设定温度
xdata UI08  Temp_Setting_Time;//温度预设定时间
/////////////////////////////////////////////////////////////
//设置定时时间
xdata UI32        Timer_run;            //定时时间 秒为单位
xdata UI08        Timer_buff;           //00-24小时 时间设定临时变量
xdata UI08        Timer_Setting_Time;   //设定数码管闪烁显示时间/可修改参数时间
xdata UI08        Timer_set;            //00-24小时 时间设定
xdata TIMER_TYPE  Timer_Type;           // 12H/24H

xdata UI08   Key_Number;    //键值
xdata UI16   Key_last;     //按键持续时间变量
/////////////////////////////////////////////////////////////
//系统公用
xdata ONOFF_STATUS     Power_Status;         //开关机变量
xdata ONOFF_STATUS     rec_buf_power2;       //开关机
xdata UI08             write_delay_time;     //写EEP
xdata UI08             Power_Delay_Time;      //系统开机延时

xdata UI16             compensate_delaytime; //温度渐变时间
xdata UI16             light_down_cont;//风机停后显示半亮时间  2016/3/29
xdata UUI08            SYS_bit;   //系统bit
xdata UUI08            SYS_bit1;  //系统bit1
xdata UUI08            SYS_bit2;  //系统bit2
xdata UUI08            SYS_bit3;  //系统bit3
xdata UUI08            SYS_bit4;  //系统bit4
xdata UUI08            SYS_bit5;  //系统bit5
xdata UUI08            SYS_bit6;  //系统bit6
xdata UI08             sleep_Time;//睡眠延时时间
xdata UI08             DE_rec_time;//rec读取
xdata ONOFF_STATUS     LAMP_Status;//灯光
xdata UI08             minute_cont;//Min计数
xdata UI16             Sys_filter_time;	     //室内马达运转时间
/////////////////////////////////////////////////////////////
xdata FAN_TYPE         sys_fan_type;//两速/三速风
xdata CF_TYPE          CF_Status;         // C/F
xdata FORM_TYPE        Sys_Tpye;          //确定多少种运行模式变量
xdata FAN_ORDER_TYPE   Sys_fan_order; //风机调速
xdata UUI08            light_down;//半亮
xdata UI08             water_light_cont;//满水
xdata UI08 Sys_EEP_SYTP;//机型
xdata UI08 Sys_Version_buf;//红外机型
xdata UI08 Buzz_Cnt;//蜂鸣器响多少声
xdata  UI08 EEP_data[EEP_MAX]= {0};
xdata  UI08 EEP_data_last[EEP_MAX]= {0};
xdata UI16 EEP_OffSet_DATA_ADDR = 0;//写EEP偏移的地址(0x00~511,一页为512)
//================================================
//8个数位一组
//================================================

//使用 bit  1                  0
//bit0     冷暖               单冷
//bit1     _F                 _C
//bit2     Auto_Restart       NO_Auto_Restart
//bit3     Hr12               Hr24
//bit4     有管温             无管温
//bit5     有温度补偿         无温度补偿
//bit6     外低风             外高风
//bit7     内三速风           内二速风
//bit8     有低功耗要求       无低功耗要求
code  UI16 mach_type_tab[TYPE_MAX]=
{
    bit0|bit4|bit7|bit8,                //单冷 C N0RESTART hr24  无补偿 外高风 内三速风 /*默认*/
    //Off cycle    机型号
    bit4|bit5|bit7|bit8,                     //单冷 C N0RESTART 外高风 内三速风   NA        /*01*/
    bit4|bit5|bit6|bit7|bit8,                //单冷 C N0RESTART 外低风 内三速风   NA        /*02*/
    bit1|bit2|bit4|bit5|bit7,                //单冷 F  RESTART 外高风 内三速风    10        /*03*/
    bit1|bit2|bit4|bit5|bit6|bit7,           //单冷 F  RESTART 外低风 内三速风    10        /*04*/
    bit1|bit2|bit4|bit5|bit7,                //单冷 F  RESTART 外高风 内三速风    15        /*05*/
    bit1|bit2|bit4|bit5|bit6|bit7,           //单冷 F  RESTART 外低风 内三速风    15        /*06*/
    bit1|bit2|bit4|bit5|bit7,                //单冷 F  RESTART 外高风 内三速风    30        /*07*/
    bit1|bit2|bit4|bit5|bit6|bit7,           //单冷 F  RESTART 外低风 内三速风    30        /*08*/
    bit0|bit4|bit5|bit7|bit8,                //冷暖 C  N0RESTART 外高风 内三速风  NA        /*09*/
    bit0|bit4|bit5|bit6|bit7|bit8,           //冷暖 C  N0RESTART 外低风 内三速风  NA        /*10*/
    bit0|bit2|bit1|bit4|bit5|bit7,           //冷暖 F  RESTART 外高风 内三速风    10        /*11*/
    bit0|bit2|bit1|bit4|bit5|bit6|bit7,      //冷暖 F  RESTART 外低风 内三速风    10        /*12*/
    bit0|bit2|bit1|bit4|bit5|bit7,           //冷暖 F  RESTART 外高风 内三速风    15        /*13*/
    bit0|bit2|bit1|bit4|bit5|bit6|bit7,      //冷暖 F  RESTART 外低风 内三速风    15        /*14*/
    bit0|bit2|bit1|bit4|bit5|bit7,           //冷暖 F  RESTART 外高风 内三速风    30        /*15*/
    bit0|bit2|bit1|bit4|bit5|bit6|bit7,      //冷暖 F  RESTART 外低风 内三速风    30        /*16*/
    bit4|bit5|bit8,                          //单冷 C  N0RESTART 外高风 内二速风  NA        /*17*/
    bit1|bit2|bit4|bit5,                     //单冷 F  RESTART 外高风 内二速风    10        /*18*/
    bit0|bit4|bit5|bit8,                     //冷暖 C  N0RESTART 外高风 内二速风  NA        /*19*/
    bit0|bit2|bit1|bit4|bit5,                //冷暖 F  RESTART 外高风 内二速风    10        /*20*/
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
*函 数 名：  void SysData_init(void)
*功能描述：  变量初始化
*输入参数：  无
*函数返回值：无
***********************************************************/
void SysData_init(void)			//系统寄存器 初始化程序
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
    Power_Delay_Time=3;      //系统开机延时
    compensate_delaytime=0;
    light_down_cont=0;//风机停后显示半亮时间  2016/3/29
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
    Timer_set=8;            //00-24小时 时间设定
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
*函 数 名：  void SYS_type(void)
*功能描述：  机型参数
*输入参数：  无
*函数返回值：无
***********************************************************/
void SYS_type(void)
{
    if(Sys_EEP_SYTP>=TYPE_MAX)
    {
        return;
    }

    if(mach_type_tab[Sys_EEP_SYTP]& bit0)//单冷/冷暖机型
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
    if(mach_type_tab[Sys_EEP_SYTP]&bit3)                   //时间格式
    {
        Timer_Type=Hr12;
    }
    else
    {
        Timer_Type=Hr24;
    }
    if(mach_type_tab[Sys_EEP_SYTP]&bit4)                  //有无管温
    {
        _Tcoil_Enable=ENABLE;
    }
    else
    {
        _Tcoil_Enable=DISABLE;
    }
    if(mach_type_tab[Sys_EEP_SYTP]&bit5)                  //温度补偿
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
*函 数 名：  void SYS_Mach_type_judge(void)
*功能描述：  机型参数
*输入参数：  无
*函数返回值：无
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
        off_cycle_Tmin=Off_cycle_Timer_tab[0];//限制
    }
    else
    {
        Sys_EEP_SYTP=buf;
        off_cycle_Tmin=Off_cycle_Timer_tab[Sys_EEP_SYTP];
    }
    off_cycle_Tmin=off_cycle_Tmin*60;//赋值


    SYS_type();
}

/***********************************************************
*函 数 名：  void prg_s_general(void)
*功能描述：  S时间片
*输入参数：  无
*函数返回值：无
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
        //制热模式下-压缩机&风机打开后显示补偿温度
        if((_heat_temp_compensation)&&(_comp_firston_in_heat))//制热补偿7F
        {
            Troom_dsp=Temp_room.value-7;
            Troom_dsp_com=Troom_dsp;
            Troom_dsp_com_timer=0;//渐变时间清零
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


    //off cycle 计时
    if(PowerKey_off_cycle_Time>0)
    {
        PowerKey_off_cycle_Time--;
    }

}
/***********************************************************
*函 数 名：  void F_C(void)
*功能描述：
*输入参数：  无
*函数返回值：无
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
*函 数 名：  void C_F(void)
*功能描述：
*输入参数：  无
*函数返回值：无
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
*函 数 名：  void delay(void)
*功能描述：
*输入参数：  无
*函数返回值：无
***********************************************************/
void delay(UI16 i)
{
    while(i)
    {
        i--;
    }
}


/***********************************************************
*函 数 名：  void prg_minute(void)
*功能描述：
*输入参数：  无
*函数返回值：无
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
*函 数 名：  void prg_minute_control(void)
*功能描述：
*输入参数：  无
*函数返回值：无
***********************************************************/
void prg_minute_control(void)
{
    Sys_filter_deal();
}
/*********************************************************
函数名: void filter_time_deal(void)
描  述: 滤网计时处理
输入值: 无
输出值: 无
返回值: 无
**********************************************************/
void Sys_filter_deal(void)
{
    UI16 num=0;
    //分钟
    //if(++filter_Minute_Count<60)//一分钟进来一次了
    //{return;}
    //filter_Minute_Count=0;
    //
    if(_EC_Fast_Test==ENABLE)//快测缩时  15000 ==> 25分钟
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
        if(Sys_filter_time%60==0)//每一小时记录一次滤网时间
        {
            write_delay_time=2;
        }
    }
    //
    if(Sys_filter_time>=15000)//滤网清洗时间 250h*60=15000分钟
    {
        write_delay_time=2;
    }
}
/***********************************************************
*函 数 名：  void Sys_data_read_eep(void)
*功能描述：  EEP
*输入参数：  无
*函数返回值：无
***********************************************************/
void Sys_data_read_eep(void)
{
    UI08 buf=0;
    UI16 buf16=0;
    UI16 i;
    UI08 crc=0;
    UI08 check_EEP_data=0;//检查是否存储有EEP数据
    UI16 check_offset=0;//EEP存储的偏移量

    for (i=0;i<SECTION_SIZE;i=i+EEP_MAX)
    {
        check_EEP_data = IAPRead(SYS_DATA_ADDR+i,IapROM);
        if(check_EEP_data != 0xAA)//检查第一个字节
        {
            if(i == 0 && check_EEP_data != 0xFF && check_EEP_data != 0x00)//没有存过数据
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
    crc=crc_check(&EEP_data[0],EEP_MAX-1);//CRC校验
    //校验
    if((0xaa!=EEP_data[0])||(crc!=EEP_data[EEP_MAX-1]))
    {
        _Write_EEP_EN=1;
        return;
    }
    //
    if(EEP_data[9]<=30)//限制 off_cycle_Tmin
    {
        off_cycle_Tmin=EEP_data[9];
        off_cycle_Tmin=off_cycle_Tmin*60;
    }

    buf16=EEP_data[10];//滤网时间 高八位
    buf16=buf16<<8;
    buf=EEP_data[11];//滤网时间 低八位
    buf16=buf16+buf;
    if(buf16!=0xffff)
    {
        Sys_filter_time=buf16;
    }

    if(!_Auto_Restart)
    {
        return;
    }
    //开关机
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
    //模式
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
    //风速  EEP_data[3]>FANOFF
    if((EEP_data[3]>3)||((sys_fan_type==SPEED2)&&(EEP_data[3]==1)))
    {
        Fan_Set=LOW;
    }
    else
    {
        Fan_Set=(FANSPEED_TYPE)(EEP_data[3]);
    }
    Fan_Buf=Fan_Set;
    //制热设定温度
    if(((EEP_data[4]<TempSet_range[3][0])
            ||(EEP_data[4]>TempSet_range[3][1])))
    {
        Temp_Heat_F=72;
    }
    else
    {
        Temp_Heat_F=EEP_data[4];
    }
    //制冷设定温度
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
    //摆页
    if(0==EEP_data[7])
    {
        _led_swing_enable=0;
    }
    else
    {
        _led_swing_enable=1;
    }
    //灯光
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
函数名: void Sys_data_write(void)
描  述: EEP 写
输入值: 无
输出值: 无
返回值: 无
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
    EEP_data[EEP_MAX-1]=crc_check(&EEP_data[0],EEP_MAX-1);//CRC校验

    for (i=0;i<EEP_MAX;i++)
    {
        if (EEP_data[i] != EEP_data_last[i])
        {
            same_flag = 1;//与上次数据不同
            break;    
        }
    }

    if (same_flag == 0)//与上一次相同
    {
        return;
    }
    
    if(EEP_OffSet_DATA_ADDR >= (SECTION_SIZE - EEP_MAX))//31*16=496byte(一页512byte)
    {
        IAPPageErase(SYS_DATA_ADDR,IapROM);
        EEP_OffSet_DATA_ADDR = 0;
    }
    
    //写入
    for(i=0; i<EEP_MAX; i++)
    {
        IAPWrite(SYS_DATA_ADDR+i+EEP_OffSet_DATA_ADDR,EEP_data[i],IapROM);
        EEP_data_last[i] = EEP_data[i];
    }

    EEP_OffSet_DATA_ADDR=EEP_OffSet_DATA_ADDR+EEP_MAX;   //偏移到下一组

    _check_EEP_EN=1;
}

/*********************************************************
函数名: void Sys_data_check(void)
描  述: EEP check
输入值: 无
输出值: 无
返回值: 无
**********************************************************/
void Sys_data_check(void)
{
    UI16 i;
    UI08 crc=0;
    UI08 check_EEP_data=0;//检查是否存储有EEP数据
    UI16 check_offset=0;//EEP存储的偏移量
    if(!_check_EEP_EN)
    {
        return;
    }
    _check_EEP_EN=0;

    for (i=0;i<SECTION_SIZE;i=i+EEP_MAX)
    {
        check_EEP_data = IAPRead(SYS_DATA_ADDR+i,IapROM);
        if(check_EEP_data != 0xAA)//检查第一个字节
        {
            if(i == 0 && check_EEP_data != 0xFF && check_EEP_data != 0x00)//没有存过数据
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

    //重新读出来校验
    for(i=0; i<EEP_MAX; i++)
    {
        EEP_data[i]=IAPRead(SYS_DATA_ADDR+i+check_offset,IapROM);
    }

    crc=crc_check(&EEP_data[0],EEP_MAX-1);//CRC校验
    //校验   || 写的范围已满  擦除从新开始
    if((0xaa!=EEP_data[0])||(crc!=EEP_data[EEP_MAX-1]))
    {
        for(i=0; i<EEP_MAX; i++)
        {
            EEP_data_last[i] = 0;
        }
    }
}
/*********************************************************
函数名: void EEP_deal(void)
描  述: EEP处理
输入值: 无
输出值: 无
返回值: 无
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
*函数名称：void WdtInit
*函数功能：看门狗初始化
*入口参数：void
*出口参数：void
**************************************************/
void WdtInit(void)
{
    //1--1 -- 00   ?可Code Option选择ENWDT
    WDTCON  = 0x10;//看门狗打开通过烧录时配置字(示波器实测850ms复位)
}
/*********************************************************
函数名: void Sys_Initial(void)
描  述: 系统初始化
输入值: 无
输出值: 无
返回值: 无
**********************************************************/
void Sys_Initial(void)	//	系统 初始化程序
{
    EA = 0;						//关闭中断
    IO_init();//端口初始化
    delay(6000);
    SysData_init();
    TimerInit();
    TouchKeyInit();
#if ((defined DEBUG_EN) && !DEBUG_EN)
    WdtInit();
#endif
    EA = 1;						//开总中断
}
/*********************************************************
函数名: void general_deal(void)
描  述:
输入值: 无
输出值: 无
返回值: 无
**********************************************************/
void general_deal(void)
{
    prg_s_general();
    // EEP_deal();
}



