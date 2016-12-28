#include "app.h"

#include <signal.h>

#include <sys/ioctl.h>

#include "common/alloc_or_die.h"

#include "screen_view.h"
#include "view.h"


static struct result
dispatch_events(struct app *app)
{
    struct result result = result_success();
    app->is_running = true;
    do {
        int key = view_read_key(app->active_view);
        if (ERR == key) {
            result = result_ncurses_err();
            app_quit(app);
            break;
        }
        
        result = app->active_view->on_key(app->active_view, app, key);
        if (!result_is_success(result)) {
            app_quit(app);
            break;
        }
    } while (app->is_running);
    return result;
}


static void
terminal_window_did_change(int signal)
{
    struct winsize winsize;
    ioctl(0, TIOCGWINSZ, &winsize);
    resizeterm(winsize.ws_row, winsize.ws_col);
}


struct result
app_activate_view(struct app *app, struct view *view)
{
    struct result result = view_enable_keyboard(view);
    if (!result_is_success(result)) return result;
    
    app->active_view = view;
    return result_success();
}


struct result
app_add_view(struct app *app, struct view *view)
{
    int index = app->views_count;
    ++app->views_count;
    app->views = reallocarray_or_die(app->views, app->views_count, sizeof(struct view *));
    app->views[index] = view;
    return result_success();
}


struct app *
app_alloc(void)
{
    struct app *app = calloc_or_die(1, sizeof(struct app));
    app->views = calloc_or_die(1, sizeof(struct view *));
    app->views[0] = screen_view_alloc();
    app->views_count = 1;
    return app;
}


void
app_free(struct app *app)
{
    if (!app) return;
    for (int i = 0; i < app->views_count; ++i) {
        view_free(app->views[i]);
    }
    free_or_die(app->views);
    free_or_die(app);
}


struct result
app_ring_bell(struct app *app)
{
    int code = cbreak();
    if (ERR == code) return result_ncurses_err();
    
    code = beep();
    if (ERR == code) return result_ncurses_err();
    
    code = raw();
    if (ERR == code) return result_ncurses_err();
    
    return result_success();
}


struct result
app_run(struct app *app)
{
    sig_t previous_sigwinch_handler = signal(SIGWINCH, terminal_window_did_change);
    if (SIG_ERR == previous_sigwinch_handler) {
        return result_system_error();
    }
    
    struct result result = result_success();
    
    for (int i = 0; i < app->views_count; ++i) {
        result = app->views[i]->create(app->views[i], app);
        if (!result_is_success(result)) break;
    }
    
    if (result_is_success(result)) {
        for (int i = 0; i < app->views_count; ++i) {
            result = app->views[i]->draw(app->views[i], app);
            if (!result_is_success(result)) break;
            wnoutrefresh(app->views[i]->window);
        }
    }
    
    if (!app->active_view) {
        int i = app->views_count - 1;
        result = app_activate_view(app, app->views[i]);
    }
    
    doupdate();
    
    if (result_is_success(result)) {
        result = dispatch_events(app);
    }
    
    for (int i = app->views_count - 1; i >= 0; --i) {
        app->views[i]->destroy(app->views[i], app);
    }
    
    signal(SIGWINCH, previous_sigwinch_handler);
    return result;
}


void
app_quit(struct app *app)
{
    app->is_running = false;
}


extern inline struct view *
app_get_screen_view(struct app *app);

extern inline struct rect
app_get_screen_view_rect(struct app *app);

extern inline void
app_hide_cursor(struct app *app);

extern inline void
app_show_cursor(struct app *app);
