#include "tool.h"

#define __LED__PC13__
#ifdef  __LED__PC13__

    // read
        #define LED_PC13_read \
            (HAL_GPIO_ReadPin(LED_PC13_GPIO_Port, LED_PC13_Pin))

    // write
        #define LED_PC13_toggle \
            (HAL_GPIO_TogglePin(LED_PC13_GPIO_Port, LED_PC13_Pin))

        #define LED_PC13_action(action) \
            (HAL_GPIO_WritePin(LED_PC13_GPIO_Port, LED_PC13_Pin, select_GPIO_status(LED_PC13, action)))

        #define LED_PC13_on \
            (LED_PC13_action(LED_ON ))

        #define LED_PC13_off \
            (LED_PC13_action(LED_OFF))

#endif



