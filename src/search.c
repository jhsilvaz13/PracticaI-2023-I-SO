#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "search.h"
#include "hashtable.h"


// Estructura para representar la información a buscar

typedef struct datos{
    int* origen;
    int* destino;
    float* tiempo;
}Viaje;

void search(int *data){
    printf("\n\nLa dirección de memoria: %p\n", data);
    printf("Los datos ingresados fueron: %d, %d, %d\n", *data, *(data+1), *(data+2));

    HashTable_t tabla_hash = cargar_tabla_hash("tabla_hash.dat");

    if (!file) { // Error abrir archivo
        printf("No se pudo abrir el archivo\n");
        exit(1);
    }

    char line[1024];
    while (fgets(line, 1024, file)) {
        // Guardar en variables diferentes los datos del archivo csv

        int sourceid; 
        int dstid; 
        int hod;

        fscanf(file2,"%d,%d,%d",&sourceid,&dstid,&hod);
        fprintf(file2, "%s", "hola");
    
        insertar(tabla,sourceid,dstid,hod);
    }    
}