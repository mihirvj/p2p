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
#include<pthread.h>
#include<stdbool.h>
#include<fcntl.h>
#include<dirent.h>

#define SERVER_PORT 7752
//#define SERVER_ADDR "192.168.1.121"
#define SERVER_ADDR "127.0.0.1"
#define BUFSIZE 256

int ssock;
int csock;
int port;
int pid;
char myport[50], myhostname[50];

void s_peer(int outpipe);
void c_peer(int inpipe);
bool download_content(int rfc_no,char* hostname,int port);
void *handle_peer(void *arg);

void terminate(int sock, char host[50], char port[50])
{
  char term_req[BUFSIZE];

  generate_request(term_req, TERMINATE, -1, host, port, "");

#ifdef APP
  printf("\nsending: %s\n", term_req);
#endif

  write_to(sock, term_req, BUFSIZE);
}

void segv(int signum)
{
  int status;
  char term_req[BUFSIZE];

#ifdef APP
  printf("\nI %s:%s am closing\n", myhostname, myport);
#endif

  if(strcmp(myhostname, "") != 0 && strcmp(myport, "") != 0)
  {
	terminate(csock, myhostname, myport);
  }

  close_sock(csock);
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
} // main ends

void s_peer(int outpipe)
{
	int cssock;
	int id;
	char buf[BUFSIZE];
	struct sockaddr_in addr;
	
	socklen_t addr_len;
	char myport[50], myhostname[50];
	pthread_attr_t attr;
	pthread_t threads;

        srand(time(NULL));
	port=(rand()%(65535-1024))+1024;

#ifdef GRAN1
	printf("\nserver peer port number: %d\n", port);
#endif

	//right now we need to know the port no. so assigned fix one
	//port=5000;
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

#ifdef GRAN1
	printf("sending port: %s host: %s\n", myport, myhostname);
#endif

	write_to(outpipe, myport, 50);
	
	write_to(outpipe, myhostname, 50);

	close(outpipe);

	listen_sock(ssock);

#ifdef GRAN1
 printf("[gran 1] now listening\n");
#endif
	pthread_attr_init(&attr);

	while(1)
        {
    	  // ready to accept connection
	  //communication with one peer completed

		cssock = accept_con(ssock);

		id=cssock;
	  // create a thread to handle client peer

		pthread_create(&threads, &attr, handle_peer, &id);          
	}

	pthread_join(threads, NULL);

} // s_speer ends


void c_peer(int inpipe)
{
 	int choice, i;
	char request[BUFSIZE], response[BUFSIZE];
	int rfc;
	char title[50];

	char c_rfc[50], host[50], port[50];

	DIR *d;
	struct dirent *entry;
	
	//client socket (cpeer);
	//get client socket descriptor  
	csock = get_sock();

	read_from(inpipe, myport, 50);
	read_from(inpipe, myhostname, 50);

	close(inpipe);

#ifdef GRAN1
	printf("received port: %s host: %s\n", myport, myhostname);
#endif

	//connect to server socket
	//connect_to(sock, SERVER_ADDR, SERVER_PORT);  //this is for communication between cpeer and server
	//right now assume we are using this function to connect  to server peer 
	connect_to(csock, SERVER_ADDR, SERVER_PORT);   //used 5000 port because we don't know server peer port

	printf("\n***********Welcome to Peer to Peer RFC Document Transfer Application***********\n");
	printf("\nMake sure you have rfc folder in directory of your executable\n");

	// add entries of current rfcs when booted up
	
	d = opendir("./rfc");

	if(d)
	{
		while((entry = readdir(d)) != NULL)
		{
			char rfc[50];

			if(strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0)
				continue;

			memset(request,(int) '\0',BUFSIZE);	
#ifdef APP
	printf("found file: %s\n", entry->d_name);
#endif

			sscanf(entry->d_name, "%s.txt", rfc);

#ifdef APP
	printf("sending to server: %s\n", rfc);
#endif
	
			generate_request(request, ADD, atoi(rfc), myhostname, myport, "title"); 
			
			write_to(csock, request, BUFSIZE);

			read_from(csock, response, BUFSIZE);
#ifdef APP
	printf("Boot Server Response is \n%s\n",response);
#endif
		}

		closedir(d);
	}
	else // TODO: handle case where no rfc folder is specified
	{

	}

	while(1) // loop forever unless user tells it to stop ; option 4
	{
		memset(request,(int) '\0',BUFSIZE);	

		printf("\nPress 1 for Lookup and download\n");
		printf("Press 2 for list\n");	
		printf("Press 3 for Terminate: \n");
		scanf("%d",&choice);

		switch(choice)
		{
		/*case 1:
			printf("\n Please Enter RFC Number: ");
			scanf("%d", &rfc);                    //what if user by mistake write wrong rfc no?
							      //whether we should automatically do this when client server 								      //starts	in begining
							      //what is the proof that I have that file??
						

			printf("\nPlease Enter RFC Title: ");
			scanf("%s", title);

			generate_request(request, ADD, rfc, myhostname, myport, title); 
		break;*/
		case 1:
			printf("\nPlease Enter RFC Number: ");
			scanf("%d", &rfc);

			printf("\nPlease Enter RFC Title: ");
			scanf("%s", title);
			generate_request(request, LOOKUP, rfc, myhostname, myport, title);
		break;
		case 2:	
			generate_request(request, LISTALL, -1, myhostname, myport, "sample");  //why -1, doesn't matter
			break;			
		case 3:			
	        	printf("Thank you for using Program\n");
			//terminate(csock, myhostname, myport);
			//close_sock(csock);
			raise(SIGINT);
			return;
		} // switch ends
	
         	//If user press 4 then we need to disable write_to and read_from otherwise it will be written to  boot server 
	 	//server will handle this and generete bad request if user type anything
		
		write_to(csock, request, BUFSIZE);

		read_from(csock, response, BUFSIZE);

		printf("Boot Server Response is \n%s\n",response);
	
 		if(choice == 1) // lookup
		{
			i = 0;
			printf("\nplease wait while your file is downloaded\n");
	
	
			bool success=false;
 			while(parse_response(response, i, c_rfc, title, host, port) != -1)
			{
#ifdef APP
	printf("\nI understand that rfc %s is with %s:%s\n", c_rfc, host, port);
#endif

	   			success = download_content(rfc,host,atoi(port));  //i am using rfc no here not rfc array
		
	   			if(success)
	   			{
					printf("\nRfc %d is sucessfully downloaded\n", atoi(c_rfc));
					//send add to server for just downloaded file
					
					generate_request(request, ADD, atoi(c_rfc), myhostname, myport, title); 

					write_to(csock, request, BUFSIZE);

					read_from(csock, response, BUFSIZE);

#ifdef APP
	printf("\nServer responded:\n%s\n", response);
#endif
					break;	
	   			}
	   			else
	   			{
					i++; // try next host
	   			}
	
	 		} // while ends

			if(!success)
			{
				printf("\nNo peer from list seems to have the file. Please retry later\n");
			}
		} // if ends

	} // while ends

} // c_peer ends


void *handle_peer(void *arg)
{
	char buf[BUFSIZE];
 	int cssock = *(int *) arg;
 	int fp;
 	int sent;
 	char err_buf[20], rfc[50], host[50], port[50], title[100], fname[20];
	int method, errcode;
 	
#ifdef GRAN1
 printf("[log] I am handling cpeer thread : %d", csock);
#endif

	read_from(cssock, buf, BUFSIZE);

	char *temp=buf;

#ifdef GRAN1
 printf("\n[log] Peer client says: %s\n", buf);
#endif
        
	errcode = parse_request(buf, rfc, host, port, title, &method);

	if(errcode != OK)
	{
#ifdef APP
	printf("\nErrcode: %d\n", errcode);
#endif
		generate_response(buf, errcode, method);

		write_to(cssock, buf, BUFSIZE);

		return;
	}

	sprintf(fname, "./rfc/%d.txt", atoi(rfc));

#ifdef APP
	printf("\ngot filename as: %s\n", fname);
#endif

	fp = open(fname, O_RDONLY, S_IRUSR);  //here we can make one rfc folder in current directory and put all files there

	if(fp != -1)
	{
		char send_buf[BUFSIZE];              //we don't know the file size in advance (It will be a waste for files which are 
						  //relatively small	
#ifdef GRAN1
	 printf("[log] writing to  Peer client %s\n", buf);
#endif
		 
		memset(send_buf, 0, BUFSIZE);

	    	while((sent = read(fp, send_buf, BUFSIZE - 1)) > 0)
	    	{
			if(sent < (BUFSIZE - 1))
			{
				send_buf[sent] = '\0';
			}
			else
			{
				send_buf[BUFSIZE] = '\0';
			}

#ifdef GRAN1
	printf("file content is:\n%s\n",send_buf); 
#endif
			write_to(cssock, send_buf, sent);

#ifdef GRAN1
       	printf("\nsent %d bytes\n", sent);	
#endif
			memset(send_buf, 0, BUFSIZE);
	    	}

	    close(fp);

#ifdef GRAN1
	printf("File operation Completed\n"); 	
#endif
	}
	else
	{
        	memset(err_buf,0,sizeof(err_buf));  
		perror("Server File Error:");
		strcpy(err_buf,"!@#$%^&*");
		write_to(cssock, "", 0);
 	}
	 
 	close_sock(cssock);

#ifdef GRAN1
 printf("[log] client socket closed %d\n", cssock);
#endif

 pthread_exit(NULL);

} // handle_peer

bool download_content(int rfc_no,char* hostname,int port)
{
	int tsock;
	char filename[BUFSIZE];
	char file_data[BUFSIZE];
	char port_no[8];
	int fp = -9999, readBytes;
	bool received = false;

	tsock=get_sock();

	connect_to(tsock,hostname, port);

	sprintf(port_no,"%d", port);

	generate_request(filename, GET, rfc_no, myhostname, myport, "title"); 

#ifdef APP
	printf("\nseding GET request as:\n%s\n", filename);
#endif

	write_to(tsock, filename, BUFSIZE); // inform peer about file name i want

	sprintf(filename, "./rfc/%d.txt", rfc_no);

	memset(file_data, 0, BUFSIZE);

#ifdef GRAN1
	printf("\ni m waiting while server delivers\n");
#endif
	while(readBytes = read(tsock, file_data, BUFSIZE - 1) > 0)
	{
		received = true;

#ifdef GRAN1
	printf("\nreceived file chunk: %s\nsize:%d\n", file_data, readBytes);
#endif

		//not sure whether to close socket here or after everything done

		/*if(strcmp(strncpy(temp_buf,file_data,8),"!@#$%^&*")==0)
		{
			printf("\nPeer %s does not have file\n", hostname);
			close_sock(tsock);	
			printf("closeing peer client socket: and connecting to different peer server one%d\n", tsock);
			return false;
		}
		else
		{*/
			printf("\rPeer server %s has the file and please wait while it delivers file",hostname);

			if(fp == -9999) // open file once
				fp = open(filename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);   //rfc folder should be there 

			if(fp != -1 && fp != -9999)
			{
				write(fp, file_data, strlen(file_data));  //write to file 
				memset(file_data, 0, BUFSIZE);
			}
			else
			{
				perror("Peer client Error: ");
				received = false;   //here problem is client side so we should not try connecting to different peer because same will 					//happen again
				break;
			}	
		//}
	}

	if(!received)
	{
		printf("\nPeer %s does not seem to have file or some error occured\n", hostname);
	}

	if(fp != -9999)
		close(fp);

	close_sock(tsock);

	return received;

} // download_content ends
