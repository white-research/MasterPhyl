#ifndef _nw_h
#define _nw_h

void find_gaps(float *B, float *C, int gaps_in_B[], int gaps_in_C[], int B_count, int C_count, int chars_per_seg);
void score(int B_count, int C_count, float N[], float *B, float *C, int gaps_in_B[], int gaps_in_C[], int chars_per_seg, float gap_cost); //creates N-W cost matrix
float transformation_cost(float *seg1, float *seg2, int seg_len); // cost of transformation between two states
int backtrack(float M[], float *s1, int s1_segs, int s1_counter, float *s2, int s2_segs, int s2_counter, float new_s1[], float new_s2[], int len_count, int char_num, int *new_seq_tracker);
void make_alignment(float *old_seq, float new_seq[], int old_seq_num, int new_seq_num, int seg_length);
void best_htu(float s1[], float s2[], int seg_count, int chars_per_seg, float *htu_seq);
extern float do_downpass(int node, int tree[], float gap_multiplier, float **sequences, int segments_per_taxon[], int chars_per_segment, float costs[], int total_nodes);
extern float initialize(int root_node, int num_tips, float tip_char_states[], int taxa_sequence_lengths[], float gap_multiplier, int segments_per_tip[], int chars_per_segment, int tree[]);

extern int test_function(int num1, int num2);

void tests(void);

void print_sequence(float *seq, int len);
void print_matrix(float M[], int r, int c); // prints out a N-W cost matrix for inspection

#endif
