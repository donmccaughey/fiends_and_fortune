#ifndef FNF_JSON_H_INCLUDED
#define FNF_JSON_H_INCLUDED


#include <stdbool.h>
#include <cJSON/cJSON.h>


int
json_object_get_int_value(struct cJSON *json_object,
                          char const *name,
                          int default_value);

bool
json_object_has_struct_member(struct cJSON *json_object,
                              char const *struct_name);


#endif
