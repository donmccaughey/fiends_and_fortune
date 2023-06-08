#include "treasure_types_table.hpp"

#include "treasure_types_table_window.hpp"

#include "fiends/application.hpp"


void
treasureTypesTable(Application &application, TEvent &event)
{
    TRect r(0, 0, 60, 20);
    auto treasureTypesTableWindow = new TreasureTypesTableWindow(r);
    Application::deskTop->insert(treasureTypesTableWindow);
    application.clearEvent(event);
}
