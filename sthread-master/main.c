


#include <stdio.h>
#include <stdlib.h>
#include "sthread.h"

#define LOOP 400
sthread_mutex_t m;
volatile int sum=0;
void f1()
{
        int i;
        for(i=0;i<LOOP;i++)
        {
                sthread_mutex_lock(m);
                sum+=1;
                printf("sum1 %d\n",sum);
                sthread_mutex_unlock(m);
                usleep(TIMESLOT/10);
        }
        printf("fun1 finished\n");
}
void f2()
{

        int i=0;
        for(i=0;i<LOOP;i++)
        {
                sthread_mutex_lock(m);
                sum+=1;
                printf("sum2----------- %d\n",sum);
                sthread_mutex_unlock(m);
                usleep(TIMESLOT/10);
        }
        printf("fun2 finished\n");
}
void f3()
{
        int i;
        for(i=0;i<LOOP;i++)
        {
                sthread_mutex_lock(m);
                sum+=1;
                printf("sum3----------- %d\n",sum);
                sthread_mutex_unlock(m);
                usleep(TIMESLOT/10);
        }
        printf("fun3 finished\n");
}

int main(void)
{
        printf("executing main function yeasss!!!!!!!\n");
        sthread_t p,q,r;
        sthread_mutex_init(&m);
        sthread_create(&p,f1,NULL);
        sthread_create(&q,f2,NULL);
        sthread_create(&r,f3,NULL);
        sthread_join(-1);
        printf("\nmain finished ans: %d \n",sum);
        return 0;
}
