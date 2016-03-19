#include "result.h"


extern inline bool
result_is_success(struct result result);

extern inline struct result
result_success(void);

extern inline struct result
result_system_error(void);

extern inline struct result
result_ncurses_err(void);

extern inline struct result
result_ncurses_error(int error_code);

extern inline struct result
result_set_system_error(int error_code);
