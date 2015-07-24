#include <stdio.h>

typedef struct partitionTag {
    int data_type, nchar, ntaxa;
    int *segments;
    float *states;
} partition;


partition new_partition(int d, int c, int t, int *segs, float *s){
    return (partition){.data_type=d, .ntaxa=t, .nchar=c, .segments=segs, .states=s};
}



typedef struct matrixTag {
    int ntaxa, nchar, npart;
    partition *partitions[10]; // discrete, continuous, and serial
} matrix;


matrix new_matrix(int t, int c){
    return (matrix){.ntaxa=t, .nchar=c, .npart=0};
}



int add_partition(matrix *m, partition p){
    if (m->ntaxa != p.ntaxa){ // check that no. of taxa match
        printf("Numbers of taxa don't match\n");
        return 1;
    }
    // check that there is space for partitions in the matrix
    if (m->ntaxa > 10){
        printf("There are too many partitions (max 10)\n")
        return 1;
    // check that the total number of characters doesn't exceed the matrix
    
    // check each taxon has the right number of characters
    
    // check that there are the right number of taxa
    
    // finally, add matrix
    m->partitions[m->npart] = &p; //pointer to partition struct
    m->npart = m->npart + 1;
    
    return 0;
}


void print_matrix(matrix m){
    printf("Taxa:%i, Characters: %i, Partitions: %i\n", m.ntaxa, m.nchar, m.npart);
    for(int p_count=0; p_count < m.npart; p_count++){
        printf("  Partition %i\n", p_count+1);
        printf("    Num. chars: %i\n", m.partitions[p_count]->nchar);
        printf("    First state: %f\n", *(m.partitions[p_count]->states) );
    }
}


void main(){
    int t = 10, c = 100;
    matrix m = new_matrix(t, c);
    print_matrix(m);
    
    float ss[2][2] = {{1,2},{3,4}};
    partition pp = new_partition(0, 10, 10, &ss[0][0]);
    add_partition(&m, pp);
    print_matrix(m);
    printf("\n");
}
