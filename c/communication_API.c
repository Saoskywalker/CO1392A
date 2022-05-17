#include "General.h"

MCU_xdata UUI08 sys_bit4 = {0};           //��־λ����
MCU_xdata UI08 communication_timer = 180; //ͨ�Ź���ʱ��
MCU_xdata UI08 M_sleep_Time = 10;         //˯��ʱ��
/*************************************************
 // ��������    : sleep_deal
 // ��������    : ˯�ߴ���
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/

void sleep_deal(void)
{
    if ((SYS_Power_Status) || (M_Dsp_Time > 0) || (M__Self_Test))
    {
        M_sleep_Time = 10;
        _Sleep_status = 0;
        _Sleep_status_buf = 0;
    }
    else
    {
        communication_timer = 180;
    }
}

/*************************************************
 // ��������    : crc_check
 // ��������    : CRCУ��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
UI08 crc_check(UI08 *buffer, UI08 length)
{
    UI08 crc = 0;
    UI08 loop = 0;
    UI08 loop_1 = 0;
    for (loop = 0; loop < length; loop++)
    {
        crc ^= buffer[loop];
        for (loop_1 = 0; loop_1 < 8; loop_1++)
        {
            if ((crc & 1) == 1)
            {
                crc = crc >> 1;
                crc ^= 0x8C;
            }
            else
            {
                crc = crc >> 1;
            }
        }
    }
    return crc;
}

/*************************************************
 // ��������    : rxd_data_protocl
 // ��������    : ͨ�Ž��մ���
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void rxd_data_protocl(void)
{

    UI08 i;
    UI16 value_buf = 0;

    if (M__Self_Test)
    {
        communication_timer = 2;
    }
    else
    {
        communication_timer = 180;
    }
    Sys_Err.comm_err = DISABLE;

    if (rxd_buf[1] & bit0)
    {
        water_full_para.Status = DI_CUT;
    }
    else
    {
        water_full_para.Status = DI_SHORT;
    }

    if (rxd_buf[1] & bit2) //�������
    {
        Sys_Err.pan_motor_err = ENABLE;
    }
    else
    {
        Sys_Err.pan_motor_err = DISABLE;
    }

    //��Դ��ظ�����ָ��
    if (rxd_buf[1] & bit1)
    {
        _Sleep_status_buf = 1;
    }
    else
    {
        _Sleep_status_buf = 0;
    }

    //����ADֵ
    Temp_coil.AD_value = (UI16)(rxd_buf[2] & 0x03);
    Temp_coil.AD_value = (Temp_coil.AD_value << 8) + rxd_buf[3];
    Temp_coil.AD_value = Temp_coil.AD_value >> 1; // 9λ
    //ѹ�����¶�ADֵ        10λ
    Temp_Comp.AD_value = (UI16)(rxd_buf[2] & 0x0c);
    Temp_Comp.AD_value = (Temp_Comp.AD_value << 6) + rxd_buf[4];

    //����SHT30-DIS ����ֵ
    Temp_room.AD_value = rxd_buf[5];
    Temp_room.AD_value = (Temp_room.AD_value << 8) + rxd_buf[6];

    Hum_para.value = rxd_buf[7] & 0X7F;
    Hum_para.value = Temp_filter(Hum_para.value);
    if (rxd_buf[7] & bit7)
    {
        Sys_Err.temp_room_err = ENABLE;
        Sys_Err.hum_Sensor_err = ENABLE;
    }
    else
    {
        Sys_Err.temp_room_err = DISABLE;
        Sys_Err.hum_Sensor_err = DISABLE;
    }
    //�������  ��ʼ��������
    M_ADC_check = 1;

    // clear buff
    for (i = 0; i < rece_data_lenth; i++)
        rxd_buf[i] = 0;
}
/*************************************************
 // ��������    : txd_data_protocol
 // ��������    : ͨ�ŷ��ʹ���
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void txd_data_protocol(void)
{
    UUI08 buf = {0};
    txd_buf[0] = 0xaa;
    //
    if ((SYS_Power_Status) || (M__Self_Test))
    {
        buf.byte = 0;
        if (SYS_Power_Status)
        {
            buf.bit_.b0 = 1;
        } //���ػ�or pump_para==1
        //
        if (Comp_para.OUT == ON)
        {
            buf.bit_.b1 = 1;
        } //ѹ����
        //
        if (M__Self_Test == 1)
        {
            buf.bit_.b3 = 1;
        } //�Լ�

        if (Swing_Tyde_Out == AUTO_SWING)
        {
            buf.bit_.b5 = 0;
            buf.bit_.b4 = 0;
        }
        else if (Swing_Tyde_Out == LEFT_RIGHT_SWING)
        {
            buf.bit_.b5 = 0;
            buf.bit_.b4 = 1;
        }
        else if (Swing_Tyde_Out == UP_DOWN_SWING)
        {
            buf.bit_.b5 = 1;
            buf.bit_.b4 = 0;
        }
        else if (Swing_Tyde_Out == STOP_SWING)
        {
            buf.bit_.b5 = 1;
            buf.bit_.b4 = 1;
        }

        if (UVC_para.OUT)
        {
            buf.bit_.b7 = 1;
        }
        txd_buf[1] = buf.byte;
        //�����λ
        buf.byte = 0;
        buf.bit_.b5 = 1;
        if (Fan_Speed_Out == HIGH_FAN)
        {
            txd_buf[3] = (UI08)(HIGH_FAN_SPEED);
        }
        else if (Fan_Speed_Out == SILENCE_FAN)
        {
            txd_buf[3] = (UI08)(SILENCE_FAN_SPEED);
        }
        else if (Fan_Speed_Out == LOW_FAN)
        {
            txd_buf[3] = (UI08)(LOW_FAN_SPEED);
        }
        else
        {
            txd_buf[3] = 0;
        }

        txd_buf[2] |= bit7; //���15V
    }
    else
    {
        txd_buf[1] = 0;
        txd_buf[2] = 0;
        txd_buf[3] = 0;

        //��Ҷģʽ
        buf.bit_.b5 = 1;
        buf.bit_.b4 = 1;
        txd_buf[1] = buf.byte;
    }

    //˯��
    if (_Sleep_status) //����Դ�巢˯�߱�־
    {
        txd_buf[1] |= bit2;
    }

    txd_buf[txd_length - 1] = crc_check(txd_buf, txd_length - 1);
    _txd_data = 0;
    _txd_end = 0;
    pulse_cnt = 0;
    _txd_start = 1;
    _txd_en = 1;
}
/*************************************************
 // ��������    : communication_txd
 // ��������    : ͨ�ŷ��ʹ���
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void communication_txd(void)
{
    if (!_txd_tick)
    {
        return;
    }
    _txd_tick = 0;
    //
    if ((_Sleep_status_buf) && (_Sleep_status) && (!M__Self_Test))
    {
        return;
    }
    txd_data_protocol();
}
/*************************************************
 // ��������    : communication_rxd
 // ��������    : ͨ�Ž��մ���
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void communication_rxd(void)
{
    UI08 sum = 0;
    if (!_Rxd_OK)
    {
        return;
    }
    _Rxd_OK = 0;
    //
    if (rxd_buf[0] != 0xaa)
    {
        return;
    }
    sum = crc_check(rxd_buf, rece_data_lenth - 1);
    //
    rxd_buf[0] = 0;
    if (rxd_buf[rece_data_lenth - 1] != sum)
    {
        return;
    }
    //
    rxd_data_protocl();
}
/*************************************************
 // ��������    : communication_Deal
 // ��������    : ͨ�Ŵ���
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void communication_Deal(void)
{
    sleep_deal();
    communication_rxd();
    communication_txd();
}

#define FILTER_MAX_NUM 5
xdata UI16 S_filter_value[FILTER_MAX_NUM] = {0};
xdata UI08 S_filter_flag = 0;
xdata UI08 S_filter_i = 0;

UI16 Temp_filter(UI16 _dat)
{

    UI08 count = 0;
    UI32 filter_sum = 0;
    S_filter_value[S_filter_i++] = _dat;
    if (S_filter_i == FILTER_MAX_NUM) //
    {
        S_filter_i = 0; //��������һ�������飬�Ƚ��ȳ�
        S_filter_flag = 1;
    }
    if (S_filter_flag)
    {
        for (count = 0; count < FILTER_MAX_NUM; count++)
        {
            filter_sum += S_filter_value[count];
        }
        return (UI16)(filter_sum / FILTER_MAX_NUM);
    }
    else //û����������һ������
    {
        for (count = 0; count < S_filter_i; count++)
        {
            filter_sum = filter_sum + S_filter_value[count];
        }
        return (UI16)(filter_sum / S_filter_i);
    }
}
