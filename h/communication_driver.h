
#ifndef _COMMUNICATION_DRIVER_H
#define _COMMUNICATION_DRIVER_H


#include "DataType.h"

#define     rece_data_lenth  11
#define     txd_length      5

extern MCU_xdata UI08 txd_buf[txd_length];
extern MCU_xdata UI08 rxd_buf[rece_data_lenth];
extern MCU_xdata UI08 pulse_cnt;


extern MCU_xdata UUI08  communication_bit1;
#define  _txd_en       communication_bit1.bit_.b0 //����ʹ��
#define  _txd_start    communication_bit1.bit_.b1 //��ʼ����
#define  _txd_end      communication_bit1.bit_.b2 //���ͽ���
#define  _txd_data     communication_bit1.bit_.b3 //��������
#define  _Rxd_start    communication_bit1.bit_.b4 //��ʼ����
#define  _Rxd_OK       communication_bit1.bit_.b5 //�������
#define  _txd_tick     communication_bit1.bit_.b6 //���ͼ��
#define  _Rxd_IO_buf   communication_bit1.bit_.b7 //�������ݵ�IO buf


extern void Communication_INT_Deal(void);
extern void communication_Rxd_IO_init(void);
#endif
//////////////////////////////////////////////////////////
//
//      eeeeeeeeee       n           nn      ddddddddd
//     ee               nnn         nn      dd       dd
//    ee               nn   n      nn      dd        dd
//   eeeeeeeeee       nn     n    nn      dd         dd
//  ee               nn       n  nn      dd         dd
// ee               nn         nnn      dd         dd
//eeeeeeeeee       nn          nn      ddddddddddd
//
//Designed by Benkye.Zhang 2010.10.28
///////////////////////////////////////////////////////
