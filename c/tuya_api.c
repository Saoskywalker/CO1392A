#include "General.h"

xdata UUI16 flag_wifi = {0};          //��־λ
xdata UI08 M_wifi_rssi_dsp = 0;       // wifi �ź�ǿ��
xdata UI08 all_data_update_point = 0; //ȫ�������ϱ�ʱ�ļ���ֵ
xdata UI08 upload_filter_status = 0;
xdata UI08 upload_defrost_status = 0;
xdata UI08 upload_hepa_filter = 0;

xdata UI08 Wifi_value_compare_delay = 0;                    //app��������ʱ�Ա�����
xdata WIFI_UPDATE Wifi_Updata;                              // WIFI���ݸ��µ�����
xdata FANSPEED_TYPE Updata_speed = OFF_FAN;                 //�ϴ��ķ���
static xdata UI08 upload_err_status = 0;                    //�ϴ��Ĵ���״̬
static xdata UI08 mcu_dp_type = 0;                          //�ϱ����ݵ�DP
xdata UI08 Wifi_Rssi_DSP_delay_time = 0;                    //
static xdata UI08 Wifi_Comm_Err_delay = WIFI_COMM_ERR_TIME; // wifiͨ�Ź���
static xdata UI08 Wifi_Comm_Err_Dsp_time = 0;               // wifiͨ�Ź�����ʾʱ��
static xdata UI08 Enter_LOW_POWER_delay_time = 150;         //����˯���ӳ�ʱ��
static xdata UI08 wifi_work_state_old = WIFI_SATE_UNKNOW;   //��һ�ι���״̬(Ĭ��δ֪����״̬)
static xdata UI08 wifi_tx_status = 0;

//���� protocol.c  �ṹ������ DOWNLOAD_CMD_S download_cmd[] ����
static code UI08 tx_dpid_cmd_list[] = {
    DPID_SWITCH,               //����(���·����ϱ�)
    DPID_DEHUMIDIFY_SET_VALUE, //�Զ�ʪ��(���·����ϱ�)
    DPID_MODE,                 //��ʪģʽ(���·����ϱ�)
    DPID_HUMIDITY_INDOOR,      //����ʪ��(ֻ�ϱ�)
    DPID_CHILD_LOCK,           //��ͯ��(���·����ϱ�)
    DPID_FAULT,                //���ϸ澯(ֻ�ϱ�)
    //��ע:E1:��ʪ��sensor����.E2:ͭ��sensor����.E4:�����ϡ�E5:MCUͨѶ����.E9:�۳�sensor����
    DPID_TANK_FULL,      //��ˮ״̬(ֻ�ϱ�)
    DPID_DEFROST_STATUS  //��˪״̬(ֻ�ϱ�)
};

/**
 * @description: ����WIFI�Ƶ���˸
 * @note:
 * @param {*}
 * @return {*}
 */
static void tuya_250ms_dsp(void)
{
    static UI08 ms1500_cnt = 0;
    static UI08 ms250_cnt = 0;

    if(!_10mS_For_SYS)
        return;

    if(++ms250_cnt<25)
        return;
    ms250_cnt = 0;

    _Flash_250ms ^= 1;
    _Self_Test_wifi_TXD_en = 1;
    if (++ms1500_cnt >= 6)
    {
        ms1500_cnt = 0;
        _Flash_1500ms ^= 1;
    }
}

/**
 * @description: 1��ִ��һ�Σ�10���ӻ�ȡһ��WIFI״̬
 * @note:
 * @param {*}
 * @return {*}
 */
static xdata UI08 cnt_10S = 0;
static void timer_s_tuya(void)
{

    UI08 wifi_work_state = 0;
    //
    if (!_1S_For_For_SYS)
    {
        return;
    }

    if(Wifi_value_compare_delay)
        Wifi_value_compare_delay--;

    if (Wifi_Rssi_DSP_delay_time)
    {
        Wifi_Rssi_DSP_delay_time--;
    }

    if (Wifi_Comm_Err_delay)
    {
        if (--Wifi_Comm_Err_delay == 0)
        {
            _Wifi_Comm_Err = TRUE;
        }
        else
        {
            _Wifi_Comm_Err = FALSE;
        }
    }
    //
    if (Wifi_Comm_Err_Dsp_time)
    {
        Wifi_Comm_Err_Dsp_time--;
    }

    if (++cnt_10S >= 10) // 10S����Ҫ�����һ��wifi��״̬
    {
        cnt_10S = 0;
        _WIFI_10S_check_status = 1;
    }
    //��ΪͿѻģ������һ��û����������10S,�����ϵ��ֱ�ӽ���˯�ߣ��ͻ�Ҫ���޸Ĳ���ֱ�ӽ���˯��,����150S��ſ���
    //�����150S�ڶ�ȡ��ģ�����˯��,�򼤻�����
    if (Enter_LOW_POWER_delay_time)
    {
        Enter_LOW_POWER_delay_time--;
        wifi_work_state = mcu_get_wifi_work_state(); //��ȡwifi״̬
        if (wifi_work_state == WIFI_LOW_POWER)       //�������͹���
        {
            _wifi_WakeUp_En = 1;                  //�����˯�� SMART_CONFIG ����
            wifi_work_state_old = WIFI_LOW_POWER; //�����һ��״̬Ϊ˯��״̬
        }
        else
        {
            //��˯��ģʽ�ɹ�����Ϊ SMART_CONFIG ����
            if ((wifi_work_state_old == WIFI_LOW_POWER) && (wifi_work_state == SMART_CONFIG_STATE))
            {
                Enter_LOW_POWER_delay_time = 0;
            } //�ɹ�����,�˳����
        }
    }
}

/**
 * @description: �����ϱ�
 * @note:
 * @param {*}
 * @return {*}
 */
static void dpid_data_upload(void)
{
    // xdata UI08 timer_set = 0x00;
    switch (mcu_dp_type)
    {
    case DPID_SWITCH: //����(���·����ϱ�)
    {
        mcu_dp_bool_update(DPID_SWITCH, (unsigned char)SYS_Power_Status); //���ػ�״̬
    }
    break;

    case DPID_DEHUMIDIFY_SET_VALUE: //�Զ�ʪ��(���·����ϱ�)
    {
        mcu_dp_value_update(DPID_DEHUMIDIFY_SET_VALUE, (unsigned char)SYS_Hum_Set); //��ʪ�趨ֵ
    }
    break;

    case DPID_MODE: //��ʪģʽ(���·����ϱ�)
    {
        if (SYS_Mode == mode_SYS_HUM && SYS_HUN_Tyde != CONTINUOUS_HUM) //��λ��ʪ
        {
            mcu_dp_enum_update(DPID_MODE, (unsigned char)0);
        }
        else if (SYS_Mode == mode_SYS_HUM && SYS_HUN_Tyde == CONTINUOUS_HUM) //������ʪ
        {
            mcu_dp_enum_update(DPID_MODE, (unsigned char)1);
        }
        else if (SYS_Mode == mode_DRY_Clothes)
        {
            mcu_dp_enum_update(DPID_MODE, (unsigned char)2);
        }
    }
    break;

    case DPID_HUMIDITY_INDOOR: //����ʪ��(ֻ�ϱ�)
    {

        mcu_dp_value_update(DPID_HUMIDITY_INDOOR, (unsigned char)Hum_dsp_com); //��ǰʪ��
    }
    break;

    case DPID_CHILD_LOCK: //��ͯ��(���·����ϱ�)
    {
        mcu_dp_bool_update(DPID_CHILD_LOCK, (unsigned char)Child_Lock_status); // BOOL�������ϱ�;
    }
    break;
        //        case DPID_COUNTDOWN_SET:
        //            Wifi_Updata.timer_set_value = M_Timer_Buf;
        //            if (M_Timer_Buf == 0) timer_set = 0x00;
        //            else if (M_Timer_Buf == 1) timer_set = 0x01;
        //            else if (M_Timer_Buf == 2) timer_set = 0x02;
        //            else if (M_Timer_Buf == 4) timer_set = 0x03;
        //            else if (M_Timer_Buf == 8) timer_set = 0x04;
        //            else if (M_Timer_Buf == 12) timer_set = 0x05;
        //            else timer_set = 0x00;
        //            mcu_dp_enum_update(DPID_COUNTDOWN_SET,(unsigned char)timer_set); //ö���������ϱ�;
        //            break;
    case DPID_FAULT: //���ϸ澯(ֻ�ϱ�)
    {

        mcu_dp_fault_update(DPID_FAULT, (unsigned char)upload_err_status); //�����������ϱ�;
    }
    break;

        //        case DPID_FILTER_RESET:
        //            Wifi_Updata.filter_status = upload_filter_status;//
        //            mcu_dp_bool_update(DPID_FILTER_RESET,(unsigned char)upload_filter_status/*��ǰ������ϴ*/); //BOOL�������ϱ�;
        //            break;

    case DPID_TANK_FULL: //��ˮ״̬(ֻ�ϱ�)
    {
        mcu_dp_bool_update(DPID_TANK_FULL, (unsigned char)Sys_Err.Water_Full); // BOOL�������ϱ�;
    }
    break;

    case DPID_DEFROST_STATUS: //��˪״̬(ֻ�ϱ�)
    {

        mcu_dp_bool_update(DPID_DEFROST_STATUS, (unsigned char)upload_defrost_status /*��ǰ��˪״̬*/); // BOOL�������ϱ�;
    }
    break;

        //        case DPID_SOUND:
        //            Wifi_Updata.sound_mode = Buzzer_Status;
        //            mcu_dp_bool_update(DPID_SOUND,(unsigned char)Buzzer_Status); //BOOL�������ϱ�;
        //            break;

    default:
        break;
    }
}

/**
 * @description: ϵͳ����dp����Ϣ�ϴ�,ʵ��APP��muc����ͬ��
 * @note:
 * @param {*}
 * @return {*}
 */
static void all_data_update(void)
{
    if (!uart_tx_OK) //����BUF���ݻ�δ����
    {
        return;
    }

    if (Power_Delay_Time > 0) //��֤����EEP ���ϱ�
    {
        return;
    }

    if (!_all_status_updata_en)
    {
        all_data_update_point = 0;
        return;
    }

    if (all_data_update_point > sizeof(tx_dpid_cmd_list)) //�ϱ����
    {
        _all_status_updata_en = 0;
        return;
    }
    else
    {
        all_data_update_point++;
    }
    mcu_dp_type = tx_dpid_cmd_list[all_data_update_point - 1];
    dpid_data_upload();
}

/**
 * @description: ϵͳ����dp����Ϣ�ϴ�,ʵ��APP��muc����ͬ��
 * @note: �˺��������� ��̨�����й����������б仯�ϱ�(�ǳ����ϵ��ϱ�)
 * @param {*}
 * @return {*}
 */
static void data_upload(void) // wifiģ���ϵ� ȫ���ϱ�һ��
{
    if (_all_status_updata_en)
    {
        return;
    }

    if (!_100mS_For_SYS)
    {
        return;
    }

    if (Wifi_value_compare_delay) //APP��������ʱ, ����APPͼ����˸
        return;

    //������ݸ���
    if (Wifi_Updata.power_status != SYS_Power_Status) //����״̬
    {
        Wifi_Updata.power_status = SYS_Power_Status;
        mcu_dp_type = DPID_SWITCH;
    }
    else if (Wifi_Updata.hum_set != SYS_Hum_Set) //ʪ������ֵ
    {
        Wifi_Updata.hum_set = SYS_Hum_Set;
        mcu_dp_type = DPID_DEHUMIDIFY_SET_VALUE;
    }
    else if (Wifi_Updata.hum_mode != SYS_HUN_Tyde) //��ʪģʽ
    {
        Wifi_Updata.hum_mode = SYS_HUN_Tyde;
        mcu_dp_type = DPID_MODE;
    }
    else if (Wifi_Updata.hum_value != Hum_dsp_com) //��ǰʪ��
    {
        Wifi_Updata.hum_value = Hum_dsp_com;
        mcu_dp_type = DPID_HUMIDITY_INDOOR;
    }
    else if (Wifi_Updata.child_lock_status != Child_Lock_status) //ͯ��
    {
        Wifi_Updata.child_lock_status = Child_Lock_status;
        mcu_dp_type = DPID_CHILD_LOCK;
    }
    //    else if (Wifi_Updata.timer_set_value != M_Timer_Buf)                //��ʱ
    //    {
    //        mcu_dp_type = DPID_COUNTDOWN_SET;
    //    }
    else if (Wifi_Updata.err_code != upload_err_status) //��������
    {
        Wifi_Updata.err_code = upload_err_status;
        mcu_dp_type = DPID_FAULT;
    }
    //    else if (Wifi_Updata.filter_status != upload_filter_status)         //��ǰ������ϴ
    //    {
    //        mcu_dp_type = DPID_FILTER_RESET;
    //    }
    else if (Wifi_Updata.water_full != Sys_Err.Water_Full) //��ǰ��ˮ״̬
    {
        Wifi_Updata.water_full = Sys_Err.Water_Full;
        mcu_dp_type = DPID_TANK_FULL;
    }
    else if (Wifi_Updata.defrost_status != upload_defrost_status) //��ǰ��˪״̬
    {
        Wifi_Updata.defrost_status = upload_defrost_status; //
        mcu_dp_type = DPID_DEFROST_STATUS;
    }
    else if (Wifi_Updata.sys_mode != SYS_Mode) //ģʽ����
    {
        Wifi_Updata.sys_mode = SYS_Mode;
        _all_status_updata_en = 1;
        /*
        if(SYS_Mode==mode_DRY)
        {
             mcu_dp_type = DPID_DRY;//����
        }
        else if(SYS_Mode==mode_SYS_HUM)
        {
            mcu_dp_type = DPID_MODE;//��ʪ/�Զ����ʪ
        }
        else if(SYS_Mode==mode_air_clear)
        {
            mcu_dp_type = DPID_UVC;//����
        }*/
    }
    else
    {
        return;
    }

    dpid_data_upload();
}

/**
 * @description: �����ϴ�����
 * @note:
 * @param {*}
 * @return {*}
 */
static void data_update_deal(void)
{
    if (!uart_tx_OK)
    {
        return;
    }
    //��������ģʽ(�����Ǳ߲�����)
    if (_Wifi_factory_test)
    {
        _Wifi_factory_test = 0;
        M_wifi_rssi_dsp = 0; //�ź�ǿ������
        mcu_start_wifitest();
    }
    //������Դ��5S-wifi��λ/����ģʽ�л�,��������״̬��-wifi��λ δ����״̬���л�������ʽ
    else if (_wifi_reset_En)
    {
        _wifi_reset_En = 0;
        mcu_reset_wifi();
    }
    else if (_wifi_WakeUp_En) //˯�߼���
    {
        _wifi_WakeUp_En = 0;
        mcu_set_wifi_mode(SMART_CONFIG); //����ΪSMART_CONFIG������ʽ(Ĭ�ϡ�����)
    }
    // wifiָʾ��״̬������wifi�����·���
    //�������з���û������״̬��  ָʾ��Ҳ����
    //����ÿ��10S  ������wifiģ����ѯһ��״̬
    //������״̬ 1.ÿ��10S������ѯ  2.wifiģ��״̬�仯�����·�  ���ַ�ʽ
    else if (_WIFI_10S_check_status)
    {
        _WIFI_10S_check_status = 0;
        mcu_get_wifi_connect_status();
    }
    //���������ϱ�
    else if (_Wifi_Online_Status) //�ɹ�������,���100ms����Ƿ����������ϱ�
    {
        data_upload();
    }
}

/**
 * @description: �����ϴ������ͽ���
 * @note:
 * @param {*}
 * @return {*}
 */
static void wifi_status_false_to_true(void)
{
    if (_Wifi_Online_Status == FALSE)
    {
        _all_status_updata_en = 1;
        // all_data_update_point = 0;
    }
}

/**
 * @description: wifi״̬����ʾ
 * @note:
 * @param {*}
 * @return {*}
 */
static void wifi_status_DSP(void)
{
    UI08 wifi_work_state_buf = 0;
    // clear buf
    WiFi_LED_Locate_buf = 0;
    /**
     * @brief  MCU������ȡ��ǰwifi����״̬
     * @param  Null
     * @return wifi work state
     * -          SMART_CONFIG_STATE: smartconfig����״̬
     * -          AP_STATE: AP����״̬
     * -          WIFI_NOT_CONNECTED: WIFI���óɹ���δ����·����
     * -          WIFI_CONNECTED: WIFI���óɹ�������·����
     * -          WIFI_CONN_CLOUD: WIFI�Ѿ��������Ʒ�����
     * -          WIFI_LOW_POWER: WIFI���ڵ͹���ģʽ
     * -          SMART_AND_AP_STATE: WIFI smartconfig&AP ģʽ
     * @note   ���Ϊģ���Դ���ģʽ,MCU������øú���
     */
    // unsigned char mcu_get_wifi_work_state(void)
    wifi_work_state_buf = mcu_get_wifi_work_state();
    switch (wifi_work_state_buf)
    {
    case SMART_CONFIG_STATE: // smart config ����״̬ LED ���� ��led ��˸���û����   �����˸ 250ms
    {
        if (_Flash_250ms)
        {
            WiFi_LED_Locate_buf = 1;
        }
        _Wifi_Online_Status = FALSE;
    }
    break;
    //
    case AP_STATE: // AP ����״̬ LED ����  �����˸ 1500ms
    {
        if (_Flash_1500ms)
        {
            WiFi_LED_Locate_buf = 1;
        }
        _Wifi_Online_Status = FALSE;
    }
    break;
    //
    case WIFI_NOT_CONNECTED: // WIFI ������ɣ���������·������LED ����
    {
        wifi_status_false_to_true();
        _Wifi_Online_Status = TRUE; // 2021-07-23
        ;                           // LED����
    }
    break;
    //
    case WIFI_CONNECTED: //·�������ӳɹ� LED ����
    {
        WiFi_LED_Locate_buf = 1;
        wifi_status_false_to_true();
        _Wifi_Online_Status = TRUE;
    }
    break;

    case WIFI_CONN_CLOUD: //�ɹ���������
    {
        WiFi_LED_Locate_buf = 1;
        wifi_status_false_to_true();
        _Wifi_Online_Status = TRUE;
    }
    break;
    //
    default:
    {
        _Wifi_Online_Status = FALSE;
    }
    break;
    }
}

/**
 * @description:�ϴ�������Ԥ����
 * @note:
 * @param {*}
 * @return {*}
 */
static void upload_data_deal(void)
{
    UI08 err_buf = 0;

    Updata_speed = Fan_Speed_Out;

    if (Sys_Err.temp_room_err) // E1,��ʪ�ȴ���������
    {
        err_buf |= TEMP_HUM_SEN_ERR_UPLOAD;
    }

    if (Sys_Err.temp_coil_err) // E2,ͭ�ܴ���������
    {
        err_buf |= COIL_SEN_ERR_UPLOAD;
    }

    if (Sys_Err.pan_motor_err) // E4,������
    {
        err_buf |= MOTOR_ERR_UPLOAD;
    }

    if (Sys_Err.comm_err) // E5,MCUͨ�Ź���
    {
        err_buf |= MCU_COMMUN_ERR_UPLOAD;
    }

    if (Sys_Err.pm25_Sensor_err) // E9,�۳�����������
    {
        err_buf |= DUST_SEN_ERR_UPLOAD;
    }

    if ((err_buf == 0) && (upload_err_status != 0))
    {
        _all_status_updata_en = 1;
        // all_data_update_point = 0;
    }

    upload_err_status = err_buf;
}

/**
 * @description: ��ʼ��
 * @note:
 * @param {*}
 * @return {*}
 */
void tuya_init(void)
{
    WIFI_UART_Driver_Init();
    wifi_protocol_init();
}

/**
 * @description: tuyaģ��Ĵ�����
 * @note: �����������е���
 * @param {*}
 * @return {*}
 */
void tuya_deal(void)
{
    upload_data_deal();
    tuya_250ms_dsp();
    timer_s_tuya();
    wifi_status_DSP();
    wifi_uart_service();
    all_data_update();
    data_update_deal();
}
/*************************************************
//����        :	wifi_rssi_dsp
//����        : wifi ����ʱ  ��ʾwifi�ź�ǿ��
//��ڲ���    :	��
//���ڲ���    :	��
//��	��: 	GCE ����ǿ		2011/07/18
//��	��: 	GCE XXX
************************************************/
void wifi_rssi_Dsp(void)
{
    UI08 wifi_buf;
    wifi_buf = M_wifi_rssi_dsp;
    //
    if (wifi_buf > 99)
    {
        wifi_buf = 99;
    }
    //���Sģʽ�r,��̖���60,���c��Wifiָʾ��
    // if (wifi_buf >= 60)
    // {
    //     LED_WIFI;
    // }
    dig1_2_dsp(wifi_buf);
}
