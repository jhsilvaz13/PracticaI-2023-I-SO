#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define ELEM_HASH 100

/* Esta es nuestra estructura que representa 
 * a una hash table */
typedef struct HashTable {
  int num_elements;       /* Número de elementos que podemos almacenar */
  int buckets_vacios;     /* Número de buckets vacíos */
  int* keys;              /* Aca almacenamos las llaves */
  char** values;          /* Aca almacenamos los valores */
} HashTable_t;


/* Este es el conjunto de funciones con las cuales
 * podemos manipular nuestra hash table */
HashTable_t *crearHash(int num_elements);
int destruirHash(HashTable_t **hashTablePP);
int insertar(HashTable_t *hashTableP, int key, char *value);
int imprimir(HashTable_t *hashTableP);


int main()
{
  HashTable_t *mi_tablaP = NULL;
  
  mi_tablaP = crearHash(ELEM_HASH);

  insertar(mi_tablaP, 19100, "Item a");
  insertar(mi_tablaP, 19200, "Item X"); // Este va a colisionar con el elemento anterior
  insertar(mi_tablaP, 18401, "Item b"); // Este va a colisionar con el elemento anterior
  insertar(mi_tablaP, 10002, "Item c"); // Este tambien :(
  insertar(mi_tablaP, 15554, "Item d");
  insertar(mi_tablaP, 14955, "Item e");
  insertar(mi_tablaP, 10095, "Item f");

  imprimir(mi_tablaP);

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

  // Necesitamos crear un arreglo para guardar las llaves
  temp_tablaP->keys = calloc(num_elements, sizeof(int));
  if (temp_tablaP->keys== NULL) {
    destruirHash(&temp_tablaP);
    return NULL;
  }

  // Y también un arreglo para guardar los valores
  temp_tablaP->values = calloc(num_elements, sizeof(char *));
  if (temp_tablaP->values == NULL) {
    destruirHash(&temp_tablaP);
    return NULL;
  }

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
    free((*hashTablePP)->values[i]);
  }

  // Liberar la memoria asociada a los dos arreglos
  free((*hashTablePP)->values);
  free((*hashTablePP)->keys);

  // Finalmente liberar la memoria asociada a la estructura
  free(*hashTablePP);
  return 0;
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

  // Si el bucket esta libre, insertamos nuestro valor ahi
  if (hashTableP->values[index] == NULL) {
    hashTableP->keys[index] = key;
    hashTableP->values[index] = strdup(value);
    hashTableP->buckets_vacios --;
  }

  // Si no esta libre, entonces tenemos una colisión.
  else {
    int rehash_index = index;

    // Aplicamos la función de rehash hasta encontrar un bucket
    // Estamos asumiendo que nuestra función de rehash puede mapear
    // a todas las entradas de la tabla
    for (;;) {
      rehash_index = (rehash_index+1) % hashTableP->num_elements;

      // Si encontramos un bucket vacío
      if (hashTableP->values[rehash_index] == NULL) {
        hashTableP->keys[rehash_index] = key;
        hashTableP->values[rehash_index] = strdup(value);
        hashTableP->buckets_vacios --;
        break;
      }
    }
  }

  return 0;
}

/*
 * Esta función permite imprimir el contenido de nuestra 
 * hash table
 */
int imprimir(HashTable_t *hashTableP) 
{
  int i;
  int num_elem;

  // Verificar que recibimos un apuntador valido
  if (hashTableP == NULL) {
    return 1;
  }

  num_elem = hashTableP->num_elements;

  // Iteramos sobre la tabla para imprimir su contenido
  printf("Index \t\t Key \t\t Value\n");
  for (i=0; i<num_elem; i++) {
    if (hashTableP->values[i] == NULL) {
      printf("%d \n", i);
    }
    else {
      printf("%d \t\t %d \t\t %s\n", i, hashTableP->keys[i], hashTableP->values[i]);
    }
  }

  return 0;
}