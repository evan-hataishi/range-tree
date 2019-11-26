#include <stdio.h>
#include <stdlib.h>
#include "range_tree.h"

// TODO - Need to sort by dimension

int main() {
    int size = 16;
    int x[] = {3, 10, 19, 23, 30, 37, 49, 59, 62, 70, 80, 89, 100, 105, 110, 115};
    point *points[size];
    for (int i = 0; i < size; i++) {
        int data[] = {x[i], i - 15, (i * 50) + 100};
        point *p = mk_new_point(data);
        points[i] = p;
        printf("(%d, %d, %d)\n", data[0], data[1], data[2]);
    }
    range_tree *tree = build_range_tree(points, size, 0, size - 1, 0);
//    range_tree *split_node = find_split_node(tree, 80, 80);
//    printf("Split node: %d\n", split_node->data);
//     traverse(tree->inner->inner);
//    print_leaves(tree->inner->inner);
    // range_query(tree, 36,50);
    int f[] = {62, -7, 500};
    int s[] = {62, -7, 500};
    multi_D_range_query(tree, f, s, 0);


    free_range_tree(tree);
    for (int i = 0; i < size; i++) {
        free(points[i]);
    }

    return 0;
}