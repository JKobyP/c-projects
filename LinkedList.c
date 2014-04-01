#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Person{
	char *name;
	int num;
};

struct Person *Person_create(char *name, int num)
{
	struct Person *who = malloc(sizeof(struct Person));
	assert(who != NULL);

	who->name = strdup(name);
	who->num = num;
	return who;
}

void Person_destroy(struct Person *who)
{
	assert(who != NULL);

	free (who->name);
	free(who);
}

struct PersonNode{
	struct Person *value;
	struct PersonNode *next;
};

struct PersonNode *PersonNode_create(struct Person *value)
{
	struct PersonNode *node = malloc(sizeof(struct PersonNode));
	assert (node != NULL);

	node->value = value;
	node->next = NULL;

	return node;
}


void setNext(struct PersonNode *node, struct PersonNode *next)
{
	assert (node != NULL && next != NULL);
	
	node->next = next;
}

void PersonNode_destroy(struct PersonNode *node)
{
	assert(node != NULL);

	struct Person *toDestroy = (node->value);
	Person_destroy(toDestroy);
	if(node->next != NULL)
	{
		struct PersonNode *nodeToDestroy = node->next;
		PersonNode_destroy(nodeToDestroy);
	}	
	free(node);
}

struct LinkedList{
	struct PersonNode *head;
	int size;
};

struct LinkedList *LinkedList_create()
{
	struct LinkedList *list = malloc(sizeof(struct LinkedList));
	assert (list != NULL);
	
	list->size = 0;
	list->head = NULL;
	return list;
}
void insert(struct LinkedList *list, struct PersonNode *node, int i)
{
	assert(list != NULL);
	assert(node != NULL);

	if(list->size == 0)
	{
		list->head = node;
		list->size++;

		return;
	}
//	printf("flag");
	if(i > list->size)
		i = list->size;
	if(i == 0)
	{
		setNext(node, list->head);
		list->head = node;
	}
	else
	{
		int j;
		struct PersonNode *temp = list->head;
		for(j = 0; j < (i-1); j++)
		{
			temp = temp->next;
			printf("loop");
		}
//		printf("%s is temp before reassignment, %s is temp.next before", temp->value->name, temp->next->value->name);
		setNext(node, temp->next);
		setNext(temp, node);
//		printf("%s is temp, %s is node, %s is temp.next, %s is node.next", temp->value->name, node->value->name, temp->next->value->name, node->next->value->name);
	}

	list->size++;
}
void LinkedList_destroy(struct LinkedList *list)
{
	assert(list != NULL);
	struct PersonNode *head = list->head;
	
	if(head!=NULL)
		PersonNode_destroy(head);
	free(list);
}

int main(int argc, char *argv[])
{
	struct Person *joe = Person_create("Joe Alex", 216);

	struct PersonNode *head = PersonNode_create(joe);
	struct LinkedList *list = LinkedList_create();
	insert(list, head, 0);
	printf("%s \n", head->value->name);
	insert(list, PersonNode_create(Person_create("Jeremy", 330)), 0);
	insert(list, PersonNode_create(Person_create("Kakarot", 383)), 1);
	printf("%s \n %s \n %s", list->head->value->name, list->head->next->value->name, list->head->next->next->value->name);
	LinkedList_destroy(list);
	return 0;
}
