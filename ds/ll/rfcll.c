/*****************************************************
                       ll.c
            linked list implementation
       Authors: Mihir Joshi, Fenil Kavathia
                     csc573 NCSU
*****************************************************/

#include<stdio.h>
#include<malloc.h>
#include "rfcll.h"

// adds node to linked list
void add_rnode(rnode **head,int rfc_no,char *title,char *hostname,int port)
{
  rnode *ptr,*old;
  int counter=0;
  int duplicate=0;

  ptr = (rnode *) malloc(sizeof(rnode)); 
  
  ptr->host=malloc(strlen(hostname)+1);
  ptr->title=malloc(strlen(title)+1);
  
  ptr->rfc_no=rfc_no;
  strcpy(ptr->host,hostname);
  strcpy(ptr->title,title);
  ptr->port=port;
  ptr->next = NULL;

  if((*head) == NULL)
  {
     *head = ptr;
	#ifdef APP
     printf("head node is added\n");
	#endif
  }
  else
  {
     rnode *temp = *head;
     
     while(temp!= NULL) 
        
     {
        if( ((temp->rfc_no)==(rfc_no)) && (strcmp(temp->host,hostname)==0) && temp->port == port)
            {
  		duplicate=1;
  		break;
            }
        else
           {  
		
                old=temp;
                temp=temp->next;
           }
      }       

    	if(duplicate==0)
         {
          old->next=ptr;
         }
        else
	{
        #ifdef APP
	printf("oops node is there");
	#endif
	}
                   
   }
}

//Here we are not passing the title as it is redundant only rfc_no is sufficient as user must pass rfc_no in LOOKUP request

void rlookupall(rnode *head, int rfc_no, rnode **list)
{
  rnode *ptr = head;

  for(ptr = head; ptr != NULL; ptr = ptr->next)
  {
      if(ptr->rfc_no == rfc_no)
      {
         add_rnode(list, ptr->rfc_no, ptr->title, ptr->host, ptr->port);  //here we need to add all data
      }
  }
} 

// traverses and prints all RFC node present in linkedlist
//  This function is somewhat remaining as we need to pass this info to client so that remaining
void rtraverse(rnode *head)
{

printf("printing current rfc list\n");
while(head!=NULL)
{
printf("Host Name:%s RFC No:%d\n",head->host,head->rfc_no);
head=head->next;

}
printf("successfully printed currently rfc list\n");
}


int delete_rnode(rnode **head, char *hostname,int port)
{
int del_count=0;
rnode *prev,*ptr=*head;
if(*head == NULL)
  return 1;

while(ptr!=NULL)
{
	if(strcmp(ptr->host,hostname)==0 && (ptr->port==port))
		{
		if(ptr==*head)
			{
			printf("RFC : %d & Host: %s Removed Successfully\n",ptr->rfc_no,ptr->host);
			*head=ptr->next;
			free(ptr);
			del_count++;
			}
		else
			{
			printf("RFC : %d & Host: %s Removed Successfully\n",ptr->rfc_no,ptr->host);
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
#ifdef APP
printf("All host and assoicated RFC are deleted");
#endif
return del_count;

}	

	
void destroy_rlist(rnode **head)
{
  rnode *current = *head;

  while(current != NULL)
  {
    rnode *next = current->next;

    free(current);

    current = next;
  }

  *head = NULL;
}

/*
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
*/

