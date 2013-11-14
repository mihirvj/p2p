/*****************************************************
                       ll.c
            linked list implementation
       Authors: Mihir Joshi, Fenil Kavathia
                     csc573 NCSU
*****************************************************/

#include<stdio.h>
#include<malloc.h>
#include "ll.h"

// adds node to linked list
void add_node(node **head, int key, int data)
{
  node *ptr;

  ptr = (node *) malloc(sizeof(node)); 
  ptr->key = key;
  ptr->data = data;
  ptr->next = NULL;

  if(*head == NULL)
  {
     *head = ptr;
  }
  else
  {
     node *temp = *head;

     while(temp->next != NULL)
     {
       temp = temp->next;
     }

     temp->next = ptr;
  }
}

// updates all occurences in a linked list when given a key
int update_node(node *head, int key, int new_data)
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

node** lookupall(node *head, int key)
{
  node *ptr = head;
  node **list = NULL;

  for(ptr = head; ptr != NULL; ptr = ptr->next)
  {
      if(ptr->key == key)
      {
         add_node(list, key, ptr->data);
      }
  }

  return list;
}

// traverses and prints all key value pairs
void traverse(node *head)
{
  node *ptr;

  printf("\nPrinting linked list\n");

  for(ptr = head; ptr != NULL; ptr = ptr->next)
  {
    printf("key: %d value: %d\n", ptr->key, ptr->data);
  }

  printf("\nLinked list traversel completed\n");
}

/*int delete_node(node **head, int key)
{
  int del_count = 0;
  node *ptr, *prev;
  
  if((*head) == NULL)
    return 1;

  if((*head)->key == key && (*head)->next == NULL)
  {
    *head = NULL;
    return 1;
  }

  ptr = (*head)->next;
  prev = *head;

  while(ptr != NULL)
  {
    if(ptr->key == key)
    {
      node *temp = ptr;
      prev->next = ptr->next;
      ptr = ptr->next;
      free(temp);
      del_count++;
    }

    prev = ptr;
    ptr = ptr->next;
  }

  return del_count;
}*/


int delete_node(node **head, int key)
{
int del_count=0;
node *ptr,*prev;
if(*head == NULL)
  return 1;

while(ptr!=NULL)
{
	if(ptr->data==key)
		{
		if(ptr==*head)
			{
			*head=ptr->next;
			free(ptr);
			del_count++;
			}
		else
			{
			prev->next=ptr->next;
			free(ptr);
			del_count++;
		        }
		}
	else
		{
			prev=ptr;
		}
ptr=ptr->next;
}
return del_count;

}	
void destroy_list(node **head)
{
  node *current = *head;

  while(current != NULL)
  {
    node *next = current->next;

    free(current);

    current = next;
  }

  *head = NULL;
}
