#ifndef _Led_driver_H
#define _Led_driver_H

#include "DataType.h"

#define COM_total 3
#define led_scan_interval 4

extern MCU_xdata UI08 LED_data[COM_total]; //数码管显示输出缓存;
extern MCU_xdata UI08 AirQ_Class_LED_out;
extern void LED_COM_OFF(void);
extern void Led_Scan(void);
extern void RBG_out(void);

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
// Designed by caozhongfu 2011.03.30
///////////////////////////////////////////////////////////
/******** (C) COPYRIGHT 2011 GCE -----END OF FILE ********/
