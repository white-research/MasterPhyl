#include "tree.h"
#include <array>
#include <exception>
#include <iostream>
#include <map>
#include <stdexcept>
#include <utility>

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

int processClusters(Tree& tree, std::vector<std::vector<int>>& clusters, std::vector<std::array<int, 2>>& cluster_list){
    int inner_node = 0;
    for (int i = 0; i < tree.getNNodes(); i++) {
        int start = -1, end = -1;
        bool is_gap = false;
        for (int j = 0; j < tree.getNTips(); j++) {
        }
        for (int j = 0; j < tree.getNTips(); j++) {
            if (clusters[i][j] == 1){
                if (start == -1) {
                    start = j;
                    end = j;
                }
                else if (end == j-1){
                    end = j;
                }
                else {
                    is_gap = true;
                }
            }
        }
        if (start != end && is_gap == false) {
            cluster_list[inner_node][0] = start + 1;
            cluster_list[inner_node][1] = end + 1;
            inner_node++;
        }
    }
    return inner_node;
}

void treeClusters(Tree& tree, Node& node, std::map<int,int>& labels, std::vector<std::vector<int>>& clusters) {
    if (node.hasDescendents()) { // if an internal node
        // recursively process descendents first
        treeClusters(tree, *(node.desc1), labels, clusters);
        treeClusters(tree, *(node.desc2), labels, clusters);
        //
        for (int i=0; i < tree.getNTips(); i++) {
            if (clusters[node.desc1->get_id() - 1][i] || clusters[node.desc2->get_id() - 1][i]){
                clusters[node.get_id() - 1][i] = 1;
            }
        }
    }
    else { // if a tip node
        for (int i=1; i <= tree.getNTips(); i++) {
            if (i == labels[node.get_id()]) {
                clusters[node.get_id() - 1][i-1] = 1;
            }
        }
    }
}

int robinsonFouldsDistance(Tree& t1, Tree& t2) {

    auto labels = relabelTree(t2);

    std::vector<std::vector<int>> t2_clusters(t2.getNNodes(), std::vector<int>(t2.getNTips()));
    treeClusters(t2, *(t2.getRootNode()), labels, t2_clusters);
    std::vector<std::array<int, 2>> t2_cluster_list(t2.getNNodes()-t2.getNTips(), std::array<int,2>({0,0}));
    int full_cluster_count = processClusters(t2, t2_clusters, t2_cluster_list);

    std::vector<std::vector<int>> t1_clusters(t1.getNNodes(), std::vector<int>(t1.getNTips()));
    treeClusters(t1, *(t1.getRootNode()), labels, t1_clusters);
    std::vector<std::array<int, 2>> t1_cluster_list(t1.getNNodes()-t1.getNTips(), std::array<int,2>({0,0}));
    int partial_cluster_count = processClusters(t1, t1_clusters, t1_cluster_list);

    int similar_count = 0;

    std::multimap<int,int> cluster_map {};
    for (auto pair: t2_cluster_list) {
        cluster_map.insert(std::pair<int,int>(pair[0], pair[1]));
    }
    for (auto pair: t1_cluster_list) {
        auto range = cluster_map.equal_range(pair[0]);
        for (auto it = range.first; it != range.second; ++it) {
            if (it->second == pair[1]) {
                similar_count++;
                break;
            }
        }
    }
    int num_internal_nodes = t1.getNNodes() - t1.getNTips();
    return num_internal_nodes - similar_count;
}
