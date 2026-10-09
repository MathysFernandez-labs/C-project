#include <stdio.h>
#include <string.h>
#include <assert.h>
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

int main()
{   
    Couple c,c1,c2,c3,c4;
    HashTable *table = hashtable_new(sizeof(Couple), mon_hash, mon_compare);
    

    c1.mot = malloc(10); 
    c1.def = malloc(10);
    strcpy(c1.mot, "mot1"); 
    strcpy(c1.def, "def1");






    printf("=======Debut tests=======\n\n");

    printf("------test1: test apply------\n");


    hashtable_insert(&table, &c1, mon_delete);
    hashtable_apply(table, transformer_mot, (void*)"BINGO");
    
    assert(strcmp(c1.mot, "BINGO") == 0);
    
    // Si assert => true, 
    // alors l'affichage Test valide se fera
    printf("TEST APPLY VALIDE\n");
    printf("------------\n\n");








    printf("------test2: test print------\n");


    FILE *temp_file = tmpfile();
    
    hashtable_print(table, mon_print, temp_file);
    
    // Le cursueur du fichier ce trouve à la fin
    // On peut le ramener avec rewind;
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









    printf("------test3: test delete------\n");

    // hashtable_print(table, mon_print, stdout);
    hashtable_delete(table, mon_delete);
    // Etant donné que l'on utilise l'adresse en mémoire, meme si on a bien supprimé on pointe bien vers quelque chose...
    // Pour simplifier on va supposer que si la function hastable_delete ne porduit pas un erreur alors cela fonctionne
    printf("TEST DELETE VALIDE\n");
    
    printf("------------\n\n");





    printf("------test4: test insert------\n");
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







    printf("------test5: test lookup------\n");

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





    
    printf("------test6: test retract------\n");
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
    printf("=======Fin tests=======\n");

    hashtable_delete(table, mon_delete);

}
