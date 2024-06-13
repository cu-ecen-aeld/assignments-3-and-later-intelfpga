#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "list.h"

int lst_append(List *lst, List_Node *node){
	if(lst->first == 0){
		lst->first = node;
		lst->last = node;
	}else{
		(lst->last)->next = node;
		 lst->last = node;
	}
	node->next = 0;
	return ++(lst->length);	
}
int lst_remove(List *lst, List_Node *node){
	
	List_Node *cur_node;
	
	// if the node is the first in the list
	if((cur_node = lst->first) == node){
		lst->first = cur_node->next;
		lst->length--;
		return 1;
	}
	else{
		while(cur_node->next != 0){
		
			if(cur_node->next == node){
			
				cur_node->next = cur_node->next->next;
				lst->length--;
				
				if(lst->last == node)
					lst->last =  cur_node;
				
				return 1;
			
			}
			cur_node = cur_node->next;
		}
		
		return 0;
	}
			
}
int lst_init(List *lst){
	INIT_LIST(lst);
	lst->length=0;	
}

int lst_init_it(List *lst){
	INIT_IT(lst);
}

List_Node* lst_next(List *lst){
	List_Node *node;
	node = lst->iterator;
	lst->iterator = node->next;
	return node;
	
}

int lst_has_next(List *lst){
	return lst->iterator;
} 	
