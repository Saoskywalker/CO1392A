#include "General.h"

UI08 G_Comp_Test_EN = 0;          //����ѹ��������
static UI08 Comp_Test_Disp_En_Timer = 0; //���������ʾһ��ʱ��
static UI16 S_Comp_Test_Time = 0;        //��ʱ

/*********************************************************
������: SYS_Inspect_s_general
��  ��: Sʱ��Ƭ
����ֵ: ��
���ֵ: ��
����ֵ: ��
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
// �������� : Comp_Test_General
// ����˵�� : ѹ����ǿ�Ʋ���ģʽ�߼�
// ��ڲ��� :
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : ����Ǭ
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
static void Comp_Test_General(void)
{
    static UI08 s_comp_test_step = 0;

    if ((SYS_Power_Status == OFF) //��̨�ػ�
        || (Sys_Err.Water_Full)   //��̨����ˮ��
        || (Sys_Err.comm_err)     //ͨ�Ź���
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
������: SYS_Inspect_Disp
��  ��: �������ʱ��ʾһ��ʱ��
����ֵ: ��
���ֵ: ��
����ֵ: ��
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
������: SYS_Inspect_Deal
��  ��: �����������
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
void SYS_Inspect_Deal(void)
{
    SYS_Inspect_s_general();
    Comp_Test_General();
}
