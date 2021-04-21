#ifndef __TIMER_API_H
#define __TIMER_API_H










//
extern xdata UUI08            SYS_ms1_bit;
#define   _rec_ms                 SYS_ms1_bit.bit_.b0
#define   _ms_EXV                 SYS_ms1_bit.bit_.b1

extern xdata UUI08            SYS_ms10_bit;
#define _ms10_for_key_deal        SYS_ms10_bit.bit_.b0

extern xdata UUI08            SYS_ms20_bit;
#define _ms20_for_key_deal  SYS_ms20_bit.bit_.b0

//100ms
extern xdata UUI08            SYS_ms100_bit;
#define    _txd_ms100_tick             SYS_ms100_bit.bit_.b0
#define      _ms100_ADC                SYS_ms100_bit.bit_.b1
#define      _ms100_disp               SYS_ms100_bit.bit_.b2
#define     _MS100_EEP_EN              SYS_ms100_bit.bit_.b3
#define     _ms100_EEP_EN              SYS_ms100_bit.bit_.b3
#define     _ms100_for_PUMP            SYS_ms100_bit.bit_.b4
#define     _ms100_for_control         SYS_ms100_bit.bit_.b5
#define     _ms100_for_IO              SYS_ms100_bit.bit_.b6

//500ms
extern xdata UUI08            SYS_ms500_bit;
#define    _led_ms500_tick             SYS_ms500_bit.bit_.b0
#define      _ms500_ADC                SYS_ms500_bit.bit_.b1

//1S
extern xdata UUI08            SYS_S_bit;
#define      _s_for_ADC         SYS_S_bit.bit_.b0
#define     _s_for_IO           SYS_S_bit.bit_.b1
#define     _s_for_conl         SYS_S_bit.bit_.b2
#define     _s_general          SYS_S_bit.bit_.b3
#define     _s_key              SYS_S_bit.bit_.b4
#define     _SYS_Inspect_s      SYS_S_bit.bit_.b5
#define     _s_for_Sleep        SYS_S_bit.bit_.b6
















extern  xdata UI08  step_time;
extern  xdata UI16  excit_time;
extern  xdata UI08  self_step_time;//°ÚÒ¶







extern void prg_ms1(void);

extern void TimerInit(void);

void timer_data_init(void);



#endif

