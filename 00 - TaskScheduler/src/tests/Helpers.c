/*============================================================================*/
/* Helper functions for Task Scheduler                                        */
/* Pepe Gallardo, 2025                                                        */
/*============================================================================*/

#include <string.h>

#include "Helpers.h"
#define UNIT_TEST_MEMORY_TRACKING
#include "test/unit/UnitTest.h"

struct Z* _z(unsigned int i, const char n[], unsigned int q) { struct Z* A = malloc(sizeof(struct Z)); A->i = i; strncpy(A->n, n, sizeof(A->n)); A->n[sizeof(A->n) - 1] = '\0'; A->q = q; return A; }
struct X* _x(struct Z B, struct X* C) { struct X* A = malloc(sizeof(struct X)); *A = (struct X){B, C}; return A; }
struct X* _n(const struct Task G[], size_t F) { if (!F) { return NULL; } struct X* B = NULL; struct X* C = NULL; B = _x(*(struct Z*)&G[0], NULL); C = B; for (size_t E = 1; E < F; E++) { C->x = _x(*(struct Z*)&G[E], NULL); C = C->x; } C->x = B; return (struct X*)C; }
void _p(char* H, size_t I, struct Y* A) { if (!A || !(A->x) || !(A->s)) { snprintf(H, I, "Scheduler()"); return; } size_t C = 0; int n = snprintf(H + C, I - C, "Scheduler("); if (n < 0 || (size_t)n >= I - C) return; C += n; struct X* B = A->x->x; for (size_t i = 0; i < A->s; i++) { n = snprintf(H + C, I - C, "\n  Task(ID: %u, Name: %s, Quantum: %u)", B->t.i, B->t.n, B->t.q); if (n < 0 || (size_t)n >= I - C) break; C += n; n = snprintf(H + C, I - C, i == A->s - 1 ? "\n" : ","); if (n < 0 || (size_t)n >= I - C) break; C += n; B = B->x; } snprintf(H + C, I - C, ")"); }
int _c(struct Y* F, struct Y* G) { if (F->s != G->s) return 0; if (!F->s) return 1; struct X* C = F->x->x; struct X* D = G->x->x; size_t A = F->s; while (A--) { if (C->t.i != D->t.i || strcmp(C->t.n, D->t.n) || C->t.q != D->t.q) { return 0; } C = C->x; D = D->x; } return 1; }