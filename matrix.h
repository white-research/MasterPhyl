#ifndef _matrix_h
#define _matrix_h


typedef struct partitionTag {
    int data_type, nchar, ntaxa;
    int *segments;
    float **states;
} Partition;


typedef struct matrixTag {
    int ntaxa, nchar, npart;
    Partition *partitions[10]; // discrete - 0, continuous - 1, and serial - 2
} Matrix;


extern Matrix *make_matrix(int ntaxa, int nchar, int npart, int part_types[], int chars_per_part[], int segs_per_taxon_per_part[], int chars_per_seg[], float all_states[]);
extern void Matrix_destroy(Matrix *m);
extern void print_matrix(Matrix *m);

#endif
