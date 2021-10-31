#ifdef __cplusplus
extern "C" {
#endif

#ifndef _MCLIENT_H_
#define _MCLIENT_H_

/* ------------------ USER CUSTOIMIZE ----------------------------*/
#define MKEY    (123456)	/* Shared Memory Key */
/* ------------------ USER CUSTOIMIZE ----------------------------*/

extern int mclient_init(void);
extern int mclient_main(void);

#endif

#ifdef __cplusplus
}
#endif

