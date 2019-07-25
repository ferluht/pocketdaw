//
// Created by Admin on 12.06.2019.
//

#ifndef PD_INSTRUMENT_H
#define PD_INSTRUMENT_H

#include "Orchestration/Midi.h"
#include "GUI/Canvas.h"
#include <cmath>
#include <set>

#define MAX_VOICES 8

class InstrumentState {
public:
    double beat;
    unsigned char note;
    unsigned char velocity;

    float volume;
	double phase;
	double frequency;
	double phase_increment;

	bool active;

	InstrumentState();
	void setActive(bool isactive);
};

bool operator<(const InstrumentState &lhs, const InstrumentState &rhs);

class InstrumentBase : public Canvas {
public:
    InstrumentBase() : Canvas(0, 0, 0, 0) {};

    virtual void midiCommand(MidiData md) = 0;
    virtual void render(double beat, float * lsample, float * rsample) = 0;
};

template <class State>
class Instrument : public InstrumentBase{
public:

    unsigned int num_voices = 8;
	std::multiset<State *> States;

	double base_frequency = 440.0;
	double sample_rate = 48000.0;

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

	unsigned char active_states;

    Instrument(){
        for (int i = 0; i < MAX_VOICES; i++) States.insert(new State());
    }

    void midiCommand(MidiData md) override;

    void render(double beat, float * lsample, float * rsample) override;

    void keyPressed(MidiData md);

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

    virtual void updateState(State * state, MidiData cmd) {};

    virtual void render(State * state, double beat, float * lsample, float * rsample) {}
};

template <class State>
void Instrument<State>::midiCommand(MidiData md)
{
    switch (md.status & 0xF0){
        case NOTEON_HEADER:
        case NOTEOFF_HEADER:
            keyPressed(md);
            break;
        case CC_HEADER:
            switch (md.data1){
                case CC_MODWHEEL:
                    modwheel = md.data2;
                    break;
                case CC_SOSTENUTO:
                    sostenuto = false;
                    if (md.data2 != 0xFF) sostenuto = true;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

template <class State>
void Instrument<State>::keyPressed(MidiData md)
{
    for (auto it = States.begin(); it != States.end(); it++ )
    {
        if ((*it)->note == md.data1){
            State * state = (*it);
            States.erase(it);
            updateState(state, md);
            States.insert(state);
            return;
        }
    }


    for (auto it = States.begin(); it != States.end(); it++ )
    {
        if (!(*it)->active){
            State * state = (*it);
            States.erase(it);
            updateState(state, md);
            States.insert(state);
            return;
        }
    }

    State * state = *States.begin();
    States.erase(States.begin());
    updateState(state, md);
    States.insert(state);
}


template <class State>
void Instrument<State>::render(double beat, float * lsample, float * rsample)
{
    for (auto it = States.begin(); it != States.end(); it++ ){
        if ((*it)->active){
            render(*it, beat, lsample, rsample);
        }
    }
    *lsample /= (float)num_voices;
    *rsample /= (float)num_voices;
}


#endif //PD_INSTRUMENT_H
