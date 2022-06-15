/********************************************************************************
  // 文件名称:   LedDsp_API.h
  // 功能描述:   驱动程序功能设计
  //            (管理硬件设备层面上数据读写的协议实现，驱动程序的封装、接口设计)
*********************************************************************************/
#ifndef _LedDsp_API_H
#define _LedDsp_API_H
#include "Led_driver.h"

#include "DataType.h"

//==============================================================================
//      2) 变量声明(需要配置变量)
//==============================================================================
//数码管显示数据定义
#define BIT_A bit6 //          A
#define BIT_B bit7 //       -------
#define BIT_C bit0 //      |       |
#define BIT_D bit2 //    F |       |  B
#define BIT_E bit1 //       ---G---
#define BIT_F bit4 //      |       |  C
#define BIT_G bit5 //    E |       |
#define BIT_P bit3 //       ---D---   P
/******************************************************************************
 *         数码管显示代码定义
 *******************************************************************************/
#define DATA_0 BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F
#define DATA_1 BIT_B | BIT_C
#define DATA_2 BIT_A | BIT_B | BIT_D | BIT_E | BIT_G
#define DATA_3 BIT_A | BIT_B | BIT_C | BIT_D | BIT_G
#define DATA_4 BIT_B | BIT_C | BIT_F | BIT_G
#define DATA_5 BIT_A | BIT_C | BIT_D | BIT_F | BIT_G
#define DATA_6 BIT_A | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_7 BIT_A | BIT_B | BIT_C
#define DATA_8 BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_9 BIT_A | BIT_B | BIT_C | BIT_D | BIT_F | BIT_G
#define DATA_A BIT_A | BIT_B | BIT_C | BIT_E | BIT_F | BIT_G
#define DATA_b BIT_C | BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_C BIT_A | BIT_D | BIT_E | BIT_F
#define DATA_E BIT_A | BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_F BIT_A | BIT_E | BIT_F | BIT_G
#define DATA_I BIT_E | BIT_F
#define DATA_i BIT_E
#define DATA_n BIT_A | BIT_B | BIT_C | BIT_E | BIT_F
#define DATA_o BIT_C | BIT_D | BIT_E | BIT_G
#define DATA_t BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_e BIT_A | BIT_D | BIT_E | BIT_F | BIT_G
#define DATA_U BIT_B | BIT_C | BIT_D | BIT_E | BIT_F
#define DATA_P BIT_A | BIT_B | BIT_E | BIT_F | BIT_G
#define DATA_L BIT_D | BIT_E | BIT_F
#define DATA_H BIT_B | BIT_C | BIT_E | BIT_F | BIT_G
#define DATA_r BIT_E | BIT_G
#define DATA_neg BIT_G
#define DATA_M1 BIT_A | BIT_B | BIT_E | BIT_F
#define DATA_M2 BIT_A | BIT_B | BIT_C | BIT_F
#define DATA_NON 0
//------------------------

extern MCU_xdata UI08 LED_data_buf[COM_total]; // LED显示data
#define dig1_num LED_data_buf[0]
#define dig2_num LED_data_buf[1]

#define Dsp_RESlocate(X, N) (LED_data_buf[X] &= ~(N))
#define Dsp_locate(X, N) (LED_data_buf[X] |= (N))

#define WATER_locate Dsp_locate(2, bit2)
#define UVC_locate Dsp_locate(2, bit0)
#define WIFI_locate Dsp_locate(2, bit1)
#define DRY_locate Dsp_locate(2, bit3)
#define TIMER_locate Dsp_locate(2, bit4)
#define DRY_Clothes_locate Dsp_locate(2, bit5)
#define NC_LED1 Dsp_locate(2, bit6)
#define NC_LED2 Dsp_locate(2, bit7)

#define Decimal_point_off   \
  {                         \
    Dsp_RESlocate(1, bit3); \
    Dsp_RESlocate(0, bit3); \
  }

extern MCU_xdata UI16 wifi_net_led_timer; // 30分钟
#define WIFI_NET_TIMER 6                  // 30分钟

extern MCU_xdata UI08 _Flash_500ms;
extern MCU_xdata UI08 Disp_Delay;
extern MCU_xdata UI08 Disp_set_Delay;
extern MCU_xdata UI08 G_Disp_Machine_Temp_Time; //进入快测时，显示室温管温的时间

extern MCU_xdata RGB_LED_TYPE RGB_Count;
//==============================================================================
//      3) 函数声明(对外接口)
//==============================================================================
extern void LED_display(void);
extern void prg_ms500_DSP(void);
extern void Disp_All(void);
extern void dig1_2_dsp(UI08 _disp_data);

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
