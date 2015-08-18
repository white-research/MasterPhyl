#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "matrix.h"


Partition *create_partition(int d, int c, int t, int *segs, float **s){
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
}


void print_matrix(Matrix *m){
    printf("Taxa:%i, Characters: %i, Partitions: %i\n", m->ntaxa, m->nchar, m->npart);
    for(int p_count=0; p_count < m->npart; p_count++){
        printf("  Partition %i\n", p_count+1);
        printf("    Num. chars: %i\n",  m->partitions[p_count]->nchar);
        printf("    States:\n");
        for (int t_count = 0; t_count < m->ntaxa; t_count++){
            float *t_states = m->partitions[p_count]->states[t_count];
            printf("     %f\n", t_states[0]);
        }
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
    //printf("Destroying partition with dtype %i\n", p->data_type);
    if (p->data_type == 2){
        //printf(" Freeing segments\n");
        free(p->segments);
    }
    for (int idx = 0; idx < p->ntaxa; idx++){
        //printf(" Freeing states for taxon %i\n", idx);
        free(p->states[idx]);
    }
    //printf("Free main states array\n");
    free(p->states);
    free(p);
}


void Matrix_destroy(Matrix *m){
    for(int p_count=0; p_count < m->npart; p_count++){
        //printf("Destroying partition %i\n", p_count);
        Partition_destroy(m->partitions[p_count]);
    }
    //printf("Free matrix\n");
    free(m);
}


Matrix *make_matrix(int ntaxa, int nchar, int npart, int part_types[], int chars_per_part[], int segs_per_taxon_per_part[], int chars_per_seg[], float all_states[]){
    Matrix *m = create_matrix(ntaxa, nchar);
    int partition_start=0;
    
    for (int p_count = 0; p_count < npart; p_count++){
        printf("Making array for partition %i\n", p_count);
        Partition *new_partition;
        assert(part_types[p_count] == 0 || part_types[p_count] == 1 || part_types[p_count] == 2);
        if (part_types[p_count] == 0 || part_types[p_count] == 1){
            float **seq_array = (float **)malloc( ntaxa * sizeof(float *));
            int *segs = NULL;
            float *states;
            int partition_length = ntaxa*chars_per_part[p_count]*2;
            printf("Total length of partition is %i\n", partition_length);
            int taxon_start_tracker = 0;
            for (int idx=0; idx<ntaxa; idx++){
                printf("On taxon %i :", idx);
                states = malloc(sizeof(float)*chars_per_part[p_count]*2);
                assert(states != NULL);
                for (int idx2 = 0; idx2 < chars_per_part[p_count]*2; idx2++) {
                    states[idx2] = all_states[partition_start+taxon_start_tracker+idx2];
                    printf("%1.3f ", all_states[partition_start+taxon_start_tracker+idx2]);
                }
                printf("\n");
                taxon_start_tracker += chars_per_part[p_count]*2;
                seq_array[idx] = states;
            }
            
            /*
            int partition_length = ntaxa*chars_per_part[p_count]*2;
            float *states = malloc(sizeof(float)*partition_length);
            for (int idx = 0; idx < partition_length; idx++){
                states[idx] = all_states[partition_start+idx];
                printf("%1.3f ", all_states[partition_start+idx]);
            }
            */
            
            printf("\n");
            new_partition = create_partition(part_types[p_count], chars_per_part[p_count], ntaxa, segs, seq_array);
            partition_start += partition_length;
            assert(partition_length == taxon_start_tracker);
        }
        else {
            float **seq_array = (float **)malloc( ntaxa * sizeof(float *));
            assert(seq_array != NULL);
            int *segs = malloc(sizeof(int)*ntaxa), taxon_length;
            assert(segs != NULL);
            int partition_length = 0; // track total length of partition and where each taxon starts
            for (int idx=0; idx<ntaxa; idx++){ // loop through taxa
                printf("On taxon %i :", idx);
                segs[idx]=segs_per_taxon_per_part[idx+p_count*ntaxa]; // record number of segments in taxon
                taxon_length = segs_per_taxon_per_part[idx+p_count*ntaxa]*chars_per_part[p_count]*2;
                
                float *states = malloc(sizeof(float)*taxon_length);
                assert(states != NULL);
                for (int idx2 = 0; idx2 < taxon_length; idx2++) {
                    states[idx2] = all_states[partition_start+partition_length+idx2];
                    printf("%1.3f ", all_states[partition_start+partition_length+idx2]);
                }
                printf("\n");
                partition_length += taxon_length;
                seq_array[idx] = states;
            }
            
            
            /*
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
            */
            
            new_partition = create_partition(part_types[p_count], chars_per_part[p_count], ntaxa, segs, seq_array);
            partition_start += partition_length;
        }
        add_partition(m, new_partition);
    }
    return m;
}



















