#ifndef ARDUINO_H
#define ARDUINO_H

#include <stdint.h>

extern uint16_t adc_data[256];
extern int      pwm_data[256];

extern int  analogRead(uint8_t pin);
extern void analogWrite(uint8_t pin, int val);

#endif
