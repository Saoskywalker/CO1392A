#include "General.h"


xdata EXVPARA          EXV_para;//EXV ����
xdata EXV_STATUS       EXV_status;//EXV״̬
xdata FunctionalState  STATUS4_stop_Status;//ֹͣ
xdata UI08             EXV_status_buf=0;//״̬
xdata UI08             step_time=0;//�ٶ�
xdata UI08	       self_step_time=0;//��Ҷ
xdata UI08             step_cnt=0;//����
xdata UI16             excit_time=0;//����ʱ��
xdata UUI08            EXV_bit= {0}; //��־λ
xdata UI08             exv_Self_Test_timer=0;
const UI08 step_tab_self[]= {0x00,0x08,0x04,0x02,0x01}; //�������������������

#if 0
const UI08 step_tab[]= {0x09,0x01,0x03,0x02,0x06,0x04,0x0c,0x08};
#else
const UI08 step_tab[]= {0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09};
#endif

/************************************************* 
 // ��������    : EXV_Data_init
 // ��������    : ���ݳ�ʼ��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void EXV_Data_init(void)
{
    EXV_para.EXV_target=0;
    EXV_para.EXV_now=0;
    EXV_para.step_time=0;
    EXV_para.EXV_inspect=0;//
    EXV_para._swing=DISABLE;
    //EXV_para._Default=ENABLE;//�ڶ������У�DISABLE ��Ҷ���ܣ���Ҷ�ص�Ĭ��λ��
    EXV_para._Default=DISABLE;//�ڶ������У�DISABLE ��Ҷ���ܣ���Ҷֹͣ�ڵ�ǰλ��

    EXV_status=STATUS1;
    STATUS4_stop_Status=DISABLE;
    EXV_status_buf=0xff;
    step_time=0;
    step_cnt=0;
    excit_time=0;
}
/*************************************************
 // ��������    : exv_adjust
 // ��������    :
 // ��ڲ���    :
 // ���ڲ���    : ��
************************************************/
void EXV_off(void)
{
    SwingA_L;
    SwingB_L;
    SwingC_L;
    SwingD_L;
}
/*************************************************
 // ��������    : EXV_position_run
 // ��������    :
 // ��ڲ���    :
 // ���ڲ���    : ��
************************************************/
void EXV_position_run(void)
{
    if(step_tab[step_cnt]&0X01)
    {
        SwingA_H;
    }
    else
    {
        SwingA_L;
    }

    if(step_tab[step_cnt]&0X02)
    {
        SwingB_H;
    }
    else
    {
        SwingB_L;
    }

    if(step_tab[step_cnt]&0X04)
    {
        SwingC_H;
    }
    else
    {
        SwingC_L;
    }

    if(step_tab[step_cnt]&0X08)
    {
        SwingD_H;
    }
    else
    {
        SwingD_L;
    }

}
/*************************************************
 // ��������    : exv_run
 // ��������    :
 // ��ڲ���    : ��
 // ���ڲ���    : ��
************************************************/
//STATUS1 ���ٹر�ȫ��
//STATUS2 ���ٴ�ȫ��
//STATUS3 ����г����ذڶ���Ĭ��λ��
//STATUS4 �ڶ�
//STATUS5 �ڶ��ڶ���Ĭ��λ��
//EXV_para.EXV_target<EXV_para.EXV_now//��ת   EXV_para.EXV_now--;
//EXV_para.EXV_target>EXV_para.EXV_now//��ת   EXV_para.EXV_now++;
void exv_ligic(void)
{
    if(Power_Status==OFF)
    {
        EXV_status=STATUS1;
    }

    switch(EXV_status)
    {
    case STATUS1:
    {
        if(EXV_status_buf!=EXV_status)
        {
            EXV_status_buf=EXV_status;
            EXV_para.step_time=EXV_step_short_time;
            EXV_para.EXV_now=P150;
            EXV_para.EXV_target=0;
        }
        //if((EXV_para.EXV_now==EXV_para.EXV_target)
        //&&(Power_Status==ON) )
        if(Power_Status==ON)//�ػ�ȫ�ع��� ����  ����ȫ�������ٿ�
        {
            EXV_status=STATUS2;
        }
    };
    break;
    case STATUS2:
    {
        if(EXV_status_buf!=EXV_status)
        {
            EXV_status_buf=EXV_status;
            EXV_para.step_time=EXV_step_short_time;
            EXV_para.EXV_now=0;
            EXV_para.EXV_target=P150;
        }

        if(EXV_para.EXV_now==EXV_para.EXV_target)
        {
            EXV_status=STATUS3;
        }
    };
    break;
    case STATUS3:
    {
        if(EXV_status_buf!=EXV_status)
        {
            EXV_status_buf=EXV_status;
            EXV_para.step_time=EXV_step_short_time;
            EXV_para.EXV_target=P155;
        }

        if( (EXV_para.EXV_now==EXV_para.EXV_target)
                &&(EXV_para._swing==ENABLE))
        {
            EXV_status=STATUS4;
        }
    }
    break;
    case STATUS4:
    {
        if(EXV_status_buf!=EXV_status)
        {
            EXV_status_buf=EXV_status;
            EXV_para.step_time=EXV_step_long_time;
            EXV_para.EXV_target=P165;
        }

        if(EXV_para.EXV_now==EXV_para.EXV_target)
        {
            if(EXV_para.EXV_now==P160)
            {
                EXV_para.EXV_target=P165;
            }
            else
            {
                EXV_para.EXV_target=P160;
            }
        }

        if((EXV_para._Default==ENABLE )&&(EXV_para._swing==DISABLE))
        {
            EXV_status=STATUS5;
        }
    }
    break;
    case STATUS5:
    {
        if(EXV_status_buf!=EXV_status)
        {
            EXV_status_buf=EXV_status;
            EXV_para.step_time=EXV_step_short_time;
            EXV_para.EXV_target=P155;
        }

        if((EXV_para._Default==DISABLE)||(EXV_para._swing==ENABLE))
        {
            EXV_status=STATUS4;
        }
    }
    break;
    }
}
/*************************************************
 // ��������    : positive_travel
 // ��������    :
 // ��ڲ���    :
 // ���ڲ���    : ��
************************************************/
void positive_travel(void)
{
    if(step_cnt==0)
    {
        step_cnt=7;
    }
    else
    {
        step_cnt--;
    }
    EXV_para.EXV_now++;
}

/*************************************************
 // ��������    : negative_travel
 // ��������    :
 // ��ڲ���    :
 // ���ڲ���    : ��
************************************************/
void negative_travel(void)
{
    step_cnt++;
    if(step_cnt>=8)
    {
        step_cnt=0;
    }
    EXV_para.EXV_now--;
}
/*************************************************
 // ��������    : exv_run
 // ��������    :
 // ��ڲ���    :
 // ���ڲ���    : ��
************************************************/
void exv_run(void)
{
    if(_Self_Test)//�Լ�
    {
        return;
    }

    if( (EXV_status==STATUS4)&&(EXV_para._swing==DISABLE) )//�ڶ�������ͣ
    {
        excit_time=0;
        EXV_off();

        //EXV_para.EXV_target=P160;//��ͣ�󣬰�Ҷ��Ҫ�ٴδ򿪵����λ�á���������ٴδ򿪵����λ�ã�ֱ�����μ���
        STATUS4_stop_Status=ENABLE ;
        return;
    }

    if(   (EXV_status==STATUS4)
            &&(EXV_para._swing==ENABLE )
            &&(excit_time<500)
            &&(ENABLE ==STATUS4_stop_Status) )
    {
        EXV_position_run();
        return;
    }

    STATUS4_stop_Status=DISABLE;


    if(EXV_para.EXV_now==EXV_para.EXV_target)
    {
        excit_time=0;
        EXV_off();
        return;
    }

    if(step_time<EXV_para.step_time)
    {
        return;
    }
    step_time=0;

    if(excit_time<500)
    {
        EXV_position_run();
        return;
    }
    excit_time=500;

    if(EXV_para.EXV_target>EXV_para.EXV_now)//��ת
    {
        positive_travel();
    }

    if(EXV_para.EXV_target<EXV_para.EXV_now)//��ת
    {
        negative_travel();
    }

    EXV_position_run();

}

/*************************************************
 // ��������    : EXV_inspect
 // ��������    :
 // ��ڲ���    :
 // ���ڲ���    : ��
************************************************/
void EXV_inspect(void)
{
    UI08  buf=0;

    if(self_step_time<8)
    {
        return;
    }
    self_step_time=0;

    if(++exv_Self_Test_timer<15)//����exv_Self_Test_timer�Ĵ�С���Ʋ��Լ�LED�Ƶ������ٶ�
    {
        return;
    }
    exv_Self_Test_timer=0;

    if(++step_cnt>=5)
    {
        step_cnt=1;
    }

    buf=step_tab_self[step_cnt];
    if(buf&0X08)
    {
        SwingA_H;
    }
    else
    {
        SwingA_L;
    }

    if(buf&0X04)
    {
        SwingB_H;
    }
    else
    {
        SwingB_L;
    }

    if(buf&0X02)
    {
        SwingC_H;
    }
    else
    {
        SwingC_L;
    }

    if(buf&0X01)
    {
        SwingD_H;
    }
    else
    {
        SwingD_L;
    }


}
/*************************************************
 // ��������    : exv_control
 // ��������    :
 // ��ڲ���    :
 // ���ڲ���    : ��
************************************************/
void exv_control(void)
{
    //prg_ms_EXV();

    if(_Self_Test)//�Լ�
    {
        EXV_inspect();
        return;
    }

    if(_led_swing_out)
    {
        EXV_para._swing=ENABLE;
    }
    else
    {
        EXV_para._swing=DISABLE;
    }


    exv_ligic();
    //exv_run();�жϵ���
}


