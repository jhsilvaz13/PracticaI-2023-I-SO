#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 2500 // tamaño de la tabla hash
#define SIZE 9000000 // número de registros en el archivo binario
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
// Node

struct node {
    int data;
    struct node* next;
};

// estructura para la tabla hash
typedef struct {
    int key;
    long int offset;
} hash_entry_t;

unsigned int hash(int key);
void insert_record(hash_entry_t *table, FILE *file, travel_t *data);
void find_record(hash_entry_t *table, FILE *file, int sourceid, int dstid, int hod);
void search();
void print_result(travel_t *result);

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

void write_list_to_file(struct node* head, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Error opening file for writing\n");
        return;
    }

    struct node* current = head;
    while (current != NULL) {
        fwrite(current, sizeof(struct node), 1, fp);
        current = current->next;
    }

    fclose(fp);
}

// función para agregar un registro a la tabla hash
void insert_record(hash_entry_t *table, FILE *file, travel_t *data) {
    // calcular el índice en la tabla hash
    int key = data->sourceid;
    unsigned int index = hash(key);
    
    // buscar un espacio libre en la tabla hash
    
    while (table[index].key != -1) {
        // Creacion del nodo

        struct node* next = malloc(sizeof(struct node));
        next->next = table[key].key;
        next->data = data;
        fwrite(next,sizeof(struct node ),1,file);
        return;

        // creacion del nodo siguiente
        //struct node*  = malloc(sizeof(struct node));
        /*
        
        if (table[index].key == key){
            printf("COLISION EN LA HASH");
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
            return;
        }
        index = (index + 1)}
        */

    }
    // agregar la nueva entrada a la tabla hash
    table[index].key = key;//añadir un registro a la tabla hash en la posicion index
    
    // buscar un espacio libre en el archivo binario para el nuevo registro
    fseek(file, 0, SEEK_END);
    long int offset = ftell(file);
    //printf("key: %d,hash=%d,offset=%d\n",key,index,offset);
    int r=fwrite(data, sizeof(travel_t), 1, file);
    if (r==0){
        perror("Error al escribir el registro");
        exit(EXIT_FAILURE);
    }
    table[index].offset = offset;
}


// función para buscar un registro en la tabla hash
void find_record(hash_entry_t *table, FILE *file, int sourceid, int dstid, int hod) {
    // calcular el índice en la tabla hash
    
    //printf("La direccion del archivo es: %p\n", file);
    
    int key = sourceid;
    unsigned int index = hash(key);
    long i = table[index].offset;
    
    //printf("El offset del registro es: %ld\n", i);
    
    travel_t record;
    fseek(file, i, SEEK_SET);
    fread(&record, sizeof(travel_t), 1, file);

    //printf("Registro encontrado en primero: %d %d %d %f\n", record.sourceid, record.dstid, record.hod, record.mean_travel_time);
    while(record.sourceid == sourceid){
        if (record.dstid == dstid && record.hod == hod){
            printf("Registro encontrado en: %d %d %d %f\n", record.sourceid, record.dstid, record.hod, record.mean_travel_time);

        }else{
            struct node* current = head;
            while()
        }
        

    }
    /*
    
    while (record.sourceid == sourceid){
        printf("Registro encontrado en: %d %d %d %f\n", record.sourceid, record.dstid, record.hod, record.mean_travel_time);
        fseek(file, i, SEEK_SET);
        fread(&record, sizeof(travel_t), 1, file);  
        if (record.dstid == dstid && record.hod == hod){
            printf("Registro encontrado: %d %d %d %f\n", record.sourceid, record.dstid, record.hod, record.mean_travel_time);
            return;
        }
        i-=RECORD_SIZE;
    }
    */
    // el registro no se encontró
    printf("Registro no encontrado\n");
}

void search(){
    // inicializar la tabla hash
    hash_entry_t table[TABLE_SIZE];

    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i].key = -1;
        table[i].offset = -1;
    }
    
     // abrir el archivo binario en modo de lectura y escritura
    FILE *file = fopen("datat.bin", "w+");
    if(file == NULL){
        printf("Error al abrir el archivo\n");
        exit(EXIT_FAILURE);
    }
    
    FILE *fp = fopen("../src/datos.csv", "r");
    if (fp == NULL){
        printf("Error al abrir el archivo\n");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    int row = 0;
    int sourceid, dstid, hod;
    float mean_travel_time;
    while (fgets(buffer, 1024, fp)){   
        if (row == 0 ){
            row++;
            continue;
        }
        if(row >=SIZE){
            break;
        }
        sscanf(buffer, "%d,%d,%d,%f",&sourceid,&dstid,&hod,&mean_travel_time);
        //printf("%d,%d,%d,%f\n",sourceid,dstid,hod,mean_travel_time);
        travel_t record;
        record.sourceid = sourceid;
        record.dstid = dstid;   
        record.hod = hod;
        record.mean_travel_time = mean_travel_time;
        insert_record(table, file,&record);
        row++;
    }

    find_record(table,file,106,996,22);
    fclose(file);
    fclose(fp);
}

int main() {
    search();
    return 0;
}