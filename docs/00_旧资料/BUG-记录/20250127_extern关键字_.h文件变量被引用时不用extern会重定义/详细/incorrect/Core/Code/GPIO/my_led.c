#include "my_led.h"



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

