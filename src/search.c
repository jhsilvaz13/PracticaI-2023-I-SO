#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "search.h"

#define TABLE_SIZE 80000 // tamaño de la tabla hash
#define SIZE 50000// número de registros en el archivo binario
#define RECORD_SIZE sizeof(travel_t) // tamaño de cada registro en el archivo binario
#define INTSIZE sizeof(int) // tamaño de un entero
#define FLOATSIZE sizeof(float) // tamaño de un flotante

// estructura para cada registro
typedef struct {
    int sourceid;
    int dstid;
    int hod;
    float mean_travel_time;
} travel_t;

// estructura para la tabla hash
typedef struct {
    int key;
    long int offset;
} hash_entry_t;

unsigned int hash(int key);
void insert_record(hash_entry_t *table, FILE *file, travel_t *data);
float find_record(hash_entry_t *table, FILE *file, int sourceid, int dstid, int hod);
float search(int *data);

// función de hash
unsigned int hash(int key) {
    unsigned int a = 2654435761;  // constante usada en la función de hash de Jenkins
    unsigned int hash = a * key;
    hash = hash ^ (hash >> 16);
    hash = hash * a;
    hash = hash ^ (hash >> 16);
    hash = hash * a;
    hash = hash ^ (hash >> 16);
    return hash % TABLE_SIZE;
}

// función para agregar un registro a la tabla hash
void insert_record(hash_entry_t *table, FILE *file, travel_t *data) {
    // calcular el índice en la tabla hash
    int key = data->sourceid;
    unsigned int index = hash(key);
    
    // buscar un espacio libre en la tabla hash
    //while (table[index].key != -1) {
        if (table[index].key == key){
            // buscar el índice del siguiente registro en el archivo binario
            long int next_offset = -1;
            next_offset = table[index].offset+RECORD_SIZE;

            // mover todos los registros después del índice del siguiente registro hacia el final del archivo
            if (next_offset != -1) {
                fseek(file, 0, SEEK_END);//desplazar al final del archivo el puntero
                long int bytes_to_move = ftell(file)-next_offset;//calcular el numero de bytes despues del puntero
                void *buffer = malloc(bytes_to_move);//crear un espacio de memoria del tamaño de los bytes a mover
                fseek(file, next_offset, SEEK_SET);//desplazar el puntero al inicio del registro a mover
                fread(buffer, bytes_to_move, 1, file);//Leer los bytes a mover
                fseek(file, next_offset+RECORD_SIZE, SEEK_SET);//desplazar el puntero al final del registro a mover
                fwrite(buffer, bytes_to_move, 1, file);//escribir los bytes a mover
                free(buffer);
            }
            // escribir el nuevo registro en el espacio vacío
            fseek(file, next_offset, SEEK_SET);//despqzar el puntero al inicio del registro a mover
            fwrite(data, sizeof(travel_t), 1, file);//escribir el registro
            table[index].offset = next_offset;//actualizar el offset del registro en la tabla hash

            for (unsigned int i = 0; i < TABLE_SIZE; i++) {//actualizar los offsets de los registros que se movieron
                    if (table[i].key != -1 && table[i].key != table[index].key){//selecccion unicamnete los registros que se movieron
                        if(table[i].offset >= next_offset)
                            table[i].offset += RECORD_SIZE;
                    }
                }
            return;
        }
        //index = (index + 1);
    //}
    // agregar la nueva entrada a la tabla hash
    table[index].key = key;//añadir un registro a la tabla hash en la posicion index
    
    // buscar un espacio libre en el archivo binario para el nuevo registro
    fseek(file, 0, SEEK_END);
    long int offset = ftell(file);
    int r=fwrite(data, sizeof(travel_t), 1, file);
    printf("Se escribió: %d %d %d %f en la pos: %ld\n",data->sourceid, data->dstid, data->hod, data->mean_travel_time,offset);
    if (r==0){
        perror("Error al escribir el registro");
        exit(EXIT_FAILURE);
    }
    table[index].offset = offset;
    return;
}

// función para buscar un registro en la tabla hash
float find_record(hash_entry_t *table, FILE *file, int sourceid, int dstid, int hod) {
    // calcular el índice en la tabla hash
    int key = sourceid;
    unsigned int index = hash(key);
    long i = table[index].offset;

    travel_t record;
    fseek(file, i, SEEK_SET);
    fread(&record, sizeof(travel_t), 1, file);

    printf("El registro encontrado en la base fue:%d %d %d %f\n",record.sourceid,record.dstid,record.hod,record.mean_travel_time,table[index].offset);
    printf("El valor hash: %d y su offset: %ld",index,i);
    while (record.sourceid == sourceid){
        fseek(file, i, SEEK_SET);
        fread(&record, sizeof(travel_t), 1, file);  
        if (record.dstid == dstid && record.hod == hod){
            return record.mean_travel_time;
        }
        i-=RECORD_SIZE;
    }
    // el registro no se encontró
    return -1;
}

float search(int *data){
    float result = -1;
    // inicializar la tabla hash
    hash_entry_t* table=(hash_entry_t*)malloc(sizeof(hash_entry_t)*TABLE_SIZE);

    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i].key = -1;
        table[i].offset = -1;
    }

    // abrir el archivo binario
    FILE *file = fopen("../data/hash_table.bin", "rb+");
        // el archivo no existe, crearlo
        file = fopen("../data/hash_table.bin", "wb+");
        if (file == NULL) {
            perror("Error al abrir el archivo binario\n");
            exit(EXIT_FAILURE);
        }
        //abrir el archivo csv
        FILE *fp = fopen("../data/bogota-cadastral-2019-3-All-HourlyAggregate.csv", "r");
        if (fp == NULL){
            perror("Error al abrir el archivo\n");
            exit(EXIT_FAILURE);
        }
        char buffer[1024];//buffer para leer cada linea del archivo csv
        int row = 0;//contador de filas
        int sourceid, dstid, hod;
        float mean_travel_time;
        while (fgets(buffer, 1024, fp)){   
            if (row == 0 ){
                row++;
                continue;
            }
            if(row >SIZE){
                //numero de registros a leer
                break;
            }
            sscanf(buffer, "%d,%d,%d,%f",&sourceid,&dstid,&hod,&mean_travel_time);//leer cada linea del archivo csv
            travel_t record;//crear un registro
            record.sourceid = sourceid;
            record.dstid = dstid;
            record.hod = hod;
            record.mean_travel_time = mean_travel_time;
            insert_record(table, file,&record);//insertar el registro en la tabla hash
            row++;
        }
        fclose(fp);
        for(int i=0; i<SIZE*RECORD_SIZE; i+=RECORD_SIZE){
            travel_t record;
            fseek(file, i, SEEK_SET);
            fread(&record, sizeof(travel_t), 1, file);
            printf("record: %d %d %d %f offset->%ld\n", record.sourceid, record.dstid, record.hod, record.mean_travel_time,i);
        }
        result=find_record(table,file,*(data),*(data+1),*(data+2));
        free(table);
        printf("Resultado: %f\n", result);
        return result;
    
    fclose(file);
}