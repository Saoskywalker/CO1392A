#ifndef __IIC24C02_DRIVER_H
#define __IIC24C02_DRIVER_H

extern xdata UI08 IIC_count;
extern xdata UUI08 IIC_buy;
#define  _LED_IIC   IIC_buy.bit_.b0
#define  _REC_IIC   IIC_buy.bit_.b1


#define  SCL_output  P3CON |= bit3

#define  SCL_input   P3CON &= ~bit3


#define  SDA_output  P3CON |= bit2

#define  SDA_input   P3CON &= ~bit2

#define SDA_high    P32=1;
#define SDA_low     P32=0;

#define SCL_high    P33=1;
#define SCL_low     P33=0;


#define Sda         (P32)
#define Scl         (P33)

#define IIC_DELAY   20

//#define delayH_IIC    delay(13)
//#define delayL_IIC    delay(3)

#define delayH_IIC    delay(35)
#define delayL_IIC    delay(20)


//extern UI08 read_data_N(UI08 Address , UI08 *ptr,UI08 n);

//extern UI08 write_Ndata_24c02(UI08 Addr,UI08 Sub_Addr,UI08 *ptr1,UI08 len);
extern UI08 read_Ndata(UI08 Addr);

extern void write_Ndata(UI08 Addr,UI08 Sub_Addr,UI08 *ptr1,UI08 len);













#endif

