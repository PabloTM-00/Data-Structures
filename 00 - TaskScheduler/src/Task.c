//
// Pepe Gallardo, Data Structures, UMA.
//

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Task.h"
#include "test/unit/UnitTest.h"

struct Task *Task_new(unsigned int id, const char name[], unsigned int quantum)
{

  // Necessary checks
  assert(quantum > 0 && "Task New: quantum must be > 0");
  assert(name != NULL && "Task New: name cannot be a NULL pointer");

  // "It must validate that the name is not longer than MAX_NAME_LEN"
  assert(strlen(name) <= MAX_NAME_LEN && "Task New: name is too long");

  struct Task* p_task = malloc(sizeof(struct Task));
  assert(p_task != NULL && "Task_new: Out of memory");

  p_task->id = id;
  p_task->quantum = quantum;

  // We cant assign an array (of strings) to another one, so we use strcpy
  //p_task->name = name; is WRONG
  strcpy(p_task->name,name);
  
  return p_task;
}

void Task_free(struct Task **p_p_task)
{
  // Pointer to pointer as a parameter
  assert(p_p_task != NULL && "Task_free: p_p_task is NULL");

  struct Task* p_task = *p_p_task; // dereference 
  assert(p_task != NULL && "Task_free: p_task is NULL");

  free(p_task); // OR *p_p_task

  *p_p_task = NULL;
  // p_task = NULL would not work, because we would be modifying the local variable/ a copy of the pointer, not the actual one

}

struct Task *Task_copyOf(const struct Task *p_task)
{
  assert(p_task != NULL && "Task_copyOf: p_task is NULL");
  return Task_new(p_task->id,p_task->name,p_task->quantum);
}

void Task_print(const struct Task *p_task)
{
  // already implemented
  assert(p_task != NULL && "Task_print: NULL p_task"); // Cant be null pointer
  printf("Task(ID: %u, Name: %s, Quantum: %u)", p_task->id, p_task->name, p_task->quantum);
}
