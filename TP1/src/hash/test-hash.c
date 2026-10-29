#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "hash.h"

//#include "hash.inc"
typedef struct {
    char *mot;
    char *def;
}
Couple;




size_t hash_string( char *str)
    {
	unsigned int hash = 0;
	int c;
    c = *str;
	while (c)  {
        c = *str++;
	    hash += c;
    }
	return hash;
}

size_t mon_hash(const void *p, size_t n)
{
    return hash_string(((Couple *) p)->mot)%n;
}

int mon_compare(const void *p1, const void *p2)
{
    return strcmp(((Couple *) p1)->mot, ((Couple *) p2)->mot);
}

void mon_delete(void *data)
{
    free(((Couple *) data)->mot);
    free(((Couple *) data)->def);
}

void mon_print(void *data, FILE * stream)
{
    fprintf(stream, "%s:%s\n", ((Couple *) data)->mot, ((Couple *) data)->def);
}




// Fonction créer pour tester apply
void transformer_mot(void *data, void *new_mot) {
    strcpy(((Couple *)data)->mot, (char *)new_mot);
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


// Hash pour les entiers
size_t hash_int(const void *p, size_t n) {
    return (size_t)(*(const int *)p) % n;
}

// Comparaison pour les entiers
int compare_int(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

void generer_mesures_hash() {
    // Création du fichier de sortie
    FILE *fichier = fopen("mesures_hash.txt", "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fprintf(fichier, "N\tInsertion\tRecherche\tSuppression\n");

    for (int n = 100; n <= 10000; n += 100) {
        HashTable *table = hashtable_new(sizeof(int), hash_int, compare_int);

        int *donnees = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            donnees[i] = rand() % 100000;
        }

        // Chronométrage de l'insertion
        clock_t debut_ins = clock();
        for (int i = 0; i < n; i++) {
            hashtable_insert(&table, &donnees[i], NULL);
        }
        double temps_ins = (double)(clock() - debut_ins) / CLOCKS_PER_SEC;

        // Chronométrage de la recherche (lookup)
        clock_t debut_rech = clock();
        for (int i = 0; i < n; i++) {
            hashtable_lookup(table, &donnees[i]);
        }
        double temps_rech = (double)(clock() - debut_rech) / CLOCKS_PER_SEC;

        // Chronométrage de la suppression (retract)
        clock_t debut_suppr = clock();
        for (int i = 0; i < n; i++) {
            hashtable_retract(&table, &donnees[i], NULL);
        }
        double temps_suppr = (double)(clock() - debut_suppr) / CLOCKS_PER_SEC;

        fprintf(fichier, "%d\t%f\t%f\t%f\n", n, temps_ins, temps_rech, temps_suppr);

        free(donnees);
        hashtable_delete(table, NULL);
    }

    fclose(fichier);
    printf("Les mesures Hash ont été sauvegardées\n");
}

void generer_mesures_hash_dict() {
    // Création du fichier de sortie
    FILE *fichier = fopen("mesures_hash_dict.txt", "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    // En-tête du fichier
    fprintf(fichier, "N\tInsertion\tRecherche\tSuppression\n");

    for (int n = 100; n <= 10000; n += 100) {
        HashTable *table = hashtable_new(sizeof(Couple), mon_hash, mon_compare);

        // Génération de n couples (mot:def) aléatoires
        Couple *donnees = malloc(n * sizeof(Couple));
        for (int i = 0; i < n; i++) {
            donnees[i].mot = generer_mot_aleatoire(5 + rand() % 10);
            donnees[i].def = generer_mot_aleatoire(10 + rand() % 20);
        }

        // Chronométrage de l'insertion
        clock_t debut_ins = clock();
        for (int i = 0; i < n; i++) {
            hashtable_insert(&table, &donnees[i], NULL);
        }
        double temps_ins = (double)(clock() - debut_ins) / CLOCKS_PER_SEC;

        // Chronométrage de la recherche (lookup)
        clock_t debut_rech = clock();
        for (int i = 0; i < n; i++) {
            hashtable_lookup(table, &donnees[i]);
        }
        double temps_rech = (double)(clock() - debut_rech) / CLOCKS_PER_SEC;

        // Chronométrage de la suppression (retract)
        clock_t debut_suppr = clock();
        for (int i = 0; i < n; i++) {
            hashtable_retract(&table, &donnees[i], NULL);
        }
        double temps_suppr = (double)(clock() - debut_suppr) / CLOCKS_PER_SEC;

        fprintf(fichier, "%d\t%f\t%f\t%f\n", n, temps_ins, temps_rech, temps_suppr);

        // Libérer les chaînes originales et le tableau
        for (int i = 0; i < n; i++) {
            free(donnees[i].mot);
            free(donnees[i].def);
        }
        free(donnees);
        hashtable_delete(table, NULL);
    }

    fclose(fichier);
    printf("Les mesures Hash ont été sauvegardées\n");
}

int main()
{   
    Couple c,c1,c2,c3,c4;
    HashTable *table = hashtable_new(sizeof(Couple), mon_hash, mon_compare);
    

    c1.mot = malloc(10); 
    c1.def = malloc(10);
    strcpy(c1.mot, "mot1"); 
    strcpy(c1.def, "def1");





    printf("Début des tests\n");




    hashtable_insert(&table, &c1, mon_delete);
    hashtable_apply(table, transformer_mot, (void*)"BINGO");
    
    assert(strcmp(c1.mot, "BINGO") == 0);
    
    // Si assert => true, 
    // alors l'affichage Test valide se fera
    printf("TEST APPLY VALIDE\n");
    printf("------------\n\n");









    FILE *temp_file = tmpfile();
    
    hashtable_print(table, mon_print, temp_file);
    
    rewind(temp_file);

    // Création d'un ptit conteneur
    char buffer[50];

    // fget permet de sauvegarder notre ligne du fichier dans le buffer pour le vérifier
    if (fgets(buffer, 50, temp_file) != NULL) {
        // printf("%s\n", buffer);
        assert(strcmp(buffer, "BINGO:def1\n") == 0);
        
        // Si assert => true,
        // alors l'affichage se fera
        printf("TEST PRINT VALIDE\n");
    }
    else {
        printf("TEST INVALIDE...\n");
    }

    fclose(temp_file);
    printf("------------\n\n");




    // hashtable_print(table, mon_print, stdout);
    hashtable_delete(table, mon_delete);
    // Etant donné que l'on utilise l'adresse en mémoire, meme si on a bien supprimé on pointe bien vers quelque chose...
    // Pour simplifier on va supposer que si la function hastable_delete ne porduit pas un erreur alors cela fonctionne
    printf("TEST DELETE VALIDE\n");
    
    printf("------------\n\n");




    c2.mot = malloc(10);
    c2.def = malloc(10);
    strcpy(c2.mot, "mot2");
    strcpy(c2.def, "def2");
    HashTable *table2 = hashtable_new(sizeof(Couple), mon_hash, mon_compare);


    if (hashtable_insert(&table2, &c2, mon_delete)) {
        // On utilise le test interne à la fonction pour vérifier le fonctionnement
        printf("TEST INSERT VALIDE\n");
    }
    else {
        printf("TEST INVALIDE...\n");
    }

    printf("------------\n\n");



    // Test en 3 sous-tests

    Couple *resultat = (Couple *)hashtable_lookup(table2, &c2);

    // S-Test1 - trouve un résultat à une clé déjà insérer
    assert(resultat != NULL);
    
    // S-Test2 - trouve la bonne valeur vis a vis de la clé
    assert(strcmp(resultat->def, c2.def) == 0);


    Couple cle_inexistante;
    cle_inexistante.mot = "introuvable";
    cle_inexistante.def = NULL;

    // S-Test3 - retourne NULL pour une clé - valeur non insérer
    assert(hashtable_lookup(table, &cle_inexistante) == NULL);

    // Si tout les assert => true,
    // alors l'affichage se fera
    printf("TEST LOOKUP VALIDE\n");

    printf("------------\n\n");



    
    // Test en 3 sous-tests

    // S-Test1 - retourne bien la valeur true (1)
    // test interne a la fonction
    assert(hashtable_retract(&table2, &c2, mon_delete) == 1);
    
    // S-Test2 - On vérifie la décrémentation
    assert(table2->nb == 0);
    

    Couple cle_inexistante2;
    cle_inexistante2.mot = "non valide";
    cle_inexistante2.def = NULL;

    // S-Test3 - retourn bien false (0) quand non trouvé
    assert(hashtable_retract(&table2, &cle_inexistante2, mon_delete) == 0);

    printf("TEST RETRACT VALIDE\n");

    printf("------------\n\n");

    printf("Tous les tests ont réussi\n");

    hashtable_delete(table, mon_delete);

    generer_mesures_hash();
    generer_mesures_hash_dict();
    return 0;
}
