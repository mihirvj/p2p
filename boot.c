/*****************************************************
                       boot.c
		  bootstrap server
       Authors: Mihir Joshi, Fenil Kavathia
                     csc573 NCSU
*****************************************************/

#include "sock/ssock.h"
#include "ds/ll/rfcll.h"
#include "ds/ll/hostll.h"
#include "parser/parse.h"

#include<signal.h>
#include<pthread.h>

#define PORT 7752
#define BUFSIZE 256

rnode *rfc_head;
hnode *host_head;
int sock;

void *handle_con(void *arg);

void clear(int signum)
{
  pthread_exit(NULL);
  destroy_rlist(&rfc_head);
  destroy_hlist(&host_head);

  close_sock(sock);

#ifdef APP
  printf("[log] list destroyed\n");

  printf("[log] server socket closed\n");

  printf("[log] list after destroying\n");

  //rtraverse(rfc_head);
#endif

  rfc_head = NULL;
  host_head=NULL;

  exit(0);
}

void segv(int signum)
{
  printf("Whoa.. It crashed, FM! Consider restarting your server while we do cleanup\n");

  destroy_rlist(&rfc_head);
  destroy_hlist(&host_head);

  close_sock(sock);

  fflush(stdout);

  signal(signum, SIG_DFL);
  kill(getpid(), signum);
}

int main(int argc, char **argv)
{
  int csock, id;
  pid_t procId;

  struct sockaddr_in addr;
  socklen_t addr_len;
  char port[50], host[50];

  pthread_attr_t attr;
  pthread_t threads;
  
  signal(SIGINT, clear);
  signal(SIGSEGV, segv);

  printf("\nserver started. press ctrl+c to stop\n");

  // get socket descriptor
  sock = get_sock();

#ifdef GRAN1
printf("[gran 1] created socket\n");
#endif

  // bind socket
  bind_sock(sock, PORT);

#ifdef GRAN1
printf("[gran 1] bind socket\n");
#endif

  //listen now
  listen_sock(sock);

#ifdef GRAN1
printf("[gran 1] now listening\n");
#endif

  pthread_attr_init(&attr);

#ifdef GRAN1
printf("[gran 1] init thread\n");
#endif
  while(1)
  {
    // accept connection
    csock = accept_con(sock);

    addr_len = sizeof(addr);

	if(getpeername(csock, (struct sockaddr * )&addr, &addr_len) == -1)
	{
		printf("[error] not able to get socket name\n");
		return;
	}

	sprintf(port, "%d", ntohs(addr.sin_port));
   	sprintf(host, "%d.%d.%d.%d", 
	(int)(addr.sin_addr.s_addr&0xFF), 
	    (int)((addr.sin_addr.s_addr&0xFF00)>>8),
		    (int)((addr.sin_addr.s_addr&0xFF0000)>>16),
			    (int)((addr.sin_addr.s_addr&0xFF000000)>>24));

	

	add_hnode(&host_head, host, atoi(port));

	printf("\ngot connection from: %s\n", host);
    id = csock;

    // create a thread to handle connection
    pthread_create(&threads, &attr, handle_con, &id);
  }

  pthread_join(threads, NULL);

  return 0;
}

void *handle_con(void *arg)
{
 char buf[BUFSIZE];
 int csock = *(int *) arg;
 char rfc[50], host[50], port[50], title[50];
 int method, errcode;

#ifdef APP
 printf("[log] I am in child process of csock: %d", csock);
#endif

while(1)
{
//somewhere here new node will be added to linklist	
 // read from client
 read_from(csock, buf, BUFSIZE);

#ifdef APP
 printf("\n[log] client %d says: %s\n", csock, buf);
#endif

//fflush(stdout);

#ifdef APP
 printf("[log] writing to client %s\n", buf);
#endif

 errcode = parse_request(buf, rfc, host, port, title, &method);

 generate_response(buf, errcode, method);

 if(method == ADD)
 {
	add_rnode(&rfc_head, atoi(rfc), title, host, atoi(port));
	append_response(buf, atoi(rfc), host, port, title);
#ifdef APP
	printf("\n----printing rfc list----\n");
	rtraverse(rfc_head);
#endif
 }
 if(method == LISTALL)
 {
	rnode *list = rfc_head;

	while(list != NULL)
	{
		char port_no[50];
		sprintf(port_no,"%d", list->port);

		append_response(buf, list->rfc_no, list->host, port_no, list->title);
		list = list->next;
	}

 }
 if(method == LOOKUP)
 {	
	rnode *rlist, *rptr;
	printf("\nlooking up: %s\n", rfc);

	rlookupall(rfc_head, atoi(rfc), &rlist);

#ifdef APP
	printf("\n----printing looked up list------\n");
	rtraverse(rlist);
#endif
	rptr = rlist;

	while(rptr != NULL)
	{
		char port_no[50];
		printf("host:%s\nrfc: %d\n", rptr->host, rptr->rfc_no);
		sprintf(port_no, "%d", rptr->port);
		append_response(buf, rptr->rfc_no, rptr->host, port_no, rptr->title);
		rptr = rptr->next;
	}

	destroy_rlist(&rlist);

#ifdef APP
	printf("\n----list after destroying---\n");
	rtraverse(rlist);
#endif
 }
 if(method == TERMINATE)
 {
	printf("\nTermination request from %d\n", csock);
	delete_hnode(&host_head, host);
	delete_rnode(&rfc_head, host);

#ifdef APP
	printf("\ndeleted from rfc list: List after deleting\n");
	rtraverse(rfc_head);
#endif
	close_sock(csock);
	break;
 }

 // echo reply to client
 write_to(csock, buf, BUFSIZE);

 //close_sock(csock);

#ifdef APP
 printf("[log] client socket closed %d\n", csock);
#endif
}

 pthread_exit(NULL);
}
