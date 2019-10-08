#ifndef __SSYNC_H__
#define __SSYNC_H__

#include "sinclude.h"
#include "sutils.h"
#include "slimits.h"
#include "squeue.h"


typedef int sthread_mutex_t;
typedef int sthread_cond_t;

struct mutex
{
	sthread_mutex_t mid;
	struct Mqueue waitq;
	int value;
	int num_waiting;
};

struct condition
{
	sthread_cond_t cid;
	sthread_mutex_t mid;
	struct Mqueue waitq;
	int num_waiting;
};


int sthread_mutex_init(sthread_mutex_t *mid);
int sthread_mutex_lock(sthread_mutex_t mid);
int sthread_mutex_unlock(sthread_mutex_t mid);
int sthread_mutex_destroy(sthread_mutex_t mid);

int sthread_cond_init(sthread_cond_t *cid);
int sthread_cond_wait(sthread_cond_t cid,sthread_mutex_t mid);
int sthread_cond_signal(sthread_cond_t cid);
int sthread_cond_broadcast(sthread_cond_t cid);
int sthread_cond_destroy(sthread_cond_t cid);

#endif