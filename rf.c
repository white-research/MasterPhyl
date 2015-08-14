#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "tree.h"


int *relabel_tree(Tree *t){
    int *labels = malloc(sizeof(int)*t->ntaxa);
    Node *node = t->root_node;
    printf("\nRoot node is %i\n", node->id);
    while (node->desc1 != NULL){
        node = node->desc1;
        printf(" On node %i\n", node->id);
    }
    int label_counter = 0;
    Node *last_node = NULL;
    int n_count = 0;
    while (node != NULL){
        printf("On node %i\n", node->id);
        //printf(" Last node %i\n", last_node->id);
        
        if (node->desc1 == NULL && node->desc2 == NULL){
            printf("  Sit 1\n");
            assert(node->id < t->ntaxa);
            labels[node->id] = label_counter++;
            last_node = node;
            node = node->anc;
        } else{
            printf(" Desc1 %i\n", node->desc1->id);
            if (last_node == node->desc1){
                printf("  Sit 2\n");
                last_node = node;
                node = node->desc2;
            } else{
                if (last_node ==node->desc2){
                    printf("  Sit 3\n");
                    last_node = node;
                    node = node->anc;
                }
                else{
                    printf("  Sit 4\n");
                    last_node = node;
                    node = node->desc1;
                }
            }
        }
        n_count++;
    }
    for(int i=0; i<t->ntaxa; i++){
        printf("%i ", labels[i]);
    }
    printf("\n");
    return labels;
}


void tree_clusters_process(Tree *t, int **clusters, int *cluster_list){
    for (int i = t->ntaxa; i < 2*t->ntaxa-1; i++){
        printf("on row %i\n", i);
        int start=-1, end=-1;
        for (int j=0; j < t->ntaxa; j++){
            printf("%i=%i ", i, clusters[i][j]);
            if (clusters[i][j] == 1){
                if (start==-1) start = j;
                end = j;
            }
        }
        printf("\n");
        printf("Inserting %i and %i at %i\n", start, end, 2*(i-t->ntaxa));
        cluster_list[2*(i-t->ntaxa)] = start;
        cluster_list[2*(i-t->ntaxa)+1] = end;
    }
}


void tree_clusters_downpass(Tree *t, Node *node, int *labels, int **clusters){
    if (node->desc1 != NULL && node->desc2 != NULL){
        tree_clusters_downpass(t, node->desc1, labels, clusters);
        tree_clusters_downpass(t, node->desc2, labels, clusters);
        
        printf(" On inner node %i\n", node->id);
        
        int *d1, *d2;
        
        if (node->desc1->desc1 == NULL){ // if tip node
            printf(" Making array for tip node %i\n", node->desc1->id);
            assert(node->desc1->desc2 == NULL);
            d1 = malloc(sizeof(int)*(t->ntaxa));
            for (int i=0; i<t->ntaxa; i++){
                if (i == labels[node->desc1->id]){
                    d1[i] = 1;
                } else{
                    d1[i] = 0;
                }
            }
            clusters[node->desc1->id] = d1;
        } else {
            d1 = clusters[node->desc1->id];
        }
        
        if (node->desc2->desc1 == NULL){
            assert(node->desc2->desc2 == NULL);
            d2 = malloc(sizeof(int)*(t->ntaxa));
            for (int i=0; i<t->ntaxa; i++){
                if (i == labels[node->desc2->id]){
                    d2[i] = 1;
                } else{
                    d2[i] = 0;
                }
            }
            clusters[node->desc2->id] = d2;
        } else {
            d2 = clusters[node->desc2->id];
        }
        
        int *this_node = malloc(sizeof(int)*(t->ntaxa));
        for (int i=0; i<t->ntaxa; i++){
            if (d1[i]==1 || d2[i]==1){
                this_node[i]=1;
            } else this_node[i] = 0;
        }
        clusters[node->id]=this_node;
    }
}


int check_tree_diff(int *labels, int *cluster_list, Tree *t2, Node *node, int **clusters){
    if (node->desc1 != NULL && node->desc2 != NULL){
        int d1_diff = check_tree_diff(labels, cluster_list, t2, node->desc1, clusters);
        if (d1_diff == 0) return 0;
        int d2_diff = check_tree_diff(labels, cluster_list, t2, node->desc2, clusters);
        if (d2_diff == 0) return 0;
        
        int *d1, *d2;
        
        if (node->desc1->desc1 == NULL){ // if tip node
            printf(" Making array for tip node %i\n", node->desc1->id);
            assert(node->desc1->desc2 == NULL);
            d1 = malloc(sizeof(int)*(t2->ntaxa));
            for (int i=0; i<t2->ntaxa; i++){
                if (i == labels[node->desc1->id]){
                    d1[i] = 1;
                } else{
                    d1[i] = 0;
                }
            }
            clusters[node->desc1->id] = d1;
        } else {
            d1 = clusters[node->desc1->id];
        }
        
        if (node->desc2->desc1 == NULL){
            assert(node->desc2->desc2 == NULL);
            d2 = malloc(sizeof(int)*(t2->ntaxa));
            for (int i=0; i<t2->ntaxa; i++){
                if (i == labels[node->desc2->id]){
                    d2[i] = 1;
                } else{
                    d2[i] = 0;
                }
            }
            clusters[node->desc2->id] = d2;
        } else {
            d2 = clusters[node->desc2->id];
        }
        
        int *this_node = malloc(sizeof(int)*(t2->ntaxa));
        for (int i=0; i<t2->ntaxa; i++){
            if (d1[i]==1 || d2[i]==1){
                this_node[i]=1;
            } else this_node[i] = 0;
        }
        clusters[node->id]=this_node;
        
        // check if this node is in list
        int start=-1, end=-1, last_zero=-1;
        for (int j=0; j < t2->ntaxa; j++){
            printf("%i=%i ", node->id, clusters[node->id][j]);
            if (clusters[node->id][j] == 1){
                if (last_zero > end){
                    start = -1;
                    end = -1;
                    break;
                }
                if (start==-1) start = j;
                end = j;
            } else{
                last_zero = j;
            }
        }
        if (start==-1 && end == -1) return 0;
    }
    return 1;
}


void main(void){
    int c;
    c = 1;
    printf("%i\n",c);
    
    Tree *t1 = make_random_tree(3);
    Tree *t2 = make_random_tree(3); // copy_tree(t1);
    
    int *t2_labels = relabel_tree(t2);
    int **t2_clusters = malloc(sizeof(int *)*(2*t2->ntaxa-1));
    tree_clusters_downpass(t2, t2->root_node, t2_labels, t2_clusters);
    for (int i=0; i<2*t2->ntaxa-1; i++){
        for (int j = 0; j < t2->ntaxa; j++){
            printf(" %i", t2_clusters[i][j]);
        }
        printf("\n");
    }
    
    int *t2_cluster_list = malloc(sizeof(int)*2*( (2*t2->ntaxa-1) - t2->ntaxa));
    
    printf("Length of clusters is %i\n", 2*( (2*t2->ntaxa-1) - t2->ntaxa));
    
    tree_clusters_process(t2, t2_clusters, t2_cluster_list);
    
    
    for (int i = 0; i < 2*(t2->ntaxa-1); i++){
        printf("%i ", t2_cluster_list[i]);
    }
    printf("\n");
    
    int **t1_clusters = malloc(sizeof(int *)*(2*t1->ntaxa-1));
    int is_diff = check_tree_diff(t2_labels, t2_cluster_list, t1, t1->root_node, t1_clusters);
    printf("\n\nis diff: %i\n", is_diff);
    
    free_tree(t1);
    free_tree(t2);
}
























