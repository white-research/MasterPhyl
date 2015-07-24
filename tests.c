#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "tree.h"
#include "nw.h"

int main( void )
{
    /* run tests:
    do_downpass();
    */

    int phyl[] = { 0, 0, 0, 1, 0,
                   0, 0, 0, 1, 0,
                   0, 0, 0, 0, 1,
                   -1,-1, 0, 0, 1,
                   0, 0,-1,-1, 0 };

    char taxa[] = {'A', 'B', 'C'};
    int num_tips = 3;
    int num_nodes = 5;
    int seqA[] = { 0, 1,  4, 5, //segment 1
                   0, 0,  5, 6, //segment 2
                   1, 2,  1, 1  //segment 3
    };

    int seqB[] = { 1, 2,  3, 4, //segment 1
                   -1,-1, -1,-1, //missing segment
                   1, 2,  1, 1  //segment 2
    };

    int seqC[] = { 2, 2,  1, 2, //segment 1
                   -1,-1, -1,-1, //missing segment
                   1, 2,  1, 1  //segment 2
    };

    int chars_per_segment = 2;
    int seq_len = 12;
    int gap_cost = 1;

    printf("\n\n");

    int **seq_array = (int **)malloc( (num_tips*2 - 1) * sizeof(int *));
    int seq_i = 0;

    /*
    for (seq_i = 0; seq_i < num_tips ; ++seq_i)
        {
        seq_array[seq_i] = (int *)malloc(12 * sizeof(int));
        }

    */


    seq_array[0] = &seqA[0];
    seq_array[1] = &seqB[0];
    seq_array[2] = &seqC[0];

    /*
    int k,l, count=0;
    for (k=0; k<num_tips; ++k)
        {
        for (l=0; l<12; ++l)
            {
            seq_array[k][l] = ++count;
            }
        }

    for (k=0; k<num_tips; ++k)
        {
        for (l=0; l<12; ++l)
            {
            printf("%3i ", seq_array[k][l]);
            }
            printf("\n");
        }
    */

    int segment_counts[num_nodes], costs[num_nodes];

    printf("\n\n\n");
    for (seq_i = 0; seq_i < num_tips ; ++seq_i)
    {
        segment_counts[seq_i] = 3;
        costs[seq_i] = 0;
    }

    int tree_cost;
    tree_cost = do_downpass(4, phyl, 1, seq_array, segment_counts, chars_per_segment, costs, num_nodes);

    printf("\n\nTree cost is: %i\n\n", tree_cost);


    for (seq_i = num_tips; seq_i < num_nodes ; ++seq_i)
    {
        printf("Freeing sequence %i\n", seq_i);
        free(seq_array[seq_i]);
    }
    printf("\nAll individual sequences freed, freeing main sequence array\n");
    free(seq_array);

    return tree_cost;
}



