#ifndef __DATA_EEP_H_
#define __DATA_EEP_H_

#include "DataType.h"

#define IapROM 0x00 // 0x00��ѡ��ROM������  0x02��ѡ��EEPROM������

#define IapCheckNum_ADDR (0xffff - 511) //�洢�����Ƿ���������(���һҳ)

#define SYS_DATA_ADDR (0xffff - 1023) //���Ͳ����洢(�����ڶ�)

#define SECTION_SIZE (512) //������С

#define EEP_MAX 20

union U_EEP
{ /* unsigned 16bit RAM */
	UI08 EEP_data[EEP_MAX];
	struct
	{
		UI08 E_Head; //������ʼ
		UI08 E_SYS_Power_Status;
		UI16 E_SYS_filter_time;
		UI08 E_SYS_Mode;		   //ϵͳģʽ
		UI08 E_SYS_DYR_Tyde;	   //��������
		UI08 E_SYS_HUN_Tyde;	   //��ʪ����
		UI08 E_SYS_Hum_Set;		   //ʪ���趨
		UI08 E_SYS_Fan_Tyde;	   //���з���
		UI08 E_SYS_Swing_Tyde;	   //��Ҷ����
		UI08 E_SYS_Air_Clear_Tyde; //������������
		UI08 E_LED_Disp_Status;	   //�ƹ��Ƿ��
		UI08 E_Buzzer_Status;	   //�������Ƿ�ʹ��
		UI08 E_Child_Lock_status;  //ͯ��
		UI08 E_buf1;
		UI08 E_buf2;
		UI08 E_buf3;
		UI08 E_buf4;
		UI08 E_buf5;
		UI08 E_CRC;
	} S_data;
};
typedef union U_EEP union_EEP_Data;

extern MCU_xdata SYS_CONFIG Sys_Config;
extern MCU_xdata UI16 EEP_OffSet_DATA_ADDR; //дEEPƫ�Ƶĵ�ַ(0x00~511,һҳΪ512)
extern MCU_xdata UI08 Write_Memory_Data_Delay;
extern void Sys_data_read_eep(void);

extern void EEP_deal(void);

#endif
