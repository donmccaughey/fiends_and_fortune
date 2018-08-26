#include <assert.h>
#include <errno.h>
#include <base/base.h>


void
result_test(void);


static void
result_success_test(void)
{
    struct result result = result_success();

    assert(result_is_success(result));
    assert(result_type_success == result.type);
    assert(0 == result.error_code);
}


static void
result_system_error_test(void)
{
    errno = ENOMEM;
    struct result result = result_system_error();

    assert( ! result_is_success(result));
    assert(result_type_system_error == result.type);
    assert(ENOMEM == result.error_code);
}


static void
result_ncurses_err_test(void)
{
    struct result result = result_ncurses_err();

    assert( ! result_is_success(result));
    assert(result_type_ncurses_err == result.type);
    assert(0 == result.error_code);
}


static void
result_ncurses_errno_test(void)
{
    errno = EDOM;
    struct result result = result_ncurses_errno();

    assert( ! result_is_success(result));
    assert(result_type_ncurses_error == result.type);
    assert(EDOM == result.error_code);
}


static void
result_ncurses_error_test(void)
{
    struct result result = result_ncurses_error(ERANGE);

    assert( ! result_is_success(result));
    assert(result_type_ncurses_error == result.type);
    assert(ERANGE == result.error_code);
}


static void
result_set_system_error_test(void)
{
    struct result result = result_set_system_error(EINVAL);

    assert( ! result_is_success(result));
    assert(result_type_system_error == result.type);
    assert(EINVAL == result.error_code);
    assert(EINVAL == errno);
}


void
result_test(void)
{
    result_success_test();
    result_system_error_test();
    result_ncurses_err_test();
    result_ncurses_errno_test();
    result_ncurses_error_test();
    result_set_system_error_test();
}
