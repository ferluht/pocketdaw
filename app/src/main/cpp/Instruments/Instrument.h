//
// Created by Admin on 12.06.2019.
//

#ifndef PD_INSTRUMENT_H
#define PD_INSTRUMENT_H

#include "../Orchestration/Midi.h"
#include <cmath>
#include <list>

class InstrumentState {
public:
    unsigned char note;
    unsigned char velocity;

	double phase;
	double frequency;
	double phase_increment;
	double time_from_trigger;
};

class InstrumentBase {
public:
    InstrumentBase() = default;

    virtual void midiCommand(MidiData md) = 0;
    virtual float render() = 0;
};

template <class State>
class Instrument : public InstrumentBase{
public:

    unsigned int num_voices = 8;
	std::list<State *> States;

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

//    Instrument();

    void midiCommand(MidiData md) override {
        switch (md.status & 0xF0){
            case NOTEON_HEADER:
//                noteOn(md.data1, md.data2);
                for (auto const& state : States) {
                    if (state->note == md.data1) {
                        delete state;
                        States.remove(state);
                    }
                }
                States.push_front(new State(md.data1, md.data2));
                if (States.size() > num_voices) States.pop_back();
                break;
            case NOTEOFF_HEADER:
//                noteOff(md.data1);
                for (auto const& state : States) {
                    if (state->note == md.data1) {
                        delete state;
                        States.remove(state);
                    }
                }
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
    };

    float render() override {
        float sample = 0;
        for (auto const& state : States) sample += 1.0/(float)States.size() * render(state);
        return sample;
    };

//    void noteOn(unsigned char note, unsigned char velocity);
//    void noteOff(unsigned char note);

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

    virtual float render(State * state) {return 0;}
};


#endif //PD_INSTRUMENT_H
