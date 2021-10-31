#include "Arduino.h"

uint16_t adc_data[256];
int      pwm_data[256];

int analogRead(uint8_t pin)
{
    return adc_data[pin];
}

void analogWrite(uint8_t pin, int val)
{
    pwm_data[pin] = val;
}
