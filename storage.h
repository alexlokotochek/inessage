#ifndef __storage__h
#define __storage__h
#include "jansson.h"

void init_storage();

void save_json(char* json);

json_t** readAllStorage();

int doesContainOnStorage(char* json);

#endif