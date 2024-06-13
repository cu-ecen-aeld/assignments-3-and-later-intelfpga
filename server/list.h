#define LIST_ITEMS\
	struct list_node *next;\
	int item;\
	
typedef struct list_node {
	LIST_ITEMS;	
}List_Node;

typedef struct list{
	List_Node *first, *last, *iterator;
	int length;
}List;

int lst_init(List *lst);
int  lst_init_it(List *lst); 
int  lst_append(List *lst, List_Node *node);
int  lst_remove(List *lst, List_Node *node);

List_Node*  lst_next(List *lst);
int	lst_has_next(List *lst);

int 	lst_insert(List *lst, List_Node *node);
	

#define INIT_LIST(list) \
	list->first = list->last = list->iterator = (List_Node *)0 
	
#define INIT_IT(list)\
	list->iterator = list->first
