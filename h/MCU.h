
#ifndef __MCU_H_
#define __MCU_H_

#include "DataType.h"

/*蜂鸣器IO定义*/
#define BUZZ_ON       P37=1
#define BUZZ_OFF      P37=0
////


/*模拟红外通讯IO定义*/
#define Txd_H      P44=1
#define Txd_L      P44=0
#define Rxd_IO     (P45)



#define SEG_A_ON        P35=1
#define SEG_A_OFF       P35=0

#define SEG_B_ON        P34=1
#define SEG_B_OFF       P34=0

#define SEG_C_ON        P33=1
#define SEG_C_OFF       P33=0

#define SEG_D_ON        P32=1
#define SEG_D_OFF       P32=0

#define SEG_E_ON        P20=1
#define SEG_E_OFF       P20=0

#define SEG_F_ON        P17=1
#define SEG_F_OFF       P17=0

#define SEG_G_ON        P15=1
#define SEG_G_OFF       P15=0

#define SEG_H_ON        P14=1    //LED6
#define SEG_H_OFF       P14=0

/*12ò???*/
#define COMDIG1_ON      P16 = 0   //COM1
#define COMDIG1_OFF     P16 = 1

#define COMDIG2_ON      P36 = 0   //COM2
#define COMDIG2_OFF     P36 = 1


#define LED_1_ON        P21=0
#define LED_1_OFF       P21=1

#define LED_2_ON        P10=0
#define LED_2_OFF       P10=1

#define LED_3_ON        P30=0
#define LED_3_OFF       P30=1

#define LED_4_ON        P24=0
#define LED_4_OFF       P24=1

#define LED_5_ON        P31 = 0
#define LED_5_OFF       P31 = 1

#define LED_6_ON        P22 = 0
#define LED_6_OFF       P22 = 1

#define LED_7_ON        P23 = 0
#define LED_7_OFF       P23 = 1

#define LED_8_ON        P25 = 0
#define LED_8_OFF       P25 = 1
////

#define  LED_R_ON       P40 = 1
#define  LED_R_OFF      P40 = 0

#define  LED_G_ON       P41 = 1
#define  LED_G_OFF      P41 = 0

#define  LED_B_ON       P42 = 1
#define  LED_B_OFF      P42 = 0

#define  GREEN_LED_Duty(_duty_buf)   PWM40_Duty(_duty_buf)
#define  BLUE_LED_Duty(_duty_buf)    PWM41_Duty(_duty_buf)
#define  RED_LED_Duty(_duty_buf)     PWM42_Duty(_duty_buf)



////
extern void IAP_Reset();
#define MCU_RESET  IAP_Reset()


extern MCU_xdata UI08        Power_Delay_Time;
extern MCU_xdata FuncState   _Self_Test;

extern void  IO_Init(void);
#endif
























