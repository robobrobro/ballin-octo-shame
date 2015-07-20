#include "list.h"
#include "debug.h"
#include "utils/color.h"

list_t * list_create(void)
{
    list_t *list = NULL;

    list = (list_t *)malloc(sizeof(list_t));
    if (!list) return NULL;

    list->data = NULL;
    list->front = NULL;
    list->back = NULL;
    list->prev = NULL;
    list->next = NULL;

    return list;
}

void list_destroy(list_t * list, list_destroy_callback callback)
{
    list_t *curr = NULL, *next = NULL;

    if (!list) return;

    curr = list->front;
    while (curr)
    {
        if (callback && curr->data) callback(curr->data);
        next = curr->next;
        free(curr); 
        curr = next;
    }

    free(list);
}

int list_append(list_t * list, void * data)
{
    list_t *curr = NULL, *back = NULL;

    if (!list) return 0;
    
    back = list->back;

    if (!back)
    {
        /* Create a new list to encapsulate the data and append it to the list */
        back = list_create();
        if (!back)
        {
            DEBUG_ERROR(L"failed to append data to list\n");
            return 0;
        }

        back->data = data;
        back->front = back;
        back->back = back;
        list->front = back;
        list->back = back;

        return 1;
    }
    else if (back->next)
    {
        DEBUG_ERROR(L"corrupt list %ls%p%ls: back (%ls%p%ls) has a next element (%ls%p%ls)\n",
            COLOR_RED, list, COLOR_END,
            COLOR_RED, back, COLOR_END,
            COLOR_RED, back->next, COLOR_END);
        return 0;
    }
    
    /* Create a new list to encapsulate the data and append it to the list */
    back->next = list_create();
    if (!back->next)
    {
        DEBUG_ERROR(L"failed to append data to list\n");
        return 0;
    }

    back->next->data = data;
    back->next->front = list->front;
    back->next->back = back;
    back->next->prev = back;
    list->back = back->next;

    /* Set the back of every list element to the new back */
    for (curr = list->front; curr->next; curr = curr->next)
    {
        curr->back = back->next;
    }

    return 1;
}

int list_extend(list_t * list, list_t * list2)
{
    list_t *curr = NULL;

    if (!list) return 0;

    curr = list->back;
    if (!curr)
    {
        DEBUG_ERROR(L"corrupt list %ls%p%ls: back is NULL\n", COLOR_RED, list, COLOR_END);
        return 0;
    }

    if (curr->next)
    {
        DEBUG_ERROR(L"corrupt list %ls%p%ls: back (%ls%p%ls) has a next element (%ls%p%ls)\n",
            COLOR_RED, list, COLOR_END,
            COLOR_RED, curr, COLOR_END,
            COLOR_RED, curr->next, COLOR_END);
        return 0;
    }

    /* Append list2 to the end of the list */
    curr->next = list2;
    curr->back = list2->back;

    return 1;
}

