#include <stdio.h>
#include "interfaz.h" 

int* home(void){
    printf("Bienvenido\n"
            "1. Ingresar origen\n"
            "2. Ingresar destino\n"
            "3. Ingresar hora\n"
            "4. Buscar tiempo de viaje medio\n"
            "5. Salir\n");
    return input();
};

int* input(){

    int *arrayData=malloc(3*sizeof(int));/*/Arreglo de tres posiciones*/
    
    if(arrayData==NULL){
        perror("Error en la asignación de memoria");
    }
    
    printf("Ingrese ID del origen: ");
    scanf("%d", arrayData+0);
    errors((arrayData+0), 1, 1160);

    printf("Ingrese ID del destino: ");
    scanf("%d", arrayData+1);    
    errors((arrayData+1), 1, 1160);

    printf("Ingrese hora: ");
    scanf("%d", arrayData+2);
    errors((arrayData+2), 0, 23);
    
};

void errors(int *value, int left, int right){
    /**/
    while (*value<left || *value>right){
        printf("Error, ingrese un valor válido, entre %d y %d:", left, right);
        scanf("%d", value);
    }
}