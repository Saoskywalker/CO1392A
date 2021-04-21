#ifndef _SLEEP_H
#define _SLEEP_H

extern xdata UI08   Sleep_scan_time;
extern xdata UUI08  Sleep_bit;

#define   _Read_Sleep_OK    Sleep_bit.bit_.b0



extern  void SYS_Sleep_deal(void);


#endif











