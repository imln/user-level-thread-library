
//subject: queue implemented as round robin manner
//         used to store Task structures
/////////////////////////////////////

#include "squeue.h"

struct Task* copy_task(struct Task *t)
{
        struct Task *temp=(struct Task*)malloc(sizeof(struct Task));
        temp->priority=t->priority;
        temp->thread_id=t->thread_id;
        temp->next=NULL;
        temp->context=t->context;
        return temp;

}
void clear_queue(struct Mqueue *q)
{
        if(q!=NULL)
        {
                q->head=NULL;
                q->tail=NULL;
                q->count=0;
        }
}
void display(struct Mqueue *q)
{
        struct Task *hd=q->head;
        while(hd!=NULL)
        {
                printf("threadid: %d\n",hd->thread_id);
                hd=hd->next;
        }
        return;
}

void insert(struct Mqueue* q,struct Task *task)
{
        if(q==NULL||task==NULL)
                perror("q or task null");
        else if(q->count==0)
        {
                q->head=task;
                q->tail=q->head;
                q->tail->next=NULL;
                q->count++;
        }
        else
        {
                q->tail->next=task;
                q->tail=q->tail->next;
                q->tail->next=NULL;
                q->count++;
        }
        return;
}
int isempty(struct Mqueue *q)
{
        printf("q->count is %d \n",q->count);
        return (q->count==0);
}


struct Task* top(struct Mqueue *q)
{
        if(q!=NULL)
        return q->head;
        else return NULL;
}

void pop(struct Mqueue *q)
{
        if(q!=NULL)
        {
                q->head=q->head->next;
                q->count--;
        }
}


struct Task* next(struct Mqueue *q,struct Task *task)
{
        struct Task *hd=q->head;
        while(hd!=NULL && hd!=task)
        {
                hd=hd->next;

        }
        if(hd==NULL)
                return hd;
        else if(hd->next==NULL)
        {
                if(hd!=q->head)return q->head;
                else return NULL;
        }
        else return hd->next;
}
void remov(struct Mqueue *q,int globalid)
{
        struct Task *hd=q->head,*tmp=NULL;
        while(hd!=NULL)
        {
                if(hd->thread_id==globalid)
                {
                        break;
                }
                tmp=hd;
                hd=hd->next;
        }
        if(tmp==NULL)  //renmove head
                q->head=q->head->next;
        else tmp->next=hd->next;
        q->count--;
}
