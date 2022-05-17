/**
  ******************************************************************************
  * @file IO_API.h
  *
  ******************************************************************************
  *改文件输入为DO_out[],将其输出到IO口,并返回DI_status,及每个输入输出的持续时间
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IO_API_H
#define __IO_API_H

#include "DataType.h"

#define DO_total 6

extern void prg_s_IO(void);

extern void IO_operate(void);
extern void  io_data_init(void);

extern MCU_xdata DO_PARA     Comp_para;//压缩机
extern MCU_xdata DO_PARA     UVC_para;
extern MCU_xdata DI_PARA     water_full_para;//水箱水位检测
//extern MCU_xdata DI_PARA     pump_sw_para;//副水箱水位检测


#endif /* __IO_API_H */

