#include "msgs.h"
static int port = 0;
Semaphore_t *mutex;
static int servers = 0;
static int clients = 0;
int serverPorts[99]={0};
time_t t;
void server ()
{
	int count = 0;
	int portNo;
	int replyTo = 0;
	int bufferS[10] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	int obtainedS[10];
	int serverNo;
	P(mutex);
		portNo = port++;
		serverPorts[servers] = portNo;
		serverNo = servers++;
	V(mutex);
	while(1){
		if(portNo > 99){
			printf("(Server) Ports not available...closing!\n");
			break;
		}
		receive(portNo, obtainedS);
		while(count < 10){
			printf("Server %d, data: %d\n", serverNo, obtainedS[count]);
			count++;		
		}
		printf("\n\n");
		replyTo = obtainedS[0];
		
		count = 0;
		while(count < 10){
			++bufferS[count];
			++count;		
		}	
		count = 0;
		sleep(1);	
		send(replyTo, bufferS);
	}
}


void client ()
{
	int count = 0;
	int portNo;
	int clientNo;
	int serverPortNo = serverPorts[rand() % servers];
	int bufferC[10] = {41, 42, 43, 44, 45, 46, 47, 48, 49, 50};
	int obtainedC[10];
	P(mutex);
		portNo = port++;
		clientNo = clients++;
	V(mutex);

	while(1){
		if(portNo > 99){
			printf("(Client)Ports not available...closing!\n");
			break;
		}
		printf("\n\n");
		bufferC[0] = portNo;
		send(serverPortNo, bufferC);
		while(count < 10){
			++bufferC[count];
			++count;		
		}
		sleep(1);
		receive(portNo, obtainedC);
		count = 0;
		while(count < 10){
			printf("Client %d, data: %d\n", clientNo, obtainedC[count]);
			count++;		
		}
		count = 0;
	}
}


int main(){
	srand((unsigned)time(&t));
	initPort();
	mutex = CreateSem(1);
	start_thread(server);
	start_thread(server);
    	start_thread(client);
	start_thread(client);
	start_thread(client);
   	run();
	while(1){
		sleep(1);
	}
}
