#include "General.h"

//=================================变量==========================================
xdata  UI08 DcPump_count=0;
xdata  UI32 DcPump_buf=0;

xdata UI08             water_delaytime=0;             //满水故障时，让Buzz鸣长音
xdata UI08             DI_read_time=0;
xdata SHORTCUT_STATUS  _DcPump_Full_status_buf=DI_SHORT;

/////////////////////////////////////////////////////////////
//IO输出

xdata DO_PARA  Comp_para,
      Way4_para,
      Pump_para,
      Hfan_para,
      Mfan_para,
      Lfan_para,
      OutHfan_para,
      OutLfan_para,
      Power_para,//负载开机延时运行，摆叶例外
      DCPUMP_para;

xdata DO_PARA *DO_struct_addr[DO_total]=
{
    &Comp_para,
    &Way4_para,
    &Pump_para,
    &Hfan_para,
    &Mfan_para,
    &Lfan_para,
    &OutHfan_para,
    &OutLfan_para,
    &Power_para,//负载开机延时运行，摆叶例外
    &DCPUMP_para
};

/* 各IO口定义*/
/*
SW1->P37  |  LED1->SEGG->P30  |  LED10->P23  |  LED3-1->P43  |  DISP_RXD->P02  |  SWINGA->P05   |   UV_LED->P04     |   DEBUG_D_TXD->P13            
SW2->P36  |  LED2->SEGB->P10  |  LED11->P24  |  LED3-2->P42  |  DISP_TXD->P01  |  SWINGB->P06   |   UV_CHECK->P03   |   DEBUG_C_RXD->P11
SW3->P35  |  LED4->SEGC->P12  |  LED12->P25  |               |  WIFI_RXD->P20  |  SWINGC->P52
SW4->P34  |  LED5->SEGD->P14  |  COM4->P41   |               |  WIFI_TXD->P21  |  SWINGD->P53
SW5->P33  |  LED6->SEGA->P15
SW6->P00  |  LED7->SEGH->P17
SW7->P27  |  LED8->SEGF->P22
SW8->P26  |  LED9->SEGE->P16
COM1->P31 |  COM3->P4.0
COM2->P32 |                           
*/
void IO_init(void)
{
    //TK上的IO设置为强推挽输出1
    P0CON = 0xFF;  //0为输入 1为输出
    P0PH  = 0x00;  //0不使能上拉电阻 1是能上拉电阻
    //
    P1CON = 0xFF;
    P1PH  = 0x00;
    //
    P2CON = 0xFF;
    P2PH  = 0x00;
    //
    P3CON = 0xFF;
    P3PH  = 0x00;
    //
    P4CON = 0xEB;
    P4PH  = 0x00;
    //
    P5CON = 0xFF;
    P5PH  = 0x00;

    P0 = 0xFF;
    P1 = 0xFF;
    P2 = 0xFF;
    P3 = 0xFF;
    P4 = 0xF7;
    P5 = 0xFF;

    DCPUMP_OFF;
    //IOHCON0= 0xFF;
    //IOHCON1= 0xFF;
}
/*************************************************
 // 函数名称    : void prg_ms100_IO(void)
 // 功能描述    : 100毫秒时间片
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void IO_data_Init(void)
{
    DO_PARA *ptr;
    UI08 i;
    for(i=0; i<DO_total; i++)
    {
        ptr=DO_struct_addr[i];
        (*ptr).BUF=OFF;
        (*ptr).OUT=OFF;
        (*ptr).off_time=0;
        (*ptr).on_time=0;
    }
    //
    Comp_para.off_time=180;
}

/*************************************************
 // 函数名称    : void prg_ms100_IO(void)
 // 功能描述    : 100毫秒时间片
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void prg_ms100_IO(void)
{
    if(!_ms100_for_IO)
    {
        return;
    }
    _ms100_for_IO=0;


    if(_DcPump_Full_status_buf==DI_SHORT)
    {
        _water_pipe_Status=1;
    }
    else
    {
        _water_pipe_Status=0;
    }

}
/*************************************************
 // 函数名称    : void DI_read(void)
 // 功能描述    : 读IO口状态
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void DI_read(void)
{
    UI32 b;

    if(DI_read_time<DI_read_interval)
    {
        return;
    }

    DI_read_time=0;

    if(Tcomp_coil.AD_value<150)//断开
    {
        b=1;
    }
    else if(Tcomp_coil.AD_value>350)//闭合
    {
        b=0;
    }

    if(b!=DcPump_buf)
    {
        DcPump_buf=b;
        DcPump_count=0;
    }
    else
    {
        DcPump_count++;
        if(DcPump_count>=3)
        {
            _first_read_IO=1;//读到一次稳定的IO口
            DcPump_count=0;
            if(DcPump_buf)
            {
                _DcPump_Full_status_buf=DI_CUT;
            }
            else
            {
                _DcPump_Full_status_buf=DI_SHORT;
            }
        }
    }
}
/*************************************************
 // 函数名称    : void prg_s_IO(void)
 // 功能描述    : S计时
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void prg_s_IO(void)
{
    UI08 i;
    DO_PARA *ptr;
    UI08 num=0;

    if(!_s_for_IO)
    {
        return;
    }
    _s_for_IO=0;

    num=(_EC_Fast_Test)?(60):(1);
    for(i=0; i<DO_total; i++)
    {
        ptr=DO_struct_addr[i];
        if((*ptr).OUT ==1)
        {
            (*ptr).off_time=0;
            if((*ptr).on_time<0xffff-num)
            {
                (*ptr).on_time+=num;
            }
        }
        else
        {
            (*ptr).on_time=0;
            if((*ptr).off_time<0xffff-num)
            {
                (*ptr).off_time+=num;
            }
        }
    }
}

/*************************************************
 // 函数名称    : void IO_operate(void)
 // 功能描述    :
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void IO_operate(void)
{
    prg_ms100_IO();
    prg_s_IO();
    DI_read();
}






/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
