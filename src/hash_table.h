#ifdef _HASH_TABLE_H_
#define _HASH_TABLE_H_

HashTable_t *crearHash(int num_elements);/*crea la tabla hash*/
int destruirHash(HashTable_t **hashTablePP);/*elimina la tabla hash de la memoria*/
int insertar(HashTable_t *hashTableP, int key, char *value); /*Inserta los valores con su llave en la tabla hash*/
int imprimir(HashTable_t *hashTableP);/*Imprime la tabla hash*/
HashTable_t *cargar_tabla_hash(char *nombre_archivo);/*lee el archivo donde se guarda la tabla hash*/
#endif