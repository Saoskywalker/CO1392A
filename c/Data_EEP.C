#include "General.h"

MCU_xdata SYS_CONFIG Sys_Config;

MCU_xdata union_EEP_Data Sys_Memory_Data;     // EEP校验数组
MCU_xdata union_EEP_Data Sys_Memory_Data_buf; // EEP校验数组

MCU_xdata UI16 EEP_OffSet_DATA_ADDR = 0; //写EEP偏移的地址(0x00~511,一页为512)

MCU_xdata FuncState _check_EEP_EN = DISABLE;

MCU_xdata UI08 Write_Memory_Data_Delay = 0;
MCU_xdata UI32 Write_Memory_Data_Flag = 0;
/*********************************************************
函数名: void Sys_data_write(void)
描  述: EEP 写
输入值: 无
输出值: 无
返回值: 无
**********************************************************/
void Sys_data_write(void)
{
    UI08 i;
    UI08 crc = 0;
    UI32 buf_flag = 0;
    if (Write_Memory_Data_Delay > 0)
    {
        Write_Memory_Data_Delay--;
    }
    Sys_Memory_Data_buf.S_data.E_Head = 0XAA;
    Sys_Memory_Data_buf.S_data.E_SYS_filter_time = SYS_filter_time_buf;
    Sys_Memory_Data_buf.S_data.E_SYS_Power_Status = (UI08)(SYS_Power_Status);
    Sys_Memory_Data_buf.S_data.E_SYS_Mode = (UI08)(SYS_Mode_Buf);
    Sys_Memory_Data_buf.S_data.E_SYS_DYR_Tyde = (UI08)SYS_DYR_Tyde_Buf;
    Sys_Memory_Data_buf.S_data.E_SYS_HUN_Tyde = (UI08)SYS_HUN_Tyde_Buf;
    Sys_Memory_Data_buf.S_data.E_SYS_Hum_Set = (UI08)SYS_Hum_Set_Buf;
    Sys_Memory_Data_buf.S_data.E_SYS_Fan_Tyde = (UI08)SYS_Fan_Tyde_Buf;
    Sys_Memory_Data_buf.S_data.E_SYS_Swing_Tyde = (UI08)SYS_Swing_Tyde_Buf;
    Sys_Memory_Data_buf.S_data.E_SYS_Air_Clear_Tyde = (UI08)SYS_Air_Clear_Tyde_Buf;
    Sys_Memory_Data_buf.S_data.E_LED_Disp_Status = (UI08)LED_Disp_Status;
    Sys_Memory_Data_buf.S_data.E_Buzzer_Status = (UI08)Buzzer_Status;
    Sys_Memory_Data_buf.S_data.E_SYS_DYR_Tyde = (UI08)SYS_DYR_Tyde_Buf;

    Sys_Memory_Data_buf.S_data.E_buf1 = 0x00;
    Sys_Memory_Data_buf.S_data.E_buf2 = 0x00;
    Sys_Memory_Data_buf.S_data.E_buf3 = 0x00;
    Sys_Memory_Data_buf.S_data.E_buf4 = 0x00;
    Sys_Memory_Data_buf.S_data.E_buf5 = 0x00;

    for (i = 0; i < EEP_MAX; i++)
    {
        if (Sys_Memory_Data.EEP_data[i] != Sys_Memory_Data_buf.EEP_data[i])
        {
            buf_flag |= 0x00000001 << i;
        }
    }

    if (Write_Memory_Data_Flag != buf_flag)
    {
        Write_Memory_Data_Flag = buf_flag;
        Write_Memory_Data_Delay = 2; // 2S内
    }
    if ((Write_Memory_Data_Delay > 0) || (Write_Memory_Data_Flag == 0))
    {
        return;
    }
    /* for (i=0;i<EEP_MAX;i++)
     {
         if (Sys_Memory_Data.EEP_data[i] != Sys_Memory_Data_buf.EEP_data[i])
         {
             same_flag = 1;//与上次数据不同
             break;
         }
     }

     if (same_flag == 0)//与上一次相同
     {
         return;
     }    */

    Sys_Memory_Data_buf.S_data.E_CRC = crc_check(&Sys_Memory_Data_buf.EEP_data, EEP_MAX - 1); // CRC校验

    if (EEP_OffSet_DATA_ADDR >= (SECTION_SIZE - EEP_MAX)) // 31*16=496byte(一页512byte)
    {
        IAPPageErase(SYS_DATA_ADDR, IapROM);
        EEP_OffSet_DATA_ADDR = 0;
    }

    //写入
    for (i = 0; i < EEP_MAX; i++)
    {
        IAPWrite(SYS_DATA_ADDR + i + EEP_OffSet_DATA_ADDR, Sys_Memory_Data_buf.EEP_data[i], IapROM);
        // Sys_Memory_Data.EEP_data[i] = Sys_Memory_Data_buf.EEP_data[i];
    }

    EEP_OffSet_DATA_ADDR = EEP_OffSet_DATA_ADDR + EEP_MAX; //偏移到下一组

    _check_EEP_EN = ENABLE;
}

/*********************************************************
函数名: void Sys_data_check(void)
描  述: EEP check
输入值: 无
输出值: 无
返回值: 无
**********************************************************/
void Sys_data_check(void)
{
    UI16 i;
    UI08 crc = 0;
    UI08 check_EEP_data = 0; //检查是否存储有EEP数据
    UI16 check_offset = 0;   // EEP存储的偏移量
    if (_check_EEP_EN == DISABLE)
    {
        return;
    }
    _check_EEP_EN = DISABLE;

    for (i = 0; i < (SECTION_SIZE - EEP_MAX); i = i + EEP_MAX)
    {
        check_EEP_data = IAPRead(SYS_DATA_ADDR + i, IapROM);
        if (check_EEP_data != 0xAA) //检查第一个字节
        {
            if (i == 0) //没有存过数据
            {
                check_offset = 0;
                break;
            }
            else
            {
                check_offset = i - EEP_MAX;
                break;
            }
        }
        else
        {
            check_offset = i;
        }
    }

    //重新读出来校验
    for (i = 0; i < EEP_MAX; i++)
    {
        Sys_Memory_Data.EEP_data[i] = IAPRead(SYS_DATA_ADDR + check_offset + i, IapROM);
    }

    crc = crc_check(&Sys_Memory_Data.EEP_data[0], EEP_MAX - 1); // CRC校验
    //校验   || 写的范围已满  擦除从新开始
    if ((0xaa != Sys_Memory_Data.S_data.E_Head) || (crc != Sys_Memory_Data.S_data.E_CRC))
    {
        for (i = 0; i < EEP_MAX; i++)
        {
            Sys_Memory_Data.EEP_data[i] = 0;
        }
        IAPPageErase(SYS_DATA_ADDR, IapROM);
        EEP_OffSet_DATA_ADDR = 0;
    }
    else
    {
        EEP_OffSet_DATA_ADDR = check_offset + EEP_MAX;
    }
}
/*********************************************************
函数名: void Sys_data_check(void)
描  述: EEP check
输入值: 无
输出值: 无
返回值: 无
**********************************************************/
void Sys_first_data_check(void)
{
    UI16 i;
    UI08 crc = 0;
    UI08 check_EEP_data = 0; //检查是否存储有EEP数据
    UI16 check_offset = 0;   // EEP存储的偏移量

    for (i = 0; i < (SECTION_SIZE - EEP_MAX); i = i + EEP_MAX)
    {
        check_EEP_data = IAPRead(SYS_DATA_ADDR + i, IapROM);
        if (check_EEP_data != 0xAA) //检查第一个字节 头都是0XAA+ EEP_MAX-1
        {
            if (i == 0) //没有存过数据
            {
                check_offset = 0;
                break;
            }
            else
            {
                check_offset = i - EEP_MAX;
                break;
            }
        }
        else
        {
            check_offset = i;
        }
    }

    //重新读出来校验
    for (i = 0; i < EEP_MAX; i++)
    {
        Sys_Memory_Data.EEP_data[i] = IAPRead(SYS_DATA_ADDR + check_offset + i, IapROM);
    }

    crc = crc_check(&Sys_Memory_Data.EEP_data[0], EEP_MAX - 1); // CRC校验
    //校验   || 写的范围已满  擦除从新开始
    if ((0xaa != Sys_Memory_Data.S_data.E_Head) || (crc != Sys_Memory_Data.S_data.E_CRC))
    {
        for (i = 0; i < EEP_MAX; i++)
        {
            Sys_Memory_Data.EEP_data[i] = 0;
        }
        IAPPageErase(SYS_DATA_ADDR, IapROM);
        EEP_OffSet_DATA_ADDR = 0;
    }
    else
    {
        EEP_OffSet_DATA_ADDR = check_offset + EEP_MAX;
    }
}

void Sys_data_read_eep(void)
{
    Sys_first_data_check();
    if (Sys_Memory_Data.S_data.E_Head != 0XAA)
    {
        return;
    }

    SYS_Power_Status = (ONOFF_STATUS)Sys_Memory_Data.S_data.E_SYS_Power_Status;
    if (SYS_Power_Status == ON)
    {
        Comp_para.off_time = 0;
    }

    SYS_filter_time = Sys_Memory_Data.S_data.E_SYS_filter_time;
    SYS_filter_time_buf = SYS_filter_time;

    SYS_Mode = (SYS_MODE)Sys_Memory_Data.S_data.E_SYS_Mode; //系统模式
    SYS_Mode_Buf = SYS_Mode;

    SYS_DYR_Tyde = (DYR_TYPE)Sys_Memory_Data.S_data.E_SYS_DYR_Tyde; //干衣类型
    SYS_DYR_Tyde_Buf = SYS_DYR_Tyde;

    SYS_HUN_Tyde = (HUM_TYPE)Sys_Memory_Data.S_data.E_SYS_HUN_Tyde; //除湿类型
    SYS_HUN_Tyde_Buf = SYS_HUN_Tyde;

    SYS_Hum_Set = (UI08)Sys_Memory_Data.S_data.E_SYS_Hum_Set; //湿度设定
    SYS_Hum_Set_Buf = SYS_Hum_Set;

    SYS_Fan_Tyde = (FANSPEED_TYPE)Sys_Memory_Data.S_data.E_SYS_Fan_Tyde; //运行风速
    SYS_Fan_Tyde_Buf = SYS_Fan_Tyde;

    SYS_Swing_Tyde = (SWING_MODE)Sys_Memory_Data.S_data.E_SYS_Swing_Tyde; //摆叶类型
    SYS_Swing_Tyde_Buf = SYS_Swing_Tyde;

    SYS_Air_Clear_Tyde = (AIR_CLEAR_MODE)Sys_Memory_Data.S_data.E_SYS_Air_Clear_Tyde; //空气净化类型
    SYS_Air_Clear_Tyde_Buf = SYS_Air_Clear_Tyde;

    LED_Disp_Status = (FuncState)Sys_Memory_Data.S_data.E_LED_Disp_Status; //灯光是否打开

    Buzzer_Status = (FuncState)Sys_Memory_Data.S_data.E_Buzzer_Status; //蜂鸣器是否使能

    // Child_Lock_status=(FuncState)Sys_Memory_Data.S_data.E_Child_Lock_status;//童锁
}

/*************************************************
 // 函数名称    : EEP_deal
 // 功能描述    : EEP处理
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void EEP_deal(void)
{
    if (!_500mS_For_SYS)
    {
        return;
    }

    Sys_data_check();
    Sys_data_write();
}
