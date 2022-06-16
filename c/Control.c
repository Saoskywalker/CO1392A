#include "General.h"

MCU_xdata Pump_str Pump_type; // PUMP变量
MCU_xdata RUN_REG Run_Reg;    // run变量
/* Private function prototypes -----------------------------------------------*/
//=========运行逻辑需要的全局变量
/* Private functions ---------------------------------------------------------*/
MCU_xdata UI16 SYS_RUN_timer = 0;                //系统运行时间
MCU_xdata UI16 SYS_UP_Feel_Disp_timer = 180;     //上电时间
MCU_xdata FuncState test_factory = DISABLE;      //工厂测试
MCU_xdata FuncState Child_Lock_status = DISABLE; //童锁
MCU_xdata UI08 Child_Lock_Disp_Count = 0;        //童锁显示
MCU_xdata UI08 Child_Lock_1s_Count = 0;          //童锁计时
MCU_xdata UI16 Child_Lock_Disp_timer = 0;        //童锁显示时间

MCU_xdata UI08 Key_ERR_Buzz_Cnt = 0; //蜂鸣器

MCU_xdata FuncState Pump_Status = DISABLE;           //自动水泵抽水功能是否打开
MCU_xdata FuncState Pump_water_pipe_Status = ENABLE; //抽水水管是否介入

MCU_xdata UI08 Feel_Mode = 1;       //湿度感知变量
MCU_xdata UI08 Dump_SW_H_count = 0; //机台倾倒开关状态累计变量
MCU_xdata UI08 Dump_SW_L_count = 0; //机台倾倒开关状态累计变量
MCU_xdata UI08 Dump_SW = 0;         //机台倾倒开关状态
// MCU_xdata UI08 Dump_Status=0;//机台倾倒情况
MCU_xdata FuncState Fan_set_enable = ENABLE; //风机设定使能
MCU_xdata UI16 Fan_Speed_delay = 0;          //风机延时
MCU_xdata UI08 filter_Minute_Count = 0;      //滤网计时

//
MCU_xdata UI08 hum_err_updata_delay_time = 180;  //湿度传感器故障上报延时
MCU_xdata UI08 temp_err_updata_delay_time = 180; //温度传感器故障上报延时

MCU_xdata UI16 fan_force_runtime = 180; //风扇强制运行一段时间

MCU_const UI08 HUM_Feel_Code[][2] = {
    {0, 40},   // 35
    {39, 70},  // 55
    {69, 100}, // 85
};

#ifdef REFRIGERANT_LEAKAGE_2016
/*************************************************
 // 函数名称    : Fan_Run_Check
 // 功能描述    : 风机
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Fan_Run_Check(void)
{
    if (SYS_Power_Status == ON)
    {
        // M_Fan_Speed=(M_Fan_Speed==FANOFF)?(fan_set):(M_Fan_Speed);
        Fan_Speed_Out_Buf = HIGH_FAN; //该机台风速不可设定  跑高速
        Comp_para.BUF = OFF;
    }
}

/*************************************************
 // 函数名称    : Ec_1s_Control
 // 功能描述    : S时间片
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
MCU_xdata bool_f sys_reg__ec_err = FALSE;   // EC故障
static MCU_xdata UI16 g_ec_count_timer = 0; // EC计时变量
static MCU_xdata UI08 g_ec_count = 0;       // EC计次变量
void Ec_1s_Control(void)
{
    if (0 == g_ec_count_timer)
    {
        return;
    }
    if (_EC_Fast_Test)
    {
        g_ec_count_timer = (g_ec_count_timer > 60) ? (g_ec_count_timer - 60) : (0);
    }
    else
    {
        g_ec_count_timer--;
    }

    if (g_ec_count_timer == 0)
    {
        g_ec_count++;
    }

    if (g_ec_count >= 3)
    {
        sys_reg__ec_err = TRUE;
    }
}
/*************************************************
 // 函数名称    : Ec_Protect_Check
 // 功能描述    : EC保护
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Ec_Protect_Check(void)
{
    static MCU_xdata bool_f sys_reg__ec_60 = FALSE; // EC保护60分钟
    UI08 g_dt = 0;                                  //

    if (sys_reg__ec_err)
    {
        Fan_Run_Check();
        return;
    }
    //
    if (Temp_room_para_C > Temp_coil_para_C)
    {
        g_dt = Temp_room_para_C - Temp_coil_para_C;
    }
    else
    {
        g_dt = Temp_coil_para_C - Temp_room_para_C;
    }
    //
    if ((g_dt > 5) || (Comp_para.BUF == OFF))
    {
        g_ec_count = 0;
        g_ec_count_timer = 0;
        sys_reg__ec_60 = FALSE;
        return;
    }
    //
    if (Comp_para.on_time > 1200) // 1200
    {
        if (g_dt <= 5)
        {
            if (g_ec_count_timer == 0)
            {
                g_ec_count_timer = (20 + 60); // 连续五分钟检测
                g_ec_count_timer = g_ec_count_timer * 60;
            }
        }
    }
    //
    if ((g_ec_count_timer <= 3600) && (g_ec_count_timer > 0))
    {
        sys_reg__ec_60 = TRUE;
        Fan_Run_Check();
    }
    else
    {
        sys_reg__ec_60 = FALSE;
    }
}
/*************************************************
 // 函数名称    : Ec_Protect_Deal
 // 功能描述    : EC保护
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Ec_Protect_Deal(void)
{
    Ec_Protect_Check();
}

/*************************************************
 // 函数名称    : Comp_Protect_1S_Control_Condition_A
 // 功能描述    : S时间片
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
MCU_xdata bool_f sys_reg__comp_protect_stause_condition_a = FALSE; //冷媒泄漏条件2中条件a压缩机保护, 需关机时清零
MCU_xdata UI16 sys_reg_comp_38_count_condition_a = 0;              //冷媒泄漏条件2中条件a时间计数, 需关机时清零
void Comp_Protect_1S_Control_Condition_A(void)
{
    UI08 l_comp_protect_dou = 0;
    if (sys_reg_comp_38_count_condition_a == 0xffff)
    {
        return;
    }
    l_comp_protect_dou = (_EC_Fast_Test) ? (60) : (1);

    sys_reg_comp_38_count_condition_a += l_comp_protect_dou;

    if (sys_reg__comp_protect_stause_condition_a)
    {
        if (sys_reg_comp_38_count_condition_a > 90 * 60) // 40分钟  改为90
        {
            sys_reg__comp_protect_stause_condition_a = FALSE;
            sys_reg_comp_38_count_condition_a = 0;
        }
        return;
    }

    if ((Temp_room_para_C >= (27 + 15)) && (Comp_para.OUT)) // 30度改为27度
    {
        if (sys_reg_comp_38_count_condition_a >= 360 * 60) // 180分钟改为360分钟
        {
            if ((Temp_room_para_C > Temp_coil_para_C) && ((Temp_room_para_C - Temp_coil_para_C) >= 25))
            {
                sys_reg__comp_protect_stause_condition_a = TRUE;
                sys_reg_comp_38_count_condition_a = 0;
            }
            else
            {
                sys_reg_comp_38_count_condition_a = 0xffff;
            }
        }
    }
    else
    {
        sys_reg_comp_38_count_condition_a = (sys_reg_comp_38_count_condition_a > l_comp_protect_dou) ? (0xffff) : (0);
    }
}
/*************************************************
 // 函数名称    : Comp_Protect_Condition_A
 // 功能描述    :
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Comp_Protect_Condition_A(void)
{
    if (sys_reg__comp_protect_stause_condition_a == FALSE)
    {
        return;
    }
    Fan_Run_Check();
}

/*************************************************
 // 函数名称    : Comp_Protect_1S_Control_Condition_B
 // 功能描述    : S时间片
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
MCU_xdata bool_f sys_reg__comp_protect_stause_condition_b = FALSE; //冷媒泄漏条件2中条件b压缩机保护, 需关机时清零
MCU_xdata UI16 sys_reg_comp_38_count_condition_b = 0;              //冷媒泄漏条件2中条件b时间计数, 需关机时清零
void Comp_Protect_1S_Control_Condition_B(void)
{
    UI08 l_comp_protect_dou = 0;
    if (sys_reg_comp_38_count_condition_b == 0xffff)
    {
        return;
    }
    l_comp_protect_dou = (_EC_Fast_Test) ? (60) : (1);

    sys_reg_comp_38_count_condition_b += l_comp_protect_dou;

    if (sys_reg__comp_protect_stause_condition_b)
    {
        if (sys_reg_comp_38_count_condition_b > 90 * 60)
        {
            sys_reg__comp_protect_stause_condition_b = FALSE;
            sys_reg_comp_38_count_condition_b = 0;
        }
        return;
    }

    if ((Temp_room_para_C >= (38 + 15)) && (Comp_para.OUT))
    {
        if (sys_reg_comp_38_count_condition_b >= 360 * 60)
        {
            sys_reg__comp_protect_stause_condition_b = TRUE;
            sys_reg_comp_38_count_condition_b = 0;
        }
    }
    else
    {
        sys_reg_comp_38_count_condition_b = (sys_reg_comp_38_count_condition_b > l_comp_protect_dou) ? (0xffff) : (0);
    }
}
/*************************************************
 // 函数名称    : Comp_Protect_Condition_B
 // 功能描述    : S时间片
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Comp_Protect_Condition_B(void)
{
    if (sys_reg__comp_protect_stause_condition_b == FALSE)
    {
        return;
    }
    Fan_Run_Check();
}
#endif

// *****************************************************************************
// 函数名称 : EC_Protect_Load_Logic
// 功能说明 : 冷媒保护，负载控制逻辑
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
static void EC_Protect_Load_Logic(void)
{
    if (SYS_Power_Status == OFF)
    {
        return;
    }

    // if (G_SYS_Mode == mode_SYS_HUM)
    // {
    //     fan_speed = G_SYS_Fan_Tyde;
    // }
    // else if (G_SYS_Mode == mode_DRY)
    // {
    //     fan_speed = HIGH;
    // }

    if (Fan_Speed_Out_Buf == OFF_FAN)
    {
        Fan_Speed_Out_Buf = HIGH_FAN; //该机台风速不可设定  跑高速
    }
    Comp_para.BUF = OFF;
}

// *****************************************************************************
// 函数名称 : Ec_Protect1_Logic
// 功能说明 : 冷媒保护条件1逻辑
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
//防冷媒泄漏变量
UI08 S_EC1_Err = FALSE;              // EC故障
static UI16 S_EC1_Count_Time = 0;   // EC计时变量
static UI16 S_EC1_COMP_ON_Time = 0; //

static void Ec_Protect1_Logic(void)
{
    static UI08 S_EC1_Step = 0;        // EC步骤变量
    static UI08 S_EC1_Cycle_Count = 0; // EC计次变量
    UI08 g_dt = 0;                     //计算温差
    UI08 temp_room_C;
    UI08 temp_coil_C;

    if (S_EC1_Err) //多次防冷媒循环后, 不恢复, 负载操作
    {
        Fan_Speed_Out_Buf = OFF_FAN;
        Comp_para.BUF = OFF;
        return;
    }

    if (((Comp_para.OUT == OFF) && (S_EC1_Step != 2) && (Comp_para.BUF == OFF)) || //正常运转，非EC停压缩机
        (SYS_Power_Status == OFF) || (M_Defrost_status) || (Sys_Err.Water_Full))    //关机 除霜 水满
    {
        S_EC1_Cycle_Count = 0;
        S_EC1_Count_Time = 0;
        S_EC1_COMP_ON_Time = 0;
        S_EC1_Step = 0;
        return;
    }

    if (Comp_para.OUT == OFF) //压缩机未启动
    {
        S_EC1_COMP_ON_Time = 0;
    }

    temp_room_C = Temp_room.C_value;
    temp_coil_C = Temp_coil.C_value;
    if (temp_room_C > temp_coil_C)
    {
        g_dt = temp_room_C - temp_coil_C;
    }
    else
    {
        g_dt = 0;
    }

    switch (S_EC1_Step)
    {
    case 0:
    {
        if (S_EC1_COMP_ON_Time >= 60UL * 5) //压缩机启动未达5分钟
        {
            S_EC1_Step = 1;
            S_EC1_Count_Time = 0;
        }
    }
    break;
    case 1:
    {
        //
        // if (g_dt > 5)  //室温管温大于5度        小能力系统
        if (g_dt > 8) //室温管温大于8度        大能力系统
        {
            S_EC1_Cycle_Count = 0;
            S_EC1_Count_Time = 0;
        }

        if (S_EC1_Count_Time > 60UL * 1) //连续1分钟 室温 - 管温 ≤ 8摄氏度
        {
            S_EC1_Count_Time = 0;
            S_EC1_Step = 2;
        }
    }
    break;
    case 2:
    {
        EC_Protect_Load_Logic(); //负载操作
        if (S_EC1_Count_Time > 60UL * 6)
        {
            S_EC1_Cycle_Count++;
            if (S_EC1_Cycle_Count >= 3)
            {
                S_EC1_Count_Time = 0;
                S_EC1_Err = TRUE;
                S_EC1_Step = 3;
            }
            else
            {
                S_EC1_Count_Time = 0;
                S_EC1_COMP_ON_Time = 0;
                S_EC1_Step = 0;
            }
        }
    }
    break;
    case 3:
    {
    }
    break;
    default:
        break;
    }
}

// Ec_Protect1_Logic模块计时
static void Ec_Protect1_timer(void)
{
    UI08 num;

    if (_Fast_Test)
    {
        num = 60;
    }
    else
    {
        num = 1;
    }

    if (S_EC1_Count_Time + num < 0xffff)
    {
        S_EC1_Count_Time += num;
    }

    if (S_EC1_COMP_ON_Time + num < 0xffff)
    {
        S_EC1_COMP_ON_Time += num;
    }
}

// *****************************************************************************
// 函数名称 : Ec_Protect2_Logic
// 功能说明 : 冷媒保护条件2逻辑
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
//防冷媒泄漏变量
UI08 S_EC2_Err = FALSE;              // EC故障
static UI16 S_EC2_Count_Time = 0;   // EC计时变量
static UI16 S_EC2_Comp_ON_Time = 0; //

static void Ec_Protect2_Logic(void)
{
    static UI08 S_EC2_Step = 0; // EC步骤变量
    UI08 g_dt = 0;              //计算温差
    UI08 temp_room_C;
    UI08 temp_coil_C;

    if (S_EC2_Err) //负载操作
    {
        Fan_Speed_Out_Buf = OFF_FAN;
        Comp_para.BUF = OFF;
        return;
    }

    if ((Comp_para.OUT == OFF) || (M_Defrost_status) || (SYS_Power_Status == OFF))
    {
        S_EC2_Comp_ON_Time = 0;
        S_EC2_Count_Time = 0;
        S_EC2_Step = 0;
        return;
    }

    //
    temp_room_C = Temp_room.C_value;
    temp_coil_C = Temp_coil.C_value;
    if (temp_room_C > temp_coil_C)
    {
        g_dt = temp_room_C - temp_coil_C;
    }
    else
    {
        g_dt = 0;
    }

    if (S_EC2_Comp_ON_Time >= 60UL * 120)
    {
        S_EC2_Step = 0;
        S_EC2_Count_Time = 0;
        // S_EC2_Comp_ON_Time = 0;
        return;
    }

    switch (S_EC2_Step)
    {
    case 0:
    {
        // if (S_EC2_Comp_ON_Time >= 60UL * 5) //压缩机启动未达5分钟
        if (S_EC2_Comp_ON_Time >= 60UL * 15) //压缩机启动未达5分钟
        {
            S_EC2_Count_Time = 0;
            S_EC2_Step = 1;
        }
    }
    break;
    case 1:
    {
        if (g_dt <= 23)
        {
            S_EC2_Count_Time = 0;
        }

        if (S_EC2_Count_Time >= 60UL * 1)
        {
            S_EC2_Step = 2;
            S_EC2_Count_Time = 0;
        }
    }
    break;
    case 2:
    {
        if (temp_room_C > 20 + 15)
        {
            S_EC2_Err = TRUE;
            S_EC2_Step = 3;
        }

        if (S_EC2_Count_Time >= 60UL * 1)
        {
            S_EC2_Step = 1;
            S_EC2_Count_Time = 0;
        }
    }
    break;
    case 3:
    {
        ;
    }
    break;
    }
}

// Ec_Protect2_Logic模块计时
static void Ec_Protect2_timer(void)
{
    UI08 num;

    if (_Fast_Test)
    {
        num = 60;
    }
    else
    {
        num = 1;
    }

    if (S_EC2_Count_Time + num < 0xffff)
    {
        S_EC2_Count_Time += num;
    }

    if (S_EC2_Comp_ON_Time + num < 0xffff)
    {
        S_EC2_Comp_ON_Time += num;
    }
}

// *****************************************************************************
// 函数名称 : High_Temperature_Protection1
// 功能说明 : 高温保护逻辑1
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
static void High_Temperature_Protection1(void)
{
    static unsigned char s_high_T_P1_step = 0;
    UI08 temp_room_C;

    if (SYS_Power_Status == OFF)
    {
        s_high_T_P1_step = 0;
        return;
    }

    temp_room_C = Temp_room.C_value;

    switch (s_high_T_P1_step)
    {
    case 0:
    {
        if (temp_room_C > 32 + 15)
        {
            s_high_T_P1_step = 1;
        }
    }
    break;
    case 1:
    {
        //负载操作
        if (Comp_para.OUT == ON)
        {
            Fan_Speed_Out_Buf = HIGH_FAN;
        }

        if (temp_room_C <= 31 + 15) //退出高温保护
        {
            s_high_T_P1_step = 0;
        }
    }
    break;
    default:
        break;
    }
}

// *****************************************************************************
// 函数名称 : High_Temperature_Protection2
// 功能说明 : 高温保护逻辑2
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
unsigned char comp_high2_error_flag = 0; //高温保护错误标志
static void High_Temperature_Protection2(void)
{
    static unsigned char s_high_T_P2_step = 0;
    UI08 temp_room_C;

    if (SYS_Power_Status == OFF)
    {
        s_high_T_P2_step = 0;
        return;
    }

    temp_room_C = Temp_room.C_value;

    switch (s_high_T_P2_step)
    {
    case 0:
    {
        comp_high2_error_flag = 0;
        if (temp_room_C > 42 + 15)
        {
            s_high_T_P2_step = 1;
        }
    }
    break;
    case 1:
    {
        //负载操作
        Comp_para.BUF = OFF; //注意:如果压缩机有强制运转3分钟时，也要强制关闭
        Fan_Speed_Out_Buf = OFF_FAN;
        comp_high2_error_flag = 1;

        if (temp_room_C <= 41 + 15)
        {
            fan_force_runtime = 180;
            s_high_T_P2_step = 0;
        }
    }
    break;
    default:
        break;
    }
}

// *****************************************************************************
// 函数名称 : Comp_OverLoad
// 功能说明 : 压缩机过载保护
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
UI08 Comp_SA_EN = DISABLE; // SA保护标志
UI08 G_Comp_Overtime_Protect_Flag = ENABLE; //压缩机连续运行停机功能标记
static void Comp_OverLoad(void)
{
    if ((SYS_Power_Status == OFF) || (Comp_SA_EN == ENABLE))
        G_Work_Para.on_time = 0;

    if (G_Comp_Overtime_Protect_Flag)
    {
        if (G_Work_Para.on_time >= 3600UL * 12) //连续运行12小时停止
        {
            G_Work_Para.on_time = 0;
            G_Work_Para.off_time = 0;
            Comp_SA_EN = ENABLE;
        }

        if (G_Work_Para.off_time >= 3600)
        {
            Comp_SA_EN = DISABLE;
        }

        if (Comp_SA_EN == ENABLE)
        {
            Comp_para.BUF = OFF;
        }
    }
    else
    {
        Comp_SA_EN = DISABLE;
        G_Work_Para.on_time = 0;
        G_Work_Para.off_time = 0;
    }
}

/*************************************************
 // 函数名称    : Run_reg_init
 // 功能描述    : 变量初始化
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Run_reg_init(void)
{
    UI08 i = 0;
    Pump_type.on_timer = 0;
    Pump_type.off_timer = 0;
    Pump_type.on_count = 0;
    Pump_type.mode = 0;
    Pump_type.SW_ON_Timer = 0;
    Pump_type.SW_OFF_Timer = 0;
    Pump_type.off_delay = 0;

    Run_Reg.HI_stay_time = 0;
    Run_Reg.LO_stay_time = 0;
    Run_Reg.def_start_time = 0;
    Run_Reg.def_start_count_enable = 0;
    Run_Reg.def_time = 0;
    Run_Reg.def_cont = 0;
    Run_Reg.temp_room_point = 0;
    Run_Reg.def_type = TYPE_A;
}
/*************************************************
 // 函数名称    : Pump_100ms_general
 // 功能描述    : 100ms时间片
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Pump_100ms_general(void)
{
#if 0
   if(pump_sw_para.Status == DI_SHORT)
   {
       Pump_type.SW_ON_Timer=0;
       if(Pump_type.SW_OFF_Timer<0xff)
       {
          Pump_type.SW_OFF_Timer++;
       }

   }
   else
   {
       Pump_type.SW_OFF_Timer=0;
       if(Pump_type.SW_ON_Timer<0xff)
       {
          Pump_type.SW_ON_Timer++;
       }
   }
#endif
}

/*************************************************
 // 函数名称    : prg_100ms_control
 // 功能描述    : 100ms时间片
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void prg_100ms_control(void)
{
    if (!_100mS_For_SYS)
    {
        return;
    }

    Pump_100ms_general();

    if (Dump_SW)
    {
        if (Dump_SW_L_count < 0xff)
        {
            Dump_SW_L_count++;
        }
        Dump_SW_H_count = 0;
    }
    else
    {
        if (Dump_SW_H_count < 0xff)
        {
            Dump_SW_H_count++;
        }
        Dump_SW_L_count = 0;
    }
}

/*************************************************
 // 函数名称    : prg_s_control
 // 功能描述    : S时间片
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void prg_s_control(void)
{
    UI16 filter_en_timer = 0;
    UI08 num = 0;

    if (!_1S_For_For_SYS)
    {
        return;
    }

    if (_Fast_Test)
        num = 60;
    else
        num = 1;

    if (hum_err_updata_delay_time) //湿度传感器故障上报延时
    {
        hum_err_updata_delay_time--;
    }
    if (temp_err_updata_delay_time) //温度传感器故障上报延时
    {
        temp_err_updata_delay_time--;
    }

    if (Fan_Speed_delay > 0)
    {
        Fan_Speed_delay--;
    }

    if ((SYS_RUN_timer < 0xffff) && (SYS_Power_Status == ON))
    {
        SYS_RUN_timer++;
    }
    else
    {
        SYS_RUN_timer = 0;
    }

    if (SYS_UP_Feel_Disp_timer < 0xffff)
    {
        SYS_UP_Feel_Disp_timer++;
    }

    if ((Run_Reg.def_start_count_enable) && (M_Defrost_status == FALSE))
    {
        Run_Reg.def_start_time++;
    }
    //
    if (M_Defrost_status)
    {
        Run_Reg.def_time++;
    }
    else
    {
        Run_Reg.def_time = 0;
    }

    if ((fan_force_runtime > 0) && (Power_Delay_Time == 0))
    {
        fan_force_runtime--;
    }
    //
#ifdef REFRIGERANT_LEAKAGE_2016
    Ec_1s_Control();
    Comp_Protect_1S_Control_Condition_A();
    Comp_Protect_1S_Control_Condition_B();
#endif

    Ec_Protect1_timer();
    Ec_Protect2_timer();

    if (G_Work_Para.on_time < 0XFF00)
        G_Work_Para.on_time += num;
    if (G_Work_Para.off_time < 0XFF00)
        G_Work_Para.off_time += num;

    if (_Fast_Test == ENABLE)
    {
        filter_Minute_Count = 60;
        filter_en_timer = 250;
    }
    else
    {
        filter_en_timer = 15000;
    }
    //
    //分钟
    if (++filter_Minute_Count < 60)
    {
        return;
    }
    filter_Minute_Count = 0;
    //
    if (Fan_Speed_Out != OFF_FAN)
    {
        if (SYS_filter_time <= 0Xffff)
        {
            SYS_filter_time++;
        }
        //
        if (SYS_filter_time % 60 == 0) //每一小时记录一次滤网时间
        {
            SYS_filter_time_buf = SYS_filter_time;
        }
    }
    //
    if (SYS_filter_time >= filter_en_timer) //滤网清洗时间
    {
        SYS_filter_time = 0;
        SYS_filter_time_buf = SYS_filter_time;
    }
}
/*************************************************
 // 函数名称    : prg_minute_control
 // 功能描述    : min时间片
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void prg_minute_control(void)
{
    UI08 buf = 0;
    if (!_1Minute_For_SYS)
    {
        return;
    }

    /*if((Hum_para.value>=90)&&(Comp_para.OUT))
    {
      if(Run_Reg.HI_stay_time<0xffff)
        Run_Reg.HI_stay_time++;
    }*/

    if (_Fast_Test) //快测判断
    {
        buf = 60;
    }
    else
    {
        buf = 1;
    }
    //湿度传感器短路判断条件
    if ((Hum_para.AD_value >= 975) // AD值判断
        && (Comp_para.OUT))        //压缩机开启
    {
        if (Run_Reg.HI_stay_time < (0xffff - buf))
        {
            Run_Reg.HI_stay_time = Run_Reg.HI_stay_time + buf;
        }
        else
        {
            Run_Reg.HI_stay_time = 0xffff;
        }
    }
    //湿度传感器开路判断条件
    if ((Hum_para.AD_value <= 56)    // AD值判断
        && (SYS_Power_Status == ON)) //判断机台是否开机
    {
        if (Run_Reg.LO_stay_time < (0xffff - buf))
        {
            Run_Reg.LO_stay_time = Run_Reg.LO_stay_time + buf;
        }
        else
        {
            Run_Reg.LO_stay_time = 0xffff;
        }
    }
}

/*************************************************
 // 函数名称    : defost_enb
 // 功能描述    : 保护逻辑
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void defost_enb(void)
{
    UI08 a;
    M_Defrost_status = TRUE;
    Run_Reg.def_start_time = 0;
    Run_Reg.def_start_count_enable = FALSE;
    if (Run_Reg.def_type == TYPE_A)
    {
        a = 5;
    }
    else
    {
        a = 4;
    }
    if (++Run_Reg.def_cont > a)
    {
        Run_Reg.def_cont = 1;
    }
}
/*************************************************
 // 函数名称    : defost_enb
 // 功能描述    : 保护逻辑
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void def_start_jude(bool_f condition1, bool_f condition2)
{
    // condition1 进入30分钟计时条件
    // condition2 30分钟计时中途清零条件
    UI16 x, a;
    if (condition1)
    {
        Run_Reg.def_start_count_enable = TRUE;
    }
    if (condition2)
    {
        Run_Reg.def_start_count_enable = FALSE;
        Run_Reg.def_start_time = 0;
    }
    if (Run_Reg.def_start_time < 2)
    {
        if (Temp_room.C_value > 15) //温度抬升 9   9为0度
        {
            Run_Reg.temp_room_point = Temp_room.C_value - 15;
        }
        else
        {
            Run_Reg.temp_room_point = 0;
        }
    } // 30分钟开始计时时捕捉室温
    //
    if ((Run_Reg.temp_room_point < defrost_para7) || (Sys_Err.temp_room_err) || (Sys_Err.temp_coil_err))
    {
        Run_Reg.def_type = TYPE_B;
        a = 19 * 60;
    }
    else
    {
        Run_Reg.def_type = TYPE_A;
        a = 24 * 60;
    }
    //
    if ((Run_Reg.def_start_time == defrost_para4) || (Run_Reg.def_start_time == a))
    {
        if (!condition1)
        {
            Run_Reg.def_start_count_enable = FALSE;
            Run_Reg.def_start_time = 0;
        }
    }
    //
    // x=defrost_para6;
    x = a + 60; // 30分钟
    if (_Fast_Test)
    {
        x /= 60;
    }
    if (Run_Reg.def_start_time >= x)
    {
        defost_enb();
    }
}
/*************************************************
 // 函数名称    : defrost_logic
 // 功能描述    : 保护逻辑
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void defrost_logic(void)
{
    UI16 def_end_time, x;
    bool_f condition1, condition2;
    if (Sys_Err.Water_Full)
    {
        Run_Reg.def_cont = 0;
    }
    if ((SYS_Power_Status == OFF) || (Sys_Err.Water_Full))
    {
        Run_Reg.def_start_time = 0;
        Run_Reg.def_time = 0;
        M_Defrost_status = FALSE;
        M__Fan_set_enable = 1;
        return;
    }
    //===================
    if (M_Defrost_status == FALSE) //上电后第一次压缩机启动10分钟内不进行除霜判断
    {
        M__Fan_set_enable = 1;
        /*x=defrost_para1;
        if(Sys_Reg._Fast_Test)
           x/=60;
        if(Sys_Reg.Power_first_on==FALSE)*/
        x = 1;

        //除霜开始条件判断
        if (Sys_Err.temp_coil_err)
        {
            condition1 = (bool_f)(Comp_para.OUT); //铜管故障压缩机开启时直接进入除霜行程2013-3-7
                                                  // &&(Temp_room.value<defrost_para12);//Troom<15

            condition2 = (bool_f)(Comp_para.OUT == OFF); // Troom>=16
            def_start_jude(condition1, condition2);
        }
        //无故障除霜
        else
        {
            condition1 = (bool_f)((Comp_para.on_time >= x) && (Temp_coil.AD_value <= defrost_para2)); // coil<0?
            condition2 = (bool_f)((Temp_coil.AD_value >= defrost_para3) || (Comp_para.OUT == OFF));   // coil>=20
            def_start_jude(condition1, condition2);
        }
    }
    else
    {
        //除霜动作
        Comp_para.BUF = OFF;
        Fan_Speed_Out_Buf = HIGH_FAN;
        M__Fan_set_enable = 0;
        //除霜结束条件判断
        if (Run_Reg.def_type == TYPE_A)
        {
            x = 5;
            def_end_time = defrost_para8;
        }
        else
        {
            x = 4;
            def_end_time = defrost_para9;
        }
        if (Run_Reg.def_cont == x)
        {
            def_end_time = defrost_para11;
        }
        if (_Fast_Test)
        {
            def_end_time /= 60;
        }
        if (Run_Reg.def_time >= def_end_time)
        {
            M_Defrost_status = FALSE;
        }
    }
}

/*************************************************
 // 函数名称    : Water_Dump_protect_deal
 // 功能描述    : 满水/机台倾倒/通信故障
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Water_protect_deal(void)
{
    if (water_full_para.cut_time >= 30)
    {
        if (Sys_Err.Water_Full == DISABLE)
        {
            Buzz_Cnt = 5;
        }
        Sys_Err.Water_Full = ENABLE;
        fan_force_runtime = 0;
        Comp_para.BUF = OFF;
        Fan_Speed_Out_Buf = OFF_FAN;

        SYS_RUN_timer = 0;
    }
    else if (water_full_para.short_time >= 10)
    {
        if (Sys_Err.Water_Full == ENABLE)
        {
            fan_force_runtime = 180;
        }
        Sys_Err.Water_Full = DISABLE;
        Buzz_Cnt = 0;
        Fan_Speed_delay = 0;
    }
}
/*************************************************
 // 函数名称    : Sys_err_deal
 // 功能描述    : 故障处理
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Sys_err_deal(void)
{
    if (Sys_Err.temp_room_err || Sys_Err.hum_Sensor_err ||
        Sys_Err.temp_coil_err || Sys_Err.comm_err)
    {
        Fan_Speed_Out_Buf = OFF_FAN;
        Comp_para.BUF = OFF;
    }
}

/*************************************************
 // 函数名称    : protect_logic
 // 功能描述    : 保护逻辑
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void protect_logic(void)
{
    defrost_logic();

#ifdef REFRIGERANT_LEAKAGE_2016
    Ec_Protect_Deal();
    Comp_Protect_Condition_A();
    Comp_Protect_Condition_B();
#endif

    //防冷媒 2022
    Ec_Protect1_Logic();
    Ec_Protect2_Logic();

    if ((S_EC1_Err == FALSE) && (S_EC2_Err == FALSE))
    {
        //高温保护 2022
        High_Temperature_Protection1();
        High_Temperature_Protection2();
    }

    Sys_err_deal();
    Comp_OverLoad();
    Water_protect_deal();
}

/*************************************************
 // 函数名称    : load_set
 // 功能描述    : 负载输出
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void load_set(void)
{
    if (Comp_para.BUF == ON)
    {
        if ((Comp_para.off_time < Comp_Protect_Time) && (Comp_para.OUT == OFF) && (!_Fast_Test))
        {
            Comp_para.OUT = OFF;
        }
        else
        {
            if (Comp_para.OUT == OFF)
            {
                Comp_para.off_time = 0;
            }
            Comp_para.OUT = ON;
        }
    }
    else
    {
        if ((Comp_para.on_time < Comp_Protect_Time) && (Comp_para.OUT == ON) && (SYS_Power_Status) &&
            (water_full_para.Status == DI_SHORT) && (Sys_Err.temp_room_err == DISABLE) && (Sys_Err.hum_Sensor_err == DISABLE) &&
            (Sys_Err.temp_coil_err == DISABLE) && (Sys_Err.comm_err == DISABLE) && (comp_high2_error_flag == 0)) // 20140807
        {
            Comp_para.OUT = ON;
        }
        else
        {
            if (Comp_para.OUT == ON)
            {
                Comp_para.on_time = 0;
            }
            Comp_para.OUT = OFF;
        }
    }

    Fan_Speed_Out = Fan_Speed_Out_Buf;
    if (Fan_Speed_Out == OFF_FAN) //风机关闭时，快速清零风机运行时间
    {
        FAN_ON_timer = 0;
    }

    UVC_para.OUT = UVC_para.BUF;
    if (Fan_Speed_Out != OFF_FAN)
    {
        Swing_Tyde_Out = Swing_Tyde_Out_Buf;
    }
    else
    {
        Swing_Tyde_Out = STOP_SWING;
    }
}

/*************************************************
 // 函数名称    : HUM_Mode
 // 功能描述    : 除湿模式 ；除濕模式下送風馬達以低速運轉
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void HUM_Mode(void)
{
    if (SYS_HUN_Tyde == CONTINUOUS_HUM) //连续运转
    {
        Comp_para.BUF = ON;
    }
    else if (Hum_para.value >= SYS_Hum_Set + 5)
    {
        Comp_para.BUF = ON;
    }
    else if (Hum_para.value + 5 <= SYS_Hum_Set)
    {
        Comp_para.BUF = OFF;
    }

    //风机
    if ((Comp_para.OUT == ON)       //压缩机运转时
        || (fan_force_runtime > 0)) //每次按电源键开机、定时到开机，或者水满报警解除之后风机强制按设定风速运转3分钟
    {
        Fan_Speed_Out_Buf = LOW_FAN;
    }
    else
    {
        Fan_Speed_Out_Buf = SILENCE_FAN;
    }

    Set_Swing_State = Enable;
    Swing_Tyde_Out_Buf = SYS_Swing_Tyde;
    Set_Fan_State = ENABLE;
}

/*************************************************
 // 函数名称    : DRY_Clothes_Mode
 // 功能描述    : 干衣模式
 // 入口参数    : 无
 // 出口参数    : 无

***************************************************/
void DRY_Clothes_Mode(void)
{
    Comp_para.BUF = ON;
    Set_Fan_State = DISABLE;
    Fan_Speed_Out_Buf = HIGH_FAN;
}
/*************************************************
 // 函数名称    : void UVC_DC_MOTOR_Control(void)
 // 功能描述    : UVC 控制
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void UVC_Control(void)
{
    if (_SYS_UVC_Status == ON)
    {
        if (Fan_Speed_Out_Buf != OFF_FAN) //手动开启的时候指示灯保持不变但是输出跟随风机启停
        {
            UVC_para.BUF = ON;
        }
        else
        {
            UVC_para.BUF = OFF;
        }
    }
    else
    {
        UVC_para.BUF = OFF;
    }
}
/*************************************************
 // 函数名称    : Sys_Control
 // 功能描述    :
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Sys_Control(void)
{
    prg_100ms_control();
    prg_s_control();
    prg_minute_control();

    if (M__Self_Test)
    {
        return;
    }

    if (SYS_Power_Status == OFF)
    {
        Fan_Speed_Out_Buf = OFF_FAN;
        Comp_para.BUF = OFF;
        UVC_para.BUF = OFF;
    }
    else if (SYS_Mode == mode_DRY_Clothes)
    {
        DRY_Clothes_Mode();
    }
    else if (SYS_Mode == mode_SYS_HUM)
    {
        HUM_Mode();
    }
    UVC_Control();

    protect_logic();

    load_set();
}
