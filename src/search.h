#ifdef _SEARCH_H_
#define _SEARCH_H_

unsigned int hash(int key);
void insert_record(hash_entry_t *table, FILE *file, travel_t *data);
float find_record(hash_entry_t *table, FILE *file, int sourceid, int dstid, int hod);
float search(int *data);

#endif