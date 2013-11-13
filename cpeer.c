/*****************************************************
                       cpeer.c
             peer client implementation
       Authors: Mihir Joshi, Fenil Kavathia
                     csc573 NCSU
*****************************************************/

#include "sock/csock.h"
#include "sock/ssock.h"
#include<string.h>

#define SERVER_PORT 7752
#define SERVER_ADDR "192.168.1.121"
#define BUFSIZE 256



int ssock;
int csock;
int port;
/*int main(int argc, char **argv)
{
 int sock;
 char buf[BUFSIZE];

 sock = get_sock();

 connect_to(sock, SERVER_ADDR, SERVER_PORT);

 printf("enter message: ");

 scanf("%s", buf);

 write_to(sock, buf, BUFSIZE);

 read_from(sock, buf, BUFSIZE);

 printf("\nserver replied: %s\n", buf);

 close_sock(sock);

 return 0;
}*/

int main(int argc, char **argv)
{
int cssock;
int pid;

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

  //listen now   " I am not sure whether this should be done in child process (speer process) or here itself"
  listen_sock(ssock);

#ifdef GRAN1
printf("[gran 1] now listening\n");
#endif



//client socket (cpeer);
//get client socket descriptor  
csock = get_sock();

//connect to server socket
//connect_to(sock, SERVER_ADDR, SERVER_PORT);  //this is for communication between cpeer and server
//right now assume we are using this function to connect  to server peer 
connect_to(csock, SERVER_ADDR, 5000);   //used 5000 port because we don't know server peer port

//now we need to fork the process

pid=fork();

if(pid==0)
	{
	char buf[BUFSIZE];
	//remove this variable after verification
	char temp[]="this is temporary reply from peer server";
	#ifdef APP
	 printf("[log] I am in server process of peer: %d", ssock);
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
		 printf("[log] writing to Peer client %s\n", temp);
		#endif

		 // echo reply to client
		 write_to(cssock, buf, BUFSIZE);
                
		//closing the communication with Peer server 
		
		close_sock(cssock);   //not sure whether we need to close this socket or not

		}

	}
else
	{
	int choice;
	char a[10]="ADD";
        char b[10]="LOOKP";
        char c[10]="LIST";
	char buf1[BUFSIZE];
	do
	{
	memset(buf1,0,256);	
	printf("choice is %d and buffer is %s",choice,buf1);
	printf("Press 1 for Add");
	printf("Press 2 for Lookup");
	printf("Press 3 for list");	
	printf("Press 4 for Terminate");
	scanf("%d",&choice);

	switch(choice)
		{
		case 1:
		strcpy(buf1,a);
		break;
		case 2:
		strcpy(buf1,b);
		break;
		case 3:
		strcpy(buf1,c);
		break;
		case 4:			
        	printf("Thank you for using Program");
		close_sock(csock);
		return 0;	
		break;
 		
		}
	

	 write_to(csock, buf1, BUFSIZE);

	 read_from(csock, buf1, BUFSIZE);

	 printf("\nserver replied: %s\n", buf1);



	 //return 0;	

	}while(choice!=3);
	

}


}
