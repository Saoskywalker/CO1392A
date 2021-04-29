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
#ifndef __remote_driver_H
#define __remote_driver_H


////===============需要配置变量==================================================

//==================对外接口===============================================================
#define total_byte 7

#define HEAD     104
#define ONE_1    18
#define ZERO_0   9


extern xdata UI08 _Rec_Ok;
extern xdata UI08 rec_data[];
extern xdata UI08 width;
extern xdata UI32 Rec_IO_buf;
extern xdata UI08 rec_bytes;







extern void remote_IRQ(void);
extern void prg_10ms_remote(void);
#endif /* _remote_driver_H */
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
