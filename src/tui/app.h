#ifndef FNF_TUI_APP_H_INCLUDED
#define FNF_TUI_APP_H_INCLUDED


#include <stdbool.h>

#include "common/result.h"


struct desktop;


struct app {
    struct desktop *desktop;
    bool is_running;
};


struct app *
app_alloc(void);

void
app_free(struct app *app);

struct result
app_run(struct app *app);

void
app_quit(struct app *app);


#endif
