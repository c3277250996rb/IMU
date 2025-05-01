#include "my_led.h"


GPIO_Class_Struct LED_Class_BLUE = {
    .Pin = LED_BLUE_PC13_Pin,
	.Port = LED_BLUE_PC13_GPIO_Port,
    .ON_Status = 0,
    .OFF_Status = 1
};

GPIO_Class_Struct LED_Class_GREEN = {
    .Pin = LED_GREEN_PB9_Pin,
	.Port = LED_GREEN_PB9_GPIO_Port,
    .ON_Status = 0,
    .OFF_Status = 1
};

GPIO_Class_Struct LED_Class_YELLOW = {
    .Pin = LED_YELLOW_PB13_Pin,
	.Port = LED_YELLOW_PB13_GPIO_Port,
    .ON_Status = 0,
    .OFF_Status = 1
};

GPIO_Class_Struct LED_Class_RED = {
    .Pin = LED_RED_PB1_Pin,
	.Port = LED_RED_PB1_GPIO_Port,
    .ON_Status = 0,
    .OFF_Status = 1
};


// Functions for BLUE LED
#if !BLUE
GPIO_PinState BLUE_Read(void) {
    return MY_GPIO_Read(LED_Class_BLUE);
}

void BLUE_ON(void) {
    MY_GPIO_ON(LED_Class_BLUE);
}

void BLUE_OFF(void) {
    MY_GPIO_OFF(LED_Class_BLUE);
}

void BLUE_Toggle(void) {
    MY_GPIO_Toggle(LED_Class_BLUE);
}
#endif

// Functions for GREEN LED
#if !GREEN
GPIO_PinState GREEN_Read(void) {
    return MY_GPIO_Read(LED_Class_GREEN);
}

void GREEN_ON(void) {
    MY_GPIO_ON(LED_Class_GREEN);
}

void GREEN_OFF(void) {
    MY_GPIO_OFF(LED_Class_GREEN);
}

void GREEN_Toggle(void) {
    MY_GPIO_Toggle(LED_Class_GREEN);
}
#endif

// Functions for YELLOW LED
#if !YELLOW
GPIO_PinState YELLOW_Read(void) {
    return MY_GPIO_Read(LED_Class_YELLOW);
}

void YELLOW_ON(void) {
    MY_GPIO_ON(LED_Class_YELLOW);
}

void YELLOW_OFF(void) {
    MY_GPIO_OFF(LED_Class_YELLOW);
}

void YELLOW_Toggle(void) {
    MY_GPIO_Toggle(LED_Class_YELLOW);
}
#endif

// Functions for RED LED
#if !RED
GPIO_PinState RED_Read(void) {
    return MY_GPIO_Read(LED_Class_RED);
}

void RED_ON(void) {
    MY_GPIO_ON(LED_Class_RED);
}

void RED_OFF(void) {
    MY_GPIO_OFF(LED_Class_RED);
}

void RED_Toggle(void) {
    MY_GPIO_Toggle(LED_Class_RED);
}
#endif

