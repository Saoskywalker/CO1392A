
#ifndef __ADC_API_H
#define __ADC_API_H

#include "DataType.h"

extern MCU_xdata ADC_PARA Temp_room;
extern MCU_xdata ADC_PARA Temp_coil;
extern MCU_xdata ADC_PARA Hum_para;//温度变量
extern MCU_xdata ADC_PARA Temp_Comp;//温度变量

extern void Temp_Deal(void);

//extern UI16  *tab_addr[1];
//extern const UI16  tab_length[1];
//extern void  coil_temp_deal(void);
//extern void  room_temp_deal(void);
extern void  Adc_data_init(void);

#endif
