#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "q.h"

//Declare two variables ReadyQ and Curr_Thread
//I'm not sure about this....
static TCB_t *Curr_Thread;
static TCB_t *ReadyQ;
static TCB_t thread;

void start_thread(void (*function)(void))
{
    TCB_t *context;
    context=malloc(sizeof(TCB_t));
    stack_t *stackP=malloc(8192*sizeof(stack_t));
    init_TCB(context,function,stackP,8192*sizeof(stack_t));
    thread.thread_id++;
    printf("Thread %d is created\n",thread.thread_id);
    if(ReadyQ==NULL){ //checks whether Run Queue is not initialised
       ReadyQ = newQueue();
    }
    
      item = NewItem(item);
      item->context=context->context;
      AddQueue(ReadyQ,item);      
}

void run()
{
    Curr_Thread = DelQueue(ReadyQ);
    ucontext_t curr_context;
    getcontext(&curr_context);//gets the current context	
    swapcontext(&curr_context,&(Curr_Thread->context));//swaps main with the first thread function
}

void yield()
{
    TCB_t *Prev_Thread;
    AddQueue(ReadyQ,Curr_Thread);
    ucontext_t curr_context;	
    Prev_Thread= Curr_Thread;
    Curr_Thread = DelQueue(ReadyQ);	
	swapcontext(&(Prev_Thread->context),&(Curr_Thread->context));
}
