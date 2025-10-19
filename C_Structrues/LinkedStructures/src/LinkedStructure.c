#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "LinkedStructure.h"

static struct Node* Node_new(int element, struct Node* p_next) {
  struct Node* p_node = malloc(sizeof(struct Node));
  assert(p_node != NULL && "Node_new: not enough memory");
  p_node->element = element;
  p_node->p_next = p_next;
  return p_node;
}

static void Node_free(struct Node** p_p_node) {
  free(*p_p_node);
  *p_p_node = NULL;
}

struct Node* LinkedStructure_new() {
  // struct Node* p_first = NULL; is whats represented by this function
  return NULL; 
}

struct Node* LinkedStructure_copyOf(const struct Node* p_first) {
    if (p_first == NULL) {
        return NULL; // Empty list
    }

    struct Node* copy_first = Node_new(p_first->element, NULL);
    struct Node* copy_current = copy_first;

    const struct Node* orig_current = p_first->p_next;
    while (orig_current != NULL) {
        copy_current->p_next = Node_new(orig_current->element, NULL);
        copy_current = copy_current->p_next;
        orig_current = orig_current->p_next;
    }

    return copy_first;
}



bool LinkedStructure_isEmpty(const struct Node* p_first) {
  return p_first == NULL;
}

size_t LinkedStructure_size(const struct Node* p_first) {
  size_t counter = 0;
  for (const struct Node* p = p_first; p != NULL; p = p->p_next)
    counter++;
  return counter;
}

void LinkedStructure_clear(struct Node** p_p_first) {
  assert(p_p_first != NULL && "LinkedStructure_clear: p_p_first is NULL");
  
  struct Node* current = *p_p_first;

  while(current != NULL){
    struct Node* p_next = current->p_next;
    free(current);
    current = current->p_next;
  }
  *p_p_first = NULL;
}

void LinkedStructure_prepend(struct Node** p_p_first, int element) {
  assert(p_p_first != NULL && "LinkedStructure_prepend: p_p_first is NULL");
  
  struct Node* p_node = Node_new(element, *p_p_first);
  *p_p_first = p_node;
}

void LinkedStructure_append(struct Node** p_p_first, int element) {
  assert(p_p_first != NULL && "LinkedStructure_append: p_p_first is NULL");
  
  struct Node* p_new = Node_new(element,NULL);

  if(*p_p_first == NULL){ // Empty list
    *p_p_first = p_new;
    return;
  }

  struct Node* current = *p_p_first;
  while(current->p_next != NULL){
    current = current->p_next;
  }
  // We exit the loop just before the node we want to append

  current->p_next = p_new;
}

void LinkedStructure_insert(struct Node** p_p_first, size_t index, int element) {
    assert(p_p_first != NULL && "LinkedStructure_insert: p_p_first is NULL");

    if (index == 0) {
        LinkedStructure_prepend(p_p_first, element);
        return;
    }

    struct Node* p_current = *p_p_first;
    for (size_t i = 0; i < index - 1; i++) {
        assert(p_current != NULL && "LinkedStructure_insert: index out of bounds");
        p_current = p_current->p_next;
    }

    struct Node* p_node = Node_new(element, p_current->p_next);
    p_current->p_next = p_node;
}

int LinkedStructure_get(const struct Node* p_first, size_t index) {
    assert(p_first != NULL && "LinkedStructure_get: p_first is NULL");

    const struct Node* p_current = p_first;
    for (size_t i = 0; i < index; i++) {
        assert(p_current != NULL && "LinkedStructure_get: index out of bounds");
        p_current = p_current->p_next;
    }

    assert(p_current != NULL && "LinkedStructure_get: index out of bounds");
    return p_current->element;
}

void LinkedStructure_set(struct Node* p_first, size_t index, int element) {
    assert(p_first != NULL && "LinkedStructure_set: p_first is NULL");

    struct Node* p_current = p_first;
    for (size_t i = 0; i < index; i++) {
        assert(p_current != NULL && "LinkedStructure_set: index out of bounds");
        p_current = p_current->p_next;
    }

    assert(p_current != NULL && "LinkedStructure_set: index out of bounds");
    p_current->element = element;
}

void LinkedStructure_delete(struct Node** p_p_first, size_t index) {
    assert(p_p_first != NULL && "LinkedStructure_delete: p_p_first is NULL");
    assert(*p_p_first != NULL && "LinkedStructure_delete: empty list");

    struct Node* p_current = *p_p_first;

    if (index == 0) { // borrar cabeza
        *p_p_first = p_current->p_next;
        free(p_current);
        return;
    }

    struct Node* p_prev = NULL;
    for (size_t i = 0; i < index; i++) {
        assert(p_current != NULL && "LinkedStructure_delete: index out of bounds");
        p_prev = p_current;
        p_current = p_current->p_next;
    }

    assert(p_current != NULL && "LinkedStructure_delete: index out of bounds");
    p_prev->p_next = p_current->p_next;
    free(p_current);
}

void LinkedStructure_print(const struct Node* p_first) {
  printf("LinkedStructure(");
  for (const struct Node* p = p_first; p != NULL; p = p->p_next) {
    printf("%d", p->element);
    if (p->p_next != NULL)
      printf(", ");
  }
  printf(")");
}
