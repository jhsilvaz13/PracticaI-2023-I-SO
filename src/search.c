#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "search.h"

#define NUM_ROWS 2000         //Constante para no tener que leer el archivo dos veces (funciona perfecto sólo con 2000)
#define NUM_ROWS_ORG 8543142  //Numero de filas original

// Estructura para representar la información a buscar

typedef struct {
    int* origen;
    int* destino;
    float* tiempo;
}Viaje;

// Estructura para representar la tabla hash

typedef struct{
    int size ; // Tamaño de la tabla hash
    int buckets; // Número de buckets disponibles
    int* keys ; // Array de keys

    char** values; // Array de valores 
    char** values2; // Array de valores 
    char** values3; // Array de valores 

}hash_table;


// Método para crear la función hash

hash_table *create_hash(int size) {

    hash_table *hash_table = malloc(sizeof(hash_table)); // Creación de tabla hash vacia
    
    
    if(hash_table == NULL){// Error creación hash table
        return NULL;
    }

    hash_table->size = size;
    hash_table->buckets = size;
    
    // Guardar llaves en un arreglo de tamaño de la cantidad de llaves
    
    hash_table->keys = calloc(size, sizeof(int));
    
    if(hash_table->keys == NULL){// Error creación key
        return NULL;
    }
    
    // Creación arreglo para los valores de las keys

    for(int i = 0 ; i < size ; i++){
        hash_table->keys[i] = (char *) malloc(sizeof(char));
        hash_table->keys[i] = 0 ;
    }
    
    hash_table->values = calloc(size, sizeof(char *)); //cambiar size a 2  
    hash_table->values2 = calloc(size, sizeof(char *));//---> al cambiarlo no cumplen con la condición para que impriman el tiempo promedio
    hash_table->values3 = calloc(size, sizeof(char *));
    
    return hash_table;


}

int destruirHash(hash_table **hash_table){
    int i ;
    int size ;

    // Verificar que el apuntador valido
    if (hash_table == NULL || *hash_table == NULL) {
        return 1;
    }

    // Liberar memoria asociada a los valores almacenados

    size = (*hash_table)->size;
    for(i = 0 ; i < size ; i ++){
        free((*hash_table)->values[i]); // Liberar memoria para el array de valores
    }

    // Liberar memoria asociada a los arreglos de key y values
    free((*hash_table)->values);
    free((*hash_table)->keys);

    // Liberar memoria de la estructura 
    free(*hash_table);
    return 0 ;

}

int insertar(hash_table *hash_table,int sourceid, int dstid,int hod,int media){
    
    //printf("Media -> %d\n",media);

    // Verificar que el apuntador de la tabla existe

    if(hash_table == NULL){
        perror("Hastable no existe");
        exit(-1);
    }
    
    // Verificar que hay buckets disponibles

    if(hash_table->buckets <= 0 ){
        perror("No hay más espacio en la tabla hash");
        exit(-1);
    }
    
    // Aplicar f(x) hash para el valor key

    int index ;

    index = sourceid % hash_table->size;
    

    // Si el array esta vacio se inserta ahí
    
    
    if(hash_table->keys[index] == 0){

        // Creación de arreglo para los valores
        int* arr = (int*) malloc(2 * sizeof(int));
        arr[0] = dstid;
        arr[1] = hod;

        hash_table->keys[index] = sourceid ; 
        hash_table->values[index] = arr[0] ;
        hash_table->values2[index] = arr[1] ;
        hash_table->values3[index] = media;
        hash_table->buckets --; // Se resta para saber que quedan menos buckets libres
        
    }

    
};

int count_rows(FILE *file) {// Contar filas
    int count = 0;
    char line[1024];

    while (fgets(line, 1024, file)) {
        count++;
    }

    return count;
}


char * search(int *data){
    printf("\n\nLa dirección de memoria: %p\n", data);
    printf("Los datos ingresados fueron: %d, %d, %d\n", *data, *(data+1), *(data+2));

    // Obtener el tamaño de filas del archivo

    /* Creación de la tabla hash */

    hash_table *tabla = NULL;
    tabla = create_hash(NUM_ROWS);
    
    //   Insertar valores en la tabla hash */
    
    
    FILE *file2 = fopen("datos.csv","r");

    if (!file2) { // Error abrir archivo
        printf("No se pudo abrir el archivo\n");
        exit(1);
    }
    
    char line[1024];
    while (fgets(line, 1024, file2)) {
        // Guardar en variables diferentes los datos del archivo csv

        int sourceid; 
        int dstid; 
        int hod;
        int m; 

        fscanf(file2,"%d,%d,%d,%d",&sourceid,&dstid,&hod,&m);
        //printf("*-> %d\n",m);
        insertar(tabla,sourceid,dstid,hod,m);
    }

    // Busqueda del elemento

    int index ;
    
    index = *data % tabla->size;

    if(*data ==tabla->keys[index] && *(data+1) == tabla->values[index] && *(data+2) == tabla->values2[index] ){
        
        //printf("Tiempo de viaje medio(search) -> %d \n",tabla->values3[index]);
        return tabla->values3[index];
    }else{
        //printf("NA(search)\n");
        return NULL;
    }
    

}


