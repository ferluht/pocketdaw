//
// Created by julien@macmini on 05/04/2018.
//

#ifndef LINKTEST_ANDROID_DUMMY_H
#define LINKTEST_ANDROID_DUMMY_H

#include <ableton/Link.hpp>

class LinkManager {

public:
    LinkManager();
    double getTempo();
    double getPhase();
    void enable();
private:
    ableton::Link link;
};

#endif //LINKTEST_ANDROID_DUMMY_H
