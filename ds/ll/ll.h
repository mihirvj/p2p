typedef struct ll
{
  int key;
  int data;
  struct ll *next;
}node;

void add_node(node **head, int key, int data);
int update_node(node *head, int key, int new_data);
void traverse(node *head);
node** lookupall(node *head, int key);
void destroy_list(node **head);
int delete_node(node **head, int key);
