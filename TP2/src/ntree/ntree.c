
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include <unistd.h>

#include "ntree.h"

#include "min-max.h"
//----------------------------------------
NTree ntree_new () {
    return NULL;
}

//----------------------------------------


NTree ntree_create (const void *data, size_t size, size_t arity) {
    NTree tree = (NTree) malloc (sizeof (NTree *) + sizeof (size_t) + size);
    if (tree) {
        tree->subtrees = (NTree *) malloc (arity * sizeof(NTree));
        if (tree->subtrees) {
            size_t i;

            for (i = 0; i < arity; i++)
                tree->subtrees[i] = NULL;
            tree->arity = arity;
            memcpy(tree->data, data, size);
        }
        else {
            free(tree);
            return 0;
        }
    }
    else
    {
        free(tree);
        tree = NULL;
    }

    return tree;
}
//----------------------------------------


void ntree_delete (NTree tree, void (*deletef) (void *)) {
    if(tree == NULL) {
        return;
    }
    if (tree->arity>0) {
        for (int i =0; i < tree->arity; i++) {
            ntree_delete(tree->subtrees[i], deletef);
        }
    }
    else {
        free(tree);
    }
    return;
}


//----------------------------------------


void *ntree_get_data(NTree tree) {
    if(tree == NULL) {
        return NULL;
    }
    return tree->data;
}
//----------------------------------------


bool ntree_set_nth(NTree tree, NTree sub, size_t nth, void (*deletef) (void *)) {
    if(tree == NULL || sub == NULL || nth > tree->arity) {
        return false;
    }
    
    if(nth < tree->arity) {
        if(tree->subtrees[nth] != NULL) {
            ntree_delete(tree->subtrees[nth], deletef);
        }
        tree->subtrees[nth] = sub;
        return true;
    }

    if(nth == tree->arity) {
        NTree *new_subtrees = realloc(tree->subtrees, (tree->arity+1)*sizeof(NTree));
        if(new_subtrees == NULL) {
            return false;
        }
        tree->subtrees = new_subtrees;
        tree->subtrees[nth] = sub;
        tree->arity++;
        return true;
    }
    return false;
    

    

    return false;
}

//----------------------------------------



bool ntree_set_data(NTree tree, const void *data, size_t size) {
    if(tree == NULL || data == NULL || size == 0) {
        return false;
    }
    if(!memcpy(tree->data, data, size)) {
        return false;
    }
    return true;
}
//----------------------------------------

// profondeur maximale
size_t ntree_height(NTree tree) {
    if (tree == NULL) {
        return 0;
    }

    size_t max_height = 0;
    size_t count_height = 0;

    for(int i = 0; i < tree->arity; i++) {
        count_height = ntree_height(tree->subtrees[i]);
        if (count_height > max_height) {
            max_height = count_height;
        }
    }

    return max_height + 1;
    
}
//----------------------------------------

// nombres d'élements
size_t ntree_size(NTree tree) {
    if (tree == NULL) {
        return 0;
    }

    size_t count = 0;
    for (int i = 0; i<tree->arity; i++) {
        count += ntree_size(tree->subtrees[i]);
    }
    return count + 1;
    
}


//----------------------------------------

void ntree_pre_order(NTree tree, void (*func)(void *, void *), void *extra_data) {
    ntree_in_order(tree, func, extra_data, 0);
}


//----------------------------------------

void ntree_post_order(NTree tree, void (*func)(void *, void *), void *extra_data) {
    ntree_in_order(tree, func, extra_data, tree->arity);
}


//----------------------------------------

void ntree_in_order(NTree tree, void (*func)(void *, void *), void *extra_data, size_t nth) {
    assert(func);
    if(tree) {

        if(nth >= tree->arity) {
            nth = tree->arity;
        }

        for (int i = 0; i<nth; i++) {
            ntree_in_order(tree->subtrees[i], func, extra_data, nth);
        }

        func(tree->data, extra_data);

        for (int j = nth; j<tree->arity; j++) {
            ntree_in_order(tree->subtrees[j], func, extra_data, nth);
        }
    }
}
//----------------------------------------

