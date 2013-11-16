/*****************************************************
                       data.c
    data send/receive functionality using sockets
       Authors: Mihir Joshi, Fenil Kavathia
                     csc573 NCSU
*****************************************************/

#include "data.h"

#define BACKOFF_LIMIT_CROSSED(backoff) if(backoff > 3) return 0; else backoff++;

int get_sock()
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  if(sock < 0)
    error("error opening socket");

  return sock;
}

int read_from(int sock, char *buffer, int buf_len)
{
  int bytes_read;
  int backoff = 0;

  bzero(buffer, buf_len);

  while((bytes_read = read(sock, buffer, buf_len)) <= 0)
  {
	sleep(100);
	BACKOFF_LIMIT_CROSSED(backoff);
  }

  //bytes_read = read(sock, buffer, buf_len);
 
#ifdef GRAN1
 printf("[log] read bytes: %d\n", bytes_read);
#endif

 if(bytes_read < 0)
   error("error while reading");  

 return bytes_read;
}

int write_to(int sock, char *buffer, int buf_len)
{
 int bytes_written;

 bytes_written = write(sock, buffer, buf_len);

 if(bytes_written < 0)
  error("error writing to socket");

 return bytes_written;
}

void error(char *msg)
{
  perror(msg);

  printf("\n");

  exit(-4249);
}

void close_sock(int sock)
{
  close(sock);
}

