#ifndef FIENDS_FIENDS_APP_HPP_INCLUDED
#define FIENDS_FIENDS_APP_HPP_INCLUDED


#include "fiends.hpp"


class TFiendsApp : public TApplication {
public:
    TFiendsApp();

    void handleEvent(TEvent& event) override;

private:
    static void greetingBox();
};


#endif
