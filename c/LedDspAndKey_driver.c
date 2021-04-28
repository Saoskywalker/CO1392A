#include "General.h"

void get_key_number(void);
void key_scan(void);
void Led_and_Key_scan(void);

xdata UI08 key_data=0;//��������
xdata UI08 key_old=0;//�ɼ�ֵ
xdata UI08 shake_count=0;//��ֵȷ�ϼ���
xdata UI08 led_scan_time=0;//LED scanʱ��
xdata UI08  key_new=0;//�¼�ֵ
xdata UI16  M_Key_last=0;//��������ʱ��
xdata UI16  Read_key_delay=0;//��ϰ����󣬵ȴ�Nʱ�����ʶ�𰴼�����������ʱʶ�𵽰���
xdata UI08 led_scan_position=0;//ɨ���N��

xdata UI32 exKeyValueFlag = 0;		//��ǰ�ְ�����־
/**************************************************
*�������ƣ�void  Sys_Scan(void)
*�������ܣ�ɨ��TK����ʾ
*��ڲ�����void
*���ڲ�����void
**************************************************/
void Sys_Scan(void)
{
    UI32 key_tmp = 0;
    if(SOCAPI_TouchKeyStatus&0x80)	    //��Ҫ����2:  ������ɨ��һ�ֱ�־���Ƿ����TouchKeyScan()һ��Ҫ���ݴ˱�־λ�����
    {
        SOCAPI_TouchKeyStatus &= 0x7f;	//��Ҫ����3: �����־λ�� ��Ҫ�ⲿ�����
        exKeyValueFlag = TouchKeyScan();//�������ݴ�����
        key_tmp = (exKeyValueFlag >> 22) & 0x07;//SW6,SW7,SW8
        exKeyValueFlag = (exKeyValueFlag & 0x1F) | ((((key_tmp>>2)&0x01)|(((key_tmp>>1)&0x01)<<1)|(((key_tmp>>0)&0x01))<<2) << 4);
        TouchKeyRestart();				//������һ��ת��
    }
}
/*************************************************
 // ��������    : LedDsp_manage
 // ��������    :
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void get_key_number(void)
{

    UI08 i=0;
    UI08 key_total_count=0;
    UI08 number=0;

    if(Read_key_delay)//�����곤����  ���ֺ�nms�ٶ�
    {
        return;
    }
    //
    Sys_Scan();
    key_data=(UI08)((exKeyValueFlag&0x7F800000)>>23);

    _SYS_Inspect_Key_OK=1;
    SYS_Inspect_Key_Data=key_data;

    for(i=0; i<8; i++)
    {
        if(key_data&(UI08)(0x01<<i))
        {
            key_total_count++;
            number=i+1;
        }
    }

    if(key_total_count==0)
    {
        if((shake_count==2)&&(!_long_key_en))
        {
            Key_Number=key_old;
        }
        _long_key_en=0;//������ʹ����,�˴ζ̰�����Ч
        shake_count=0;
        Key_last=0;
        return;
    }
    else if(key_total_count==1)
    {
        key_new=number;
    }
    else if(key_total_count==2)
    {
        if((key_data==_c_f)&&( Key_last>3000))
        {
            Key_Number=_c_f;
            Key_last=0;
            shake_count=0;
            Read_key_delay=50;
            _long_key_en=1;//������ʹ����,�˴ζ̰�����Ч
        }
    }
    else
    {
        shake_count=0;
        Key_last=0;
        return;
    }
    ///
    if(key_new!=key_old)
    {
        key_old=key_new;
        shake_count=0;
        //M_Key_last=0;
        return;
    }
    //
    if(shake_count<2)
    {
        shake_count++;
        if((shake_count>=2)&&(key_total_count==1))
        {
            //M_Key_Number=key_old;
            //M_Key_last=0;
            ;//�ɿ���Ч
        }
    }
    //�����ƹⰴ��
    if((Key_last>=3000)&&(Key_last<=3500)&&(key_new==lamp_key))
    {
        if(Sys_filter_time>FITER_TIMER)
        {
            _long_key_en=1;//������ʹ����,�˴ζ̰�����Ч
            Key_Number=filter_key;
            shake_count=0;
            Key_last=0xffff;
            Read_key_delay=50;
        }
    }
    //������ʱ����
    if((Key_last>=10000)&&(Key_last<=10500)&&(key_new==timer_key))
    {
        Key_Number=EC_Fast_key;
        Key_last=0xffff;
        shake_count=0;
        Read_key_delay=50;
        _long_key_en=1;//������ʹ����,�˴ζ̰�����Ч
    }
    //
    if((Key_last>=5000)&&(Key_last<=5500)&&(PowerKey_off_cycle_Time>0)&&(Temp_Set_F==77)&&(key_new==fan_key))
    {
        Key_Number=OFF_CYCLE_KEY;
        Key_last=0xffff;
        shake_count=0;
        Read_key_delay=50;
        _long_key_en=1;//������ʹ����,�˴ζ̰�����Ч
    }
}
/*************************************************
 // ��������    : SEGNum_Scan
 // ��������    :
 // ��ڲ���    : UI08 data    UI08 com--8bitÿ��
 // ���ڲ���    : ��
***************************************************/
void SEGNum_out(UI08 Data,UI08 com)
{
    UUI08 buf;
    buf.byte=Data;
    if(com==0)
    {
        if(buf.bit_.b0)//LED1
        {
            SEG_A_ON;
        }
        else
        {
            SEG_A_OFF;
        }

        if(buf.bit_.b1)//LED2
        {
            SEG_B_ON;
        }
        else
        {
            SEG_B_OFF;
        }

        if(buf.bit_.b2)//LED3
        {
            SEG_C_ON;
        }
        else
        {
            SEG_C_OFF;
        }

        if(buf.bit_.b3)//LED4
        {
            SEG_D_ON;
        }
        else
        {
            SEG_D_OFF;
        }

        if(buf.bit_.b4)//LED6
        {
            SEG_F_ON;
        }
        else
        {
            SEG_F_OFF;
        }

        if(buf.bit_.b5)//LED7
        {
            SEG_G_ON;
        }
        else
        {
            SEG_G_OFF;
        }

        if(buf.bit_.b6)//LED8
        {
            SEG_H_ON;
        }
        else
        {
            SEG_H_OFF;
        }

        //����
        if(buf.bit_.b7)//LED10
        {
            SEG_J_ON;
        }
        else
        {
            SEG_J_OFF;
        }
    }
    else  if(com==1)
    {
        if(buf.bit_.b0)
        {
            SEG_I_ON;//LED9
        }
        else
        {
            SEG_I_OFF;
        }

        if(buf.bit_.b1)
        {
            SEG_E_ON;//LED5
        }
        else
        {
            SEG_E_OFF;
        }
    }
}

/*************************************************
 // ��������    : Led_and_Key_scan
 // ��������    : �˺�����prg_ms1()��ִ��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Led_and_Key_scan(void)
{
    if(light_down.byte!=0)
    {
        SEGNum_out(0,0);// data���� ȫ��
        SEGNum_out(0,1);
    }
    if(++led_scan_time<led_scan_interval)
    {
        return;
    }
    led_scan_time=0;
    //
    led_scan_position^=1;
    if(led_scan_position==0)
    {
        SEGNum_out(LED_data_DISP[led_scan_position],led_scan_position);
    }
    else if(led_scan_position==1)
    {
        SEGNum_out(LED_data_DISP[led_scan_position],led_scan_position);
    }
}

