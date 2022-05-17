#include "General.h"


MCU_xdata UI08   txd_buf[txd_length]={0};      //通信数据 TXD
MCU_xdata UI08   rxd_buf[rece_data_lenth]={0}; //通信数据 RXD
MCU_xdata UUI08  communication_bit1={0};     //通信变量标志位
MCU_xdata UI08   pulse_cnt=0;//数据长度
MCU_xdata UI08   txd_byte=0;//发送字节
MCU_xdata UI08   txd_bit=0;//发送bit
MCU_xdata UI08   Rxd_bits=0;//接收数据长度
MCU_xdata UI08   Rxd_bytes=0;//接收数据长度
MCU_xdata UI08   communication_width_count=0;//接收数据长度

/*************************************************
 // 函数名称    : communication_Txd_IRQ
 // 功能描述    : 通信发送
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void communication_Txd_IRQ(void)
{
      UI08 buf=0;

      pulse_cnt++;

       if(_txd_data) //发数据
  	  {
	  	buf=5;
        if(txd_buf[txd_byte]&txd_bit)
        {
          if(pulse_cnt>=15)
          {
             pulse_cnt=0;
            txd_bit<<=1;
          }
        }
        else
        {
           if(pulse_cnt>=10)
           {
             pulse_cnt=0;
            txd_bit<<=1;
           }
        }


 	 	if(0x00==txd_bit)
 	    {
 	 	 	 txd_bit=0x01;

            if(++txd_byte>= txd_length)
            {
                pulse_cnt=0;
                txd_byte=0;
                _txd_end=1;
                _txd_data=0;
                _txd_start=0;
            }

 	 	}
  	  }
      else if(_txd_start)//发头码
  	  {
	     //T_R_IO_OUT;
	     buf=5+1;
             if(pulse_cnt>=(20+1))
  		   {
              pulse_cnt=0;
              txd_bit=0x01;
              _txd_start=0;
              _txd_data=1;
  		 	  _txd_end=0;
           }
  	  }
      else if(_txd_end)
  	  {
	         buf=5;
             if(pulse_cnt>=10)
              {
                 _txd_start=0;
                 _txd_data=0;
  		          _txd_end=0;
                 _txd_en=0;
		 //T_R_IO_IN;
              }
  	  }
      else
      {
             pulse_cnt=5;
             _txd_start=1;
             _txd_data=0;
  	          _txd_end=0;
             _txd_en=0;
	     //T_R_IO_IN;
      }

      if(pulse_cnt<buf)
      {
          Txd_L;
	  }
      else
      {
           Txd_H;
      }
}

/*************************************************
 // 函数名称    : communication_Rxd_IRQ
 // 功能描述    : 通信接收
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void communication_Rxd_IRQ(void)
{
         if((communication_width_count>=17)&&(communication_width_count<=23))//head=20
         {
             _Rxd_start=1;
             Rxd_bits=0x01;
             Rxd_bytes=0;
             return;
         }

         if(!_Rxd_start)
         {return;}

         if(Rxd_bytes>=rece_data_lenth)
         {
             _Rxd_start=0;
            return;
	     }

         if((communication_width_count>=13)&&(communication_width_count<=17))//one=15
         {
             rxd_buf[Rxd_bytes]|=Rxd_bits;
         }
         else if((communication_width_count>=8)&&(communication_width_count<=12))// zero=10
         {
            rxd_buf[Rxd_bytes]&=~Rxd_bits;
         }
         else
         {
            _Rxd_start=0;
            return;
         }

         Rxd_bits<<=1;
         if(0==Rxd_bits)
         {
             Rxd_bits=0x01;
             Rxd_bytes++;
             if(Rxd_bytes>=rece_data_lenth)
             {
                 _Rxd_OK=1;
                 _Rxd_start=0;
             }
         }
}

//每0.125mS调用一次
void Communication_INT_Deal(void)
{

    if(_txd_en)
    {
        communication_Txd_IRQ();
    }
    else
    {
    	communication_width_count++;
        if(!Rxd_IO)
        {
            if(_Rxd_IO_buf)
            {
                _Rxd_IO_buf=0;
                communication_Rxd_IRQ();
                communication_width_count=0;
            }
        }
        else
        {
            _Rxd_IO_buf=1;
        }
    }

}


