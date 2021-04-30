#ifndef __LEDDSPANDKEY_DRIVER_H
#define __LEDDSPANDKEY_DRIVER_H


//说明：该函数只对IO模式的引脚进行定义，复用引脚在各个复用模块重新定义
//PMx : 0表示输出(out) 1表示输入(in)
//Pxx : 0表示输出低电平 1表示输出高电平

#define SEG_A_ON        P17=1    //LED7
#define SEG_A_OFF       P17=0

#define SEG_B_ON        P22=1    //LED8
#define SEG_B_OFF       P22=0

#define SEG_C_ON        P16=1    //LED9
#define SEG_C_OFF       P16=0

#define SEG_D_ON        P15=1    //LED6
#define SEG_D_OFF       P15=0

#define SEG_E_ON        P14=1    //LED5
#define SEG_E_OFF       P14=0

#define SEG_F_ON        P12=1    //LED4
#define SEG_F_OFF       P12=0

#define SEG_G_ON        P30=1    //LED1
#define SEG_G_OFF       P30=0

#define SEG_P_ON        P10=1    //LED2
#define SEG_P_OFF       P10=0

/*共阴极*/
#define COMDIG1_ON      P31 = 0   //COM1
#define COMDIG1_OFF     P31 = 1

#define COMDIG2_ON      P32 = 0   //COM2
#define COMDIG2_OFF     P32 = 1

#define COMDIG3_ON      P40 = 0   //COM3 控制LED1、LED2、LED4、LED5、LED6、LED7、LED8、LED9
#define COMDIG3_OFF     P40 = 1

#define COMDIG4_ON      P41 = 1   //COM4 控制LED10、LED11、LED12
#define COMDIG4_OFF     P41 = 0

/*受控于COM4*/  
#define LED10_ON        P23 = 0   //LED10
#define LED10_OFF       P23 = 1

#define LED11_ON        P24 = 0   //LED11
#define LED11_OFF       P24 = 1

#define LED12_ON        P25 = 0   //LED12
#define LED12_OFF       P25 = 1

/*独立IO口控制*/    
#define LED3_BLUE_ON    P43 = 0   //LED3蓝色
#define LED3_BLUE_OFF   P43 = 1

#define LED3_RED_ON     P42 = 0   //LED3红色
#define LED3_RED_OFF    P42 = 1

extern xdata UI16  M_Key_last;//按键按下时间
extern xdata UI16  Read_key_delay;
extern xdata UI08  shake_count;
extern xdata UI08  key_new;//新键值

extern void get_key_number(void);

extern void key_scan(void);
extern void Led_and_Key_scan(void);











#endif

