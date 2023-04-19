#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "search.h"

#define TABLE_SIZE 800000

// Estructura para representar la información a buscar

typedef struct {
    int sourceid;
    int dstid;
    int hod;
    float mean_travel_time;
}travel;

typedef struct{
    travel t;
    struct node *next;
}node;


// Insertar un valor en la tabla hash
void insert(node* hashTable[], travel t) {
    // Obtener el índice en la tabla hash
    int index = hash(t.sourceid);

    // Crear un nuevo nodo para el valor a insertar
    node* newNode = (node*)malloc(sizeof(node));
    
    newNode->t = t;
    newNode->next = NULL;

    // Si la posición en la tabla hash está vacía, insertar el nuevo nodo
    if (hashTable[index] == NULL) {
        hashTable[index] = newNode;
    }
    // Si la posición ya contiene elementos, agregar el nuevo nodo al final de la lista
    else {
        printf("Colisión en la posición %d\n", index);
        node* current = hashTable[index];
        while (current->next != NULL) {
            current = current->next;

        }
        current->next = newNode;
    }
}

// Buscar un valor en la tabla hash
float search_v(node* table[], int sourceid) {
    // Obtener el índice en la tabla hash
    int index = hash(sourceid);
    printf("LA llave es: %d\n", index);
    // Recorrer la lista en la posición correspondiente hasta encontrar el valor o llegar al final de la lista
    node* current = table[index];
    
    while (current != NULL) {
        travel t = current->t;
        if (t.sourceid == sourceid){
            printf("El valor encontrado es: %f\n", t.mean_travel_time);
            return t.mean_travel_time;
        }
        current = current->next;
    }

    // Si no se encontró el valor, retornar -1
    return -1;
}


int hash(int key) {
    int index = (key % 800000) + 1; // Convertir la llave en un índice en el rango de 1 a 1162
    return index;
}
/*void search(int *data){
    printf("\n\nLa dirección de memoria: %p\n", data);
    printf("Los datos ingresados fueron: %d, %d, %d\n", *data, *(data+1), *(data+2));

    // Obtener el tamaño de filas del archivo
    
    FILE *file = fopen("datos.csv", "r");
    
    if (!file) {
        printf("No se pudo abrir el archivo\n");
        exit(1);
    }

    int num_rows = count_rows(file);

    fclose(file);

    nodo* tabla = crearTabla();

}*/

void search(){
    static node* table[TABLE_SIZE];
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = NULL;
    } 
    
    FILE* fp = fopen("../data/bogota-cadastral-2019-3-All-HourlyAggregate.csv", "r");
    if (fp == NULL) {
        printf("No se pudo abrir el archivo.\n");
        exit(EXIT_FAILURE);
    }
    char buffer[1024];
    // Leer los valores del archivo y agregarlos a la tabla hash
    int sourceid, dstid, hod;
    float mean_travel_time;
    int row = 0;

    while (fgets(buffer, 1024, fp)){   
        if (row == 0){
            row++;
            continue;
        }
        sscanf(buffer, "%d,%d,%d,%f",&sourceid,&dstid,&hod,&mean_travel_time);
        travel current;
        current.sourceid = sourceid;
        current.dstid = dstid;
        current.hod = hod;
        current.mean_travel_time = mean_travel_time;
        insert(table, current);
        row++;
    }
    
        
    // Cerrar el archivo
    fclose(fp);

    printf("El valor encontrado es: %f\n", search_v(table, 462));
}

int main(int argc, char *argv[]) {
    search();
    return 0;
}

/*
Cosas por hacer:
1. Los datos deben quedar almacenados en un archivo binario, generar la tabla hash en memoria solo en la primera compilacion
2. Comunicar el resultado de busqueda hacia el proceso hijo

*/
