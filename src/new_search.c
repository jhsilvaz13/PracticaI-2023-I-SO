// Implementación de una tabla hash con encadenamiento separado

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdint.h>
#include "search.h"

#define TABLE_SIZE 20000000 // Tamaño de la tabla hash



// Estructura de nodo para realizar la linkedlist en el encadenamiento separado
typedef struct node {
    int sourceid;
    int dstid;
    int hod;
    int m;
    struct node *next;
} Node;


// Estructura de la hash table
typedef struct hash_table {
    int size;
    Node** table;
} HashTable;

// Función hash de Murmur para claves de 32 bits

uint32_t murmurhash32(const void* key, size_t len, uint32_t seed) {


    const uint8_t* data = (const uint8_t*) &key;
    const int nblocks = len / 4;

    uint32_t h = seed;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    // body
    const uint32_t* blocks = (const uint32_t*) (data + nblocks * 4);

    for (int i = -nblocks; i; i++) {
        uint32_t k = blocks[i];

        k *= c1;
        k = (k << 15) | (k >> 17);
        k *= c2;

        h ^= k;
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }

    // tail
    const uint8_t* tail = (const uint8_t*) (data + nblocks * 4);
    uint32_t k = 0;

    switch (len & 3) {
        case 3:
            k ^= tail[2] << 16;
        case 2:
            k ^= tail[1] << 8;
        case 1:
            k ^= tail[0];
            k *= c1;
            k = (k << 15) | (k >> 17);
            k *= c2;
            h ^= k;
    }

    // finalization
    h ^= len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

// Función hash
int hash_function(int key,int size) {

    uint32_t hash = murmurhash32(key, sizeof(key), 0);
    return hash % size;

}

// Crear tabla hash

HashTable* create_table(int size){
    
    HashTable* table = (HashTable*) malloc(sizeof(HashTable));
    table->size = size;
    table->table = (Node**) calloc(size,sizeof(Node*));
    return table;

}

// Función para insertar los valores en la tabla hash

void insert(HashTable* ht, int sourceid, int dstid, int hod, int m) {

    int index = hash_function(sourceid,ht->size); // valor de la pos en la tabla hash
    
    Node *new_node = (Node*) malloc(sizeof(Node));
    
    // Creación del nodo que contiene los datos

    new_node->sourceid = sourceid;
    new_node->dstid = dstid;
    new_node->hod = hod;
    new_node->m = m;
    new_node->next = NULL;


    if (ht->table[index] == NULL) { // Si esta vacio 
        
        
        ht->table[index] = new_node;
    } else {

        Node *current = ht->table[index]; // Creacion de nodo con los valores del que esta en la pos de hash

        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node; // Asignación de nuevo nodo en la Linked List
    }
    
}


// Función para buscar un valor en la tabla hash

int search(HashTable* table, int key,int dstid,int hod) {
    // Obtener el índice de la tabla hash

    int index = hash_function(key, table->size);

    // Buscar el valor en la lista correspondiente en la tabla hash
    Node* current = table->table[index];
    while (current != NULL) {

        if (current->sourceid == key && current->dstid == dstid && current->hod == hod) {
            return current->m;
        }
        current = current->next;
    }

    // Si no se encuentra el valor, devolver -1
    return -1;
}

int main() {

    printf("Inicio \n");
    HashTable* ht = create_table(TABLE_SIZE); // Creación objeto Tabla Hash

    // Abrir archivo Uber
    FILE *file = fopen("datos.csv", "r");

    if (!file){
        perror("Error abrir archivo Uber \n");
        exit(-1);
    }

    // Lectura archivo
    char line[1024];
    int cont = 0;

    int sourceid;
    int dstid;
    int hod;
    int m;

    while (fgets(line, sizeof(line), file) && cont < 9000000) {
        // Guardar en variables diferentes los datos del archivo csv
        sscanf(line, "%d,%d,%d,%d", &sourceid, &dstid, &hod, &m);
        insert(ht, sourceid, dstid, hod, m);
        cont++;
    }

    

    printf("Se encontro %d \n", search(ht, 462, 1028, 1));
    printf("Fin \n");

}
