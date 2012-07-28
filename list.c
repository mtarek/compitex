#include <string.h>
#include "list.h"

static int (*eq) (void *d1, void *d2);
static int dummy_eq(void *d1, void *d2)
{
	if(!d1 || !d2)
		return 0;
		
	if(d1 == d2)
		return 1;
}

void List_init(List *l, int (*compare) (void *d1, void *d2))
{
	//l = calloc(1, sizeof(List));
	
	if(compare)
		eq = compare;
	else
		eq = dummy_eq;
		
	l->size = 0;
}

void List_add(List *l, void *data)
{
	if(l->size == 0) {
		l->head = calloc(1, sizeof(Node));
		l->head->data = data;
		l->tail = l->head;
		l->head->next = NULL;

		l->size++;
	} else {
		Node *it = l->head;
		while(it->next != NULL)
			it = it->next;

		it->next = calloc(1, sizeof(Node));
		it = it->next;
		it->data = data;
		it->next = NULL;

		l->tail = it;
		l->size++;
	}
}

int List_contains(List *l, void *data)
{

	if(l->size == 0)
		return 0;
	
	Node *it = l->head;
	
	while(it) {
		if(eq(data, it->data))
			return 1;
		it = it->next;
	}
	
	return 0;
}
void List_addonce(List *l, void *data)
{
		
	if(List_contains(l, data))
		return;
	
	List_add(l, data);
}
	

void List_rm(List *l, void *data)
{
	if(!l)
		return;
		
	if(l->size == 0)
		return;

	if(l->head->data == data) {
		Node *tmp = l->head;
		l->head = l->head->next;

		free(tmp->data);
		free(tmp);
		l->size--;
	} else {
		Node *itprev = l->head;
		Node *it = itprev->next;

		while(it != NULL) {
			if(it->data == data) {
				itprev->next = it->next;

				free(it->data);
				free(it);
				l->size--;
				break;
			}

			itprev = it;
			it = it->next;
		}
	}
}

void List_destroy(List *l)
{
	while(l->size > 0)
		List_rm(l, l->head->data);
}
