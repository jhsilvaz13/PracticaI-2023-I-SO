#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "hash_table.h"

#define ELEM_HASH 2000
#define MAX_LONGITUD_VALOR 200

/* Esta es la estructura que representa 
 * cada entrada de la hash table */
typedef struct entrada {
    int key;
    char value[MAX_LONGITUD_VALOR];
    struct entrada *siguiente;
}StrEntrada;

/* Esta es nuestra estructura que representa 
 * a una hash table */
typedef struct HashTable {
  int num_elements;       /* Número de elementos que podemos almacenar */
  int buckets_vacios;     /* Número de buckets vacíos */
  //int* keys;              /* Aca almacenamos las llaves */
  //char** values;          /* Aca almacenamos los valores */
  StrEntrada **entradas;
} HashTable_t;



/* Este es el conjunto de funciones con las cuales
 * podemos manipular nuestra hash table */
HashTable_t *crearHash(int num_elements);
int destruirHash(HashTable_t **hashTablePP);
int insertar(HashTable_t *hashTableP, int key, char *value);
int imprimir(HashTable_t *hashTableP);
HashTable_t *cargar_tabla_hash(char *nombre_archivo);
void guardar_tabla_hash(HashTable_t *tabla, char *nombre_archivo);


int main()
{
  HashTable_t *mi_tablaP = NULL;
  mi_tablaP = crearHash(ELEM_HASH);
  
  FILE *datos = fopen("bogota-cadastral-2019-3-All-HourlyAggregate.csv", "r");

    if (!datos) {
      printf("No se pudo abrir el archivo\n");
      exit(1);
    }

    char linea[100];
    while (fgets(linea, sizeof(linea), datos)) {
      //leer el archivo csv y guardar los datos relevantes en la tabla hash
      int origenID, i = 1;
      char *campo;
      char *dstID;
      char *tiempo;
      char *valor;
      campo = strtok(linea, ",");
      
      while(campo != NULL && i<=3){
        switch (i)
        {
        case 1:
          origenID = (int) strtol(campo, NULL, 10);
          break;
        case 2:
          dstID = campo;
          break;
        case 3:
          tiempo = campo;
          valor = strcat(strcat(dstID, ","), tiempo);
          break;
        default:
          break;
        }
        campo = strtok(NULL, ",");
        i++;
      }
      insertar(mi_tablaP, origenID, valor);
    }
  
  
  // insertar(mi_tablaP, 1160, "Item a");
  // insertar(mi_tablaP, 1160, "Item X"); // Este va a colisionar con el elemento anterior
  // insertar(mi_tablaP, 18401, "Item b"); // Este va a colisionar con el elemento anterior
  // insertar(mi_tablaP, 10002, "Item c"); // Este tambien :(
  // insertar(mi_tablaP, 15554, "Item d");
  // insertar(mi_tablaP, 14955, "Item e");
  // insertar(mi_tablaP, 10095, "Item f");

  imprimir(mi_tablaP);

  guardar_tabla_hash(mi_tablaP, "tabla_hash.dat");

  destruirHash(&mi_tablaP);

  return 0;
}

/*
 * Esta función crea una estructura de tipo 
 * HashTable_t. Para eso necesitamos pasar
 * el tamaño que tendrá la tabla.
 */
HashTable_t *crearHash(int num_elements)
{
  HashTable_t *temp_tablaP = NULL;

  // Reservar espacio para la estructura
  temp_tablaP = malloc(sizeof(HashTable_t));
  if (temp_tablaP == NULL) {
    return NULL;
  }

  // Asignamos el numero de elements que puede contener la tabla
  // y la cantidad de buckets que están aún vacíos
  temp_tablaP->num_elements = num_elements;
  temp_tablaP->buckets_vacios = num_elements;
  temp_tablaP->entradas = malloc(num_elements * sizeof(StrEntrada *));
    for (int i = 0; i < num_elements; i++) {
        temp_tablaP->entradas[i] = NULL;
    }

  // Necesitamos crear un arreglo para guardar las llaves
  // temp_tablaP->keys = calloc(num_elements, sizeof(int));
  // if (temp_tablaP->keys== NULL) {
  //   destruirHash(&temp_tablaP);
  //   return NULL;
  // }

  // // Y también un arreglo para guardar los valores
  // temp_tablaP->values = calloc(num_elements, sizeof(char *));
  // if (temp_tablaP->values == NULL) {
  //   destruirHash(&temp_tablaP);
  //   return NULL;
  // }

  // Al final devolvemos el apuntador a la recién creada 
  // estructura
  return temp_tablaP;
}

/* 
 * Esta función libera la memoria asociada con
 * una estructura hash
 */
int destruirHash(HashTable_t **hashTablePP)
{
  int i;
  int num_elements;

  // Verificar que el apuntador que recibimos es válido
  if (hashTablePP == NULL || *hashTablePP == NULL) {
    return 1;
  }

  // Liberar la memoria asociada a cada uno de los 
  // valores almacenados
  num_elements = (*hashTablePP)->num_elements;
  for (i=0; i<num_elements; i++) {
    //free((*hashTablePP)->values[i]);
     StrEntrada *entrada_actual = (*hashTablePP)->entradas[i];
        while (entrada_actual != NULL) {
            StrEntrada *entrada_anterior = entrada_actual;
            entrada_actual = entrada_actual->siguiente;
            free(entrada_anterior);
        }
    }
    free((*hashTablePP)->entradas);
    free(*hashTablePP);

  // Liberar la memoria asociada a los dos arreglos
  // free((*hashTablePP)->values);
  // free((*hashTablePP)->keys);

  // Finalmente liberar la memoria asociada a la estructura
  //free(*hashTablePP);
  return 0;
}

/*
 * Esta función crea una estructura entrada para el 
 * nuevo par llave-valor que se quiere agragar a la 
 * tabla hash. Necesitamos pasarle el apuntador a la 
 * estructura, así como la llave y el valor que deseamos
 * insertar.
 */
StrEntrada *crear_entrada(int key, char *value) {
    StrEntrada *entrada_nueva = malloc(sizeof(StrEntrada));
    entrada_nueva->key = key;
    strcpy(entrada_nueva->value, value);
    entrada_nueva->siguiente = NULL;
    return entrada_nueva;
}

/*
 * Esta función permite insertar un element en nuestra 
 * tabla hash. Necesitamos pasarle el apuntador a la 
 * estructura, así como la llave y el valor que deseamos
 * insertar.
 */
int insertar(HashTable_t *hashTableP, int key, char *value)
{
  int index;

  // Verificar que recibimos un apuntador valido
  if (hashTableP == NULL) {
    return 1;
  }

  // Verificar que aún hay buckets disponibles
  if (hashTableP->buckets_vacios <=0) {
    return 1;
  }

  // Aplicar la función hash a la llave
  index = key % hashTableP->num_elements;
  StrEntrada *entrada_actual = hashTableP->entradas[index];
  StrEntrada *entrada_anterior = NULL;

  while (entrada_actual != NULL && entrada_actual->key != key) {
        entrada_anterior = entrada_actual;
        entrada_actual = entrada_actual->siguiente;
    }
    if (entrada_actual == NULL) {
        StrEntrada *entrada_nueva = crear_entrada(key, value);
        if (entrada_anterior == NULL) {
            hashTableP->entradas[index] = entrada_nueva;
        } else {
            entrada_anterior->siguiente = entrada_nueva;
        }
    } else {
        strcpy(entrada_actual->value, value);
    }

  // Si el bucket esta libre, insertamos nuestro valor ahi
  // if (hashTableP->values[index] == NULL) {
  //   hashTableP->keys[index] = key;
  //   hashTableP->values[index] = strdup(value);
  //   hashTableP->buckets_vacios --;
  // }

  // // Si no esta libre, entonces tenemos una colisión.
  // else {
  //   int rehash_index = index;

  //   // Aplicamos la función de rehash hasta encontrar un bucket
  //   // Estamos asumiendo que nuestra función de rehash puede mapear
  //   // a todas las entradas de la tabla
  //   for (;;) {
  //     rehash_index = (rehash_index+1) % hashTableP->num_elements;

  //     // Si encontramos un bucket vacío
  //     if (hashTableP->values[rehash_index] == NULL) {
  //       hashTableP->keys[rehash_index] = key;
  //       hashTableP->values[rehash_index] = strdup(value);
  //       hashTableP->buckets_vacios --;
  //       break;
  //     }
  //   }
  //}

  return 0;
}

/*
 * Esta función permite imprimir el contenido de nuestra 
 * hash table
 */
int imprimir(HashTable_t *hashTableP) 
{
  //falta verificar el resto de coincidencias en elementos
  int i;
    for (i = 0; i < hashTableP->num_elements; i++) {
        if (hashTableP->entradas[i] != NULL) {
            printf("Indice %d:\n", i);
            StrEntrada *actual = hashTableP->entradas[i];
            while (actual != NULL) {
                printf("\tClave: %d, Valor: %s\n", actual->key, actual->value);
                actual = actual->siguiente;
            }
        }
    }
//   int i;
//   int num_elem;

//   // Verificar que recibimos un apuntador valido
//   if (hashTableP == NULL) {
//     return 1;
//   }

//   num_elem = hashTableP->num_elements;

//   // Iteramos sobre la tabla para imprimir su contenido
//   printf("Index \t\t Key \t\t Value\n");
//   for (i=0; i<num_elem; i++) {
//     if (hashTableP->values[i] == NULL) {
//       printf("%d \n", i);
//     }
//     else {
//       printf("%d \t\t %d \t\t %s\n", i, hashTableP->keys[i], hashTableP->values[i]);
//     }
//   }
  return 0;
}

void guardar_tabla_hash(HashTable_t *tabla, char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "wb");
    fwrite(&(tabla->num_elements), sizeof(int), 1, archivo);
    for (int i = 0; i < tabla->num_elements; i++) {
        StrEntrada *entrada_actual = tabla->entradas[i];
        while (entrada_actual != NULL) {
            fwrite(&(entrada_actual->key), sizeof(int), 1, archivo);
            fwrite(entrada_actual->value, sizeof(char), strlen(entrada_actual->value) + 1, archivo);
            entrada_actual = entrada_actual->siguiente;
        }
    }
    fclose(archivo);
}

HashTable_t *cargar_tabla_hash(char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "rb");
    if (archivo == NULL) {
        return NULL;
    }
    int tamanio;
    fread(&tamanio, sizeof(int), 1, archivo);
    HashTable_t *hashTableP = crearHash(tamanio);
    for (int i = 0; i < tamanio; i++) {
        StrEntrada *entrada_actual = NULL;
        StrEntrada *entrada_anterior = NULL;
        int clave;
        char valor[MAX_LONGITUD_VALOR];
        while (fread(&clave, sizeof(int), 1, archivo) == 1) {
            fread(valor, sizeof(char), MAX_LONGITUD_VALOR, archivo);
            entrada_actual = crear_entrada(clave, valor);
            if (entrada_anterior == NULL) {
                hashTableP->entradas[i] = entrada_actual;
            } else {
                entrada_anterior->siguiente = entrada_actual;
            }
            entrada_anterior = entrada_actual;
        }
    }
    fclose(archivo);
    return hashTableP;
}