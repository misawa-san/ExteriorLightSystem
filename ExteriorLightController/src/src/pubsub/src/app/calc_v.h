#ifndef CALC_BAT_V_H
#define CALC_BAT_V_H

#include <stdint.h>

#define CALC_V_ENV_LIGHT_LSBGAIN  (10UL)  /* 1LSB GAIN:10          */
#define CALC_V_ENV_LIGHT_LSB      (49UL)  /* 1LSB:49[mV] / LSBGAIN */
#define CALC_V_IGV_LSBGAIN        (10UL)  /* 1LSB GAIN:10          */
#define CALC_V_IGV_LSB            (49UL)  /* 1LSB:49[mV] / LSBGAIN */

extern void        calc_v_step(void);
extern void        calc_v_set_env_light(uint16_t env_light);
extern uint16_t    calc_v_get_env_light(void);
extern void        calc_v_set_igv(uint16_t igv);
extern uint16_t    calc_v_get_igv(void);

#endif
