// Team members: Saurabh Jagdhane (1209572595), Sandesh Shetty (1209395990)
// CSE-531 (DMOS) [Project-5]
// Compile using: gcc file_test.c
// Run using: ./a.out 5 f1 f2 f3 f4 f5

#include "msgs.h"

//static int clients = 0;
static int servers = 0;

static int clientPortNo = 0;
static int serverPortNo= 99;

//Requested filename by client
char *reqFileName[100];
Semaphore_t *mutex;

// Server Thread
void server ()
{
	int rc[10];
	int serverPort;
	char fileNameRecv[32];
	char fileData[36];
	int replyTo;
	FILE *fp[10];

	P(mutex);
		servers++;
		serverPort = serverPortNo;	
	V(mutex);

		while(1){
			receive(serverPort,rc);
			 
			if (rc[0]==-1){
				replyTo=rc[1];
				if (replyTo>2){
					continue;
				}
				memcpy(fileNameRecv,rc+2,32);
				if (strlen(fileNameRecv) > 15){
					continue;
				}
				strcat(fileNameRecv,".server");
				fp[replyTo] = fopen(fileNameRecv, "w");
			}else{
				replyTo=rc[0];
				if(fp[replyTo]){
					int size = ftell(fp[replyTo]);

					if (size>=1048576){
						int fd = fileno(fp[replyTo]); 
						// printf("File transfer exceeded 1 MB\n");
						continue;
					}

					if (replyTo>2){
						continue;
					}

					memcpy(fileData,rc+1,36);
					//printf("filesize: %d\n",filesize); 
					//printf("%s\n",fileData);
					fprintf(fp[replyTo], "%s",fileData);
					fseek(fp[replyTo],0,SEEK_CUR);
				}
			}
			//printf("RC: %d %s\n",replyTo, fileNameRecv);	
		}

	while(1){
		sleep();
	}
}


// Client Thread
void client ()
{
	FILE *fp;
	char buff[36];
	int c[10];
	int clientNumber;

	// Mutex are not necessary. 
	// But only to avoid writing client thread, number of times and assigning hardcoded port number. 
	P(mutex);
		clientNumber=clientPortNo++;
	V(mutex);

	while(1){
	if (servers>0){
		c[0]=-1;
		c[1]=clientNumber;
		memcpy(c+2,reqFileName[clientNumber],32);
		send(serverPortNo,c);
		printf("I'm client requesting file transfer: %s\n",c+2);
		break;
		}
	yield();
	}	

	fp=fopen(reqFileName[clientNumber],"r");

	c[0]=clientNumber;
	while(fgets(buff, 36,fp)){
		memcpy(c+1,buff,36);
		send(serverPortNo,c);
	}

	/* To inform which file transfer is completed. */
	if (clientNumber < 3){
		char path[1024]; 
	    	char result[1024];

	    	int fd = fileno(fp); 

		/* Read out the link to file descriptor. */
		sprintf(path, "/proc/self/fd/%d", fd);
		memset(result, 0, sizeof(result));
		readlink(path, result, sizeof(result)-1);

		/* Print the result. */
		printf("%s transfer attempted.\n", result);
	}

	fclose(fp);
	

	while(1){
		yield();	
	}

}

int main(int argc, char * argv[]){

	initPort();
	mutex = CreateSem(1);
	
	// Starting server
	start_thread(server);
	//printf("Number of args: %d\n", argc);
	printf("Number of clients requested for file transfer are: %s\n", argv[1]);	
	
	if (argc<3 && atoi(argv[1])>0){
		printf("***Missing filename***\n");
		exit(1);
	}
	
	int numberOfClients = atoi(argv[1]);
	int i;
	for(i=0; i<numberOfClients; i++){
		reqFileName[i] = (char *) malloc(strlen(argv[2+i])*sizeof(char));
		memcpy(reqFileName[i],argv[2+i],strlen(argv[2+i])*sizeof(char));
		start_thread(client);	
	}
	   
   	run();

	while(1){
		sleep(1);
	}
}
