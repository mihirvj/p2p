///

#include "data.h"

#define MAX_CONNECTIONS 5

void bind_sock(int sock, int port, struct sockaddr_in addr);
void listen_sock(int sock);
int accept_con(int sock);
