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
    /* address,                                        mask,                    name */
/* ------------------------------- USER CUSTOIMIZE ---------------------------------------------------------*/
    { (unsigned long)(&fin),                           0xFFFFFFFF00000000,        "fin"                          },    /* 0:  fin                    */
    { (unsigned long)(&res_ecu_time),                  0x0000000000000000,        "res_ecu_time"                 },    /* 1:  res_ecu_time           */
    { (unsigned long)(&pwm_data[PARKINGLIGHT]),        0xFFFFFFFF00000000,        "pwm_data.PARKINGLIGHT"        },    /* 2:  pwm_data[PARKINGLIGHT] */
    { (unsigned long)(&pwm_data[PASSINGLIGHT]),        0xFFFFFFFF00000000,        "pwm_data.PASSINGLIGHT"        },    /* 3:  pwm_data[PASSINGLIGHT] */
    { (unsigned long)(&payload[0]),                    0xFFFFFFFFFFFFFF00,        "payload.0"                    },    /* 4:  payload[0]             */
    { (unsigned long)(&payload[1]),                    0xFFFFFFFFFFFFFF00,        "payload.1"                    },    /* 5:  payload[1]             */
    { (unsigned long)(&payload[2]),                    0xFFFFFFFFFFFFFF00,        "payload.2"                    },    /* 6:  payload[2]             */
    { (unsigned long)(&payload[3]),                    0xFFFFFFFFFFFFFF00,        "payload.3"                    },    /* 7:  payload[3]             */
    { (unsigned long)(&payload[4]),                    0xFFFFFFFFFFFFFF00,        "payload.4"                    },    /* 8:  payload[4]             */
    { (unsigned long)(&payload[5]),                    0xFFFFFFFFFFFFFF00,        "payload.5"                    },    /* 9:  payload[5]             */
    { (unsigned long)(&payload[6]),                    0xFFFFFFFFFFFFFF00,        "payload.6"                    },    /* 10: payload[6]             */
    { (unsigned long)(&payload[7]),                    0xFFFFFFFFFFFFFF00,        "payload.7"                    }     /* 11: payload[7]             */
/* ------------------ USER CUSTOIMIZE ----------------------------*/
};
