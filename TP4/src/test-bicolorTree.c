#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"

// Fonction de comparaison
int compare_int(const void *a, const void *b) {
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

extern TypeCouleur couleur_noeud(Tree n); 

int main() {
    Tree arbre = tree_new();
    
    int val1 = 10, val2 = 20, val3 = 5, val4 = 15;

    printf("Début des tests de l'arbre Rouge-Noir...\n");

    // Tests d'insertion
    assert(tree_insert_sorted(&arbre, &val1, sizeof(int), compare_int) == true);
    assert(tree_insert_sorted(&arbre, &val2, sizeof(int), compare_int) == true);
    assert(tree_insert_sorted(&arbre, &val3, sizeof(int), compare_int) == true);
    assert(tree_insert_sorted(&arbre, &val4, sizeof(int), compare_int) == true);

    // Test de la racine (doit toujours être NOIRE)
    assert(couleur_noeud(arbre) == BLACK);

    // Test de recherche (éléments présents)
    int *res1 = (int *)tree_search(arbre, &val1, compare_int);
    assert(res1 != NULL);
    assert(*res1 == 10);

    int *res2 = (int *)tree_search(arbre, &val4, compare_int);
    assert(res2 != NULL);
    assert(*res2 == 15);

    // Test de recherche (élément absent)
    int val_absente = 99;
    assert(tree_search(arbre, &val_absente, compare_int) == NULL);

    // Test de suppression
    assert(tree_remove(&arbre, &val2, sizeof(int), compare_int) == true);
    
    // Vérification que l'élément est bien supprimé
    assert(tree_search(arbre, &val2, compare_int) == NULL);

    // Vérification que la racine est toujours NOIRE après une suppression
    assert(couleur_noeud(arbre) == BLACK);

    // Nettoyage de la mémoire
    // On passe la fonction standard `free` de stdlib pour libérer les données allouées par tree_create
    tree_delete(arbre, free);

    printf("Tous les tests ont réussi !\n");
    return 0;
}