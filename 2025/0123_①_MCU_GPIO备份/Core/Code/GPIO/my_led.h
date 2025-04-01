#include "code.h"

// GPIO_PIN_RESET = 0;
// GPIO_PIN_SET = 1;

GPIO_Class_Struct LED_Class_BLUE = {
    Pin = LED_BLUE_PC13_Pin;
	Port = LED_BLUE_PC13_GPIO_Port;
    ON_Status = 0;
    OFF_Status = 1 - ON_Status;
};

GPIO_Class_Struct LED_Class_GREEN = {
    Pin = LED_GREEN_PB9_Pin;
	Port = LED_GREEN_PB9_GPIO_Port;
    ON_Status = 0;
    OFF_Status = 1 - ON_Status;
};

GPIO_Class_Struct LED_Class_YELLOW = {
    Pin = LED_YELLOW_PB13_Pin;
	Port = LED_YELLOW_PB13_GPIO_Port;
    ON_Status = 0;
    OFF_Status = 1 - ON_Status;
};

GPIO_Class_Struct LED_Class_RED = {
    Pin = LED_RED_PB1_Pin;
	Port = LED_RED_PB1_GPIO_Port;
    ON_Status = 0;
    OFF_Status = 1 - ON_Status;
};


#define BLUE_READ   MY_GPIO_READ(LED_Class_BLUE)
#define BLUE_ON     MY_GPIO_ON(LED_Class_BLUE)
#define BLUE_OFF    MY_GPIO_OFF(LED_Class_BLUE)
#define BLUE_Toggle MY_GPIO_Toggle(LED_Class_BLUE)

#define GREEN_READ   MY_GPIO_READ(LED_Class_GREEN)
#define GREEN_ON     MY_GPIO_ON(LED_Class_GREEN)
#define GREEN_OFF    MY_GPIO_OFF(LED_Class_GREEN)
#define GREEN_Toggle MY_GPIO_Toggle(LED_Class_GREEN)
    
#define YELLOW_READ   MY_GPIO_READ(LED_Class_YELLOW)
#define YELLOW_ON     MY_GPIO_ON(LED_Class_YELLOW)
#define YELLOW_OFF    MY_GPIO_OFF(LED_Class_YELLOW)
#define YELLOW_Toggle MY_GPIO_Toggle(LED_Class_YELLOW)

#define RED_READ   MY_GPIO_READ(LED_Class_RED)
#define RED_ON     MY_GPIO_ON(LED_Class_RED)
#define RED_OFF    MY_GPIO_OFF(LED_Class_RED)
#define RED_Toggle MY_GPIO_Toggle(LED_Class_RED)





