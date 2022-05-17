
//*************************************************************************************************
//  Copyright (c) 	深圳市赛元微电子有限公司
//	文件名称	:  S_TouchKeyCFG.h
//	作者		: 
//	模块功能	:  触控键配置文件
// 	版本		:  V0.2
// 	更改记录	:
//************************************************************************************************
#ifndef __S_TOUCHKEYCFG_H__
#define __S_TOUCHKEYCFG_H__
#define		    SOCAPI_SET_TOUCHKEY_TOTAL					  4
#define			SOCAPI_SET_TOUCHKEY_CHANNEL					  0x15800000
unsigned int  code TKCFG[17] = {0,1,0,5,10,3000,200,100,2,0,0,4,0,65535,65535,65535,31}; 
unsigned char code TKChannelCfg[4][8]={
0x03,0x3c,0x04,0x08,0x18,0x05,0x01,0x79,
0x03,0x3c,0x04,0x08,0x38,0x05,0x01,0x17,
0x03,0x3c,0x04,0x08,0x16,0x05,0x01,0xa8,
0x03,0x3c,0x04,0x08,0x33,0x05,0x01,0x2d,
};
#endif