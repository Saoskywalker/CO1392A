/**
  ******************************************************************************
  * @file remote.h
  *
  ******************************************************************************
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __remote_API_H
#define __remote_API_H

#include "remote_driver.h"
////===============需要配置变量==================================================
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
}rec_T;

extern   xdata UUI08 rec_bit;
#define    _rec_ok     rec_bit.bit_.b0
#define    _first_rec  rec_bit.bit_.b1



extern   xdata UI08           buzz_delay_time;
extern   xdata UI08           M__Func_Test_realfeel;
extern   xdata UI08           Remote_key_flag;
extern   xdata ONOFF_STATUS   rec_buf_power2;
extern   xdata UI08           Self_Test_DATA;
//==================对外接口===============================================================
extern void Remote_Deal(void);
extern void Remote_init(void);
extern void sys_reset(void);
extern void Settings_Reset(void);

#endif /* _remote_API_H */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
