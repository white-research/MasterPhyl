#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "matrix.h"
#include "tree.h"
#include "analysis.h"
#include "output.h"



int main(void){ //TODO take arguments
    srand(time(NULL));
    printf("Working\n");
    float states[] = {//Partition 1
      0,0, 0,0, //t1
      0,0, 1,1, //t2
      1,1, 1,1, //t3
     
      //Partition 2
      0.0,0.0, 1.0,1.0, //t1
      1.0,1.0, 3.2,3.2, //t2
      0.8,0.8, 2.1,2.1, //t3
      
      //Partition 3
      1,1, 1.3,1.3,   2,2, 2.6,2.6, //t1
      1,1, 1.3,1.3,   1,1, 1.3,1.3,   2,2, 2.6,2.6, //t2
      1,1, 1.3,1.3,   2,2, 2.6,2.6 //t3
      };
    int pt[] = {0,1,2};
    Matrix *new_m = make_matrix(3, 6, 3, pt, new int[3] {2,2,2},  new int[9] {0,0,0, 0,0,0, 2,3,2}, new int[3] {0,0,2}, states);
    print_matrix(new_m);
    printf("\n");
    printf("Making random tree\n");
    // TODO: read in data
    
    TreeSet final_trees = run_analysis(new_m, 5, 100); 
    
    printf("Final cost of trees is %f\n\n", final_trees.tcost);
    
    FILE *f = fopen("output.tre", "w");
    assert(f != NULL);
    for (int idx = 0; idx < final_trees.ntrees; idx++){
        char *tre = to_newick(final_trees.trees[idx], final_trees.trees[idx]->root_node);
        printf("%s\n",  tre);
        print_tree(final_trees.trees[idx], final_trees.trees[idx]->root_node, 0);
        fprintf(f, "%s\n", tre);
        free_tree(final_trees.trees[idx]);
    }
    fclose(f);
    Matrix_destroy(new_m);
    return 1;
}
