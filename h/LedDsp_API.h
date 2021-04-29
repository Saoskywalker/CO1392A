#ifndef __LEDDSP_API_H
#define __LEDDSP_API_H

    
#define   FITER_TIMER     15000 //250h
#define  SCREEN_MAX  5

///////////////////////////////////////////////////////////////////////////////
//数码管显示数据定义
#define SEG_total 9
#define COM_total 4
#define led_scan_interval 2
//数码管显示数据定义
#define BIT_A bit1    //          A
#define BIT_B bit0    //       -------
#define BIT_C bit4    //      |       |
#define BIT_D bit6    //    F |       |  B
#define BIT_E bit5    //       ---G---
#define BIT_F bit2    //      |       |  C
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
extern code   UI08 BCD_tab[10];  /*LED显示编码，用于查表*/

/*显示变量声明*/
extern xdata  UUI08 Display_data_buf[COM_total];
extern xdata  UUI08 LED_data_buf;

extern xdata  UUI08  Display_out_buf[COM_total];
extern xdata  UUI08 LED_out_buf;
    



#define dig1_num            Display_data_buf[0].byte          //数码管高位
#define dig2_num            Display_data_buf[1].byte          //数码管低位
#define led1_num            Display_data_buf[2].byte          //LED灯显示1    LED1、LED2、LED4、LED5、LED6、LED7、LED8、LED9
#define led2_num            Display_data_buf[3].byte          //LED灯显示2    LED10、LED11、LED12

#define LED_hfan 		    Display_data_buf[3].bit_.b0=1     //高速风指示    LED10
#define LED_mfan 		    Display_data_buf[2].bit_.b2=1     //中速风指示    LED9  
#define LED_lfan 		    Display_data_buf[2].bit_.b1=1     //低速风指示    LED8  
#define LED_water 	        Display_data_buf[3].bit_.b2=1     //满水指示      LED12
#define LED_CoolAir   	    LED_data_buf.bit_.b0=1            //冷气指示      LED3 蓝灯
#define LED_fan   	        Display_data_buf[2].bit_.b4=1     //送风指示      LED5  
#define LED_hum   	        Display_data_buf[2].bit_.b5=1     //除湿指示      LED4  
#define LED_HEAT   	        LED_data_buf.bit_.b1=1            //制热指示      LED3_红灯
#define LED_timer 	        Display_data_buf[2].bit_.b7=1     //定时指示      LED2  
#define LED_filter 	        LED_data_buf.bit_.b7=1            //滤网指示      无
#define LED_CoolAir_off     LED_data_buf.bit_.b0=0            //冷气关闭      LED3 蓝灯
#define LED_HEAT_off        LED_data_buf.bit_.b1=0            //制热关闭      LED3_红灯
#define LED_Sleep_on        Display_data_buf[2].bit_.b3=1     //睡眠灯开      LED6  
#define LED_Sleep_off       Display_data_buf[2].bit_.b3=0     //睡眠灯关      LED6
#define LED_UvcOn           Display_data_buf[2].bit_.b6=1     //UVC灯         LED1  
#define LED_UvcOff          Display_data_buf[2].bit_.b6=0     //UVC灯         LED1 
#define LED_MuteOn          Display_data_buf[2].bit_.b0=1     //静音模式      LED7  
#define LED_MuteOff         Display_data_buf[2].bit_.b0=0     //静音模式      LED7 
#define LED_SwingOn         Display_data_buf[3].bit_.b1=1     //摆叶指示      LED11
#define LED_SwingOff        Display_data_buf[3].bit_.b1=0     //摆叶指示      LED11



//自检变量
extern xdata UI08    test_cont1;
extern xdata UI08    test_cont2;
extern xdata UI08    test_seq;
extern xdata UI08    test_key_data;

extern void LED_display(void);
extern void prg_ms500_DSP(void);
extern void LED_data_intit(void);
extern UI08 Verify_dat(UI08 *ptr,UI08 len);
extern void disp_All(void);
extern void dig1_2_dsp(UI08 data_buf);

#endif
