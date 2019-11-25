//
// Created by evan on 11/24/19.
//

#ifndef RANGE_TREE_H
#define RANGE_TREE_H

//typedef struct rdt_tree {
//    int num_points;
//
//    struct rdt_tree *left;
//    struct rdt_tree *right;
//
//    /* Bounds... */
//    double hi[RDT_DIM];
//    double lo[RDT_DIM];
//    double mid[RDT_DIM];
//    double rad[RDT_DIM];
//
//    double radius;     /* Radius for RA/DEC */
//
//    ivec *pts;
//} rdt_tree;

typedef struct range_tree {
    int data;
    struct range_tree *left;
    struct range_tree *right;
} range_tree;

range_tree *mk_empty_range_tree();

void free_range_tree(range_tree *old);

// end is inclusive
range_tree *build_range_tree(int values[], int size, int start, int end);

void traverse(range_tree *tree);

void print_leaves(range_tree *tree);

void print_data(range_tree *t);

int is_leaf(range_tree *tree);

range_tree *find_split_node(range_tree *tree, int xs, int xe);

int in_range(range_tree *t, int xs, int xe);

void report_subtree(range_tree *t, int xs, int xe);

void range_query(range_tree *t, int xs, int xe);

#endif
