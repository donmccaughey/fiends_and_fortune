#ifndef FIENDS_FIENDS_APP_HPP_INCLUDED
#define FIENDS_FIENDS_APP_HPP_INCLUDED


#include "fiends.hpp"


class FiendsApp : public TApplication {
public:
    FiendsApp();

    void handleEvent(TEvent& event) override;

private:
    static void aboutBox();
    static void treasureTypesTable();
};


#endif
