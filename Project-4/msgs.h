#include <stdlib.h>
#include "sem.h"

#define N 10


struct port{
	Semaphore_t* mutex;
	Semaphore_t* full;
	Semaphore_t* empty;
	int in;
	int out;
	int messages[N][N];
}p[100];



void initPort(){
	int i;
	for(i = 0; i<100; i++){
		p[i].mutex = CreateSem(1);
		p[i].full = CreateSem(0);
		p[i].empty = CreateSem(N);
		p[i].in = 0;
		p[i].out = 0;
	}
}


int send(int portNo, int buffer[]){ // write equivalent to port
	P(p[portNo].empty);
          P(p[portNo].mutex);
          memcpy(p[portNo].messages[p[portNo].in], buffer, N*sizeof(int));
		  p[portNo].in = (p[portNo].in+1) % N;
          V(p[portNo].mutex);
	V(p[portNo].full);
	return 0;
}



int receive(int portNo, int buffer[]){ // read equivalent from port
	P(p[portNo].full);
          P(p[portNo].mutex);
		  memcpy(buffer, p[portNo].messages[p[portNo].out], N*sizeof(int));
          p[portNo].out = (p[portNo].out+1) % N;
          V(p[portNo].mutex);
	V(p[portNo].empty);
	return 0;
}

