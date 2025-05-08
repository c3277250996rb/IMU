#ifndef __ENCODER_H
#define __ENCODER_H

void encoder_init(void);
int16_t encoder1_get(void);
int16_t encoder2_get(void);

#define encoder_pin_1           GPIO_Pin_6
#define encoder_pin_2           GPIO_Pin_7

#endif
