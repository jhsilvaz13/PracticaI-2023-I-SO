#include <stdio.h>
#include "interfaz.h" 

int home(void){
    int OPTION;
    printf("Bienvenido\n"
            "1. Ingresar origen\n"
            "2. Ingresar destino\n"
            "3. Ingresar hora\n"
            "4. Buscar tiempo de viaje medio\n"
            "5. Salir\n");
    scanf("%d", &OPTION);
    error(OPTION);    
    printf("Selected option: %d", OPTION);
    
    return 0;
};

void error(int number){
    if(number == 0 || number > 5){
        perror("Error, debe ingresar una de las opciones disponibles");
        exit(-1);
    }
};
