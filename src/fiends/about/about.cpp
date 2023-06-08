#include "about.hpp"

#include "about_dialog.hpp"

#include "fiends/application/application.hpp"


void
about(Application &application, TEvent &event)
{
    auto d = newAboutDialog();
    Application::deskTop->execView(d);
    Application::destroy(d);
    application.clearEvent(event);
}
