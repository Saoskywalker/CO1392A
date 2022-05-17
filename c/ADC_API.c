#include "General.h"


MCU_xdata ADC_PARA Temp_room;
MCU_xdata ADC_PARA Temp_coil;
MCU_xdata ADC_PARA Hum_para;//温度变量
MCU_xdata ADC_PARA Temp_Comp;//温度变量

MCU_xdata UI08   Temp_room_Err_cont=0;//室温故障计数
MCU_xdata UI08   Temp_room_ok_cont=0;//室温故障计数
MCU_xdata UI08   Temp_coil_Err_cont=0;//管温故障计数
MCU_xdata UI08   Hum_Err_cont=0;//湿度故障计数
MCU_xdata UI08   Hum_ok_cont=0;//湿度故障计数


MCU_const UI16 Temp_tab_10k_3450[];//温度查表
/*************************************************
 // 函数名称    : Adc_data_init
 // 功能描述    : 变量初始化
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void  Adc_data_init(void)
{
	Temp_room.AD_value=528;
	Temp_room.value=36;
	Temp_room.status=AI_NORMAL;

        Temp_coil.AD_value=528;
	Temp_coil.value=36;
	Temp_coil.status=AI_NORMAL;

	Hum_para.AD_value=256;
	Hum_para.value=60;
        Hum_para.status=AI_NORMAL;

}

/*************************************************
//名称        :	F_C
//功能        : F转换为C
//入口参数    :	无
//出口参数    :	无
//构	建: 	GCE XXX
//修	改: 	GCE XXX

原始公式 四舍五入
(X-32)*5/9+0.5
优化公式
((X-32)*10+9)/18
**************************************************/
UI08 F_C(UI08 cfdata)
{
	UI08 a=0;
      if(cfdata>32)
      {a=((cfdata-32)*10+9)/18;}
      else
      {a=((32-cfdata)*10+9)/18;}

 	return 	a;
}

//==========================================================
UI16 ADC_lookup(UI16 value,UI16 *ptr,UI16 tab_leng)
{
	UI16 low,high,mid,x,y;
	low=0;
	mid=0;
	high=tab_leng-1;
	if(value<=*ptr)
	   return low;
	if(value>=*(ptr+high))
	   return high;
	//
	while(low<=high)
	{
    mid=(low+high)/2;
	  if(*(ptr+mid)==value)
		   return mid;
		else if(*(ptr+mid)>value)
			 high=mid-1;
		else
			 low=mid+1;
  }
	if(value<*(ptr+mid))
	  {
			x=*(ptr+mid)-value;
      y=value-*(ptr+mid-1);
		  if(x<=y)
			   return mid;
			else
			   return mid-1;

	  }
		else
		{
			x=*(ptr+mid+1)-value;
      y=value-*(ptr+mid);
		  if(x<=y)
			   return mid+1;
			else
			   return mid;

	  }
}
/*************************************************
 // 函数名称    : room_temp_deal
 // 功能描述    : 室温处理
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void room_temp_deal(void)
{
	UI16 data_buf=0;
	UI32 AD_buf;

	AD_buf=Temp_room.AD_value;
    data_buf=AD_buf*315/65535;//度F温度范围-49~266 ,0代表-49华氏度
    if(data_buf<(49+5))
    {
        Temp_room.value=5;
    }
    else
    {
        Temp_room.value=data_buf-(49);
    }

    data_buf=AD_buf*175/65535;  //度C温度范围-45~130 ,0代表-45摄氏度
    if(data_buf<(45-15))
    {
        Temp_room.C_value=0;
    }
    else
    {
        Temp_room.C_value=data_buf-(45-15);
    }




}
/*************************************************
 // 函数名称    : coil_temp_deal
 // 功能描述    : 管温处理
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void coil_temp_deal(void)
{
    UI08 data_buf=0;
    UI16 AD_buf=0;

     if((Temp_coil.AD_value<30)&&(Power_Delay_Time==0))
     {
     	 if(++Temp_coil_Err_cont>=2)
     	 {
	    Temp_coil_Err_cont=0;
	    Temp_coil.status=AI_CUT;
	    Temp_coil.value=0;
	 }
     }
     else if((Temp_coil.AD_value>500)&&(Power_Delay_Time==0))
     {
         if(++Temp_coil_Err_cont>=2)
     	 {
	    Temp_coil_Err_cont=0;
	    Temp_coil.status=AI_SHORT;
	    Temp_coil.value=0;
	 }
     }
    else
     {
     	Temp_coil_Err_cont=0;
	Temp_coil.status=AI_NORMAL;

	Temp_coil.value=Temp_tab_10k_3450[data_buf-48];
	Temp_coil.value=5+ADC_lookup(Temp_coil.AD_value,Temp_tab_10k_3450,154);//表从5F开始
      //
      if(Temp_coil.value<=32)
      {Temp_coil.C_value=15-F_C(Temp_coil.value);}
      else
      {Temp_coil.C_value=F_C(Temp_coil.value)+15;}
     }
}
/*************************************************
 // 函数名称    : hum_deal
 // 功能描述    : 管温处理
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
/*void hum_deal(void)
{

}  */
/*************************************************
 // 函数名称    : Temp_deal
 // 功能描述    : 温度处理
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Temp_Deal(void)
{
    if(!M_ADC_check)
    {return;}
    M_ADC_check=0;

    room_temp_deal();
    coil_temp_deal();
   // hum_deal();
}

 //5F--158F -15C--70C
MCU_const UI16 Temp_tab_10k_3450[]=
{
75  ,// 0
77  ,// 1
79  ,// 2
81  ,// 3
83  ,// 4
85  ,// 5
86  ,// 6
88  ,// 7
90  ,// 8
93  ,// 9
95  ,// 10
97  ,// 11
99  ,// 12
101 ,// 13
103 ,// 14
105 ,// 15
108 ,// 16
110 ,// 17
112 ,// 18
114 ,// 19
117 ,// 20
119 ,// 21
121 ,// 22
124 ,// 23
126 ,// 24
128 ,// 25
131 ,// 26
133 ,// 27
136 ,// 28
138 ,// 29
141 ,// 30
143 ,// 31
146 ,// 32
149 ,// 33
151 ,// 34
154 ,// 35
156 ,// 36
159 ,// 37
162 ,// 38
164 ,// 39
167 ,// 40
170 ,// 41
173 ,// 42
175 ,// 43
178 ,// 44
181 ,// 45
184 ,// 46
186 ,// 47
189 ,// 48
192 ,// 49
195 ,// 50
197 ,// 51
200 ,// 52
203 ,// 53
206 ,// 54
209 ,// 55
211 ,// 56
214 ,// 57
217 ,// 58
220 ,// 59
223 ,// 60
225 ,// 61
228 ,// 62
231 ,// 63
234 ,// 64
237 ,// 65
239 ,// 66
242 ,// 67
245 ,// 68
248 ,// 69
251 ,// 70
253 ,// 71
256 ,// 72
259 ,// 73
261 ,// 74
264 ,// 75
267 ,// 76
270 ,// 77
272 ,// 78
275 ,// 79
278 ,// 80
280 ,// 81
283 ,// 82
285 ,// 83
288 ,// 84
291 ,// 85
293 ,// 86
296 ,// 87
298 ,// 88
301 ,// 89
303 ,// 90
306 ,// 91
308 ,// 92
311 ,// 93
313 ,// 94
316 ,// 95
318 ,// 96
320 ,// 97
323 ,// 98
325 ,// 99
327 ,// 100
330 ,// 101
332 ,// 102
334 ,// 103
336 ,// 104
339 ,// 105
341 ,// 106
343 ,// 107
345 ,// 108
347 ,// 109
349 ,// 110
351 ,// 111
353 ,// 112
356 ,// 113
358 ,// 114
360 ,// 115
362 ,// 116
364 ,// 117
365 ,// 118
367 ,// 119
369 ,// 120
371 ,// 121
373 ,// 122
375 ,// 123
377 ,// 124
379 ,// 125
380 ,// 126
382 ,// 127
384 ,// 128
385 ,// 129
387 ,// 130
389 ,// 131
390 ,// 132
392 ,// 133
394 ,// 134
395 ,// 135
397 ,// 136
398 ,// 137
400 ,// 138
401 ,// 139
403 ,// 140
404 ,// 141
406 ,// 142
407 ,// 143
409 ,// 144
410 ,// 145
412 ,// 146
413 ,// 147
415 ,// 148
416 ,// 149
417 ,// 150
418 ,// 151
419 ,// 152
421 // 153
};


/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
