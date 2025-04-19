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


void insertMap(HashMap * map, char * key, void * value) {
    if (map->size == map->capacity){
        printf("la tabla hash esta llena\n");
        return;
    }

    if (searchMap(map, key) != NULL){
        printf("la clave ya existe\n");
        return;
    }

    
    size_t posi = hash(key, map->capacity);
    while (1){
        if(map->buckets[posi] == NULL || map->buckets[posi]->key == NULL){
            Pair * nuevoPar = createPair(key, value);
            map->buckets[posi] = nuevoPar;
            map->size++;
            map->current = posi;
            return;
            }
        else posi = (posi+1) % map->capacity;
    }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    long cap_antigua = map->capacity;
    map->capacity = map->capacity * 2;
    
    Pair ** buckets_antiguos = map->buckets;
    map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
    map->size = 0;

    
    for(size_t l = 0; l < cap_antigua; l++){
        if(buckets_antiguos[l] != NULL && buckets_antiguos[l]->key != NULL){
            insertMap(map, buckets_antiguos[l]->key, buckets_antiguos[l]->value);
        }
    }
    free(buckets_antiguos);
    return;
}


HashMap * createMap(long capacity) {
    HashMap * Mapa = (HashMap *)malloc(sizeof(HashMap));
    Mapa->buckets = (Pair **) calloc(capacity, sizeof(Pair *)); //esto deberia dejar nulas todas las casillas bucket
    Mapa->size = 0;
    Mapa->capacity = capacity;
    Mapa->current = -1;
    
    return Mapa;
}


void eraseMap(HashMap * map,  char * key) {    
    size_t posi = hash(key, map->capacity);
    if(map->buckets[posi] == NULL) return;
    while(map->buckets[posi] != NULL){
        if(is_equal(map->buckets[posi]->key, key)){
            map->buckets[posi]->key = NULL;
            map->size--;
            return;
        }
        posi = (posi+1) % map->capacity;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
    size_t posiTeo = hash(key, map->capacity);
    if(map->buckets[posiTeo] == NULL) {
        //printf("la clave no esta\n");
        return NULL;
    }
    while(map->buckets[posiTeo] != NULL){
        if(is_equal(map->buckets[posiTeo]->key, key)){
            map->current = posiTeo;
            return map->buckets[posiTeo];
        }
        posiTeo = (posiTeo+1) % map->capacity;
    }
    //printf("la clave no esta\n");
    return NULL;
}

Pair * firstMap(HashMap * map) {
    for (size_t posi = 0; posi < map->capacity; posi++) {
        if (map->buckets[posi] != NULL && map->buckets[posi]->key != NULL) {
            map->current = posi;
            return map->buckets[posi];
        }
    }
    map->current = -1;
    return NULL;
}


Pair * nextMap(HashMap * map) {
    if(map->current == -1) return NULL;
    
    for (long aux = map->current + 1; aux < map->capacity; aux++){
        if(map->buckets[aux] != NULL)
            if(map->buckets[aux]->key != NULL){
                map->current = aux;
                return map->buckets[aux];
            }
        
    }
    return NULL;
}


