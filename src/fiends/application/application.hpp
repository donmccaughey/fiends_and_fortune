#ifndef FIENDS_APPLICATION_HPP_INCLUDED
#define FIENDS_APPLICATION_HPP_INCLUDED


#include "fiends/fiends.hpp"


class Application : public TApplication {
public:
    Application();

    void handleEvent(TEvent& event) override;
};


#endif
