//
// Created by Admin on 12.06.2019.
//

#ifndef PD_INSTRUMENT_H
#define PD_INSTRUMENT_H

#include "Engine/Engine.h"
#include "GUI/Canvas.h"
#include <cmath>
#include <set>

#define MAX_VOICES 8

class InstrumentState {
public:
    bool active;

    unsigned char note;

	InstrumentState();
	void setActive(bool isactive);
};

template <class State>
class Instrument : public AMGCanvas{
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

    void MIn(MData md) override;

    void ARender(double beat, float * lsample, float * rsample) override;

    void keyPressed(MData md);

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

    virtual void updateState(State * state, MData cmd) {};

    virtual void render(State * state, double beat, float * lsample, float * rsample) {}
};

template <class State>
void Instrument<State>::MIn(MData cmd)
{
    switch (cmd.status & 0xF0){
        case NOTEON_HEADER:
        case NOTEOFF_HEADER:
            keyPressed(cmd);
            break;
        case CC_HEADER:
            switch (cmd.data1){
                case CC_MODWHEEL:
                    modwheel = cmd.data2;
                    break;
                case CC_SOSTENUTO:
                    sostenuto = false;
                    if (cmd.data2 != 0xFF) sostenuto = true;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    MOut(cmd);
}

template <class State>
void Instrument<State>::keyPressed(MData md)
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
void Instrument<State>::ARender(double beat, float * lsample, float * rsample)
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
