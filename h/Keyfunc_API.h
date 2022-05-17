#ifndef __key_function_H
#define __key_function_H

#include "DataType.h"

#define Dry_Clothes_key 0x01 //干衣
#define power_key 0x02       //
#define dry_key 0x03
#define set_timer_key 0x04
#define Child_key 0x06
#define fast_test_key 0x07
#define UVC_key 0x08

#define hum_and_clothes 0x05
#define timer_and_hum 0x0C

#define LAMP_key 0x09

//组合长按键

#define self_test_key 22
#define EC_Fast_key 30
#define non_key 0xff

#define TK_0 0x00000001
#define TK_1 0x00000002
#define TK_2 0x00000004
#define TK_3 0x00000008

#define TK_4 0x00000010
#define TK_5 0x00000020
#define TK_6 0x00000040
#define TK_7 0x00000080

#define TK_8 0x00000100
#define TK_9 0x00000200
#define TK_10 0x00000400
#define TK_11 0x00000800

#define TK_12 0x00001000
#define TK_13 0x00002000
#define TK_14 0x00004000
#define TK_15 0x00008000

#define TK_16 0x00010000
#define TK_17 0x00020000
#define TK_18 0x00040000
#define TK_19 0x00080000

#define TK_20 0x00100000
#define TK_21 0x00200000
#define TK_22 0x00400000
#define TK_23 0x00800000

#define TK_24 0x01000000
#define TK_25 0x02000000
#define TK_26 0x04000000
#define TK_27 0x08000000

#define TK_28 0x10000000
#define TK_29 0x20000000
#define TK_30 0x40000000
#define TK_31 0x80000000

#define EEP_write_T 2
#define Child_Lock_Disp_NUM 3

extern MCU_xdata UUI08 M_Key_flag;
#define _KEY_OK M_Key_flag.bit_.b0

extern MCU_xdata UI08 M_Key_Number;
extern MCU_xdata UI16 M_Key_last;
extern MCU_xdata UI08 shake_count;
extern MCU_const UI08 Timer_SET[6];
extern MCU_const UI08 Hum_SET[5];

extern void prg_ms_key(void);
extern void Rest_Key_Buzzer(void);
extern void Key_Deal(void);
extern void prg_s_key(void);
extern void prg_ms10_key(void);
#endif /* __key_function_H*/
