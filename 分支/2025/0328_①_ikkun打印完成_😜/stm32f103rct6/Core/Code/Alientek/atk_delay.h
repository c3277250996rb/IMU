#ifndef __DELAY_H
#define __DELAY_H 			   
#include "atk_sys.h"  

#ifdef USE_MY_ALIENTEK

//////////////////////////////////////////////////////////////////////////////////	 
//创建日期:2019/9/17
//版本：V1.8
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//********************************************************************************
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif  // USE_MY_ALIENTEK

#endif  // __DELAY_H





























