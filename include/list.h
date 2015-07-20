#ifndef __OCTO_LIST_H__
#define __OCTO_LIST_H__

typedef struct _list
{
    void *data;
    struct _list *front;
    struct _list *back;
    struct _list *prev;
    struct _list *next;
} list_t;

typedef void (*list_destroy_callback)(void * data);

list_t * list_create(void);
void list_destroy(list_t * list, list_destroy_callback callback);
int list_append(list_t * list, void * data);
int list_extend(list_t * list, list_t * list2);

#endif

