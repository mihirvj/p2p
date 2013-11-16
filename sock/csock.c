/*****************************************************
                       csock.c
           client socket implementation
       Authors: Mihir Joshi, Fenil Kavathia
                     csc573 NCSU
*****************************************************/

#include "csock.h"

int connect_to(int sock, char *server_addr, int port)
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
   //error("error while connecting");
	return -1;
 }

 return 0;
}

void read_my_ip(char ip[50])
{
 int fd;
 struct ifreq ifr;
 struct sockaddr_in *client;

 fd = socket(AF_INET, SOCK_STREAM, 0);

 ifr.ifr_addr.sa_family = AF_INET;

 strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);

 ioctl(fd, SIOCGIFADDR, &ifr);

 close(fd);

 client = (struct sockaddr_in *) &ifr.ifr_addr;

 /* display result */
 sprintf(ip, "%d.%d.%d.%d", (int)(client->sin_addr.s_addr&0xFF),
    (int)((client->sin_addr.s_addr&0xFF00)>>8),
    (int)((client->sin_addr.s_addr&0xFF0000)>>16),
    (int)((client->sin_addr.s_addr&0xFF000000)>>24));
}
