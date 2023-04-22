#ifdef _SEARCH_H_
#define _SEARCH_H_

unsigned int hash(int key);//funcion de hash
void insert_record( FILE *file, travel_t *data);//funcion para agregar un registro a la tabla hash
float find_record(FILE *file, int sourceid, int dstid, int hod);//funcion para buscar un registro en la tabla hash
float search(int *data);//funcion para buscar un registro en la tabla hash

#endif