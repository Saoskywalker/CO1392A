#include "General.h"			//系统 通用调用文件

static  rec_T  rec_buf={COOL,LOW,0,_C,0,OFF,0,0};//data buf
static  UI08   old_timer=0;//old  time

xdata UUI08          rec_bit={0};//标志位
//xdata ONOFF_STATUS   rec_buf_power2=OFF;//开关机备份
xdata UI08           Self_Test_DATA=0;//机型

/*************************************************
//名称        :	remote_selftest
//功能        :	机型遥控
//入口参数    :	无
//出口参数    :	无
//构	建:
//修	改: 	GCE XXX
************************************************/
void remote_selftest(void)
{
  // UUI32 data_buf;

   if((_Self_Test==ENABLE)||(!_Self_Test_EEp))
   {
       return;
   }

   _Self_Test=ENABLE;
   _Self_Test_EEp=0;

   Sys_Version_buf=rec_data[4];
  /* /////IAP_PageErase(SYS_TYPE_ADDR);

   data_buf.byte.ll=rec_data[4];
   data_buf.byte.lh=rec_data[4];
   data_buf.byte.hl=rec_data[4];
   data_buf.byte.hh=rec_data[4];
   //////IAP_WordProgram(SYS_TYPE_ADDR+4,data_buf.lword);*/
    IAPPageErase(TYPE_DATA_ADDR,IapROM);
    IAPWrite(TYPE_DATA_ADDR,rec_data[4],IapROM);

   Self_Test_DATA=0xff;

//    Auto_Restart=ENABLE;
   Power_Status=OFF;
   Mode_Set=COOL;
   Fan_Set=LOW;
   Temp_Heat_F=72;
   Temp_Cool_F=75;
   _Write_EEP_EN=1;
   Sys_data_write();
   test_cont2=0xff;
//   test_cont3=0;
//   test_cont4=0;
   test_seq=0;
   test_key_data=0;

   Buz_mode(BUZZ_TYPE7);
   Power_Delay_Time=0;

}


/*************************************************
//名称        :	cor258_inf_load
//功能        :	IR  液晶遥控
//入口参数    :	无
//出口参数    :	无
//构	建:
//修	改: 	GCE XXX
************************************************/
void cor258_inf_load(void)
{
     UI08  temp_buff_F;
     UI08  temp_buff_C;
     if(_Self_Test==ENABLE)
     {return;}
     if(_Water_Full)
     {return;}

     Buz_mode(BUZZ_TYPE7);

     if((rec_buf.power!=rec_buf_power2)||(!_first_rec))
     {
     	Timer_Setting_Time=0;
        Temp_Setting_Time=0;
        rec_buf_power2=rec_buf.power;
        if(rec_buf.power==ON)
        {
            Turn_On();
            _DISP_En=1;
            _DISP_En2=0;
        }
        else
        {
            Turn_Off();
            _DISP_En=0;
            _DISP_En2=1;
        }

        Buz_mode(BUZZ_TYPE8);

     }

     if(_sys_err)
     {return;}

     if(rec_buf.cf!=CF_Status)
     {
     	CF_Status= rec_buf.cf;
        Timer_Setting_Time=0;
     }

     //
     if(rec_buf.mode!=Mode_Buf)
     {
     	Mode_Buf=rec_buf.mode;
        Timer_Setting_Time=0;
        Temp_Setting_Time=0;
        if(!_first_rec)
        {
            Mode_Set=Mode_Buf;
        }
        else
        {
            Mode_Delay_Timer=2;
        }
     }

//     if((rec_buf.fan!=Fan_Buf)&&(Fan_set_enable==ENABLE))
     if(rec_buf.fan!=Fan_Buf)
     {
     	Fan_Buf=rec_buf.fan;
        Timer_Setting_Time=0;
        Temp_Setting_Time=0;
        if(!_first_rec)
        {
            Fan_Set=Fan_Buf;
        }
        else
        {
            Fan_Delay_Timer=2;
        }
        Fan_Delay_disp=10;
     }
     /////
     if(Mode_Buf==HEAT)
     {
        temp_buff_F=Temp_Heat_F;
     }
     else
     {
        temp_buff_F=Temp_Cool_F;
     }

     temp_buff_C=F_C(temp_buff_F);

     /////
     if(CF_Status==_C)
     {
     	if(rec_buf.temp!=temp_buff_C)
        {
            Temp_Set_C=rec_buf.temp;

            Temp_Set_F=C_F(Temp_Set_C);

            if(Mode_Buf==HEAT)
            {
                Temp_Heat_F=Temp_Set_F;
            }
            else
            {
                Temp_Cool_F=Temp_Set_F;
            }
            if((Power_Status==ON)&&((Mode_Buf==HEAT)||(Mode_Buf==COOL)))
            {
                Temp_Setting_Time=TEMP_SET_TIME;
            }
            Timer_Setting_Time=0;
            write_delay_time=2;
         }
     }
     else
     {
        if(rec_buf.temp!=temp_buff_F)
        {
            Temp_Set_F=rec_buf.temp;

            Temp_Set_C=F_C(Temp_Set_F);

            if(Mode_Buf==HEAT)
            {
                Temp_Heat_F=Temp_Set_F;
            }
            else
            {
                Temp_Cool_F=Temp_Set_F;
            }
            if((Power_Status==ON)&&((Mode_Buf==HEAT)||(Mode_Buf==COOL)))
            {
                Temp_Setting_Time=TEMP_SET_TIME;
            }
            Timer_Setting_Time=0;
            write_delay_time=2;
        }
     }
   	 //
     if(rec_buf.timer_f)
     {
     	Timer_set =rec_buf.timer_time;
        Timer_buff=Timer_set;
        Timer_run =Timer_set;
        Timer_run =Timer_run*3600;
        if(old_timer!=rec_buf.timer_time)
        {
	       old_timer=rec_buf.timer_time;
               Timer_Setting_Time=Set_Time;
               Temp_Setting_Time=0;
        }
     }
     else
     {
        Timer_run=0;
        Timer_Setting_Time=0;
     }
     if(!_first_rec)
     {
        _first_rec=1;
     }
}


/*************************************************
//名称        :	rec_cor279
//功能        :	IR  279简易遥控
//入口参数    :	无
//出口参数    :	无
//构	建:
//修	改: 	GCE XXX
************************************************/
void rec_cor279(void)
{
	switch(rec_data[2])
	{
		case 0x02:
        {
            Key_Number=power_key;
        }break;
		case 0x09:
        {
            Key_Number=func_key ;
        }break;
		case 0x11:
        {
            Key_Number=down_key;
        }break;
		case 0x1a:
        {
            Key_Number=fan_key ;
        }break;
		case 0x0a:
        {
            Key_Number=up_key ;
        }break;
		case 0x19:
        {
            Key_Number=timer_key;
        } break;
        case 0x01:
        {
            Key_Number=SWING_key;
        } break;
        case 0x12:
        {
	    Key_Number=lamp_key;
        } break;
        default:
        {
            return;
        };
	}
	rec_data[2]=0xff;
    _Key_Ok=1;
}


void rec_cor258(void)
{
    //////////////////////////////////////////////////////////////////////
    switch(rec_data[1]>>4)
    {
        case 1:
        {
            rec_buf.mode=COOL;
        }break;
        case 2:
        {
            if(Sys_Tpye==HEATCOOL)
            {
                rec_buf.mode=HEAT;
            }
            else
            {
                return;
            }
        };break;
        case 4:
        {
            rec_buf.mode=DRY;
        }break;
        case 8:
        {
            rec_buf.mode=FAN;
        }break;
        default:
        {
            ;
        }break;
    }
    //////////////////////////////////////////////////////////////////////
    switch(rec_data[1]&0x0f)
    {
        case 2:
        {
            rec_buf.fan=LOW;
        }break;
        case 4:
        {
            if(sys_fan_type==SPEED3)
            {
                rec_buf.fan=MID;
            }
        };break;
        case 8:
        {
            rec_buf.fan=HIGH;
        }break;
        default:
        {
            ;
        }break;
    }
    //////////////////////////////////////////////////////////////////////
    if(rec_data[2]&bit5)
    {
        if(rec_buf.cf!=_F)
        {
           write_delay_time=1;
           rec_buf.cf=_F;
        }
    }
    else
    {
        if(rec_buf.cf!=_C)
        {
           write_delay_time=1;
           rec_buf.cf=_C;
        }
    }
    //////////////////////////////////////////////////////////////////////
    if(rec_buf.cf==_F)
    {
        rec_buf.temp=rec_data[3];
    }
    else
    {
        rec_buf.temp=rec_data[3]+16;
    }
    //////////////////////////////////////////////////////////////////////
    if(rec_data[2]&bit7)
    {
        rec_buf.power=ON;
    }
    else
    {
        rec_buf.power=OFF;
    }
    //////////////////////////////////////////////////////////////////////
    if(rec_data[2]&bit6)
    {
    	rec_buf.timer_time=rec_data[2]&0x1f;
        if(rec_buf.timer_f==0)
        {
            old_timer=rec_buf.timer_time+1;
        }
        rec_buf.timer_f=1;
    }
    else
    {
    	rec_buf.timer_f=0;
    }
    //////////////////////////////////////////////////////////////////////
    cor258_inf_load();
}


/*************************************************
//名称        :	remote_type_judge
//功能        :	区别不同遥控
//入口参数    :	无
//出口参数    :	无
//构	建:
//修	改: 	GCE XXX
************************************************/
void remote_type_judge(void)
{
  UI08 i=0;

  if(!_Rec_Ok)
  { return; }
  _Rec_Ok=0;

  if(  (rec_bytes==7) //判断满足接收长度
     &&(rec_data[0]==0x84)
     &&(rec_data[5]==0xAA)
     &&(rec_data[6]==0x55))
  {
  	 _Self_Test_EEp=1;
	 remote_selftest();//机型遥控器
  }
  else if(  (rec_bytes==4)&&(rec_data[0]==0x48))//
  {
	 rec_cor258();     //面板遥控 4个字节
  }
  else if(  (rec_bytes==4)&&(rec_data[0]==0x20)
  	  &&(rec_data[1]==0xdf)
  	  &&(rec_data[3]==(UI08)(~rec_data[2])))
  {
	  rec_cor279();    //LCD遥控
  }

  for(i=0;i<total_byte;i++)//清除接收数据
  {
    rec_data[i]=0;
  }
}


/*************************************************
//名称        :	Remote_Deal
//功能        :	遥控数据处理（主函数中调用）
//入口参数    :	无
//出口参数    :	无
//构	建:
//修	改: 	GCE XXX
************************************************/
void Remote_Deal(void)
{
  prg_10ms_remote();
  remote_type_judge();//不同遥控解码处理
}

/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
