#include "General.h"

xdata UUI16 flag_wifi = {0};          //标志位
xdata UI08 M_wifi_rssi_dsp = 0;       // wifi 信号强度
xdata UI08 all_data_update_point = 0; //全部数据上报时的计数值
xdata UI08 upload_filter_status = 0;
xdata UI08 upload_defrost_status = 0;
xdata UI08 upload_hepa_filter = 0;

xdata UI08 Wifi_value_compare_delay = 0;                    //app操作后延时对比数据
xdata WIFI_UPDATE Wifi_Updata;                              // WIFI数据更新的数据
xdata FANSPEED_TYPE Updata_speed = OFF_FAN;                 //上传的风速
static xdata UI08 upload_err_status = 0;                    //上传的错误状态
static xdata UI08 mcu_dp_type = 0;                          //上报数据的DP
xdata UI08 Wifi_Rssi_DSP_delay_time = 0;                    //
static xdata UI08 Wifi_Comm_Err_delay = WIFI_COMM_ERR_TIME; // wifi通信故障
static xdata UI08 Wifi_Comm_Err_Dsp_time = 0;               // wifi通信故障显示时间
static xdata UI08 Enter_LOW_POWER_delay_time = 150;         //进入睡眠延迟时间
static xdata UI08 wifi_work_state_old = WIFI_SATE_UNKNOW;   //上一次工作状态(默认未知网络状态)
static xdata UI08 wifi_tx_status = 0;

//根据 protocol.c  结构体数组 DOWNLOAD_CMD_S download_cmd[] 制作
static code UI08 tx_dpid_cmd_list[] = {
    DPID_SWITCH,               //开关(可下发可上报)
    DPID_DEHUMIDIFY_SET_VALUE, //自订湿度(可下发可上报)
    DPID_MODE,                 //除湿模式(可下发可上报)
    DPID_HUMIDITY_INDOOR,      //室内湿度(只上报)
    DPID_CHILD_LOCK,           //儿童锁(可下发可上报)
    DPID_FAULT,                //故障告警(只上报)
    //备注:E1:温湿度sensor故障.E2:铜管sensor故障.E4:马达故障。E5:MCU通讯故障.E9:粉尘sensor故障
    DPID_TANK_FULL,      //满水状态(只上报)
    DPID_DEFROST_STATUS  //除霜状态(只上报)
};

/**
 * @description: 控制WIFI灯的闪烁
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
 * @description: 1秒执行一次，10秒钟获取一次WIFI状态
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

    if (++cnt_10S >= 10) // 10S主动要求更新一次wifi的状态
    {
        cnt_10S = 0;
        _WIFI_10S_check_status = 1;
    }
    //因为涂鸦模块在上一次没有配网超过10S,重新上电会直接进入睡眠，客户要求修改不让直接进入睡眠,至少150S后才可以
    //如果在150S内读取到模块进入睡眠,则激活配网
    if (Enter_LOW_POWER_delay_time)
    {
        Enter_LOW_POWER_delay_time--;
        wifi_work_state = mcu_get_wifi_work_state(); //获取wifi状态
        if (wifi_work_state == WIFI_LOW_POWER)       //如果进入低功耗
        {
            _wifi_WakeUp_En = 1;                  //激活唤醒睡眠 SMART_CONFIG 配网
            wifi_work_state_old = WIFI_LOW_POWER; //标记上一次状态为睡眠状态
        }
        else
        {
            //由睡眠模式成功激活为 SMART_CONFIG 配网
            if ((wifi_work_state_old == WIFI_LOW_POWER) && (wifi_work_state == SMART_CONFIG_STATE))
            {
                Enter_LOW_POWER_delay_time = 0;
            } //成功激活,退出检查
        }
    }
}

/**
 * @description: 数据上报
 * @note:
 * @param {*}
 * @return {*}
 */
static void dpid_data_upload(void)
{
    // xdata UI08 timer_set = 0x00;
    switch (mcu_dp_type)
    {
    case DPID_SWITCH: //开关(可下发可上报)
    {
        mcu_dp_bool_update(DPID_SWITCH, (unsigned char)SYS_Power_Status); //开关机状态
    }
    break;

    case DPID_DEHUMIDIFY_SET_VALUE: //自订湿度(可下发可上报)
    {
        mcu_dp_value_update(DPID_DEHUMIDIFY_SET_VALUE, (unsigned char)SYS_Hum_Set); //除湿设定值
    }
    break;

    case DPID_MODE: //除湿模式(可下发可上报)
    {
        if (SYS_Mode == mode_SYS_HUM && SYS_HUN_Tyde != CONTINUOUS_HUM) //挡位除湿
        {
            mcu_dp_enum_update(DPID_MODE, (unsigned char)0);
        }
        else if (SYS_Mode == mode_SYS_HUM && SYS_HUN_Tyde == CONTINUOUS_HUM) //连续除湿
        {
            mcu_dp_enum_update(DPID_MODE, (unsigned char)1);
        }
        else if (SYS_Mode == mode_DRY_Clothes)
        {
            mcu_dp_enum_update(DPID_MODE, (unsigned char)2);
        }
    }
    break;

    case DPID_HUMIDITY_INDOOR: //室内湿度(只上报)
    {

        mcu_dp_value_update(DPID_HUMIDITY_INDOOR, (unsigned char)Hum_dsp_com); //当前湿度
    }
    break;

    case DPID_CHILD_LOCK: //儿童锁(可下发可上报)
    {
        mcu_dp_bool_update(DPID_CHILD_LOCK, (unsigned char)Child_Lock_status); // BOOL型数据上报;
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
        //            mcu_dp_enum_update(DPID_COUNTDOWN_SET,(unsigned char)timer_set); //枚举型数据上报;
        //            break;
    case DPID_FAULT: //故障告警(只上报)
    {

        mcu_dp_fault_update(DPID_FAULT, (unsigned char)upload_err_status); //故障型数据上报;
    }
    break;

        //        case DPID_FILTER_RESET:
        //            Wifi_Updata.filter_status = upload_filter_status;//
        //            mcu_dp_bool_update(DPID_FILTER_RESET,(unsigned char)upload_filter_status/*当前滤网清洗*/); //BOOL型数据上报;
        //            break;

    case DPID_TANK_FULL: //满水状态(只上报)
    {
        mcu_dp_bool_update(DPID_TANK_FULL, (unsigned char)Sys_Err.Water_Full); // BOOL型数据上报;
    }
    break;

    case DPID_DEFROST_STATUS: //除霜状态(只上报)
    {

        mcu_dp_bool_update(DPID_DEFROST_STATUS, (unsigned char)upload_defrost_status /*当前除霜状态*/); // BOOL型数据上报;
    }
    break;

        //        case DPID_SOUND:
        //            Wifi_Updata.sound_mode = Buzzer_Status;
        //            mcu_dp_bool_update(DPID_SOUND,(unsigned char)Buzzer_Status); //BOOL型数据上报;
        //            break;

    default:
        break;
    }
}

/**
 * @description: 系统所有dp点信息上传,实现APP和muc数据同步
 * @note:
 * @param {*}
 * @return {*}
 */
static void all_data_update(void)
{
    if (!uart_tx_OK) //发送BUF数据还未发完
    {
        return;
    }

    if (Power_Delay_Time > 0) //保证读完EEP 再上报
    {
        return;
    }

    if (!_all_status_updata_en)
    {
        all_data_update_point = 0;
        return;
    }

    if (all_data_update_point > sizeof(tx_dpid_cmd_list)) //上报完成
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
 * @description: 系统所有dp点信息上传,实现APP和muc数据同步
 * @note: 此函数运用在 机台在运行过程中数据有变化上报(非初次上电上报)
 * @param {*}
 * @return {*}
 */
static void data_upload(void) // wifi模块上电 全部上报一次
{
    if (_all_status_updata_en)
    {
        return;
    }

    if (!_100mS_For_SYS)
    {
        return;
    }

    if (Wifi_value_compare_delay) //APP操作后延时, 避免APP图标闪烁
        return;

    //检查数据更新
    if (Wifi_Updata.power_status != SYS_Power_Status) //开关状态
    {
        Wifi_Updata.power_status = SYS_Power_Status;
        mcu_dp_type = DPID_SWITCH;
    }
    else if (Wifi_Updata.hum_set != SYS_Hum_Set) //湿度设置值
    {
        Wifi_Updata.hum_set = SYS_Hum_Set;
        mcu_dp_type = DPID_DEHUMIDIFY_SET_VALUE;
    }
    else if (Wifi_Updata.hum_mode != SYS_HUN_Tyde) //除湿模式
    {
        Wifi_Updata.hum_mode = SYS_HUN_Tyde;
        mcu_dp_type = DPID_MODE;
    }
    else if (Wifi_Updata.hum_value != Hum_dsp_com) //当前湿度
    {
        Wifi_Updata.hum_value = Hum_dsp_com;
        mcu_dp_type = DPID_HUMIDITY_INDOOR;
    }
    else if (Wifi_Updata.child_lock_status != Child_Lock_status) //童锁
    {
        Wifi_Updata.child_lock_status = Child_Lock_status;
        mcu_dp_type = DPID_CHILD_LOCK;
    }
    //    else if (Wifi_Updata.timer_set_value != M_Timer_Buf)                //定时
    //    {
    //        mcu_dp_type = DPID_COUNTDOWN_SET;
    //    }
    else if (Wifi_Updata.err_code != upload_err_status) //报警错误
    {
        Wifi_Updata.err_code = upload_err_status;
        mcu_dp_type = DPID_FAULT;
    }
    //    else if (Wifi_Updata.filter_status != upload_filter_status)         //当前滤网清洗
    //    {
    //        mcu_dp_type = DPID_FILTER_RESET;
    //    }
    else if (Wifi_Updata.water_full != Sys_Err.Water_Full) //当前满水状态
    {
        Wifi_Updata.water_full = Sys_Err.Water_Full;
        mcu_dp_type = DPID_TANK_FULL;
    }
    else if (Wifi_Updata.defrost_status != upload_defrost_status) //当前除霜状态
    {
        Wifi_Updata.defrost_status = upload_defrost_status; //
        mcu_dp_type = DPID_DEFROST_STATUS;
    }
    else if (Wifi_Updata.sys_mode != SYS_Mode) //模式更新
    {
        Wifi_Updata.sys_mode = SYS_Mode;
        _all_status_updata_en = 1;
        /*
        if(SYS_Mode==mode_DRY)
        {
             mcu_dp_type = DPID_DRY;//干衣
        }
        else if(SYS_Mode==mode_SYS_HUM)
        {
            mcu_dp_type = DPID_MODE;//除湿/自定义除湿
        }
        else if(SYS_Mode==mode_air_clear)
        {
            mcu_dp_type = DPID_UVC;//空清
        }*/
    }
    else
    {
        return;
    }

    dpid_data_upload();
}

/**
 * @description: 数据上传处理
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
    //工厂测试模式(国富那边测试用)
    if (_Wifi_factory_test)
    {
        _Wifi_factory_test = 0;
        M_wifi_rssi_dsp = 0; //信号强度清零
        mcu_start_wifitest();
    }
    //长按电源键5S-wifi复位/配网模式切换,在联网的状态下-wifi复位 未联网状态下切换配网方式
    else if (_wifi_reset_En)
    {
        _wifi_reset_En = 0;
        mcu_reset_wifi();
    }
    else if (_wifi_WakeUp_En) //睡眠激活
    {
        _wifi_WakeUp_En = 0;
        mcu_set_wifi_mode(SMART_CONFIG); //激活为SMART_CONFIG配网方式(默认、快闪)
    }
    // wifi指示灯状态都是由wifi主动下发的
    //但测试中发现没联网的状态下  指示灯也常亮
    //所以每隔10S  主动向wifi模块问询一次状态
    //致网络状态 1.每隔10S主动问询  2.wifi模块状态变化主动下发  两种方式
    else if (_WIFI_10S_check_status)
    {
        _WIFI_10S_check_status = 0;
        mcu_get_wifi_connect_status();
    }
    //数据主动上报
    else if (_Wifi_Online_Status) //成功联上网,间隔100ms检查是否有数据需上报
    {
        data_upload();
    }
}

/**
 * @description: 数据上传开启和禁用
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
 * @description: wifi状态灯显示
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
     * @brief  MCU主动获取当前wifi工作状态
     * @param  Null
     * @return wifi work state
     * -          SMART_CONFIG_STATE: smartconfig配置状态
     * -          AP_STATE: AP配置状态
     * -          WIFI_NOT_CONNECTED: WIFI配置成功但未连上路由器
     * -          WIFI_CONNECTED: WIFI配置成功且连上路由器
     * -          WIFI_CONN_CLOUD: WIFI已经连接上云服务器
     * -          WIFI_LOW_POWER: WIFI处于低功耗模式
     * -          SMART_AND_AP_STATE: WIFI smartconfig&AP 模式
     * @note   如果为模块自处理模式,MCU无须调用该函数
     */
    // unsigned char mcu_get_wifi_work_state(void)
    wifi_work_state_buf = mcu_get_wifi_work_state();
    switch (wifi_work_state_buf)
    {
    case SMART_CONFIG_STATE: // smart config 配置状态 LED 快闪 ，led 闪烁请用户完成   间隔闪烁 250ms
    {
        if (_Flash_250ms)
        {
            WiFi_LED_Locate_buf = 1;
        }
        _Wifi_Online_Status = FALSE;
    }
    break;
    //
    case AP_STATE: // AP 配置状态 LED 慢闪  间隔闪烁 1500ms
    {
        if (_Flash_1500ms)
        {
            WiFi_LED_Locate_buf = 1;
        }
        _Wifi_Online_Status = FALSE;
    }
    break;
    //
    case WIFI_NOT_CONNECTED: // WIFI 配置完成，正在连接路由器，LED 常暗
    {
        wifi_status_false_to_true();
        _Wifi_Online_Status = TRUE; // 2021-07-23
        ;                           // LED不亮
    }
    break;
    //
    case WIFI_CONNECTED: //路由器连接成功 LED 常亮
    {
        WiFi_LED_Locate_buf = 1;
        wifi_status_false_to_true();
        _Wifi_Online_Status = TRUE;
    }
    break;

    case WIFI_CONN_CLOUD: //成功连接上云
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
 * @description:上传的数据预处理
 * @note:
 * @param {*}
 * @return {*}
 */
static void upload_data_deal(void)
{
    UI08 err_buf = 0;

    Updata_speed = Fan_Speed_Out;

    if (Sys_Err.temp_room_err) // E1,温湿度传感器故障
    {
        err_buf |= TEMP_HUM_SEN_ERR_UPLOAD;
    }

    if (Sys_Err.temp_coil_err) // E2,铜管传感器故障
    {
        err_buf |= COIL_SEN_ERR_UPLOAD;
    }

    if (Sys_Err.pan_motor_err) // E4,马达故障
    {
        err_buf |= MOTOR_ERR_UPLOAD;
    }

    if (Sys_Err.comm_err) // E5,MCU通信故障
    {
        err_buf |= MCU_COMMUN_ERR_UPLOAD;
    }

    if (Sys_Err.pm25_Sensor_err) // E9,粉尘传感器故障
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
 * @description: 初始化
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
 * @description: tuya模块的处理函数
 * @note: 放在主函数中调用
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
//名称        :	wifi_rssi_dsp
//功能        : wifi 产测时  显示wifi信号强度
//入口参数    :	无
//出口参数    :	无
//构	建: 	GCE 唐永强		2011/07/18
//修	改: 	GCE XXX
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
    //工廠模式時,信號低於60,不點亮Wifi指示燈
    // if (wifi_buf >= 60)
    // {
    //     LED_WIFI;
    // }
    dig1_2_dsp(wifi_buf);
}
