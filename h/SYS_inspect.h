#ifndef _SYS_INSPECT_H
#define _SYS_INSPECT_H



extern xdata UUI08  SYS_Inspect_bit;
extern xdata UI08   SYS_Inspect_Key_Data;
extern xdata UI08   PowerKey_Delay_Time;
extern xdata UI08   Disp_Temp_Timer;
extern xdata UI16   Comp_Test_Time;
extern xdata UI16   Comp_Test_Key_count;
extern xdata UI16   Comp_Test_Disp_En_Timer;

#define _SYS_Inspect_ms                SYS_Inspect_bit.bit_.b0
//#define _SYS_Inspect_s                 SYS_Inspect_bit.bit_.b1
#define _COMP_TEST_EN                  SYS_Inspect_bit.bit_.b2
#define _Comp_Test_Key_Count_EN        SYS_Inspect_bit.bit_.b3
 //#define _SYS_Inspect_Key_OK            SYS_Inspect_bit.bit_.b4
#define _Comp_Test_Key_First_Status    SYS_Inspect_bit.bit_.b5

#if 1
#define COMP_TEST_OFF_TIME  60*3  // 3分钟关
#define COMP_TEST_ON_TIME   60*6  //6分钟开
#else
#define COMP_TEST_OFF_TIME  3  // 3分钟关
#define COMP_TEST_ON_TIME   6  //6分钟开
#endif

extern void SYS_Inspect_Data_Init(void);
extern void SYS_Inspect_Deal(void);
extern void SYS_Inspect_Disp(void);
extern void disp_temp(void);
extern void SYS_Inspect_ms_general(void);

#endif














