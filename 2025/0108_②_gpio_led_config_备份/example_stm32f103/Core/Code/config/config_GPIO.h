#include "main.h"

// GPIO_PIN_RESET = 0;
// GPIO_PIN_SET = 1;

typedef enum
{
    GPIO_enum_begin = 0,
    /* begin */

    /* end */
    GPIO_enum_end
} GPIO_enum;

#define LED_PC13_on_status 0
#define LED_PC13_off_status (1 - LED_PC13_on_status)

#define LED_PA0_on_status 0
#define LED_PA0_off_status (1 - LED_PA0_on_status)





