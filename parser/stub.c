#include "parse.h"

int main()
{
  char request[1000], rfc[50], host[50], port[50], title[100], response[1000];
  int err = OK;
  int method = LOOKUP;
  int i;
  int recvmethod;

  generate_request(request, method, 1234, "www.google.com", "5675", "ABCD"); // client

  printf("sending request:\n%s\n", request); 

  err = parse_request(request, rfc, host, port, title, &recvmethod); // server

  printf("method is: %d\n", recvmethod);

  if(err == OK)
  {
  generate_response(response, err, recvmethod);

  append_response(response, atoi(rfc), host, port, title);

  append_response(response, atoi(rfc), "www.facebook.com", port, title);

  printf("\nsending\n %s\n", response);
  
  printf("\n------Parsing-------\n");

  for(i=0; i <100; i++)
  {  
    if(parse_response(response, i, rfc, title, host, port) > 0)
    {

    printf("parsed rfc: %s\n", rfc);
    printf("parsed host: %s\n", host);
    printf("parsed title: %s\n", title);
    printf("parsed port: %s\n", port);
    }
    else
    {
	printf("\nbreaking at i: %d\n", i);
	break;
    }
  }

  }
  else
  {
	generate_response(request, err, method);
	printf("server replied: %s\n", request);
  }
}
