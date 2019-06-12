#include "tree.h"
#include <array>
#include <iostream>
#include <map>

std::map<int, int> relabelTree(Tree& tree) {
    // Relabel nodes from 1 to number_tips
    std::map<int, int> labels {};
    std::shared_ptr<Node> node = tree.getRootNode();
    std::shared_ptr<Node> last_node = nullptr;
    int label_counter = 1;
    // Find left-most child (end desc1)
    while (node->desc1 != nullptr) {
        node = node->desc1;
    }
    // Iterate through tips from left to right.
    // If tip, record new label in labels[node_id]. First tip -> 1, 2nd tip ->2, etc.
    while (node != nullptr){
        if (node->desc1 == nullptr && node->desc2 == nullptr){
            labels.emplace(node->get_id(), label_counter++);
            last_node = node;
            node = node->anc;
        }
        else {
            if (last_node == node->desc1){
                last_node = node;
                node = node->desc2;
            } else {
                if (last_node ==node->desc2){
                    last_node = node;
                    node = node->anc;
                }
                else{
                    last_node = node;
                    node = node->desc1;
                }
            }
        }
    }
    return labels;
}

//void tree_clusters_process(Tree *t, int **clusters, int *cluster_list)
void processClusters(Tree& tree, std::vector<std::vector<int>>& clusters, std::vector<std::array<int, 2>>& cluster_list){
    int inner_node = 0;
    for (int i = 0; i < tree.getNNodes(); i++) {
        int start = -1, end = -1;
        for (int j = 0; j < tree.getNTips(); j++) {
            if (clusters[i][j] == 1){
                if (start == -1) start = j;
                end = j;
            }
        }
        if (start != end) {
            cluster_list[inner_node][0] = start + 1;
            cluster_list[inner_node][1] = end + 1;
            inner_node++;
        }
    }
}

//void tree_clusters_downpass(Tree *t, Node *node, int *labels, int **clusters)
void treeClusters(Tree& tree, Node& node, std::map<int,int>& labels, std::vector<std::vector<int>>& clusters) {
    if (node.hasDescendents()) {
        treeClusters(tree, *(node.desc1), labels, clusters);
        treeClusters(tree, *(node.desc2), labels, clusters);
        for (int i=0; i < tree.getNTips(); i++) {
            if (clusters[node.desc1->get_id() - 1][i] || clusters[node.desc2->get_id() - 1][i]){
                clusters[node.get_id() - 1][i] = 1;
            }
        }
    }
    else {
        for (int i=1; i <= tree.getNTips(); i++) {
            if (i == labels[node.get_id()]) {
                clusters[node.get_id() - 1][i-1] = 1;
            }
        }
    }
}


//int check_tree_diff(int *labels, int *cluster_list, Tree *t2, Node *node, int **clusters){
//    if (node->desc1 != NULL && node->desc2 != NULL){
//        if (DEBUG >= 1) printf("On node %i\n", node->id);
//        int d1_diff = check_tree_diff(labels, cluster_list, t2, node->desc1, clusters);
//        if (d1_diff == 0) return 0;
//        int d2_diff = check_tree_diff(labels, cluster_list, t2, node->desc2, clusters);
//        if (d2_diff == 0) return 0;
//
//        int *d1, *d2;
//
//        if (node->desc1->desc1 == NULL){ // if tip node
//            if (DEBUG >= 1) printf(" Making array for tip node %i\n", node->desc1->id);
//            assert(node->desc1->desc2 == NULL);
//            d1 = (int *)malloc(sizeof(int)*(t2->ntaxa));
//            for (int i=0; i<t2->ntaxa; i++){
//                if (i == labels[node->desc1->id]){
//                    d1[i] = 1;
//                } else{
//                    d1[i] = 0;
//                }
//            }
//            clusters[node->desc1->id] = d1;
//        } else {
//            d1 = clusters[node->desc1->id];
//        }
//
//        if (node->desc2->desc1 == NULL){
//            if (DEBUG >= 1) printf(" Making array for tip node %i\n", node->desc2->id);
//            assert(node->desc2->desc2 == NULL);
//            d2 = (int *)malloc(sizeof(int)*(t2->ntaxa));
//            for (int i=0; i<t2->ntaxa; i++){
//                if (i == labels[node->desc2->id]){
//                    d2[i] = 1;
//                } else{
//                    d2[i] = 0;
//                }
//            }
//            clusters[node->desc2->id] = d2;
//        } else {
//            d2 = clusters[node->desc2->id];
//        }
//
//        int *this_node = (int *)malloc(sizeof(int)*(t2->ntaxa));
//        if (DEBUG >= 1) printf(" Making array for inner node %i\n", node->id);
//        for (int i=0; i<t2->ntaxa; i++){
//            if (d1[i]==1 || d2[i]==1){
//                this_node[i]=1;
//            } else this_node[i] = 0;
//            if (DEBUG >= 1) printf("%i ", this_node[i]);
//        }
//        if (DEBUG >= 1) printf("\n");
//        clusters[node->id]=this_node;
//
//
//        // check if this node is in list
//        int start=-1, end=-1, last_zero=-1;
//        for (int j=0; j < t2->ntaxa; j++){
//            if (DEBUG >= 1) printf("%i=%i ", node->id, clusters[node->id][j]);
//            if (clusters[node->id][j] == 1){
//                if (last_zero > end){
//                    start = -1;
//                    end = -1;
//                    break;
//                }
//                if (start==-1) start = j;
//                end = j;
//            } else{
//                if (start != -1){
//                    last_zero = j;
//                }
//            }
//        }
//        if (start==-1 && end == -1) return 0;
//    }
//    return 1;
//}
//
//
//int compare_clusters(int *clusters1, int *clusters2, int nclusters){
//    for(int i = 0; i<nclusters; i++){
//        if (DEBUG >= 1) printf("%i %i\n", clusters1[i*2], clusters1[i*2+1]);
//        int not_present = 0;
//        for (int j = 0; j<nclusters; j++){
//            if (DEBUG >= 1) printf(" %i %i\n", clusters2[j*2], clusters2[j*2+1]);
//            if (clusters1[i*2] == clusters2[j*2] && clusters1[i*2+1] == clusters2[j*2+1]){
//                if (DEBUG >= 1) printf("  match\n");
//                not_present = 1;
//            }
//        }
//        if (not_present == 0){
//            return 0;
//        }
//    }
//    return 1;
//}
//
//

int robinsonFouldsDistance(Tree& t1, Tree& t2) {

    auto labels = relabelTree(t2);
    std::vector<std::vector<int>> t2_clusters(t2.getNNodes(), std::vector<int>(t2.getNTips()));
    treeClusters(t2, *(t2.getRootNode()), labels, t2_clusters);
    std::vector<std::array<int, 2>> t2_cluster_list(t2.getNNodes()-t2.getNTips(), std::array<int,2>({0,0}));
    processClusters(t2, t2_clusters, t2_cluster_list);

    return 1;
}

//int trees_different(Tree *t1, Tree *t2){
//    int *t2_labels = relabel_tree(t2);
//    int **t2_clusters = (int **)malloc(sizeof(int *)*(2*t2->ntaxa-1));
//    tree_clusters_downpass(t2, t2->root_node, t2_labels, t2_clusters);
//    for (int i=0; i<2*t2->ntaxa-1; i++){
//        for (int j = 0; j < t2->ntaxa; j++){
//            //printf(" %i", t2_clusters[i][j]);
//        }
//        //printf("\n");
//    }
//
//    int *t2_cluster_list = (int *)malloc(sizeof(int)*2*( (2*t2->ntaxa-1) - t2->ntaxa));
//
//    //printf("Length of clusters is %i\n", 2*( (2*t2->ntaxa-1) - t2->ntaxa));
//
//    tree_clusters_process(t2, t2_clusters, t2_cluster_list);
//
//    //printf("Clusters are:\n");
//    //for (int i = 0; i < 2*(t2->ntaxa-1); i++){
//    //    printf("%i ", t2_cluster_list[i]);
//    //}
//    //printf("\n");
//
//    int **t1_clusters = (int **)malloc(sizeof(int *)*(2*t1->ntaxa-1));
//    for (int i = 0; i < (2*t1->ntaxa-1); i++){
//            t1_clusters[i] = NULL;
//        }
//    //printf("\nStarting difference check\n");
//    int is_diff = check_tree_diff(t2_labels, t2_cluster_list, t1, t1->root_node, t1_clusters);
//    if (is_diff == 1){
//        int *t1_cluster_list = (int *)malloc(sizeof(int)*2*( (2*t1->ntaxa-1) - t1->ntaxa));
//
//        tree_clusters_process(t1, t1_clusters, t1_cluster_list);
//        is_diff = compare_clusters(t1_cluster_list, t2_cluster_list, t1->ntaxa-1);
//
//        free(t1_cluster_list);
//    }
//    //printf("\n\nis diff: %i\n\n", is_diff);
//
//    //printf("Tree 1:\n");
//    //print_tree(t1, t1->root_node, 0);
//    //printf("\nTree 2:\n");
//    //print_tree(t2, t2->root_node, 0);
//    for (int i = 0; i < (2*t1->ntaxa-1); i++){
//    //    printf("%i", i);
//        if (t1_clusters[i] != NULL) free(t1_clusters[i]);
//    }
//    free(t1_clusters);
//    for (int i = 0; i < (2*t2->ntaxa-1); i++){
//    //    printf("%i", i);
//        if (t2_clusters[i] != NULL) free(t2_clusters[i]);
//    }
//    free(t2_clusters);
//    free(t2_cluster_list);
//    free(t2_labels);
//    return is_diff;
//}
//
//
///*
//void main(void){
//    srand(time(NULL));
//    int c;
//    c = 1;
//    printf("%i\n",c);
//
//    Tree *t1 = make_random_tree(4);
//    Tree *t2 = make_random_tree(4); // copy_tree(t1); //
//
//    int *t2_labels = relabel_tree(t2);
//    int **t2_clusters = malloc(sizeof(int *)*(2*t2->ntaxa-1));
//    tree_clusters_downpass(t2, t2->root_node, t2_labels, t2_clusters);
//    for (int i=0; i<2*t2->ntaxa-1; i++){
//        for (int j = 0; j < t2->ntaxa; j++){
//            printf(" %i", t2_clusters[i][j]);
//        }
//        printf("\n");
//    }
//
//    int *t2_cluster_list = malloc(sizeof(int)*2*( (2*t2->ntaxa-1) - t2->ntaxa));
//
//    printf("Length of clusters is %i\n", 2*( (2*t2->ntaxa-1) - t2->ntaxa));
//
//    tree_clusters_process(t2, t2_clusters, t2_cluster_list);
//
//    printf("Clusters are:\n");
//    for (int i = 0; i < 2*(t2->ntaxa-1); i++){
//        printf("%i ", t2_cluster_list[i]);
//    }
//    printf("\n");
//
//    int **t1_clusters = malloc(sizeof(int *)*(2*t1->ntaxa-1));
//    for (int i = 0; i < (2*t1->ntaxa-1); i++){
//            t1_clusters[i] = NULL;
//        }
//    printf("\nStarting difference check\n");
//    int is_diff = check_tree_diff(t2_labels, t2_cluster_list, t1, t1->root_node, t1_clusters);
//    if (is_diff == 1){
//        int *t1_cluster_list = malloc(sizeof(int)*2*( (2*t1->ntaxa-1) - t1->ntaxa));
//
//        tree_clusters_process(t1, t1_clusters, t1_cluster_list);
//        is_diff = compare_clusters(t1_cluster_list, t2_cluster_list, t1->ntaxa-1);
//
//
//        free(t1_cluster_list);
//    }
//    printf("\n\nis diff: %i\n\n", is_diff);
//
//    printf("Tree 1:\n");
//    print_tree(t1, t1->root_node, 0);
//    printf("\nTree 2:\n");
//    print_tree(t2, t2->root_node, 0);
//    for (int i = 0; i < (2*t1->ntaxa-1); i++){
//        printf("%i", i);
//        if (t1_clusters[i] != NULL) free(t1_clusters[i]);
//    }
//    free(t1_clusters);
//    for (int i = 0; i < (2*t2->ntaxa-1); i++){
//        printf("%i", i);
//        if (t2_clusters[i] != NULL) free(t2_clusters[i]);
//    }
//    free(t2_clusters);
//    free(t2_cluster_list);
//    free_tree(t1);
//    free_tree(t2);
//}
//
//
//
//*/
//
