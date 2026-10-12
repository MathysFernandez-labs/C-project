

#include <string.h>
#include "tree.h"
#include <stdbool.h>
#include "min-max.h"

/*--------------------------------------------------------------------*/
Tree
tree_new ()
{
  return NULL;
}

void
tree_delete (Tree tree, void (*delete) (void *))
{
  if (tree)
    {
      tree_delete (tree->left, delete);
      tree_delete (tree->right, delete);
      if (delete)
        delete (tree->data);
      free (tree);
    }
}

TypeCouleur getCouleur (Tree tree) {
  if(tree == NULL) {
    return BLACK;
  }
  else {
    return tree->couleur.couleur;
  }
}

void reequilibrage (Tree * ptree) {

}























bool tree_remove(Tree * ptree, const void *data, size_t size, int (*compare)(const void *, const void *)) {
  if(ptree == NULL || *ptree == NULL || data == NULL || size <= 0) {
    return false;
  }

  if(compare((*ptree)->data, data) == 0){

    // Si au moins 1 enfant gauche:
    if ((*ptree)->left != NULL) {

      // Si 2 enfants:
      if ((*ptree)->right != NULL) {
        Tree remplacant = (*ptree)->right;
        
        // Chercher le remplacant:
        while(remplacant->left != NULL) {
          remplacant = remplacant->left;
        }
        
        // échange de donnée;
        void *donnee_temporaire = (*ptree)->data;
        (*ptree)->data = remplacant->data;
        remplacant->data = donnee_temporaire;

        tree_remove(&((*ptree)->right), donnee_temporaire, size, compare);
      }

      // Si 1 enfant gauche:
      else {
        Tree temporaireTree = *ptree;
        *ptree = (*ptree)->left;

        (*ptree)->parent = temporaireTree->parent;

        if(temporaireTree->couleur == NOIR) {
          reequilibrage(&(*ptree));
        }
        free(temporaireTree->data);
        free(temporaireTree);
      }
    }

    // Si seulement enfant droite:
    else if ((*ptree)->right != NULL) {
      Tree temporaireTree = *ptree;
      *ptree = (*ptree)->right;

      (*ptree)->parent = temporaireTree->parent;

      if(temporaireTree->couleur == NOIR) {
        reequilibrage(&(*ptree));
      }

      free(temporaireTree->data);
      free(temporaireTree);
    }

    // Si aucun enfant:
    else {
      Tree temporaireTree = *ptree;
      (*ptree) = NULL;

      if(temporaireTree->couleur == NOIR) {
        reequilibrage(&(*ptree));
      }

      free(temporaireTree->data);
      free(temporaireTree);
    }

    return true;
  }
  else if (compare((*ptree)->data, data) < 0 && (*ptree)->right != NULL) {
    return tree_remove(&(*ptree)->right, data, size, compare);
  }
  else if (compare((*ptree)->data, data) > 0 && (*ptree)->left != NULL) {
    return tree_remove(&(*ptree)->left, data, size, compare);
  }
  else {
    return false;
  }
}





















Tree tree_create (const void *data, size_t size, Tree parent) {
  Tree tree = (Tree) malloc(sizeof(*tree));
  if (tree){
    tree->left = NULL;
    tree->right = NULL;
    tree->parent = parent;
    tree->couleur.couleur = RED;

    tree->data = malloc(size);
    if (tree->data) {
      memcpy (tree->data, data, size);
    }
    else {
      free(tree);
      return NULL;
    }
  }

  return tree;
};

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
                void (*func) (void *, void *),
                void *extra_data)
{
  if (tree)
    {
      func (tree->data, extra_data);
      tree_pre_order (tree->left, func, extra_data);
      tree_pre_order (tree->right, func, extra_data);
    }
}

void
tree_in_order (Tree tree,
               void (*func) (void *, void *),
               void *extra_data)
{
  if (tree)
    {
      tree_in_order (tree->left, func, extra_data);
      func (tree->data, extra_data);
      tree_in_order (tree->right, func, extra_data);
    }
}

void
tree_post_order (Tree tree,
                 void (*func) (void *, void *),
                 void *extra_data)
{
  if (tree)
    {
      tree_post_order (tree->left, func, extra_data);
      tree_post_order (tree->right, func, extra_data);
      func (tree->data, extra_data);
    }
}

size_t
tree_height (Tree tree)
{
  if (tree)
    return 1 + MAX (tree_height (tree->left),
tree_height (tree->right));
  else
    return 0;
}

size_t
tree_size (Tree tree)
{
  if (tree)
    return 1 + tree_size (tree->left) + tree_size
(tree->right);
  else
    return 0;
}

bool
tree_insert_sorted (Tree * ptree,
                    const void *data,
                    size_t size,
                    int (*compare) (const void *, const
void *))
{
  if (*ptree)
    {
      switch (compare (data, (*ptree)->data))
        {
        case -1:
          return tree_insert_sorted (&(*ptree)->left,
data, size, compare);
        default:
          return tree_insert_sorted (&(*ptree)->right,
data, size, compare);
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

void *
tree_search (Tree tree,
             const void *data,
             int (*compare) (const void *, const void
*))
{
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




