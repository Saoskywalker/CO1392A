#ifndef __LEDDSPANDKEY_DRIVER_H
#define __LEDDSPANDKEY_DRIVER_H


//˵�����ú���ֻ��IOģʽ�����Ž��ж��壬���������ڸ�������ģ�����¶���
//PMx : 0��ʾ���(out) 1��ʾ����(in)
//Pxx : 0��ʾ����͵�ƽ 1��ʾ����ߵ�ƽ

#define SEG_A_ON       P15=0
#define SEG_A_OFF      P15=1

#define SEG_B_ON       P16=0
#define SEG_B_OFF      P16=1

#define SEG_C_ON       P25=0
#define SEG_C_OFF      P25=1

#define SEG_D_ON       P24=0
#define SEG_D_OFF      P24=1

#define SEG_E_ON       P23=0
#define SEG_E_OFF      P23=1

#define SEG_F_ON       P22=0
#define SEG_F_OFF      P22=1

#define SEG_G_ON       P21=0
#define SEG_G_OFF      P21=1

#define SEG_H_ON       P20=0
#define SEG_H_OFF      P20=1

#define SEG_I_ON       P17=0
#define SEG_I_OFF      P17=1

#define SEG_J_ON       P26=0
#define SEG_J_OFF      P26=1

extern xdata UI16  M_Key_last;//��������ʱ��
extern xdata UI16  Read_key_delay;
extern xdata UI08  shake_count;
extern xdata UI08  key_new;//�¼�ֵ

extern void get_key_number(void);

extern void key_scan(void);
extern void Led_and_Key_scan(void);











#endif

