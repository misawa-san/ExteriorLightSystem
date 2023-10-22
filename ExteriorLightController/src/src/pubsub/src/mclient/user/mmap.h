#ifndef _MMAP_H_

#include <stdio.h>

/* ------------------ USER CUSTOIMIZE ----------------------------*/
#define VAL_LIST_NUM (14)
/* ------------------ USER CUSTOIMIZE ----------------------------*/

struct Val
{
	unsigned long addr;
	unsigned char size;
	unsigned long mask;
	char name[64];
};

extern struct Val val_list[VAL_LIST_NUM];

#endif
