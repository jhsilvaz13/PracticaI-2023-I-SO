//Fields:
//sourceid,dstid,hod,mean_travel_time,standard_deviation_travel_time,geometric_mean_travel_time,geometric_standard_deviation_travel_time

#include <stdio.h>
#include <string.h>

struct elemento {
    int clave;
    int valor;
};

struct tabla_hash {
    struct elemento *elementos;
};


int createHT() {
    FILE *datosViajes = fopen("../bogota-cadastral-2019-3-All-HourlyAggregate.csv", "r");
    FILE *HashTable = fopen("../hashTable.dat", "wb");

    if (datosViajes == NULL) {
        printf("Error al abrir el archivo csv\n");
        return 1;
    }

    if (HashTable == NULL) {
        printf("Error al abrir el archivo hash table\n");
        return 1;
    }

    char linea[100];
    while (fgets(linea, sizeof(linea), datosViajes)) {
        //Lectura del archivo csv y creación de la tabla hash
        struct elemento elem;
        char *campo;
        campo = strtok(linea, ",");
        elem.clave = 
        while (campo != NULL) {

            printf("%s\n", campo);
            campo = strtok(NULL, ",");
        }
    }

    fclose(datosViajes);
    fclose(HashTable);
    return 0;
}

int hashFunc(int key){

}
