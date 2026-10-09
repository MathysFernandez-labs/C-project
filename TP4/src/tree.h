#ifndef _TREE_H_
#define _TREE_H_

#include <stdlib.h>
#include <stdbool.h>

typedef enum {RED, BLACK} TypeCouleur;

typedef struct InfoCouleur {
  TypeCouleur couleur;
} InfoCouleur;

typedef struct _TreeNode *Tree;

struct _TreeNode{
  Tree left;
  Tree right;
  void* data;
  Tree parent;
  InfoCouleur couleur;
};


Tree tree_new ();

void tree_delete (Tree tree, void (*delete) (void *));

// A Faire
bool tree_remove(Tree * ptree, const void *data, size_t size, int (*compare)(const void *, const void *));

// Nouveau
Tree tree_create (const void *data, size_t size, Tree parent);

Tree tree_get_left (Tree tree);

Tree tree_get_right (Tree tree);

void *tree_get_data (Tree tree);

bool tree_set_left(Tree tree, Tree left);

bool tree_set_right (Tree tree, Tree right);

bool tree_set_data(Tree tree, const void *data, size_t size);

void tree_pre_order (Tree tree,
                     void (*func) (void *, void *),
                     void *extra_data);

void tree_in_order (Tree tree,
                    void (*func) (void *, void *),
                    void *extra_data);

void tree_post_order (Tree tree,
                      void (*func) (void *, void *),
                      void *extra_data);

size_t tree_height (Tree tree);

size_t tree_size (Tree tree);

// A Faire
bool tree_insert_sorted(Tree * ptree,
                        const void *data,
                        size_t size,
                        int (*compare) (const void *, const void *));

// A Faire
void *tree_search (Tree tree,
                   const void *data,
                   int (*compare) (const void *, const
void *));

int tree_sort (void *array,
               size_t length,
               size_t size,
               int (*compare) (const void *, const void
*));

#endif
