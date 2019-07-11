//
// Created by Admin on 12.06.2019.
//

#ifndef PD_METRONOME_H
#define PD_METRONOME_H

#include "../Orchestration/Master.h"
#include <list>
#include <string>

class Interface {
public:

    Interface(Master * master_);


    std::list<string> getAvailableMidiEffects();
    std::list<string> getAvailableInstruments();
    std::list<string> getAvailableAudioEffects();

    void createTrack();
    void insertMidiEffect(std::string effect, std::string track);
	void insertInstrument(std::string instrument, std::string track);
    void insertAudioEffect(std::string effect, std::string track);



};


#endif //PD_METRONOME_H
