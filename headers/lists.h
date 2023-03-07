#ifndef  FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include <stdlib.h>

// list structure
typedef struct node {
    long long value;
    struct node* next;
} node;

//append value to the end of the list
void append(node** list, long long value) {
    // create a new node
    node* new_node = (node*)malloc(sizeof(node));
    new_node->value = value;
    new_node->next = NULL;

    // if the list is empty, make the new node the head
    if (*list == NULL) {
        *list = new_node;
        return;
    }

    // if first node is -1 change its value
    if ((*list)->next == NULL && (*list)->value == -1) {
        (*list)->value = value;
        return;
    }

    // go to the end of the list
    node* last = *list;
    while (last->next != NULL) {
        last = last->next;
    }
    // append the new node
    last->next = new_node;
}

//remove duplicates from the list
void removeDuplicates(node** list) {
    node* ptr1, * ptr2, * duplicate;
    ptr1 = *list;

    // pick elements one by one
    while (ptr1 != NULL && ptr1->next != NULL) {
        ptr2 = ptr1;
        // have the second pointer compare the first pointer with the rest of the list
        while (ptr2->next != NULL) {
            if (ptr1->value == ptr2->next->value) {
                // if duplicate is found, remove it by changing the next pointer and freeing the memory
                duplicate = ptr2->next;
                ptr2->next = ptr2->next->next;
                free(duplicate);
                continue;
            }
            ptr2 = ptr2->next;
        }
        // move the first pointer to the next element
        ptr1 = ptr1->next;
    }

    return;
}

// concatenate two lists
void concatenateLists(node** list1, node** list2) {
    node* last = *list1;
    // go to the end of the first list
    while (last->next != NULL) {
        last = last->next;
    }
    // append the second list to the first one
    last->next = *list2;
}

// initialize every list of the array with -1
void init2dArray(node* list[1000][1000], long long nrReducers, long long nrMappers) {
    for (long long i = 0; i < nrMappers; i++) {
        for (long long j = 0; j < nrReducers; j++) {
            append(&list[i][j], -1);
        }
    }
}

// initialize every list of the array with -1
void initArray(node* list[1000], long long nrReducers, long long nrMappers) {
    for (long long j = 0; j < nrReducers; j++) {
        append(&list[j], -1);
    }
}

// count the number of elements in the list
long long countList(node* list) {
    long long count = 0;
    node* current = list;
    while (current->next != NULL) {
        current = current->next;
        count++;
    }
    return count;
}
#endif
