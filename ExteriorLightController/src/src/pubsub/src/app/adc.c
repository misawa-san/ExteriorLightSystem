#include "Arduino.h"
#include "pina.h"
#include "adc.h"

static uint16_t adc_env_light = 0L;   /* ENV_LIGHT : 0-1023 */
static uint16_t adc_igv       = 0L;   /* IGV       : 0-1023 */

void     adc_step(void);
void     adc_set_env_light(uint16_t igv);
uint16_t adc_get_env_light(void);
void     adc_set_igv(uint16_t igv);
uint16_t adc_get_igv(void);


void adc_step(void)
{
    uint16_t val = 0L;

    val = analogRead(ENV_LIGHT);
    adc_env_light = val;

    val = analogRead(IGV);
    adc_igv = val;
}

void adc_set_env_light(uint16_t env_light)
{
    adc_env_light = env_light;
}

uint16_t adc_get_env_light(void)
{
    return (adc_env_light);
}

void adc_set_igv(uint16_t igv)
{
    adc_igv = igv;
}

uint16_t adc_get_igv(void)
{
    return (adc_igv);
}
