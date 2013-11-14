/*****************************************************
                       cpeer.c
             peer client implementation
       Authors: Mihir Joshi, Fenil Kavathia
                     csc573 NCSU
*****************************************************/

#include "sock/csock.h"
#include "sock/ssock.h"
#include<string.h>
#include "parser/parse.h"
#include<signal.h>

#define SERVER_PORT 7752
#define SERVER_ADDR "127.0.0.1"
#define BUFSIZE 256

int ssock;
int csock;
int port;
int pid;

void s_peer(int outpipe);
void c_peer(int inpipe);

void segv(int signum)
{
  int status;

  close_sock(ssock);

  printf("\npress ctrl + c to exit\n");

  wait(&status);

  signal(signum, SIG_DFL);
  kill(getpid(), signum);
}

int main(int argc, char **argv)
{
 int pfd[2];

 pipe(pfd);

//now we need to fork the process

 signal(SIGINT, segv);

pid=fork();

if(pid==0)
	{
		close(pfd[0]);
		s_peer(pfd[1]);
	}
else
	{
		close(pfd[1]);
		c_peer(pfd[0]);
	}
}

void s_peer(int outpipe)
{
	int cssock;
	char buf[BUFSIZE];
	struct sockaddr_in addr;
	
	socklen_t addr_len;
	char myport[50], myhostname[50];

	//port=(rand()%(65535-1024))+1024;
	//right now we need to know the port no. so assigned fix one
	port=5000;
	//server socket(speer)
	ssock=get_sock();

	#ifdef GRAN1
	printf("[gran 1] created socket\n");
	#endif

	// bind socket
  	bind_sock(ssock, port);

	#ifdef GRAN1
		printf("[gran 1] bind socket\n");
	#endif

	addr_len = sizeof(addr);

	if(getsockname(ssock, (struct sockaddr * )&addr, &addr_len) == -1)
	{
		printf("[error] not able to get socket name\n");
		return;
	}

	sprintf(myport, "%d", ntohs(addr.sin_port));
   	/*sprintf(myhostname, "%d.%d.%d.%d", 
	(int)(addr.sin_addr.s_addr&0xFF), 
	    (int)((addr.sin_addr.s_addr&0xFF00)>>8),
		    (int)((addr.sin_addr.s_addr&0xFF00)>>16),
			    (int)((addr.sin_addr.s_addr&0xFF00)>>24));

	*/

	read_my_ip(myhostname);

	printf("sending port: %s host: %s\n", myport, myhostname);

	write_to(outpipe, myport, 50);
	
	write_to(outpipe, myhostname, 50);

	close(outpipe);

	  //listen now   " I am not sure whether this should be done in child process (speer process) or here itself"
	listen_sock(ssock);

	#ifdef GRAN1
	printf("[gran 1] now listening\n");
	#endif

	while(1)
        {
    	  // ready to accept connection
	  //communication with one peer completed

    	  cssock = accept_con(ssock);

          //connected to a peer client		
	  read_from(cssock, buf, BUFSIZE);

#ifdef APP
   printf("\n[log] Peer client says: %s\n", buf);
#endif
		
#ifdef APP
#endif
	  // echo reply to client
	  write_to(cssock, buf, BUFSIZE);
                
          //closing the communication with Peer server 
		
	  close_sock(cssock);   //not sure whether we need to close this socket or not

	}
}

void c_peer(int inpipe)
{
 	int choice, i;
	char buf1[BUFSIZE];
	char myport[50], myhostname[50];
	int rfc;
	char title[50];

	char c_rfc[50], host[50], port[50];

	//client socket (cpeer);
	//get client socket descriptor  
	csock = get_sock();

	read_from(inpipe, myport, 50);
	read_from(inpipe, myhostname, 50);

	close(inpipe);

	printf("received port: %s host: %s\n", myport, myhostname);

	//connect to server socket
	//connect_to(sock, SERVER_ADDR, SERVER_PORT);  //this is for communication between cpeer and server
	//right now assume we are using this function to connect  to server peer 
	connect_to(csock, SERVER_ADDR, SERVER_PORT);   //used 5000 port because we don't know server peer port

	do
	{
	memset(buf1,0,BUFSIZE);	
	printf("\nPress 1 for Add\n");
	printf("Press 2 for Lookup\n");
	printf("Press 3 for list\n");	
	printf("Press 4 for Terminate: ");
	scanf("%d",&choice);

	switch(choice)
		{
		case 1:
			printf("\nenter rfc number: ");
			scanf("%d", &rfc);

			printf("\nenter title: ");
			scanf("%s", title);

			generate_request(buf1, ADD, rfc, myhostname, myport, title); 
		break;
		case 2:
			generate_request(buf1, LOOKUP, 1000, myhostname, myport, "sample");
		break;
		case 3:	
			generate_request(buf1, LISTALL, 1000, myhostname, myport, "sample"); 
		break;
		case 4:			
        	printf("Thank you for using Program\n");
		close_sock(csock);
		raise(SIGINT);
		return;
		break;
 		
		}
	

	 write_to(csock, buf1, BUFSIZE);

	 read_from(csock, buf1, BUFSIZE);

 	if(choice == 2)
	{
	i = 0;
 	while(parse_response(buf1, i, c_rfc, title, host, port) != -1)
	{
	   printf("\nI understand that rfc %s is with %s:%s\n", c_rfc, host, port);
	   i++;
	}
	}

	 printf("\n%s\n", buf1);

	 //return 0;	

	}while(choice!=4);
}
