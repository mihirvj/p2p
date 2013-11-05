/*****************************************************
                       data.c
    data send/receive functionality using sockets
       Authors: Mihir Joshi, Fenil Kavathia
                     csc573 NCSU
*****************************************************/

#include "data.h"

int get_sock()
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  if(sock < 0)
    error("error opening socket");

  return sock;
}

void read_from(int sock, char *buffer, int buf_len)
{
  int bytes_read;

  bzero(buffer, buf_len);

  bytes_read = read(sock, buffer, buf_len);

 if(bytes_read < 0)
   error("error while reading");  
}

void write_to(int sock, char *buffer, int buf_len)
{
 int bytes_written;

 bytes_written = write(sock, buffer, buf_len);

 if(bytes_written < 0)
  error("error writing to socket");
}

void error(char *msg)
{
  perror(msg);
  exit(-4249);
}

void close_sock(int sock)
{
  close(sock);
}

