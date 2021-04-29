#include "General.h"

void get_key_number(void);
void key_scan(void);
void Led_and_Key_scan(void);
void LED_out(UI08 led_data);

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
    // UI32 key_tmp = 0;
    if(SOCAPI_TouchKeyStatus&0x80)	    //��Ҫ����2:  ������ɨ��һ�ֱ�־���Ƿ����TouchKeyScan()һ��Ҫ���ݴ˱�־λ�����
    {
        SOCAPI_TouchKeyStatus &= 0x7f;	//��Ҫ����3: �����־λ�� ��Ҫ�ⲿ�����
        exKeyValueFlag = TouchKeyScan();//�������ݴ�����
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
    //!SW1->TK0
    //!SW2->TK1
    //!SW3->TK2
    //!SW4->TK3
    //!SW5->TK4
    //!SW6->TK24
    //!SW7->TK23
    //!SW8->TK22
    Sys_Scan();
    key_data=(UI08)(((exKeyValueFlag>>22)<<5) | (exKeyValueFlag)&0x1F);

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
    if(com < 3)
    {
        if(buf.bit_.b0) {SEG_A_ON;} else {SEG_A_OFF;}
        if(buf.bit_.b1) {SEG_B_ON;} else {SEG_B_OFF;}
        if(buf.bit_.b2) {SEG_C_ON;} else {SEG_C_OFF;}
        if(buf.bit_.b3) {SEG_D_ON;} else {SEG_D_OFF;}
        if(buf.bit_.b4) {SEG_E_ON;} else {SEG_E_OFF;}
        if(buf.bit_.b5) {SEG_F_ON;} else {SEG_F_OFF;}
        if(buf.bit_.b6) {SEG_G_ON;} else {SEG_G_OFF;}
        if(buf.bit_.b7) {SEG_P_ON;} else {SEG_P_OFF;}
    }
    else if(com == 3)
    {
        if(buf.bit_.b0) {LED10_ON;} else {LED10_OFF;}
        if(buf.bit_.b1) {LED11_ON;} else {LED11_OFF;}
        if(buf.bit_.b2) {LED12_ON;} else {LED12_OFF;}
    }
}

void LED_out(UI08 led_data)
{
    UUI08 buf;
    buf.byte=led_data;
    if(buf.bit_.b0) {LED3_BLUE_ON;   } else {LED3_BLUE_OFF; }
    if(buf.bit_.b1) {LED3_RED_ON;  } else {LED3_RED_OFF;}
}

/*************************************************
 // ��������    : void LED_COM_OFF(void)
 // ��������    : �ر�com
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void LED_COM_OFF(void)
{
    COMDIG1_OFF;
    COMDIG2_OFF;
    COMDIG3_OFF;
    COMDIG4_OFF;
    // LED3_BLUE_OFF;
    // LED3_RED_OFF;
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
        LED_COM_OFF();
    }
    if(++led_scan_time<led_scan_interval)
    {
        return;
    }
    led_scan_time=0;
    
    LED_COM_OFF();

    LED_out(LED_out_buf.byte);
    if(led_scan_position < COM_total)    
    {
        SEGNum_out(Display_out_buf[led_scan_position].byte,led_scan_position);
        if(led_scan_position==0)      {COMDIG1_ON;}    //COM1
        else if(led_scan_position==1) {COMDIG2_ON;}    //COM2   
        else if(led_scan_position==2) {COMDIG3_ON;}    //COM3
        else if(led_scan_position==3) {COMDIG4_ON;}    //COM4     
    }

    if (++led_scan_position >= COM_total)
    {
        led_scan_position = 0;   
    }
}

