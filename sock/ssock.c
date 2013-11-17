/*****************************************************
                       ssock.c
           server socket implementation
       Authors: Mihir Joshi, Fenil Kavathia
                     csc573 NCSU
*****************************************************/

#include "ssock.h"
#include "../ds/ll/hostll.h"

void bind_sock(int sock, int port)
{
 int opt = 1;
 struct sockaddr_in addr;

 bzero((char *) &addr, sizeof(addr));

 addr.sin_family = AF_INET;
 addr.sin_addr.s_addr = INADDR_ANY;
 addr.sin_port = htons(port);

 setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *) &opt, sizeof(int));

 if(bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
   error("error while binding");
}

void listen_sock(int sock)
{
 listen(sock, MAX_CONNECTIONS);
}

int accept_con(int sock)
{
 struct sockaddr_in client;
 int client_len = sizeof(client);
 int csock;
 char req_from[50];

 csock = accept(sock, (struct sockaddr *) &client, &client_len);

 if(csock < 0)
   error("error while accepting connection");
 else
 {
#ifdef GRAN1
   printf("\naccepted connection successfully from ");

   sprintf(req_from, "%d.%d.%d.%d", (int)(client.sin_addr.s_addr&0xFF),
    (int)((client.sin_addr.s_addr&0xFF00)>>8),
    (int)((client.sin_addr.s_addr&0xFF0000)>>16),
    (int)((client.sin_addr.s_addr&0xFF000000)>>24));

   printf("%s:%d\n", req_from, ntohs(client.sin_port));
#endif
 }

 return csock;
}
