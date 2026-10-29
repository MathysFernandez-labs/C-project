#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>


#include "ntree.h"





void monPrintF (void * a, void * b){
    printf("Valeur du noeud : %d\n", *(int*)a);
}

void testArbresNAires(void){
    int i = 5, j = 10, k = 15, m = 20, n = 25, o = 30;
    size_t sizeInt = sizeof(int);

    NTree racine = ntree_create(&i, sizeInt, 5);
    NTree fils1 = ntree_create(&j, sizeInt, 2);
    NTree fils2 = ntree_create(&k, sizeInt, 2);
    NTree fils3 = ntree_create(&m, sizeInt, 2);
    NTree fils4 = ntree_create(&n, sizeInt, 2);
    NTree fils5 = ntree_create(&o, sizeInt, 2);

    // Tests d'exemples
    assert(10 == *(int*)fils1->data);
    assert(15 == *(int*)fils2->data);
    assert(20 == *(int*)fils3->data);
    assert(25 == *(int*)fils4->data);

    assert(10 == *(int*)ntree_get_data(fils1));
    assert(15 == *(int*)ntree_get_data(fils2));
    assert(20 == *(int*)ntree_get_data(fils3));
    assert(25 == *(int*)ntree_get_data(fils4));

    assert(racine->arity == 5);

    assert(NULL == racine->subtrees[0]);
    assert(NULL == racine->subtrees[1]);
    assert(NULL == racine->subtrees[2]);
    assert(NULL == racine->subtrees[3]);
    assert(NULL == racine->subtrees[4]);

    assert(ntree_set_nth(racine, fils1, 0, 0));
    assert(ntree_set_nth(racine, fils2, 1, 0));
    assert(ntree_set_nth(racine, fils3, 2, 0));
    assert(ntree_set_nth(racine, fils4, 3, 0));
    assert(ntree_set_nth(racine, fils5, 4, 0));

    assert(*(int*)racine->subtrees[0]->data == 10);
    assert(*(int*)racine->subtrees[1]->data == 15);
    assert(*(int*)racine->subtrees[2]->data == 20);
    assert(*(int*)racine->subtrees[3]->data == 25);
    assert(*(int*)racine->subtrees[4]->data == 30);

    ntree_set_data(fils1, &o ,sizeInt);
    ntree_set_data(fils2, &o ,sizeInt);
    ntree_set_data(fils3, &o ,sizeInt);
    ntree_set_data(fils4, &o ,sizeInt);

    assert(30 == *(int*)ntree_get_data(fils1));
    assert(30 == *(int*)ntree_get_data(fils2));
    assert(30 == *(int*)ntree_get_data(fils3));
    assert(30 == *(int*)ntree_get_data(fils4));

    assert(6 == ntree_size(racine));
    NTree fils1fils1 = ntree_create(&i, sizeInt, 2);
    NTree fils1fils2 = ntree_create(&j, sizeInt, 2);

    assert(ntree_set_nth(fils1, fils1fils1, 0, 0));
    assert(ntree_set_nth(fils1, fils1fils2, 1, 0));
    
    printf("%zu\n", ntree_size(racine));
    assert(8 == ntree_size(racine));


    assert(2 == ntree_height(fils1));

    assert(3 == ntree_height(racine));

    NTree fils1fils1fils1 = ntree_create(&i, sizeInt, 0);
    assert(ntree_set_nth(fils1fils1, fils1fils1fils1, 1, 0));
    assert(4 == ntree_height(racine));
    assert(3 == ntree_height(fils1));
    assert(ntree_size(racine) == 9);

    
    printf("\nTri pre-order : \n");
    ntree_pre_order(racine, monPrintF, NULL);

    printf("\nTri post-order : \n");
    ntree_post_order(racine, monPrintF, NULL);

    printf("\nTri in-order : \n");
    ntree_in_order(racine, monPrintF, NULL, 3);
    
    // Mes tests
    
    // Nouvel arbre avec la valeur 20
    NTree new5 = ntree_create(&m, sizeInt, 2);
    assert(new5);
    assert(1 == ntree_height(new5));
    assert(1 == ntree_size(new5));

    // On remplace le fils5
    assert(ntree_set_nth(racine, new5, 4, 0));
    assert(m == *(int*)ntree_get_data(new5));
    // On modifie sa valeur
    assert(ntree_set_data(new5, &o, sizeInt));
    assert(o == *(int*)ntree_get_data(new5));
    
    
    // Nouvel arbre avec la valeur 15
    NTree fils6 = ntree_create(&k, sizeInt, 3);
    assert(fils6);
    assert(1 == ntree_height(fils6));
    assert(1 == ntree_size(fils6));
    
    // Fonctionnalité "bonus", quand nth == arity, ajout du sous-arbre supplémentaire à la fin du tableau
    assert(ntree_set_nth(racine, fils6, 5, 0));
    assert(ntree_size(racine) == 10);

    // Les données sont sur le stack (variables locales i, j, k, m, n, o).
    // Pas besoin d'appeler free donc pointeur nul.
    ntree_delete(racine, 0);
}



int main(){

    testArbresNAires();

    return EXIT_SUCCESS;
}
