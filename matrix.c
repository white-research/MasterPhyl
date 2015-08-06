#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "matrix.h"


Partition *create_partition(int d, int c, int t, int *segs, float *s){
    Partition *new_partition = malloc(sizeof(Partition));
    assert(new_partition != NULL);
    
    new_partition->data_type = d;
    new_partition->nchar = c;
    new_partition->ntaxa = t;
    new_partition->segments = segs;
    new_partition->states = s;
    
    return new_partition;
}


Matrix *create_matrix(int t, int c){
    Matrix *new_matrix = malloc(sizeof(Matrix));
    assert(new_matrix != NULL);
    
    new_matrix->ntaxa = t;
    new_matrix->nchar = c;
    new_matrix->npart = 0;
    
    return new_matrix;
}


void add_partition(Matrix *m, Partition *p){
    printf("%i %i\n", m->ntaxa, p->ntaxa);
    assert( (m->ntaxa == p->ntaxa && "Number of taxa don't match")); // check that no. of taxa match
    assert( (m->npart < 11) && "You cannot add more than 10 partitions"); // check that there is space for partitions in the matrix
    // TO-DO:
    // check that the total number of characters doesn't exceed the matrix
    
    // check each taxon has the right number of characters
    
    // check that there are the right number of taxa
    
    // finally, add matrix
    m->partitions[m->npart] = p; //pointer to partition struct
    m->npart = m->npart + 1;
    
    return 0;
}


void print_matrix(Matrix *m){
    printf("Taxa:%i, Characters: %i, Partitions: %i\n", m->ntaxa, m->nchar, m->npart);
    for(int p_count=0; p_count < m->npart; p_count++){
        printf("  Partition %i\n", p_count+1);
        printf("    Num. chars: %i\n",  m->partitions[p_count]->nchar);
        printf("    First state: %f\n", *(m->partitions[p_count]->states) );
        if (m->partitions[p_count]->data_type == 2){
            printf("    Segments per taxon: ");
            for (int idx=0; idx < m->ntaxa; idx++){
                printf("%i ", m->partitions[p_count]->segments[idx]);
            }
            printf("\n");
        }
    }
}


void Partition_destroy(Partition *p){
    free(p);
}


void Matrix_destroy(Matrix *m){
    for(int p_count=0; p_count < m->npart; p_count++){
        Partition_destroy(m->partitions[p_count]);
        free(m);
    }
}


Matrix *make_matrix(int ntaxa, int nchar, int npart, int part_types[], int chars_per_part[], int segs_per_taxon_per_part[], int chars_per_seg[], float all_states[]){
    Matrix *m = create_matrix(ntaxa, nchar);
    int partition_start=0;
    for (int p_count = 0; p_count < npart; p_count++){
        Partition *new_partition;
        assert(part_types[p_count] == 0 || part_types[p_count] == 1 || part_types[p_count] == 2);
        if (part_types[p_count] == 0 || part_types[p_count] == 1){
            int partition_length = ntaxa*chars_per_part[p_count]*2;
            float *states = malloc(sizeof(float)*partition_length);
            for (int idx = 0; idx < partition_length; idx++){
                states[idx] = all_states[partition_start+idx];
                printf("%1.3f ", all_states[partition_start+idx]);
            }
            printf("\n");
            new_partition = create_partition(part_types[p_count], chars_per_part[p_count], ntaxa, NULL, states);
            partition_start += partition_length;
        }
        else {
            int partition_length = 0;
            int *segs = malloc(sizeof(int)*ntaxa);
            for (int idx=0; idx<ntaxa; idx++){
                partition_length += segs_per_taxon_per_part[idx+p_count*ntaxa]*chars_per_part[p_count]*2;
                segs[idx]=segs_per_taxon_per_part[idx+p_count*ntaxa];
            }
            float *states = malloc(sizeof(float)*partition_length);
            for (int idx = 0; idx < partition_length; idx++) {
                states[idx] = all_states[partition_start+idx];
                printf("%1.3f ", all_states[partition_start+idx]);
            }
            printf("\n");
            new_partition = create_partition(part_types[p_count], chars_per_part[p_count], ntaxa, segs, states);
            partition_start += partition_length;
        }
        add_partition(m, new_partition);
    }
    return m;
}

void tests(){
    int t = 10, c = 100;
    Matrix *m = create_matrix(t, c);
    print_matrix(m);
    
    float ss[4] = {1,2,3,4};
    int seg_counts[2] = {2,2};
    Partition *pp = create_partition(0, 10, 10, &seg_counts[0], &ss[0]);
    add_partition(m, pp);
    print_matrix(m);
    printf("\n");
    Matrix_destroy(m);
}
