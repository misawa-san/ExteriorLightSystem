#include "mmap.h"
/* ------------------ USER CUSTOIMIZE ----------------------------*/
#include "pina.h"
#include "Arduino.h"

extern int fin;
extern double res_ecu_time;
extern volatile uint8_t payload[8];
extern uint16_t calc_v_env_light;
extern uint16_t calc_v_igv;
/* ------------------ USER CUSTOIMIZE ----------------------------*/


struct Val val_list[VAL_LIST_NUM] = 
{
	/* address,						mask */
/* ------------------ USER CUSTOIMIZE ----------------------------*/
	{ (unsigned long)(&fin),						0xFFFFFFFF00000000 },	/* 0:  fin                 */
	{ (unsigned long)(&res_ecu_time),			0x0000000000000000 },	/* 1:  res_ecu_time        */
	{ (unsigned long)(&adc_data[ENV_LIGHT]),	0xFFFFFFFFFFFF0000 },	/* 2:  adc_data[ENV_LIGHT] */
	{ (unsigned long)(&adc_data[IGV]),			0xFFFFFFFFFFFF0000 },	/* 3:  adc_data[IGV]       */
	{ (unsigned long)(&payload[0]),				0xFFFFFFFFFFFFFF00 },	/* 4:  payload[0]          */
	{ (unsigned long)(&payload[1]),				0xFFFFFFFFFFFFFF00 },	/* 5:  payload[1]          */
	{ (unsigned long)(&payload[2]),				0xFFFFFFFFFFFFFF00 },	/* 6:  payload[2]          */
	{ (unsigned long)(&payload[3]),				0xFFFFFFFFFFFFFF00 },	/* 7:  payload[3]          */
	{ (unsigned long)(&payload[4]),				0xFFFFFFFFFFFFFF00 },	/* 8:  payload[4]          */
	{ (unsigned long)(&payload[5]),				0xFFFFFFFFFFFFFF00 },	/* 9:  payload[5]          */
	{ (unsigned long)(&payload[6]),				0xFFFFFFFFFFFFFF00 },	/* 10: payload[6]          */
	{ (unsigned long)(&payload[7]),				0xFFFFFFFFFFFFFF00 },	/* 11: payload[7]          */
	{ (unsigned long)(&calc_v_env_light),		0xFFFFFFFFFFFF0000 },	/* 12: calc_v_env_light    */
	{ (unsigned long)(&calc_v_igv),				0xFFFFFFFFFFFF0000 }		/* 13: calc_v_igv          */
/* ------------------ USER CUSTOIMIZE ----------------------------*/
};
