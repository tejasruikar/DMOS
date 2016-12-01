// Team members: Saurabh Jagdhane, Sandesh Shetty
// CSE-531 (DMOS) [Project-1]
// Compile using: gcc -pthread proj-1.c -o p1
// Run using: ./p1

#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

semaphore_t mutex, child1, child2, child3;

int arr[3]={0,0,0};

void child_1(int *arg)
{

    while (1){
        P(&child1);
        //sleep(1);
        ++(*arg);
        V(&child2);
    }
}    

void child_2(int *arg)
{

    while (1){
        P(&child2);
        //sleep(1);
        ++(*arg);
        V(&child3);
    }
}    

void child_3(int *arg)
{

    while (1){
        P(&child3);
        //sleep(1);
	++(*arg);
        V(&mutex);
    }
}  


int main(){
  
  init_sem(&mutex,1);
  init_sem(&child1,0);
  init_sem(&child2,0);
  init_sem(&child3,0);
  start_thread(child_1, &arr[0]);
  start_thread(child_2, &arr[1]);
  start_thread(child_3, &arr[2]);
	
  while(1) {
   P(&mutex);
   printf("Contents of array: %d %d %d\n", arr[0], arr[1], arr[2]);
   sleep(1); //Infinite loop
   V(&child1);
}

  return 0;
}
	
