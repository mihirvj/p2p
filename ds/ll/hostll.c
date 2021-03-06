/*****************************************************
                       ll.c
            linked list implementation
       Authors: Mihir Joshi, Fenil Kavathia
                     csc573 NCSU
*****************************************************/

#include<stdio.h>
#include<malloc.h>
#include "hostll.h"

// adds node to linked list
void add_hnode(hnode **head, char *hostname,int port)
{
  hnode *ptr;

  ptr = (hnode *) malloc(sizeof(hnode)); 
  
  ptr->host=malloc(strlen(hostname)+1);
  
  strcpy(ptr->host,hostname);
  ptr->port=port;
  ptr->next = NULL;

  if((*head) == NULL)
  {
     *head = ptr;
  }
  else
  {
     hnode *temp = *head;

     while(temp->next != NULL)
     {
       temp = temp->next;
     }

     temp->next = ptr;
  }
}



/*node** lookupall(node *head, int key)
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
}*/

// traverses and prints all key value pairs
void htraversal(hnode *head)
{

printf("printing currently active nodes\n");
while(head!=NULL)
{
printf("Host Name:%s Port No:%d\n",head->host,head->port);
head=head->next;

}
printf("successfully printed currently active nodes\n");
}

int delete_hnode(hnode **head, char *hostname,int port)
{
int del_count=0;
hnode *prev,*ptr=*head;
if(*head == NULL)
  return 1;

while(ptr!=NULL)
{
	if(strcmp(ptr->host,hostname)==0 && (ptr->port==port))
		{
		if(ptr==*head)
			{
			printf("Host  %s Removed Successfully\n",ptr->host);
			*head=ptr->next;
			free(ptr);
			del_count++;
			return del_count;
			}
		else
			{
			printf("Host  %s Removed Successfully\n",ptr->host);
			prev->next=ptr->next;
			free(ptr);
			del_count++;
			return del_count;
		        }
		}
	else
		{
			prev=ptr;
		}
ptr=ptr->next;
}
#ifdef APP
printf("No Host found with this Name");
#endif
return del_count;

}	


void destroy_hlist(hnode **head)
{
  hnode *current = *head;

  while(current != NULL)
  {
    hnode *next = current->next;

    free(current);

    current = next;
  }

  *head = NULL;
}
