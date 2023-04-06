#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "interfaz.h"
#include "search.h"
int main(){
    
    int* data;
    /*Crear una tubería que se encargue de la comunicación entre la interfaz y la busqueda o back*/
    int pipefd[2];
    int r=pipe(pipefd);
    if(r==-1){
        perror("Error en la creación de tuberías");
        exit(EXIT_FAILURE);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     

    }
    
    /*Crear dos procesos*/
    __pid_t pid=fork();

    if(pid==-1){
        perror("Error en la creación de procesos");
        exit(EXIT_FAILURE);
    }   
    else if(pid==0){
        /*Hijo*/
        int* aux;
        aux=home();
        /*Escritura de datos con pipe*/
        close(pipefd[0]);
        r=write(pipefd[1],aux,sizeof(int*));
        if(r==-1){
            perror("Error en la escritura de datos con pipe");
        }
        close(pipefd[1]);
        exit(EXIT_SUCCESS);
    }else{
        /*Padre*/
        while (wait(NULL)!=-1){/*Esperar hasta que el proceso hijo termine su ejecución*/
            /*Lectura de datos con pipe*/
            close(pipefd[1]);
            r=read(pipefd[  0],data,sizeof(int*));
            if(r==-1){
                perror("Error en la lectura de datos con pipe");
            }
            close(pipefd[0]);
            prueba(data);
        }
    }
    return 0;
}