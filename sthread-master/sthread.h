#ifndef __STHREAD_H__
#define __STHREAD_H__

#include "sinclude.h" //system required header files
#include "slimits.h"// macros defined
#include "squeue.h" //task queue and task related fn and data structure
#include "sutils.h" //signal block and release fn
#include "ssync.h" //mutex fn and data structures

int main(void);

void scheduler(int argc);
void init_thread(void);
void initmain(void) __attribute__ ((constructor)); //constructor added so that this function runs before main always
//By passing the constructor attribute to a function, the function will be called before the main function of the C program.
int sthread_create(sthread_t *tid,void(*fun)(void),void *args);
int sthread_exit(void);
int sthread_join(sthread_t sid);

#endif
