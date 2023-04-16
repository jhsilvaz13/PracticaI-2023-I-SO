#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "interfaz.h"
#include "search.h"

int* input();
 main(){
    key_t key=1234;//llave para la memoria compartida
    int *shm;//puntero a la memoria compartida
    int shmId;//identificador de la memoria compartida
    //crear espacio de memoria compartida
    shmId = shmget(key, 3*sizeof(float), 0666|IPC_CREAT);
    if(shmId < 0){
        perror("Error en shmget");
        exit(EXIT_FAILURE);
    }
    //asociar espacio de memoria creada a este proceso
    shm = shmat(shmId, NULL, 0);
    /*Crear dos procesos*/
    __pid_t pid=fork();
    
    if(pid==-1){
        perror("Error en la creación de procesos");
        exit(EXIT_FAILURE);
    }
    else if(pid==0){
    
        input(shm);
    }else{
        /*Padre*/
        while (wait(NULL)!=-1){/*Esperar hasta que el proceso hijo termine su ejecución*/
            
            search(shm);
        }
    }
    return 0;
}