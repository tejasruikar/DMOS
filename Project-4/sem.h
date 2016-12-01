// (c) Partha Dasgupta 2009
// permission to use and distribute granted.

#include "threads.h"

typedef struct semaphore_t {
     int count;
	 TCB_t *Queue;
} Semaphore_t;


/* 
 * Creates a new semaphore with a counter and queue
 */
Semaphore_t* CreateSem(int i)
{   
	Semaphore_t* s;
	s = (Semaphore_t*)malloc(sizeof(Semaphore_t));
	if(!s){
		printf("Mem error!\n");	
		return NULL;
	}
    s->count = i;
	s->Queue = newQueue();
	return s;
}


/*
 * The P routine decrements the semaphore, and if the value is less than
 * zero then blocks the process 
 */
void P(Semaphore_t *sem)
{   
	sem->count--;
	//printf("Count: %d\n", sem->count);
    if (sem->count < 0){
		TCB_t *Prev_Thread;
		AddQueue(sem->Queue, Curr_Thread);
		ucontext_t curr_context;
		Prev_Thread = Curr_Thread;
    	Curr_Thread = DelQueue(ReadyQ);	
		swapcontext(&(Prev_Thread->context),&(Curr_Thread->context));
	}
}


/*
 * The V routine increments the semaphore, and if the value is 0 or
 * negative, deletes TCB from semaphore queue & adds it to ReadyQueue and yields
 */

void V(Semaphore_t * sem)
{   
	TCB_t *Prev_Thread;
    sem->count++;
    if (sem->count <= 0) {
		AddQueue(ReadyQ, DelQueue(sem->Queue));
    }
	yield();
}


