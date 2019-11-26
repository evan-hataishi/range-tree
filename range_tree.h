//
// Created by evan on 11/24/19.
//

#ifndef RANGE_TREE_H
#define RANGE_TREE_H

#define DIM 3

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

typedef struct point {
    int data[DIM];
} point;

typedef struct range_tree {
    int val;
    // This should only have data when leaf?
    point *p;
    // which dimension does this tree represent in p
    int dim;
    struct range_tree *left;
    struct range_tree *right;
    struct range_tree *inner;
} range_tree;

range_tree *mk_empty_range_tree();

point *mk_new_point(int values[]);

void free_range_tree(range_tree *old);

// end is inclusive
range_tree *build_range_tree(point *points[], int size, int start, int end, int dim);

void traverse(range_tree *tree);

void print_leaves(range_tree *tree);

void print_data(range_tree *t);

int is_leaf(range_tree *tree);

range_tree *find_split_node(range_tree *tree, int xs, int xe);

int point_in_range(range_tree *t, int xs[], int xe[]);

int in_range(range_tree *t, int xs, int xe);

void report_subtree(range_tree *t, int xs[], int xe[], int dim);

void range_query(range_tree *t, int xs[], int xe[], int dim);

void multi_D_range_query(range_tree *t, int xs[], int xe[], int dim);

#endif
