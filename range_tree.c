
#include "range_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

range_tree *mk_empty_range_tree() {
    range_tree *res = (range_tree *) malloc(sizeof(range_tree));

    res->val = 0;

    res->p = NULL;
    res->dim = 0;

    res->left = NULL;
    res->right = NULL;
    res->inner = NULL;

    return res;
}

point *mk_new_point(int values[]) {
    point *res = (point *) malloc(sizeof(point));
    for (int i = 0; i < DIM; i++) {
        res->data[i] = values[i];
    }
    return res;
}

void free_range_tree(range_tree *old) {
    if (old == NULL) {
        return;
    }

    if (old->left) {
        free_range_tree(old->left);
    }
    if (old->right) {
        free_range_tree(old->right);
    }

    if (old->inner) {
        free_range_tree(old->inner);
    }

    // TODO - should I be freeing points here?
//    if (old->p) {
//        free(old->p);
//    }

    free(old);

    // make sure we never use it again
    old = NULL;
}

// end is inclusive
range_tree *build_range_tree(point *points[], int size, int start, int end, int dim) {
    if (start > end) {
        return NULL;
    }

    range_tree *root = mk_empty_range_tree();

    // TODO - decide what median should be
    int mid = (start + end + 1) / 2;

    // printf("MID: %d ITEM: %d DIM: %d\n", mid, points[mid]->data[dim], dim);

    root->val = points[mid]->data[dim];
    root->left = build_range_tree(points, size, start, mid - 1, dim);
    root->right = build_range_tree(points, size, mid + 1, end, dim);
    root->dim = dim;

    // Create a left leaf
    if (root->left == NULL) {
        // printf("Creating a left leaf\n");
        root->left = mk_empty_range_tree();
        root->left->val = points[mid]->data[dim];
        // Actually store the point if leaf
        root->left->p = points[mid];
//        if (root->left->p != NULL) {
//            printf("MID: %d, dim: %d\n", mid, dim);
//        }
        // printf("left leaf %d\n", root->val);
        root->left->dim = dim;
    }

    // Create a right leaf
    if (root->right == NULL) {
        if ((mid + 1) <= size - 1) {
            // printf("Creating a right leaf\n");
            root->right = mk_empty_range_tree();
            root->right->val = points[mid + 1]->data[dim];
            // Actually store the point if leaf
            root->right->p = points[mid + 1];
//            if (root->right->p != NULL) {
//                printf("MID: %d, dim: %d\n", mid+1, dim);
//            }
            // printf("right leaf %d\n", mid);
            root->right->dim = dim;
        }
    }

    // Build d-1 range tree if not leaf
    // TODO must resort data on dimension when building inner tree
    // Dimensions are 0, 1, 2, ..., DIM - 1
    if (!is_leaf(root) && dim < (DIM - 1) && root->inner == NULL) {
        root->inner = build_range_tree(points, size, start, end, dim + 1);
    }


    return root;
}

void traverse(range_tree *tree) {
    if (tree == NULL) {
        return;
    }

    // Don't print leaves
    if (is_leaf(tree)) {
        assert(tree->inner == NULL);
        return;
    }

    // Only node values in innermost tree
//    if (tree->dim == (DIM-1)) {
//        printf("%d\n", tree->val);
//    }


    traverse(tree->left);
    printf("Split val: %d\n", tree->val);
    traverse(tree->right);
    // traverse(tree->inner);
}

void print_leaves(range_tree *tree) {
    if (tree == NULL) {
        return;
    }

    if (is_leaf(tree)) {
        print_data(tree);
    }

    print_leaves(tree->left);
    print_leaves(tree->right);
}

void print_data(range_tree *t) {
    if (t == NULL) {
        printf("Tree is NULL\n");
    } else {
        printf("Split Value: %d, DIM: %d, (%d, %d, %d)\n", t->val, t->dim, t->p->data[0], t->p->data[1], t->p->data[2]);
    }
}

int is_leaf(range_tree *tree) {
    if (tree == NULL) {
        return 1;
    } else if (tree->left == NULL && tree->right == NULL) {
        assert(tree->inner == NULL);
        return 1;
    }
    return 0;
    // return tree == NULL || (tree->left == NULL && tree->right == NULL);
}

int point_in_range(range_tree *t, int xs[], int xe[]) {
    assert((t != NULL) && is_leaf(t) && (t->p != NULL));
    for (int i = 0; i < DIM; i++) {
        if (t->p->data[i] >= xs[i] && t->p->data[i] <= xe[i]) {
            continue;
        } else {
            return 0;
        }
    }
    return 1;
}

// TODO - what does this return if we're out of range?
range_tree *find_split_node(range_tree *tree, int xs, int xe) {
    range_tree *v = tree;
    while ((v != NULL) && !is_leaf(v) && (xe <= v->val || xs > v->val)) {
        if (xe <= v->val) {
            v = v->left;
        } else {
            v = v->right;
        }
    }
    return v;
}

int in_range(range_tree *t, int xs, int xe) {
    if (t == NULL) {
        return 0;
    }
    return t->val >= xs && t->val <= xe;
}

// TODO  does this do unnecessary work?
void report_subtree(range_tree *t, int xs[], int xe[], int dim) {
    assert(dim == (DIM - 1));
    if (t == NULL) {
        return;
    }

    // assert(t->dim == (DIM - 1));

    if (is_leaf(t)) {
        assert(t->p != NULL);
        if (point_in_range(t, xs, xe)) {
            printf("Found (%d, %d, %d)\n", t->p->data[0], t->p->data[1], t->p->data[2]);
            // print_data(t);
        }
        return;
    }

    if (in_range(t, xs[dim], xe[dim])) {
        // I'm in range, so let's check both children
        report_subtree(t->left, xs, xe, dim);
        report_subtree(t->right, xs, xe, dim);
    } else {
        // Not in range, but may have missed a left/right child. Prune search space.
        if (t->val >= xe[dim]) {
            report_subtree(t->left, xs, xe, dim);
        }
        if (t->val <= xs[dim]) {
            report_subtree(t->right, xs, xe, dim);
        }
    }

}

void range_query(range_tree *t, int xs[], int xe[], int dim) {
    printf("RANGE [%d, %d]\n", xs[dim], xe[dim]);
    range_tree *v_split = find_split_node(t, xs[dim], xe[dim]);
    if (v_split == NULL) {
        printf("No nodes in range...\n");
        return;
    }
    printf("Split node: %d\n", v_split->val);
    if (is_leaf(v_split)) {
        report_subtree(v_split, xs, xe, dim);
    } else {
        // Search right subtrees of left split
        range_tree *v = v_split->left;
        while (!is_leaf(v)) {
            if (xs[dim] <= v->val) {
                report_subtree(v->right, xs, xe, dim);
                v = v->left;
            } else {
                v = v->right;
            }
        }
        report_subtree(v, xs, xe, dim);
        // Search left subtrees of right split
        v = v_split->right;
        while (!is_leaf(v)) {
            if (xe[dim] >= v->val) {
                report_subtree(v->left, xs, xe, dim);
                v = v->right;
            } else {
                v = v->left;
            }
        }
        report_subtree(v, xs, xe, dim);
    }
}

void multi_D_range_query(range_tree *t, int xs[], int xe[], int dim) {
    range_tree *v_split = t;
    while (dim < DIM - 1) {
        printf("DIM: %d [%d, %d]\n", dim, xs[dim], xe[dim]);
        v_split = find_split_node(v_split, xs[dim], xe[dim]);
        if (v_split == NULL) {
            printf("No nodes found in range...\n");
            return;
        } else if (is_leaf(v_split)) {
            break;
        }
        v_split = v_split->inner;
        dim += 1;
        assert(v_split->dim == dim);
    }

    printf("Outside loop, DIM: %d [%d, %d]\n", dim, xs[dim], xe[dim]);

    if (v_split == NULL) {
        printf("FML is this supposed to happen?\n");
        return;
    }

    if (is_leaf(v_split)) {
        for (int i = 0; i < DIM; i++) {
            if (!point_in_range(t, xs, xe)) {
                printf("Found leaf, but not in range\n");
                return;
            }
        }
        print_data(v_split);
        return;
    }

    assert(v_split->dim == (DIM - 1));

    printf("Split node: %d\n", v_split->val);

    // print_leaves(v_split);
    range_query(v_split, xs, xe, dim);
}