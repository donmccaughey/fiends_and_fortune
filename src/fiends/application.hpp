#ifndef FIENDS_APPLICATION_HPP_INCLUDED
#define FIENDS_APPLICATION_HPP_INCLUDED


#include "fiends.hpp"


class Application : public TApplication {
public:
    Application();

    void handleEvent(TEvent& event) override;

private:
    static void about();
    static void treasureTypesTable();
};


#endif
