//
// Created by julien@macmini on 05/04/2018.
//

#include "LinkManager.h"

using Clock = ableton::link::platform::Clock;

LinkManager::LinkManager(): link(120.){
    link.enable(true);
}

void LinkManager::enable() {
    link.enable(true);
}

double LinkManager::getTempo() {
    if (link.isEnabled()) {
        auto state = link.captureAppSessionState();
        return state.tempo();
    } else {
        return -1;
    }
}

double LinkManager::getPhase() {

    if (link.isEnabled()) {

        const auto time = link.clock().micros();
        return link.captureAppSessionState().phaseAtTime(time, 4);

    } else {
        return -1;
    }
}
