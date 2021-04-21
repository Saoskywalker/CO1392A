#include "General.h"

//======================全局变量=================================
xdata UI08 lookup_seq=0;	//查表索引
code  UI16 Temp_tab_10k_3450[];   //5F--158F -15C--70C 长度154

/////////////////////////////////////////////////////////////
//ADC采样变量
xdata ADC_PARA    Temp_room;
xdata ADC_PARA	  Temp_coil;
xdata ADC_PARA	  Tcomp_coil;
xdata UI08        Troom_dsp;//温度显示变量
xdata UI08        Troom_dsp_com;//温度显示变量
xdata UI08        Troom_dsp_com_timer;//温度显示变量
xdata UI08        Troom_update_time;//温度显示更新变量
xdata ADC_PARA    *ADC_struct_addr[2];
/***********************************************************
*函 数 名：  void  ADC_Data_Init(void)
*功能描述：  adc初始化程序
*输入参数：  无
*函数返回值：无
***********************************************************/
void ADC_Data_Init(void)
{
    lookup_seq=0;

    Temp_room.value=77;
    Temp_room.status=AI_NORMAL;
    Temp_room.AD_value=255;
    //
    Temp_coil.AD_value=255;
    Temp_coil.value=77;
    Temp_coil.status=AI_NORMAL;
    //
    Tcomp_coil.AD_value=255;
    Tcomp_coil.value=77;
    Tcomp_coil.status=AI_NORMAL;


    Troom_dsp=77;
    Troom_dsp_com=77;
    Troom_dsp_com_timer=30;
    Troom_update_time=0;

    ADC_struct_addr[0]=&Temp_room;
    ADC_struct_addr[1]=&Temp_coil;
}
/***********************************************************
*函 数 名：  UI16 ADC_lookup(UI16 value,UI16 tab_leng)
*功能描述：  查表函数
*输入参数：  value：需要查表的值
             tab_leng：长度
*函数返回值：查表返回值
***********************************************************/
UI16 ADC_lookup(UI16 value,UI16 tab_leng)
{
    UI16 low=0;
    UI16 high=0;
    UI16 mid=0;
    UI16 x=0;
    UI16 y=0;
    high=tab_leng-1;
    if(value<=Temp_tab_10k_3450[0])
    {
        return low;
    }
    if(value>=Temp_tab_10k_3450[high])
    {
        return high;
    }
    //
    while(low<=high)
    {
        mid=(low+high)/2;
        if(Temp_tab_10k_3450[mid]==value)
        {
            return mid;
        }
        else if(Temp_tab_10k_3450[mid]>value)
        {
            high=mid-1;
        }
        else
        {
            low=mid+1;
        }
    }
    if(value<Temp_tab_10k_3450[mid])
    {
        x=Temp_tab_10k_3450[mid]-value;
        y=value-Temp_tab_10k_3450[mid-1];
        if(x<=y)
        {
            return mid;
        }
        else
        {
            return mid-1;
        }
    }
    else
    {
        x=Temp_tab_10k_3450[mid+1]-value;
        y=value-Temp_tab_10k_3450[mid];
        if(x<=y)
        {
            return mid+1;
        }
        else
        {
            return mid;
        }
    }
}

/*************************************************
 // 函数名称    : void Temp_Deal(void)
 // 功能描述    : 温度处理函数
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Temp_Deal(void)
{
    //ADC_sample();

    if(!_ms100_ADC)  //1秒查1个通道的表
    {
        return;
    }
    _ms100_ADC=0;
    //
    if(++lookup_seq>=ADC_channel_total)
    {
        lookup_seq=0;
    }
    //
    if((!_Tcoil_Enable)&&(lookup_seq==1))//无管温
    {
        ADC_struct_addr[lookup_seq]->AD_value=255;
    }
    //
    if(ADC_struct_addr[lookup_seq]->AD_value<30) //开路报警
    {
        ADC_struct_addr[lookup_seq]->status=AI_CUT;
        ADC_struct_addr[lookup_seq]->value=0;
    }
    else if(ADC_struct_addr[lookup_seq]->AD_value>500)//短路报警
    {
        ADC_struct_addr[lookup_seq]->status=AI_SHORT;
        ADC_struct_addr[lookup_seq]->value=0;
    }
    else
    {
        ADC_struct_addr[lookup_seq]->status=AI_NORMAL;
        if(_temp_update_enb)//
        {
            ADC_struct_addr[lookup_seq]->value=5+ADC_lookup(ADC_struct_addr[lookup_seq]->AD_value,154);//从5F开始
        }
    }
}

//======================常量数组=================================
code  UI16 Temp_tab_10k_3450[]=   //5F--158F -15C--70C 长度154
{
    75	,//	0
    77	,//	1
    79	,//	2
    81	,//	3
    83	,//	4
    85	,//	5
    86	,//	6
    88	,//	7
    90	,//	8
    93	,//	9
    95	,//	10
    97	,//	11
    99	,//	12
    101	,//	13
    103	,//	14
    105	,//	15
    108	,//	16
    110	,//	17
    112	,//	18
    114	,//	19
    117	,//	20
    119	,//	21
    121	,//	22
    124	,//	23
    126	,//	24
    128	,//	25
    131	,//	26
    133	,//	27
    136	,//	28
    138	,//	29
    141	,//	30
    143	,//	31
    146	,//	32
    149	,//	33
    151	,//	34
    154	,//	35
    156	,//	36
    159	,//	37
    162	,//	38
    164	,//	39
    167	,//	40
    170	,//	41
    173	,//	42
    175	,//	43
    178	,//	44
    181	,//	45
    184	,//	46
    186	,//	47
    189	,//	48
    192	,//	49
    195	,//	50
    197	,//	51
    200	,//	52
    203	,//	53
    206	,//	54
    209	,//	55
    211	,//	56
    214	,//	57
    217	,//	58
    220	,//	59
    223	,//	60
    225	,//	61
    228	,//	62
    231	,//	63
    234	,//	64
    237	,//	65
    239	,//	66
    242	,//	67
    245	,//	68
    248	,//	69
    251	,//	70
    253	,//	71
    256	,//	72
    259	,//	73
    261	,//	74
    264	,//	75
    267	,//	76
    270	,//	77
    272	,//	78
    275	,//	79
    278	,//	80
    280	,//	81
    283	,//	82
    285	,//	83
    288	,//	84
    291	,//	85
    293	,//	86
    296	,//	87
    298	,//	88
    301	,//	89
    303	,//	90
    306	,//	91
    308	,//	92
    311	,//	93
    313	,//	94
    316	,//	95
    318	,//	96
    320	,//	97
    323	,//	98
    325	,//	99
    327	,//	100
    330	,//	101
    332	,//	102
    334	,//	103
    336	,//	104
    339	,//	105
    341	,//	106
    343	,//	107
    345	,//	108
    347	,//	109
    349	,//	110
    351	,//	111
    353	,//	112
    356	,//	113
    358	,//	114
    360	,//	115
    362	,//	116
    364	,//	117
    365	,//	118
    367	,//	119
    369	,//	120
    371	,//	121
    373	,//	122
    375	,//	123
    377	,//	124
    379	,//	125
    380	,//	126
    382	,//	127
    384	,//	128
    385	,//	129
    387	,//	130
    389	,//	131
    390	,//	132
    392	,//	133
    394	,//	134
    395	,//	135
    397	,//	136
    398	,//	137
    400	,//	138
    401	,//	139
    403	,//	140
    404	,//	141
    406	,//	142
    407	,//	143
    409	,//	144
    410	,//	145
    412	,//	146
    413	,//	147
    415	,//	148
    416	,//	149
    417	,//	150
    418	,//	151
    419	,//	152
    421	,//	153
};


