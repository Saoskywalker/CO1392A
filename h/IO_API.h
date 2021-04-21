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

#define              DI_read_interval    100

extern xdata UI08          DI_read_time;


/////////////////////////////////////////////////////////////
//IO输出
#define  DO_total 10

extern xdata DO_PARA  Comp_para,
       Way4_para,
       Pump_para,
       Hfan_para,
       Mfan_para,
       Lfan_para,
       OutHfan_para,
       OutLfan_para,
       Power_para,//负载开机延时运行，摆叶例外
       DCPUMP_para;

extern void IO_data_Init(void);
extern void IO_init(void);
extern void prg_s_IO(void);
extern void IO_operate(void);
extern void IO_selftest(void);


#endif /* __IO_API_H */
