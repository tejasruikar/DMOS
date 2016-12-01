#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TCB.h"

static TCB_t *ReadyQ =NULL;
TCB_t *item =NULL;

//Initialize the Queue
TCB_t * NewItem(TCB_t *item)
{
 	item = malloc(sizeof(TCB_t));
	item->next=NULL;
	item->prev=NULL;
	return item;
}

TCB_t *newQueue()
{
	ReadyQ=malloc(sizeof(TCB_t));
	ReadyQ->next=ReadyQ;
	ReadyQ->prev=ReadyQ;
	return ReadyQ;	
}

//Add elements to the Queue
void AddQueue(TCB_t *ReadyQ, TCB_t *node)
{
  //Checks whether queue is empty and if it is sets the header node of the queue
  if(ReadyQ->prev==ReadyQ && ReadyQ->next==ReadyQ)
  {
      ReadyQ->next=node;
      ReadyQ->prev=node;
      node->next=ReadyQ;
      node->prev=ReadyQ;
  }
//Adding elements to the queue
else
 {
    ReadyQ->prev->next =node;
    node->prev=ReadyQ->prev;
    ReadyQ->prev=node;
    node->next=ReadyQ;
 }
}


TCB_t* DelQueue(TCB_t *ReadyQ)
{
	TCB_t *temp=NULL;	

	if(ReadyQ==ReadyQ->prev && ReadyQ==NULL) 	// check for empty queue
	{
		printf("Cannot Delete from empty queue");
		return NULL;
	}
	else
	{
		temp=ReadyQ->next;
		ReadyQ->next=ReadyQ->next->next;
		ReadyQ->next->prev=ReadyQ;
	}
	
	return temp;
}

