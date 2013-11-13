typedef struct hostnode
{
  
  char *host;
  int port;	
  struct hostnode *next;
}hnode;

void add_hnode(hnode **head,char *hostname, int port);
void htraverse(hnode *head);
void destroy_hlist(hnode **head);
int delete_hnode(hnode **head,char *hostname);
