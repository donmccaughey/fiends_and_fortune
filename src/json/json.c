#include "json.h"

#include <assert.h>
#include <limits.h>
#include <base/base.h>


bool
json_object_get_bool_value(struct cJSON *json_object,
                           char const *name,
                           bool default_value)
{
    assert(cJSON_IsObject(json_object));

    struct cJSON *item = cJSON_GetObjectItemCaseSensitive(json_object, name);
    if (cJSON_IsTrue(item)) {
        return true;
    } else if (cJSON_IsFalse(item)) {
        return false;
    } else {
        return default_value;
    }
}


int
json_object_get_int_value(struct cJSON *json_object,
                          char const *name,
                          int default_value)
{
    assert(cJSON_IsObject(json_object));

    struct cJSON *item = cJSON_GetObjectItemCaseSensitive(json_object, name);
    if ( ! cJSON_IsNumber(item)) return default_value;
    if (item->valuedouble > (double)INT_MAX) return INT_MAX;
    if (item->valuedouble < (double)INT_MIN) return INT_MIN;
    return (int)item->valuedouble;
}


int
json_object_get_string_enum_value(struct cJSON *json_object,
                                  char const *name,
                                  json_string_enum_lookup_fn lookup_fn,
                                  int default_value)
{
    assert(cJSON_IsObject(json_object));

    struct cJSON *item = cJSON_GetObjectItemCaseSensitive(json_object, name);
    if (cJSON_IsString(item)) {
        return lookup_fn(item->valuestring, default_value);
    } else {
        return default_value;
    }
}


char const *
json_object_get_string_value(struct cJSON *json_object,
                             char const *name,
                             char const *default_value)
{
    assert(cJSON_IsObject(json_object));

    struct cJSON *item = cJSON_GetObjectItemCaseSensitive(json_object, name);
    return cJSON_IsString(item) ? item->valuestring : default_value;
}


bool
json_object_has_struct_member(struct cJSON *json_object,
                              char const *struct_name)
{
    assert(cJSON_IsObject(json_object));

    struct cJSON *struct_member = cJSON_GetObjectItem(json_object, "struct");
    char const *string_value = cJSON_GetStringValue(struct_member);
    return string_value && str_eq(struct_name, string_value);
}
