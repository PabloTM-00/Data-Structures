//
// Pepe Gallardo, Data Structures, UMA.
//

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "Scheduler.h"
#include "test/unit/UnitTest.h"

struct Node *Scheduler_new()
{
  return NULL;
}

size_t Scheduler_size(const struct Node *p_last)
{
  size_t counter = 0;

  if(p_last != NULL){
    // scheduler is non-empty
    struct Node* p_first = p_last->p_next; // always points to first 
    struct Node* p_current = p_first; // used to access different nodes
    do{
      counter++;
      p_current = p_current->p_next;
    } while(p_current != p_first);
  }
  return counter;
}

void Scheduler_clear(struct Node **p_p_last)
{
  assert(p_p_last != NULL && "Scheduler_clear: p_p_last is NULL");

  struct Node* p_last = *p_p_last;
  if(p_last != NULL){
    // Scheduler non-empty
    struct Node* p_first = p_last->p_next;
    struct Node* p_current = p_first;
    do{
      struct Node* p_next = p_current->p_next;
      free(p_current);
      p_current = p_next;
      
    } while(p_current != p_first);
    *p_p_last = NULL;
  }
}

struct Task *Scheduler_first(const struct Node *p_last)
{
  assert(p_last != NULL && "Scheduler_first: p_last is NULL");

  struct Node* p_first = p_last->p_next;

  return   Task_copyOf(&(p_first->task)); // & to return the address of the variable, the task in this case
}

void Scheduler_enqueue(struct Node **p_p_last, const struct Task *p_task)
{
  assert(p_p_last != NULL && "Scheduler_enqueue: p_p_last is NULL");
  assert(p_task != NULL &&  "Scheduler_enqueue: p_task is NULL");

  struct Node* p_new_node = malloc(sizeof(struct Node)); 
  assert(p_new_node != NULL && "Scheduler_enqueue: out of memory");

  p_new_node->task = *p_task;

  if (*p_p_last == NULL) {
    p_new_node->p_next = p_new_node;
    *p_p_last = p_new_node;
  } else {
    p_new_node->p_next = (*p_p_last)->p_next; // points to first
    (*p_p_last)->p_next = p_new_node;
    *p_p_last = p_new_node;
  }
}

void Scheduler_dequeue(struct Node **p_p_last) {
  assert(p_p_last != NULL && "Scheduler_dequeue: p_p_last is NULL");

  struct Node* p_last = *p_p_last;
  assert(p_last != NULL && "Scheduler_dequeue: scheduler is empty");

  struct Node* p_first = p_last->p_next;

  if (p_first == p_last) {
    free(p_first);
    *p_p_last = NULL;
  } else {
    p_last->p_next = p_first->p_next;
    free(p_first);
  }
}

void Scheduler_print(const struct Node *p_last)
{
  // already implemented
  printf("Scheduler(");
  if (p_last != NULL)
  {
    struct Node *p_first = p_last->p_next;
    struct Node *p_current = p_first;
    do
    {
      printf("\n  ");
      Task_print(&(p_current->task));
      p_current = p_current->p_next;
      printf(p_current != p_first ? "," : "\n");
    } while (p_current != p_first);
  }
  printf(")");
}