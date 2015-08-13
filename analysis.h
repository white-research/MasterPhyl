#ifndef _analysis_h
#define _analysis_h

#include "matrix.h"
#include "tree.h"

extern TreeSet spr(Tree *start_tree, Matrix *m, float current_best_cost);

float continuous_downpass(Tree *t, Partition *p, int char_idx, Node *n, float *node_states);

#endif
