typedef struct binary_search_tree
{
  int data;
  struct binary_search_tree *left;
  struct binary_search_tree *right;
}bst;

// adds data to bst
void add_node(int data);

// adds node to bst. if it already cotains that data, then depending on flag it updates the same.
//void add_node(int data, int replace_bit);

// traces route to data from root
void trace_node(int data);

// traverses the tree from root
void traverse();
