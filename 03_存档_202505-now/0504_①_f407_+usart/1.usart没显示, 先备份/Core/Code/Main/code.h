#ifndef __CODE_H
#define __CODE_H
#include "main.h"

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usmart.h"





#define POWER_ON() { printf("�����ɹ�!\r\n"); printf("Ұ��-F407-���컢\r\n"); }





void CODE_MAIN(void);
void CODE_INIT(void);
void CODE_SELF_TEST(void);
void CODE_WHILE(void);


#endif

