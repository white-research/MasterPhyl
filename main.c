#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "matrix.h"
#include "tree.h"
#include "analysis.h"



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
    Matrix *new_m = make_matrix(3, 6, 3, pt, (int []){2,2,2},  (int []){0,0,0, 0,0,0, 2,3,2},  (int []){0,0,2}, states);
    printf("\n");
    print_matrix(new_m);
    printf("\n");
    printf("Making random tree\n");
    // TODO: read in data
    // TODO: run_analysis function, which does all this as a pipeline 
    Tree *rand_tree = make_random_tree(3);
    printf("Made random tree\n");
    assert(tree_is_correct(rand_tree));
    TreeSet final_trees = spr(rand_tree, new_m, 20.0); 
    printf("Final cost of trees is %f\n\n", final_trees.tcost);
    // TODO: Robinson Foulds comparison of trees to catch identical
    // TODO: Merge non-identical into on set
    
    // TODO: write_trees function writes trees to file as newick
    for (int idx = 0; idx < final_trees.ntrees; idx++){
        print_tree(final_trees.trees[idx]);
        printf("Freeing final tree %i\n", idx);
        free_tree(final_trees.trees[idx]);
    }
    free_tree(rand_tree);
    printf("Free'd tree\n");
    print_matrix(new_m);
    printf("Destroying matrix\n");
    Matrix_destroy(new_m);
    return 1;
}
