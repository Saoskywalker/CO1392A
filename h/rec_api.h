#ifndef __REC_API_H
#define __REC_API_H

#define SCREEN_REC_LEN   8
#define VERIFY_REC_LEN   7


extern xdata UI08      rec_data[SCREEN_REC_LEN];
extern xdata UI08 Screen_rec_data[SCREEN_REC_LEN];


extern void set_cool_temp_chack(void);
extern void set_heat_temp_chack(void);
extern void rec_decode(void);
extern void REC_data_init(void);




typedef struct REC
{
    MODE_TYPE mode;
    FANSPEED_TYPE fan;
    UI08 temp;
    CF_TYPE cf;
    UI08 sleep;
    ONOFF_STATUS power;
    UI08 timer_f;
    UI08 timer_time;
} rec_T;














#endif

