#ifndef __SQUEUE_H__
#define __SQUEUE_H__

#include "sinclude.h"

typedef int sthread_t;
struct Task
{
        int priority;
        struct Task *next;
        sthread_t thread_id;
        ucontext_t context;
};

struct Mqueue
{
        struct Task *head;
        struct Task *tail;
        int count;
};

struct Task* copy_task(struct Task *t);
void clear_queue(struct Mqueue *q);
void display(struct Mqueue *q);
void insert(struct Mqueue* q,struct Task *task);
int isempty(struct Mqueue *q);
struct Task* top(struct Mqueue *q);
void pop(struct Mqueue *q);
struct Task* next(struct Mqueue *q,struct Task *task);
void remov(struct Mqueue *q,int globalid);

#endif
