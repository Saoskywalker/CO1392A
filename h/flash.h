#ifndef __FLASH_H
#define __FLASH_H


#define IapROM     0x00	  //0x00��ѡ��ROM������  0x02��ѡ��EEPROM������

#define SYS_DATA_ADDR  (0xffff-511)       //ϵͳ�����洢(���һҳ)
#define TYPE_DATA_ADDR (0xffff-1023)       //���Ͳ����洢(�����ڶ�)




extern void IAPPageErase(UI16 Addr,UI08 IAPArea);
extern UI08 IAPRead(UI16 Addr,UI08 IAPArea);
extern void IAPWrite(UI16 Addr,UI08 Value,UI08 IAPArea);

#endif

