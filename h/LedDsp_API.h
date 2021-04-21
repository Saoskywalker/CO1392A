#ifndef __LEDDSP_API_H
#define __LEDDSP_API_H

#define   FITER_TIMER     15000 //250h
#define  SCREEN_MAX  5


#define Dsp_locate(X,N) (LED_data_buf[X]|=(N))
#define Dsp_RES_locate(X,N) (LED_data_buf[X]&=(~N))

///////////////////////////////////////////////////////////////////////////////
//数码管显示数据定义
#define SEG_total 9
#define COM_total 2
#define led_scan_interval 2
//数码管显示数据定义
#define BIT_A bit5    //          A
#define BIT_B bit6    //       -------
#define BIT_C bit2    //      |       |
#define BIT_D bit0    //    F |       |  B
#define BIT_E bit1    //       ---G---
#define BIT_F bit4    //      |       |  C
#define BIT_G bit3    //    E |       |
#define BIT_P bit7    //       ---D---   P
//
#define DATA_0  BIT_A|BIT_B|BIT_C|BIT_D|BIT_E|BIT_F
#define DATA_1  BIT_B|BIT_C
#define DATA_2  BIT_A|BIT_B|BIT_D|BIT_E|BIT_G
#define DATA_3  BIT_A|BIT_B|BIT_C|BIT_D|BIT_G
#define DATA_4  BIT_B|BIT_C|BIT_F|BIT_G
#define DATA_5  BIT_A|BIT_C|BIT_D|BIT_F|BIT_G
#define DATA_6  BIT_A|BIT_C|BIT_D|BIT_E|BIT_F|BIT_G
#define DATA_7  BIT_A|BIT_B|BIT_C
#define DATA_8  BIT_A|BIT_B|BIT_C|BIT_D|BIT_E|BIT_F|BIT_G
#define DATA_9  BIT_A|BIT_B|BIT_C|BIT_D|BIT_F|BIT_G
#define DATA_A  BIT_A|BIT_B|BIT_C|BIT_E|BIT_F|BIT_G
#define DATA_b  BIT_C|BIT_D|BIT_E|BIT_F|BIT_G
#define DATA_C  BIT_A|BIT_D|BIT_E|BIT_F
#define DATA_E  BIT_A|BIT_D|BIT_E|BIT_F|BIT_G
#define DATA_F  BIT_A|BIT_E|BIT_F|BIT_G
#define DATA_I  BIT_E|BIT_F
#define DATA_i  BIT_E
#define DATA_n  BIT_A|BIT_B|BIT_C|BIT_E|BIT_F
#define DATA_o  BIT_C|BIT_D|BIT_E|BIT_G
#define DATA_t  BIT_D|BIT_E|BIT_F|BIT_G
#define DATA_e  BIT_A|BIT_D|BIT_E|BIT_F|BIT_G
#define DATA_u  BIT_B|BIT_C|BIT_D|BIT_E|BIT_F
#define DATA_P  BIT_A|BIT_B|BIT_E|BIT_F|BIT_G
#define DATA_L  BIT_D|BIT_E|BIT_F
#define DATA_H  BIT_B|BIT_C|BIT_E|BIT_F|BIT_G
#define DATA_r  BIT_E|BIT_G
#define DATA_neg  BIT_G
#define DATA_M1  BIT_A|BIT_B|BIT_E|BIT_F
#define DATA_M2  BIT_A|BIT_B|BIT_C|BIT_F
#define DATA_g   BIT_G
#define DATA_NON 0

extern xdata  UI08  Dsp_Time;
extern xdata  UI08  LED_data[COM_total];
extern code   UI08 BCD_tab[10];  /*LED显示编码，用于查表*/


#define dig1_num LED_data_buf[0]
#define dig2_num LED_data_buf[1]
//#define dig3_num LED_data_buf[2]

#define LED_hfan 		  LED_buf[0].bit_.b2=1     //高速风指示
#define LED_mfan 		  LED_buf[0].bit_.b3=1     //中速风指示
#define LED_lfan 		  LED_buf[1].bit_.b1=1     //低速风指示
#define LED_water 	          LED_buf[0].bit_.b7=1     //满水指示
#define LED_CoolAir   	          LED_buf[0].bit_.b1=1     //冷气指示
#define LED_fan   	          LED_buf[0].bit_.b6=1     //送风指示
#define LED_hum   	          LED_buf[1].bit_.b0=1     //除湿指示
#define LED_HEAT   	          LED_buf[0].bit_.b0=1     //制热指示
#define LED_timer 	          LED_buf[0].bit_.b4=1     //定时指示
#define LED_filter 	          LED_buf[0].bit_.b5=1     //滤网指示
#define LED_CoolAir_off   	  LED_buf[0].bit_.b1=0     //冷气关闭
#define LED_HEAT_off   	          LED_buf[0].bit_.b0=0     //冷气关闭

//自检变量
extern xdata UI08    test_cont1;
extern xdata UI08    test_cont2;
extern xdata UI08    test_seq;
extern xdata UI08    test_key_data;



extern xdata UI08 LED_data_DISP[2];
extern xdata UI08 LED_data_buf[2];

extern void LED_display(void);
extern void prg_ms500_DSP(void);
extern void LED_data_intit(void);
extern UI08 Verify_dat(UI08 *ptr,UI08 len);
extern void disp_All(void);
extern void dig1_2_dsp(UI08 data_buf);

#endif
