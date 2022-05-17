#include "General.h"

MCU_const UI08 manufacturer[] = "manufacturer nwt";                             //������
MCU_const UI08 serial_number[] = "serial_number 105821277";                     //���к�
MCU_const UI08 MIIO_model_req[] = "MIIO_model_req";                             //ģ����Ϣ
MCU_const UI08 model_nwt[] = "model nwt.derh.330ef";                            //ģ����Ϣ
MCU_const UI08 MIIO_mcu_version_req[] = "MIIO_mcu_version_req";                 //����������汾
MCU_const UI08 mcu_version[] = "mcu_version 0016";                              //����������汾
MCU_const UI08 ble_config[] = "ble_config set 1720 0001";                       // ble��������
MCU_const UI08 net[] = "net";                                                   //ѯ����������״̬
MCU_const UI08 reboot[] = "reboot";                                             //����WIFI
MCU_const UI08 Restore_WIFI[] = "restore";                                      //���WIFI����
MCU_const UI08 factory[] = "factory";                                           //���ù���ģʽ
MCU_const UI08 get_down[] = "get_down";                                         //��ѯ��������û���·�����
MCU_const UI08 result_ok[] = "result \"ok\"";                                   //���ظ�������������������
MCU_const UI08 result[] = "result";                                             //���ظ�������������������
MCU_const UI08 update_fw[] = "update_fw";                                       //���������
MCU_const UI08 error_invalid_operation[] = "error \"invalid operation\" -5000"; //�޷�ʶ���ָ��

MCU_const UI08 set_properties[] = "set_properties";         //�������� ���û�̨���� ��������
MCU_const UI08 get_properties[] = "get_properties";         //��ȡ���� ��ȡ��̨���� ��������
MCU_const UI08 properties_changed[] = "properties_changed"; //�����ϱ�  ��̨�����ϱ� ��������
MCU_const UI08 down[] = "down";                             //�·�down
MCU_const UI08 none[] = "none";                             //������
MCU_const UI08 MIIO_net_change[] = "MIIO_net_change";       //ʶ������״̬�ı�
////
MCU_const UI08 NO_properties[] = "-4003"; //���ԡ��������¼�������

MCU_const UI08 event_code[] = "event_occured"; //�¼�
MCU_const UI08 com_ok[] = "ok";                // OK
MCU_const UI08 uap[] = "uap";                  //�ȴ�������
MCU_const UI08 cloud[] = "cloud";              //�ɹ����ӵ�·����
MCU_const UI08 local[] = "local";              //�ɹ����ӵ�·����
MCU_const UI08 offline[] = "offline";          //����������
MCU_const UI08 updating[] = "updating";        //������
MCU_const UI08 unprov[] = "unprov";            //�������ܹر�
MCU_const UI08 Code_true[] = "true";           //"true";
MCU_const UI08 Code_false[] = "false";         //"false";

MCU_const UI08 Ascii_0[] = "0"; // Ascii 0
MCU_const UI08 Ascii_1[] = "1"; // Ascii 1
MCU_const UI08 Ascii_2[] = "2"; // Ascii 2
MCU_const UI08 Ascii_3[] = "3"; // Ascii 3
MCU_const UI08 Ascii_4[] = "4"; // Ascii 4
MCU_const UI08 Ascii_5[] = "5"; // Ascii 5
MCU_const UI08 Ascii_6[] = "6"; // Ascii 6
MCU_const UI08 Ascii_7[] = "7"; // Ascii 7

MCU_const UI08 Self_Test[] = "Self_Test";   //�Լ���Ϣ
MCU_xdata UI08 Self_Test_wifi_err_time = 0; //�Լ�err��ʱ
// SIID TAB
MCU_const UI32 get_properties_SIID_PIID_EN[9] = {
    get_properties_SIID1_PIID_EN,
    get_properties_SIID2_PIID_EN,
    get_properties_SIID3_PIID_EN,
    get_properties_SIID4_PIID_EN,
    get_properties_SIID5_PIID_EN,
    get_properties_SIID6_PIID_EN,
    get_properties_SIID7_PIID_EN,
    get_properties_SIID8_PIID_EN,
    get_properties_SIID9_PIID_EN,
};

MCU_xdata UUI32 Txd_props_bit[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // SIID ����
//
MCU_xdata Wifi_UART_str SYS_W_Uart;   // wifi����
MCU_xdata Wifi_props_str SYS_W_props; // wifi����

MCU_xdata UI08 Star_Process_Num = 1;                //��ʼ��WIFIģ������������
MCU_xdata FuncState Sec_Txd_Star_Process = ENABLE;  //����ʹ��
MCU_xdata UI08 Wifi_Tick = 0;                       //ʱ��-����ʹ��
MCU_xdata net_Status_e Wifi_net_Status = e_uap;     //����״̬
MCU_xdata net_Status_e Wifi_net_Status_buf = e_uap; //����״̬
MCU_xdata UI08 Wifi_net_delay = 0;                  // net ��ʱ
MCU_xdata FuncState Wifi_txd_en = DISABLE;          //����ʹ��
MCU_xdata FuncState Wifi_model_load = ENABLE;       // wifiģ��װ��-����ϵ�ǰû��wifiģ��ʱ wifiָʾ�Ʋ���

MCU_xdata UUI32 Txd_props_ok_bit[9]; //��ǰ���͵ķ�����ͨ��ָ��

MCU_xdata UUI32 Txd_event_bit = {0};     //�ȴ����͵��¼�
MCU_xdata UUI32 Txd_event_bit_buf = {0}; //�ȴ����͵��¼� buf
MCU_xdata UUI32 Txd_event_ok_bit = {0};  //��ǰ���͵��¼�

MCU_xdata UUI32 Txd_COM_bit = {0};    //����ָ��
MCU_xdata UUI32 Txd_COM_ok_bit = {0}; //��ǰ���͵Ĺ���ָ��
MCU_xdata UUI32 Set_props_err = {0};  //�ȴ����͵Ĵ���
MCU_xdata UI16 addr_point = 0;        //���͵�ַ

//�����ȼ���ָ��
MCU_xdata UUI16 wifi_com_bit = {0};               //��־λ
MCU_xdata UUI16 wifi_com_ok_bit = {0};            //��־λ
MCU_xdata UUI16 T_result_bit = {0};               //��־λ
MCU_xdata UUI16 R_result_bit = {0};               //��־λ
MCU_xdata UUI16 Get_Props_bit = {0};              // wifi������
MCU_xdata FANSPEED_TYPE Wifi_Fan_Speed = OFF_FAN; //���ٱ仯ʱ  �ϱ�app(�������ٱ仯��)
MCU_xdata FANSPEED_TYPE Wifi_Set_Fan = OFF_FAN;   // wifi�������  ����ʹ��
MCU_xdata UI08 get_down_cont = 0;                 // get_down ���͵Ĵ���

void Wifi_COM_Deal(void);
void Txd_data(UI08 *point, UI16 lenth);
void Txd_data_props(UI08 *point, UI16 addr, UI16 lenth);
void get_properties_deal(void);
void Txd_props_Deal(void);
void Rest_Wifi(void);
/*************************************************
 // ��������    : Wifi_UART_Init
 // ��������    : ����/���ݼ����ݵĳ�ʼ��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Wifi_UART_Init(void)
{
   UI16 i;

   Uart_Driver_Iint();
   //
   for (i = 0; i < TXD_MAX; i++)
   {
      SYS_W_Uart.Txd_buf[i] = 0;
   }
   SYS_W_Uart.Txd_byte = 0;  //
   SYS_W_Uart.Txd_Lenth = 0; //
   SYS_W_Uart.Txd_OK = ENABLE;
   //
   for (i = 0; i < RXD_MAX; i++)
   {
      SYS_W_Uart.Rxd_buf[i] = 0;
   }
   SYS_W_Uart.Rxd_byte = 0;
   SYS_W_Uart.Rxd_Timerout = 0;
   SYS_W_Uart.Rxd_OK = DISABLE;
   //
   SYS_W_props.props_humidity_rec = 0;
   SYS_W_props.props_temp_rec = 0;
   SYS_W_props.props_temp_coil_rec = 0;
   //
   for (i = 0; i < 9; i++)
   {
      Txd_props_bit[i].lword = 0;
      Txd_props_ok_bit[i].lword = 0; //��ǰ���͵ķ�����ͨ��ָ��
   }
}
/*************************************************
 // ��������    : prg_Wifi_UART_ms
 // ��������    : msʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_Wifi_UART_ms(void)
{
   /*if(!_Wifi_Uart_ms)
   {return;}
   _Wifi_Uart_ms=0;*/
   //
   if (SYS_W_Uart.Rxd_Timerout > 0) //����������ɺ�N�뿪ʼ��������
   {
      SYS_W_Uart.Rxd_Timerout--;
      if (SYS_W_Uart.Rxd_Timerout == 0)
      {
         SYS_W_Uart.Rxd_OK = ENABLE;
      }
   }
}
/*************************************************
 // ��������    : prg_Wifi_UART_ms10
 // ��������    : 10msʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_Wifi_UART_ms10(void)
{
   if (!_Wifi_Uart_ms10)
   {
      return;
   }
   _Wifi_Uart_ms10 = 0;

   Wifi_Tick++;
   if (Wifi_Tick == 62)
   {
      _txd_tick = 1; //���Դ���ͨ��
   }
   else if (Wifi_Tick >= 84)
   {
      Wifi_Tick = 0;
   }

   if ((Wifi_Tick % 15 == 0) && (Wifi_Tick <= 60))
   {
      Wifi_txd_en = ENABLE;
   }

   if (Self_Test_wifi_err_time)
   {
      if (--Self_Test_wifi_err_time == 0)
      {
         _Self_Test_wifi_err = 1;
      }
      else
      {
         _Self_Test_wifi_err = 0;
      }
   }
}
/*************************************************
 // ��������    : prg_S_Wifi_UART
 // ��������    : sʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_S_Wifi_UART(void)
{
   if (!_Wifi_Uart_s)
   {
      return;
   }
   _Wifi_Uart_s = 0;

   if (Wifi_net_delay > 0)
   {
      Wifi_net_delay--;
      if (Wifi_net_delay == 0)
      {
         Wifi_net_Status = Wifi_net_Status_buf;
      }
   }
}
/*************************************************
 // ��������    : Wifi_UART_Txd_Data
 // ��������    : �������ݵķ���
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Wifi_UART_Txd_Data(void)
{
   if (SYS_W_Uart.Txd_byte < SYS_W_Uart.Txd_Lenth)
   {
      UART_SFR = SYS_W_Uart.Txd_buf[SYS_W_Uart.Txd_byte];
      SYS_W_Uart.Txd_byte++;
      SYS_W_Uart.Txd_OK = DISABLE;
   }
   else
   {
      SYS_W_Uart.Txd_OK = ENABLE;
   }
}
/*************************************************
 // ��������    : Clear_Data
 // ��������    : ��������
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Clear_Data(UI08 *point, UI08 lenth)
{
   UI08 i = 0;
   for (i = 0; i < lenth; i++)
   {
      *point = 0;
      point++;
   }
}
/*****************************************************************************
�������� : my_strcmp
�������� : �Ƚ��ַ�����С
������� : s1:�ַ���1
           s2:�ַ���2
���ز��� : ��С�Ƚ�ֵ��0:s1=s2; -1:s1<s2; 1:s1>s2
*****************************************************************************/
UI08 my_strcmp(const UI08 *s1, UI08 *s2)
{
   UI08 buf = 0;
   while (*s1 && *s2 && (*s1 == *s2))
   {
      s1++;
      s2++;
      buf = 1;
   }
   if ((*s1 == 0) && (buf == 1))
   {
      return 1;
   }
   else
   {
      return 0;
   }
}
/*************************************************
 // ��������    : net_deal
 // ��������    : ���紦��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void net_deal(UI08 addr)
{
   if (my_strcmp(uap, &SYS_W_Uart.Rxd_buf[addr]))
   {
      if (Wifi_net_Status == e_cloud)
      {
         if (Wifi_net_Status_buf == e_cloud)
         {
            Wifi_net_delay = Wifi_net_NUM_L;
         }
      }
      else
      {
         // Wifi_net_delay=Wifi_net_NUM_S;
         Wifi_net_Status = e_uap;
      }
      Wifi_net_Status_buf = e_uap;
      // Wifi_net_Status=e_uap;

      _Txd_COM_net = 1;
      Sec_Txd_Star_Process = ENABLE;
   }
   else if (my_strcmp(cloud, &SYS_W_Uart.Rxd_buf[addr]))
   {
      Wifi_net_Status = e_cloud;
      Wifi_net_Status_buf = e_cloud;
      _Txd_COM_net = 0;
   }
   else if (my_strcmp(local, &SYS_W_Uart.Rxd_buf[addr]))
   {
      Wifi_net_Status = e_cloud;
      Wifi_net_Status_buf = e_cloud;
      _Txd_COM_net = 0;
   }
   else if (my_strcmp(offline, &SYS_W_Uart.Rxd_buf[addr]))
   {
      if (Wifi_net_Status == e_cloud)
      {
         if (Wifi_net_Status_buf == e_cloud)
         {
            Wifi_net_delay = Wifi_net_NUM_L;
         }
      }
      else
      {
         Wifi_net_Status = e_offline;
      }

      Wifi_net_Status_buf = e_offline;
      _Txd_COM_net = 1;
      Sec_Txd_Star_Process = ENABLE;
   }
   else if (my_strcmp(updating, &SYS_W_Uart.Rxd_buf[addr]))
   {
      Wifi_net_Status = e_updating;
      Wifi_net_Status_buf = e_updating;
      _Txd_COM_net = 0;
      Sec_Txd_Star_Process = ENABLE;
   }
   else if (my_strcmp(unprov, &SYS_W_Uart.Rxd_buf[addr]))
   {
      Wifi_net_Status = e_unprov;
      Wifi_net_Status_buf = e_unprov;
      _Txd_COM_net = 1;
      Sec_Txd_Star_Process = ENABLE;
   }
}
/*************************************************
 // ��������    : chack_down_deal
 // ��������    : �� type_buf ״̬��  app���ɲ�����̨
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
UI08 chack_down_deal(UI08 type_buf)
{
   UI08 key_type = 0;

   // if(Child_Lock_status==ENABLE)//ͯ��
   //{key_type|=key_Child_Lock_bit;}

   if ((SYS_Power_Status == OFF) && (M_Timer_Run == 0)) //�ػ��Ƕ�ʱ
   {
      key_type |= key_power_bit;
   }

   // if(sys_mode_buf==mode_DRY_Clothes)
   //{key_type|=key_dry_cloth_bit; }

   if (Sys_Err.Water_Full == ENABLE) //ˮ�����߻�̨�㵹
   {
      key_type |= key_Water_Full_bit;
   }

   // if(Fan_set_enable==DISABLE)
   //{key_type|=key_Fan_set_bit;}

   type_buf = type_buf & key_type;

   if (type_buf & (key_Child_Lock_bit))
   {
      if (Child_Lock_Disp_Count == 0)
      {
         Key_ERR_Buzz_Cnt = 3;
         Child_Lock_Disp_Count = Child_Lock_Disp_NUM;
         Child_Lock_Disp_timer = 50;
         Child_Lock_1s_Count = 0;
      }
      //�ı� -4003
      //_wifi_com_props_error=1;
      return 1;
   }
   //
   if (type_buf)
   {
      Key_ERR_Buzz_Cnt = 3;
      //�ı� -4003
      //_wifi_com_props_error=1;
      return 1;
   }
   return 0;
}
/*************************************************
 // ��������    : set_properties_deal
 // ��������    : wifi�·�ָ���
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
/*
 SET_POWER_Err
SET_MODE_Err
 SET_HUM_Err
 SET_FAN_Err
  SET_LAMP_Err
  SET_BUZZER_Err
 SET_CHILD_LOCK_Err
  SET_PUMP_Err
 SET_TIME_Err
*/
void set_properties_deal(UI08 buf)
{
   UI08 type_buf = 0;
   UI08 Set_buf = 0;
   UI08 SIID_data = 0;
   //�� down set_properties 2 5 37
   //
   SIID_data = SYS_W_Uart.Rxd_buf[buf]; //��ȡSIID��
   //
   type_buf = SYS_W_Uart.Rxd_buf[buf + 2]; //����5��λ��
   buf = buf + 4;                          //���� 37��λ��
   //
   if (SIID_data == '1') // SIIDΪ1
   {
      ;
   }
   else if (SIID_data == '2') // SIIDΪ2
   {
      switch (type_buf)
      {
      case SET_POWER: //���ػ�
      {
         _wifi_T_result_SET_POWER = 1;
         //
         if (my_strcmp(Code_false, &SYS_W_Uart.Rxd_buf[buf]))
         {
            if (chack_down_deal(key_Water_Full_bit)) //
            {
               SET_POWER_Err = 1; //���ԡ��������¼�������-�ػ������趨
               return;
            }
            Turn_Off();
         }
         else if (my_strcmp(Code_true, &SYS_W_Uart.Rxd_buf[buf]))
         {
            if (chack_down_deal(key_Water_Full_bit)) //
            {
               SET_POWER_Err = 1; //���ԡ��������¼�������-�ػ������趨
               return;
            }
            Turn_On();
         }
         else
         {
            SET_POWER_Err = 1;
            return;
         } //����ָ�� �ظ�-4003
         Buzz_Time = BUZZ_long_time;
      }
      break;
      case SET_MODE: //ģʽ
      {
         _wifi_T_result_SET_MODE = 1;                  //����OK
                                                       //
         if (my_strcmp("1", &SYS_W_Uart.Rxd_buf[buf])) // HUM mode
         {
            if (chack_down_deal(key_power_bit | key_Water_Full_bit))
            {
               SET_MODE_Err = 1;
               return;
            }
            //
            Disp_set_Delay = 10;
            //		M_continuous=0;
            //		M_Dry_Status=OFF;
            M_Timer_Setting_Time = 0;
            _Timer_set_ok = 0;
            Buzz_Time = BUZZ_short_time;
            //
            if (Set_SYS_Hum_timer == 0)
            {
               Set_SYS_Hum_timer = 5;
               return;
            }
            //		M_Hum_Set_buf+=10;
            //		if(M_Hum_Set_buf>70)
            //		{ M_Hum_Set_buf=30; }
            //		M_Hum_Setting_Time=5;
         }
         else if (my_strcmp("2", &SYS_W_Uart.Rxd_buf[buf])) //������ʪ
         {
            if (chack_down_deal(key_power_bit | key_Water_Full_bit))
            {
               SET_MODE_Err = 1;
               return;
            }
            //
            Disp_set_Delay = 10;
            // if(M_continuous)
            //{return;}
            Buzz_Time = BUZZ_short_time;
            //	         M_continuous=1;
            //	         M_write_delay_time=2;
            //	         M_Dry_Status=OFF;
         }
         else if (my_strcmp("3", &SYS_W_Uart.Rxd_buf[buf])) //����ģʽ
         {
            if (chack_down_deal(key_power_bit | key_Water_Full_bit))
            {
               SET_MODE_Err = 1;
               return;
            }
            // if(M_Dry_Status==ON)
            //{return;}
            Disp_set_Delay = 10;
            Set_SYS_Hum_timer = 0;
            Buzz_Time = BUZZ_short_time;
            //                M_Dry_Status=ON;
            //		M_write_delay_time=2;
            //		M_continuous=0;
         }
         else
         {
            SET_MODE_Err = 1;
            return;
         } //���ʹ���
      }
      break;
      //
      case SET_HUM: //ʪ��
      {
         //
         _wifi_T_result_SET_HUM = 1;                              //����OK
         if (chack_down_deal(key_power_bit | key_Water_Full_bit)) //
         {
            SET_HUM_Err = 1;
            return;
         }
         //
         Set_buf = SYS_W_Uart.Rxd_buf[24] - '0'; //
         Set_buf = Set_buf * 10;
         Set_buf += SYS_W_Uart.Rxd_buf[25] - '0';
         //                SYS_Hum_Set_buf=Set_buf;
         //
         Disp_set_Delay = 10;
         //		M_continuous=0;
         //		M_Dry_Status=OFF;
         M_Timer_Setting_Time = 0;
         _Timer_set_ok = 0;
         Buzz_Time = BUZZ_short_time;
         /*if(M_Hum_Setting_Time==0)
         {
             M_Hum_Setting_Time=5;
                       return;
         }*/
         // M_Hum_Set_buf+=10;
         // if(M_Hum_Set_buf>70)
         //{ M_Hum_Set_buf=30; }
         Set_SYS_Hum_timer = 5;
      }
      break;
      // ���÷���   ֻ�����С�׵�Э��
      // �������   ��̨��������
      case SET_FAN:
      {
         _wifi_T_result_SET_FAN = 1;
         //
         if (my_strcmp("0", &SYS_W_Uart.Rxd_buf[buf])) //
         {
            Wifi_Set_Fan = OFF_FAN;
         }
         else if (my_strcmp("1", &SYS_W_Uart.Rxd_buf[buf]))
         {
            Wifi_Set_Fan = SILENCE_FAN;
         }
         else if (my_strcmp("2", &SYS_W_Uart.Rxd_buf[buf]))
         {
            Wifi_Set_Fan = LOW_FAN;
         }
         else if (my_strcmp("3", &SYS_W_Uart.Rxd_buf[buf]))
         {
            Wifi_Set_Fan = HIGH_FAN;
         }
      }
      break;
      }
   }
   else if (SIID_data == '3') // SIIDΪ3
   {
      ;
   }
   else if (SIID_data == '4') // SIIDΪ4
   {
      if (type_buf == SET_BUZZER)
      {
         _wifi_T_result_SET_BUZZER = 1;
         //
         if (my_strcmp("false", &SYS_W_Uart.Rxd_buf[buf]))
         {
            Buzzer_Status = DISABLE;
         }
         else if (my_strcmp("true", &SYS_W_Uart.Rxd_buf[buf]))
         {
            Buzzer_Status = ENABLE;
         }
         else
         {
            SET_BUZZER_Err = 1;
            return;
         }
         //	     M_write_delay_time=EEP_write_T;
         Buzz_Time = BUZZ_long_time;
      }
   }
   else if (SIID_data == '5') // SIIDΪ5
   {
      if (type_buf == SET_LAMP)
      {
         _wifi_T_result_SET_LAMP = 1;
         //
         if (chack_down_deal(key_power_bit | key_Water_Full_bit)) //
         {
            SET_LAMP_Err = 1;
            return;
         }
         //
         if (my_strcmp("false", &SYS_W_Uart.Rxd_buf[buf]))
         {
            LED_Disp_Status = DISABLE;
            Disp_Delay = 0;
            Disp_set_Delay = 0;
         }
         else if (my_strcmp("true", &SYS_W_Uart.Rxd_buf[buf]))
         {
            LED_Disp_Status = ENABLE;
            Disp_set_Delay = 10;
         }
         else //�趨����
         {
            SET_LAMP_Err = 1;
            return;
         }
         //
         Buzz_Time = BUZZ_long_time;
         //	     M_write_delay_time=2;
      }
   }
   else if (SIID_data == '6') // SIIDΪ6
   {
      if (type_buf == SET_CHILD_LOCK)
      {
         _wifi_T_result_SET_Child_Lock = 1;
         //
         if (chack_down_deal(key_Water_Full_bit))
         {
            SET_CHILD_LOCK_Err = 1;
            return;
         }
         //
         Rest_Key_Buzzer();
         if (my_strcmp("false", &SYS_W_Uart.Rxd_buf[buf]))
         {
            Child_Lock_status = DISABLE;
            Child_Lock_Disp_Count = 1;
            Child_Lock_Disp_timer = 50;
         }
         else if (my_strcmp("true", &SYS_W_Uart.Rxd_buf[buf]))
         {
            Child_Lock_status = ENABLE;
            Child_Lock_Disp_Count = Child_Lock_Disp_NUM;
            Child_Lock_Disp_timer = 50;
         }
         else
         {
            SET_CHILD_LOCK_Err = 1;
            return;
         } //ʶ���˵�ָ��

         Child_Lock_1s_Count = 0;
         Buzz_Time = BUZZ_short_time;
         //             M_write_delay_time=2;
      }
   }
   else if (SIID_data == '7') // SIIDΪ7
   {
      ;
   }
   else if (SIID_data == '8') // SIIDΪ8
   {
      if (type_buf == SET_PUMP)
      {
         _wifi_T_result_SET_PUMP = 1;
         if (chack_down_deal(key_power_bit | key_Water_Full_bit)) //
         {
            SET_PUMP_Err = 1;
            return;
         }
         //
         if (my_strcmp("false", &SYS_W_Uart.Rxd_buf[buf]))
         {
            Pump_Status = DISABLE;
         }
         else if (my_strcmp("true", &SYS_W_Uart.Rxd_buf[buf]))
         {
            Pump_Status = ENABLE;
         }
         else
         {
            SET_PUMP_Err = 1;
            return;
         }
         //
         _props_pump_pipe_Status = 1;
         Disp_set_Delay = 10;
         Buzz_Time = BUZZ_short_time;
      }
   }
   else if (SIID_data == '9') // SIIDΪ9
   {
      if (type_buf == SET_TIME)
      {
         //
         _wifi_T_result_SET_TIMER = 1;            //����OK
                                                  //
         if (chack_down_deal(key_Water_Full_bit)) //���� result����
         {
            SET_TIME_Err = 1;
            return;
         }
         //�·���Ϊ>=10h�Ķ�ʱ
         if ((SYS_W_Uart.Rxd_buf[25] <= '9') && (SYS_W_Uart.Rxd_buf[25] >= '0'))
         {
            Set_buf = SYS_W_Uart.Rxd_buf[24] - '0'; //
            Set_buf = Set_buf * 10;
            Set_buf += SYS_W_Uart.Rxd_buf[25] - '0';
         }
         else // ��ʱ<=9
         {
            Set_buf = SYS_W_Uart.Rxd_buf[24] - '0'; //
         }
         M_Timer_Buf = Set_buf;
         M_Timer_Setting_Time = 5;
         _Timer_set_ok = 1;
         Buzz_Time = BUZZ_short_time;
      }
   }
}
/*************************************************
 // ��������    : Wifi_Rxd_COM_Deal
 // ��������    : ���紦��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Wifi_Rxd_COM_Deal(void)
{
   UI08 buf;
   if (_Txd_COM_ok_net) //����״̬��ѯ
   {
      net_deal(0);
      _Txd_COM_ok_net = 0;
      return;
   }
   //
   if (_Txd_COM_ok_get_down)
   {
      if (my_strcmp(down, SYS_W_Uart.Rxd_buf))
      {
         buf = sizeof(down);
      }
      else
      {
         return;
      }
      //
      _Txd_COM_get_down = 0;
      if (my_strcmp(none, &SYS_W_Uart.Rxd_buf[buf]))
      {
         return;
      }
      //
      if (my_strcmp(update_fw, &SYS_W_Uart.Rxd_buf[buf])) //��ѯ�Ƿ����������
      {
         MCU_RESET; //�����λ��
      }
      else if (my_strcmp(MIIO_net_change, &SYS_W_Uart.Rxd_buf[buf])) //ʶ������״̬�ı�
      {
         net_deal(sizeof(MIIO_net_change) + buf);
      }
      else if (my_strcmp(MIIO_mcu_version_req, &SYS_W_Uart.Rxd_buf[buf]))
      {
         _wifi_com_mcu_version_req = 1;
      }
      else if (my_strcmp(MIIO_model_req, &SYS_W_Uart.Rxd_buf[buf]))
      {
         _wifi_com_MIIO_model_req = 1;
      }
      else if (my_strcmp(set_properties, &SYS_W_Uart.Rxd_buf[buf])) //���û�̨״̬
      {
         buf = buf + sizeof(set_properties);
         set_properties_deal(buf);
      }
      else if (my_strcmp(get_properties, &SYS_W_Uart.Rxd_buf[buf])) //��ȡ��̨״̬
      {
         get_properties_deal();
      }
      else
      {
         _wifi_com_Rxd_down_error = 1;
      }
   }
}

/*************************************************
 // ��������    : get_properties_err_deal
 // ��������    : ���wifi��ȡ����ķ�������
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void get_properties_err_deal(UI08 addr_buf)
{
   if ((SYS_W_Uart.Rxd_buf[addr_buf + 3] == ' ') || (SYS_W_Uart.Rxd_buf[addr_buf + 3] == 0x0d))
   {
      SYS_W_Uart.Txd_buf[addr_point++] = ' ';
      SYS_W_Uart.Txd_buf[addr_point++] = SYS_W_Uart.Rxd_buf[addr_buf];     //    '2'
      SYS_W_Uart.Txd_buf[addr_point++] = SYS_W_Uart.Rxd_buf[addr_buf + 1]; //  ' '
      SYS_W_Uart.Txd_buf[addr_point++] = SYS_W_Uart.Rxd_buf[addr_buf + 2]; //  'num'
      SYS_W_Uart.Txd_buf[addr_point++] = ' ';                              //  ' '
   }
   else if ((SYS_W_Uart.Rxd_buf[addr_buf + 4] == ' ') || (SYS_W_Uart.Rxd_buf[addr_buf + 4] == 0x0d)) // get_properties 2 1 2 3 2 5 2 4  ��: 1λ�ò�������10���
   {
      SYS_W_Uart.Txd_buf[addr_point++] = SYS_W_Uart.Rxd_buf[addr_buf];     // '2'
      SYS_W_Uart.Txd_buf[addr_point++] = SYS_W_Uart.Rxd_buf[addr_buf + 1]; // ' '
      SYS_W_Uart.Txd_buf[addr_point++] = SYS_W_Uart.Rxd_buf[addr_buf + 2]; // 'num/10'
      SYS_W_Uart.Txd_buf[addr_point++] = SYS_W_Uart.Rxd_buf[addr_buf + 3]; // 'num%10'
      SYS_W_Uart.Txd_buf[addr_point++] = ' ';                              // ' '
   }
   Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //����  result -4003
   addr_point += sizeof(NO_properties) - 1;
}
/*************************************************
 // ��������    : void  get_properties_deal(void)
 // ��������    : wifiģ�� ��(��)����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void get_properties_deal(void)
{

   UI08 i = 0;
   UI08 j = 0;
   UI08 addr_buf = 0;
   UI08 siid = 0;
   UI08 piid = 0;
   // down get_properties 2 1 2 3 2 5 2 4
   for (i = 0; i < 9; i++)
   {
      Txd_props_bit[i].lword = 0;
   } //������з��ͱ�־
   //
   addr_buf = sizeof(down);
   addr_buf = addr_buf + sizeof(get_properties); //+1  + �ո��λ��

   Txd_data_props(result, 0, sizeof(result) - 1); //����  result
   addr_point = sizeof(result) - 1;
   // get_prop_bit.word=0;
   //
   for (j = 0; j < (get_prop_num + 16); j++)
   {
      if (addr_buf >= SYS_W_Uart.Rxd_byte) //��ȡ�ĵ�ַ�Ƿ񳬳�
      {
         Buzz_Time = 200; //
         return;
      }
      //��ѯ��ѯ����
      if ((SYS_W_Uart.Rxd_buf[addr_buf + 3] == ' ') || (SYS_W_Uart.Rxd_buf[addr_buf + 3] == 0x0d))
      {
         piid = SYS_W_Uart.Rxd_buf[addr_buf + 2] - '0';
         if ((piid < 1) || (piid > 9))
         {
            return;
         }
      }
      else if ((SYS_W_Uart.Rxd_buf[addr_buf + 4] == ' ') || (SYS_W_Uart.Rxd_buf[addr_buf + 4] == 0x0d)) // get_properties 2 1 2 3 2 5 2 4  ��: 1λ�ò�������10���
      {
         piid = SYS_W_Uart.Rxd_buf[addr_buf + 2] - '0';
         piid = piid * 10;
         piid = SYS_W_Uart.Rxd_buf[addr_buf + 3] - '0';
         if ((piid < 1) || (piid > 99))
         {
            return;
         }
      }
      //
      if ((SYS_W_Uart.Rxd_buf[addr_buf] < '1') && (SYS_W_Uart.Rxd_buf[addr_buf] > '9')) //δ�ڶ����siid
      {
         get_properties_err_deal(addr_buf);
      }
      //
      siid = SYS_W_Uart.Rxd_buf[addr_buf] - '0'; //��ȡsiid

      if ((get_properties_SIID_PIID_EN[siid - 1] & (0x00000001 << (piid - 1))) == 0) //û���������
      {
         get_properties_err_deal(addr_buf);
      }
      else
      {
         Txd_props_bit[siid - 1].lword = 0x00000001 << (piid - 1);
         _wifi_Get_Props_load = 1; // get���ݵ�װ��  ���ϱ��������е�����  ���Ӹ�'0'
         Txd_props_Deal();
         Txd_props_bit[siid - 1].lword = 0;
      }

      if (piid < 10)
      {
         if (SYS_W_Uart.Rxd_buf[addr_buf + 3] == 0x0d) //������
         {
            break;
         }
         addr_buf = addr_buf + 4;
      }
      else
      {
         if (SYS_W_Uart.Rxd_buf[addr_buf + 4] == 0x0d) //������
         {
            break;
         }
         addr_buf = addr_buf + 5;
      }
   }
   //
   _wifi_Get_Props_En = 1;
}
/*************************************************
 // ��������    : clear_props_bit
 // ��������    : �����ϱ�ʱ  �յ�OK  ���־
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
/*
void clear_props_bit(void)
{
  UI08 i;
  for(i=0;i<9;i++)//���¼�
  {Txd_props_bit[i].lword&=~Txd_props_ok_bit[i].lword;}
  //���־
  for(i=0;i<9;i++)
  {Txd_props_ok_bit[i].lword=Txd_props_bit[i].lword;}
}
*/
/*************************************************
 // ��������    : load_props_bit
 // ��������    : ����־λ��λ   �յ�OKʱ���
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void load_props_bit(void)
{
   UI08 i;
   for (i = 0; i < 9; i++) // bit װ��
   {
      Txd_props_ok_bit[i].lword = Txd_props_bit[i].lword;
   }
}
/*************************************************
 // ��������    : Wifi_Rxd_Deal
 // ��������    : ���紦��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Wifi_Rxd_Deal(void)
{
   UI08 i;

   if (SYS_W_Uart.Rxd_OK == DISABLE)
   {
      return;
   }
   SYS_W_Uart.Rxd_OK = DISABLE;
   //
   wifi_net_led_timer = 0;
   //
   if (Star_Process_Num < Star_Process_OK_Num) //ʶ���ʼ�����յ�����
   {
      if (my_strcmp(com_ok, SYS_W_Uart.Rxd_buf)) // OK      \r
      {
         Star_Process_Num++;
         if (Star_Process_Num == Star_Process_OK_Num)
         {
            if (Sec_Txd_Star_Process == ENABLE)
            {
               _Txd_COM_net = 1;
            }
         }
      }
      //�����Լ�
      if (my_strcmp(model_nwt, SYS_W_Uart.Rxd_buf))
      {
         if ((!M__Self_Test) && (Power_Delay_Time > 0))
         {
            M__Self_Test = 1;
            M_Power_Delay_Time2 = 0;
            //
            communication_timer = 2;
            //�Լ�д��ϵͳĬ�ϲ���
            SYS_Power_Status = OFF;
            SYS_Hum_Set = 0;
            Child_Lock_status = DISABLE;
            //              M_continuous=1;
            //	      M_Dry_Status=0;
            LED_Disp_Status = ENABLE;
            //              Pump_Status=DISABLE;
            Buzzer_Status = ENABLE;
            SYS_filter_time = 0;
         }
         // Star_Process_Num=2;
      }
   }
   else if (_wifi_com_ok_factory) //��������
   {
      if (my_strcmp(com_ok, SYS_W_Uart.Rxd_buf))
      {
         _wifi_com_factory = 0;
         Wifi_net_Status = e_uap;
         Wifi_net_Status_buf = e_uap;
         _Txd_COM_net = 1;
      }
      _wifi_com_ok_factory = 0;
   }
   else if (_wifi_com_ok_Txd_result_ok)
   {
      /*
      if(my_strcmp(com_ok,SYS_W_Uart.Rxd_buf))
      {
           _wifi_com_Txd_result_ok=0;
      }
      else if(my_strcmp(wifi_error,SYS_W_Uart.Rxd_buf))
      {
            _wifi_com_Txd_result_ok=0;
      }
      */
      _wifi_com_Txd_result_ok = 0;
      _wifi_com_ok_Txd_result_ok = 0;
   }
   else if (_wifi_com_ok_mcu_version_req)
   {
      _wifi_com_mcu_version_req = 0;
      _wifi_com_ok_mcu_version_req = 0;
   }
   else if (_wifi_com_ok_MIIO_model_req)
   {
      _wifi_com_MIIO_model_req = 0;
      _wifi_com_ok_MIIO_model_req = 0;
   }
   else if (_wifi_com_ok_Rxd_down_error)
   {
      _wifi_com_Rxd_down_error = 0;
      _wifi_com_ok_Rxd_down_error = 0;
   }
   else if (_wifi_com_ok_Rxd_get_prop_error)
   {
      _wifi_com_Rxd_get_prop_error = 0;
      _wifi_com_ok_Rxd_get_prop_error = 0;
   }
   /*else if(_wifi_com_ok_props_error)
   {
      _wifi_com_props_error=0;
   _wifi_com_ok_props_error=0;
   }*/
   else if (_wifi_com_ok_MIIO_reboot)
   {
      if (my_strcmp(com_ok, SYS_W_Uart.Rxd_buf))
      {
         _wifi_com_MIIO_reboot = 0;
         Star_Process_Num = 1;
         Wifi_net_Status = e_uap;
         Wifi_net_Status_buf = e_uap;
      }
      _wifi_com_ok_MIIO_reboot = 0;
   }
   //
   else if (Txd_event_ok_bit.lword)
   {
      // if(my_strcmp(com_ok,SYS_W_Uart.Rxd_buf))
      {
         Txd_event_bit.lword &= ~Txd_event_ok_bit.lword;
         Txd_event_ok_bit.lword &= ~Txd_event_ok_bit.lword; // 20191109
      }
      // Txd_event_bit.lword=0;
      // Txd_event_ok_bit.lword=0;
   }
   else if ((Txd_props_ok_bit[0].lword) // 8��SIID�����ݸ���
            || (Txd_props_ok_bit[1].lword) || (Txd_props_ok_bit[2].lword) || (Txd_props_ok_bit[3].lword) || (Txd_props_ok_bit[4].lword) || (Txd_props_ok_bit[5].lword) || (Txd_props_ok_bit[6].lword) || (Txd_props_ok_bit[7].lword) || (Txd_props_ok_bit[8].lword))
   {
      /*if(my_strcmp(com_ok,SYS_W_Uart.Rxd_buf))//�յ�ok  ����ϱ��¼�bit
      {
       clear_props_bit();
      }*/

      for (i = 0; i < 9; i++)
      {
         Txd_props_ok_bit[i].lword = 0;
      }
   }
   else if (Txd_COM_ok_bit.lword)
   {
      Wifi_Rxd_COM_Deal();
      Txd_COM_ok_bit.lword = 0;
   }
   else if (R_result_bit.word)
   {
      // if(my_strcmp(com_ok,SYS_W_Uart.Rxd_buf))
      //{
      //    R_result_bit.word=0;
      //}
      R_result_bit.word = 0;
   }
   Wifi_model_load = ENABLE; //�յ�OK-wifiģ����װ��
   Clear_Data(SYS_W_Uart.Rxd_buf, SYS_W_Uart.Rxd_byte);
   SYS_W_Uart.Rxd_byte = 0;
}
/*************************************************
 // ��������    : Txd_data
 // ��������    : ���ݷ���
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Txd_data(const UI08 *point, UI16 lenth)
{
   UI08 i = 0;

   if (lenth >= TXD_MAX)
   {
      return;
   }
   for (i = 0; i < lenth; i++)
   {
      SYS_W_Uart.Txd_buf[i] = *point;
      point++;
   }
   SYS_W_Uart.Txd_buf[lenth] = 0X0D; // \r
   SYS_W_Uart.Txd_Lenth = lenth + 1;
   SYS_W_Uart.Txd_byte = 0;
   Wifi_UART_Txd_Data();
}
/*************************************************
 // ��������    : Txd_data_props
 // ��������    : ���紦��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Txd_data_props(const UI08 *point, UI16 addr, UI16 lenth)
{
   UI16 i = addr;

   if (addr + lenth >= TXD_MAX)
   {
      return;
   }

   for (; i < (addr + lenth); i++)
   {
      SYS_W_Uart.Txd_buf[i] = *point;
      point++;
   }
}
/*************************************************
 // ��������    : sys_humidity
 // ��������    : ���紦��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void sys_humidity(UI16 *point)
{
   UI08 buf = 0;
   if (Hum_para.value > 90)
   {
      buf = 90;
   }
   else if (Hum_para.value < 30)
   {
      buf = 30;
   }
   else
   {
      buf = Hum_para.value;
   }
   SYS_W_Uart.Txd_buf[(*point)++] = (buf / 10) + '0';
   SYS_W_Uart.Txd_buf[(*point)++] = (buf % 10) + '0';
}
/*************************************************
 // ��������    : sys_temp
 // ��������    : ���紦��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void sys_temp(UI16 *point, UI08 temp_buf)
{
   if (temp_buf >= 9)
   {
      temp_buf = temp_buf - 9;
      SYS_W_Uart.Txd_buf[(*point)++] = (temp_buf / 10) + '0';
      SYS_W_Uart.Txd_buf[(*point)++] = (temp_buf % 10) + '0';
   }
   else
   {
      temp_buf = 9 - temp_buf;
      SYS_W_Uart.Txd_buf[(*point)++] = '-';
      SYS_W_Uart.Txd_buf[(*point)++] = (temp_buf / 10) + '0';
      SYS_W_Uart.Txd_buf[(*point)++] = (temp_buf % 10) + '0';
   }
}
/*************************************************
 // ��������    : Txd_event_Deal
 // ��������    : ͨ�Ź��ϴ���
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Txd_event_Deal(void)
{
   UI08 i = 0;

   Txd_data_props(event_code, 0, sizeof(event_code) - 1); //װ���ַ��� event_occured
   addr_point = sizeof(event_code) - 1;
   //
   for (i = 0; i < 8; i++) //�߸�
   {
      if (Txd_event_bit.lword & (0x00000001 << i))
      {
         SYS_W_Uart.Txd_buf[addr_point++] = ' ';
         SYS_W_Uart.Txd_buf[addr_point++] = 7 + '0';
         SYS_W_Uart.Txd_buf[addr_point++] = ' ';
         SYS_W_Uart.Txd_buf[addr_point++] = i + 1 + '0'; // 0��ʱ�� ��'1'
         Txd_event_ok_bit.lword = (0x00000001 << i);
         break;
      }
   }
}
/*************************************************
 // ��������    : SIID_PIID_Load
 // ��������    : ���ݵ�װ��
 // ��ڲ���    : data_buf1 data_buf2
 // ���ڲ���    : ��
***************************************************/
void SIID_PIID_Load(UI08 data_buf1, UI08 data_buf2)
{
   SYS_W_Uart.Txd_buf[addr_point++] = ' ';
   SYS_W_Uart.Txd_buf[addr_point++] = data_buf1;
   SYS_W_Uart.Txd_buf[addr_point++] = ' ';
   SYS_W_Uart.Txd_buf[addr_point++] = data_buf2;
   SYS_W_Uart.Txd_buf[addr_point++] = ' ';
}
/*************************************************
 // ��������    : get_Props_load_Ascii_0
 // ��������    : wifi get����ʱ �� PIID��
                  ״̬һ��'0'(0-���سɹ�)
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void get_Props_load_Ascii_0(void)
{
   if (!_wifi_Get_Props_load)
   {
      return;
   }
   _wifi_Get_Props_load = 0;
   //
   // SYS_W_Uart.Txd_buf[addr_point++]=' ';
   SYS_W_Uart.Txd_buf[addr_point++] = '0';
   SYS_W_Uart.Txd_buf[addr_point++] = ' ';
}
/*************************************************
 // ��������    : SIID1_Props
 // ��������    : �ظ�/�ϱ� SIID1����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void SIID1_Props(void)
{
   if (_props_manufacturer) //������
   {
      SIID_PIID_Load('1', '1');
      get_Props_load_Ascii_0();
      Txd_data_props(manufacturer, addr_point, sizeof(manufacturer) - 1);
      addr_point = addr_point + sizeof(manufacturer) - 1; //
   }
   if (_props_model) //ģ����Ϣ
   {
      SIID_PIID_Load('1', '2');
      get_Props_load_Ascii_0();
      Txd_data_props(model_nwt, addr_point, sizeof(model_nwt) - 1);
      addr_point = addr_point + sizeof(model_nwt) - 1; //
   }
   if (_props_serial_number) //���к�
   {
      SIID_PIID_Load('1', '3');
      get_Props_load_Ascii_0();
      Txd_data_props(serial_number, addr_point, sizeof(serial_number) - 1);
      addr_point = addr_point + sizeof(serial_number) - 1; //
   }
   if (_props_revision) //����汾
   {
      SIID_PIID_Load('1', '4');
      get_Props_load_Ascii_0();
      Txd_data_props(mcu_version, addr_point, sizeof(mcu_version) - 1);
      addr_point = addr_point + sizeof(mcu_version) - 1; //
   }
}
/*************************************************
 // ��������    : SIID2_Props
 // ��������    : �ظ�/�ϱ� SIID2����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void SIID2_Props(void)
{
   //���ÿ��ػ�
   if (_props_on_off)
   {
      SIID_PIID_Load('2', '1');
      get_Props_load_Ascii_0();
      //
      if (SYS_Power_Status == ON)
      {
         Txd_data_props(Code_true, addr_point, sizeof(Code_true) - 1);
         addr_point = addr_point + sizeof(Code_true) - 1; //
      }
      else
      {
         Txd_data_props(Code_false, addr_point, sizeof(Code_false) - 1);
         addr_point = addr_point + sizeof(Code_false) - 1;
      }
   }
   // fault
   if (_props_fault)
   {
      SIID_PIID_Load('2', '2');
      get_Props_load_Ascii_0();
      SYS_W_Uart.Txd_buf[addr_point++] = '0';
      // Txd_data_props(Code_false,addr_point, sizeof(Code_false)-1);
      // addr_point=addr_point+sizeof(Code_false)-1;
   }
   //����ģʽ
   if (_props_mode)
   {
      SIID_PIID_Load('2', '3');
      get_Props_load_Ascii_0();
      //
      /*
       if(M_continuous)
       {
            Txd_data_props(Ascii_2,addr_point, sizeof(Ascii_2)-1);
      addr_point=addr_point+sizeof(Ascii_2)-1;//

       }
       else if(M_Dry_Status)
       {
            Txd_data_props(Ascii_3,addr_point, sizeof(Ascii_3)-1);
      addr_point=addr_point+sizeof(Ascii_3)-1;//
       }
       else
       {
          Txd_data_props(Ascii_1,addr_point, sizeof(Ascii_1)-1);
      addr_point=addr_point+sizeof(Ascii_1)-1;//
       }	 */
   }
   //״̬
   if (_props_status)
   {
      SIID_PIID_Load('2', '4');
      get_Props_load_Ascii_0();
      SYS_W_Uart.Txd_buf[addr_point++] = '1';
   }
   //
   if (_props_hum_set)
   {
      SIID_PIID_Load('2', '5');
      get_Props_load_Ascii_0();
      //
      SYS_W_Uart.Txd_buf[addr_point++] = SYS_Hum_Set_Buf / 10 + '0';
      SYS_W_Uart.Txd_buf[addr_point++] = SYS_Hum_Set_Buf % 10 + '0';
   }
   //
   /*if(_props_tank_full)
   {
     SIID_PIID_Load('2','6');
   }*/
   //
   if (_props_fan_speed)
   {
      SIID_PIID_Load('2', '7');
      get_Props_load_Ascii_0();
      //��Ϊ������Ϊ�����wifiЭ���(���ٲ��ɵ�)
      //�лظ��ķ���Ҫ���趨��һ��
      //�����ϱ���Ҳ�����ķ���
      if (Wifi_Set_Fan == SILENCE_FAN)
      {
         SYS_W_Uart.Txd_buf[addr_point++] = '1';
      }
      else if (Wifi_Set_Fan == LOW_FAN)
      {
         SYS_W_Uart.Txd_buf[addr_point++] = '2';
      }
      else if (Wifi_Set_Fan == HIGH_FAN)
      {
         SYS_W_Uart.Txd_buf[addr_point++] = '3';
      }
      else // FANOFF
      {
         SYS_W_Uart.Txd_buf[addr_point++] = '0';
      }
   }
}
/*************************************************
 // ��������    : SIID3_Props
 // ��������    : �ظ�/�ϱ� SIID3����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void SIID3_Props(void)
{
   //����ʪ��
   if (_props_humidity)
   {
      SIID_PIID_Load('3', '1');
      get_Props_load_Ascii_0();
      sys_humidity(&addr_point);
   }
   //����
   if (_props_temp)
   {
      SIID_PIID_Load('3', '7');
      get_Props_load_Ascii_0();
      sys_temp(&addr_point, Temp_room.value);
   }
}
/*************************************************
 // ��������    : SIID4_Props
 // ��������    : �ظ�/�ϱ� SIID4����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void SIID4_Props(void)
{
   //������ʾ��
   if (_props_buzzer)
   {
      SIID_PIID_Load('4', '1');
      get_Props_load_Ascii_0();
      //
      if (Buzzer_Status)
      {
         Txd_data_props(Code_true, addr_point, sizeof(Code_true) - 1);
         addr_point = addr_point + sizeof(Code_true) - 1; //
      }
      else
      {
         Txd_data_props(Code_false, addr_point, sizeof(Code_false) - 1);
         addr_point = addr_point + sizeof(Code_false) - 1;
      }
   }
}
/*************************************************
 // ��������    : SIID5_Props
 // ��������    : �ظ�/�ϱ� SIID5����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void SIID5_Props(void)
{
   //����ָʾ��
   if (_props_led)
   {
      SIID_PIID_Load('5', '1');
      get_Props_load_Ascii_0();
      //
      if (LED_Disp_Status)
      {
         Txd_data_props(Code_true, addr_point, sizeof(Code_true) - 1);
         addr_point = addr_point + sizeof(Code_true) - 1; //
      }
      else
      {
         Txd_data_props(Code_false, addr_point, sizeof(Code_false) - 1);
         addr_point = addr_point + sizeof(Code_false) - 1;
      }
   }
}
/*************************************************
 // ��������    : SIID6_Props
 // ��������    : �ظ�/�ϱ� SIID6����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void SIID6_Props(void)
{
   //����ͯ��
   if (_props_child_lock)
   {
      SIID_PIID_Load('6', '1');
      get_Props_load_Ascii_0();
      //
      if (Child_Lock_status)
      {
         Txd_data_props(Code_true, addr_point, sizeof(Code_true) - 1);
         addr_point = addr_point + sizeof(Code_true) - 1; //
      }
      else
      {
         Txd_data_props(Code_false, addr_point, sizeof(Code_false) - 1);
         addr_point = addr_point + sizeof(Code_false) - 1;
      }
   }
}
/*************************************************
 // ��������    : SIID7_Props
 // ��������    : �ظ�/�ϱ� SIID7����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void SIID7_Props(void)
{
   //����
   if (_props_temp_coil)
   {
      SIID_PIID_Load('7', '1');
      get_Props_load_Ascii_0();
      sys_temp(&addr_point, Temp_coil.value);
   }
   //ѹ����״̬
   if (_props_compressor_status)
   {
      SIID_PIID_Load('7', '2');
      get_Props_load_Ascii_0();
      if (Comp_para.OUT == ON)
      {
         Txd_data_props(Code_true, addr_point, sizeof(Code_true) - 1);
         addr_point = addr_point + sizeof(Code_true) - 1; //
      }
      else
      {
         Txd_data_props(Code_false, addr_point, sizeof(Code_false) - 1);
         addr_point = addr_point + sizeof(Code_false) - 1;
      }
   }
   //��ˮ
   if (_props_tank_full)
   {
      SIID_PIID_Load('7', '3');
      get_Props_load_Ascii_0();
      if (Sys_Err.Water_Full == ENABLE)
      {
         Txd_data_props(Code_true, addr_point, sizeof(Code_true) - 1);
         addr_point = addr_point + sizeof(Code_true) - 1; //
      }
      else
      {
         Txd_data_props(Code_false, addr_point, sizeof(Code_false) - 1);
         addr_point = addr_point + sizeof(Code_false) - 1;
      }
   }
   //
   if (_props_defrost_status)
   {
      SIID_PIID_Load('7', '4');
      get_Props_load_Ascii_0();
      if (M_Defrost_status)
      {
         Txd_data_props(Code_true, addr_point, sizeof(Code_true) - 1);
         addr_point = addr_point + sizeof(Code_true) - 1; //
      }
      else
      {
         Txd_data_props(Code_false, addr_point, sizeof(Code_false) - 1);
         addr_point = addr_point + sizeof(Code_false) - 1;
      }
   }
   if (_props_pump_sw_status)
   {
      SIID_PIID_Load('7', '5');
   }
}
/*************************************************
 // ��������    : SIID8_Props
 // ��������    : �ظ�/�ϱ� SIID8����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void SIID8_Props(void)
{
   //����ˮ��
   if (_props_pump)
   {
      SIID_PIID_Load('8', '1');
      get_Props_load_Ascii_0();
      //
      if (Pump_Status)
      {
         Txd_data_props(Code_true, addr_point, sizeof(Code_true) - 1);
         addr_point = addr_point + sizeof(Code_true) - 1; //
      }
      else
      {
         Txd_data_props(Code_false, addr_point, sizeof(Code_false) - 1);
         addr_point = addr_point + sizeof(Code_false) - 1;
      }
   }
   if (_props_pump_pipe_Status)
   {
      SIID_PIID_Load('8', '2');
      get_Props_load_Ascii_0();
      //
      if ((Pump_water_pipe_Status == DISABLE) && (Pump_Status == ENABLE))
      {
         Txd_data_props(Code_false, addr_point, sizeof(Code_false) - 1);
         addr_point = addr_point + sizeof(Code_false) - 1;
      }
      else
      {
         Txd_data_props(Code_true, addr_point, sizeof(Code_true) - 1);
         addr_point = addr_point + sizeof(Code_true) - 1; //
      }
   }
}

/*************************************************
 // ��������    : SIID9_Props
 // ��������    : �ظ�/�ϱ� SIID9����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/

void SIID9_Props(void)
{
   UI16 time_run_min = 0;
   UI08 wifi_set_timer = 0;
   //��ʱ
   if (_props_time_run)
   {
      SIID_PIID_Load('9', '1');
      get_Props_load_Ascii_0();
      //�ϴ�����
      time_run_min = (M_Timer_Run + 1) / 60;
      //
      if (time_run_min <= 9)
      {
         SYS_W_Uart.Txd_buf[addr_point++] = time_run_min % 10 + '0';
      }
      else if (time_run_min <= 99)
      {
         SYS_W_Uart.Txd_buf[addr_point++] = time_run_min / 10 + '0';
         SYS_W_Uart.Txd_buf[addr_point++] = time_run_min % 10 + '0';
      }
      else
      {
         SYS_W_Uart.Txd_buf[addr_point++] = time_run_min / 100 + '0';
         SYS_W_Uart.Txd_buf[addr_point++] = time_run_min % 100 / 10 + '0';
         SYS_W_Uart.Txd_buf[addr_point++] = time_run_min % 10 + '0';
      }
   }
   //
   if (_props_run_set)
   {
      SIID_PIID_Load('9', '2');
      get_Props_load_Ascii_0();

      if (M_Timer_Run == 0)
      {
         wifi_set_timer = 0;
      }
      else if (M_Timer_Setting_Time)
      {
         wifi_set_timer = M_Timer_Buf;
      }
      else
      {
         wifi_set_timer = M_Timer_Set;
      }

      if (wifi_set_timer <= 9)
      {
         SYS_W_Uart.Txd_buf[addr_point++] = wifi_set_timer % 10 + '0';
      }
      else if (M_Timer_Set <= 99)
      {
         SYS_W_Uart.Txd_buf[addr_point++] = wifi_set_timer / 10 + '0';
         SYS_W_Uart.Txd_buf[addr_point++] = wifi_set_timer % 10 + '0';
      }
      else
      {
         ;
      }
   }
}
/*************************************************
 // ��������    : Txd_props_Deal
 // ��������    : �����б仯ʱ�ϱ�
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Txd_props_Deal(void)
{
   // SIID 1
   if (Txd_props_bit[0].lword)
   {
      SIID1_Props();
   }
   // SIID 2
   if (Txd_props_bit[1].lword)
   {
      SIID2_Props();
   }
   // SIID 3
   if (Txd_props_bit[2].lword)
   {
      SIID3_Props();
   }
   // SIID 4
   if (Txd_props_bit[3].lword)
   {
      SIID4_Props();
   }
   // SIID 5
   if (Txd_props_bit[4].lword)
   {
      SIID5_Props();
   }
   // SIID 6
   if (Txd_props_bit[5].lword)
   {
      SIID6_Props();
   }
   // SIID 7
   if (Txd_props_bit[6].lword)
   {
      SIID7_Props();
   }
   // SIID 8
   if (Txd_props_bit[7].lword)
   {
      SIID8_Props();
   }
   //
   if (Txd_props_bit[8].lword)
   {
      SIID9_Props();
   }
}
/*************************************************
 // ��������    : Wifi_Txd_result
 // ��������    : ���紦��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Wifi_Txd_result(void)
{
   addr_point = 0;
   Txd_data_props("result", addr_point, sizeof(result) - 1);
   addr_point = sizeof(result) - 1;

   //Ӧ�� ��/�� ��
   if (_wifi_T_result_SET_POWER)
   {
      _wifi_T_result_SET_POWER = 0;
      _wifi_R_result_SET_POWER = 1;
      //
      SIID_PIID_Load('2', '1');
      if (SET_POWER_Err == 0)
      {
         _props_on_off = 1;
         SYS_W_Uart.Txd_buf[addr_point++] = '0';
      }
      else
      {
         SET_POWER_Err = 0;
         SYS_W_Uart.Txd_buf[addr_point++] = ' ';                               // ' '
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //����  result -4003
         addr_point += sizeof(NO_properties) - 1;
      }
   }
   //Ӧ�� ģʽ
   if (_wifi_T_result_SET_MODE)
   {
      _wifi_T_result_SET_MODE = 0;
      _wifi_R_result_SET_MODE = 1;
      //
      SIID_PIID_Load('2', '3');
      if (SET_MODE_Err == 0)
      {
         SYS_W_Uart.Txd_buf[addr_point++] = '0';
         _props_mode = 1;
         _props_hum_set = 1;
      }
      else
      {
         SET_MODE_Err = 0;
         SYS_W_Uart.Txd_buf[addr_point++] = ' ';                               // ' '
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //����  result -4003
         addr_point += sizeof(NO_properties) - 1;
      }
   }
   //
   //Ӧ�� ģʽ
   if (_wifi_T_result_SET_HUM)
   {
      _wifi_T_result_SET_HUM = 0;
      _wifi_R_result_SET_HUM = 1;
      SIID_PIID_Load('2', '5');
      //
      if (SET_HUM_Err == 0)
      {
         SYS_W_Uart.Txd_buf[addr_point++] = '0';
         _props_hum_set = 1;
      }
      else
      {
         SET_HUM_Err = 0;
         SYS_W_Uart.Txd_buf[addr_point++] = ' ';                               // ' '
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //����  result -4003
         addr_point += sizeof(NO_properties) - 1;
      }
   }
   //
   if (_wifi_T_result_SET_LAMP)
   {
      _wifi_T_result_SET_LAMP = 0;
      _wifi_R_result_SET_LAMP = 1;
      //
      SIID_PIID_Load('5', '1');
      //
      if (SET_LAMP_Err == 0)
      {
         _props_led = 1;
         SYS_W_Uart.Txd_buf[addr_point++] = '0';
      }
      else
      {
         SET_LAMP_Err = 0;
         SYS_W_Uart.Txd_buf[addr_point++] = ' ';                               // ' '
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //����  result -4003
         addr_point += sizeof(NO_properties) - 1;
      }
   }
   //
   if (_wifi_T_result_SET_BUZZER)
   {
      _wifi_T_result_SET_BUZZER = 0;
      _wifi_R_result_SET_BUZZER = 1;
      //
      SIID_PIID_Load('4', '1');

      if (SET_BUZZER_Err == 0)
      {
         _props_buzzer = 1; //���Ա仯  �����ϱ�
         SYS_W_Uart.Txd_buf[addr_point++] = '0';
      }
      else
      {
         SET_BUZZER_Err = 0;
         SYS_W_Uart.Txd_buf[addr_point++] = ' ';                               // ' '
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //����  result -4003
         addr_point += sizeof(NO_properties) - 1;
      }
   }

   if (_wifi_T_result_SET_Child_Lock)
   {
      _wifi_T_result_SET_Child_Lock = 0;
      _wifi_R_result_SET_Child_Lock = 1;
      //
      SIID_PIID_Load('6', '1');
      //
      if (SET_CHILD_LOCK_Err == 0)
      {
         _props_child_lock = 1;
         SYS_W_Uart.Txd_buf[addr_point++] = '0';
      }
      else
      {
         SET_CHILD_LOCK_Err = 0;
         SYS_W_Uart.Txd_buf[addr_point++] = ' ';                               // ' '
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //����  result -4003
         addr_point += sizeof(NO_properties) - 1;
      }
   }

   if (_wifi_T_result_SET_PUMP)
   {
      _wifi_T_result_SET_PUMP = 0;
      _wifi_R_result_SET_PUMP = 1;

      //
      SIID_PIID_Load('8', '1');
      if (SET_PUMP_Err == 0)
      {
         _props_pump = 1;
         SYS_W_Uart.Txd_buf[addr_point++] = '0';
      }
      else
      {
         SET_PUMP_Err = 0;
         SYS_W_Uart.Txd_buf[addr_point++] = ' ';                               // ' '
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //����  result -4003
         addr_point += sizeof(NO_properties) - 1;
      }
   }

   if (_wifi_T_result_SET_TIMER)
   {
      _wifi_T_result_SET_TIMER = 0;
      _wifi_R_result_SET_TIMER = 1;
      SIID_PIID_Load('9', '2');
      //
      if (SET_TIME_Err == 0)
      {
         _props_run_set = 1;
         SYS_W_Uart.Txd_buf[addr_point++] = '0';
      }
      else
      {
         SET_TIME_Err = 0;
         SYS_W_Uart.Txd_buf[addr_point++] = ' ';                               // ' '
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //����  result -4003
         addr_point += sizeof(NO_properties) - 1;
      }
   }

   //
   if (_wifi_T_result_SET_FAN)
   {
      _wifi_T_result_SET_FAN = 0;
      _wifi_R_result_SET_FAN = 1;
      _props_fan_speed = 1;
      //
      SIID_PIID_Load('2', '7');
      SYS_W_Uart.Txd_buf[addr_point++] = '0';
   }

   Txd_data(SYS_W_Uart.Txd_buf, addr_point);
}

/*************************************************
 // ��������    : Wifi_Txd_Deal
 // ��������    : ���紦��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Wifi_Txd_Deal(void)
{

   if (Wifi_txd_en == DISABLE) //ÿ100ms����һ��
   {
      return;
   }
   Wifi_txd_en = DISABLE;

   Wifi_COM_Deal();
   /*
   if(_wifi_com_result_busy)
   {
          _wifi_com_ok_result_busy=1;
       Txd_data(result_busy,sizeof(result_busy)-1);
       return;
   }
   */
   if (_wifi_com_factory) //��������
   {
      Txd_data(factory, sizeof(factory) - 1);
      _wifi_com_ok_factory = 1;
      return;
   }

   if (_wifi_com_Txd_result_ok)
   {
      Txd_data(result_ok, sizeof(result_ok) - 1);
      _wifi_com_ok_Txd_result_ok = 1;
      _wifi_com_Txd_result_ok = 0;
      return;
   }

   if (_wifi_com_mcu_version_req)
   {
      _wifi_com_ok_mcu_version_req = 1;
      Txd_data(mcu_version, sizeof(mcu_version) - 1);
      return;
   }

   if (_wifi_com_MIIO_model_req)
   {
      _wifi_com_ok_MIIO_model_req = 1;
      Txd_data(model_nwt, sizeof(model_nwt) - 1);
      return;
   }

   if (_wifi_com_Rxd_down_error)
   {
      Txd_data(error_invalid_operation, sizeof(error_invalid_operation) - 1);
      _wifi_com_ok_Rxd_down_error = 1;
      return;
   }

   if (_wifi_com_Rxd_get_prop_error)
   {
      Txd_data(error_invalid_operation, sizeof(error_invalid_operation) - 1);
      _wifi_com_ok_Rxd_get_prop_error = 1;
      return;
   }

   /*if(_wifi_com_props_error)
   {
       Txd_data(error_invalid_operation1,sizeof(error_invalid_operation1)-1);
       _wifi_com_ok_props_error=1;
   return;
   }*/

   if (_wifi_com_MIIO_reboot)
   {
      Txd_data(reboot, sizeof(reboot) - 1);
      _wifi_com_ok_MIIO_reboot = 1;
      return;
   }
   //Ӧ��wifiģ���ȡ������
   if (Get_Props_bit.word)
   {
      Get_Props_bit.word = 0;
      Txd_data(SYS_W_Uart.Txd_buf, addr_point);
      return;
   }
   //�����¼�   1s����һ��  ��Ȼ�������̫��  APP�᲻����
   if ((Txd_event_bit.lword) && (_s_event_Updata))
   {
      _s_event_Updata = 0;
      Txd_event_Deal();
      Txd_data(SYS_W_Uart.Txd_buf, addr_point);
      // 20200311  �޸��ϱ����Ϸ��꼴����(Ԥ��wifiģ�鲻��Ӧ,����������������������)
      Txd_event_bit.lword &= ~Txd_event_ok_bit.lword; //�����Ѿ��ϱ���bit����
      // Txd_event_ok_bit.lword&=~Txd_event_ok_bit.lword;
      return;
   }

   //Ӧ��wifiģ���·�������
   if (T_result_bit.word)
   {
      Wifi_Txd_result();
      return;
   }
   //�ϱ���̨�仯�Ĳ���
   if ((Txd_props_bit[0].lword) // 8��SIID�����ݸ���
       || (Txd_props_bit[1].lword) || (Txd_props_bit[2].lword) || (Txd_props_bit[3].lword) || (Txd_props_bit[4].lword) || (Txd_props_bit[5].lword) || (Txd_props_bit[6].lword) || (Txd_props_bit[7].lword) || (Txd_props_bit[8].lword))
   {
      addr_point = 0;
      Txd_data_props(properties_changed, addr_point, sizeof(properties_changed) - 1);
      addr_point = sizeof(properties_changed) - 1;
      //
      Txd_props_Deal();
      //��������
      Txd_data(SYS_W_Uart.Txd_buf, addr_point);
      //
      // Txd_props_ok_bit.lword=Txd_props_bit.lword;
      load_props_bit();
      //����
      Txd_props_bit[0].lword = 0; // 8��SIID�����ݸ���
      Txd_props_bit[1].lword = 0;
      Txd_props_bit[2].lword = 0;
      Txd_props_bit[3].lword = 0;
      Txd_props_bit[4].lword = 0;
      Txd_props_bit[5].lword = 0;
      Txd_props_bit[6].lword = 0;
      Txd_props_bit[7].lword = 0;
      Txd_props_bit[8].lword = 0;
      return;
   }
   //
   if (Txd_COM_bit.lword)
   {
      if (_Txd_COM_get_down) //��wifiģ�������·�����
      {
         Txd_data(get_down, sizeof(get_down) - 1);
         _Txd_COM_ok_get_down = 1;
      }
      else if (_Txd_COM_net) //ѯ������״̬
      {
         Txd_data(net, sizeof(net) - 1);
         _Txd_COM_ok_net = 1;
      }

      return;
   }
}

/*************************************************
 // ��������    : Wifi_Star_Process
 // ��������    : ���紦��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Wifi_Star_Process(void)
{
   if (Wifi_txd_en == DISABLE)
   {
      return;
   }
   Wifi_txd_en = DISABLE;
   //
   switch (Star_Process_Num)
   {
   case 0: //���WIFI����-wifi��λ
   {
      Txd_data(Restore_WIFI, sizeof(Restore_WIFI) - 1);
   }
   break;
   case 1: //��������ʶ���
   {
      Txd_data(model_nwt, sizeof(model_nwt) - 1);
   }
   break;
   case 2: //���Ͱ汾
   {
      Txd_data(mcu_version, sizeof(mcu_version) - 1);
   }
   break;
   case 3: //��������
   {
      Txd_data(ble_config, sizeof(ble_config) - 1);
   }
   break;
   }
}
/*************************************************
 // ��������    : rest_wifi
 // ��������    : wifi��λ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Rest_Wifi(void)
{
   test_factory = DISABLE;
   SYSData_Rest();
   Star_Process_Num = 0; //
   Wifi_net_Status = e_uap;
   Wifi_net_Status_buf = e_uap;
   Sec_Txd_Star_Process = ENABLE;
}
/*************************************************
 // ��������    : Wifi_COM_Deal
 // ��������    : ���紦��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Wifi_COM_Deal(void)
{
   //�������ӵ�·����
   if ((Wifi_net_Status == e_cloud) && (Wifi_net_Status_buf == e_cloud)) //ÿ�������ɹ������ѻ�̨״̬����һ��
   {
      // wifi_net_led_timer=WIFI_NET_TIMER;
      if (Sec_Txd_Star_Process == ENABLE)
      {
         Sec_Txd_Star_Process = DISABLE;
         Star_Process_Num = 1;
         // Txd_props_bit.lword=0xffffffff;
         //�������ϱ�
         _props_on_off = 1;
         _props_mode = 1;
         _props_hum_set = 1;
         _props_led = 1;
         _props_buzzer = 1;
         _props_pump = 1;
         _props_child_lock = 1;
         _props_temp = 1;
         _props_humidity = 1;
         _props_temp_coil = 1;
         _props_compressor_status = 1;
         _props_tank_full = 1;
         _props_defrost_status = 1;
         _props_pump_sw_status = 1;
         _props_fan_speed = 1;
         _props_time_run = 1;
         _props_run_set = 1;
         _props_pump_pipe_Status = 1;
      }
   }

   if ((Wifi_net_Status == e_cloud) //ѯ��״̬  ����״̬��  50��get_down��ѯһ��
       || (Wifi_net_Status == e_updating))
   {
      if (++get_down_cont < 50)
      {
         _Txd_COM_get_down = 1;
      }
      else
      {
         get_down_cont = 0;
         _Txd_COM_net = 1;
      }
   }
   else //ѯ��״̬  ����  10��get_down��ѯһ��
   {
      if (++get_down_cont < 10)
      {
         _Txd_COM_get_down = 1;
      }
      else
      {
         get_down_cont = 0;
         _Txd_COM_net = 1;
      }
   }
}
/*************************************************
 // ��������    : Wifi_Self_Test_Deal
 // ��������    : wifi ��·�Լ�
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Wifi_Self_Test_Deal(void)
{
   // 100ms����һ��
   if (_Self_Test_wifi_TXD_en)
   {
      _Self_Test_wifi_TXD_en = 0;
      Txd_data(Self_Test, sizeof(Self_Test) - 1);
   }
   //����
   if (SYS_W_Uart.Rxd_OK == DISABLE)
   {
      return;
   }
   SYS_W_Uart.Rxd_OK = DISABLE;
   //
   if (my_strcmp(Self_Test, SYS_W_Uart.Rxd_buf))
   {
      Self_Test_wifi_err_time = 200;
   }

   Clear_Data(SYS_W_Uart.Rxd_buf, SYS_W_Uart.Rxd_byte);
   SYS_W_Uart.Rxd_byte = 0;
}
/*************************************************
 // ��������    : Wifi_UART_Deal
 // ��������    : ���紦��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Wifi_UART_Deal(void)
{
   // prg_Wifi_UART_ms();
   prg_Wifi_UART_ms10();
   prg_S_Wifi_UART();
   //�Լ�  ͨ���Է�����Self_Test
   //�ж�ͨ���Ƿ�����
   if (M__Self_Test)
   {
      Wifi_Self_Test_Deal();
      return;
   }
   //
   Wifi_Rxd_Deal();
   if ((SYS_W_Uart.Txd_OK == ENABLE) && (SYS_W_Uart.Rxd_Timerout == 0))
   {
      if (Star_Process_Num < Star_Process_OK_Num) // WIFI �������ã��������ɹ�
      {
         Wifi_Star_Process();
      }
      else
      {
         Wifi_Txd_Deal();
      }
   }
   // WIFI��������ģʽ����������������ʶ��ˮ��״̬�����󣬹ر�WIFI��������ģʽ
   if ((test_factory == ENABLE) && (Wifi_net_Status == e_cloud) && (Sys_Err.Water_Full == DISABLE))
   {
      Rest_Wifi();
   }
}
