//
// Pepe Gallardo, Data Structures, UMA.
//
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "LinkedList.h"
#include "test/unit/UnitTest.h"

/// @brief Creates a new Node with given element and pointer to next Node
/// @param element Element to be stored in Node
/// @param p_next Pointer to next Node to be stored in Node
/// @return Pointer to new Node
static struct Node* Node_new(int element, struct Node* p_next) {
    struct Node* p_node = malloc(sizeof(struct Node));
    assert(p_node != NULL && "Node_new: not enough memory");

    p_node->element = element;
    p_node->p_next = p_next;
    return p_node;
}

/// @brief Frees memory allocated for a Node. Must also set pointer to Node to NULL
/// @param p_p_node Pointer to pointer to Node
static void Node_free(struct Node** p_p_node) {
    free(*p_p_node);
    *p_p_node = NULL;
}

/// @brief Validates index in LinkedList. 
/// @param p_list Pointer to LinkedList
/// @param index Index to be validated
static void validateIndex(const struct LinkedList* p_list, size_t index) {
    assert(index < p_list->size && "validateIndex: invalid index");
}

struct LinkedList* LinkedList_new() {
    struct LinkedList* p_list = malloc(sizeof(struct LinkedList));
    assert(p_list != NULL && "LinkedList_new: not enough memory");
    p_list->p_first = NULL;
    p_list->p_last = NULL;
    p_list->size = 0;
    return p_list;
}

struct LinkedList* LinkedList_copyOf(const struct LinkedList* p_list) {
    assert(p_list != NULL && "LinkedList_copyOf: invalid list");

    struct LinkedList* copy = LinkedList_new();
    struct Node* current = p_list->p_first;

    for(size_t i = 0; i < p_list->size; i++){
        LinkedList_append(copy,current->element);
        current = current->p_next;
    }

    return copy;
}

bool LinkedList_isEmpty(const struct LinkedList* p_list) {
    assert(p_list != NULL && "LinkedList_isEmpty: invalid list");
    return p_list->size == 0; 
}

size_t LinkedList_size(const struct LinkedList* p_list) {
    assert(p_list != NULL && "LinkedList_size: invalid list");
    return p_list->size;
}

void LinkedList_prepend(struct LinkedList* p_list, int element) {
    assert(p_list != NULL && "LinkedList_prepend: invalid list");

     struct Node* p_new = Node_new(element,p_list->p_first);
     p_list->p_first = p_new;
     
    if(p_list->size == 0){ // If list empty, p_last must point to new node   
        p_list->p_last = p_new;
    }
     
    p_list->size++;
}

void LinkedList_append(struct LinkedList* p_list, int element) {
    assert(p_list != NULL && "LinkedList_append: invalid list");

    struct Node* p_new = Node_new(element,NULL);

    if(p_list->size == 0){ //List is empty so new node is first one and also last one
        p_list->p_first = p_new;
        p_list->p_last = p_new;
    }
    else{
        p_list->p_last->p_next = p_new;
        p_list->p_last = p_new;
    }

    p_list->size++;
}

void LinkedList_insert(struct LinkedList* p_list, size_t index, int element) {
    assert(p_list != NULL && "LinkedList_insert: invalid list");
    assert(index <= p_list->size && "LinkedList_insert: invalid index"); 
     
    if (index == 0) {
        LinkedList_prepend(p_list, element);
        return;
    }
    if (index == p_list->size) {
        LinkedList_append(p_list, element);
        return;
    }

    struct Node* current = p_list->p_first;
    for (size_t i = 0; i < index - 1; i++) {
        current = current->p_next;
    }

    struct Node* p_new = Node_new(element, current->p_next);
    current->p_next = p_new;
    p_list->size++;
}


void LinkedList_insert_sorted(struct LinkedList *p_list, int element) {
    assert(p_list != NULL && "LinkedList_insert_sorted: p_list is NULL");

    if(p_list->p_first == NULL || element <= p_list->p_first->element){
        LinkedList_prepend(p_list,element);
        return;
    }

    struct Node* current = p_list->p_first;

    while(current->p_next != NULL && current->p_next->element < element){
        current = current->p_next;
    }
    //When exiting the loop, it will be in the node before the one we must insert new element

    struct Node* p_new = Node_new(element, current->p_next);

    current->p_next = p_new;

    p_list->size++;
}

int LinkedList_get(const struct LinkedList* p_list, size_t index) {
    assert(p_list != NULL && "LinkedList_get: invalid list");
    assert(index < p_list->size && "LinkedList_get: invalid index"); 

    struct Node* current = p_list->p_first;

    for (size_t count = 0; count < index; count++) {
        current = current->p_next;
    }

    return current->element;
}

void LinkedList_set(struct LinkedList* p_list, size_t index, int element) {
    assert(p_list != NULL && "LinkedList_set: invalid list");
    assert(index < p_list->size && "LinkedList_set: invalid index");  

    struct Node* current = p_list->p_first;

    for(size_t i = 0; i < index; i++){
        current = current->p_next;
    }

    current->element = element;
}

void LinkedList_delete(struct LinkedList* p_list, size_t index) {
    assert(p_list != NULL && "LinkedList_delete: invalid list");
    assert(index < p_list->size && "LinkedList_delete: invalid index");

    struct Node* to_delete;

    if (index == 0) {
        to_delete = p_list->p_first;
        p_list->p_first = p_list->p_first->p_next;

        if (p_list->size == 1) {  
            p_list->p_last = NULL;
        }

        free(to_delete);
    } else {
        struct Node* current = p_list->p_first;
        for (size_t i = 0; i < index - 1; i++) {
            current = current->p_next;
        }

        to_delete = current->p_next;
        current->p_next = to_delete->p_next;

        if (index == p_list->size - 1) {  
            p_list->p_last = current;
        }

        free(to_delete);
    }

    p_list->size--;
}


void LinkedList_print(const struct LinkedList* p_list) {
    assert(p_list != NULL && "LinkedList_print: invalid list");
    printf("LinkedList(");
    struct Node* p_node = p_list->p_first;
    while (p_node != NULL) {
        printf("%d", p_node->element);
        if (p_node->p_next != NULL) {
            printf(", ");
        }
        p_node = p_node->p_next;
    }
    printf(")");
}

void LinkedList_clear(struct LinkedList* p_list) {
    assert(p_list != NULL && "LinkedList_clear: invalid list");

    struct Node* current = p_list->p_first;

    while (current != NULL) {
        struct Node* delete = current;   
        current = current->p_next;           
        free(delete);                     
    }

    p_list->p_first = NULL;
    p_list->p_last = NULL;
    p_list->size = 0;
}

void LinkedList_free(struct LinkedList** p_p_list) {
    assert(p_p_list != NULL && "LinkedList_free: invalid p_p_list");
    assert(*p_p_list != NULL && "LinkedList_free: p_list is NULL");

    LinkedList_clear(*p_p_list);
    free(*p_p_list);
    *p_p_list = NULL;
}
