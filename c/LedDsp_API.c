#include "General.h"

MCU_xdata RGB_LED_TYPE AirQ_Class_LED_buf = 0;

MCU_xdata UI08 LED_data_buf[COM_total] = {0xff, 0xff, 0xff}; // LED��ʾdata

MCU_xdata FlagStatus _Flash_500ms = SET; // 50ms��˸
MCU_xdata UI08 Disp_Delay = 0;           //��ʾdelay
MCU_xdata UI08 Disp_set_Delay = 0;       //����delay

MCU_xdata UI08 RGB_Count = 0;     // RBG count
MCU_xdata UI08 UP_disp_count = 0; //��ʾ count

MCU_xdata UI16 Wifi_Disp_Count = 0;                 // wifi ��ʾcount
MCU_xdata UI16 Wifi_Disp_Length = 0;                //��ʾ����
MCU_xdata UI16 wifi_net_led_timer = WIFI_NET_TIMER; // 30����
MCU_xdata UI08 Type_num = 0;                        // num
MCU_xdata UI08 test_key_data = 0;                   //�Լ�data
MCU_xdata UI08 RGB_Cycle_Disp_cnt = 0;
// wifiָʾ��
MCU_const UI08 Wifi_Disp_Tab[5][2] = {
    {20, 80},
    {10, 0},
    {10, 20},
    {10, 20},
    {0, 10}};
//�̼�����
MCU_const UI08 Wifi_updating_Disp_Tab[2][2] = {
    {5, 10},
    {5, 80}};

/******************************************************************************
 *         �������ʾ���ݶ���
 *******************************************************************************/
MCU_const UI08 BCD_tab[] = /*LED��ʾ���룬���ڲ��*/
    {
        (DATA_0), /*0*/
        (DATA_1), /*1*/
        (DATA_2), /*2*/
        (DATA_3), /*3*/
        (DATA_4), /*4*/
        (DATA_5), /*5*/
        (DATA_6), /*6*/
        (DATA_7), /*7*/
        (DATA_8), /*8*/
        (DATA_9), /*9*/

};
//==============================================================================
//      3) Private function prototypes
//==============================================================================
void temp_set_dsp(void);
void LedDsp_content(void);
void LedDsp_Outdata(void);

/*************************************************
 // ��������    : LED_MS10_Deal
 // ��������    : 10MSʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void LED_mS10_Deal(void)
{
  if (!_10mS_For_SYS)
  {
    return;
  }

  if (Child_Lock_Disp_timer > 0)
  {
    Child_Lock_Disp_timer--;
  }

  Child_Lock_1s_Count++;
  if (Child_Lock_1s_Count >= 100)
  {
    Child_Lock_1s_Count = 0;
    if (Child_Lock_Disp_Count > 0)
    {
      Child_Lock_Disp_Count--;
      Child_Lock_Disp_timer = 50;
    }
  }


}

/*************************************************
 // ��������    : prg_S_DSP
 // ��������    : Sʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_mS500_DSP(void)
{
  if (!_500mS_For_SYS)
  {
    return;
  }

  _Flash_500ms = (!_Flash_500ms);
}
/*************************************************
 // ��������    : prg_S_DSP
 // ��������    : Sʱ��Ƭ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void prg_S_DSP(void)
{
  if (!_1S_For_For_SYS)
  {
    return;
  }
  
  if (RGB_Cycle_Disp_cnt < 0xff)
  {
    RGB_Cycle_Disp_cnt++;
  }

  if (G_Disp_Machine_Temp_Time > 0)
  {
    G_Disp_Machine_Temp_Time--;
  }

//  if (wifi_net_led_timer > 0)
//  {
//    wifi_net_led_timer--;
//    if (wifi_net_led_timer == 0)
//    {
//      if ((Wifi_net_Status_buf != e_cloud) || (Wifi_net_Status_buf != e_updating))
//      {
//        Wifi_net_Status = e_unprov;
//        Wifi_net_Status_buf = e_unprov;
//      }
//    }
//  }
}

/*********************************************************
������: disp_fast_test_temp
��  ��:
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
MCU_xdata UI08 G_Disp_Machine_Temp_Time = 0; //������ʱ����ʾ���¹��µ�ʱ��
void disp_fast_test_temp(void)
{
  UI08 dsp_temp = 0;

  if (G_Disp_Machine_Temp_Time == 9)
  {
    dsp_temp = (UI08)Temp_room.C_value;
    //
    if (dsp_temp > 15)
    {
      dig1_2_dsp(dsp_temp - 15);
    }
    else
    {
      dsp_temp = 15 - dsp_temp;
      if (dsp_temp > 9)
      {
        dsp_temp = 9;
      }
      dig1_2_dsp(dsp_temp);
      dig1_num = DATA_neg; //-
    }
  }
  else if (G_Disp_Machine_Temp_Time == 7)
  {
    dsp_temp = (UI08)Temp_coil.C_value;
    //
    if (dsp_temp > 15)
    {
      dig1_2_dsp(dsp_temp - 15);
    }
    else
    {
      dsp_temp = 15 - dsp_temp;
      if (dsp_temp > 9)
      {
        dsp_temp = 9;
      }
      dig1_2_dsp(dsp_temp);
      dig1_num = DATA_neg; //-
    }
  }
  else if (G_Disp_Machine_Temp_Time == 5)
  {
    Disp_All();
  }
}

/*************************************************
 // ��������    : dig1_2_dsp
 // ��������    : ��ʾ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void dig1_2_dsp(UI08 _disp_data)
{

  dig2_num = BCD_tab[_disp_data % 10];
  dig1_num = BCD_tab[_disp_data / 10];
}

/*************************************************
 // ��������    : Err_dsp
 // ��������    : ������ʾ
 // ��ڲ���    : ��
 // ���ڲ���    : UI08
***************************************************/
UI08 Err_dsp(void)
{
  if (Sys_Err.temp_room_err) //~~~~~~~~~~~~~`
  {
    dig1_num = DATA_E;
    dig2_num = DATA_1;
    return 1;
  }
  else if (Sys_Err.temp_coil_err) //~~~~~~~~~~~~~~
  {
    dig1_num = DATA_E;
    dig2_num = DATA_2;
    return 1;
  }
  else if (Sys_Err.hum_Sensor_err)
  {
    dig1_num = DATA_E;
    dig2_num = DATA_3;
    return 1;
  }
  else if (Sys_Err.comm_err) //~~~~~~~~~~~~~~
  {
    dig1_num = DATA_E;
    dig2_num = DATA_5;
    return 1;
  }
  return 0;
}
/*************************************************
 // ��������    : clear_all
 // ��������    : ���������ʾ
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void clear_all(void)
{
  UI08 i = 0;

  for (i = 0; i < sizeof(LED_data_buf); i++)
  {
    LED_data_buf[i] = 0;
  }

  AirQ_Class_LED_buf = 0;
}

// *****************************************************************************
// �������� : LedDsp_Test
// ����˵�� : LED������ʾ
// ��ڲ��� : ��
// ���ڲ��� : ��
// ��ǰ�汾 : V1.0
// ��д��Ա : Aysi-E
// �����Ա :
// ������� :
// �޸ļ�¼ :   V1.0�״η���
// ��ע     ��
//
// *****************************************************************************
void LedDsp_Test(void)
{
  static UI08 S_Test_cont1 = 0; //�Լ����
  static UI08 test_key_data = 0;
  static UI08 test_seq = 0;
  UI08 key_num = 0;

  if (_250mS_For_SYS)
  {
    if (Power_Delay_Time == 0)
    {
      if (S_Test_cont1 < 0xff)
        S_Test_cont1++;
    }
  }

  key_num = Get_Key_Data();

  if (test_seq == 0) //��ʾ���
  {
    switch (S_Test_cont1)
    {
    case 0:
      clear_all();
      break;
    case 1:
      dig1_num |= BIT_A;
      WIFI_locate;
      AirQ_Class_LED_buf = RED_LED;
      break;
    case 2:
      dig1_num |= BIT_B;
      WATER_locate;
      AirQ_Class_LED_buf = GREEN_LED;
      break;
    case 3:
      dig1_num |= BIT_C;
      UVC_locate;
      AirQ_Class_LED_buf = BLUE_LED;
      break;
    case 4:
      dig1_num |= BIT_D;
      TIMER_locate;
      AirQ_Class_LED_buf = ALL_ON_LED;
      break;
    case 5:
      dig1_num |= BIT_E;
      NC_LED2;
      break;
    case 6:
      dig1_num |= BIT_F;
      DRY_locate;
      break;
    case 7:
      dig1_num |= BIT_G;
      NC_LED1;
      break;
    case 8:
      dig1_num |= BIT_P;
      DRY_Clothes_locate;
      break;
    default:
      S_Test_cont1 = 0;
      break;
    }
  }
  dig2_num = dig1_num;

  if (test_seq == 2) //�����ʾ
  {
    clear_all();

    if (Sys_Err.comm_err)
    {
      dig1_num = DATA_E;
      dig2_num = DATA_5;
    }
    else if (G_Uart_Test_Error)
    {
      dig1_num = DATA_E;
      dig2_num = DATA_6;
    }
    else
    {
      dig1_num = DATA_r;
      dig2_num = BCD_tab[Soft_Version];
    }
  }

  switch (key_num) //�������
  {
  case power_key:
    test_key_data |= bit0;
    break;
  case set_timer_key:
    test_key_data |= bit1;
    break;
  case dry_key:
    test_key_data |= bit2;
    break;
  case Dry_Clothes_key:
    test_key_data |= bit3;
    break;
  default:
    break;
  }

  if (test_key_data == 0X0F) //������
  {
    test_seq = 2; //��ת�������ʾ
  }

  if (key_num)
  {
    Buzz_Time = BUZZ_short_time;
  }
}

/*************************************************
 // ��������    : Disp_All
 // ��������    : ȫ��
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Disp_All(void)
{
  UI08 i = 0;
  /*for(i=0;i<sizeof(LED_data_buf);i++)
  {
        LED_data_buf[i]=0xff;
        Decimal_point_off;//С����
  }*/
  dig1_2_dsp(88);
  WATER_locate;
  // UVC_locate;
  WIFI_locate;
  DRY_locate;
  TIMER_locate;
  DRY_Clothes_locate;
}

/*************************************************
 // ��������    : LedDsp_content
 // ��������    : ��ʾ����
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void LedDsp_content(void)
{
  //���������ʾ����
  clear_all();

  if ((LED_Disp_Status == DISABLE) && (Disp_Delay == 0))
  {
    return;
  }

  if (Power_Delay_Time > 0)
  {
    Disp_All();

    if (RGB_Cycle_Disp_cnt < 1)
    {
      AirQ_Class_LED_buf = ORANGE_LED;
    }
    else if (RGB_Cycle_Disp_cnt < 2)
    {
      AirQ_Class_LED_buf = GREEN_LED;
    }
    else if (RGB_Cycle_Disp_cnt < 3)
    {
      AirQ_Class_LED_buf = BLUE_LED;
    }
    else
    {
      RGB_Cycle_Disp_cnt = 0;
    }
    return;
  }

  //��������ʾ�¶�
  if ((_Fast_Test) && (G_Disp_Machine_Temp_Time > 0))
  {
    disp_fast_test_temp();
    return;
  }
  //��ʾwifi�ź�ǿ��
  if (Wifi_Rssi_DSP_delay_time)
  {
    if (SYS_Power_Status == ON)
      Wifi_Rssi_DSP_delay_time = 0;
    else
      wifi_rssi_Dsp();
    return;
  }
  //����ѹ����ǿ�Ʋ���ȫ��
  if (Comp_Test_Disp_En_Timer > 0)
  {
    SYS_Inspect_Disp();
    return;
  }
#if 0
  if( test_factory==ENABLE)
  {
      if(Wifi_Disp_Count<Wifi_Disp_Tab[(UI08)(Wifi_net_Status)][0])
      {
         WIFI_locate;
      }
  }
#endif

  if (Sys_Err.Water_Full)
  {
    // dig1_num = DATA_F;
    // dig2_num = DATA_U;
    if (_Flash_500ms)
    {
      WATER_locate;
    }
    return;
  }

  if(Err_dsp())
    return;

  if ((LED_Disp_Status == DISABLE) && (Child_Lock_Disp_Count == 0) && (Child_Lock_status == ENABLE))
  {
    return;
  }

  if (Child_Lock_Disp_Count > 0)
  {
    if (Child_Lock_Disp_timer > 0)
    {
      Disp_All();
      Decimal_point_off;
      AirQ_Class_LED_buf = ALL_ON_LED;
    }
    return;
  }

  if (M_Timer_Run > 0)
  {
    TIMER_locate;
  }
  //============��ʱ��ʾ
  if (Set_SYS_Hum_timer > 0) //�趨ʪ����ʾ
  {
    if (_Flash_500ms)
    {
      if (SYS_HUN_Tyde_Buf != CONTINUOUS_HUM)
      {
        dig1_2_dsp(SYS_Hum_Set_Buf);
      }
      else
      {
        dig1_num = DATA_C;
        dig2_num = DATA_o;
      }
    }
  }
  else if (M_Timer_Setting_Time > 0) //��ʱ��ʾ
  {
    if (_Flash_500ms)
    {
      dig1_2_dsp(M_Timer_Buf);
    }
    TIMER_locate;
  }
  else if ((SYS_Power_Status == OFF) && (M_Timer_Run))
  {
    dig1_2_dsp(M_Timer_Buf);
  }
  else if (SYS_Power_Status) //����ʪ����ʾ
  {
    if ((SYS_Mode_Buf == mode_SYS_HUM) || (SYS_Mode_Buf == mode_DRY_Clothes))
    {
      if (Hum_dsp_com >= 90)
      {
        dig1_num = DATA_9; // DATA_H;  20131122
        dig2_num = DATA_0; // DATA_1;
      }
      else if (Hum_dsp_com <= 30)
      {
        dig1_num = DATA_3; // DATA_L;  20131122
        dig2_num = DATA_0; // DATA_o;
      }
      else
      {
        dig1_2_dsp(Hum_dsp_com);
      }
      // dig2_num|=BIT_P;
    }
  }
#if 0
  if(Wifi_net_Status==e_updating)
  {
      if(Wifi_Disp_Count<Wifi_updating_Disp_Tab[Type_num][0])
      {
         WIFI_locate;
      }
  }
  else if((Wifi_Disp_Count<Wifi_Disp_Tab[(UI08)(Wifi_net_Status)][0])&&(Wifi_model_load==ENABLE))//��⵽wifiģ�����лظ�
  {
      WIFI_locate;
  }
#endif

  // wifi״̬��ʾ
  if (WiFi_LED_Locate_buf)
  {
    WIFI_locate;
  }

  if (SYS_Power_Status == OFF)
  {
    return;
  }

  if (_SYS_UVC_Status) // UVC��
  {
    UVC_locate;
  }

  switch (SYS_Mode_Buf)
  {
  case mode_SYS_HUM: //��ʪģʽ
  {
    DRY_locate;
  }
  break;
  case mode_DRY_Clothes: //����ģʽ
  {
    DRY_Clothes_locate;
  }
  break;
  }
  /**********************************************************

   �C�w�_�C�\�Dǰ1���,�h����Bָʾ��Ϩ��
   ��ɫ,�h����ȸ��70%RH.
  .�{ɫ,�h�������40%RH~70%RH
  .�Gɫ,�h����ȵ��40%RH
  *****************************************************/
  if ((SYS_RUN_timer > 59) && (!M_Defrost_status))
  {
    if (Hum_dsp_state < 40)
    {
      AirQ_Class_LED_buf = GREEN_LED;
    }
    else if (Hum_dsp_state < 70)
    {
      AirQ_Class_LED_buf = BLUE_LED;
    }
    else
    {
      AirQ_Class_LED_buf = ORANGE_LED;
    }
  }
  else if (M_Defrost_status)
  {
    if (_Flash_500ms)
    {
      if (Hum_dsp_state < 40)
      {
        AirQ_Class_LED_buf = GREEN_LED;
      }
      else if (Hum_dsp_state < 70)
      {
        AirQ_Class_LED_buf = BLUE_LED;
      }
      else
      {
        AirQ_Class_LED_buf = ORANGE_LED;
      }
    }
  }

  if (_Fast_Test)
  {
    if (_Flash_500ms)
    {
      AirQ_Class_LED_buf = ALL_ON_LED;
    }
  }
}
/*************************************************
 // ��������    : LedDsp_Outdata
 // ��������    :
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void Get_LED_data(void)
{
  UI08 i = 0;
  for (i = 0; i < sizeof(LED_data); i++)
  {
    LED_data[i] = LED_data_buf[i];
  }

  AirQ_Class_LED_out = AirQ_Class_LED_buf; // RGB��  ����ʪ����ʾ
}
/*************************************************
 // ��������    : LED_display
 // ��������    :
 // ��ڲ���    : ��
 // ���ڲ���    : ��
***************************************************/
void LED_display(void)
{
  LED_mS10_Deal();
  prg_mS500_DSP();
  prg_S_DSP();
  if (M__Self_Test)
  {
    LedDsp_Test();
  }
  else
  {
    LedDsp_content();
  }

  Get_LED_data();

  RBG_out();
}
