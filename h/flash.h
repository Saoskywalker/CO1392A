#ifndef __FLASH_H
#define __FLASH_H


#define IapROM     0x00	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作

#define SYS_DATA_ADDR  (0xffff-511)       //系统参数存储(最后一页)
#define TYPE_DATA_ADDR (0xffff-1023)       //机型参数存储(倒数第二)




extern void IAPPageErase(UI16 Addr,UI08 IAPArea);
extern UI08 IAPRead(UI16 Addr,UI08 IAPArea);
extern void IAPWrite(UI16 Addr,UI08 Value,UI08 IAPArea);

#endif

