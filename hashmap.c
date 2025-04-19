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
        printf("la tabla hash esta llena");
        return;
    }

    if (searchMap(map, key) != NULL){
        printf("la clave ya existe");
        return;
    }
    
    size_t posi = hash(key, map->capacity);
    while (map->buckets[posi] != NULL){
        posi++;
    }

    
    if (posi == map->capacity){
        posi = 0; //por si el bucket libre estaba más atras
        while (map->buckets[posi] != NULL){
            posi++;
        }
    }

    Pair * nuevoPar = createPair(key, value);
    map->buckets[posi] = nuevoPar;
    map->size++;
    map->current = posi;
    return;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
    HashMap * Mapa = (HashMap *)malloc(sizeof(HashMap));
    Mapa->buckets = (Pair **)malloc(sizeof(Pair *) * capacity); //esto deberia dejar nulas todas las casillas bucket
    Mapa->size = 0;
    Mapa->capacity = capacity;
    Mapa->current = -1;
    
    return Mapa;
}

void eraseMap(HashMap * map,  char * key) {    


}

Pair * searchMap(HashMap * map,  char * key) {   
    size_t posiTeo = hash(key, map->capacity);
    while(map->buckets[posiTeo] != NULL){
        if(is_equal(map->buckets[posiTeo]->key, key)){
            map->current = posiTeo;
            return map->buckets[posiTeo];
        }
        if(posiTeo == map->capacity) posiTeo = 0;
        else posiTeo++;
    }
    printf("la clave no esta");
    return NULL;
}

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
