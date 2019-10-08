
//subject: main sthread controller. Execution starts here.
//       ( in function initmain )
/////////////////////////////////////

#include "sthread.h"

int thread_count=0;
int terminated=0;
struct Task tpool[MAX_THREAD];
ucontext_t delete[MAX_THREAD];
int suspend[MAX_THREAD];
int allocation[MAX_THREAD];
struct itimerval malarm;
struct sigaction scsignal;
struct Mqueue readyq;
struct Task *current_thread=NULL;

void initmain(void) //runs before the main function ..creates main thread
{
    freopen("output.txt","w",stdout);
	init_thread();
	sthread_t Main;
	current_thread=&tpool[0];
   	sthread_create(&Main,(void (*)(void))main,NULL);
   	scheduler(0);
}

void scheduler(int argc)
{
    //signal_block();

    if(debug)
    {
            printf("Scheduler: entered scheduler. Bacha gelo!!!!!!!!!!!!\n");
            isempty(&readyq);
            display(&readyq);// prints all tasks ids
    }

    struct Task *new = next(&readyq,current_thread);  //next thread to be chosen depends
    while(new!=NULL && suspend[new->thread_id]==1)new=next(&readyq,new);
    												//on scheduling algorithm used
    if(new==NULL)
    {
    	if(debug)
        {
        	printf("no next thread\n");
		}

        if(terminated==1)
        {

            if(debug)
            {
                    printf("schedulerr called after terminated thread %d %d: ",argc,current_thread->thread_id);
            }
            thread_count--;
            allocation[current_thread->thread_id]=0;
            terminated=0;
            remov(&readyq,current_thread->thread_id);
            if(debug)
            {
                    printf("removed. readyq display:\n");
                    display(&readyq);
                    printf("stopping execution\n");
            }
            current_thread=NULL;
            ///////All threads completed execution////////////
            //signal_release();
            exit(0);
        } //if(terminated==1) end here
        else
        {
        	printf("setting context to same thread\n");
            ///keep executing the same thread
            //signal_release();
            setcontext(&current_thread->context);
        }
    }//if(new==NULL )end here
    else
    {

        if(debug)
        {
            printf("next thread to be executed: %d\n",new->thread_id);
        }

        struct Task *old=current_thread;
        if(terminated==1)
        {

            if(debug)
            {
                printf("terminated thread %d %dand called schedulerr : ",argc,current_thread->thread_id);
            }
            thread_count--;
            allocation[current_thread->thread_id]=0;
            terminated=0;
			remov(&readyq,old->thread_id);
			///////////if main exits all exits/////////////
            if(old->thread_id==0)
            {
            	exit(0);
            }
            //////////////////////
            if(debug)
            {
                    printf("removed. readyq display:\n");
                    display(&readyq);
                    ///exit(1);/////////remove later//////
            }
            current_thread=new;
            //signal_release();
            setcontext(&new->context);
        } //if(terminated==1) end here
        else
        {
            if(debug)
            {
                    printf("swapping context from %d to %d\n",old->thread_id,new->thread_id);
            }
            current_thread=new;
            //signal_release();
            swapcontext(&old->context,&new->context);
        }
    } // else new!=null ends here

    if(debug)
    {
            printf("leaving schedulerr.\n");
    }
}

void init_thread(void)
{
    if(debug)
    {
            printf("entered init_thread\n");
    }

    scsignal.sa_handler=scheduler;
    sigemptyset(&scsignal.sa_mask);
    scsignal.sa_flags=0;
    ////////setting sigaction for
    sigaction(SIGPROF,&scsignal,NULL);
    readyq.count=0;
    int i;
		for(i=0;i<MAX_THREAD;i++)allocation[i]=0;

    malarm.it_value.tv_sec=0;
    malarm.it_value.tv_usec=TIMESLOT;
    malarm.it_interval.tv_sec=0;
    malarm.it_interval.tv_usec=TIMESLOT;

    if(debug)
    {
           printf("left init_thread\n");
    }

    return;
}

int sthread_create(sthread_t *tid,void(*fun)(void),void *args)
{

    if(debug)
    {
            printf("entered sthread_create\n");
    }

    int i=0;
    while(i<MAX_THREAD && allocation[i]!=0)
            i++;

    if(debug)
    {
            printf("sthread_create: id alloted to thread: %d\n",i);
    }

    if(i==MAX_THREAD)
    {
            printf("ERROR:max threads created\n");
            tid=NULL;
            return -1;
    }

    struct Task *task=&tpool[i];
	allocation[i]=1;
	*tid=i;

	if(getcontext(&task->context)==-1)
    {
            printf("ERROR: threadcontext error\n");
            tid=NULL;
            return -1;
    }

    if(getcontext(&delete[i])==-1)
    {
            printf("ERROR: deletecontext error");
            tid=NULL;
            return -1;
    }o
    else
	{

        task->context.uc_stack.ss_sp=malloc(STACK_SIZE);
        task->context.uc_stack.ss_size=STACK_SIZE;
        task->context.uc_stack.ss_flags=0;
        task->context.uc_link=&delete[i];
        
        
        task->next=NULL;
        task->thread_id=i;

        delete[i].uc_link=0;
        delete[i].uc_stack.ss_sp=malloc(STACK_SIZE);
        delete[i].uc_stack.ss_size=STACK_SIZE;
        delete[i].uc_stack.ss_flags=0;

        makecontext(&delete[i],(void*)&sthread_exit,0);
        makecontext(&task->context,fun,0);
        //should run after getcontext and after setting stack pointer and stack allocation
        //when activated by swapcontext or setcontext executes the provided fn

        if(thread_count==0)//first thread so set timer
        {
            if(debug)
            {
                    printf("first thread setting timer\n");
            }

            if(setitimer(ITIMER_PROF,&malarm,NULL)==-1)
                    perror("ERROR: itimer check\n");
        }
        thread_count++;

        /////////insert thread to ready queue//////////
        signal_block();
        insert(&readyq,task);
        if(debug)
        {
                printf("displaying current ready queue: ");
                display(&readyq);
        }
        signal_release();
    }
    if(debug)
    {
        printf("left sthread_create\n");
    }
  	return 0;
}


int sthread_exit(void)
{

    if(debug)
    {
            printf("entered sthread_exit.thread killed: %d\n",current_thread->thread_id);
    }

    terminated=1;
    scheduler(current_thread->thread_id);
    return 0;
    if(debug)
    {
            printf("left sthread_exit\n");
    }
}

int sthread_join(sthread_t tid)
{
	if(tid==-1)
	{
		while(thread_count>1)
		{
			printf("waiting all thread to sthread_join %d\n",thread_count);
            scheduler(tid);
		}
	}
	else
	{
		while(allocation[tid]>0)
		{
			printf("waiting  thread  %d to sthread_join\n",tid);
            scheduler(tid);
		}
	}
	return 0;
}
