#ifndef _analysis_h
#define _analysis_h

#include "matrix.h"
#include "tree.h"

extern TreeSet spr(Tree *start_tree, Matrix *m, float current_best_cost, int max_trees);
extern TreeSet run_analysis(Matrix *m, int spr_rounds, int max_trees);

extern void run_analysis_shared(float states[], int ntaxa, int nchar, int npart, int part_types[], int chars_per_part[], int segs_per_taxon_per_part[], int chars_per_seg[], int spr_rounds);

float continuous_downpass(Tree *t, Partition *p, int char_idx, Node *n, float *node_states);

#endif
