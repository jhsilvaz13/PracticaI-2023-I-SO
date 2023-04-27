#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "search.h"

#define TABLE_SIZE 8543142      // número de registros que se van a leer del archivo csv
#define RECORD_SIZE sizeof(travel_t) // tamaño de cada registro en el archivo binario
#define INTSIZE sizeof(int)          // tamaño de un entero
#define FLOATSIZE sizeof(float)      // tamaño de un flotante
#define PATH "../data/bogota-cadastral-2019-3-All-HourlyAggregate.csv"// ruta del archivo binario
// estructura para cada registro
typedef struct
{
    int sourceid;//identificador del origen
    int dstid;//identificador del destino
    int hod;//hora del dial
    float mean_travel_time;//tiempo de viaje medio
    long int next_offset;//posicion del siguiente registro en la lista enlazada en el archivo
    int first;

} travel_t;

unsigned int hash(int key);//funcion de hash
void insert_record( FILE *file, travel_t *data);//funcion para agregar un registro a la tabla hash
float find_record(FILE *file, int sourceid, int dstid, int hod);//funcion para buscar un registro en la tabla hash
float search(int *data);//funcion para buscar un registro en la tabla hash

// función de hash
unsigned int hash(int key)
{
    return key;
}

// función para agregar un registro a la tabla hash
void insert_record(FILE *file, travel_t *data)
{
    // calcular el índice en donde se va escribir el registro
    int key = data->sourceid;
    unsigned int index = hash(key);
    travel_t record;//almacenar en memoria el registro que se va a leer
    fseek(file, index*RECORD_SIZE, SEEK_SET);//poner el puntero en la posicion del registro que se va a leer
    fread(&record,sizeof(travel_t), 1, file);//leer el registro
    if(record.sourceid==data->sourceid)
    //Leemos el regitro en la posicion de index y vemos si el srcid es el mismo 
    {
        while(record.next_offset!=-1){
             //Colision en el archivo binario
            fseek(file, record.next_offset,SEEK_SET);
            fread(&record,sizeof(travel_t),1,file);
        }
            //Se agrega el nuevo registro al final del achivo binario
            fseek(file,-RECORD_SIZE,SEEK_CUR);
            long int aux=ftell(file);
            fseek(file, 0, SEEK_END);
            long int next_offset=ftell(file);
            record.next_offset=next_offset;
            fseek(file, aux, SEEK_SET);
            fwrite(&record, sizeof(travel_t), 1, file);
            data->next_offset =-1;
            data->first==0;       
            fseek(file, 0, SEEK_END);
            //se escribe el nuevo registro en la posicion actual del archivo
            fwrite(data, sizeof(travel_t), 1, file);
        //}
        return;
        
    }

    if(record.sourceid==0 && record.dstid==0 && record.hod==0 && record.mean_travel_time==0){
       fseek(file, -RECORD_SIZE, SEEK_CUR); 
    }

    //escribir el nuevo registro en la posicion index del archivo binario
    data->first=1;
    data->next_offset =-1;
    int r =fwrite(data, sizeof(travel_t), 1, file); // escribir el registro en el archivo binario en la posicion
    if (r == 0){
        perror("Error al escribir el registro");
        exit(EXIT_FAILURE);
    }
    return;
}

// función para buscar un registro en la tabla hash
float find_record(FILE *file, int sourceid, int dstid, int hod){
    int key = sourceid;
    unsigned int index = hash(key);
    travel_t record;//almacenar en memoria el registro que se va a leer
    fseek(file, index*RECORD_SIZE, SEEK_SET);//poner el puntero en la posicion del registro que se va a leer
    while (record.next_offset!=-1){
        fread(&record,sizeof(travel_t), 1, file);//leer el registro
        if(record.sourceid==sourceid && record.dstid==dstid && record.hod==hod){
            return record.mean_travel_time;
        }
        fseek(file, record.next_offset,SEEK_SET);
        printf("record: %d %d %d %f %ld %d\n", record.sourceid, record.dstid, record.hod, record.mean_travel_time, record.next_offset, record.first);
    }

    return -1;
    

}

float search(int *data)
{
    float result = -1;//variable para almacenar el resultado de la busqueda
    //Si el archivo binario no existe se crea y se inserta la tabla hash
    //este proceso puede demorarse varios minutos debido a que primero
    //escribe toda la estructura de la tabla hash en el archivo binario
    FILE *file = fopen("../data/hash_table.bin", "rb+");
    if (file == NULL)//Si el archivo no existe es necesario crear e insertar la estructura hash
    {
        printf("El archivo no existe, tomara unos minutos crearlo.\n");
        // abrir y crear el archivo binario
        FILE *file = fopen("../data/hash_table.bin", "wb+");
        if (file == NULL){
            perror("Error en la creación del archivo binario\n");
            exit(EXIT_FAILURE);
        }

        long int max=1159*RECORD_SIZE;
        travel_t a;
        a.sourceid=0;
        a.dstid=0;
        a.hod=0;
        a.mean_travel_time=0;
        a.next_offset=0;
        a.first=0;
        fseek(file, max, SEEK_SET);
        fwrite(data, sizeof(travel_t), 1, file);
        fseek(file, 0, SEEK_SET);
        // abrir el archivo csv
        FILE *fp = fopen(PATH, "r");
        if (fp == NULL){
            perror("Error al abrir el archivo csv\n");
            exit(EXIT_FAILURE);
        }
        char buffer[1024]; // buffer para leer cada linea del archivo csv
        int row = 0;// contador de filas
        int sourceid, dstid, hod; // variables para almacenar los datos de cada linea del archivo csv
        float mean_travel_time; 
        while (fgets(buffer, 1024, fp)){//mientras se pueda leer una linea del archivo csv
            if (row == 0){//si es la primera linea del archivo csv descartarla
                row++;
                continue;
            }
            if (row > TABLE_SIZE){// numero de registros a leer
                break;
            }
            sscanf(buffer, "%d,%d,%d,%f", &sourceid, &dstid, &hod, &mean_travel_time); // leer cada linea del archivo csv
            travel_t record;// crear un registro
            record.sourceid = sourceid;//asignar los valores leidos al registro
            record.dstid = dstid;//asignar los valores leidos al registro
            record.hod = hod;//asignar los valores leidos al registro
            record.mean_travel_time = mean_travel_time;//asignar los valores leidos al registro
            printf("Insertando el registro %d\n",row);
            insert_record(file, &record); // insertar el registro en la tabla hash
            row++;
        }
        fclose(fp);//cerrar el archivo csv
        /*Descomentar si desea imprimir toda la estructura del archivo binario que almacena la tabla hash */
        for (int i = 0; i < TABLE_SIZE * RECORD_SIZE*10; i += RECORD_SIZE){
            travel_t record;
            fseek(file, i, SEEK_SET);
            fread(&record, sizeof(travel_t), 1, file);
            printf("record: %d %d %d %f %ld %d offset --> %d\n", record.sourceid, record.dstid, record.hod, record.mean_travel_time, record.next_offset, record.first,i);
        }

        //Una vez escrita la tabla hash en el archivo binario se busca el registro
        printf("Buscando el registro en la tabla hash...\n");
        fseek(file, 0, SEEK_SET);//se coloca el puntero al inicio del archivo binario
        result = find_record(file, *(data), *(data + 1), *(data + 2));//se busca el registro en la tabla hash
        return result;
    }else{//el archivo binario ya existe

        fseek(file, 0, SEEK_SET);//se coloca el puntero al inicio del archivo binario
        result = find_record(file, *(data), *(data + 1), *(data + 2));//se busca el registro en la tabla hash
        return result;
    }
    fclose(file);//cerrar el archivo binario
}

