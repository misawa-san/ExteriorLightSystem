#ifndef _MMAP_H_

#include <stdio.h>

/* ------------------ USER CUSTOIMIZE ----------------------------*/
#define VAL_LIST_NUM (12)
/* ------------------ USER CUSTOIMIZE ----------------------------*/

struct Val
{
	unsigned long addr;
	unsigned long mask;
	char name[64];
};

extern struct Val val_list[VAL_LIST_NUM];

#endif
