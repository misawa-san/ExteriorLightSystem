#include "mmap.h"
/* ------------------ USER CUSTOIMIZE ----------------------------*/
#include "pina.h"
#include "Arduino.h"

extern int fin;
extern double res_ecu_time;
extern volatile uint8_t payload[8];
/* ------------------ USER CUSTOIMIZE ----------------------------*/


struct Val val_list[VAL_LIST_NUM] = 
{
    /* address,                                        size,              mask,                    name */
/* ------------------------------- USER CUSTOIMIZE ---------------------------------------------------------*/
    { (unsigned long)(&fin),                           4,                 0xFFFFFFFF00000000,        "fin"                          },    /* 0:  fin                    */
    { (unsigned long)(&res_ecu_time),                  8,                 0x0000000000000000,        "res_ecu_time"                 },    /* 1:  res_ecu_time           */
    { (unsigned long)(&pwm_data[PARKINGLIGHT]),        1,                 0xFFFFFFFF00000000,        "uint8_pwm_data.PARKINGLIGHT"  },    /* 2:  pwm_data[PARKINGLIGHT] */
    { (unsigned long)(&pwm_data[PASSINGLIGHT]),        1,                 0xFFFFFFFF00000000,        "sint16_pwm_data.PASSINGLIGHT" },    /* 3:  pwm_data[PASSINGLIGHT] */
    { (unsigned long)(&payload[0]),                    1,                 0xFFFFFFFFFFFFFF00,        "payload.0"                    },    /* 4:  payload[0]             */
    { (unsigned long)(&payload[1]),                    1,                 0xFFFFFFFFFFFFFF00,        "payload.1"                    },    /* 5:  payload[1]             */
    { (unsigned long)(&payload[2]),                    1,                 0xFFFFFFFFFFFFFF00,        "payload.2"                    },    /* 6:  payload[2]             */
    { (unsigned long)(&payload[3]),                    1,                 0xFFFFFFFFFFFFFF00,        "payload.3"                    },    /* 7:  payload[3]             */
    { (unsigned long)(&payload[4]),                    1,                 0xFFFFFFFFFFFFFF00,        "payload.4"                    },    /* 8:  payload[4]             */
    { (unsigned long)(&payload[5]),                    1,                 0xFFFFFFFFFFFFFF00,        "payload.5"                    },    /* 9:  payload[5]             */
    { (unsigned long)(&payload[6]),                    1,                 0xFFFFFFFFFFFFFF00,        "payload.6"                    },    /* 10: payload[6]             */
    { (unsigned long)(&payload[7]),                    1,                 0xFFFFFFFFFFFFFF00,        "payload.7"                    }     /* 11: payload[7]             */
/* ------------------ USER CUSTOIMIZE ----------------------------*/
};

