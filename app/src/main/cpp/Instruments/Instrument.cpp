//
// Created by Admin on 12.06.2019.
//

#include "Instrument.h"

InstrumentState::InstrumentState()
{
    note = 0;
    phase = 0;
    active = false;
}

void InstrumentState::setActive(bool isactive){
    active = isactive;
}

bool operator<(const InstrumentState &lhs, const InstrumentState &rhs) {
    return lhs.beat > rhs.beat;
}