/*****************************************************
                       cpeer.c
             peer client implementation
       Authors: Mihir Joshi, Fenil Kavathia
                     csc573 NCSU
*****************************************************/

#include "sock/csock.h"
#include<string.h>

#define SERVER_PORT 7752
#define SERVER_ADDR "127.0.0.1"
#define BUFSIZE 256

int main(int argc, char **argv)
{
 int sock;
 char buf[BUFSIZE];

 sock = get_sock();

 connect_to(sock, SERVER_ADDR, SERVER_PORT);

 strcpy(buf, "pinging");

 write_to(sock, buf, BUFSIZE);

 read_from(sock, buf, BUFSIZE);

 printf("\nserver replied: %s", buf);

 close_sock(sock);

 return 0;
}
