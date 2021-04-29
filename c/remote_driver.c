/**
  ******************************************************************************
  * @file remote.c

  ******************************************************************************

  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "General.h"			//系统 通用调用文件

xdata UI08 width=0;//计时红外
xdata UI08 rec_time;//判断遥控器接收完成变量
xdata UI32 Rec_IO_buf=0;//存储上一次IO值
xdata UI08 _rec_enb=0;//遥控器使能标志
xdata UI08 _Rec_Ok=0;//遥控器接收完成标志
xdata UI08 rec_data[total_byte]={0};//遥控器接收数据
xdata UI08 rec_bits=0;//遥控器数据bit索引
xdata UI08 rec_bytes=0;//遥控器数据byte索引


/*************************************************
//名称        :	prg_10ms_remote
//功能        :	每10MS判断一次，红外接收是否完成
//入口参数    :	无
//出口参数    :	无
//构	建:
//修	改: 	GCE XXX
************************************************/
void prg_10ms_remote(void)
{
   if(!_10ms_for_remote)
   { return; }
   _10ms_for_remote=0;

   if(rec_time != 0)//30ms内无正确数据遥控码进来
   {
	  if(--rec_time==0)
	  {
	  	_Rec_Ok=1;
	  }
   }
}

/*************************************************
//名称        :	Remote_Req
//功能        :	红外时序解析程序
//入口参数    :	无
//出口参数    :	无
//构	建:
//修	改: 	GCE XXX
************************************************/
void  Remote_Req(void)//125us扫描
{
    if((width > HEAD-15)&&(width <= HEAD+15))//头码识别 HEAD: 11.125~14.875 ms
    {
      	_rec_enb=1;
      	rec_bits=0X01;
      	rec_bytes=0;
        rec_data[rec_bytes]=0;
      	return;
    }

    if(!_rec_enb)
    {
    	_rec_enb=0;
        return;
    }

    if(rec_bytes>=total_byte)//防止接收数组溢出 total_byte:7
    {
    	_rec_enb=0;
        return;
    }
    rec_time=3;
    if((width>=ONE_1-4)&&(width<=ONE_1+4))//ONE_1: 1.75~2.75 ms
    {
        rec_data[rec_bytes]|=rec_bits;
    }
    else if((width>=ZERO_0-3)&&(width<=ZERO_0+3))//ZERO_0: 0.75~1.5ms
    {
        //rec_data[rec_bytes]&=~rec_bits;//已提前清零
    }
    else
    {
        _rec_enb=0;
    }

    rec_bits=rec_bits<<1;
    if(rec_bits==0)
    {
    	rec_bytes++;
        rec_bits=0x01;
        if(rec_bytes<total_byte)
        {
            rec_data[rec_bytes]=0;
        }
    }

}


/*************************************************
//名称        :	remote_IRQ
//功能        :	0.0125MS定时中断调用
//入口参数    :	无
//出口参数    :	无
//构	建:
//修	改: 	GCE XXX
************************************************/
void remote_IRQ(void)
{
    width++;
    if(REC_IO_VALUE==0)//下降沿
    {
	    if(Rec_IO_buf)
        {
            Remote_Req();
            width=0;
	    }
	    Rec_IO_buf=0;
    }
    else
    {
        Rec_IO_buf=1;
    }
}
/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
