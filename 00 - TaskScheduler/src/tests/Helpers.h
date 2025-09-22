/*============================================================================*/
/* Helper functions for Task Scheduler                                        */
/* Pepe Gallardo, 2025                                                        */
/*============================================================================*/

#ifndef HELPERS_H
#define HELPERS_H

#include <stddef.h>

#include "Scheduler.h"
#include "Task.h"

struct Z {
  unsigned int i;
  char n[1 + MAX_NAME_LEN];
  unsigned int q;
};

struct X {
  struct Z t; 
  struct X* x;
};

struct Y {
  struct X* x; 
  size_t s;    
};

struct Z* _z(unsigned int i, const char n[], unsigned int q);
struct X* _n(const struct Task G[], size_t F);
void _p(char* H, size_t I, struct Y* A);
int _c(struct Y* F, struct Y* G);

#endif // HELPERS_H