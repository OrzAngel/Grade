// thread_list.c
/*
 * A linkedlist of pointer of pthreads
 * When the outside user want a new thread the tl_get() function should be called.
 * A new node is created and appended to the list. And the pointer to the new pthread is returned. 
 * Once the server stopped dealing with request, the tl_clear() function should be called to join all threads and free all memory   
 */
#include <stdlib.h>
#include <pthread.h>
#include "thread_list.h"

node * tl_head;
node * tl_tail;

/*
 * Initialize the linkedlist, create an empty sentinel node. 
 */
void tl_init()
{

	tl_head = malloc(sizeof(node));
	if (!tl_head)
	{
		return NULL;
	}
	tl_head -> thread = NULL;
	tl_head -> next   = NULL;

	tl_tail = tl_head;
}

/*
 * allocate memory for a new node and a new thread, append the new node to the tail of the linkedlist.
 * return a pointer to the new pthread
 */
pthread_t* tl_get()
{
	node* next = malloc(sizeof(node));
	if (!next) 
	{
		return NULL;
	}
	next -> thread = malloc(sizeof(pthread_t));
	if (!next -> thread)
	{
		free(next);
		return NULL;
	}
	next -> next   = NULL;
	tl_tail -> next = next;
	tl_tail = next;

	return tl_tail -> thread;
} 

/*
 * Join and free all the threads in the linkedlist from head to tail
 * After this function being called the linkedlist is reset to an empty list 
 */
void tl_join() 
{
	node * curr = tl_head -> next;

	tl_head -> next = NULL;
	tl_tail = tl_head;

	node * next;
	while (curr)
	{
		pthread_join(*(curr->thread),NULL);
		free(curr->thread);
		next = curr -> next;
		free(curr);
		curr = next;
	}
}

/*
 * free all the memory allcoated for the linkedlist 
 */
void tl_clear()
{
	if (tl_head != tl_tail)
	{
		tl_join();
	}

	free(tl_head);
}