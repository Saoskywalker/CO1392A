/**
  ******************************************************************************
  * @file IO_API.h
  *
  ******************************************************************************
  *���ļ�����ΪDO_out[],���������IO��,������DI_status,��ÿ����������ĳ���ʱ��
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

extern MCU_xdata DO_PARA     Comp_para;//ѹ����
extern MCU_xdata DO_PARA     UVC_para;
extern MCU_xdata DI_PARA     water_full_para;//ˮ��ˮλ���
//extern MCU_xdata DI_PARA     pump_sw_para;//��ˮ��ˮλ���


#endif /* __IO_API_H */

