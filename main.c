#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "matrix.h"
#include "tree.h"
#include "analysis.h"


int main(void){
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
    Tree *rand_tree = make_random_tree(5);
    assert(tree_is_correct(rand_tree));
    spr(rand_tree, new_m, 20.0);
    free_tree(rand_tree);
    Matrix_destroy(new_m);
    return 1;
}
