

#include <string.h>
#include "tree-avl.h"
#include <stdbool.h>
#include "min-max.h"

/*--------------------------------------------------------------------*/
Tree tree_new ()
{
  return NULL;
}

void tree_delete (Tree tree, void (*delete) (void *))
{
  if (tree)
    {
      tree_delete (tree->left, delete);
      tree_delete (tree->right, delete);
      if (delete)
        delete(tree->data);
      free (tree);
    }
}

void tree_delete_unit_node (Tree *tree, void (*delete)(void *)) {
  if (tree && *tree) {
    if((*tree)->data != NULL) {
      delete((*tree)->data);
    }
    free(*tree);
  }
}


// Déja appelé de manière récursive dans insert
void update_balance(Tree k2) {
  // Sur la remonté:

  // Si feuille:
  if(k2->left == NULL && k2->right == NULL) {
    k2->balance = 0;
  }

  // Si seulement branche G
  else if(k2->left != NULL && k2->right == NULL) {
    k2->balance = tree_height(k2->left);
  }

  // Si seulement branche D
  else if(k2->left == NULL && k2->right != NULL) {
    k2->balance = - tree_height(k2->right);
  }

  // Si 2 branches
  else if(k2->left != NULL && k2->right != NULL) {
    int k2LH = tree_height(k2->left);
    int k2RH = tree_height(k2->right);
    
    k2->balance = k2LH - k2RH;
  }
}

// fonction pour supprimer un noeud sans supprimer ses enfants
int tree_delete_node (Tree * tree, const void *data, int (*compare), void (*delete) (void *), size_t size) {
  if (tree && *tree){
    // -1 ou 1 on continue de chercher
    // 0 on a trouvé alors on supprime
    switch (compare (data, (*tree)->data)){
      case -1:
        if((*tree)->left != NULL) {
          return tree_delete_node (&((*tree)->left), data, compare, delete, size);
        }
        return 0;
      
      case 1:
        if((*tree)->right != NULL) {
          return tree_delete_node (&((*tree)->right), data, compare, delete, size);
        }
        return 0;

      case 0:
        // Noeuds trouvé
        // Suppression

        // Pas d'enfant:
        if((*tree)->right == NULL && (*tree)->left == NULL) {
          tree_delete_unit_node(*tree, delete);
          *tree = NULL;
        }

        // 1 enfant:
        // A gauche
        if((*tree)->right == NULL && (*tree)->left != NULL) {
          Tree k2 = *tree;
          *tree = (*tree)->left;
          (*tree)->parent = k2->parent;
          tree_delete_unit_node(k2, delete);
          update_balance(*tree);
          
        }

        // 1 enfant:
        // A droite
        if ((*tree)->right != NULL && (*tree)->left == NULL)) {
          Tree k2 = *tree;
          *tree = (*tree)->right;
          (*tree)->parent = k2->parent;
          tree_delete_unit_node(k2, delete);
          update_balance(*tree);
        }

        // 2 enfants:
        if((*tree)->right != NULL && (*tree)->left != NULL){
          // Enfant le plus a droite de l'enfant gauche:
          if((*tree)->left->right != NULL) {
            Tree k1 = (*tree)->left->right;
            while(k1->right != NULL) {
              k1 = k1->right;
            }
            k1->parent->right = NULL;
            memcpy((*tree)->data, k1->data, size);
            tree_delete_unit_node(k1, delete);
            update_balance(*tree);
          }

          // Pas d'enfant droite à l'enfant gauche
          // On remplace k2 par k2->left (k1);
          else {
            Tree k2 = *tree;
            Tree k1 = k2->left;
            k1->parent = k2->parent;

            k1->right = k2->right;
            *tree = k1;
            tree_delete_unit_node(k1, delete);
            tree_delete_unit_node(k2, delete);
            update_balance(*tree);
          }
        }
        return 1;
      default:
        return 0;
    }
    
    // Penche trop a droite
    if((*tree)->balance < -1) {
      // Penche trop a gauche (D-G)
      if((*tree)->right->balance > 0){
        rotate_R(&((*tree)->right));
      }
      rotate_L(tree);
    }

    // Penche trop a gauche
    else if ((*tree)->balance > 1) {
      // Penche trop a droite (G-D)
      if((*tree)->left->balance < 0){
        rotate_L(&((*tree)->left));
      }
      rotate_R(tree);
    }
  }
  else {
    return 0;
  }
}























Tree
tree_create (const void *data, size_t size, int balance, Tree parent)
{
  Tree tree = (Tree) malloc (sizeof (*tree) + size);
  if (tree)
    {
      tree->left = NULL;
      tree->right = NULL;
      memcpy (tree->data, data, size);
      tree->balance = balance;
      tree->parent = parent;
    }

  return tree;
}

Tree
tree_get_left (Tree tree)
{
  if (tree)
    return tree->left;
  else
    return NULL;
}

Tree
tree_get_right (Tree tree)
{
  if (tree)
    return tree->right;
  else
    return NULL;
}

void *
tree_get_data (Tree tree)
{
  if (tree)
    return tree->data;
  else
    return NULL;
}

bool
tree_set_left (Tree tree, Tree left)
{
  if (tree)
    {
      tree->left = left;
      return true;
    }
  else
    return false;
}

bool
tree_set_right (Tree tree, Tree right)
{
  if (tree)
    {
      tree->right = right;
      return true;
    }
  else
    return false;
}

bool
tree_set_data (Tree tree, const void *data, size_t
size)
{
  if (tree)
    {
      memcpy (tree->data, data, size);
      return true;
    }
  else
    return false;
}
void
tree_pre_order (Tree tree,
                void (*func) (void *, int, void *),
                void *extra_data)
{
  if (tree)
    {
      func (tree->data, tree->balance, extra_data);
      tree_pre_order (tree->left, func, extra_data);
      tree_pre_order (tree->right, func, extra_data);
    }
}

void
tree_in_order (Tree tree,
               void (*func) (void *, int, void *),
               void *extra_data)
{
  if (tree)
    {
      tree_in_order (tree->left, func, extra_data);
      func (tree->data, tree->balance, extra_data);
      tree_in_order (tree->right, func, extra_data);
    }
}

void
tree_post_order (Tree tree,
                 void (*func) (void *, int, void *),
                 void *extra_data)
{
  if (tree)
    {
      tree_post_order (tree->left, func, extra_data);
      tree_post_order (tree->right, func, extra_data);
      func (tree->data, tree->balance, extra_data);
    }
}

size_t
tree_height (Tree tree){
  if (tree)
    return 1 + MAX (tree_height (tree->left), tree_height (tree->right));
  else
    return 0;
}

size_t
tree_size (Tree tree){
  if (tree)
    return 1 + tree_size (tree->left) + tree_size
    (tree->right);
  else
    return 0;
}







// Vielle Version

/*
bool tree_insert_sorted (Tree * ptree,const void *data,size_t size,int (*compare) (const void *, const void *)) {
  if (*ptree)
    {
      switch (compare (data, (*ptree)->data))
        {
        case -1:
          return tree_insert_sorted (&(*ptree)->left, data, size, compare);
        default:
          return tree_insert_sorted (&(*ptree)->right, data, size, compare);
        }
    }
  else
    {
      Tree new = tree_create (data, size);
      if (new)
        {
          *ptree = new;
          return true;
        }
      else
        return false;
    }

}
*/

void rotate_L (Tree *tree) {
  if(tree == NULL || *tree == NULL) {
    return;
  }
  Tree k2 = *tree;
  Tree k1 = k2->right;

  if(k1 == NULL) {
    return;
  }
  k2->right = k1->left;

  if(k1->left != NULL) {
    k1->left->parent = k2;
  }
  k1->parent = k2->parent;
  k2->parent = k1;
  k1->left = k2;
  *tree = k1;

  update_balance(k2);
  update_balance(k1);
}


void rotate_R (Tree *tree) {
  if(tree == NULL || *tree == NULL) {
    return;
  }
  Tree k2 = *tree;
  Tree k1 = k2->left;
  
  if(k1 == NULL) {
    return;
  }

  k2->left = k1->right;

  if(k1->right != NULL) {
    k1->right->parent = k2;
  }

  k1->parent = k2->parent;
  k1->right = k2;
  k2->parent = k1;
  *tree = k1;

  update_balance(k2);
  update_balance(k1);
}

bool tree_insert_sorted (Tree * ptree,const void *data,size_t size,int (*compare) (const void *, const void *)) {
  if (*ptree) {
    bool result = false;
    switch (compare (data, (*ptree)->data))
      {
      case -1:
        result = tree_insert_sorted (&(*ptree)->left, data, size, compare);
        if (result) {
          if((*ptree)->left->parent == NULL) {
            // MAJ parent
            (*ptree)->left->parent = *ptree;
          }

          // MAJ de la balance sur la remonter
          update_balance(*ptree);

          // Penche trop a droite
          if((*ptree)->balance < -1) {
            // Penche trop a gauche (D-G)
            if((*ptree)->right->balance > 0){
              rotate_R(&((*ptree)->right));
            }
            rotate_L(ptree);
          }

          // Penche trop a gauche
          else if ((*ptree)->balance > 1) {
            // Penche trop a droite (G-D)
            if((*ptree)->left->balance < 0){
              rotate_L(&((*ptree)->left));
            }
            rotate_R(ptree);
          }
        }
        return result;

      default:
        result = tree_insert_sorted (&(*ptree)->right, data, size, compare);
        if (result) {
          if((*ptree)->right->parent == NULL) {
            // MAJ parent
            (*ptree)->right->parent = *ptree;
          }

          // MAJ de la balance sur la remonter
          update_balance(*ptree);

          // Penche trop a droite
          if((*ptree)->balance < -1) {
            // Penche trop a gauche (D-G)
            if((*ptree)->right->balance > 0){
              rotate_R(&((*ptree)->right));
            }
            rotate_L(ptree);
          }

          // Penche trop a gauche
          else if ((*ptree)->balance > 1) {
            // Penche trop a droite (G-D)
            if((*ptree)->left->balance < 0){
              rotate_L(&((*ptree)->left));
            }
            rotate_R(ptree);
          }
        }
        return result;
    }
  }
  else{
    Tree new = tree_create (data, size, 0, NULL);
    if (new){
      *ptree = new;
      return true;
    }
    else{
      return false;
    }
  }

}
























void *
tree_search (Tree tree, const void *data, int (*compare) (const void *, const void*)){
  if (tree)
    {
      switch (compare (data, tree->data))
        {
        case -1:
          return tree_search (tree->left, data, compare);
        case 0:
          return tree->data;
        case 1:
          return tree_search (tree->right, data, compare);
        default:
          return NULL; // RAJOUTE CAR WARNING !!!
        }
    }
  else
    return NULL;
}

static void
set (void *data,void *array)
{
  static size_t size;
  static size_t offset;

  if (data)
    {
      memcpy (array + offset, data, size);
      offset += size;
    }
  else
    {
      offset = 0;
      size = *(size_t *) array;
    }
}

int
tree_sort (void *array,
           size_t length,
           size_t size,
           int (*compare) (const void *, const void *))
{
  size_t i;
  Tree tree = tree_new ();
  void *pointer;

  pointer = array;
  for (i = 0; i < length; i++)
    {
      if (tree_insert_sorted (&tree, pointer, size,
compare))
        pointer += size;
      else
        {
          tree_delete (tree, NULL);
          return false;
        }
    }
  set (NULL, &size);
  tree_in_order (tree, set, array);
  tree_delete (tree, NULL);
  return true;
}




