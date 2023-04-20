#ifdef _SEARCH_H_
#define _SEARCH_H_

unsigned int hash(int key);
void insert_record(hash_entry_t *table, FILE *file, int key, travel_t *data);
void find_record(hash_entry_t *table, FILE *file, int key) ;
void search();
void print_result(travel_t *result);
#endif