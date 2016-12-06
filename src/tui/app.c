#include "app.h"

#include <signal.h>

#include <sys/ioctl.h>

#include "common/alloc_or_die.h"

#include "screen.h"
#include "view.h"


static struct result
update_windows(struct app *app)
{
    int code = doupdate();
    return (ERR == code) ? result_ncurses_err(): result_success();
}


static struct result
dispatch_events(struct app *app)
{
    struct result result = result_success();
    app->is_running = true;
    do {
        result = update_windows(app);
        if (!result_is_success(result)) {
            app_quit(app);
            break;
        }
        
        int ch = view_read_char(app->active_view);
        if (ERR == ch) {
            result = result_ncurses_err();
            app_quit(app);
            break;
        }
        
        // TODO: dispatch key event
        
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
    
    result = view_invalidate(view);
    if (!result_is_success(result)) return result;
    
    app->active_view = view;
    return result_success();
}


struct app *
app_alloc(void)
{
    struct app *app = calloc_or_die(1, sizeof(struct app));
    app->views = calloc_or_die(1, sizeof(struct view *));
    app->views[0] = screen_alloc();
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
            
            result = view_invalidate(app->views[i]);
            if (!result_is_success(result)) break;
        }
    }
    
    if (!app->active_view) {
        result = app_activate_view(app, app->views[0]);
    }
    
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
app_get_screen(struct app *app);

extern inline struct rect
app_get_screen_rect(struct app *app);
