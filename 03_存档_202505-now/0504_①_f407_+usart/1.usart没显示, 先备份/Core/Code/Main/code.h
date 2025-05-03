#ifndef __CODE_H
#define __CODE_H
#include "main.h"

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usmart.h"





#define POWER_ON() { printf("启动成功!\r\n"); printf("野火-F407-霸天虎\r\n"); }





void CODE_MAIN(void);
void CODE_INIT(void);
void CODE_SELF_TEST(void);
void CODE_WHILE(void);


#endif

