//
// Created by Admin on 12.06.2019.
//

#ifndef PD_INSTRUMENT_H
#define PD_INSTRUMENT_H

#include "../Orchestration/Midi.h"
#include <cmath>

struct InstrumentState {
	double phase;
	double frequency;
	double phase_increment;
	
};

class Instrument{
public:

	std::vector<InstrumentState> Voices;

	double base_frequency;
	double sample_rate;

	unsigned char modwheel;
	unsigned char pitchwheel;
	unsigned char volume;
	
	bool damper_pedal;
	bool portamento;
	bool sostenuto;
	bool soft_pedal;
	bool chorus;
	bool celeste;
	bool phaser;

	unsigned int num_voices;

    Instrument();

    void midiCommand(MidiData md);
    float render();

    void noteOn(unsigned char note, unsigned char velocity);
    void noteOff(unsigned char note);

    inline double getFrequency(unsigned char note)
    {
    	return base_frequency*(pow(POWER_BASE, (note - BASE_NOTE)/SEMITONES));
	}

	inline double getFrequency(unsigned char note, short cents)
    {
    	return base_frequency*(pow(POWER_BASE, (note - BASE_NOTE)/SEMITONES))*(pow(POWER_BASE, cents/SEMITONES/1000.0));
	}

	inline double getPhaseIncrement(double frequency)
    {
    	return 2*M_PI*frequency/sample_rate;
	}

	inline double getPhaseIncrement(unsigned char note)
    {
    	return getPhaseIncrement(getFrequency(note));
	}

	inline double getPhaseIncrement(unsigned char note, short cents)
    {
    	return getPhaseIncrement(getFrequency(note, cents));
	}

    virtual float render(InstrumentState state);


private:

	unsigned char modwheel, 
};


#endif //PD_INSTRUMENT_H
