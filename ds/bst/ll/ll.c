#include<stdio.h>
#include<malloc.h>
#include "ll.h"

node *head = NULL;
node *last = NULL;

// adds node to linked list
void add_node(int key, int data)
{
  node *ptr;

  ptr = (node *) malloc(sizeof(node)); 
  ptr->key = key;
  ptr->data = data;
  ptr->next = NULL;

  if(head == NULL)
  {
     head = ptr;
     last = ptr;
  }
  else
  {
     last->next = ptr;
     last = ptr;
  }
}

// updates all occurences in a linked list when given a key
int update_node(int key, int new_data)
{
   node *ptr;
   int replace_count = 0;

   for(ptr = head; ptr != NULL; ptr = ptr->next)
   {
      if(ptr->key == key)
      {
         ptr->data = new_data;
         replace_count++;
      }
   }

   return replace_count;
}

// traverses and prints all key value pairs
void traverse()
{
  node *ptr;
  
  printf("\nPrinting linked list\n");

  for(ptr = head; ptr != NULL; ptr = ptr->next)
  {
    printf("key: %d value: %d\n", ptr->key, ptr->data);
#if MJDB
getch();
#endif
  }

  printf("\nLinked list traversel completed\n");
}

// reverses linked list
void reverse()
{
  node *ptr = head;
  node *prev = head;

  if(ptr->next != NULL)
  {
    head = head->next;
    ptr->next = NULL;
    ptr = head;
  }

  while(ptr->next != NULL)
  {
    head = head->next;
    ptr->next = prev;
    prev = ptr;
    ptr = head;
  }

  head = ptr;
  head->next = prev;
}
