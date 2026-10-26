

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

// Fonction pour retourner la couleur du noeud
// Noir si NULL car une feuille
TypeCouleur couleur_noeud(Tree n) {
    if (n == NULL) {
      return BLACK;
    }
    else {
      return n->couleur.couleur;
    }
}

// Fonction pour retrouver la racine
// return NULL si noeud est NULL
Tree trouver_racine(Tree noeud) {
    if (noeud == NULL) {
      return NULL;
    }

    while (noeud->parent != NULL) {
        noeud = noeud->parent;
    }

    return noeud;
}




void rotate_L(Tree *racine, Tree x) {
    if (x == NULL || x->right == NULL) return;
    
    Tree y = x->right;
    x->right = y->left;
    
    if (y->left != NULL) {
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == NULL) {
        *racine = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
}

void rotate_R(Tree *racine, Tree x) {
    if (x == NULL || x->left == NULL) return;
    
    Tree y = x->left;
    x->left = y->right;
    
    if (y->right != NULL) {
        y->right->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == NULL) {
        *racine = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    
    y->right = x;
    x->parent = y;
}




// paramètres: 
//  - racine, 
//  - l'arbre actuel, 
//  - arbre du parent de x
void tree_delete_reequilibrage(Tree *racine, Tree x, Tree parent_de_x) {
    Tree w;

    while (x != *racine && couleur_noeud(x) == BLACK) {
        
        if (x == parent_de_x->left) {
            w = parent_de_x->right;

            if (couleur_noeud(w) == RED) {
                w->couleur.couleur = BLACK;
                parent_de_x->couleur.couleur = RED;
                rotate_L(racine, parent_de_x);
                w = parent_de_x->right;
            }

            if (couleur_noeud(w->left) == BLACK && couleur_noeud(w->right) == BLACK) {
                if (w != NULL) {
                    w->couleur.couleur = RED;
                }
                x = parent_de_x; 
                parent_de_x = x->parent;
            } 
            else {
                if (couleur_noeud(w->right) == BLACK) {
                    if (w->left != NULL) {
                        w->left->couleur.couleur = BLACK;
                    }
                    w->couleur.couleur = RED;
                    rotate_R(racine, w);
                    w = parent_de_x->right;
                }

                if (w != NULL) {
                    w->couleur.couleur = parent_de_x->couleur.couleur;
                }
                parent_de_x->couleur.couleur = BLACK;
                
                if (w != NULL && w->right != NULL) {
                    w->right->couleur.couleur = BLACK;
                }
                
                rotate_L(racine, parent_de_x);
                
                x = *racine;
            }
        } 
        
        else {
            w = parent_de_x->left;

            if (couleur_noeud(w) == RED) {
                w->couleur.couleur = BLACK;
                parent_de_x->couleur.couleur = RED;
                rotate_R(racine, parent_de_x);
                w = parent_de_x->left;
            }

            if (couleur_noeud(w->right) == BLACK && couleur_noeud(w->left) == BLACK) {
                if (w != NULL) {
                    w->couleur.couleur = RED;
                }
                x = parent_de_x;
                parent_de_x = x->parent;
            } 
            else {
                if (couleur_noeud(w->left) == BLACK) {
                    if (w->right != NULL) {
                        w->right->couleur.couleur = BLACK;
                    }
                    w->couleur.couleur = RED;
                    rotate_L(racine, w);
                    w = parent_de_x->left;
                }

                if (w != NULL) {
                    w->couleur.couleur = parent_de_x->couleur.couleur;
                }
                parent_de_x->couleur.couleur = BLACK;
                
                if (w != NULL && w->left != NULL) {
                    w->left->couleur.couleur = BLACK;
                }
                
                rotate_R(racine, parent_de_x);
                
                x = *racine;
            }
        }
    }

    if (x != NULL) {
        x->couleur.couleur = BLACK;
    }
}


















bool tree_remove(Tree *ptree, const void *data, size_t size, int (*compare)(const void *, const void *)) {
  if (ptree == NULL || *ptree == NULL || data == NULL || size <= 0) {
      return false;
  }

  // Recherche du noeud à supprimer
  Tree z = *ptree;
  while (z != NULL) {
    int cmp = compare(data, z->data);
    if (cmp == 0) {
        break;
    } else if (cmp == -1) {
        z = z->left;
    } else {
        z = z->right;
    }
  }

  // Si la donnée n'est pas dans l'arbre
  if (z == NULL) {
      return false;
  }

  // Si le noeud a deux enfants, on trouve son successeur
  if (z->left != NULL && z->right != NULL) {
    Tree y = z->right;
    while (y->left != NULL) {
        y = y->left;
    }
    
    // Échange de données
    void *donnee_temporaire = z->data;
    z->data = y->data;
    y->data = donnee_temporaire;
    
    z = y; 
  }

  // Now, z a au maximum un enfant
  Tree x = (z->left != NULL) ? z->left : z->right;
  Tree parent_de_x = z->parent;
  TypeCouleur couleur_z_supprime = couleur_noeud(z);
  
  if (x != NULL) {
    x->parent = z->parent;
  }

  if (z->parent == NULL) {
      *ptree = x;
  } else if (z == z->parent->left) {
      z->parent->left = x;
  } else {
      z->parent->right = x;
  }

  // violation couleur supprimé ? 
  if (couleur_z_supprime == BLACK) {
    // On fait appel à la fonction de rééquilibrage
    tree_delete_reequilibrage(ptree, x, parent_de_x);
  }

  // Libération
  free(z->data);
  free(z);

  return true;
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


// Fonction de réequilibrage spécifique à l'insert
void tree_insert_reequilibrage(Tree *racine, Tree x) {
  Tree oncle;
  
  while (x != *racine && couleur_noeud(x->parent) == RED) {
      
      // Si le parent de x est le fils gauche du grand-père
      if (x->parent == x->parent->parent->left) {
          oncle = x->parent->parent->right;
          
          // C1: L'oncle est ROUGE
          if (couleur_noeud(oncle) == RED) {
              x->parent->couleur.couleur = BLACK;
              oncle->couleur.couleur = BLACK;
              x->parent->parent->couleur.couleur = RED;
              x = x->parent->parent; // On remonte
          } 
          // Cas2&3: L'oncle est NOIR
          else {
              // C2: x est un fils droit
              if (x == x->parent->right) {
                  x = x->parent;
                  rotate_L(racine, x);
              }
              // C3: x est un fils gauche
              x->parent->couleur.couleur = BLACK;
              x->parent->parent->couleur.couleur = RED;
              rotate_R(racine, x->parent->parent);
          }
      } 
      else {
          oncle = x->parent->parent->left;
          
          if (couleur_noeud(oncle) == RED) {
              x->parent->couleur.couleur = BLACK;
              oncle->couleur.couleur = BLACK;
              x->parent->parent->couleur.couleur = RED;
              x = x->parent->parent;
          } else {
              if (x == x->parent->left) {
                  x = x->parent;
                  rotate_R(racine, x);
              }
              x->parent->couleur.couleur = BLACK;
              x->parent->parent->couleur.couleur = RED;
              rotate_L(racine, x->parent->parent);
          }
      }
  }
  // La racine doit finir BLACK
  (*racine)->couleur.couleur = BLACK;
}

bool tree_insert_sorted(Tree *ptree, const void *data, size_t size, int (*compare)(const void *, const void *)) {
    Tree y = NULL;
    Tree x = *ptree;

    // Descente dans l'arbre pour trouver la position d'insertion
    while (x != NULL) {
        y = x;
        if (compare(data, x->data) == -1) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    Tree new_node = tree_create(data, size, y);
    // si creation échouer
    if (new_node == NULL) {
        return false;
    }

    if (y == NULL) {
        *ptree = new_node;
    } else if (compare(data, y->data) == -1) {
        y->left = new_node;
    } else {
        y->right = new_node;
    }

    // Appel du rééquilibrage Rouge-Noir
    tree_insert_reequilibrage(ptree, new_node);
    
    return true;
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




