#ifndef _MCLIENT_CALLOUT_
#define _MCLIENT_CALLOUT_

/* ------------------ USER CUSTOIMIZE ----------------------------*/
#define MAIN_CYCLE (0.02) /* 20ms cycle   */
#define MMAP_DUMP_CYCLE (0.000001)  /* 1us cycle */
/* ------------------ USER CUSTOIMIZE ----------------------------*/

extern void mclient_initcallout(void);
extern void mclient_callout(void);

#endif
