#ifndef _COMMUNICATION_API_H
#define _COMMUNICATION_API_H

#include "DataType.h"

extern MCU_xdata UI08 communication_timer;
//
extern MCU_xdata UUI08 sys_bit4;
#define _Sleep_status sys_bit4.bit_.b1     //睡眠状态
#define _Sleep_status_buf sys_bit4.bit_.b2 //睡眠状态buf
#define _INFAN_ERR sys_bit4.bit_.b3        //内风机故障
#define _OUTFAN_ERR sys_bit4.bit_.b4       //外风机故障
#define communication_ERR sys_bit4.bit_.b5 //通信故障
// 200为100%
#define SILENCE_FAN_SPEED 64 // 10  1392A没用到
#define HIGH_FAN_SPEED 164   // 74
#define LOW_FAN_SPEED 142    // 66
#define OFF_FAN_SPEED 0

#define SLEEP_TIMER 16

extern MCU_xdata UI08 M_sleep_Time;
extern void rxd_data_protocl(void);
extern void txd_data_protocol(void);
UI08 crc_check(UI08 *buffer, UI08 length);

extern void Buz_mode(UI08 mode_buf);
extern void sleep_deal(void);
extern void communication_Deal(void);
extern UI16 Temp_filter(UI16 _dat);

#endif

//////////////////////////////////////////////////////////
//
//      eeeeeeeeee       n           nn      ddddddddd
//     ee               nnn         nn      dd       dd
//    ee               nn   n      nn      dd        dd
//   eeeeeeeeee       nn     n    nn      dd         dd
//  ee               nn       n  nn      dd         dd
// ee               nn         nnn      dd         dd
// eeeeeeeeee       nn          nn      ddddddddddd
//
// Designed by Benkye.Zhang 2010.10.28
///////////////////////////////////////////////////////
