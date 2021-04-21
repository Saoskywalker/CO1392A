#include "General.h"


xdata UUI08  SYS_Inspect_bit;//bit
xdata UI08   SYS_Inspect_Key_Data;//按键键值
xdata UI08   PowerKey_Delay_Time;//上电延时
xdata UI08   Disp_Temp_Timer;//显示时间
xdata UI16   Comp_Test_Time;//测试时间

xdata UI16   Comp_Test_first_count;//上电累计时间
xdata UI16   Comp_Test_Key_count;//按键
xdata UI16   Comp_Test_Disp_En_Timer;//显示时间


void SYS_Inspect_Data_Init(void)
{
   SYS_Inspect_bit.byte=0;
   Comp_Test_Time=0;
   Comp_Test_Key_count=0;
   SYS_Inspect_Key_Data=0;
   Comp_Test_Disp_En_Timer=0;
   Comp_Test_first_count=0;
	 PowerKey_Delay_Time=0;
	 Disp_Temp_Timer=0xff;
}

void SYS_Inspect_ms_general(void)
{
    if(!_SYS_Inspect_ms)
    {
       return;
	}
	_SYS_Inspect_ms=0;

	if(Comp_Test_Key_count<0Xffff)
	{
            Comp_Test_Key_count++;
	}


	if(Comp_Test_Disp_En_Timer>0)
	{
       Comp_Test_Disp_En_Timer--;
	}


	if(Comp_Test_first_count<0xffff)
  {
       Comp_Test_first_count++;
	}

	if(PowerKey_Delay_Time>0)
  {
       PowerKey_Delay_Time--;
	}

}

void SYS_Inspect_s_general(void)
{
        if(!_SYS_Inspect_s)
       {
        return;
       }
	_SYS_Inspect_s=0;


	if(_COMP_TEST_EN)
	{
	   if(Comp_Test_Time<0Xffff)
       {
         Comp_Test_Time++;
	   }
	}
	else
	{
       Comp_Test_Time=0;
	}

	if(Disp_Temp_Timer<0xff)
	{
       Disp_Temp_Timer++;
	}
}



void SYS_Inspect_Key(void)
{
	if(Power_Status==OFF)
	{
       if(_COMP_TEST_EN)
       {
           _COMP_TEST_EN=0;
	   }
	}

    if(!_SYS_Inspect_Key_OK)
    {
       return;
	}
	_SYS_Inspect_Key_OK=0;

    if(SYS_Inspect_Key_Data!=(power_key))
    {
           Comp_Test_Key_count=0;
	   _Comp_Test_Key_Count_EN=0;

	   return;
    }

    if((Comp_Test_Key_count<100)||(Comp_Test_Key_count==0xffff))
    {
       return;
	}

	if((Comp_Test_first_count<=200)||(_Comp_Test_Key_First_Status))
    {
       _Comp_Test_Key_First_Status=1;

	   return;
    }

	if(PowerKey_Delay_Time>0)
	{
           _Comp_Test_Key_Count_EN=1;
	}

	if(       (Comp_Test_Key_count>(UI16)(1000*20))
		&&(Comp_Test_Key_count<0xffff)
		&&(_Comp_Test_Key_Count_EN))
		//&&(Power_Status==ON))//不适用松手检测
	{
		Turn_On();
        	shake_count=0xff;
		Read_key_delay=50;

                _COMP_TEST_EN=1;
		_Comp_Test_Key_Count_EN=0;

		Comp_Test_Disp_En_Timer=2000;

		Comp_Test_Key_count=0xffff;

		Mode_Buf=COOL;
		Mode_Set=COOL;
		Buz_mode(BUZZ_TYPE8);
	}

}


void Comp_Test_general(void)
{
    if(!_COMP_TEST_EN)
    {
       Comp_Test_Time=0;
       return;
	}

  if(_Water_Full)
	{
	   _COMP_TEST_EN=0;
	   return;
	}


	if(Comp_Test_Time<COMP_TEST_ON_TIME)
    {
		 Comp_para.OUT=ON;
		 Outfan_Speed=HIGH;
	     OutLfan_para.OUT=OFF;
         OutHfan_para.OUT=ON;
		 fan_speed=HIGH;
		 Lfan_para.OUT=OFF;
         Mfan_para.OUT=OFF;
         Hfan_para.OUT=ON;
		 Way4_para.OUT=OFF;
         Pump_para.OUT=ON;
	}
    else if(Comp_Test_Time<(COMP_TEST_ON_TIME+COMP_TEST_OFF_TIME))
    {
         Comp_para.OUT=OFF;
	 Outfan_Speed=FANOFF;
	     OutLfan_para.OUT=OFF;
         OutHfan_para.OUT=OFF;
	 fan_speed=FANOFF;
		 Lfan_para.OUT=OFF;
         Mfan_para.OUT=OFF;
         Hfan_para.OUT=OFF;
		 Way4_para.OUT=OFF;
         Pump_para.OUT=OFF;
	}
	else
	{
         Comp_Test_Time=0;
	}
}


void SYS_Inspect_Disp(void)
{

   if(Comp_Test_Disp_En_Timer>0)
   {
       disp_All();
   }

}

void disp_temp(void)
{
   UI08  dsp_temp=0;

   if(Disp_Temp_Timer==1)
   {
	   dsp_temp=Temp_room.value;
	   if(CF_Status==_C)
       {
          dsp_temp=F_C(dsp_temp);
       }

       if(Temp_room.value<32)
       {
          dig1_2_dsp(dsp_temp);
   	      dig1_num=DATA_g;  //-
       }
       else
       {
          dig1_2_dsp(dsp_temp);
       }
   }
   else if(Disp_Temp_Timer==3)
   {
	   dsp_temp=Temp_coil.value;
	   if(CF_Status==_C)
       {
          dsp_temp=F_C(dsp_temp);
       }

       if(Temp_coil.value<32)
       {
          dig1_2_dsp(dsp_temp);
   	  dig1_num=DATA_g;  //-
       }
       else
       {
          dig1_2_dsp(dsp_temp);
       }
   }
   else if(Disp_Temp_Timer==5)
   {
       disp_All();
   }
   else
   {
             dig1_num=0;
	     dig2_num=0;
   }
}
void SYS_Inspect_Deal(void)
{
  SYS_Inspect_s_general();
  SYS_Inspect_Key();
  Comp_Test_general();

}




