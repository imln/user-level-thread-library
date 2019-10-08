#include "ssync.h"

extern struct Mqueue readyq;
extern struct Task *current_thread;
extern int suspend[MAX_THREAD];

int mallocation[MAX_MUTEX] = {0};
int callocation[MAX_COND] = {0};
struct mutex mpool[MAX_MUTEX];
struct condition cpool[MAX_COND];


int sthread_mutex_init(sthread_mutex_t *mid)
{
	int id=0;
	while(id < MAX_MUTEX && mallocation[id] > 0)id++;
	if(id >= MAX_MUTEX)
	{
		if(debug == 1)
		{
			printf(" max mutex allocated \n");
		}
		return -1;
	}
	else
	{
		if(debug == 1)
		{
			printf("\n mutex_init: id allocated %d\n",id);
		}
		mallocation[id]=1;
		*mid=id;
		mpool[id].mid=id;
		mpool[id].num_waiting=0;
		mpool[id].value=1;
		return 0;
	}
}

int sthread_mutex_lock(sthread_mutex_t mid)
{
	signal_block();
	if(debug)
	{
		printf("entered mutex lock\n");

	}

	if(mid>MAX_MUTEX || mid<0 ||  mallocation[mid]==0)
	{
		signal_release();
		return -1;
	}
	else
	{
		if(mpool[mid].value==1)
		{
			mpool[mid].value=0;
			if(debug)
			{
				printf("left mutex lock\n");
			}
			signal_release();
		}
		else
		{
			mpool[mid].num_waiting++;
			struct Task *temp = copy_task(current_thread);
			insert(&mpool[mid].waitq,temp);
			suspend[current_thread->thread_id]=1;
			if(debug)
			{
				printf("numwaiting is : %d and q content:\n",mpool[mid].num_waiting);
				display(&mpool[mid].waitq);
				printf("displaying contents of readyq :\n");
				display(&readyq);
				printf("left mutex lock\n");
			}
			signal_release();
			scheduler(current_thread->thread_id);
		}
	}
	return 0;
}

int sthread_mutex_unlock(sthread_mutex_t mid)
{
	signal_block();
	if(debug)
	{
		printf("enterd mutex unlock\n");
	}

	if(mid>MAX_MUTEX || mid<0 ||  mallocation[mid]==0)
	{
		signal_release();
		return -1;
	}
	else
	{
		if(mpool[mid].num_waiting == 0)
		{
			mpool[mid].value=1;
		}
		else
		{
			mpool[mid].value=1;
			mpool[mid].num_waiting--;
			struct Task *next= top(&mpool[mid].waitq);
			if(debug)
			{
				printf("next thread is : %d\n",next->thread_id);
			}
			pop(&mpool[mid].waitq);
			suspend[next->thread_id]=0;
		}
		if(debug)
		{
			printf("left mutex unlock\n");
		}
		signal_release();
		return 0;
	}
}

int sthread_mutex_destroy(sthread_mutex_t mid)
{
	if(mid>MAX_MUTEX || mid<0 ||  mallocation[mid]==0)
	{
		return -1;
	}
	else
	{
		mallocation[mid]=0;
		clear_queue(&mpool[mid].waitq);
		return 0;
	}
}

int sthread_cond_init(sthread_cond_t *cid)
{
	int id=0;
	while(id<MAX_COND && callocation[id]>0)id++;
	if(id >= MAX_COND)
	{
		if(debug)
		{
			printf("max allocation of condition variables\n");
		}
		return -1;
	}
	else
	{
		callocation[id]=1;
		*cid=id;
		cpool[id].num_waiting=0;
		cpool[id].cid=id;
		return 0;
	}
}
int sthread_cond_wait(sthread_cond_t cid,sthread_mutex_t mid)
{
	signal_block();
	if(cid<0 || cid >= MAX_COND || callocation[cid] == 1)
	{
		signal_release();
		return -1;
	}
	else
	{
		cpool[cid].mid = mid;
		suspend[current_thread->thread_id]=1;
		struct Task *temp = copy_task(current_thread);
		insert(&cpool[cid].waitq,temp);
		cpool[cid].num_waiting++;
		sthread_mutex_unlock(mid);
		signal_release();
		scheduler();
		return 0;
	}

}
int sthread_cond_signal(sthread_cond_t cid)
{
	signal_block();
	if(cid<0 || cid >= MAX_COND || callocation[cid] == 0)
	{
		signal_release();
		return -1;
	}
	else
	{
		struct Task *next = top(&cpool[cid].waitq);
		pop(&cpool[cid].waitq);
		suspend[next->thread_id]=0;
		cpool[cid].num_waiting--;
		sthread_mutex_lock(cpool[cid].mid);
		signal_release();
		return 0;
	}
}
int sthread_cond_broadcast(sthread_cond_t cid)
{
	signal_block();
	if(cid<0 || cid >= MAX_COND || callocation[cid] == 1)
	{
		signal_release();
		return -1;
	}
	else
	{
		struct Task *next = top(&cpool[cid].waitq);
		pop(&cpool[cid].waitq);
		sthread_mutex_lock(cpool[cid].mid);
		while(next !=NULL && cpool[cid].num_waiting>0)
		{
			suspend[next->thread_id]=0;
			cpool[cid].num_waiting--;
			next = top(&cpool[cid].waitq);
			pop(&cpool[cid].waitq);
		}
		signal_release();
		return 0;
	}

}
int sthread_cond_destroy(sthread_cond_t cid)
{
	if(cid<0 || cid >= MAX_COND || callocation[cid] == 1)
	{
		return -1;
	}
	else
	{
		callocation[cid]=0;
		clear_queue(&cpool[cid].waitq);
		return 0;
	}
}
