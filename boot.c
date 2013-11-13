/*****************************************************
                       boot.c
		  bootstrap server
       Authors: Mihir Joshi, Fenil Kavathia
                     csc573 NCSU
*****************************************************/

#include "sock/ssock.h"
#include "ds/ll/hostll.h"
#include "ds/ll/rfcll.h"
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
  destroy_list(&rfc_head);
  destroy_list(&host_head);

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
  printf("Whoa.. It crashed, MJ! Consider restarting your server while we do cleanup\n");

  destroy_list(&rfc_head);
  destroy_list(&host_head);

  close_sock(sock);

  fflush(stdout);

  signal(signum, SIG_DFL);
  kill(getpid(), signum);
}

int main(int argc, char **argv)
{
  int csock, id;
  pid_t procId;

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

#ifdef APP
 printf("[log] I am in child process of csock: %d", csock);
#endif

//somewhere here new node will be added to linklist	
 // read from client
 read_from(csock, buf, BUFSIZE);

#ifdef APP
 printf("\n[log] client says: %s\n", buf);
#endif

//fflush(stdout);
 if(strcmp(buf, "add") == 0)
 {
#ifdef APP
   printf("[log] comparison success rfc head: %ld\n", &rfc_head);
#endif

   //add_node(&rfc_head, csock, 0);
   printf("here do something");
#ifdef APP
   printf("[log] node added\n");

   //traverse(rfc_head);

   printf("[log] list traversed\n");
#endif
 }
 else
 {
#ifdef APP
   printf("[log] comparison fail\n");
#endif
 }

#ifdef APP
 printf("[log] writing to client %s\n", buf);
#endif

 // echo reply to client
 write_to(csock, buf, BUFSIZE);

 close_sock(csock);

#ifdef APP
 printf("[log] client socket closed %d\n", csock);
#endif

 pthread_exit(NULL);
}
