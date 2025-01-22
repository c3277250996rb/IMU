#include "code.h"
/* macro function example */

    // #define LED_PC13_on() { \
    //     int i = LED_ON; \
    //     LED_PC13_action(i); \
    // }

    // #define LED_PC13_on \
    // (LED_PC13_action(LED_ON))

/* ---------------------- */


/* LED */

#define __LED__PC13__
#ifdef  __LED__PC13__

    // read
        #define LED_PC13_read \
            (HAL_GPIO_ReadPin(LED_PC13_GPIO_Port, LED_PC13_Pin))

    // write
        #define LED_PC13_toggle \
            (HAL_GPIO_TogglePin(LED_PC13_GPIO_Port, LED_PC13_Pin))

        #define LED_PC13_action(status) \
            (HAL_GPIO_WritePin(LED_PC13_GPIO_Port, LED_PC13_Pin, status))

        #define LED_PC13_on \
            (LED_PC13_action(LED_PC13_on_status))

        #define LED_PC13_off \
            (LED_PC13_action(LED_PC13_off_status))

#endif

#define __LED__PA0__
#ifdef  __LED__PA0__

    // read
        #define LED_PA0_read \
            (HAL_GPIO_ReadPin(LED_PA0_GPIO_Port, LED_PA0_Pin))

    // write
        #define LED_PA0_toggle \
            (HAL_GPIO_TogglePin(LED_PA0_GPIO_Port, LED_PA0_Pin))

        #define LED_PA0_action(status) \
            (HAL_GPIO_WritePin(LED_PA0_GPIO_Port, LED_PA0_Pin, status))

        #define LED_PA0_on \
            (LED_PA0_action(LED_PA0_on_status))

        #define LED_PA0_off \
            (LED_PA0_action(LED_PC13_off_status))

#endif

#define __LED__PB13__
#ifdef  __LED__PB13__

    // read
        #define LED_PB13_read \
            (HAL_GPIO_ReadPin(LED_PB13_GPIO_Port, LED_PB13_Pin))

    // write
        #define LED_PB13_toggle \
            (HAL_GPIO_TogglePin(LED_PB13_GPIO_Port, LED_PB13_Pin))

        #define LED_PB13_action(status) \
            (HAL_GPIO_WritePin(LED_PB13_GPIO_Port, LED_PB13_Pin, status))

        #define LED_PB13_on \
            (LED_PB13_action(LED_PB13_on_status))

        #define LED_PB13_off \
            (LED_PB13_action(LED_PC13_off_status))

#endif

/* --- */


