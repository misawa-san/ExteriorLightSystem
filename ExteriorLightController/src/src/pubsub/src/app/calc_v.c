#include <adc.h>
#include <calc_v.h>

uint16_t calc_v_env_light = 0U;   /* ENV_LIGHT : unit mV */
uint16_t calc_v_igv       = 0U;   /* IGV       : unit mV */

void        calc_v_step(void);
void        calc_v_set_env_light(uint16_t env_light);
uint16_t    calc_v_get_env_light(void);
void        calc_v_set_igv(uint16_t igv);
uint16_t    calc_v_get_igv(void);

static void calc_v_ad2env_light(void)
{
    uint32_t val = 0UL;

    val = adc_get_env_light();

    if(val < 1024UL)
    {
        val = CALC_V_ENV_LIGHT_LSB * val;
        if(val > 65535UL)
        {
            val = 65535UL;
        }
        calc_v_set_env_light((uint16_t)val);
    }
    else
    {
        /* 前回値保持 */
    }
}

static void calc_v_ad2igv(void)
{
    uint32_t val = 0UL;

    val = adc_get_igv();

    if(val < 1024UL)
    {
        val = CALC_V_IGV_LSB * val;
        if(val > 65535UL)
        {
            val = 65535UL;
        }
        calc_v_set_igv((uint16_t)val);
    }
    else
    {
        /* 前回値保持 */
    }
}

void calc_v_step(void)
{
    calc_v_ad2env_light();
    calc_v_ad2igv();
}

void calc_v_set_env_light(uint16_t env_light)
{
    calc_v_env_light = env_light;
}

uint16_t calc_v_get_env_light(void)
{
    return (calc_v_env_light);
}

void calc_v_set_igv(uint16_t igv)
{
    calc_v_igv = igv;
}

uint16_t calc_v_get_igv(void)
{
    return (calc_v_igv);
}
