#ifndef ADC_H
#define ADC_H

#include <stdint.h>

extern void     adc_step(void);
extern void     adc_set_env_light(uint16_t igv);
extern uint16_t adc_get_env_light(void);
extern void     adc_set_igv(uint16_t igv);
extern uint16_t adc_get_igv(void);

#endif
