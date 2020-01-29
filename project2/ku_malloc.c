/*#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>

typedef struct _hnode_t {
    size_t size;
    struct _hnode_t *next;
} hnode_t;

typedef struct _hheader_t {
    size_t size;
    unsigned long magic;
} hheader_t;

hnode_t *free_list;

hnode_t *first;

#define KU_MAGIC 0x19311946u
#define head_size 16
#define node_size sizeof(hheader_t)
*/

#include "ku_malloc.h"

void change_header(size_t size, void *ptr, unsigned long addr)
{
    hheader_t *head;
    head = (hheader_t *)ptr;
    head->size = size;
    head->magic = addr ^ KU_MAGIC;
}

void *next_fit(size_t size)
 {
     hnode_t *temp = first;
        
     while(temp != NULL)
     {
	if(temp->size >= size)
	{
	    first = temp;
	    return first;
	}

	else
	{
	    temp = temp->next;
	}
     }
     
     temp = free_list;

     while(temp != first)
     {
	 if(temp->size >= size)
	 {
	     first = temp;
	     return first;
	 }

	 else
	 {
	     temp = temp->next;
	 }
     }

     return NULL;
}

int remake_free_list(size_t size)
{
    if(first->size-size > head_size)
    {
	hnode_t *temp = free_list;

	while(temp != NULL)
	{
	    if(temp == first)
	    {
		void *ptr;
		ptr = ((int)temp + sizeof(hnode_t) + size);
		temp = (hnode_t *)ptr;
		temp->size = first->size - size - head_size;
		temp->next = first->next;

		if(free_list == first)
		{
		    first = temp;
		    free_list = first;
		}
		else
		{
		    first = temp;
		}

		return 1;
	    }

	    temp = temp->next;
	}
    }

    else
    {
	first = first->next;
	
	return 0;
    }
}


void coalescing()
{
    hnode_t *temp;
    temp = free_list;
    
    while(temp != NULL)
    {
	if(((unsigned long)temp + node_size + temp->size) == (unsigned long)temp->next)
	{
	    hnode_t *next = temp->next->next;
	    temp->size = temp->size + node_size + temp->next->size;
	    temp->next = next;
	}

	temp = temp->next;
    }
}

void *heap_make(size_t size)
{
    void *ptr;
    unsigned long addr;
    hheader_t *head;

    ptr = sbrk(size + head_size);

    if(ptr != -1)
    {
	head = (hheader_t *)ptr;
	head->size = size;
	addr = (unsigned long)ptr + head_size;
	head->magic = addr ^ KU_MAGIC;
	
	return head;
    }

    else
    {
	fprintf(stderr, "error\n");

	return NULL;
    }
}


void *malloc(size_t size)
{
    void *ptr;
    unsigned long addr;

    if(free_list == NULL)
    {
	ptr = heap_make(size);
	addr = (unsigned long)ptr + head_size;

	if(addr != 0)
	{
	    fprintf(stderr, "[Alloc] Addr: %p Length: %lu\n", addr, size);
	    
	    return (void *)addr;
	}

	else
	    return NULL;
    }

    else
    {
	ptr = next_fit(size);
	
	if(ptr == NULL)
	{
	    ptr = heap_make(size);
	    addr = (unsigned long)ptr + head_size;

	    if(addr != 0)
	    {
		fprintf(stderr, "[Alloc] Addr: %p Length: %lu\n", addr, size);
	    
		return (void *)addr;
	    }

	    else
		return NULL;
	}

	else
	{
	    addr = (unsigned long)ptr + head_size;
	    remake_free_list(size);
	    change_header(size, ptr, addr);
	    fprintf(stderr, "[Alloc] Addr: %p Length: %lu\n", addr, size);

	    return (void *)addr;
	}
    }
}

void free(void *ptr)
{
    unsigned long magic_num = (unsigned long)ptr ^ KU_MAGIC;
    hheader_t *check = ptr - node_size;

    if(check->magic == magic_num)
    {
	hnode_t *node;
	node = (hnode_t *)check;
	node->size = check->size;
	
	if(free_list != NULL)
	{
	    if(node < free_list)
	    {
		node->next = free_list;
		free_list = node;
	    }
	    else
	    {
		hnode_t *check;
		check = free_list;
		while(check->next != NULL)
		{
		    if(node < check->next)
		    {
			node->next = check->next;
			check->next = node;
			break;
		    }
		    else
			check = check->next;
		}
		
		if(check->next == NULL)
		{
		    check->next = node;
		    node->next = NULL;
		}
	    }
	}

	else
	{
	    free_list = node;
	    free_list->next = NULL;
	}

	fprintf(stderr, "[Free]  Addr: %p Length: %lu\n", ptr, node->size);
	coalescing();
    }

    else
	fprintf(stderr, "error\n");
}
