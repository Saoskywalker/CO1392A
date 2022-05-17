#include "General.h"



MCU_static MCU_xdata UI08 led_scan_time=0;//LED扫描时间
MCU_static MCU_xdata UI08 led_scan_position=0;//LED扫描位

MCU_xdata UI08 LED_data[COM_total]={0,0,0};    //数码管显示输出缓存
MCU_xdata RGB_LED_TYPE AirQ_Class_LED_out=0;
/*************************************************
 // 函数名称    : SEGNum_Scan
 // 功能描述    :
 // 入口参数    : UI08 data    UI08 com--8bit每段
 // 出口参数    : 无
***************************************************/
void SEGNum_out(UI08 _data_buf)
{
    if(_data_buf&bit0)
    {SEG_A_ON;}
    else
    {SEG_A_OFF;}

    if(_data_buf&bit1)
    {SEG_B_ON;}
    else
    {SEG_B_OFF;}

    if(_data_buf&bit2)
    {SEG_C_ON;}
    else
    {SEG_C_OFF;}

    if(_data_buf&bit3)
    {SEG_D_ON;}
    else
    {SEG_D_OFF;}

    if(_data_buf&bit4)
    {SEG_E_ON;}
    else
    {SEG_E_OFF;}

    if(_data_buf&bit5)
    {SEG_F_ON;}
    else
    {SEG_F_OFF;}

    if(_data_buf&bit6)
    {SEG_G_ON;}
    else
    {SEG_G_OFF;}

    if(_data_buf&bit7)
    {SEG_H_ON;}
    else
    {SEG_H_OFF;}
}

/*************************************************
 // 函数名称    : LED_out
 // 功能描述    :
 // 入口参数    : UI08 data    UI08 com--8bit每段
 // 出口参数    : 无
***************************************************/
void LED_out(UI08 _data_buf)
{
    if(_data_buf&bit0)
    {LED_1_ON;}
    else
    {LED_1_OFF;}

    if(_data_buf&bit1)
    {LED_2_ON;}
    else
    {LED_2_OFF;}

    if(_data_buf&bit2)
    {LED_3_ON;}
    else
    {LED_3_OFF;}

    if(_data_buf&bit3)
    {LED_4_ON;}
    else
    {LED_4_OFF;}

    if(_data_buf&bit4)
    {LED_5_ON;}
    else
    {LED_5_OFF;}

    if(_data_buf&bit5)
    {LED_6_ON;}
    else
    {LED_6_OFF;}

    if(_data_buf&bit6)
    {LED_7_ON;}
    else
    {LED_7_OFF;}

    if(_data_buf&bit7)
    {LED_8_ON;}
    else
    {LED_8_OFF;}
 }


void RBG_out(void)
{
      //AirQ_Class_LED_out =  ORANGE_LED;
    if(AirQ_Class_LED_out==OFF_LED)
    {
    	 BLUE_LED_Duty(0);
    	 GREEN_LED_Duty(0);
    	 RED_LED_Duty(0);

    }
    else if(AirQ_Class_LED_out==GREEN_LED)
    {
    	BLUE_LED_Duty(0);
    	GREEN_LED_Duty(100);
    	RED_LED_Duty(0);
    }
    else if(AirQ_Class_LED_out==YELLOW_LED)
    {
    	BLUE_LED_Duty(0);
    	GREEN_LED_Duty(40);
    	RED_LED_Duty(100);
    }
    else if(AirQ_Class_LED_out==RED_LED)
    {
    	BLUE_LED_Duty(0);
    	GREEN_LED_Duty(0);
    	RED_LED_Duty(100);
    }
    else if(AirQ_Class_LED_out==BLUE_LED)
    {
    	BLUE_LED_Duty(100);
    	GREEN_LED_Duty(0);
    	RED_LED_Duty(0);
    }
    else if(AirQ_Class_LED_out==ORANGE_LED)
    {
    	BLUE_LED_Duty(0);
    	GREEN_LED_Duty(15);
    	RED_LED_Duty(100);
    }
    else if(AirQ_Class_LED_out==ALL_ON_LED)
    {
    	BLUE_LED_Duty(100);
    	GREEN_LED_Duty(100);
    	RED_LED_Duty(100);
    }

}
/*************************************************
 // 函数名称    : void LED_COM_OFF(void)
 // 功能描述    : 关闭com
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void LED_ALL_OFF(void)
{
    COMDIG1_OFF;
    COMDIG2_OFF;
}

/*************************************************
 // 函数名称    : Led_scan
 // 功能描述    : 此函数在prg_ms1()中执行
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Led_Scan(void)
{
    led_scan_time++;
    if(led_scan_time<led_scan_interval)
    {
        return;
    }
    led_scan_time=0;


    LED_ALL_OFF();

    if(led_scan_position==0)//COM1
    {
    	LED_out(0X00);
    	SEGNum_out(LED_data[led_scan_position]);
    	COMDIG1_ON;
    }
    else if(led_scan_position==1)//COM2
    {
        SEGNum_out(LED_data[led_scan_position]);
        COMDIG2_ON;
    }
    else if(led_scan_position==2)//COM3
    {
    	LED_out(LED_data[led_scan_position]);
    }

    led_scan_position++;
    if (led_scan_position >= COM_total)
    {
        led_scan_position = 0;
    }

}





