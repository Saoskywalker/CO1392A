#include "General.h"

//xdata UUI16 SYS_FlashAddr;
//xdata UUI16 SYS_FlashData;
//xdata UI08  SYS_flash_en;

UI08 code *IapAddr = 0x00;

/*****************************************************
*函数名称：void IAPPageErase(uint Addr,uchar IAPArea)
*函数功能：IAP页擦除
*入口参数：Addr,IAPArea
IAPArea 0x00：选择ROM区操作  0x02：选择EEPROM区操作
Addr    0~0xffff
*出口参数：void
*****************************************************/
void IAPPageErase(UI16 Addr,UI08 IAPArea)
{
    bit temp = EA;
    EA = 0;
    IAPADE = IAPArea;

    IAPADH = (unsigned char)((Addr >> 8)); //写入待擦除首地址高位
    IAPADL = (unsigned char)Addr;          //写入待擦除首地址低位

    IAPKEY = 0XF0;
    IAPCTL = 0X20;
    IAPCTL |= 0X02;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    IAPADE = 0x00;
    EA = temp;
}
/*****************************************************
*函数名称：uchar IAPRead(uchar Addr,uchar IAPArea)
*函数功能：IAP读初始化
*入口参数：Addr,IAPArea
IAPArea 0x00：选择ROM区操作  0x02：选择EEPROM区操作
Addr    0~0xffff
*出口参数：ReadValue
*****************************************************/
UI08 IAPRead(UI16 Addr,UI08 IAPArea)
{
    UI08 ReadValue = 0x00;
    bit temp = EA;
    EA = 0;
    IAPKEY = 0XF0;
    IAPADE = IAPArea;
    ReadValue = *(IapAddr+Addr); //读取的数据
    IAPADE = 0x00;               //MOVC指向ROM
    IAPKEY = 0X00;
    EA = temp;
    return ReadValue;
}

/*****************************************************
*函数名称：void IAPWrite(uchar Addr,uchar Value,uchar IAPArea)
*函数功能：IAP写初始化
*入口参数：Addr,Value,IAPArea
IAPArea 0x00：选择ROM区操作  0x02：选择EEPROM区操作
Addr    0~0xffff
Value   写入的数据
*出口参数：void
*****************************************************/
//IAP写操作
void IAPWrite(UI16 Addr,UI08 Value,UI08 IAPArea)
{
    bit temp = EA;
    EA = 0;
    IAPADE = IAPArea;   //IAPArea=0x00：选择ROM区操作  IAPArea=0x02：选择EEPROM区操作
    IAPDAT = Value;     //写入数据Data
    IAPADH = (unsigned char)((Addr >> 8));   //写入地址的高8位
    IAPADL = (unsigned char)Addr;            //写入地址的低8位
    IAPKEY = 0xF0;		//IAP开启保护控制时间
    IAPCTL = 0X10;      //执行“写入或快擦除操作命令”后，进入flash编程操作
    IAPCTL |= 0x02;     //执行“写入或快擦除操作命令”
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    IAPADE = 0x00;      //MOVC指向ROM
    EA = temp;
}


