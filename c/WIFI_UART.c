#include "General.h"

MCU_const UI08 manufacturer[] = "manufacturer nwt";                             //制造商
MCU_const UI08 serial_number[] = "serial_number 105821277";                     //序列号
MCU_const UI08 MIIO_model_req[] = "MIIO_model_req";                             //模块信息
MCU_const UI08 model_nwt[] = "model nwt.derh.330ef";                            //模块信息
MCU_const UI08 MIIO_mcu_version_req[] = "MIIO_mcu_version_req";                 //操作板软件版本
MCU_const UI08 mcu_version[] = "mcu_version 0016";                              //操作板软件版本
MCU_const UI08 ble_config[] = "ble_config set 1720 0001";                       // ble链接配置
MCU_const UI08 net[] = "net";                                                   //询问网络连接状态
MCU_const UI08 reboot[] = "reboot";                                             //重启WIFI
MCU_const UI08 Restore_WIFI[] = "restore";                                      //清除WIFI配置
MCU_const UI08 factory[] = "factory";                                           //设置工厂模式
MCU_const UI08 get_down[] = "get_down";                                         //查询服务器有没有下发数据
MCU_const UI08 result_ok[] = "result \"ok\"";                                   //返回给服务器接收数据正常
MCU_const UI08 result[] = "result";                                             //返回给服务器接收数据正常
MCU_const UI08 update_fw[] = "update_fw";                                       //有软件更新
MCU_const UI08 error_invalid_operation[] = "error \"invalid operation\" -5000"; //无法识别的指令

MCU_const UI08 set_properties[] = "set_properties";         //设置属性 设置机台参数 动作命令
MCU_const UI08 get_properties[] = "get_properties";         //读取属性 读取机台参数 动作命令
MCU_const UI08 properties_changed[] = "properties_changed"; //属性上报  机台参数上报 动作命令
MCU_const UI08 down[] = "down";                             //下发down
MCU_const UI08 none[] = "none";                             //无数据
MCU_const UI08 MIIO_net_change[] = "MIIO_net_change";       //识别网络状态改变
////
MCU_const UI08 NO_properties[] = "-4003"; //属性、方法、事件不存在

MCU_const UI08 event_code[] = "event_occured"; //事件
MCU_const UI08 com_ok[] = "ok";                // OK
MCU_const UI08 uap[] = "uap";                  //等待快连中
MCU_const UI08 cloud[] = "cloud";              //成功连接到路由器
MCU_const UI08 local[] = "local";              //成功连接到路由器
MCU_const UI08 offline[] = "offline";          //掉线重连中
MCU_const UI08 updating[] = "updating";        //升级中
MCU_const UI08 unprov[] = "unprov";            //配网功能关闭
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

MCU_const UI08 Self_Test[] = "Self_Test";   //自检信息
MCU_xdata UI08 Self_Test_wifi_err_time = 0; //自检err延时
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

MCU_xdata UUI32 Txd_props_bit[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // SIID 变量
//
MCU_xdata Wifi_UART_str SYS_W_Uart;   // wifi变量
MCU_xdata Wifi_props_str SYS_W_props; // wifi变量

MCU_xdata UI08 Star_Process_Num = 1;                //初始化WIFI模块参数步骤变量
MCU_xdata FuncState Sec_Txd_Star_Process = ENABLE;  //配网使能
MCU_xdata UI08 Wifi_Tick = 0;                       //时间-发送使能
MCU_xdata net_Status_e Wifi_net_Status = e_uap;     //网络状态
MCU_xdata net_Status_e Wifi_net_Status_buf = e_uap; //网络状态
MCU_xdata UI08 Wifi_net_delay = 0;                  // net 延时
MCU_xdata FuncState Wifi_txd_en = DISABLE;          //发送使能
MCU_xdata FuncState Wifi_model_load = ENABLE;       // wifi模块装载-检测上电前没插wifi模块时 wifi指示灯不闪

MCU_xdata UUI32 Txd_props_ok_bit[9]; //当前发送的服务器通信指令

MCU_xdata UUI32 Txd_event_bit = {0};     //等待发送的事件
MCU_xdata UUI32 Txd_event_bit_buf = {0}; //等待发送的事件 buf
MCU_xdata UUI32 Txd_event_ok_bit = {0};  //当前发送的事件

MCU_xdata UUI32 Txd_COM_bit = {0};    //公共指令
MCU_xdata UUI32 Txd_COM_ok_bit = {0}; //当前发送的公共指令
MCU_xdata UUI32 Set_props_err = {0};  //等待发送的错误
MCU_xdata UI16 addr_point = 0;        //发送地址

//高有先级的指令
MCU_xdata UUI16 wifi_com_bit = {0};               //标志位
MCU_xdata UUI16 wifi_com_ok_bit = {0};            //标志位
MCU_xdata UUI16 T_result_bit = {0};               //标志位
MCU_xdata UUI16 R_result_bit = {0};               //标志位
MCU_xdata UUI16 Get_Props_bit = {0};              // wifi读属性
MCU_xdata FANSPEED_TYPE Wifi_Fan_Speed = OFF_FAN; //风速变化时  上报app(做检测风速变化用)
MCU_xdata FANSPEED_TYPE Wifi_Set_Fan = OFF_FAN;   // wifi数据填充  不做使用
MCU_xdata UI08 get_down_cont = 0;                 // get_down 发送的次数

void Wifi_COM_Deal(void);
void Txd_data(UI08 *point, UI16 lenth);
void Txd_data_props(UI08 *point, UI16 addr, UI16 lenth);
void get_properties_deal(void);
void Txd_props_Deal(void);
void Rest_Wifi(void);
/*************************************************
 // 函数名称    : Wifi_UART_Init
 // 功能描述    : 串口/数据及数据的初始化
 // 入口参数    : 无
 // 出口参数    : 无
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
      Txd_props_ok_bit[i].lword = 0; //当前发送的服务器通信指令
   }
}
/*************************************************
 // 函数名称    : prg_Wifi_UART_ms
 // 功能描述    : ms时间片
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void prg_Wifi_UART_ms(void)
{
   /*if(!_Wifi_Uart_ms)
   {return;}
   _Wifi_Uart_ms=0;*/
   //
   if (SYS_W_Uart.Rxd_Timerout > 0) //接收数据完成后N秒开始处理数据
   {
      SYS_W_Uart.Rxd_Timerout--;
      if (SYS_W_Uart.Rxd_Timerout == 0)
      {
         SYS_W_Uart.Rxd_OK = ENABLE;
      }
   }
}
/*************************************************
 // 函数名称    : prg_Wifi_UART_ms10
 // 功能描述    : 10ms时间片
 // 入口参数    : 无
 // 出口参数    : 无
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
      _txd_tick = 1; //与电源板的通信
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
 // 函数名称    : prg_S_Wifi_UART
 // 功能描述    : s时间片
 // 入口参数    : 无
 // 出口参数    : 无
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
 // 函数名称    : Wifi_UART_Txd_Data
 // 功能描述    : 串口数据的发送
 // 入口参数    : 无
 // 出口参数    : 无
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
 // 函数名称    : Clear_Data
 // 功能描述    : 数据清零
 // 入口参数    : 无
 // 出口参数    : 无
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
函数名称 : my_strcmp
功能描述 : 比较字符串大小
输入参数 : s1:字符串1
           s2:字符串2
返回参数 : 大小比较值，0:s1=s2; -1:s1<s2; 1:s1>s2
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
 // 函数名称    : net_deal
 // 功能描述    : 网络处理
 // 入口参数    : 无
 // 出口参数    : 无
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
 // 函数名称    : chack_down_deal
 // 功能描述    : 在 type_buf 状态下  app不可操作机台
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
UI08 chack_down_deal(UI08 type_buf)
{
   UI08 key_type = 0;

   // if(Child_Lock_status==ENABLE)//童锁
   //{key_type|=key_Child_Lock_bit;}

   if ((SYS_Power_Status == OFF) && (M_Timer_Run == 0)) //关机非定时
   {
      key_type |= key_power_bit;
   }

   // if(sys_mode_buf==mode_DRY_Clothes)
   //{key_type|=key_dry_cloth_bit; }

   if (Sys_Err.Water_Full == ENABLE) //水满或者机台倾倒
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
      //改报 -4003
      //_wifi_com_props_error=1;
      return 1;
   }
   //
   if (type_buf)
   {
      Key_ERR_Buzz_Cnt = 3;
      //改报 -4003
      //_wifi_com_props_error=1;
      return 1;
   }
   return 0;
}
/*************************************************
 // 函数名称    : set_properties_deal
 // 功能描述    : wifi下发指令处理
 // 入口参数    : 无
 // 出口参数    : 无
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
   //如 down set_properties 2 5 37
   //
   SIID_data = SYS_W_Uart.Rxd_buf[buf]; //读取SIID号
   //
   type_buf = SYS_W_Uart.Rxd_buf[buf + 2]; //如上5的位置
   buf = buf + 4;                          //如上 37的位置
   //
   if (SIID_data == '1') // SIID为1
   {
      ;
   }
   else if (SIID_data == '2') // SIID为2
   {
      switch (type_buf)
      {
      case SET_POWER: //开关机
      {
         _wifi_T_result_SET_POWER = 1;
         //
         if (my_strcmp(Code_false, &SYS_W_Uart.Rxd_buf[buf]))
         {
            if (chack_down_deal(key_Water_Full_bit)) //
            {
               SET_POWER_Err = 1; //属性、方法、事件不存在-关机不可设定
               return;
            }
            Turn_Off();
         }
         else if (my_strcmp(Code_true, &SYS_W_Uart.Rxd_buf[buf]))
         {
            if (chack_down_deal(key_Water_Full_bit)) //
            {
               SET_POWER_Err = 1; //属性、方法、事件不存在-关机不可设定
               return;
            }
            Turn_On();
         }
         else
         {
            SET_POWER_Err = 1;
            return;
         } //错误指令 回复-4003
         Buzz_Time = BUZZ_long_time;
      }
      break;
      case SET_MODE: //模式
      {
         _wifi_T_result_SET_MODE = 1;                  //设置OK
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
         else if (my_strcmp("2", &SYS_W_Uart.Rxd_buf[buf])) //连续除湿
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
         else if (my_strcmp("3", &SYS_W_Uart.Rxd_buf[buf])) //干衣模式
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
         } //发送错误
      }
      break;
      //
      case SET_HUM: //湿度
      {
         //
         _wifi_T_result_SET_HUM = 1;                              //设置OK
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
      // 设置风速   只是配合小米的协议
      // 填充数据   机台不作采用
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
   else if (SIID_data == '3') // SIID为3
   {
      ;
   }
   else if (SIID_data == '4') // SIID为4
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
   else if (SIID_data == '5') // SIID为5
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
         else //设定错误
         {
            SET_LAMP_Err = 1;
            return;
         }
         //
         Buzz_Time = BUZZ_long_time;
         //	     M_write_delay_time=2;
      }
   }
   else if (SIID_data == '6') // SIID为6
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
         } //识别不了的指令

         Child_Lock_1s_Count = 0;
         Buzz_Time = BUZZ_short_time;
         //             M_write_delay_time=2;
      }
   }
   else if (SIID_data == '7') // SIID为7
   {
      ;
   }
   else if (SIID_data == '8') // SIID为8
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
   else if (SIID_data == '9') // SIID为9
   {
      if (type_buf == SET_TIME)
      {
         //
         _wifi_T_result_SET_TIMER = 1;            //设置OK
                                                  //
         if (chack_down_deal(key_Water_Full_bit)) //放在 result后面
         {
            SET_TIME_Err = 1;
            return;
         }
         //下发的为>=10h的定时
         if ((SYS_W_Uart.Rxd_buf[25] <= '9') && (SYS_W_Uart.Rxd_buf[25] >= '0'))
         {
            Set_buf = SYS_W_Uart.Rxd_buf[24] - '0'; //
            Set_buf = Set_buf * 10;
            Set_buf += SYS_W_Uart.Rxd_buf[25] - '0';
         }
         else // 定时<=9
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
 // 函数名称    : Wifi_Rxd_COM_Deal
 // 功能描述    : 网络处理
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Wifi_Rxd_COM_Deal(void)
{
   UI08 buf;
   if (_Txd_COM_ok_net) //连接状态查询
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
      if (my_strcmp(update_fw, &SYS_W_Uart.Rxd_buf[buf])) //查询是否有软件更新
      {
         MCU_RESET; //软件复位，
      }
      else if (my_strcmp(MIIO_net_change, &SYS_W_Uart.Rxd_buf[buf])) //识别网络状态改变
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
      else if (my_strcmp(set_properties, &SYS_W_Uart.Rxd_buf[buf])) //设置机台状态
      {
         buf = buf + sizeof(set_properties);
         set_properties_deal(buf);
      }
      else if (my_strcmp(get_properties, &SYS_W_Uart.Rxd_buf[buf])) //读取机台状态
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
 // 函数名称    : get_properties_err_deal
 // 功能描述    : 填充wifi读取错误的返回数据
 // 入口参数    : 无
 // 出口参数    : 无
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
   else if ((SYS_W_Uart.Rxd_buf[addr_buf + 4] == ' ') || (SYS_W_Uart.Rxd_buf[addr_buf + 4] == 0x0d)) // get_properties 2 1 2 3 2 5 2 4  如: 1位置参数大于10情况
   {
      SYS_W_Uart.Txd_buf[addr_point++] = SYS_W_Uart.Rxd_buf[addr_buf];     // '2'
      SYS_W_Uart.Txd_buf[addr_point++] = SYS_W_Uart.Rxd_buf[addr_buf + 1]; // ' '
      SYS_W_Uart.Txd_buf[addr_point++] = SYS_W_Uart.Rxd_buf[addr_buf + 2]; // 'num/10'
      SYS_W_Uart.Txd_buf[addr_point++] = SYS_W_Uart.Rxd_buf[addr_buf + 3]; // 'num%10'
      SYS_W_Uart.Txd_buf[addr_point++] = ' ';                              // ' '
   }
   Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //发送  result -4003
   addr_point += sizeof(NO_properties) - 1;
}
/*************************************************
 // 函数名称    : void  get_properties_deal(void)
 // 功能描述    : wifi模块 拉(读)属性
 // 入口参数    : 无
 // 出口参数    : 无
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
   } //清除所有发送标志
   //
   addr_buf = sizeof(down);
   addr_buf = addr_buf + sizeof(get_properties); //+1  + 空格的位置

   Txd_data_props(result, 0, sizeof(result) - 1); //发送  result
   addr_point = sizeof(result) - 1;
   // get_prop_bit.word=0;
   //
   for (j = 0; j < (get_prop_num + 16); j++)
   {
      if (addr_buf >= SYS_W_Uart.Rxd_byte) //读取的地址是否超出
      {
         Buzz_Time = 200; //
         return;
      }
      //查询问询参数
      if ((SYS_W_Uart.Rxd_buf[addr_buf + 3] == ' ') || (SYS_W_Uart.Rxd_buf[addr_buf + 3] == 0x0d))
      {
         piid = SYS_W_Uart.Rxd_buf[addr_buf + 2] - '0';
         if ((piid < 1) || (piid > 9))
         {
            return;
         }
      }
      else if ((SYS_W_Uart.Rxd_buf[addr_buf + 4] == ' ') || (SYS_W_Uart.Rxd_buf[addr_buf + 4] == 0x0d)) // get_properties 2 1 2 3 2 5 2 4  如: 1位置参数大于10情况
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
      if ((SYS_W_Uart.Rxd_buf[addr_buf] < '1') && (SYS_W_Uart.Rxd_buf[addr_buf] > '9')) //未在定义的siid
      {
         get_properties_err_deal(addr_buf);
      }
      //
      siid = SYS_W_Uart.Rxd_buf[addr_buf] - '0'; //获取siid

      if ((get_properties_SIID_PIID_EN[siid - 1] & (0x00000001 << (piid - 1))) == 0) //没有这个参数
      {
         get_properties_err_deal(addr_buf);
      }
      else
      {
         Txd_props_bit[siid - 1].lword = 0x00000001 << (piid - 1);
         _wifi_Get_Props_load = 1; // get数据的装载  与上报的数据有点区别  需多加个'0'
         Txd_props_Deal();
         Txd_props_bit[siid - 1].lword = 0;
      }

      if (piid < 10)
      {
         if (SYS_W_Uart.Rxd_buf[addr_buf + 3] == 0x0d) //结束符
         {
            break;
         }
         addr_buf = addr_buf + 4;
      }
      else
      {
         if (SYS_W_Uart.Rxd_buf[addr_buf + 4] == 0x0d) //结束符
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
 // 函数名称    : clear_props_bit
 // 功能描述    : 数据上报时  收到OK  清标志
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
/*
void clear_props_bit(void)
{
  UI08 i;
  for(i=0;i<9;i++)//清事件
  {Txd_props_bit[i].lword&=~Txd_props_ok_bit[i].lword;}
  //清标志
  for(i=0;i<9;i++)
  {Txd_props_ok_bit[i].lword=Txd_props_bit[i].lword;}
}
*/
/*************************************************
 // 函数名称    : load_props_bit
 // 功能描述    : 给标志位置位   收到OK时清除
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void load_props_bit(void)
{
   UI08 i;
   for (i = 0; i < 9; i++) // bit 装载
   {
      Txd_props_ok_bit[i].lword = Txd_props_bit[i].lword;
   }
}
/*************************************************
 // 函数名称    : Wifi_Rxd_Deal
 // 功能描述    : 网络处理
 // 入口参数    : 无
 // 出口参数    : 无
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
   if (Star_Process_Num < Star_Process_OK_Num) //识别初始化接收的数据
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
      //进入自检
      if (my_strcmp(model_nwt, SYS_W_Uart.Rxd_buf))
      {
         if ((!M__Self_Test) && (Power_Delay_Time > 0))
         {
            M__Self_Test = 1;
            M_Power_Delay_Time2 = 0;
            //
            communication_timer = 2;
            //自检写入系统默认参数
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
   else if (_wifi_com_ok_factory) //工厂测试
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
   else if ((Txd_props_ok_bit[0].lword) // 8个SIID的数据更新
            || (Txd_props_ok_bit[1].lword) || (Txd_props_ok_bit[2].lword) || (Txd_props_ok_bit[3].lword) || (Txd_props_ok_bit[4].lword) || (Txd_props_ok_bit[5].lword) || (Txd_props_ok_bit[6].lword) || (Txd_props_ok_bit[7].lword) || (Txd_props_ok_bit[8].lword))
   {
      /*if(my_strcmp(com_ok,SYS_W_Uart.Rxd_buf))//收到ok  清掉上报事件bit
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
   Wifi_model_load = ENABLE; //收到OK-wifi模块已装载
   Clear_Data(SYS_W_Uart.Rxd_buf, SYS_W_Uart.Rxd_byte);
   SYS_W_Uart.Rxd_byte = 0;
}
/*************************************************
 // 函数名称    : Txd_data
 // 功能描述    : 数据发送
 // 入口参数    : 无
 // 出口参数    : 无
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
 // 函数名称    : Txd_data_props
 // 功能描述    : 网络处理
 // 入口参数    : 无
 // 出口参数    : 无
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
 // 函数名称    : sys_humidity
 // 功能描述    : 网络处理
 // 入口参数    : 无
 // 出口参数    : 无
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
 // 函数名称    : sys_temp
 // 功能描述    : 网络处理
 // 入口参数    : 无
 // 出口参数    : 无
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
 // 函数名称    : Txd_event_Deal
 // 功能描述    : 通信故障处理
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Txd_event_Deal(void)
{
   UI08 i = 0;

   Txd_data_props(event_code, 0, sizeof(event_code) - 1); //装载字符串 event_occured
   addr_point = sizeof(event_code) - 1;
   //
   for (i = 0; i < 8; i++) //七个
   {
      if (Txd_event_bit.lword & (0x00000001 << i))
      {
         SYS_W_Uart.Txd_buf[addr_point++] = ' ';
         SYS_W_Uart.Txd_buf[addr_point++] = 7 + '0';
         SYS_W_Uart.Txd_buf[addr_point++] = ' ';
         SYS_W_Uart.Txd_buf[addr_point++] = i + 1 + '0'; // 0的时候 发'1'
         Txd_event_ok_bit.lword = (0x00000001 << i);
         break;
      }
   }
}
/*************************************************
 // 函数名称    : SIID_PIID_Load
 // 功能描述    : 数据的装载
 // 入口参数    : data_buf1 data_buf2
 // 出口参数    : 无
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
 // 函数名称    : get_Props_load_Ascii_0
 // 功能描述    : wifi get数据时 在 PIID后
                  状态一个'0'(0-返回成功)
 // 入口参数    : 无
 // 出口参数    : 无
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
 // 函数名称    : SIID1_Props
 // 功能描述    : 回复/上报 SIID1数据
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void SIID1_Props(void)
{
   if (_props_manufacturer) //制造商
   {
      SIID_PIID_Load('1', '1');
      get_Props_load_Ascii_0();
      Txd_data_props(manufacturer, addr_point, sizeof(manufacturer) - 1);
      addr_point = addr_point + sizeof(manufacturer) - 1; //
   }
   if (_props_model) //模块信息
   {
      SIID_PIID_Load('1', '2');
      get_Props_load_Ascii_0();
      Txd_data_props(model_nwt, addr_point, sizeof(model_nwt) - 1);
      addr_point = addr_point + sizeof(model_nwt) - 1; //
   }
   if (_props_serial_number) //序列号
   {
      SIID_PIID_Load('1', '3');
      get_Props_load_Ascii_0();
      Txd_data_props(serial_number, addr_point, sizeof(serial_number) - 1);
      addr_point = addr_point + sizeof(serial_number) - 1; //
   }
   if (_props_revision) //软件版本
   {
      SIID_PIID_Load('1', '4');
      get_Props_load_Ascii_0();
      Txd_data_props(mcu_version, addr_point, sizeof(mcu_version) - 1);
      addr_point = addr_point + sizeof(mcu_version) - 1; //
   }
}
/*************************************************
 // 函数名称    : SIID2_Props
 // 功能描述    : 回复/上报 SIID2数据
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void SIID2_Props(void)
{
   //设置开关机
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
   //设置模式
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
   //状态
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
      //因为风速是为了填充wifi协议的(风速不可调)
      //切回复的风速要跟设定的一致
      //所以上报的也是填充的风速
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
 // 函数名称    : SIID3_Props
 // 功能描述    : 回复/上报 SIID3数据
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void SIID3_Props(void)
{
   //环境湿度
   if (_props_humidity)
   {
      SIID_PIID_Load('3', '1');
      get_Props_load_Ascii_0();
      sys_humidity(&addr_point);
   }
   //室温
   if (_props_temp)
   {
      SIID_PIID_Load('3', '7');
      get_Props_load_Ascii_0();
      sys_temp(&addr_point, Temp_room.value);
   }
}
/*************************************************
 // 函数名称    : SIID4_Props
 // 功能描述    : 回复/上报 SIID4数据
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void SIID4_Props(void)
{
   //设置提示音
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
 // 函数名称    : SIID5_Props
 // 功能描述    : 回复/上报 SIID5数据
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void SIID5_Props(void)
{
   //设置指示灯
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
 // 函数名称    : SIID6_Props
 // 功能描述    : 回复/上报 SIID6数据
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void SIID6_Props(void)
{
   //设置童锁
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
 // 函数名称    : SIID7_Props
 // 功能描述    : 回复/上报 SIID7数据
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void SIID7_Props(void)
{
   //管温
   if (_props_temp_coil)
   {
      SIID_PIID_Load('7', '1');
      get_Props_load_Ascii_0();
      sys_temp(&addr_point, Temp_coil.value);
   }
   //压缩机状态
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
   //满水
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
 // 函数名称    : SIID8_Props
 // 功能描述    : 回复/上报 SIID8数据
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void SIID8_Props(void)
{
   //设置水泵
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
 // 函数名称    : SIID9_Props
 // 功能描述    : 回复/上报 SIID9数据
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/

void SIID9_Props(void)
{
   UI16 time_run_min = 0;
   UI08 wifi_set_timer = 0;
   //定时
   if (_props_time_run)
   {
      SIID_PIID_Load('9', '1');
      get_Props_load_Ascii_0();
      //上传分钟
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
 // 函数名称    : Txd_props_Deal
 // 功能描述    : 数据有变化时上报
 // 入口参数    : 无
 // 出口参数    : 无
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
 // 函数名称    : Wifi_Txd_result
 // 功能描述    : 网络处理
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Wifi_Txd_result(void)
{
   addr_point = 0;
   Txd_data_props("result", addr_point, sizeof(result) - 1);
   addr_point = sizeof(result) - 1;

   //应答 开/关 机
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
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //发送  result -4003
         addr_point += sizeof(NO_properties) - 1;
      }
   }
   //应答 模式
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
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //发送  result -4003
         addr_point += sizeof(NO_properties) - 1;
      }
   }
   //
   //应答 模式
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
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //发送  result -4003
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
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //发送  result -4003
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
         _props_buzzer = 1; //属性变化  主动上报
         SYS_W_Uart.Txd_buf[addr_point++] = '0';
      }
      else
      {
         SET_BUZZER_Err = 0;
         SYS_W_Uart.Txd_buf[addr_point++] = ' ';                               // ' '
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //发送  result -4003
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
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //发送  result -4003
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
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //发送  result -4003
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
         Txd_data_props(NO_properties, addr_point, sizeof(NO_properties) - 1); //发送  result -4003
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
 // 函数名称    : Wifi_Txd_Deal
 // 功能描述    : 网络处理
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Wifi_Txd_Deal(void)
{

   if (Wifi_txd_en == DISABLE) //每100ms进入一次
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
   if (_wifi_com_factory) //工厂测试
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
   //应答wifi模块读取的命令
   if (Get_Props_bit.word)
   {
      Get_Props_bit.word = 0;
      Txd_data(SYS_W_Uart.Txd_buf, addr_point);
      return;
   }
   //发送事件   1s进入一次  不然两个间隔太短  APP会不推送
   if ((Txd_event_bit.lword) && (_s_event_Updata))
   {
      _s_event_Updata = 0;
      Txd_event_Deal();
      Txd_data(SYS_W_Uart.Txd_buf, addr_point);
      // 20200311  修改上报故障发完即清零(预防wifi模块不回应,连续发出现连续报的问题)
      Txd_event_bit.lword &= ~Txd_event_ok_bit.lword; //清零已经上报的bit故障
      // Txd_event_ok_bit.lword&=~Txd_event_ok_bit.lword;
      return;
   }

   //应答wifi模块下发的命令
   if (T_result_bit.word)
   {
      Wifi_Txd_result();
      return;
   }
   //上报机台变化的参数
   if ((Txd_props_bit[0].lword) // 8个SIID的数据更新
       || (Txd_props_bit[1].lword) || (Txd_props_bit[2].lword) || (Txd_props_bit[3].lword) || (Txd_props_bit[4].lword) || (Txd_props_bit[5].lword) || (Txd_props_bit[6].lword) || (Txd_props_bit[7].lword) || (Txd_props_bit[8].lword))
   {
      addr_point = 0;
      Txd_data_props(properties_changed, addr_point, sizeof(properties_changed) - 1);
      addr_point = sizeof(properties_changed) - 1;
      //
      Txd_props_Deal();
      //启动发送
      Txd_data(SYS_W_Uart.Txd_buf, addr_point);
      //
      // Txd_props_ok_bit.lword=Txd_props_bit.lword;
      load_props_bit();
      //测试
      Txd_props_bit[0].lword = 0; // 8个SIID的数据更新
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
      if (_Txd_COM_get_down) //向wifi模块申请下发数据
      {
         Txd_data(get_down, sizeof(get_down) - 1);
         _Txd_COM_ok_get_down = 1;
      }
      else if (_Txd_COM_net) //询问链接状态
      {
         Txd_data(net, sizeof(net) - 1);
         _Txd_COM_ok_net = 1;
      }

      return;
   }
}

/*************************************************
 // 函数名称    : Wifi_Star_Process
 // 功能描述    : 网络处理
 // 入口参数    : 无
 // 出口参数    : 无
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
   case 0: //清除WIFI参数-wifi复位
   {
      Txd_data(Restore_WIFI, sizeof(Restore_WIFI) - 1);
   }
   break;
   case 1: //发送网络识别号
   {
      Txd_data(model_nwt, sizeof(model_nwt) - 1);
   }
   break;
   case 2: //发送版本
   {
      Txd_data(mcu_version, sizeof(mcu_version) - 1);
   }
   break;
   case 3: //蓝牙配置
   {
      Txd_data(ble_config, sizeof(ble_config) - 1);
   }
   break;
   }
}
/*************************************************
 // 函数名称    : rest_wifi
 // 功能描述    : wifi复位
 // 入口参数    : 无
 // 出口参数    : 无
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
 // 函数名称    : Wifi_COM_Deal
 // 功能描述    : 网络处理
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Wifi_COM_Deal(void)
{
   //网络连接到路由器
   if ((Wifi_net_Status == e_cloud) && (Wifi_net_Status_buf == e_cloud)) //每次联网成功，都把机台状态发送一次
   {
      // wifi_net_led_timer=WIFI_NET_TIMER;
      if (Sec_Txd_Star_Process == ENABLE)
      {
         Sec_Txd_Star_Process = DISABLE;
         Star_Process_Num = 1;
         // Txd_props_bit.lword=0xffffffff;
         //将数据上报
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

   if ((Wifi_net_Status == e_cloud) //询问状态  联网状态下  50次get_down问询一次
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
   else //询问状态  否则  10次get_down问询一次
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
 // 函数名称    : Wifi_Self_Test_Deal
 // 功能描述    : wifi 电路自检
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Wifi_Self_Test_Deal(void)
{
   // 100ms发送一次
   if (_Self_Test_wifi_TXD_en)
   {
      _Self_Test_wifi_TXD_en = 0;
      Txd_data(Self_Test, sizeof(Self_Test) - 1);
   }
   //接收
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
 // 函数名称    : Wifi_UART_Deal
 // 功能描述    : 网络处理
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Wifi_UART_Deal(void)
{
   // prg_Wifi_UART_ms();
   prg_Wifi_UART_ms10();
   prg_S_Wifi_UART();
   //自检  通过自发自收Self_Test
   //判断通信是否正常
   if (M__Self_Test)
   {
      Wifi_Self_Test_Deal();
      return;
   }
   //
   Wifi_Rxd_Deal();
   if ((SYS_W_Uart.Txd_OK == ENABLE) && (SYS_W_Uart.Rxd_Timerout == 0))
   {
      if (Star_Process_Num < Star_Process_OK_Num) // WIFI 参数配置，并联网成功
      {
         Wifi_Star_Process();
      }
      else
      {
         Wifi_Txd_Deal();
      }
   }
   // WIFI工厂测试模式，网络连接正常后，识别到水满状态正常后，关闭WIFI工厂测试模式
   if ((test_factory == ENABLE) && (Wifi_net_Status == e_cloud) && (Sys_Err.Water_Full == DISABLE))
   {
      Rest_Wifi();
   }
}
