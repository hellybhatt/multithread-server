#include<pthread.h>
#include<sys/select.h>
#include<fcntl.h>
#include<errno.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<sys/utsname.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<getopt.h>
#include<sys/time.h>
#include<sys/resource.h>

pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
void *ThreadMain(void *arg);
void *Threadudp(void *arg);
void exitSysWithError(const char *call){
	fprintf(stderr, "Syscall %s failed with errno= %d: %s\n", call, errno,
	 strerror(errno));
	exit(-1);
}
void exitWithError(){
	printf("Please provide valid arguments\n");
	exit(-1);
}
int max(int x, int y) 
{ 
    if (x > y) 
        return x; 
    else
        return y; 
}
void HandleTCPClient(int clntSocket)
{
    char echoBuffer[1024];        /* Buffer for echo string */
    int recvMsgSize;                    /* Size of received message */
    char stuff[1000]={'\0'};

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, 1024, 0)) < 0)
        exitSysWithError("recv()");
	printf("message size = %d\n",recvMsgSize);
    if(recvMsgSize == 1 || recvMsgSize == 0){
	char *nocmd = "Please pass the commands";
                strcpy(stuff, nocmd);
	if (send(clntSocket, stuff, sizeof(stuff), 0) < 0)
            exitSysWithError("send()");
	close(clntSocket);
	//break;
	}
    
	else{
    echoBuffer[recvMsgSize]=0;
    char *cmd = (char *)&echoBuffer;
	struct utsname buff1;
	errno =0; 
   	if(uname(&buff1)!=0) 
   	{
      		perror("uname doesn't return 0, so there is an error"); 
      		exitSysWithError("uname()"); 
   	}
	if (strstr(cmd, "s") != NULL|| strstr(cmd,"kernel-name")!= NULL ||strstr(cmd,"-s") != NULL || strstr(cmd, "--kernel-name") !=NULL)
			{
				strcpy(stuff, buff1.sysname);
				printf("systemName=%s\n",buff1.sysname);
			}
		if (strstr(cmd, "n") != NULL|| strstr(cmd,"nodename")!= NULL ||strstr(cmd,"-n") != NULL || strstr(cmd, "--nodename") !=NULL)
			{
				if(stuff != NULL)
					{
					strcat(stuff," ");
					strcat(stuff,buff1.nodename); 
				}
				else{
				strcpy(stuff, buff1.nodename);
				printf("NodeName=%s\n",buff1.nodename);}
			}
		if (strstr(cmd, "r") != NULL|| strstr(cmd,"kernel-release")!= NULL ||strstr(cmd,"-r") != NULL || strstr(cmd, "--kernel-release") !=NULL)
			{
				if(stuff != NULL)
					{
					strcat(stuff," ");
					strcat(stuff,buff1.release); 
					}
				else{
				strcpy(stuff, buff1.release);
				printf("Release=%s\n",buff1.release);}
			}
		if (strstr(cmd, "v") != NULL|| strstr(cmd,"kernel-version")!= NULL ||strstr(cmd,"-v") != NULL || strstr(cmd, "--kernel-version") !=NULL)
			{
				if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,buff1.version); 
				}
				else{
				strcpy(stuff, buff1.version);
				printf("version=%s\n",buff1.version);}
			}
		if (strstr(cmd, "m") != NULL|| strstr(cmd,"--machine")!= NULL ||strstr(cmd,"-m") != NULL || strstr(cmd, "--machine") !=NULL)
			{
				if(stuff != NULL)
					{
					strcat(stuff," ");
					strcat(stuff,buff1.machine); 
					}
				else{
				strcpy(stuff, buff1.machine);
				printf("machine=%s\n",buff1.machine);}
			}
		if (strstr(cmd, "p") != NULL|| strstr(cmd,"processor")!= NULL ||strstr(cmd,"-p") != NULL || strstr(cmd, "--processor") !=NULL)
			{
				if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,buff1.machine); 
				}
				else{
				strcpy(stuff, buff1.machine);
				printf("processor=%s\n",buff1.machine);}
			}
		if (strstr(cmd, "i") != NULL|| strstr(cmd,"--hardware-platform")!= NULL ||strstr(cmd,"-i") != NULL || strstr(cmd, "--hardware-platform") !=NULL)
			{
				if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,buff1.machine); 
				}
				else{
				strcpy(stuff, buff1.machine);
				printf("platform=%s\n",buff1.machine);}
			}
	if ((strstr(cmd, "h") != NULL)|| (strstr(cmd,"help")!= NULL) ||
	(strstr(cmd,"-h") != NULL) || (strstr(cmd, "--help") !=NULL) ||
	 	(cmd == "h") || (cmd =="--help") || (cmd == "-h") || (cmd == "help"))
			{
			printf("in help");
			FILE *infile;
			long numbytes;
			infile = fopen("help.txt","r");
			fseek(infile, 0, SEEK_END);
			numbytes = ftell(infile);
			//printf("%ld",numbytes);
			fseek(infile, 0, SEEK_SET);
			//stuff = 
			//fread(stuff, sizeof(char), numbytes, infile);
			fread(stuff,1000,numbytes,infile);
			fclose(infile);
			printf("Help:%s",stuff);
			//fclose(infile);
		}
		if (strstr(cmd, "o") != NULL|| strstr(cmd,"operating-system")!= NULL ||strstr(cmd,"-o") != NULL || strstr(cmd, "--operating-system") !=NULL)
			{
			char *os = "GNU-Linux";
			if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,os); 
				}
			else{
			strcpy(stuff, os);
			printf("operating system=GNU-Linux\n");}
			}
		if (strstr(cmd, "asn") != NULL|| strstr(cmd,"-asn")!= NULL || 
		strstr(cmd,"all")!= NULL || strstr(cmd,"--all")!= NULL ||
		strstr(cmd,"-a")!= NULL || strstr(cmd,"a")!= NULL)
			{
				strcpy(stuff,buff1.sysname);
				strcat(stuff," ");
				strcat(stuff,buff1.nodename);
				strcat(stuff," ");
				strcat(stuff,buff1.release);
				strcat(stuff," ");
				strcat(stuff,buff1.version);
				strcat(stuff," ");
				strcat(stuff,buff1.machine);
				printf("ALL=%s\n",stuff);
			}
	
	int stuflen = strlen(stuff);
	if(stuflen != 0 && stuff != NULL && stuff != " " && stuff != "")
	{}	//{printf("end point will be:%s:",stuff);}
	else{
		char *novalue = "Please provide valid commands";
		strcpy(stuff, novalue);
	}
        if (send(clntSocket, stuff, sizeof(stuff), 0) < 0)
            exitSysWithError("send()");
    close(clntSocket);    /* Close client socket */
   }
}
void HandleUDPthreadClient(int udpsock)
{
    //printf("In udp handle\n");
    char echoBuffer[30];        /* Buffer for echo string */
    int recvMsgSize;                    /* Size of received message */
    char stuff[1000]={'\0'};
    struct sockaddr_in cliaddr;
    int clilen = sizeof(cliaddr);
    if ((recvMsgSize = recvfrom(udpsock, echoBuffer, sizeof(echoBuffer), MSG_WAITALL,(struct sockaddr*)&cliaddr, &clilen)) < 0)
       exitSysWithError("recvfrom()");
	printf("message size = %d\n",recvMsgSize);
    if(recvMsgSize == 1 || recvMsgSize == 0){
	char *nocmd = "Please pass the commands";
                strcpy(stuff, nocmd);
	if (sendto(udpsock, stuff, sizeof(stuff), 0,(struct sockaddr*)&cliaddr,sizeof(cliaddr)) < 0)
            exitSysWithError("send()");
	close(udpsock);
	//break;
	}
else{
    echoBuffer[recvMsgSize]=0;
    char *cmd = (char *)&echoBuffer;
	struct utsname buff1;
	errno =0; 
   	if(uname(&buff1)!=0) 
   	{
      		perror("uname doesn't return 0, so there is an error"); 
      		exitSysWithError("uname()"); 
   	}
	if (strstr(cmd, "s") != NULL|| strstr(cmd,"kernel-name")!= NULL ||strstr(cmd,"-s") != NULL || strstr(cmd, "--kernel-name") !=NULL)
			{
				strcpy(stuff, buff1.sysname);
				printf("systemName=%s\n",buff1.sysname);
			}
		if (strstr(cmd, "n") != NULL|| strstr(cmd,"nodename")!= NULL ||strstr(cmd,"-n") != NULL || strstr(cmd, "--nodename") !=NULL)
			{
				if(stuff != NULL)
					{
					strcat(stuff," ");
					strcat(stuff,buff1.nodename); 
				}
				else{
				strcpy(stuff, buff1.nodename);
				printf("NodeName=%s\n",buff1.nodename);}
			}
		if (strstr(cmd, "r") != NULL|| strstr(cmd,"kernel-release")!= NULL ||strstr(cmd,"-r") != NULL || strstr(cmd, "--kernel-release") !=NULL)
			{
				if(stuff != NULL)
					{
					strcat(stuff," ");
					strcat(stuff,buff1.release); 
					}
				else{
				strcpy(stuff, buff1.release);
				printf("Release=%s\n",buff1.release);}
			}
		if (strstr(cmd, "v") != NULL|| strstr(cmd,"kernel-version")!= NULL ||strstr(cmd,"-v") != NULL || strstr(cmd, "--kernel-version") !=NULL)
			{
				if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,buff1.version); 
				}
				else{
				strcpy(stuff, buff1.version);
				printf("version=%s\n",buff1.version);}
			}
		if (strstr(cmd, "m") != NULL|| strstr(cmd,"--machine")!= NULL ||strstr(cmd,"-m") != NULL || strstr(cmd, "--machine") !=NULL)
			{
				if(stuff != NULL)
					{
					strcat(stuff," ");
					strcat(stuff,buff1.machine); 
					}
				else{
				strcpy(stuff, buff1.machine);
				printf("machine=%s\n",buff1.machine);}
			}
		if (strstr(cmd, "p") != NULL|| strstr(cmd,"processor")!= NULL ||strstr(cmd,"-p") != NULL || strstr(cmd, "--processor") !=NULL)
			{
				if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,buff1.machine); 
				}
				else{
				strcpy(stuff, buff1.machine);
				printf("processor=%s\n",buff1.machine);}
			}
		if (strstr(cmd, "i") != NULL|| strstr(cmd,"--hardware-platform")!= NULL ||strstr(cmd,"-i") != NULL || strstr(cmd, "--hardware-platform") !=NULL)
			{
				if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,buff1.machine); 
				}
				else{
				strcpy(stuff, buff1.machine);
				printf("platform=%s\n",buff1.machine);}
			}
	if ((strstr(cmd, "h") != NULL)|| (strstr(cmd,"help")!= NULL) ||
	(strstr(cmd,"-h") != NULL) || (strstr(cmd, "--help") !=NULL) ||
	 	(cmd == "h") || (cmd =="--help") || (cmd == "-h") || (cmd == "help"))
			{
			printf("in help");
			FILE *infile;
			long numbytes;
			infile = fopen("help.txt","r");
			fseek(infile, 0, SEEK_END);
			numbytes = ftell(infile);
			//printf("%ld",numbytes);
			fseek(infile, 0, SEEK_SET);
			//stuff = 
			//fread(stuff, sizeof(char), numbytes, infile);
			fread(stuff,1000,numbytes,infile);
			fclose(infile);
			printf("Help:%s",stuff);
			//fclose(infile);
		}
		if (strstr(cmd, "o") != NULL|| strstr(cmd,"operating-system")!= NULL ||strstr(cmd,"-o") != NULL || strstr(cmd, "--operating-system") !=NULL)
			{
			char *os = "GNU-Linux";
			if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,os); 
				}
			else{
			strcpy(stuff, os);
			printf("operating system=GNU-Linux\n");}
			}
		if (strstr(cmd, "asn") != NULL|| strstr(cmd,"-asn")!= NULL || 
		strstr(cmd,"all")!= NULL || strstr(cmd,"--all")!= NULL ||
		strstr(cmd,"-a")!= NULL || strstr(cmd,"a")!= NULL)
			{
				strcpy(stuff,buff1.sysname);
				strcat(stuff," ");
				strcat(stuff,buff1.nodename);
				strcat(stuff," ");
				strcat(stuff,buff1.release);
				strcat(stuff," ");
				strcat(stuff,buff1.version);
				strcat(stuff," ");
				strcat(stuff,buff1.machine);
				printf("ALL=%s\n",stuff);
			}
	int stuflen = strlen(stuff);
	if(stuflen != 0 && stuff != NULL && stuff != " " && stuff != "")
	{}	//{printf("end point will be:%s:",stuff);}
	else{
		char *novalue = "Please provide valid commands";
		strcpy(stuff, novalue);
	}
	 if (sendto(udpsock, stuff, sizeof(stuff), 0,(struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0)
           exitSysWithError("sendto()");
	//close(udpsock);
	exit(0);
   }
}
struct ThreadArgs
{
    	int sock; 
	int udpfd;                     /* Socket descriptor for client */
};
void tcpserver(int port,struct sockaddr_in ina){
	pthread_t threadID;
        struct ThreadArgs *threadArgs;
	int maxfdp1;
	fd_set rset;
	int sock;
	int udpb;
	char buff[100]={'\0'};
	struct sockaddr_in cli;
	int s = socket(AF_INET, SOCK_STREAM,0);
	if (s == -1){
		printf("Socket connection failed\n");
		exitSysWithError("socket()");
	}

	int length = sizeof(ina);
	int b = bind(s, (struct sockaddr *)&ina , sizeof(ina));
	if(b == -1){
		printf("binding fails\n");
		exitSysWithError("bind()");
	}
	getsockname(s,(struct sockaddr *)&ina, &length);
	int udpfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (port == 0){
		printf("Listening on port: %d\n",ntohs(ina.sin_port));
		struct sockaddr_in bnd;
        bnd.sin_family = AF_INET;
        bnd.sin_addr.s_addr = htonl(INADDR_ANY);
        bnd.sin_port = htons(ntohs(ina.sin_port));
	udpb = bind(udpfd, (struct sockaddr*)&bnd, sizeof(bnd));
	}
	else{
    		udpb = bind(udpfd, (struct sockaddr*)&ina, sizeof(ina));}
	FD_ZERO(&rset);
	maxfdp1 = max(s,udpfd) + 1;
	int l = listen(s,5);
	if (l==-1){
		exitSysWithError("listen()");
	}
	for(;;){
		FD_SET(s, &rset);
		FD_SET(udpfd, &rset);
		int nready = select(maxfdp1, &rset, NULL, NULL, NULL);
		if(FD_ISSET(s, &rset)){
		int len  = sizeof(cli);
		sock = accept(s, (struct sockaddr *)&cli , &len);
		if (sock <0){
			exitSysWithError("accept()");
		}
		//printf("Connection Accepted\n");
		if ((threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs))) 
               		== NULL)
				exitSysWithError("malloc()");
		threadArgs->sock = sock;
		//printf("malloc successful\n");
		if (pthread_create(&threadID, NULL, &ThreadMain, (void *)threadArgs) != 0)
            		exitSysWithError("pthread_create()");
        	//printf("with thread %ld\n", (long int) threadID);
		}
		if(FD_ISSET(udpfd, &rset)){
		int ulen = sizeof(cli);
		//printf("Within UDP");
		
		if ((threadArgs = (struct ThreadArgs *) 
			malloc(sizeof(struct ThreadArgs))) == NULL)
                                exitSysWithError("malloc()");
                threadArgs->udpfd = udpfd;
                //printf("malloc successful\n");
                if (pthread_create(&threadID, NULL, &Threadudp, (void *)threadArgs) != 0)
                        exitSysWithError("pthread_create()");
                //printf("with thread %ld\n", (long int) threadID);
		}
	
    	}
}
void *Threadudp(void *threadArgs){
	//printf("In thread main udp\n");
    	int udpfd;
	 pthread_detach(pthread_self()); 
    	udpfd = ((struct ThreadArgs *) threadArgs) -> udpfd;
    	free(threadArgs);
//int pthread_mutex_lock (pthread_mutex_t *mutex);
    HandleUDPthreadClient(udpfd);
//int pthread_mutex_unlock (pthread_mutex_t *mutex);

    	//HandleUDPthreadClient(udpfd,0);
	//close(udpfd);
    	return (NULL);

}
void *ThreadMain(void *threadArgs)
{
   //printf("In thread main\n");
    int sock;
    pthread_detach(pthread_self()); 
    sock = ((struct ThreadArgs *) threadArgs) -> sock;
    free(threadArgs);
//int pthread_mutex_lock (pthread_mutex_t *mutex);
    HandleTCPClient(sock);
//int pthread_mutex_unlock (pthread_mutex_t *mutex);
    return (NULL);
}
int main(int argc, char** argv)
{

	int opt;
	char *portname;
	int portnumber;
	portnumber = 0;
	
	while ((opt = getopt(argc, argv, "p:t:u:")) != -1){
		switch(opt){
		case 'p':
			for(int i=0; i<argc; i++){
				if(strcmp(argv[i],"-port") == 0)
					portnumber =(int)atol(argv[i+1]);
			}
			//printf("portnumber%d",portnumber);
			break;
		case 't':
			portname= "tcp";
			//printf("IN TCP portname%s\n",portname);
			break;
		case 'u':
			portname = "udp";
			//printf("portname%s\n",portname);
			break;
		}
	}
	//printf("portnumber outside%d\n",portnumber);
	struct servent *sp;
	struct sockaddr_in ina;
	ina.sin_family = AF_INET;
	ina.sin_addr.s_addr = htonl(INADDR_ANY);
	ina.sin_port = htons(portnumber);
	//if(portname == "tcp" || portname == "udp"){
		tcpserver(portnumber,ina);
	//}
	//else if (portname == "udp") {
	//	udpserver(portnumber,ina);
	//}
}
