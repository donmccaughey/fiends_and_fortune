#ifndef FNF_JSON_H_INCLUDED
#define FNF_JSON_H_INCLUDED


#include <stdbool.h>
#include <cJSON/cJSON.h>


typedef int
(json_string_enum_lookup_fn)(char const *value, int default_value);


bool
json_object_get_bool_value(struct cJSON *json_object,
                           char const *name,
                           bool default_value);

int
json_object_get_int_value(struct cJSON *json_object,
                          char const *name,
                          int default_value);

int
json_object_get_string_enum_value(struct cJSON *json_object,
                                  char const *name,
                                  json_string_enum_lookup_fn lookup_fn,
                                  int default_value);

char const *
json_object_get_string_value(struct cJSON *json_object,
                             char const *name,
                             char const *default_value);

bool
json_object_has_struct_member(struct cJSON *json_object,
                              char const *struct_name);


#endif
