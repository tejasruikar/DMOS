// Team members: Saurabh Jagdhane (1209572595), Sandesh Shetty (1209395990)
// CSE-531 (DMOS) [Project-2]
// Compile using: gcc thread_test.c
// Run using: ./a.out

#include <stdio.h>
#include <stdlib.h>
#include "threads.h"

int global_var=0;

void fun1()
{
    static int local_var1 = 0;
    while(1)
    {
        global_var++;
        local_var1++;
        printf("f1: global_variable = %d and local_variable = %d \n",global_var,local_var1);
	sleep(1);
        yield();
    }
}

void fun2()
{
    static int local_var2=0;
    while(1)
    {
        global_var++;
        local_var2++;
        printf("f2: global_variable = %d and local_variable = %d \n",global_var,local_var2);
	sleep(1);
        yield();
    }
}

void fun3()
{
    static int local_var3=0;
    while(1)
    {
        global_var++;
        local_var3++;
        printf("f3: global_variable = %d and local_variable = %d \n",global_var,local_var3);
	sleep(1);
        yield();
    }
}


int main(int argc, char *argv)
{
    start_thread(fun1);
    start_thread(fun2);
    start_thread(fun3);	
    run();
}
