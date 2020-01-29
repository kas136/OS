#define _GNU_SOURCE

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

void change_header(size_t size, void *ptr, unsigned long addr);
void *next_fit(size_t size);
int remake_free_list(size_t size);
void coalescing();
void *heap_make(size_t size);
void *malloc(size_t size);
void free(void *ptr);
