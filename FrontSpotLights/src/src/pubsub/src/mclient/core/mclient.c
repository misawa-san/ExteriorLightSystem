#include <stdio.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include "mclient.h"
#include "mclient_callout.h"
#include "mmap.h"

#define REQ_READ (1)  /* Read  Request */
#define REQ_WRITE (2) /* Write Request */
#define REQ_TIME (4)  /* Time  Request */

#define U_MEMORY_SIZE (8) /* Shared memory size(long) */

typedef union
{
    unsigned long d[U_MEMORY_SIZE];
    struct
    {
        long op;              /* server->ecu  Operation Request          */
        long req;             /* server->ecu  Read Write Request         */
        long mem_idx;         /* server->ecu  ECU Memory index Request   */
        long w_val;           /* server->ecu  Write Value Request        */
        long res_r_val;       /* server<-ecu  Read  Value Response       */
        double curr_req_time; /* server->ecu  Current Request Time(s)    */
        double res_ecu_time;  /* server<-ecu  ECU Time(s) Response       */
        long res;             /* server<-ecu  ECU Operetion Response     */
    } Memory;
} U_Memory;

U_Memory *ShmPTR = NULL;
key_t ShmKEY = MKEY;
int ShmID = 0;
int fin = 0;
double res_ecu_time = 0;
double res_ecu_time_pre = 0;

static void p_mem(void);
static void p_mem2(void);
static unsigned long read_val(unsigned long idx, int *ret);
static void write_val(unsigned long idx, unsigned long write_val, int *ret, unsigned long *read_back);

int mclient_init(void);
int mclient_main(void);
int mmap_dump(void);
int mmap_dumpf(char valname[], long w_val);

int mclient_init(void)
{
    int err = 0;
    int i;
    int ret = 0;

    res_ecu_time = 0;

    // printf("hello envci world! 20230611\n");
    fflush(stdout);

    ShmID = shmget(ShmKEY, sizeof(U_Memory), IPC_CREAT | IPC_EXCL | 0666);
    if (ShmID < 0)
    {
        ShmID = shmget(ShmKEY, sizeof(U_Memory), 0666);
        if (ShmID < 0)
        {
            /* for case of error does not return a valid shmid */
            err = errno;
            printf("Error getting shared memory id %d %d\n", ShmID, err);
            fflush(stdout);
            if (err == EEXIST)
            {
                printf("memory exist for this key\n");
                fflush(stdout);
            }

            if (shmctl(ShmID, IPC_RMID, NULL) == -1) {
                perror("shmctl");
            }
            ret = -1;
        }
    }
    if(!ret)
    {
        /* attach shared memory */
        ShmPTR = (U_Memory *)shmat(ShmID, NULL, 0);
        if ((long)ShmPTR == -1)
        {
            printf("***     shmat error (server) ***\n");
            fflush(stdout);
            ret = -1;
        }
    }

    /* If Shared Memory normally created */
    if (!ret)
    {
        /* Initializing Shared Memory */
        for (i = 0; i < U_MEMORY_SIZE; i++)
        {
            ShmPTR->d[i] = 0;
        }

        for (int i = 2; i < (sizeof(val_list) / sizeof(val_list[0])); i++)
        {
            printf(",%s", val_list[i].name);
        }

        // printf( "Call the InitFunction\n" );
        fflush(stdout);
        mclient_initcallout();
    }

    return ret;
}

int mclient_main(void)
{
    int ret = 0;

    long op;
    long req;
    long mem_idx;
    long w_val;
    long res_r_val;
    double curr_req_time;
    char res;

    op = ShmPTR->Memory.op;
    req = ShmPTR->Memory.req;
    mem_idx = ShmPTR->Memory.mem_idx;
    w_val = ShmPTR->Memory.w_val;
    curr_req_time = ShmPTR->Memory.curr_req_time;
    res = ShmPTR->Memory.res;

    if ((op) && (!res)) /* Operation Request exists and response not yet */
    {

        if (REQ_TIME == req) /* Time Request  */
        {
            // printf( "req time:%f ecu time:%f \n", curr_req_time, res_ecu_time);
            fflush(stdout);
            while (curr_req_time > res_ecu_time) /* current request time > current time(ECU software) */
            {
                // printf( "Call the MainFunction\n" );
                fflush(stdout);

                mclient_callout();
                p_mem2();
                {
                    double ecutime = res_ecu_time_pre + MAIN_CYCLE;
                    if (ecutime > res_ecu_time)
                    {
                        res_ecu_time = ecutime;
                    }
                    else
                    {
                        printf("too many use mmap_dump() or mmap_dumpf(). please reduce using mmap_dump() or mmap_dumpf()\n");
                    }
                }
                res_ecu_time_pre = res_ecu_time;

                ShmPTR->Memory.res_ecu_time = res_ecu_time;
                // printf( "req time:%f ecu time:%f \n", curr_req_time, res_ecu_time);
            }
        }
        else if (REQ_READ == req) /* Read Request */
        {
            // printf("READ Request addr %lx \n", mem_idx);
            res_r_val = read_val(mem_idx, &ret);
            // printf("read response:%lx ret:%d\n", res_r_val, ret);
            fflush(stdout);

            ShmPTR->Memory.res_ecu_time = res_ecu_time;
            ShmPTR->Memory.res_r_val = res_r_val;
        }
        else if (REQ_WRITE == req) /* Write Request */
        {
            // printf("WRITE Request addr %ld \n", mem_idx);
            fflush(stdout);
            write_val(mem_idx, w_val, &ret, &res_r_val);
            p_mem2();

            ShmPTR->Memory.res_ecu_time = res_ecu_time;
            ShmPTR->Memory.res_r_val = res_r_val;
        }
        else
        {
            /* Do Nothing */
        }

        if (!ret)
        {
            ShmPTR->Memory.res = 1;
        }
        else
        {
            ShmPTR->Memory.res = -1;
        }

        // p_mem();
    }

    return fin;
}

int mmap_dump(void)
{
    p_mem2();
    res_ecu_time += MMAP_DUMP_CYCLE;

    return 0;
}

int mmap_dumpf(char valname[], long w_val)
{
    int ret = -1;
    int loop;
    long res_r_val;

    for (loop = 0; loop < (sizeof(val_list) / sizeof(val_list[0])); loop++)
    {
        if (strcmp(valname, val_list[loop].name) == 0)
        {
            write_val(loop, w_val, &ret, &res_r_val);
            p_mem2();
            res_ecu_time += MMAP_DUMP_CYCLE;
            ret = 0;
            break;
        }
    }

    if (ret != 0)
    {
        printf("%s not found\n", valname);
    }

    return ret;
}

static void p_mem2(void)
{
    int ret = 0;

    printf("\n%f", res_ecu_time);
    for (int i = 2; i < (sizeof(val_list) / sizeof(val_list[0])); i++)
    {
        long temp;
        temp = read_val(i, &ret);
        printf(",%ld", temp);
    }
    fflush(stdout);
}

static void p_mem(void)
{
    int i;

    for (i = 0; i < U_MEMORY_SIZE; i++)
    {
        printf("%lx: %lx\n", i, ShmPTR->d[i]);
    }
    fflush(stdout);
}

static unsigned long read_val(unsigned long idx, int *ret)
{
    unsigned char *p1;
    unsigned short *p2;
    unsigned int *p4;
    unsigned long *p8;

    unsigned long lp;
    unsigned char size;
    unsigned long mask;
    unsigned long read_val = 0;

    if (idx < (sizeof(val_list) / sizeof(val_list[0])))
    {
        lp = val_list[idx].addr;
        size = val_list[idx].size;
        mask = val_list[idx].mask;

        switch (size)
        {
        case 1: /* 1byte */
            p1 = (unsigned char *)lp;
            read_val = (unsigned char)((*p1) & (~((0xFF) & mask)));
            break;

        case 2: /* 2byte */
            p2 = (unsigned short *)lp;
            read_val = (unsigned short)((*p2) & (~((0xFFFF) & mask)));
            break;

        case 4: /* 4byte */
            p4 = (unsigned int *)lp;
            read_val = (unsigned int)((*p4) & (~((0xFFFFFFFF) & mask)));
            break;

        case 8: /* 8byte */
        default:
            p8 = (unsigned long *)lp;
            read_val = (unsigned long)((*p8) & (~((0xFFFFFFFFFFFFFFFF) & mask)));
            break;
        }

        *ret = 0;
        fflush(stdout);
    }
    else
    {
        *ret = -1;
    }

    return read_val;
}

static void write_val(unsigned long idx, unsigned long write_val, int *ret, unsigned long *read_back)
{
    unsigned char *p1;
    unsigned short *p2;
    unsigned int *p4;
    unsigned long *p8;

    unsigned long lp;
    unsigned char size;
    unsigned long mask;
    unsigned long read_val;

    if (idx < (sizeof(val_list) / sizeof(val_list[0])))
    {
        lp = val_list[idx].addr;
        size = val_list[idx].size;
        mask = val_list[idx].mask;
        // printf("idx:%lx adder:%lx mask:%lx \n", idx, lp, mask);

        switch (size)
        {
        case 1: /* 1byte */
            p1 = (unsigned char *)lp;
            read_val = (*p1) & (~((0xFF) & mask));
            write_val = ((*p1) & ((0xFF) & mask)) | (((0xFF) & write_val) & (~((0xFF) & mask)));
            *p1 = (unsigned char)write_val;
            read_val = (unsigned char)((*p1) & (~((0xFF) & mask)));
            *read_back = read_val;
            break;

        case 2: /* 2byte */
            p2 = (unsigned short *)lp;
            read_val = (*p2) & (~((0xFFFF) & mask));
            write_val = ((*p2) & ((0xFFFF) & mask)) | (((0xFFFF) & write_val) & (~((0xFFFF) & mask)));
            *p2 = (unsigned short)write_val;
            read_val = (unsigned short)((*p2) & (~((0xFFFF) & mask)));
            *read_back = read_val;
            break;

        case 4: /* 4byte */
            p4 = (unsigned int *)lp;
            read_val = (*p4) & (~((0xFFFFFFFF) & mask));
            write_val = ((*p4) & ((0xFFFFFFFF) & mask)) | (((0xFFFFFFFF) & write_val) & (~((0xFFFFFFFF) & mask)));
            *p4 = (unsigned int)write_val;
            read_val = (unsigned int)((*p4) & (~((0xFFFFFFFF) & mask)));
            *read_back = read_val;
            break;

        case 8: /* 8byte */
        default:
            p8 = (unsigned long *)lp;
            read_val = (*p8) & (~((0xFFFFFFFFFFFFFFFF) & mask));
            write_val = ((*p8) & ((0xFFFFFFFFFFFFFFFF) & mask)) | (((0xFFFFFFFFFFFFFFFF) & write_val) & (~((0xFFFFFFFFFFFFFFFF) & mask)));
            *p8 = (unsigned int)write_val;
            read_val = (unsigned long)((*p8) & (~((0xFFFFFFFFFFFFFFFF) & mask)));
            *read_back = read_val;
            break;
        }

        *ret = 0;
        fflush(stdout);
    }
    else
    {
        *ret = -1;
    }
}
