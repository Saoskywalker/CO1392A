#ifndef _EXV_DEVICE_H
#define _EXV_DEVICE_H

#define SWING_MODE1   1


#define EXV_step_short_time  12  //3//ms
#define EXV_step_long_time   48//20//ms  5的倍数
//#define degree45 400
//#define degree80 800

/*
       .P150
      .      .P160
     .     .    .P155
    .    .    .       .P165
   .   .    .     .
  .  .   .    .
 . . .
.....................
*/



#define P150  7000  //maximum available
#define P155  7000  //Default position
#define P160  7000  //positive travel
#define P165  5000  //negative travel



#define SwingA_H   P05=1
#define SwingA_L   P05=0

#define SwingB_H   P06=1
#define SwingB_L   P06=0

#define SwingC_H   P52=1
#define SwingC_L   P52=0

#define SwingD_H   P53=1
#define SwingD_L   P53=0




typedef struct
{
    UI16 EXV_target;
    UI16 EXV_now;
    UI08  step_time;
    UI08  EXV_inspect;//
    FunctionalState _swing;
    FunctionalState _Default;
} EXVPARA;
extern  xdata EXVPARA EXV_para;

typedef enum
{
    STATUS1=0,
    STATUS2=1,
    STATUS3=2,
    STATUS4=3,
    STATUS5=4
} EXV_STATUS;


//==============================================================================
//      3) 函数声明(对外接口)
//==============================================================================


void exv_control(void);

void EXV_Data_init(void);
extern void exv_run(void);



#endif

