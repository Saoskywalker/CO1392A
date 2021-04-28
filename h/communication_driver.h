/*
 * @文件内容描述: 
 * @作者: GCE
 * @Date: 2021-04-22 16:37:12
 * @LastEditTime: 2021-04-28 12:33:05
 * @最后编辑: 赵军
 * @参考: 
 */

#ifndef _COMMUNICATION_DRIVER_H
#define _COMMUNICATION_DRIVER_H

#define     COM_HEAD   15
#define     COM_ONE    9
#define     COM_ZERO   4


#define     rece_data_lenth  8
#define     txd_length      8
extern xdata UI08 txd_buf[txd_length];
extern xdata UI08 rxd_buf[rece_data_lenth];

extern xdata UI08 pulse_cnt;


extern xdata UUI08  communication_bit1;
#define  _txd_en       communication_bit1.bit_.b0
#define  _txd_start    communication_bit1.bit_.b1
#define  _txd_end      communication_bit1.bit_.b2
#define  _txd_data     communication_bit1.bit_.b3
#define  _Rxd_start    communication_bit1.bit_.b4
#define  _Rxd_OK       communication_bit1.bit_.b5
#define  _txd_tick     communication_bit1.bit_.b6

#define  _Rxd_IO_     communication_bit1.bit_.b7


extern  UI08 communication_width_count;

#define Txd_H   P01=1
#define Txd_L   P01=0

#define Rxd_IO   (P02)




void communication_Txd_IRQ(void);
void communication_Rxd_IRQ(void);


void communication_Rxd_IO_init(void);

void comm_data_init(void);

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







