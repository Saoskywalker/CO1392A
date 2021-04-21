#include "General.h"

xdata UUI08  Sleep_bit= {0}; //bit
xdata UI08 Sleep_scan_time=0;//扫描时间
xdata UI08 Sleep_delay_time=10;//睡眠延时
xdata UI08 Scl_Low_Count=0;//low计时
xdata UI08 Scl_High_Count=0;//high计时
xdata UI08 IICEN_Count=0;//计时

/***********************************************************
*函 数 名： void prg_s_Sleep()
*功能描述：
*输入参数：  无
*函数返回值：无
***********************************************************/
void  prg_s_Sleep(void)
{
    if(!_s_for_Sleep)
    {
        return;
    }

    _s_for_Sleep=0;

    if(Sleep_delay_time>0)
    {
        Sleep_delay_time--;
    }
}
/*******************************************************************************
待机时。检测数码管屏是否请求接收遥控器数据
*******************************************************************************/
void  Read_Status(void)
{
    if( (_IICSleep_status)
            ||(Sleep_scan_time<1))
    {
        return;
    }

    Sleep_scan_time=0;

    SCL_input;
    if(Scl)
    {
        if(Scl_High_Count<0xff)
        {
            Scl_High_Count++;
        }
        Scl_Low_Count=0;
        if((Scl_High_Count>=3)&&(IICEN_Count>0))
        {
            _Read_Sleep_OK=1;
            IICEN_Count=0;
        }
    }
    else
    {
        if(Scl_Low_Count<0xff)
        {
            Scl_Low_Count++;
        }
        Scl_High_Count=0;
        if((Scl_Low_Count>=3)&&(IICEN_Count<0xff))
        {
            IICEN_Count++;
        }
    }
}
/*******************************************************************************
确认IIC总线是否发给数码管屏数据
*******************************************************************************/
void  Verify_Status(void)
{
    if(Sys_EEP_SYTP==0xff)
    {
        _IICSleep_status=1;
        Sleep_delay_time=10;
        IICEN_Count=0;
        _Read_Sleep_OK=0;
        Scl_Low_Count=0;
        Scl_High_Count=0;
    }

    if((Power_Status==ON)|| //开机
            (Dsp_Time>0)||//待机设置参数
            (Timer_run>0)||// 定时开机
            (Power_Delay_Time>0)||// 首次上电3秒
            (_temp_room_err)||//室温故障
            (_temp_coil_err)||//管温故障
            (Temp_Setting_Time>0))  //温度设置
    {
        _IICSleep_status=1;
        Sleep_delay_time=10;
        IICEN_Count=0;
        _Read_Sleep_OK=0;
        Scl_Low_Count=0;
        Scl_High_Count=0;
    }
    else
    {
        if((IICEN_Count==0)&&(Sleep_delay_time==0))
        {
            _IICSleep_status=0;
        }

        if((_Read_Sleep_OK)&&(!_IICSleep_status))
        {
            _IICSleep_status=1;
            IICEN_Count=0;
            Sleep_delay_time=10;
            _Read_Sleep_OK=0;
        }

        if(!_IICSleep_status)
        {
            SCL_input;
            SDA_input;
        }
    }
}
/**************************************************
*函数名称：void SYS_Sleep_deal(void)
*函数功能：
*入口参数：void
*出口参数：void
**************************************************/
void SYS_Sleep_deal(void)
{
    if(_Self_Test==ENABLE)
    {
        return;
    }

    prg_s_Sleep();
    Read_Status();
    Verify_Status();
}
























