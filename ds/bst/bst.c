#include<malloc.h>
#include<stdio.h>
#include "bst.h"

bst *root = NULL;

void add_to_tree(struct binary_search_tree *node, struct binary_search_tree *ptr);

void print_tree(struct binary_search_tree *node);

void locate_node(struct binary_search_tree *node, int data);

void add_node(int data)
{
  bst *ptr = (bst *) malloc(sizeof(bst));
  ptr->data = data;
  ptr->left = NULL;
  ptr->right = NULL;

  if(root == NULL)
  {
     root = ptr;
  }
  else
  {
     add_to_tree(root, ptr);
  }
}

void add_to_tree(bst *node, bst *ptr)
{
  if(node == NULL)
    return;

  if(node->left != NULL && ptr->data < node->data)
  {
    add_to_tree(node->left, ptr);
  }

  if(node->right != NULL && ptr->data >= node->data)
  {
    add_to_tree(node->right, ptr);
  }

  if(node->left == NULL && ptr->data < node->data)
  {
      node->left = ptr;
  }

  if(node->right == NULL && ptr->data >= node->data)
  {
    node->right = ptr;
  }
}

void trace_node(int data)
{
  locate_node(root, data);   
}

void locate_node(bst *node, int data)
{
  if(node == NULL)
    return;

  if(node->data == data)
  {
    printf("\nLocated node %d\n", data);
    return;
  }
 
  if(data < node->data)
  {
    printf("\nGoing left of %d\n", node->data);
    locate_node(node->left, data);
  }

  if(data >= node->data)
  {
    printf("\nGoing right of %d\n", node->data);
    locate_node(node->right, data);
  }
}

void traverse()
{
  print_tree(root);
}

void print_tree(bst *node)
{
 if(node == root)
 {
   printf("\nRoot value: %d", node->data);
 }
 else
 {
   printf("%d\n", node->data);
 }

 if(node->left)
 {
   printf("\nLeft of %d is ", node->data);
   print_tree(node->left);
 }

 if(node->right)
 {
   printf("\nRight of %d is ", node->data);
   print_tree(node->right);
 }
}
