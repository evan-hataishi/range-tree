#include <stdio.h>
#include "range_tree.h"



int main() {
    int data[] = {3, 10, 19, 23, 30, 37, 49, 59, 62, 70, 80, 89, 100, 105};
    range_tree *tree = build_range_tree(data, 14, 0, 13);
//    range_tree *split_node = find_split_node(tree, 80, 80);
//    printf("Split node: %d\n", split_node->data);
//    traverse(tree);
//    print_leaves(tree);
    range_query(tree, 70,70);
    free_range_tree(tree);
    return 0;
}