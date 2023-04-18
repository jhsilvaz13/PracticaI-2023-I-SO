#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "hash_table.h"

#define ELEM_HASH 2000
#define MAX_LONGITUD_VALOR 200

//sourceid,dstid,hod,mean_travel_time

/* Esta es la estructura que representa 
 * cada entrada de la hash table */
typedef struct HashElement {
    int key;
    char* value;
    struct HashElement* next; /* Puntero al siguiente elemento si hay colisiones */
}HashElement_t;

/* Esta es nuestra estructura que representa 
 * a una hash table */
typedef struct HashTable {
  int num_elements;         /* Número de elementos que podemos almacenar */
  int buckets_vacios;       /* Número de buckets vacíos */
  HashElement_t** elemento; /* Aca almacenamos los elementos de la tabla */

} HashTable_t;



/* Este es el conjunto de funciones con las cuales
 * podemos manipular nuestra hash table */
HashTable_t *crearHash(int num_elements);
int destruirHash(HashTable_t **hashTablePP);
//int insertar(HashTable_t *hashTableP, FILE *arch, int key, char *value);
int imprimir(HashTable_t *hashTableP);
HashTable_t *cargar_tabla_hash(char *nombre_archivo);
void guardar_tabla_hash(HashTable_t *tabla, char *nombre_archivo);
void search(HashTable_t *tabla_hash, int org, int dst);


int main()
{
  //HashTable_t *mi_tablaP = NULL;
  //mi_tablaP = crearHash(ELEM_HASH);
  
  FILE *datos = fopen("bogota-cadastral-2019-3-All-HourlyAggregate.csv", "r");
  FILE *tablafl = fopen("tabla_hash.dat", "rb+");

    if (!datos) {
      printf("No se pudo abrir el archivo de datos\n");
      exit(1);
    }
    if (!tablafl) {
      printf("No se pudo abrir el archivo de la tabla hash\n");
      exit(1);
    }

    char linea[100];
    while (fgets(linea, sizeof(linea), datos)) {
      //leer el archivo csv y guardar los datos relevantes en la tabla hash
      int origenID, i = 1;
      char *campo;
      char *value = "";
      campo = strtok(linea, ",");
      
      while(campo != NULL && i<=4){
        switch (i)
        {
        case 1:
          origenID = (int) strtol(campo, NULL, 10);
          break;
        case 2:
          int dsrID = (int) strtol(campo, NULL, 10);
          char *num;
          // if (asprintf(&num, "%d", dsrID) == -1) {
          //     perror("asprintf");
 //Razón del error         // } else {         //     strcat(value, num);
          //     free(num);
          // }
          // printf("%s", value);
          break;
        case 3:
          int hod = (int) strtol(campo, NULL, 10);
          
          break;
        case 4:
          float mean_travel_time = (float) strtol(campo, NULL, 10);
          gcvt (mean_travel_time, 14, num);
          strcat(value, num);
          free(num);
          
          break;  
        default:
          break;
        }
        campo = strtok(NULL, ",");
        i++;
      }
      printf("%s, valor", value);
      //insertar(mi_tablaP, tablafl, origenID, valor);
    }

  //imprimir(mi_tablaP);

  //guardar_tabla_hash(mi_tablaP, "tabla_hash.dat");

  //search(mi_tablaP, 462, 1028);

  //destruirHash(&mi_tablaP);

  fclose(datos);
  fclose(tablafl);

  return 0;
}

// /*
//  * Esta función crea una estructura de tipo 
//  * HashTable_t. Para eso necesitamos pasar
//  * el tamaño que tendrá la tabla.
//  */
// HashTable_t *crearHash(int num_elements)
// {
//   HashTable_t *temp_tablaP = NULL;

//   // Reservar espacio para la estructura
//   temp_tablaP = malloc(sizeof(HashTable_t));
//   if (temp_tablaP == NULL) {
//     return NULL;
//   }

//   // Asignamos el numero de elements que puede contener la tabla
//   // y la cantidad de buckets que están aún vacíos
//   temp_tablaP->num_elements = num_elements;
//   temp_tablaP->buckets_vacios = num_elements;
//   temp_tablaP->entradas = malloc(num_elements * sizeof(StrEntrada *));
//     for (int i = 0; i < num_elements; i++) {
//         temp_tablaP->entradas[i] = NULL;
//     }
    
//   // Al final devolvemos el apuntador a la recién creada 
//   // estructura
//   return temp_tablaP;
// }

// /* 
//  * Esta función libera la memoria asociada con
//  * una estructura hash
//  */
// int destruirHash(HashTable_t **hashTablePP)
// {
//   int i;
//   int num_elements;

//   // Verificar que el apuntador que recibimos es válido
//   if (hashTablePP == NULL || *hashTablePP == NULL) {
//     return 1;
//   }

//   // Liberar la memoria asociada a cada uno de los 
//   // valores almacenados
//   num_elements = (*hashTablePP)->num_elements;
//   for (i=0; i<num_elements; i++) {
//     //free((*hashTablePP)->values[i]);
//      StrEntrada *entrada_actual = (*hashTablePP)->entradas[i];
//         while (entrada_actual != NULL) {
//             StrEntrada *entrada_anterior = entrada_actual;
//             entrada_actual = entrada_actual->siguiente;
//             free(entrada_anterior);
//         }
//     }
//     free((*hashTablePP)->entradas);
//     free(*hashTablePP);

//   // Liberar la memoria asociada a los dos arreglos
//   // free((*hashTablePP)->values);
//   // free((*hashTablePP)->keys);

//   // Finalmente liberar la memoria asociada a la estructura
//   //free(*hashTablePP);
//   return 0;
// }

// /*
//  * Esta función crea una estructura entrada para el 
//  * nuevo par llave-valor que se quiere agragar a la 
//  * tabla hash. Necesitamos pasarle el apuntador a la 
//  * estructura, así como la llave y el valor que deseamos
//  * insertar.
//  */
// StrEntrada *crear_entrada(int key, StrValue *value) {
//     StrEntrada *entrada_nueva = malloc(sizeof(StrEntrada));
//     entrada_nueva->key = key;
//     entrada_nueva->value = value;
//     entrada_nueva->siguiente = NULL;
//     return entrada_nueva;
// }

/*
 * Esta función permite insertar un elemento en nuestra 
 * tabla hash. Necesitamos pasarle el apuntador a la 
 * estructura, así como la llave y el valor que deseamos
 * insertar.
 */
// int insertar(FILE *arch, int key, char *value)
// {
//   int index;

//   // Verificar que recibimos un apuntador valido
//   if (arch == NULL) {
//     return 1;
//   }

//   // Verificar que aún hay buckets disponibles
//   if (hashTableP->buckets_vacios <=0) {
//     return 1;
//   }

//   // Aplicar la función hash a la llave
//   index = key % hashTableP->num_elements;
//   StrEntrada *entrada_actual = hashTableP->entradas[index];
//   StrEntrada *entrada_anterior = NULL;

//   while (entrada_actual != NULL) {
//         entrada_anterior = entrada_actual;
//         entrada_actual = entrada_actual->siguiente;
//     if (entrada_actual == NULL) {
//         StrEntrada *entrada_nueva = crear_entrada(key, value);
//         if (entrada_anterior == NULL) {
//             hashTableP->entradas[index] = entrada_nueva;
//         } else {
//             entrada_anterior->siguiente = entrada_nueva;
//         }
//     }
//   }

//   // Si el bucket esta libre, insertamos nuestro valor ahi
//   // if (hashTableP->values[index] == NULL) {
//   //   hashTableP->keys[index] = key;
//   //   hashTableP->values[index] = strdup(value);
//   //   hashTableP->buckets_vacios --;
//   // }

//   // // Si no esta libre, entonces tenemos una colisión.
//   // else {
//   //   int rehash_index = index;

//   //   // Aplicamos la función de rehash hasta encontrar un bucket
//   //   // Estamos asumiendo que nuestra función de rehash puede mapear
//   //   // a todas las entradas de la tabla
//   //   for (;;) {
//   //     rehash_index = (rehash_index+1) % hashTableP->num_elements;

//   //     // Si encontramos un bucket vacío
//   //     if (hashTableP->values[rehash_index] == NULL) {
//   //       hashTableP->keys[rehash_index] = key;
//   //       hashTableP->values[rehash_index] = strdup(value);
//   //       hashTableP->buckets_vacios --;
//   //       break;
//   //     }
//   //   }
//   //}

//   return 0;
// }

// /*
//  * Esta función permite imprimir el contenido de nuestra 
//  * hash table
//  */
// int imprimir(HashTable_t *hashTableP) 
// {
//   //falta verificar el resto de coincidencias en elementos
//   int i;
//     for (i = 0; i < hashTableP->num_elements; i++) {
//         if (hashTableP->entradas[i] != NULL) {
//             printf("Indice %d:\n", i);
//             StrEntrada *actual = hashTableP->entradas[i];
//             while (actual != NULL) {
//                 //printf("\tClave: %d, Valor: %s\n", actual->key, actual->value);
//                 actual = actual->siguiente;
//             }
//         }
//     }
// //   int i;
// //   int num_elem;

// //   // Verificar que recibimos un apuntador valido
// //   if (hashTableP == NULL) {
// //     return 1;
// //   }

// //   num_elem = hashTableP->num_elements;

// //   // Iteramos sobre la tabla para imprimir su contenido
// //   printf("Index \t\t Key \t\t Value\n");
// //   for (i=0; i<num_elem; i++) {
// //     if (hashTableP->values[i] == NULL) {
// //       printf("%d \n", i);
// //     }
// //     else {
// //       printf("%d \t\t %d \t\t %s\n", i, hashTableP->keys[i], hashTableP->values[i]);
// //     }
// //   }
//   return 0;
// }

// // void guardar_tabla_hash(HashTable_t *tabla, char *nombre_archivo) {
// //     FILE *archivo = fopen(nombre_archivo, "wb");
// //     fwrite(&(tabla->num_elements), sizeof(int), 1, archivo);
// //     for (int i = 0; i < tabla->num_elements; i++) {
// //         StrEntrada *entrada_actual = tabla->entradas[i];
// //         while (entrada_actual != NULL) {
// //             fwrite(&(entrada_actual->key), sizeof(int), 1, archivo);
// //             fwrite(entrada_actual->value, sizeof(char), strlen(entrada_actual->value) + 1, archivo);
// //             entrada_actual = entrada_actual->siguiente;
// //         }
// //     }
// //     fclose(archivo);
// // }

// // HashTable_t *cargar_tabla_hash(char *nombre_archivo) {
// //     FILE *archivo = fopen(nombre_archivo, "rb");
// //     if (archivo == NULL) {
// //         exit(1);
// //     }
// //     int tamanio;
// //     fread(&tamanio, sizeof(int), 1, archivo);
// //     HashTable_t *hashTableP = crearHash(tamanio);
// //     for (int i = 0; i < tamanio; i++) {
// //         StrEntrada *entrada_actual = NULL;
// //         StrEntrada *entrada_anterior = NULL;
// //         int clave;
// //         StrValue valor;
// //         while (fread(&clave, sizeof(int), 1, archivo) == 1) {
// //             fread(valor, sizeof(char), MAX_LONGITUD_VALOR, archivo);
// //             entrada_actual = crear_entrada(clave, valor);
// //             if (entrada_anterior == NULL) {
// //                 hashTableP->entradas[i] = entrada_actual;
// //             } else {
// //                 entrada_anterior->siguiente = entrada_actual;
// //             }
// //             entrada_anterior = entrada_actual;
// //         }
// //     }
// //     fclose(archivo);
// //     return hashTableP;
// // }

// void search(HashTable_t *tabla_hash, int org, int dst){

//     int index = org % tabla_hash->num_elements;
//     StrEntrada *entrada_actual = tabla_hash->entradas[index];
//     StrValue *val = entrada_actual->value;
//     if(val->dsrID == dst) {
//         printf("Se encontró el origen %d con el destino %d, se tiene guardado en la hora del dia lo sig. %d\n", org, dst, val->hod);
//     }   
// //         // Guardar en variables diferentes los datos del archivo csv

// //         int sourced; 
// //         int dstid; 
// //         int hod;

// //         fscanf(file2,"%d,%d,%d",&sourceid,&dstid,&hod);
// //         fprintf(file2, "%s", "hola");
    
// //         insertar(tabla,sourceid,dstid,hod);    
// }