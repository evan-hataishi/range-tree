#include <stdio.h>
#include "range_tree.h"

// TODO - Need to sort by dimension

int main() {
    int x[] = {3, 10, 19, 23, 30, 37, 49, 59, 62, 70, 80, 89, 100, 105, 110};
    point *points[15];
    for (int i = 0; i < 15; i++) {
        int data[] = {x[i], i};
        point *p = mk_new_point(data);
        points[i] = p;
    }
     range_tree *tree = build_range_tree(points, 15, 0, 14, 0);
//    range_tree *split_node = find_split_node(tree, 80, 80);
//    printf("Split node: %d\n", split_node->data);
//     traverse(tree);
//    print_leaves(tree);
//    range_query(tree, 3,100);
    free_range_tree(tree);
    return 0;
}