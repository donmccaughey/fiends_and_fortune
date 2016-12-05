#include "app.h"

#include <signal.h>

#include <sys/ioctl.h>

#include "common/alloc_or_die.h"

#include "desktop.h"


static struct result
dispatch_events(struct app *app)
{
    struct result result = result_success();
    WINDOW *active_window = app->desktop->window;
    app->is_running = true;
    do {
        result = window_enable_keyboard(active_window);
        if (!result_is_success(result)) {
            app_quit(app);
            break;
        }
        
        int ch = wgetch(app->desktop->window);
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


struct app *
app_alloc(void)
{
    struct app *app = calloc_or_die(1, sizeof(struct app));
    app->desktop = desktop_alloc();
    return app;
}


void
app_free(struct app *app)
{
    if (!app) return;
    desktop_free(app->desktop);
    free_or_die(app);
}


struct result
app_run(struct app *app)
{
    sig_t previous_sigwinch_handler = signal(SIGWINCH, terminal_window_did_change);
    if (SIG_ERR == previous_sigwinch_handler) {
        return result_system_error();
    }
    
    struct result result = desktop_show(app->desktop);
    if (result_is_success(result)) {
        result = dispatch_events(app);
    }
    desktop_hide(app->desktop);
    
    signal(SIGWINCH, previous_sigwinch_handler);
    return result;
}


void
app_quit(struct app *app)
{
    app->is_running = false;
}
