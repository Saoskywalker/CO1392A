/********************************************************************************
  * @file timer_API.c
*******************************************************************************
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/

#include "General.h"

xdata UUI08 IIC_buy={0};
xdata UI08 IIC_count=0;


/***********************************************************
*函 数 名：  void i2c_start(void)
*功能描述：  IIC起始
*输入参数：  无
*函数返回值：无
***********************************************************/
void i2c_start(void)
{
   SCL_output;
   SDA_output;
   SDA_high;
   SCL_high;
   delayL_IIC;
   SDA_low;
   delayL_IIC;
   SCL_low;
   delayL_IIC;
}
/***********************************************************
*函 数 名：  void i2c_stop(void)
*功能描述：  IIC结束
*输入参数：  无
*函数返回值：无
***********************************************************/
void i2c_stop(void)
{
   SCL_low;
   delayL_IIC;
   SDA_low;
   delayL_IIC;
   SCL_high;
   delayH_IIC;
   SDA_high;
   delayL_IIC;
}
/***********************************************************
*函 数 名：  void ack(void)
*功能描述：  IIC ack
*输入参数：  无
*函数返回值：无
***********************************************************/
void ack(void)
{
   SCL_low;
   delayL_IIC;
   SDA_low;
   delayL_IIC;
   SCL_high;
   delayL_IIC;
   SCL_low;
   delayL_IIC;
}
/***********************************************************
*函 数 名：  void ack(void)
*功能描述：  IIC noAck
*输入参数：  无
*函数返回值：无
***********************************************************/
void noAck(void)
{
    SCL_low;
    delayL_IIC;
    SDA_high;
    delayL_IIC;
    SCL_high;
    delayH_IIC;
}
/***********************************************************
*函 数 名：  void send(void)
*功能描述：  IIC 发送字节
*输入参数：  无
*函数返回值：无
***********************************************************/
void send(UI08 data1)
{
	UI08 i=0;
	for(i=0;i<8;i++)
	{       SCL_low;
	        delayL_IIC;
		if (data1&0x80)
		{
                  SDA_high;
		}
		else
		{
                   SDA_low;
		}
		//
		delayL_IIC;
		SCL_high;
		delayH_IIC;
		data1<<=1;
		//delayL_IIC;
	}
	SCL_low;
	SDA_input;
}
/***********************************************************
*函 数 名：  void send_DATA(void)
*功能描述：  IIC 发送数据
*输入参数：  无
*函数返回值：无
***********************************************************/
UI08 send_DATA(UI08 data1)
{
    send(data1);
    //SDA_input;
    delayH_IIC;
    SCL_high;
    delayH_IIC;
    if(Sda)
    {
        //SDA_output;
        //i2c_stop();
        //return 1;
    }
    SCL_low;
    SDA_output;
    SDA_high;
    delayH_IIC;
    //SDA_output;
    //SDA_high;
    //delayL_IIC;

    return 0;
}

/***********************************************************
*函 数 名：  void read(void)
*功能描述：  IIC 数据读取
*输入参数：  无
*函数返回值：无
***********************************************************/
UI08 read(void)
{
    UI08  i=0;
    UI08  buff=0;
    SCL_low;
	SDA_input;
	for(i=0;i<8;i++)
	{
		SCL_low;
		delayL_IIC;
		SCL_high;
		delayH_IIC;
		buff<<=1;
        if(Sda)
		{
            buff++;
        }
	}
    SCL_low;
    delayL_IIC;
    SDA_output;
	return buff;
}
/***********************************************************
*函 数 名：  void write_Ndata(void)
*功能描述：  IIC 数据写
*输入参数：  无
*函数返回值：无
***********************************************************/
void write_Ndata(UI08 Addr,UI08 Sub_Addr,UI08 *ptr1,UI08 len)
{
   i2c_start();

   send_DATA(Addr);

   send_DATA(Sub_Addr);

   while(len--)
  {
     send_DATA(*ptr1);
      ptr1++;
   }
   i2c_stop();
}

/***********************************************************
*函 数 名：  void read_Ndata(void)
*功能描述：  IIC 数据读
*输入参数：  无
*函数返回值：无
***********************************************************/
//UI08 read_Ndata(UI08 Addr,UI08 Sub_Addr,UI08 *ptr,UI08 len)
UI08 read_Ndata(UI08 Addr)
{
   UI08 i=0;
   i2c_start();
   send_DATA(Addr+1);

  for(i=0;i<7;i++)
  {
    Screen_rec_data[i]=read();
    ack();
  }
   Screen_rec_data[i]=read();
   noAck();
   i2c_stop();
   return 0;
}

/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
