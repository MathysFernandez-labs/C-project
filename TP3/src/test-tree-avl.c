#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#include "tree-avl.h"

typedef struct {
    char *mot;
    char *def;
} Couple;

// Fonction de comparaison pour Couple (clé = mot)
int compare_couple(const void *a, const void *b) {
    int cmp = strcmp(((const Couple *)a)->mot, ((const Couple *)b)->mot);
    if (cmp < 0) return -1;
    if (cmp > 0) return 1;
    return 0;
}

// Génère un mot aléatoire de longueur donnée
char* generer_mot_aleatoire(int longueur) {
    char *mot = malloc(longueur + 1);
    for (int i = 0; i < longueur; i++) {
        mot[i] = 'a' + (rand() % 26);
    }
    mot[longueur] = '\0';
    return mot;
}

// Libère les chaînes d'un Couple
void delete_couple(void *data) {
    Couple *c = (Couple *)data;
    free(c->mot);
    free(c->def);
}

void monPrintF (void * a, int balance, void * b){
    printf("Valeur du noeud : %d (balance: %d)\n", *(int*)a, balance);
}

void printCouple(void *a, int balance, void *b) {
    Couple *c = (Couple *)a;
    printf("%s : %s (balance: %d)\n", c->mot, c->def, balance);
}

// Fonction de comparaison pour les entiers
int compare_int(const void *a, const void *b) {
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}



// Test réalisés pour les arbres AVL

void testArbresAVL(void){
    int i = 5, j = 10, k = 15, m = 20;
    size_t sizeInt = sizeof(int);

    Tree racine = tree_new();

    tree_insert_sorted(&racine, &i, sizeInt, compare_int);
    tree_insert_sorted(&racine, &j, sizeInt, compare_int);
    tree_insert_sorted(&racine, &k, sizeInt, compare_int);
    tree_insert_sorted(&racine, &m, sizeInt, compare_int);

    printf("Tri pre-order : \n");
    tree_pre_order(racine, monPrintF, NULL);

    printf("\nTri post-order : \n");
    tree_post_order(racine, monPrintF, NULL);

    printf("\nTri in-order : \n");
    tree_in_order(racine, monPrintF, NULL);
    
    tree_delete(racine, NULL);
}

void testArbresDictionnaire(void) {
    Tree arbre = tree_new();

    Couple c1 = { .mot = "chat",    .def = "animal domestique" };
    Couple c2 = { .mot = "arbre",   .def = "plante avec un tronc" };
    Couple c3 = { .mot = "maison",  .def = "lieu d'habitation" };
    Couple c4 = { .mot = "ballon",  .def = "objet spherique" };

    // ! //
    // Fonctionne pas
    tree_insert_sorted(&arbre, &c1, sizeof(Couple), compare_couple);
    tree_insert_sorted(&arbre, &c2, sizeof(Couple), compare_couple);
    tree_insert_sorted(&arbre, &c3, sizeof(Couple), compare_couple);
    tree_insert_sorted(&arbre, &c4, sizeof(Couple), compare_couple);

    printf("Dictionnaire trié (in-order) :\n");
    tree_in_order(arbre, printCouple, NULL);

    // Recherche d'un mot existant
    Couple recherche = { .mot = "chat", .def = NULL };
    Couple *resultat = (Couple *)tree_search(arbre, &recherche, compare_couple);
    assert(resultat != NULL);
    printf("\nRecherche 'chat' => %s\n", resultat->def);

    // Recherche d'un mot inexistant
    Couple recherche2 = { .mot = "voiture", .def = NULL };
    assert(tree_search(arbre, &recherche2, compare_couple) == NULL);
    printf("Recherche 'voiture' => non trouvé (OK)\n");

    tree_delete(arbre, NULL);
    printf("Test dictionnaire AVL ok\n");
    return;
}


void generer_mesures_avl() {
    FILE *fichier = fopen("mesures_avl.txt", "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fprintf(fichier, "N\tInsertion\tRecherche\tSuppression\n");

    for (int n = 100; n <= 10000; n += 100) {
        /*
        if (n%500 == 0) {
            printf("test dans boucle n : %d \n", n);
        }
        */
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

        // Suppression de l'arbre entier
        clock_t debut_suppr = clock();
        tree_delete(arbre, NULL);
        double temps_suppr = (double)(clock() - debut_suppr) / CLOCKS_PER_SEC;

        fprintf(fichier, "%d\t%f\t%f\t%f\n", n, temps_ins, temps_rech, temps_suppr);

        free(donnees);
    }

    fclose(fichier);
    printf("Les mesures AVL ont été sauvegardées\n");
}


void generer_mesures_avl_dict() {
    FILE *fichier = fopen("mesures_avl_dict.txt", "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fprintf(fichier, "N\tInsertion\tRecherche\tSuppression\n");

    for (int n = 100; n <= 10000; n += 100) {
        /*
        if (n%500 == 0) {
            printf("test dans boucle n : %d \n", n);
        }
        */
        Tree arbre = tree_new();

        Couple *donnees = malloc(n * sizeof(Couple));
        for (int i = 0; i < n; i++) {
            donnees[i].mot = generer_mot_aleatoire(5 + rand() % 10);
            donnees[i].def = generer_mot_aleatoire(10 + rand() % 20);
        }

        // Chronométrage de l'insertion
        clock_t debut_ins = clock();
        for (int i = 0; i < n; i++) {
            tree_insert_sorted(&arbre, &donnees[i], sizeof(Couple), compare_couple);
        }
        double temps_ins = (double)(clock() - debut_ins) / CLOCKS_PER_SEC;

        // Chronométrage de la recherche
        clock_t debut_rech = clock();
        for (int i = 0; i < n; i++) {
            tree_search(arbre, &donnees[i], compare_couple);
        }
        double temps_rech = (double)(clock() - debut_rech) / CLOCKS_PER_SEC;

        // Suppression de l'arbre entier
        clock_t debut_suppr = clock();
        tree_delete(arbre, delete_couple);
        double temps_suppr = (double)(clock() - debut_suppr) / CLOCKS_PER_SEC;

        fprintf(fichier, "%d\t%f\t%f\t%f\n", n, temps_ins, temps_rech, temps_suppr);

        free(donnees);
    }

    fclose(fichier);
    printf("Les mesures AVL ont été sauvegardées\n");
}

int main(){

    testArbresAVL();
    testArbresDictionnaire();

    generer_mesures_avl();
    generer_mesures_avl_dict();

    return EXIT_SUCCESS;
}
