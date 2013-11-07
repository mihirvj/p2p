/*****************************************************
                       boot.c
		  bootstrap server
       Authors: Mihir Joshi, Fenil Kavathia
                     csc573 NCSU
*****************************************************/

#include "sock/ssock.h"
#define PORT 7752
#define BUFSIZE 256

int main(int argc, char **argv)
{
  int sock, csock;
  char buf[BUFSIZE];
  pid_t procId;

  // get socket descriptor
  sock = get_sock();

  // bind socket
  bind_sock(sock, PORT);
  
  //listen now

  listen_sock(sock);

  while(1)
  {
    // accept connection
    csock = accept_con(sock);

    procId = fork();

    if(procId < 0)
    {
       error("error on fork");
    }

    if(procId == 0) // client process
    {
       close_sock(sock);

       // read from client
       read_from(csock, buf, BUFSIZE);

       // echo reply to client
       write_to(csock, buf, BUFSIZE);
    }
    else
    {
       close_sock(csock);
    }
  }

  return 0;
}
