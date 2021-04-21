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

#define              DI_read_interval    100

extern xdata UI08          DI_read_time;


/////////////////////////////////////////////////////////////
//IO���
#define  DO_total 10

extern xdata DO_PARA  Comp_para,
       Way4_para,
       Pump_para,
       Hfan_para,
       Mfan_para,
       Lfan_para,
       OutHfan_para,
       OutLfan_para,
       Power_para,//���ؿ�����ʱ���У���Ҷ����
       DCPUMP_para;

extern void IO_data_Init(void);
extern void IO_init(void);
extern void prg_s_IO(void);
extern void IO_operate(void);
extern void IO_selftest(void);


#endif /* __IO_API_H */
