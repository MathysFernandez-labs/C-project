#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

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

void generer_mesures_arbre_rouge_noir() {
    // Création du fichier de sortie
    FILE *fichier = fopen("mesures_rb.txt", "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    
    // En-tête du fichier
    fprintf(fichier, "N\tInsertion\tRecherche\tSuppression\n");

    // Boucle de 100 à 10000 par pas de 100
    for (int n = 100; n <= 10000; n += 100) {
        Tree arbre = tree_new();
        
        int *donnees = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            donnees[i] = rand() % 100000;
        }

        // Chronométrage de l'insertion
        clock_t debut_ins = clock();
        for (int i = 0; i < n; i++) {
            tree_insert_sorted(&arbre, &donnees[i], sizeof(int), compare_int);
        }
        double temps_ins = (double)(clock() - debut_ins) / CLOCKS_PER_SEC;

        // Chronométrage de la recherche
        clock_t debut_rech = clock();
        for (int i = 0; i < n; i++) {
            tree_search(arbre, &donnees[i], compare_int);
        }
        double temps_rech = (double)(clock() - debut_rech) / CLOCKS_PER_SEC;

        // Chronométrage de la suppression
        clock_t debut_suppr = clock();
        for (int i = 0; i < n; i++) {
            tree_remove(&arbre, &donnees[i], sizeof(int), compare_int);
        }
        double temps_suppr = (double)(clock() - debut_suppr) / CLOCKS_PER_SEC;

        // Écriture de la ligne dans le fichier
        fprintf(fichier, "%d\t%f\t%f\t%f\n", n, temps_ins, temps_rech, temps_suppr);
        
        // Libération de la mémoire
        free(donnees);
        tree_delete(arbre, free);
    }

    fclose(fichier);
    printf("Les mesures ont été sauvegardées.\n");
}

int main() {
    Tree arbre = tree_new();
    
    int val1 = 10, val2 = 20, val3 = 5, val4 = 15;

    printf("Début des tests\n");

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

    generer_mesures_arbre_rouge_noir();
    return 0;
}