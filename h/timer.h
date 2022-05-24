
#ifndef __TIMER_H__
#define __TIMER_H__

#include "DataType.h"

//
extern MCU_xdata UUI08 ms_bit;
#define _ms_for_key ms_bit.bit_.b0
#define _ms_for_IO ms_bit.bit_.b1
#define _ms_for_remote ms_bit.bit_.b2
#define _ms_EXV ms_bit.bit_.b3
#define _Wifi_Uart_ms ms_bit.bit_.b4
#define _ms_for_CT1642 ms_bit.bit_.b5
//
extern MCU_xdata UUI08 ms5_bit;
#define _5ms_for_ADC ms5_bit.bit_.b0
#define _Update_DSP ms5_bit.bit_.b1
#define _5ms_for_key ms5_bit.bit_.b2
//
extern MCU_xdata UUI08 ms10_bit;
#define _Wifi_Uart_ms10 ms10_bit.bit_.b0
#define _LED_ms10 ms10_bit.bit_.b1
#define _ms10_for_key ms10_bit.bit_.b2
#define _ms10_for_key_deal ms10_bit.bit_.b3

//
extern MCU_xdata UUI08 ms100_bit;
#define _txd_ms100_tick ms100_bit.bit_.b0
#define _ms100_for_ADC ms100_bit.bit_.b1
#define _ms100_general ms100_bit.bit_.b2
#define _ms100_for_conl ms100_bit.bit_.b3
#define _ms100_EEP_EN ms100_bit.bit_.b4
#define _ms100_for_key ms100_bit.bit_.b5
#define _ms100_for_IO ms100_bit.bit_.b6
#define _ms100_for_Wifi ms100_bit.bit_.b7
//
extern MCU_xdata UUI08 ms500_bit;
#define _ms500_for_DSP ms500_bit.bit_.b0
#define _ms500_for_ADC ms500_bit.bit_.b1
#define _mS500_EEP_EN ms500_bit.bit_.b2
#define _mS500_for_LED ms500_bit.bit_.b3
//
extern MCU_xdata UUI16 s_bit;
#define _Timer_second s_bit.bit_.b0
#define _s_for_IO s_bit.bit_.b1
#define _s_for_key s_bit.bit_.b2
#define _s_for_conl s_bit.bit_.b3
#define _s_general s_bit.bit_.b4
#define _s_for_AD s_bit.bit_.b5
#define _s_for_Pump s_bit.bit_.b6
#define _SYS_Inspect_s s_bit.bit_.b7
#define _LED_DSP_S s_bit.bit_.b8
#define _Wifi_Uart_s s_bit.bit_.b9
#define _s_event_Updata s_bit.bit_.b10
#define _s_for_Sleep s_bit.bit_.b11
#define _s_for_PM25_sensor s_bit.bit_.b12
//
extern MCU_xdata UUI08 minute_bit;
#define _Control_minute minute_bit.bit_.b0

extern void TimerInit(void);
extern void BTM_Init(void);
extern void PWM_Init(void);
extern void PWM40_Duty(UI08 _duty_buf);
extern void PWM41_Duty(UI08 _duty_buf);
extern void PWM42_Duty(UI08 _duty_buf);
extern void PWM43_Duty(UI08 _duty_buf);

extern void Timer_Deal(void);
extern void prg_ms1(void);

#endif
