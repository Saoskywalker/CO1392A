#include "General.h"

UI08 G_Comp_Test_EN = 0;          //开启压缩机测试
static UI08 Comp_Test_Disp_En_Timer = 0; //进入测试显示一段时间
static UI16 S_Comp_Test_Time = 0;        //计时

/*********************************************************
函数名: SYS_Inspect_s_general
描  述: S时间片
输入值: 无
输出值: 无
返回值: 无
**********************************************************/
static void SYS_Inspect_s_general(void)
{
    if (!_1S_For_For_SYS)
    {
        return;
    }

    if (G_Comp_Test_EN)
    {
        if (S_Comp_Test_Time < 0Xffff)
            S_Comp_Test_Time++;
        if (Comp_Test_Disp_En_Timer < 0Xffff)
            Comp_Test_Disp_En_Timer++;
    }
    else
    {
        S_Comp_Test_Time = 0;
        Comp_Test_Disp_En_Timer = 0;
    }
}

// *****************************************************************************
// 函数名称 : Comp_Test_General
// 功能说明 : 压缩机强制测试模式逻辑
// 入口参数 :
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
static void Comp_Test_General(void)
{
    static UI08 s_comp_test_step = 0;

    if ((SYS_Power_Status == OFF) //机台关机
        || (Sys_Err.Water_Full)   //机台发生水满
        || (Sys_Err.comm_err)     //通信故障
    )
    {
        G_Comp_Test_EN = 0;
    }

    if (!G_Comp_Test_EN)
    {
        Comp_Test_Disp_En_Timer = 0;
        S_Comp_Test_Time = 0;
        s_comp_test_step = 0;
        return;
    }

    switch (s_comp_test_step)
    {
    case 0:
    {
        Comp_para.OUT = ON;
        Fan_Speed_Out_Buf = HIGH_FAN;

        if (S_Comp_Test_Time >= COMP_TEST_ON_TIME)
        {
            S_Comp_Test_Time = 0;
            s_comp_test_step = 1;
        }
    }
    break;
    case 1:
    {
        Comp_para.OUT = OFF;
        Fan_Speed_Out_Buf = OFF_FAN;

        if (S_Comp_Test_Time >= COMP_TEST_OFF_TIME)
        {
            S_Comp_Test_Time = 0;
            s_comp_test_step = 0;
        }
    }
    break;
    }
}

/*********************************************************
函数名: SYS_Inspect_Disp
描  述: 进入测试时显示一段时间
输入值: 无
输出值: 无
返回值: 无
**********************************************************/
UI08 SYS_Inspect_Disp(void)
{
    if (!G_Comp_Test_EN)
        return 0;

    if (Comp_Test_Disp_En_Timer <= 1)
    {
        Disp_All();
        return 1;
    }
    else
    {
        return 0;
    }
}

/*********************************************************
函数名: SYS_Inspect_Deal
描  述: 测试输出处理
输入值: 无
输出值: 无
返回值: 无
**********************************************************/
void SYS_Inspect_Deal(void)
{
    SYS_Inspect_s_general();
    Comp_Test_General();
}
