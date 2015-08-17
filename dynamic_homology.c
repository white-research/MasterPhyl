#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "matrix.h"
#include "tree.h"

#define DEBUG 0 //0 means no debugging output, 1 means basic debugging, 2 means detailed debugging


void print_sequence(float *seq, int len)
{
    int i;
    for (i=0; i<len; ++i)
    {
        if (DEBUG >= 1) printf("%4.2f ", seq[i]);
    }
    if (DEBUG >= 1) printf("\n");
}


void print_NW_matrix(float M[], int r, int c)
{
    int i, j;
    for(i = 0; i < (r+1); ++i) {
        for(j = 0; j < (c+1) ; ++j) {
            if (DEBUG >= 1) printf("%4.2f ", M[i*(c+1)+j]);
        }
        if (DEBUG >= 1) printf("\n");
    }
}


void find_gaps(float *B, float *C, int gaps_in_B[], int gaps_in_C[], int B_count, int C_count, int chars_per_seg)
{
    //create and fill two arrays which note whether segments in each sequence are filled (1) or empty (0)
    if (DEBUG == 1) printf(" - Calculating where gaps are present in the sequence\n");
    int seg_counter, char_counter, presence;
    if (DEBUG == 2) printf("    Calculating presence/absence in B:\n");
    for ( seg_counter = 0; seg_counter < B_count; ++seg_counter) // Do for B
    {
        presence = 0;
        for ( char_counter = 0; char_counter < chars_per_seg; ++char_counter )
        {
            if (DEBUG == 2)  printf("%5.2f ", B[seg_counter*(chars_per_seg*2)+(char_counter*2)]);
            if (B[seg_counter*(chars_per_seg*2)+(char_counter*2)] >= -0.00001 ) presence = 1;
        }
        gaps_in_B[seg_counter] = presence;
        if (DEBUG == 2) printf(" presence: %u \n", gaps_in_B[seg_counter]);
    }
    if (DEBUG == 2) printf("    Calculating presence/absence in C:\n");
    for ( seg_counter = 0; seg_counter < C_count; ++seg_counter) // Do for C
    {
        presence = 0;
        for ( char_counter = 0; char_counter < chars_per_seg; ++char_counter )
        {
            if (DEBUG == 2) printf("%5.2f ", C[seg_counter*(chars_per_seg*2)+(char_counter*2)]);
            if (C[seg_counter*(chars_per_seg*2)+(char_counter*2)] >= -0.0001 ) presence = 1;
        }
        gaps_in_C[seg_counter] = presence;
        if (DEBUG == 2) printf("     presence: %u \n", gaps_in_C[seg_counter]);
    }

}


float transformation_cost(float *seg1, float *seg2, int seg_len)
{
    /* Uses Farris's algorithm -> assumes additive characters */
    if (DEBUG == 2) printf("  - Calculating transformation cost (%i characters):\n", seg_len);
    float cost=0;
    if (seg1 == NULL || seg2 == NULL)
    {
        if (DEBUG >= 1) printf("\n        Failed to align two segments\n");
        abort(); // no cost to align two gaps
    }
    int i;
    float seg1_l, seg1_u, seg2_l, seg2_u;
    for (i=0; i < seg_len; ++i){
        seg1_l = *(seg1 + i*2) ;
        seg1_u = *(seg1 + i*2 + 1) ;
        seg2_l = *(seg2 + i*2) ;
        seg2_u = *(seg2 + i*2 + 1) ;
        if (seg1_l == -1 || seg2_l == -1)
        {
            ; // do nothing if missing
        }
        else if (seg1_l > seg2_u)
        {
            cost = cost + ( seg1_l - seg2_u);
        }
        else if (seg2_l > seg1_u)
        {
            cost = cost + ( seg2_l - seg1_u);
        }
        if (DEBUG == 2) printf("        Character %u - B:%5.2f-%5.2f   C: %5.2f-%5.2f - Total cost: %6.2f\n", i, seg1_l, seg1_u, seg2_l, seg2_u, cost);
    }
    return cost;
}


void score(int B_count, int C_count, float N[], float *B, float *C, int gaps_in_B[], int gaps_in_C[], int chars_per_seg, float gap_cost)
{
    if (DEBUG >= 1) printf(" - Starting score function\n");
    if (DEBUG >= 1) printf("    - Filling matrix based on sequences:\n       ");
    if (DEBUG >= 1) print_sequence(B, B_count*chars_per_seg*2);
    if (DEBUG >= 1) printf("       ");
    if (DEBUG >= 1) print_sequence(C, C_count*chars_per_seg*2);

    // Fill the N-W cost matrix
    int i,j;
    float cost_align = 0, cost_gap_in_B = 0, cost_gap_in_C = 0, trans;
    N[0] = 0;
    for(i=1; i < (B_count+1); ++i) // Put gap align costs into first row and column of N-W cost matrix
    {
        N[i*(C_count+1)] = (gap_cost*chars_per_seg) + N[(i-1)*(C_count+1)];
    }
    for(j=1; j < (C_count+1); ++j)
    {
        N[j] = (gap_cost*chars_per_seg) + N[j-1];
    }

    for(i=1; i < (B_count+1); ++i){ // Put remaining values into N-W cost matrix
        for(j=1; j < (C_count+1); ++j){
            if (DEBUG >= 1) printf("\n    Working on matrix cell: row %i, col %i\n", i+1, j+1);
            if (gaps_in_B[i-1] == 1 && gaps_in_C[j-1] == 1)
            {
                if (DEBUG >= 1) printf("    No gaps\n");
                trans = transformation_cost( B+(i-1)*(chars_per_seg*2), C+(j-1)*(chars_per_seg*2), chars_per_seg) ;
                if (DEBUG >= 1) printf("     ->Transformation cost to align = %6.2f\n", trans);
                cost_align = transformation_cost( B+(i-1)*(chars_per_seg*2), C+(j-1)*(chars_per_seg*2), chars_per_seg) + N[(i-1)*(C_count+1)+j-1];
                cost_gap_in_B = gap_cost*chars_per_seg + N[i*(C_count+1)+j-1];
                cost_gap_in_C = gap_cost*chars_per_seg + N[(i-1)*(C_count+1)+j];
            }
            else if (gaps_in_B[i-1] == 0 && gaps_in_C[j-1] == 0)
            {
                if (DEBUG >= 1) printf("    Gaps in both\n");
                cost_align = N[(i-1)*(C_count+1)+j-1];
                cost_gap_in_B = N[(i)*(C_count+1)+j-1];
                cost_gap_in_C = N[(i-1)*(C_count+1)+j];
            }
            else if (gaps_in_B[i-1] == 0){
                if (DEBUG >= 1) printf("    Gap segment in sequence of B\n");
                cost_align = gap_cost*chars_per_seg + N[(i-1)*(C_count+1)+j-1];
                cost_gap_in_B = gap_cost*chars_per_seg + N[(i)*(C_count+1)+j-1];
                cost_gap_in_C = N[(i-1)*(C_count+1)+j];
            }
            else if (gaps_in_C[j-1] == 0){
                if (DEBUG >= 1) printf("    Gap segment in sequence of C\n");
                cost_align = gap_cost*chars_per_seg + N[(i-1)*(C_count+1)+j-1];
                cost_gap_in_B = N[(i)*(C_count+1)+j-1];
                cost_gap_in_C = gap_cost*chars_per_seg + N[(i-1)*(C_count+1)+j];
            }
            else {
                printf("Error (1) in function score");
                abort();
            }

            if (DEBUG >= 1) printf("    Cost of aligning: %5.2f\n            gap in B: %5.2f\n            gap in C: %5.2f\n", cost_align, cost_gap_in_B, cost_gap_in_C);
            if(cost_align <= cost_gap_in_B && cost_align <= cost_gap_in_C)
            {
                if (DEBUG >= 1) printf("     -> Aligning\n");
                N[i*(C_count+1)+j] = cost_align;
            }
            else if(cost_gap_in_B <= cost_gap_in_C)
            {
                if (DEBUG >= 1) printf("     -> Inserting gap in B\n");
                N[i*(C_count+1)+j] = cost_gap_in_B;
            }
            else
            {
                if (DEBUG >= 1) printf("     -> Inserting gap in B\n");
                N[i*(C_count+1)+j] = cost_gap_in_C;
            }
        }

    } // End the outer matrix filling 'for' loop
} // End SCORE function


void make_alignment(float *old_seq, float new_seq[], int old_seq_num, int new_seq_num, int seg_length)
{
    if (DEBUG == 1) printf("  - Making alignment. Old segment: %i New segment: %i", old_seq_num, new_seq_num);
    int i;
    if (old_seq_num == -1)
    {
        if (DEBUG == 1) printf(" -> Inserting gap segment\n");
        for ( i=0; i<seg_length*2; ++i)
        {
            new_seq[ new_seq_num * seg_length * 2 + i] = -1.0;
        }
    } // end "if gap then insert -1's"
    else
    {
        if (DEBUG == 1) printf("\n");
        for ( i=0; i<seg_length*2; ++i)
        {
            new_seq[ new_seq_num * seg_length * 2 + i] = old_seq[ old_seq_num*seg_length*2 + i ];
        }
    }
}


int backtrack(float M[], float *s1, int s1_segs, int s1_counter, float *s2, int s2_segs, int s2_counter, float new_s1[], float new_s2[], int len_count, int char_num, int *new_seq_tracker)
{
    if (s1_counter < 2 && s2_counter < 2)
    {
        return len_count;
    }
    if (s1_counter < 1 || s2_counter < 1)
    {
        if (DEBUG >= 1) printf("Error in backtrack function - outside bounds of Needleman-Wunsch matrix");
        abort();
    }
    if (DEBUG == 1) printf("    On cell with indices %u, %u - ", s1_counter, s2_counter);
    if (DEBUG == 1) printf("    Indices are %i, %i, %i \n", (s1_counter-2)*(s1_segs+1) + (s2_counter-2), (s1_counter-1)*(s1_segs+1) + (s2_counter-2), (s1_counter-2)*(s1_segs+1) + (s2_counter-1));
    float diag_cost = -1, Bgap_cost = -1, Cgap_cost = -1;
    if (s1_counter > 1 && s2_counter > 1)
    {
        diag_cost = M[(s1_counter-2)*(s2_segs+1) + (s2_counter-2)];
        Bgap_cost = M[(s1_counter-1)*(s2_segs+1) + (s2_counter-2)];
        Cgap_cost = M[(s1_counter-2)*(s2_segs+1) + (s2_counter-1)];
    }

    if (DEBUG == 1) printf("    Align cost is %6.2f, gap in B is %6.2f, gap in C is %6.2f\n", diag_cost, Bgap_cost, Cgap_cost);


    if (s1_counter > 1 && s2_counter > 1 && diag_cost <= Bgap_cost && diag_cost <= Cgap_cost)
    {
        if (DEBUG == 1) printf("    Aligning B %i and C %i \n\n", s1_counter-1, s2_counter-1);
        len_count = backtrack(M, s1, s1_segs, s1_counter-1, s2, s2_segs, s2_counter-1, new_s1, new_s2, len_count+1, char_num, new_seq_tracker);
        if (DEBUG == 2) printf("    Building new seqs - Count: %i Tracker: %i B: %i C: %i\n", len_count, *new_seq_tracker, s1_counter-1, s2_counter-1);
        if (DEBUG == 1) printf("      For s1:");
        make_alignment(s1, new_s1, s1_counter-2, *new_seq_tracker, char_num);
        if (DEBUG == 1) printf("      For s2:");
        make_alignment(s2, new_s2, s2_counter-2, *new_seq_tracker, char_num);

        if (DEBUG == 1) printf(" Aligned: Old value of new_seq_tracker: %i", *new_seq_tracker);
        *new_seq_tracker = *new_seq_tracker+1;
        if (DEBUG == 1) printf(" -> New value: %i\n", *new_seq_tracker);
    }
    else if( (s2_counter > 1 && Bgap_cost <= Cgap_cost) || s1_counter == 1)
    {
        if (DEBUG == 1) printf("    Aligning C %i with gap in B\n\n", s2_counter-1);
        len_count = backtrack(M, s1, s1_segs, s1_counter, s2, s2_segs, s2_counter-1, new_s1, new_s2, len_count+1, char_num, new_seq_tracker);
        if (DEBUG == 2) printf("    Building new seqs - Count: %i Tracker: %i B: gap C: %i\n", len_count, *new_seq_tracker, s2_counter-1);
        if (DEBUG == 1) printf("      For s1:\n");
        if (DEBUG == 1) printf(" Gap in B: Old value of new_seq_tracker: %i\n", *new_seq_tracker);
        make_alignment(s1, new_s1, -1, *new_seq_tracker, char_num);

        if (DEBUG == 1) printf("              Old value of new_seq_tracker: %i\n", *new_seq_tracker);
        if (DEBUG == 1) printf("      For s2:");
        make_alignment(s2, new_s2, s2_counter-2, *new_seq_tracker, char_num);

        if (DEBUG == 1) printf("             Old value of new_seq_tracker: %i", *new_seq_tracker);
        *new_seq_tracker = *new_seq_tracker+1;
        if (DEBUG == 1) printf(" -> New value: %i\n", *new_seq_tracker);
    }
    else
    {
        if (DEBUG == 1) printf("    Aligning B %i with gap in C\n\n", s1_counter-1);
        len_count = backtrack(M, s1, s1_segs, s1_counter-1, s2, s2_segs, s2_counter, new_s1, new_s2, len_count+1, char_num, new_seq_tracker);
        if (DEBUG == 2) printf("    Building new seqs - Count: %i Tracker: %i B: %i C: gap\n", len_count, *new_seq_tracker, s1_counter-1);
        if (DEBUG == 1) printf("      For s1:");
        make_alignment(s1, new_s1, s1_counter-2, *new_seq_tracker, char_num);
        if (DEBUG == 1) printf("      For s2:");
        make_alignment(s2, new_s2, -1, *new_seq_tracker, char_num);

        if (DEBUG == 1) printf(" Gap in C: Old value of new_seq_tracker: %i", *new_seq_tracker);
        *new_seq_tracker = *new_seq_tracker+1;
        if (DEBUG == 1) printf(" -> New value: %i\n", *new_seq_tracker);
    }

    return len_count;
}


void best_htu(float s1[], float s2[], int seg_count, int chars_per_seg, float *htu_seq)
{
    if (DEBUG == 1) printf(" - Finding best HTU\n");
    int i; //, j; TODO: double check that j is unnecessary
    float s1_l, s1_u, s2_l, s2_u;
    for( i = 0; i < (seg_count*chars_per_seg); ++i )
    {
        if (s1[i*2] < -0.0001 && s2[i*2] < -0.0001) // if both sequences are missing for that character
        {
            if (DEBUG == 2) printf("    Char %i -> both missing\n", i);
            htu_seq[i*2] = -1;
            htu_seq[i*2 + 1] = -1;
        }
        else if (s1[i*2] < -0.0001) // if s1 only is missing
        {
            if (DEBUG == 1) printf("    Char %i -> s1 missing\n", i);

            htu_seq[i*2] = s2[i*2];
            htu_seq[i*2 + 1] = s2[i*2 + 1];
        }
        else if (s2[i*2] < -0.0001) // if s2 only is missing
        {
            if (DEBUG == 1) printf("    s2 missing\n");

            htu_seq[i*2] = s1[i*2];
            htu_seq[i*2 + 1] = s1[i*2 + 1];
        }
        else // if both are present then calculate new range
        {
            if (DEBUG == 1) printf("    Char %i -> both present, calculating new range\n", i);

            s1_l = s1[i*2];
            s1_u = s1[i*2+1];
            s2_l = s2[i*2];
            s2_u = s2[i*2+1];

            if (s1_l >= s2_u)
            {
                htu_seq[i*2] = s2_u;
                htu_seq[i*2+1] = s1_l;
            }
            else if (s2_l >= s1_u)
            {
                htu_seq[i*2] = s1_u;
                htu_seq[i*2+1] = s2_l;
            }
            else
            {
                if (s1_l > s2_l) { htu_seq[i*2] = s1_l; }
                else { htu_seq[i*2] = s2_l; }
                if (s1_u > s2_u) { htu_seq[i*2 + 1] = s2_u; }
                else { htu_seq[i*2 + 1] = s1_u; }
            }
        }
    } // end for loop through segments
}//end best_htu function


float do_downpass(Tree *tree, Node *node, float gap_multiplier, float **sequences, int segments_per_taxon[], int chars_per_segment, float costs[], int total_nodes)
{
    // int node_children[2];
    if (node->desc1 != NULL && node->desc2 != NULL){
        
        do_downpass(tree, node->desc1, gap_multiplier, sequences, segments_per_taxon, chars_per_segment, costs, total_nodes);
        do_downpass(tree, node->desc2, gap_multiplier, sequences, segments_per_taxon, chars_per_segment, costs, total_nodes);
        
        //assert(node->desc1->desc1 == NULL && node->desc2->desc1 == NULL);
        
        if (DEBUG >= 1) printf("\nCalculations for inner node %i with children %i and %i \n", node->id, node->desc1->id, node->desc2->id);
        // calculate gaps
        int seq1_gaps[segments_per_taxon[node->desc1->id]], seq2_gaps[segments_per_taxon[node->desc2->id]];
        find_gaps(sequences[node->desc1->id], sequences[node->desc2->id], seq1_gaps, seq2_gaps, segments_per_taxon[node->desc1->id], segments_per_taxon[node->desc2->id], chars_per_segment);

        // declare and fill Needlman-Wunsch matrix
        float M[(segments_per_taxon[node->desc1->id]+1)*(segments_per_taxon[node->desc2->id]+1)]; // add 1 to be able to add gap at the start of either sequence
        score(segments_per_taxon[node->desc1->id], segments_per_taxon[node->desc2->id], M, sequences[node->desc1->id], sequences[node->desc2->id], seq1_gaps, seq2_gaps, chars_per_segment, gap_multiplier);

        if (DEBUG == 1) printf("\nThe N-W matrix is:\n");
        if (DEBUG == 1) print_NW_matrix(M, segments_per_taxon[node->desc1->id], segments_per_taxon[node->desc2->id]);

        // backtrack(int M[], int s1[], int s1_segs, int s1_counter, int s2[], int s2_segs, int s2_counter, int new_s1[], int new_s2[], int len_count, int char_num, int *new_seq_tracker)

        float new_seq1[(segments_per_taxon[node->desc1->id]+segments_per_taxon[node->desc2->id])*chars_per_segment*2], new_seq2[(segments_per_taxon[node->desc1->id]+segments_per_taxon[node->desc2->id])*chars_per_segment*2];
        int new_seq_len = 0, tracker = 0;
        int *to_tracker = &tracker;
        new_seq_len = backtrack(M, sequences[node->desc1->id], segments_per_taxon[node->desc1->id], segments_per_taxon[node->desc1->id]+1, sequences[node->desc2->id], segments_per_taxon[node->desc2->id], segments_per_taxon[node->desc2->id]+1, new_seq1, new_seq2, new_seq_len, chars_per_segment, to_tracker);
        if (DEBUG == 1) printf("There are %i segments in this new alignment\n", new_seq_len);

        if (DEBUG == 1) printf("About allocate memory...\n");
        sequences[node->id] = (float *)malloc(new_seq_len*chars_per_segment*2 * sizeof(float));
        if (DEBUG == 1) printf(" -> Created new memory space for the htu sequence of same size as these alignments, of memory size %lu\n", new_seq_len*chars_per_segment*2 * sizeof(int));

        segments_per_taxon[node->id] = new_seq_len;
        costs[node->id] = costs[node->desc1->id] + costs[node->desc2->id] + M[(segments_per_taxon[node->desc1->id]+1)*(segments_per_taxon[node->desc2->id]+1)-1];
        
        if (DEBUG == 1) printf("About to start calculating the best HTU:\n");
        best_htu(new_seq1, new_seq2, new_seq_len, chars_per_segment, sequences[node->id]);
        if (DEBUG == 1) printf("    Reconstructed sequence for node %i is:\n    ", node->id);
        float *htu = sequences[node->id];
        if (DEBUG == 1) print_sequence(htu, new_seq_len*chars_per_segment*2);
    }
    else{
        if (DEBUG >= 1) printf("\nVisited tip %i\n", node->id);
    }
    return costs[node->id];
}


float dynamic_homology(Tree *t, Partition *p, float gap_multiplier)
{
    int num_tips = t->ntaxa;
    int num_nodes = num_tips*2-1;
    float tree_cost = 0;
    float **seq_array;
    
    if (DEBUG >= 1) printf(" partition type is %i\n", p->data_type);
    if (DEBUG >= 1) printf(" root index is %i\n", t->root_node->id);
    
    if (DEBUG >= 1) printf(" We calculate there are %i nodes\n", num_nodes);
    seq_array = (float **)malloc( num_nodes * sizeof(float *));
    
    int segments[num_nodes];
    float costs[num_nodes];
    int tip_counter; //, sequence_start_index = 0, char_counter;
    
    
    // TODO: put matrices into this form when making in matrix.c -> saves remaking it each tree
    for (tip_counter = 0; tip_counter < num_tips; ++tip_counter){
        if (DEBUG >= 1) printf(" Making array for tip %i\n", tip_counter);
        if (DEBUG >= 1) printf("  with %i segments\n", p->segments[tip_counter]);
        if (DEBUG >= 1) printf("  and %i characters per segment\n", p->nchar);
        seq_array[tip_counter] = p->states[tip_counter];
        //seq_array[tip_counter] = (float *)malloc(p->segments[tip_counter] * p->nchar * 2 * sizeof(float));
        //for (char_counter = 0; char_counter < (p->segments[tip_counter] * p->nchar * 2); ++char_counter){
        //    printf("  %i: %2.2f", char_counter, p->states[sequence_start_index+char_counter]);
        //    seq_array[tip_counter][char_counter] = p->states[sequence_start_index+char_counter];
        //}
        //printf("\n");
        //sequence_start_index += p->segments[tip_counter] * p->nchar * 2;
        segments[tip_counter] = p->segments[tip_counter];
        costs[tip_counter] = 0.0;
    }
    
    tree_cost = do_downpass(t, t->root_node, gap_multiplier, seq_array, segments, p->nchar, costs, num_nodes);
    if (DEBUG >= 1) printf("\n\nTree cost is: %6.2f\n\n", tree_cost);
    
    for (int idx = 0; idx <num_tips; idx++){
        seq_array[idx] = NULL;
    }
    
    for (int idx = num_tips; idx < num_nodes ; ++idx)
    {
        if (DEBUG >= 1) printf(" Freeing sequence of tip %i\n", idx);
        free(seq_array[idx]);
    }
    if (DEBUG >= 1) printf(" All individual sequences freed, freeing main sequence array\n");
    free(seq_array);
    
    
    return tree_cost;
}
