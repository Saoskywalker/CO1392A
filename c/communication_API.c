#include "General.h"


//======================全局变量=================================
xdata UI08   Buz_list;		//BUZZ模式
xdata UI08   communication_timer;	//通信故障时间
xdata UI08   Txd_count;		//发送次数
xdata UI08   Txd_timer;		//发送间隔时间
xdata UUI08  COM_bit0;		//标志位
/***********************************************************
*函 数 名：  void  communication_data_init(void)
*功能描述：  通讯初始化程序
*输入参数：  无
*函数返回值：无
***********************************************************/
void   communication_data_init(void)
{
    Buz_list=0;
    COM_bit0.byte=0;
    communication_timer=180;
    Txd_count=0;
    Txd_timer=0;
}


/***********************************************************
*函 数 名：  void  sleep_deal(void)
*功能描述：  睡眠处理函数
*输入参数：  无
*函数返回值：无
***********************************************************/
void  sleep_deal(void)
{
    if((Power_Status)||(_sys_err )||(Dsp_Time>0)||(_Self_Test))
    {
        sleep_Time=10;
        _Sleep_status=0;
        _Sleep_status_buf=0;
    }
    else
    {
        communication_timer=180;
    }
}


/***********************************************************
*函 数 名：  void  Buz_mode(UI08  mode_buf)
*功能描述：  BUZZ模式
*输入参数：  mode_buf：蜂鸣器模式
*函数返回值：无
***********************************************************/
void  Buz_mode(UI08  mode_buf)
{
    UI08 buf=0;
    buf=(Buz_list&0xf0)>>4;
    buf++;
    Buz_list=(mode_buf&0x0f)+(buf<<4);
}


/***********************************************************
*函 数 名：  UI08 crc_check(UI08 *buffer ,UI08 length)
*功能描述：  8位CRC检验
*输入参数：  *buffer:校验数据
             length:数据长度
*函数返回值: 检验值
***********************************************************/
UI08 crc_check(UI08 *buffer,UI08 length)
{
    UI08 crc = 0;
    UI08 loop=0;
    UI08 loop_1=0;
    for(loop = 0; loop < length; loop++)
    {
        crc ^= buffer[loop];
        for(loop_1 = 0; loop_1 < 8; loop_1++)
        {
            if((crc & 1) == 1)
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


/***********************************************************
*函 数 名：  void rxd_data_protocl(void)
*功能描述：  发送数据转载
*输入参数：  无
*函数返回值: 无
***********************************************************/
void rxd_data_protocl(void)
{
//   UUI16  buf;
    UUI08  UUI08_buf;

    if(_Self_Test)
    {
        communication_timer=2;
    }
    else
    {
        communication_timer=180;
    }
    Temp_room.AD_value=rxd_buf[1]&0x03;
    Temp_room.AD_value=Temp_room.AD_value<<8;
    Temp_room.AD_value=Temp_room.AD_value+rxd_buf[2];
    Temp_room.AD_value=Temp_room.AD_value>>1;

    Temp_coil.AD_value=(rxd_buf[1]&0x0c)>>2;
    Temp_coil.AD_value=Temp_coil.AD_value<<8;
    Temp_coil.AD_value=Temp_coil.AD_value+rxd_buf[3];
    Temp_coil.AD_value=Temp_coil.AD_value>>1;

    Tcomp_coil.AD_value=(rxd_buf[1]&0x30)>>4;
    Tcomp_coil.AD_value=Tcomp_coil.AD_value<<8;
    Tcomp_coil.AD_value=Tcomp_coil.AD_value+rxd_buf[4];
    Tcomp_coil.AD_value=Tcomp_coil.AD_value>>1;

    if(rxd_buf[5]&bit7)
    {
        _Water_Full_status=1;
    }
    else
    {
        _Water_Full_status=0;
    }

    UUI08_buf.byte=rxd_buf[5];
    _Sleep_status_buf=UUI08_buf.bit_.b6;
    _pump_sw_status =UUI08_buf.bit_.b5;
    _INFAN_ERR=UUI08_buf.bit_.b4;
    _OUTFAN_ERR=UUI08_buf.bit_.b3;

    _INFAN_ERR=0;
    _OUTFAN_ERR=0;

}


/***********************************************************
*函 数 名：  void txd_data_protocol(void)
*功能描述：  接收数据
*输入参数：  无
*函数返回值: 无
***********************************************************/
void txd_data_protocol(void)
{
    UUI08  UUI08_buf;

    if(!_txd_ms100_tick)
    {
        return;
    }
    _txd_ms100_tick=0;

    Txd_timer++;
    if(Txd_timer<2)
    {
        return;
    }
    Txd_timer=0;

    if( (_Sleep_status_buf)&&(_Sleep_status) )
    {
        return;
    }


    txd_buf[0]=0xaa;

    Txd_count++;
    if(Txd_count>=5)//报警响10次，1s/次
    {
        Txd_count=0;
        if(Buzz_Cnt<10)
        {
            Buzz_Cnt++;
            Buz_mode(BUZZ_TYPE8);
        }
    }
    txd_buf[1]=Buz_list;

    if((Power_Status)||(_Self_Test==ENABLE))
    {
        UUI08_buf.byte=0;
        if(Lfan_para.OUT==ON)
        {
            UUI08_buf.bit_.b0=1;
        }
        else if(Mfan_para.OUT==ON)
        {
            UUI08_buf.bit_.b1=1;
        }
        else if(Hfan_para.OUT==ON)
        {
            UUI08_buf.bit_.b2=1;
        }

        if(_Self_Test)
        {
            UUI08_buf.bit_.b4=1;
        }

        if(Comp_para.OUT==ON)
        {
            UUI08_buf.bit_.b5=1;
        }

        if(Pump_para.OUT==ON)
        {
            UUI08_buf.bit_.b6=1;
        }

        if(Way4_para.OUT==ON)
        {
            UUI08_buf.bit_.b7=1;
        }

        txd_buf[2]=UUI08_buf.byte;

        txd_buf[3]=0;


        UUI08_buf.byte=0;

        if(OutLfan_para.OUT==ON)
        {
            UUI08_buf.bit_.b0=1;
        }
        else if(OutHfan_para.OUT==ON)
        {
            UUI08_buf.bit_.b2=1;
        }

        UUI08_buf.bit_.b7=1;

        txd_buf[4]=UUI08_buf.byte;
        txd_buf[5]=0;
        txd_buf[6]=0;


    }
    else
    {
        txd_buf[2]=0;
        txd_buf[3]=0;
        txd_buf[4]=0;
        txd_buf[5]=0;
        txd_buf[6]=0;
    }

    if(_Sleep_status)
    {
        txd_buf[4]|=bit6;//给电源板发睡眠状态
    }

    txd_buf[txd_length-1]=crc_check(txd_buf,(txd_length-1));

    _txd_data=0;
    _txd_end=0;
    pulse_cnt=0;
    _txd_start=1;
    _txd_en=1;


}


/***********************************************************
*函 数 名：  void communication_txd(void)
*功能描述：  通信发送
*输入参数：  无
*函数返回值: 无
***********************************************************/
void communication_txd(void)
{
    txd_data_protocol();
}


/***********************************************************
*函 数 名：  void communication_rxd(void)
*功能描述：  通信接收
*输入参数：  无
*函数返回值: 无
***********************************************************/
void communication_rxd(void)
{
    UI08 sum=0;
    if(!_Rxd_OK)
    {
        return;
    }
    _Rxd_OK=0;
    if(rxd_buf[0]!=0xaa)//头码
    {
        return;
    }
    sum=crc_check(rxd_buf,rece_data_lenth-1);//校验
    rxd_buf[0]=0;
    if(rxd_buf[rece_data_lenth-1]!=sum)
    {
        return;
    }
    rxd_data_protocl();//处理数据
}


/***********************************************************
*函 数 名：  void communication_Deal(void)
*功能描述：  通信处理
*输入参数：  无
*函数返回值: 无
***********************************************************/
void communication_Deal(void)
{
    sleep_deal();
    communication_rxd();
    communication_txd();
}


