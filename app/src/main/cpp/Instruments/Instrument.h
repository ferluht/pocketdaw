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

    std::mutex keyPressedLock;

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

    Instrument(int num_voices){
        GAttachTexture("Textures/effect_canvas.bmp");
        for (int i = 0; i < num_voices; i++) States.insert(new State());
    }

    void MIn(MData md) override;

    bool ARender(double beat, float * lsample, float * rsample) override;

    void IKeyPressed(MData md);

    inline double getFrequency(unsigned char note)
    {
    	return base_frequency*(pow(POWER_BASE, (note - BASE_NOTE)/SEMITONES));
	}

	inline double getFrequency(unsigned char note, float cents)
    {
    	return base_frequency*(pow(POWER_BASE, (note - BASE_NOTE)/SEMITONES))*(pow(POWER_BASE, cents/SEMITONES));
	}

	inline double getPhaseIncrement(double frequency)
    {
    	return 2*M_PI*frequency/sample_rate;
	}

	inline double getPhaseIncrement(unsigned char note)
    {
    	return getPhaseIncrement(getFrequency(note));
	}

	inline double getPhaseIncrement(unsigned char note, float cents)
    {
    	return getPhaseIncrement(getFrequency(note, cents));
	}

    virtual void IUpdateState(State * state, MData cmd) {};

    virtual void IARender(State * state, double beat, float * lsample, float * rsample) {}
};

template <class State>
void Instrument<State>::MIn(MData cmd)
{
    switch (cmd.status & 0xF0){
        case NOTEON_HEADER:
        case NOTEOFF_HEADER:
            IKeyPressed(cmd);
            break;
        case CC_HEADER:
            switch (cmd.data1){
//                case CC_MODWHEEL:
//                    modwheel = cmd.data2;
//                    break;
//                case CC_SOSTENUTO:
//                    sostenuto = false;
//                    if (cmd.data2 != 0xFF) sostenuto = true;
//                    break;
                default:
                    MOut(cmd);
            }
            break;
        default:
            MOut(cmd);
    }
}

template <class State>
void Instrument<State>::IKeyPressed(MData md)
{
    keyPressedLock.lock();

    for (auto it = States.begin(); it != States.end(); it++ )
    {
        if ((*it)->note == md.data1){
            State * state = (*it);
            States.erase(it);
            IUpdateState(state, md);
            States.insert(state);
            keyPressedLock.unlock();
            return;
        }
    }

    for (auto it = States.begin(); it != States.end(); it++ )
    {
        if (!(*it)->active){
            State * state = (*it);
            States.erase(it);
            IUpdateState(state, md);
            States.insert(state);
            keyPressedLock.unlock();
            return;
        }
    }

    State * state = *States.begin();
    IUpdateState(state, md);
    if(num_voices > 1) {
        States.erase(States.begin());
        States.insert(state);
    }
    keyPressedLock.unlock();
}


template <class State>
bool Instrument<State>::ARender(double beat, float * lsample, float * rsample)
{
    keyPressedLock.lock();

    for (auto it = States.begin(); it != States.end(); it++ ){
        if ((*it)->active){
            IARender(*it, beat, lsample, rsample);
        }
    }

    keyPressedLock.unlock();

    *lsample /= (float)num_voices;
    *rsample /= (float)num_voices;

    return true;
}


#endif //PD_INSTRUMENT_H
