#include "hash.h"
//#include "hash.inc"
#include <stdlib.h>
#include <stdbool.h>   
#include <string.h> 

const static size_t hashtable_primes[] =
{
 11,
 19,
 37,
 73,
 109,
 163,
 251,
 367,
 557,
 823,
 1237,
 1861,
 2777,
 4177,
 6247,
 9371,
 14057,
 21089,
 31627,
 47431,
 71143,
 106721,
 160073,
 240101,
 360163,
 540217,
 810343,
 1215497,
 1823231,
 2734867,
 4102283,
 6153409,
 9230113,
 13845163,
};
static const double HASHTABLE_MIN_COLLISIONS = 0.3;
static const double HASHTABLE_MAX_COLLISIONS = 3.0;

static const size_t HASHTABLE_NB_PRIMES = sizeof(hashtable_primes) / sizeof(size_t);

#define HASHTABLE_MIN_SIZE hashtable_primes[0]
#define HASHTABLE_MAX_SIZE hashtable_primes[HASHTABLE_NB_PRIMES - 1]

static HashTable * _hashtable_new(size_t length, size_t size, size_t(*hash) (const void *, size_t), int (*compare) (const void *, const void *))
{
    HashTable *table = (HashTable *) malloc(sizeof(size_t) * 3 + 2 * sizeof(void *) + sizeof(List) * length);

    if (table) {
        size_t              i;

        table->hash = hash;
        table->compare = compare;
        table->length = length;
        table->nb = 0;
        table->size = size;
        for (i = 0; i < length; i++) {
            table->list[i] = NULL;
        }
    }
    return table;

}

HashTable * hashtable_new(size_t size, size_t(*hash) (const void *, size_t), int (*compare) (const void *, const void *))
{
    return _hashtable_new(HASHTABLE_MIN_SIZE, size, hash, compare);
}

static void _hashtable_resize(HashTable ** ptable)
{
    double              ratio = (*ptable)->nb / (double)(*ptable)->length;

    if ((*ptable)->nb >= HASHTABLE_MIN_SIZE &&
        (*ptable)->nb <= HASHTABLE_MAX_SIZE &&
    (ratio > HASHTABLE_MAX_COLLISIONS || ratio < HASHTABLE_MIN_COLLISIONS)) {
        size_t              i = 1;
        HashTable          *new;

        while (hashtable_primes[i] < (*ptable)->nb)
            i++;

        new = _hashtable_new(hashtable_primes[i],
                             (*ptable)->size,
                             (*ptable)->hash,
                             (*ptable)->compare);

        if (new) {
            size_t              j;

            new->nb = (*ptable)->nb;
            for (j = 0; j < (*ptable)->length; j++) {
                List                list = (*ptable)->list[j], next;

                while (list) {
                    size_t              h = new->hash(list->data, new->length);

                    next = list->next;
                    list->next = new->list[h];
                    new->list[h] = list;
                    list = next;
                }
            }
            free(*ptable);
            *ptable = new;
        }
    }
}



















void hashtable_apply(HashTable * table, void (*func) (void *, void *), void *extra_data)
{
    // Vérif
    if(table == NULL || func == NULL) {
        return;
    }
        
    // Pour la list à chaque index, appliquer la function func
    for (size_t i = 0; i< table->length; i++) {
        // utilisation d'une fonction list pour parcourir l'ensemble de chaque list et y appliquer une function
        list_foreach(table->list[i], func, extra_data);
    }
}

void hashtable_print(HashTable * table, void (*print) (void *, FILE *), FILE * stream)
{
    // Vérif
    if (table == NULL || print == NULL) {
        return;
    }

    // Pour la list à chaque index, appliquer la function print
    for (size_t i = 0; i < table->length; i++) {
        // utilisation d'une fonction list pour parcourir l'ensemble de chaque list et y appliquer une function
        list_foreach(table->list[i], (void (*)(void *, void *))print, stream);
    }
}

void hashtable_delete(HashTable * table, void (*delete) (void *))
{
    // Vérif
    if (table == NULL || delete == NULL) {
        return;
    }

    // Parcours de l'ensemble des list de la hastable
    for (size_t i = 0; i < table-> length; i++) {
        // utilisation d'une fonction list pour supprimer une liste
        list_delete(&table->list[i], delete);
    }

    // On libère ensuite la table
    free(table);
}

int hashtable_insert(HashTable ** ptable, void *data, void (*delete) (void *))
{
    // Vérif
    if (ptable == NULL || *ptable == NULL || data == NULL) {
        return false;
    }

    HashTable *table = *ptable;

    // index dans le hashtable
    size_t index = table->hash(data, table->length);
    
    // ajout au debut de la list de la hastable à l'index 
    // Test por savoir si la fonction à fonctionné
    // Rem : Mon choix s'est tourné vers prepend au lieu de append car il est plus rapide d'introduire un élément dans une liste chainé au début que de tout parcourir pour le rajouter à la fin
    if (!list_prepend(&(table->list[index]), data, table->size)) {
        if (delete != NULL) {
            delete(data);
        }
        return false;
    }

    table->nb++;
    return true;
}

int hashtable_retract(HashTable ** ptable, void *data, void (*delete) (void *))
{
    // Vérif
    if(ptable == NULL || *ptable == NULL || data == NULL) {
        return false;
    }

    HashTable *table = *ptable;
    
    // index dans le hashtable
    size_t index = table->hash(data, table->length);
    
    // index trouvé pour une donnée dans la liste à la poisiton de l'index de la hashtable
    // Dificile à expliquer en une phrase XD
    int list_index = list_index_data(table->list[index], data, table->compare);
    
    // Utilisation d'une fonction list avec l'index de la liste à l'index de la list de la hashtable pour supprimer l'élément à la position nth
    // Test du résultat
    if (list_index != -1 && list_remove_nth(&(table->list[index]), list_index, delete)) {
        table->nb--;
        return true;
    }

    return false;
}

void * hashtable_lookup(HashTable * table, void *data)
{
    // Vérif
    if(table == NULL || data == NULL) {
        return NULL;
    }
    
    // index dans le hashtable
    size_t index = table->hash(data, table->length);

    // Utilisation d'une fonction list avec la list à l'index de la list de la hashtable
    return list_find_data(table->list[index], data, table->compare);
}
