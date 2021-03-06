#include <stdio.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include "mclient.h"
#include "mclient_callout.h"
#include "mmap.h"

#define REQ_READ    (1)		/* Read  Request */
#define REQ_WRITE   (2)		/* Write Request */
#define REQ_TIME    (4)		/* Time  Request */

#define U_MEMORY_SIZE	(8)	/* Shared memory size(long) */

typedef union
{
	unsigned long d[U_MEMORY_SIZE];
	struct
	{
		long   op;				/* server->ecu  Operation Request          */
		long   req;				/* server->ecu  Read Write Request         */
		long   mem_idx;			/* server->ecu  ECU Memory index Request   */
		long   w_val;			/* server->ecu  Write Value Request        */
		long   res_r_val;		/* server<-ecu  Read  Value Response       */
		double curr_req_time;	/* server->ecu  Current Request Time(s)    */
		double res_ecu_time;	/* server<-ecu  ECU Time(s) Response       */
		long   res;				/* server<-ecu  ECU Operetion Response     */
	} Memory;
} U_Memory;

U_Memory *ShmPTR = NULL;
key_t    ShmKEY  = MKEY;
int      ShmID   = 0;
int      fin = 0;
double   res_ecu_time = 0;

static void p_mem(void);
static unsigned long read_val(unsigned long idx, int *ret);
static void write_val(unsigned long idx, unsigned long write_val, int *ret, unsigned long *read_back);

int mclient_init(void);
int mclient_main(void);

int mclient_init(void)
{
	int err=0;
	int i;
	int ret=0;

	res_ecu_time = 0;

	printf("hello envci world! 20210504\n");
	fflush(stdout);

	ShmID = shmget(ShmKEY, sizeof(U_Memory), IPC_CREAT | IPC_EXCL | 0666);
	if (ShmID < 0) 
	{
		/* for case of error does not return a valid shmid */
		err = errno;
		printf("Error getting shared memory id %d %d\n",ShmID,err);
		fflush(stdout);
		if(err == EEXIST)
		{
			printf("memory exist for this key\n");
		}

		ShmID = shmget(ShmKEY, sizeof(U_Memory), 0666);
		ret = -1;
	}

	/* attach shared memory */
	ShmPTR = (U_Memory *) shmat(ShmID, NULL, 0);
	if ((long) ShmPTR == -1) {
		printf("***	 shmat error (server) ***\n");
		fflush(stdout);
		ret = -1;
	}

	/* If Shared Memory normally created */
	if(!ret)
	{
		/* Initializing Shared Memory */
		for(i=0; i<U_MEMORY_SIZE; i++)
		{
			ShmPTR->d[i] = 0;
		}

		printf( "Call the InitFunction\n" );
		fflush(stdout);
		mclient_initcallout();
	}

	return ret;
}

int mclient_main(void)
{
	int ret=0;

	long   op;
	long   req;
	long   mem_idx;
	long   w_val;
	long   res_r_val;
	double curr_req_time;
	char   res;

	op            = ShmPTR->Memory.op;
	req           = ShmPTR->Memory.req;
	mem_idx       = ShmPTR->Memory.mem_idx;
	w_val         = ShmPTR->Memory.w_val;
	curr_req_time = ShmPTR->Memory.curr_req_time;
	res           = ShmPTR->Memory.res;

	if( (op)&&(!res) )  /* Operation Request exists and response not yet */
	{

		if( ( REQ_READ  == req   )   /* Read Request  */
		||  ( REQ_WRITE == req   )   /* Write Request */
		||  ( REQ_TIME  == req ) )   /* Time Request  */
		{
			printf( "req time:%f ecu time:%f \n", curr_req_time, res_ecu_time);
			fflush(stdout);
			while( curr_req_time > res_ecu_time )	/* current request time > current time(ECU software) */
			{
				printf( "Call the MainFunction\n" );
				fflush(stdout);
				mclient_callout();
				res_ecu_time += MAIN_CYCLE;
				ShmPTR->Memory.res_ecu_time = res_ecu_time;
				printf( "req time:%f ecu time:%f \n", curr_req_time, res_ecu_time);
			}
		}
		else
		{
			/* Do Nothing */
		}

		if( REQ_READ == req )         /* Read Request */
		{
			printf("READ Request addr %lx \n", mem_idx);
			res_r_val = read_val(mem_idx, &ret);
			printf("read response:%lx ret:%d\n", res_r_val, ret);
			fflush(stdout);
			ShmPTR->Memory.res_ecu_time = res_ecu_time;
			ShmPTR->Memory.res_r_val = res_r_val;
		}
		else if( REQ_WRITE == req )   /* Write Request */
		{
			printf("WRITE Request addr %ld \n", mem_idx);
			fflush(stdout);
			write_val(mem_idx, w_val, &ret, &res_r_val);
			ShmPTR->Memory.res_ecu_time = res_ecu_time;
			ShmPTR->Memory.res_r_val = res_r_val;
		}
		else
		{
			/* Do Nothing */
		}
		if(!ret)
		{
			ShmPTR->Memory.res = 1;
		}
		else
		{
			ShmPTR->Memory.res = -1;
		}

		p_mem();
	}

	return fin;
}

static void p_mem(void)
{
	int  i;

	for(i=0; i<U_MEMORY_SIZE; i++)
	{
		printf("%lx: %lx\n", i, ShmPTR->d[i]);
	}
	fflush(stdout);
}

static unsigned long read_val(unsigned long idx, int *ret)
{
	unsigned long *p;
	unsigned long lp;
	unsigned long mask;
	unsigned long read_val=0;

	if( idx < VAL_LIST_NUM )
	{
		lp   = val_list[idx].addr;
		mask = val_list[idx].mask;
		printf("%lx adder:%lx mask:%lx \n", idx, lp, mask);

		p=(unsigned long*)lp;

		printf("p: %lx\n", *p);

		read_val = (*p)&(~mask);
		printf("read val: %lx\n", read_val);
		*ret = 0;
		fflush(stdout);
	}
	else{
		*ret = -1;
	}

	return read_val;
}

static void write_val(unsigned long idx, unsigned long write_val, int *ret, unsigned long *read_back)
{
	unsigned long *p;
	unsigned long lp;
	unsigned long mask;
	unsigned long read_val;

	if( idx < VAL_LIST_NUM )
	{
		lp   = val_list[idx].addr;
		mask = val_list[idx].mask;
		printf("idx:%lx adder:%lx mask:%lx \n", idx, lp, mask);

		p=(unsigned long*)lp;

		printf("*p: %lx\n", *p);

		read_val = (*p)&(~mask);
		printf("read val: %lx\n", read_val);

		write_val = ((*p)&(mask)) | (write_val&(~mask));
		printf("write val: %lx\n", write_val);

		*p = write_val;

		read_val   = (*p)&(~mask);
		*read_back = read_val;
		printf("read val: %lx\n", read_val);

		*ret = 0;
		fflush(stdout);
	}
	else{
		*ret = -1;
	}
}
