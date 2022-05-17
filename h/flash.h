#ifndef __FLASH_H
#define __FLASH_H

#include "DataType.h"

extern void IAPPageErase(UI16 Addr, UI08 IAPArea);
extern UI08 IAPRead(UI16 Addr, UI08 IAPArea);
extern void IAPWrite(UI16 Addr, UI08 Value, UI08 IAPArea);

#endif
