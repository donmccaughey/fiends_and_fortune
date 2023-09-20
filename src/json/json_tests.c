#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <base/base.h>
#include <json/json.h>


static void
json_array_alloc_string_value_when_missing_test(void)
{
    char const *json_string = "[]";
    struct cJSON *json_array = cJSON_Parse(json_string);

    char const missing[] = "(missing)";
    char *value = json_array_alloc_string_value(json_array, 0, missing);

    assert(str_eq(missing, value));
    assert(missing != value);

    free(value);
    cJSON_Delete(json_array);
}


static void
json_array_alloc_string_value_when_present_test(void)
{
    char const *json_string = "[ \"present\" ]";
    struct cJSON *json_array = cJSON_Parse(json_string);

    char const missing[] = "(missing)";
    char *value = json_array_alloc_string_value(json_array, 0, missing);

    assert(str_eq("present", value));

    free(value);
    cJSON_Delete(json_array);
}


static void
json_array_alloc_string_value_when_bad_value_test(void)
{
    char const *json_string = "[ true ]";
    struct cJSON *json_array = cJSON_Parse(json_string);

    char const missing[] = "(missing)";
    char *value = json_array_alloc_string_value(json_array, 0, missing);

    assert(str_eq(missing, value));
    assert(missing != value);

    free(value);
    cJSON_Delete(json_array);
}


static void
json_array_alloc_string_value_when_default_is_null_test(void)
{
    char const *json_string = "[]";
    struct cJSON *json_array = cJSON_Parse(json_string);

    char *value = json_array_alloc_string_value(json_array, 0, NULL);

    assert(NULL == value);

    free(value);
    cJSON_Delete(json_array);
}


static void
json_array_get_string_value_when_missing_test(void)
{
    char const *json_string = "[]";
    struct cJSON *json_array = cJSON_Parse(json_string);

    char const missing[] = "(missing)";
    char const *value = json_array_get_string_value(json_array, 0, missing);

    assert(str_eq(missing, value));
    assert(missing == value);

    cJSON_Delete(json_array);
}


static void
json_array_get_string_value_when_present_test(void)
{
    char const *json_string = "[ \"present\" ]";
    struct cJSON *json_array = cJSON_Parse(json_string);

    char const missing[] = "(missing)";
    char const *value = json_array_get_string_value(json_array, 0, missing);

    assert(str_eq("present", value));

    cJSON_Delete(json_array);
}


static void
json_array_get_string_value_when_bad_value_test(void)
{
    char const *json_string = "[ false ]";
    struct cJSON *json_array = cJSON_Parse(json_string);

    char const missing[] = "(missing)";
    char const *value = json_array_get_string_value(json_array, 0, missing);

    assert(str_eq(missing, value));
    assert(missing == value);

    cJSON_Delete(json_array);
}


static void
json_array_get_string_value_when_default_is_null_test(void)
{
    char const *json_string = "[]";
    struct cJSON *json_array = cJSON_Parse(json_string);

    char const *value = json_array_get_string_value(json_array, 0, NULL);

    assert(NULL == value);

    cJSON_Delete(json_array);
}


static void
json_object_alloc_string_value_when_missing_test(void)
{
    char const *json_string = "{}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    char const missing[] = "(missing)";
    char *value = json_object_alloc_string_value(json_object, "foo", missing);

    assert(str_eq(missing, value));
    assert(missing != value);

    free(value);
    cJSON_Delete(json_object);
}


static void
json_object_alloc_string_value_when_present_test(void)
{
    char const *json_string = "{"
                              "  \"foo\": \"bar\""
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    char const missing[] = "(missing)";
    char *value = json_object_alloc_string_value(json_object, "foo", missing);

    assert(str_eq("bar", value));

    free(value);
    cJSON_Delete(json_object);
}


static void
json_object_alloc_string_value_when_bad_value_test(void)
{
    char const *json_string = "{"
                              "  \"foo\": 42"
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    char const missing[] = "(missing)";
    char *value = json_object_alloc_string_value(json_object, "foo", missing);

    assert(str_eq(missing, value));
    assert(missing != value);

    free(value);
    cJSON_Delete(json_object);
}


static void
json_object_alloc_string_value_when_default_is_null_test(void)
{
    char const *json_string = "{}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    char *value = json_object_alloc_string_value(json_object, "foo", NULL);

    assert(NULL == value);

    cJSON_Delete(json_object);
}


static void
json_object_get_bool_value_when_missing_test(void)
{
    char const *json_string = "{}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    int value = json_object_get_bool_value(json_object, "missing", true);
    assert(true == value);

    cJSON_Delete(json_object);
}


static void
json_object_get_bool_value_when_present_test(void)
{
    char const *json_string = "{"
                              "  \"foo\": true"
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    int value = json_object_get_bool_value(json_object, "foo", false);
    assert(true == value);

    cJSON_Delete(json_object);
}


static void
json_object_get_int_value_when_missing_test(void)
{
    char const *json_string = "{}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    int value = json_object_get_int_value(json_object, "missing", 42);
    assert(42 == value);

    cJSON_Delete(json_object);
}


static void
json_object_get_int_value_when_present_test(void)
{
    char const *json_string = "{"
                              "  \"foo\": 29"
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    int value = json_object_get_int_value(json_object, "foo", 42);
    assert(29 == value);

    cJSON_Delete(json_object);
}


static void
json_object_get_int_value_when_too_big_test(void)
{
    char const *json_string = "{"
                              "  \"foo\": 2147483648"
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    int value = json_object_get_int_value(json_object, "foo", 42);
    assert(INT_MAX == value);

    cJSON_Delete(json_object);
}


static void
json_object_get_int_value_when_too_small_test(void)
{
    char const *json_string = "{"
                              "  \"foo\": -2147483649"
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    int value = json_object_get_int_value(json_object, "foo", 42);
    assert(INT_MIN == value);

    cJSON_Delete(json_object);
}


static void
json_object_get_int_value_when_float_test(void)
{
    char const *json_string = "{"
                              "  \"foo\": 2.999"
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    int value = json_object_get_int_value(json_object, "foo", 42);
    assert(2 == value);

    cJSON_Delete(json_object);
}


static void
json_object_get_string_value_when_missing_test(void)
{
    char const *json_string = "{}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    char const missing[] = "(missing)";
    char const *value = json_object_get_string_value(json_object, "foo", missing);

    assert(missing == value);

    cJSON_Delete(json_object);
}


static void
json_object_get_string_value_when_present_test(void)
{
    char const *json_string = "{"
                              "  \"foo\": \"bar\""
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    char const missing[] = "(missing)";
    char const *value = json_object_get_string_value(json_object, "foo", missing);

    assert(str_eq("bar", value));

    cJSON_Delete(json_object);
}


static void
json_object_get_string_value_when_bad_value_test(void)
{
    char const *json_string = "{"
                              "  \"foo\": 42"
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    char const missing[] = "(missing)";
    char const *value = json_object_get_string_value(json_object, "foo", missing);

    assert(missing == value);

    cJSON_Delete(json_object);
}


static void
json_object_get_string_value_when_default_is_null_test(void)
{
    char const *json_string = "{}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    char const *value = json_object_get_string_value(json_object, "foo", NULL);

    assert(NULL == value);

    cJSON_Delete(json_object);
}


static void
json_object_has_struct_member_when_no_key_test(void)
{
    char const *json_string = "{}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    assert( ! json_object_has_struct_member(json_object, "person"));

    cJSON_Delete(json_object);
}


static void
json_object_has_struct_member_when_bad_value_test(void)
{
    char const *json_string = "{"
                              "  \"struct\": null"
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    assert( ! json_object_has_struct_member(json_object, "person"));

    cJSON_Delete(json_object);
}


static void
json_object_has_struct_member_when_wrong_value_test(void)
{
    char const *json_string = "{"
                              "  \"struct\": \"company\""
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    assert( ! json_object_has_struct_member(json_object, "person"));

    cJSON_Delete(json_object);
}


static void
json_object_has_struct_member_test(void)
{
    char const *json_string = "{"
                              "  \"struct\": \"person\""
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);

    assert(json_object_has_struct_member(json_object, "person"));

    cJSON_Delete(json_object);
}


int
main(int argc, char *argv[])
{
    json_array_alloc_string_value_when_missing_test();
    json_array_alloc_string_value_when_present_test();
    json_array_alloc_string_value_when_bad_value_test();
    json_array_alloc_string_value_when_default_is_null_test();

    json_array_get_string_value_when_missing_test();
    json_array_get_string_value_when_present_test();
    json_array_get_string_value_when_bad_value_test();
    json_array_get_string_value_when_default_is_null_test();

    json_object_alloc_string_value_when_missing_test();
    json_object_alloc_string_value_when_present_test();
    json_object_alloc_string_value_when_bad_value_test();
    json_object_alloc_string_value_when_default_is_null_test();

    json_object_get_bool_value_when_missing_test();
    json_object_get_bool_value_when_present_test();

    json_object_get_int_value_when_missing_test();
    json_object_get_int_value_when_present_test();
    json_object_get_int_value_when_too_big_test();
    json_object_get_int_value_when_too_small_test();
    json_object_get_int_value_when_float_test();

    json_object_get_string_value_when_missing_test();
    json_object_get_string_value_when_present_test();
    json_object_get_string_value_when_bad_value_test();
    json_object_get_string_value_when_default_is_null_test();

    json_object_has_struct_member_when_no_key_test();
    json_object_has_struct_member_when_bad_value_test();
    json_object_has_struct_member_when_wrong_value_test();
    json_object_has_struct_member_test();

    return EXIT_SUCCESS;
}
