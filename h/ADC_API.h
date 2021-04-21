#ifndef __ADC_API_H
#define __ADC_API_H


#define ADC_channel_total    2

/////////////////////////////////////////////////////////////
//ADC采样变量
extern xdata ADC_PARA    Temp_room;
extern xdata ADC_PARA	  Temp_coil;
extern xdata ADC_PARA	  Tcomp_coil;
extern xdata UI08        Troom_dsp;//温度显示变量
extern xdata UI08        Troom_dsp_com;//温度显示变量
extern xdata UI08        Troom_dsp_com_timer;//温度显示变量
extern xdata UI08        Troom_update_time;//温度显示更新变量
//范围0-64
#define  LED_LUX1   6
#define  LED_LUX2   14
#define  LED_LUX3   22
#define  LED_LUX4   32
#define  LED_LUX5   64


#define  LED_LUX_V1   8
#define  LED_LUX_V2   16
#define  LED_LUX_V3   24
#define  LED_LUX_V4   32
#define  LED_LUX_V5   40


extern void Temp_Deal(void);
extern void ADC_Data_Init(void);


#endif

