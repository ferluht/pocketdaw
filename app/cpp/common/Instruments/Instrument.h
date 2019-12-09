//
// Created by Admin on 12.06.2019.
//

#ifndef PD_INSTRUMENT_H
#define PD_INSTRUMENT_H

#include "Engine/Engine.h"
#include "GUI/IECanvas.h"
#include <cmath>
#include <set>

class InstrumentState {

    unsigned char key;
    bool active;

public:

	InstrumentState() {
	    active = false;
	};

	inline void setActive(bool isactive) { active = isactive; };
    inline bool isActive() { return active; }

	template <class State> friend class Instrument;
};

template <class State>
class Instrument : public GUI::IECanvas{

    unsigned int num_voices = 8;
    std::multiset<State *> States;

    std::mutex keyPressedLock;

    bool damper_pedal;
    bool portamento;
    bool sostenuto;
    bool soft_pedal;
    bool chorus;
    bool celeste;
    bool phaser;

public:

    float base_note = 69.0;
    float power_base = 2.0;
    float semitones = 24.0;
	double base_frequency = 440.0;

	float pitch;
    float pitch_distance = 12.0;

    Instrument(unsigned int num_voices_, const char * name_) : IECanvas(name_){
        num_voices = num_voices_;
        pitch = 0;
        for (int i = 0; i < num_voices; i++) States.insert(new State());
    }

    void MIn(MData md) override;

    bool ARender(double beat, float * lsample, float * rsample) override;

    void IKeyPressed(MData md);

    inline double getFrequency(double note)
    {
    	return base_frequency*(pow(power_base, (note - base_note + pitch)/semitones));
	}

	inline double getPhaseIncrement(double note)
    {
    	return 2*M_PI*getFrequency(note)/sample_rate;
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
        case PITCHWHEEL_HEADER:
            pitch = (cmd.data1 + cmd.data2*128.0f) / (float)0xFFFF * pitch_distance;
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
        if ((*it)->key == md.data1){
            State * state = (*it);
            States.erase(it);
            IUpdateState(state, md);
            state->key = md.data1;
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
            state->key = md.data1;
            States.insert(state);
            keyPressedLock.unlock();
            return;
        }
    }

    State * state = *States.begin();
    IUpdateState(state, md);
    state->key = md.data1;
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
