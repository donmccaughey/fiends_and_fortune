#include "json.h"

#include <assert.h>
#include <limits.h>
#include <base/base.h>


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


bool
json_object_has_struct_member(struct cJSON *json_object,
                              char const *struct_name)
{
    assert(cJSON_IsObject(json_object));

    struct cJSON *struct_member = cJSON_GetObjectItem(json_object, "struct");
    char const *string_value = cJSON_GetStringValue(struct_member);
    return string_value && str_eq(struct_name, string_value);
}
