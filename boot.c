/*****************************************************
                       boot.c
		  bootstrap server
       Authors: Mihir Joshi, Fenil Kavathia
                     csc573 NCSU
*****************************************************/

#include "sock/ssock.h"
#include "ds/ll/ll.h"
#include<signal.h>
#include<pthread.h>

#define PORT 7752
#define BUFSIZE 256

node *rfc_head;
int sock;

void *handle_con(void *arg);

void clear(int signum)
{
  destroy_list(rfc_head);

  close_sock(sock);

#ifdef APP
  printf("[log] list destroyed\n");

  printf("[log] server socket closed\n");
#endif

  exit(0);
}

int main(int argc, char **argv)
{
  int csock, id;
  pid_t procId;

  pthread_attr_t attr;
  pthread_t threads;
  
  signal(SIGINT, clear);

  printf("\nserver started. press ctrl+c to stop\n");

  // get socket descriptor
  sock = get_sock();

  // bind socket
  bind_sock(sock, PORT);
  
  //listen now
  listen_sock(sock);

  pthread_attr_init(&attr);

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

   add_node(&rfc_head, csock, 0);

#ifdef APP
   printf("[log] node added\n");
#endif

   traverse(rfc_head);

#ifdef APP
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
