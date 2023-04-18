#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "search.h"
#include "hash_table.h"

#define MAX_LONGITUD_VALOR 200


// Estructura para representar la información a buscar

typedef struct datos{
    int* origen;
    int* destino;
    float* tiempo;
}Viaje;

/* Esta es la estructura que representa 
 * los valores a guardar por llave de 
 * la hash table */
typedef struct value {
    int dsrID;
    int hod;
    float mean_travel_time;
}StrValue;

/* Esta es la estructura que representa 
 * cada entrada de la hash table */
typedef struct entrada {
    int key;
    StrValue value;
    struct entrada *siguiente;
}StrEntrada;

/* Esta es nuestra estructura que representa 
 * a una hash table */
typedef struct HashTable {
  int num_elements;       /* Número de elementos que podemos almacenar */
  int buckets_vacios;     /* Número de buckets vacíos */
  //int* keys;              /* Aca almacenamos las llaves */
  //char** values;          /* Aca almacenamos los valores */
  StrEntrada **entradas;
} HashTable_t;

void search(int *data){
    printf("\n\nLa dirección de memoria: %p\n", data);
    printf("Los datos ingresados fueron: %d, %d, %d\n", *data, *(data+1), *(data+2));

    HashTable_t tabla_hash = cargar_tabla_hash("tabla_hash.dat");

    if (tabla_hash == NULL) { // Error al obtener la tabla hash
        printf("No se pudo obtener la tabla hash\n");
        exit(1);
    }

    //char line[1024];

    int index = *data % tabla_hash->num_elements;
    StrEntrada *entrada_actual = tabla_hash->entradas[index];
    StrValue *val = entrada_actual->value;
    if(val->dsrID == *(data+1)) {
        printf("Se encontró el origen %d con el destino %d, se tiene guardado en la hora del dia lo sig. %d\n", *data, *(data+1),val->hod);
    }   
//         // Guardar en variables diferentes los datos del archivo csv

//         int sourced; 
//         int dstid; 
//         int hod;

//         fscanf(file2,"%d,%d,%d",&sourceid,&dstid,&hod);
//         fprintf(file2, "%s", "hola");
    
//         insertar(tabla,sourceid,dstid,hod);    
}