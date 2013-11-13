typedef struct rfcnode
{
  int rfc_no;
  char *title;	
  char *host;
  int port;
  struct rfcnode *next;
}rnode;

void add_rnode(rnode **head,int rfc_no,char *title,char *hostname,int port);
//int update_node(node *head, int key, int new_data);
void rtraverse(rnode *head);
//node** lookupall(node *head, int key);
void destroy_rlist(rnode **head);
int delete_node(rnode **head, char *hostname);
rnode** rlookupall(rnode *head, int rfc_no);
