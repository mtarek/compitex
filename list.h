//typedef struct List List;
typedef struct Node Node;

struct Node
{
	void *data;
	Node *next;
};

typedef struct List
{
	Node *head;
	Node *tail;

	unsigned int size;
}List;

void List_init(List *l, int (*eq) (void *d1, void *d2));
void List_add(List *l, void *data);
void List_addonce(List *l, void *data);
void List_rm(List *l, void *data);
void List_destroy(List *l);