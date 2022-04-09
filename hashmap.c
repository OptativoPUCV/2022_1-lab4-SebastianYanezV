#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{
    int porcentajeDeCapacidad;

    porcentajeDeCapacidad = (map->size * 100) / map->capacity;

    if (porcentajeDeCapacidad == 70)
    {
        map->buckets = (Pair **) realloc(map->buckets, (map->capacity + 1));
        map->capacity++;
    }

    long pos = hash(key, map->capacity);
    Pair *newPair = createPair(key, value);
    
    if (map->buckets[pos] == NULL)
    {
        map->buckets[pos] = newPair;
        map->current = pos;
        map->size++;
    }
    else
    {
        while (map->buckets[pos] != NULL && map->buckets[pos]->key != NULL)
        {
            pos = (pos + 1) % map->capacity;
        }

        map->buckets[pos] = newPair;
        map->current = pos;
        map->size++;
    }
}

void enlarge(HashMap * map) 
{
    enlarge_called = 1; //no borrar (testing purposes)

    /*Pair **oldBuckets = (Pair **) malloc(map->capacity * sizeof(Pair *));
    oldBuckets = map->buckets;
    long largoOldBuckets = map->size;

    map->capacity *= 2;
    map->buckets = (Pair **) malloc(map->capacity * sizeof(Pair *));
    map->size = 0;

    for (int i = 0 ; i < largoOldBuckets ; i++)
    {
        insertMap(map, oldBuckets[i]->key, oldBuckets[i]->value);
    }*/
}

HashMap * createMap(long capacity) 
{
    HashMap *mapa = (HashMap *)malloc(sizeof(HashMap));
    mapa->buckets = (Pair **) malloc(capacity * sizeof(Pair *));
    mapa->size = 0;
    mapa->capacity = capacity;
    mapa->current = -1;

    return mapa;
}

void eraseMap(HashMap * map,  char * key) 
{    
    long pos = hash(key, map->capacity);
    if (map->buckets[pos] == NULL) return;

    if (is_equal(map->buckets[pos]->key, key) == 1)
    {
        map->buckets[pos]->key = NULL;
        map->size--;
        map->current = pos;
    }
    else
    {
        while (map->buckets[pos] != NULL && map->buckets[pos]->key != NULL)
        {
            //if (map->buckets[pos] == NULL) return;
            if (is_equal(map->buckets[pos]->key, key) == 1) break;
            pos = (pos + 1) % map->capacity;
        }

        if (map->buckets[pos] == NULL) return;
        map->buckets[pos]->key = NULL;
        map->size--;
        map->current = pos;
    }
}

Pair * searchMap(HashMap * map,  char * key) 
{   
    long pos = hash(key, map->capacity);

    if (is_equal(map->buckets[pos]->key, key) == 1)
    {
        map->current = pos;
        return map->buckets[pos];
    }
    else
    {
        while (map->buckets[pos] != NULL && map->buckets[pos]->key != NULL)
        {
            if (map->buckets[pos] == NULL) return NULL;
            if (is_equal(map->buckets[pos]->key, key) == 1) break;
            pos = (pos + 1) % map->capacity;
        }

        map->current = pos;
        return map->buckets[pos];
    }

    return NULL;
}

Pair * firstMap(HashMap * map) 
{
    long pos = 0;

    if (map->buckets[pos] != NULL) return map->buckets[pos];
    else
    {
        while (map->buckets[pos] == NULL && map->buckets[pos]->key == NULL)
        {
            pos = (pos + 1) % map->capacity;
        }
        return map->buckets[pos];
    }
}

Pair * nextMap(HashMap * map) 
{
    map->current++;
    return map->buckets[map->current];
}
