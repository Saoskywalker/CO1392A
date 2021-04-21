
#ifndef _COMMUNICATION_API_H
#define _COMMUNICATION_API_H

typedef enum
{
    BUZZ_TYPE1=0,
    BUZZ_TYPE2=1,
    BUZZ_TYPE3=2,
    BUZZ_TYPE4=3,
    BUZZ_TYPE5=4,
    BUZZ_TYPE6=5,
    BUZZ_TYPE7=6,
    BUZZ_TYPE8=7,
    BUZZ_TYPE9=8,
    BUZZ_OFF=9
}
BUZZ_TYPE;

extern  xdata UUI08  COM_bit0;

extern  xdata UI08   communication_timer;
extern  xdata UI08   Txd_count;
extern  xdata UI08   Buz_list;
extern  xdata UI08   Txd_timer;
//#define  _swing_status       COM_bit0.bit_.b0
#define  _Sleep_status       COM_bit0.bit_.b1
#define  _Sleep_status_buf   COM_bit0.bit_.b2
#define  _INFAN_ERR          COM_bit0.bit_.b3
#define  _OUTFAN_ERR         COM_bit0.bit_.b4
#define  communication_ERR   COM_bit0.bit_.b5




//+20
#define  FAN_LOW_SPEED    770UL
#define  FAN_MID_SPEED    870UL
#define  FAN_HIG_SPEED    970UL

#define  OUTFAN_LOW_SPEED   1020UL
#define  OUTFAN_HIG_SPEED   1320UL

#define  SLEEP_TIMER  16


//extern bool receiv_OK;
extern void   communication_data_init(void);
extern void rxd_data_protocl(void);
extern void txd_data_protocol(void);
//extern UI08 check_sum(UI08 *point,UI08 len);
extern void Buz_mode(UI08 mode_buf);
extern void  sleep_deal(void);
extern void communication_Deal(void);
extern UI08 crc_check(UI08 *buffer,UI08 length);

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







