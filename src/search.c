#include <stdio.h>
#include "search.h"

void search(int *data){
    printf("\n\nLa dirección de memoria: %p\n", data);
    printf("Los datos ingresados fueron: %d, %d, %d\n", *data, *(data+1), *(data+2));
}