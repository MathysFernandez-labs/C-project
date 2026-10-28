#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#include "tree.h"

// ============================================================
// Type Dictionnaire (mot:def) pour tests génériques
// ============================================================
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

// Libère les chaînes d'un Couple stocké dans un noeud (appelé par tree_delete)
void delete_couple(void *data) {
    Couple *c = (Couple *)data;
    free(c->mot);
    free(c->def);
}

void monPrintF (void * a, void * b){
    printf("Valeur du noeud : %d\n", *(int*)a);
}

void printCouple(void *a, void *b) {
    Couple *c = (Couple *)a;
    printf("%s : %s\n", c->mot, c->def);
}

// Fonction de comparaison pour les entiers
int compare_int(const void *a, const void *b) {
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

/**
 * Test réalisés pour les arbres binaires (entiers)
 */
void testArbresBinaires(void){
    int i = 5, j = 10, k = 15, m = 20;
    size_t sizeInt = sizeof(int);

    Tree racine = tree_create(&i, sizeInt);
    Tree fils1 = tree_create(&j, sizeInt);
    Tree fils2 = tree_create(&k, sizeInt);

    tree_set_left(racine, fils1);
    tree_set_right(racine, fils2);

    Tree fils1fils1 = tree_create(&m, sizeInt);
    Tree fils2fils1 = tree_create(&i, sizeInt);

    tree_set_left(fils1, fils1fils1);
    tree_set_right(fils1, fils2fils1);

    printf("Tri pre-order : \n");
    tree_pre_order(racine, monPrintF, NULL);

    printf("\nTri post-order : \n");
    tree_post_order(racine, monPrintF, NULL);

    printf("\nTri in-order : \n");
    tree_in_order(racine, monPrintF, NULL);
    
    // Les données sont sur le stack (variables locales i, j, k, m).
    // Pas besoin d'appeler free donc pointeur nul.
    tree_delete(racine, 0);
}

/**
 * Test avec le type dictionnaire (Couple mot:def)
 * Démontre la généricité de l'arbre BST
 */
void testArbresDictionnaire(void) {
    printf("\n========== Test BST avec Dictionnaire (mot:def) ==========\n");

    Tree arbre = tree_new();

    Couple c1 = { .mot = "chat",    .def = "animal domestique" };
    Couple c2 = { .mot = "arbre",   .def = "plante avec un tronc" };
    Couple c3 = { .mot = "maison",  .def = "lieu d'habitation" };
    Couple c4 = { .mot = "ballon",  .def = "objet spherique" };

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

    // Pas de free sur les Couples car les chaînes sont sur le stack (littéraux)
    tree_delete(arbre, NULL);
    printf("Test dictionnaire BST réussi !\n");
}

// ============================================================
// Benchmark - BST (entiers)
// ============================================================
void generer_mesures_bst() {
    FILE *fichier = fopen("mesures_bst.txt", "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fprintf(fichier, "N\tInsertion\tRecherche\tSuppression\n");

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

        // Suppression de l'arbre entier (pas de tree_remove unitaire dans le BST simple)
        clock_t debut_suppr = clock();
        tree_delete(arbre, NULL);
        double temps_suppr = (double)(clock() - debut_suppr) / CLOCKS_PER_SEC;

        fprintf(fichier, "%d\t%f\t%f\t%f\n", n, temps_ins, temps_rech, temps_suppr);

        free(donnees);
    }

    fclose(fichier);
    printf("Les mesures BST (entiers) ont été sauvegardées dans mesures_bst.txt\n");
}

// ============================================================
// Benchmark - BST (dictionnaire mot:def)
// ============================================================
void generer_mesures_bst_dict() {
    FILE *fichier = fopen("mesures_bst_dict.txt", "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fprintf(fichier, "N\tInsertion\tRecherche\tSuppression\n");

    for (int n = 100; n <= 10000; n += 100) {
        Tree arbre = tree_new();

        // Génération de n couples (mot:def) aléatoires
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
        // On passe delete_couple pour libérer les chaînes copiées dans les noeuds
        clock_t debut_suppr = clock();
        tree_delete(arbre, delete_couple);
        double temps_suppr = (double)(clock() - debut_suppr) / CLOCKS_PER_SEC;

        fprintf(fichier, "%d\t%f\t%f\t%f\n", n, temps_ins, temps_rech, temps_suppr);

        // Libérer les chaînes originales (les copies dans l'arbre sont déjà libérées)
        for (int i = 0; i < n; i++) {
            free(donnees[i].mot);
            free(donnees[i].def);
        }
        free(donnees);
    }

    fclose(fichier);
    printf("Les mesures BST (dictionnaire) ont été sauvegardées dans mesures_bst_dict.txt\n");
}


int main(){

    testArbresBinaires();
    testArbresDictionnaire();

    generer_mesures_bst();
    generer_mesures_bst_dict();

    return EXIT_SUCCESS;
}
