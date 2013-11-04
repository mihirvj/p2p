typedef struct ll
{
  int key;
  int data;
  struct ll *next;
}node;

void add_node(int key, int data);
int update_node(int key, int new_data);
void traverse();
void reverse();
