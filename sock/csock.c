/*****************************************************
                       csock.c
           client socket implementation
       Authors: Mihir Joshi, Fenil Kavathia
                     csc573 NCSU
*****************************************************/

#include "csock.h"

void connect_to(int sock, char *server_addr, int port)
{
 struct sockaddr_in sin_serversock;
 struct hostent *h_server;

 h_server = gethostbyname(server_addr);

 if(h_server == NULL)
 {
  error("No such host");
 }

 bzero((char *) &sin_serversock, sizeof(sin_serversock));

 sin_serversock.sin_family = AF_INET;
 bcopy((char *) h_server->h_addr,
  (char *) &sin_serversock.sin_addr.s_addr,
  h_server->h_length);

 sin_serversock.sin_port = htons(port);

 if(connect(sock, (struct sockaddr *) &sin_serversock, sizeof(sin_serversock)) < 0)
 {
  error("error while connecting");
 }
}

