#ifndef _SYS_INSPECT_H
#define _SYS_INSPECT_H

#include "DataType.h"

#define COMP_TEST_OFF_TIME 60 * 3 // 3分钟关
#define COMP_TEST_ON_TIME 60 * 6  // 6分钟开

extern UI08 G_Comp_Test_EN; //开启压缩机测试

void SYS_Inspect_Deal(void);
UI08 SYS_Inspect_Disp(void);

#endif
