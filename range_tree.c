
#include "range_tree.h"

#include <stdio.h>
#include <stdlib.h>

range_tree *mk_empty_range_tree() {
    range_tree *res = (range_tree *) malloc(sizeof(range_tree));

    res->data = 0;
    res->left = NULL;
    res->right = NULL;

    return res;
}

void free_range_tree(range_tree *old) {
    if (old->left) {
        free_range_tree(old->left);
    }
    if (old->right) {
        free_range_tree(old->right);
    }

    free(old);

    // make sure we never use it again
    old = NULL;
}

// end is inclusive
range_tree *build_range_tree(int values[], int size, int start, int end) {
    if (start > end) {
        return NULL;
    }

    range_tree *root = mk_empty_range_tree();

    // TODO - decide what median should be
    int mid = (start + end + 1) / 2;

    root->data = values[mid];
    root->left = build_range_tree(values, size, start, mid - 1);
    root->right = build_range_tree(values, size, mid + 1, end);

    // Create a left leaf
    if (root->left == NULL) {
        root->left = mk_empty_range_tree();
        root->left->data = values[mid];
//        printf("left leaf %d\n", root->data);
    }

    // Create a right leaf
    if (root->right == NULL) {
        if ((mid + 1) <= size - 1) {
            root->right = mk_empty_range_tree();
            root->right->data = values[mid + 1];
//            printf("right leaf %d\n", mid);
        }
    }

    return root;
}

void traverse(range_tree *tree) {
    if (tree == NULL) {
        return;
    }

    // Don't print leaves
    if (tree->left == NULL && tree->right == NULL) {
        return;
    }

    printf("%d\n", tree->data);
    traverse(tree->left);
    traverse(tree->right);
}

void print_leaves(range_tree *tree) {
    if (tree == NULL) {
        return;
    }

    if (tree->left == NULL && tree->right == NULL) {
        printf("%d\n", tree->data);
    }

    print_leaves(tree->left);
    print_leaves(tree->right);
}

void print_data(range_tree *t) {
    if (t == NULL) {
        printf("Tree is NULL\n");
    } else {
        printf("Data: %d\n", t->data);
    }
}

int is_leaf(range_tree *tree) {
    return tree == NULL || (tree->left == NULL && tree->right == NULL);
}

range_tree *find_split_node(range_tree *tree, int xs, int xe) {
    range_tree *v = tree;
    while (!is_leaf(v) && (xe <= v->data || xs > v->data)) {
        if (xe <= v->data) {
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
    return t->data >= xs && t->data <= xe;
}

// TODO  does this do unnecessary work?
void report_subtree(range_tree *t, int xs, int xe) {
    if (t == NULL) {
        return;
    }

    if (is_leaf(t)) {
        if (in_range(t, xs, xe)) {
            print_data(t);
        }
        return;
    }

    if (in_range(t, xs, xe)) {
        // I'm in range, so let's check both children
        report_subtree(t->left, xs, xe);
        report_subtree(t->right, xs, xe);
    } else {
        // Not in range, but may have missed a left/right child. Prune search space.
        if (t->data >= xe) {
            report_subtree(t->left, xs, xe);
        }
        if (t->data <= xs) {
            report_subtree(t->right, xs, xe);
        }
    }

}

void range_query(range_tree *t, int xs, int xe) {
    range_tree *v_split = find_split_node(t, xs, xe);
    if (v_split == NULL) {
        printf("No nodes in range...\n");
        return;
    }
    printf("Split node: %d\n", v_split->data);
    if (is_leaf(v_split)) {
        report_subtree(v_split, xs, xe);
    } else {
        range_tree *v = v_split->left;
        while (!is_leaf(v)) {
            if (xs <= v->data) {
                report_subtree(v->right, xs, xe);
                v = v->left;
            } else {
                v = v->right;
            }
        }
        report_subtree(v, xs, xe);
        v = v_split->right;
        while (!is_leaf(v)) {
            if (xe >= v->data) {
                report_subtree(v->left, xs, xe);
                v = v->right;
            } else {
                v = v->left;
            }
        }
        report_subtree(v, xs, xe);
    }
}
