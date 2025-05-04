#ifndef __CODE_H
#define __CODE_H
#include "main.h"

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usmart.h"

#include "my_flash.h"
#include "my_fatfs.h"

// template
#if 0

    #include "main.h"
    #if defined(__STM32F1XX_H)
        // F103
    #elif defined(__STM32L1XX_H)
        // L151
    #elif defined(__STM32F4xx_H)
        // F407
    #else
        #error "��? MCU����F103, L151����F407��? ��..."
    #endif

#endif
// template


// configure
#if 1

    #if defined(__STM32F1XX_H)
        /* power on */
        #define DEVICE_STRING "XX-F103-XX"
    #elif defined(__STM32L1XX_H)
        /* power on */
        #define DEVICE_STRING "�ļ�-L151-����"
        /* delay & usmart */
        #define di_sysclk 36
        #define ui_sysclk 36
    #elif defined(__STM32F4xx_H)
        /* power on */
        #define DEVICE_STRING "Ұ��-F407-���컢"
        /* delay & usmart */
        #define di_sysclk 168
        #define ui_sysclk 84
    #else
        #error "��? MCU����F103, L151����F407��? ��..."
    #endif

#define POWER_ON() { printf("\r\n�����ɹ�!\r\n"); printf("%s\r\n", DEVICE_STRING); }

#endif
// configure


// ikkun
#if 1
#define ikkun() { \
    printf("\r\nHardFault_Handler: Ӳ��������.\r\n"); \
    printf("��������...\r\n"); \
    char kun[] = "\
                                 &BGG#&     \r\n\
                                P55PPP5G&   \r\n\
                      &##BB5?B#BGBBG5GGGP&  \r\n\
                     #BBBBBG^PBBBGB57J5PP&  \r\n\
                   #B##BBBB#!J#BBBBBYJPGB   \r\n\
                 &###&#BBBB#J!BB###BB#      \r\n\
                 ###&5GBBBB#P!PBPB###B&     \r\n\
                 &###GBBB###75##BY?5B##     \r\n\
                  &#######&P~#####GJ7YB     \r\n\
                    ######&?Y&#####&GJP     \r\n\
                  &#PY5GBBBJB######&&#&     \r\n\
                  PJ??JJJYYY55PGB####       \r\n\
               &GY??JJJYYYYYYYYYY5G&        \r\n\
             &PJ???JJJYYYYYYYYYYYYJ5&       \r\n\
            BJ?JJJJYYY55PPP555555YYJB       \r\n\
          &Y?JYYYY5PGB#&    #G555YYYJ#      \r\n\
          Y7J55PG#&           &G5555YY&     \r\n\
         #7?JYG                 #GPPY?J&    \r\n\
         G7?JY#                   #YJ??P    \r\n\
         5?JJ5                     GJ??J&   \r\n\
         Y?JY&                     &Y???G   \r\n\
        &J?JG                       &Y??5   \r\n\
        &PGB                         #55P   \r\n\
        BGPB                          #GPB  \r\n\
      &BGBGPB                         #PGBB&\r\n\
    "; \
    printf("%s", &kun); \
    HAL_NVIC_SystemReset();\
}

#endif
// ikkun


void CODE_MAIN(void);
void CODE_INIT(void);
void CODE_SELF_TEST(void);
void CODE_WHILE(void);


#endif

