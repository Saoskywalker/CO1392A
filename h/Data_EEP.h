#ifndef __DATA_EEP_H_
#define __DATA_EEP_H_

#include "DataType.h"

#define IapROM 0x00 // 0x00：选择ROM区操作  0x02：选择EEPROM区操作

#define IapCheckNum_ADDR (0xffff - 511) //存储程序是否升级正常(最后一页)

#define SYS_DATA_ADDR (0xffff - 1023) //机型参数存储(倒数第二)

#define SECTION_SIZE (512) //扇区大小

#define EEP_MAX 20

union U_EEP
{ /* unsigned 16bit RAM */
	UI08 EEP_data[EEP_MAX];
	struct
	{
		UI08 E_Head; //数据起始
		UI08 E_SYS_Power_Status;
		UI16 E_SYS_filter_time;
		UI08 E_SYS_Mode;		   //系统模式
		UI08 E_SYS_DYR_Tyde;	   //干衣类型
		UI08 E_SYS_HUN_Tyde;	   //除湿类型
		UI08 E_SYS_Hum_Set;		   //湿度设定
		UI08 E_SYS_Fan_Tyde;	   //运行风速
		UI08 E_SYS_Swing_Tyde;	   //摆叶类型
		UI08 E_SYS_Air_Clear_Tyde; //空气净化类型
		UI08 E_LED_Disp_Status;	   //灯光是否打开
		UI08 E_Buzzer_Status;	   //蜂鸣器是否使能
		UI08 E_Child_Lock_status;  //童锁
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
extern MCU_xdata UI16 EEP_OffSet_DATA_ADDR; //写EEP偏移的地址(0x00~511,一页为512)
extern MCU_xdata UI08 Write_Memory_Data_Delay;
extern void Sys_data_read_eep(void);

extern void EEP_deal(void);

#endif
