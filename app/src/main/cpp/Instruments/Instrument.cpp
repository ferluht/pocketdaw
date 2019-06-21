//
// Created by Admin on 12.06.2019.
//

#include "Instrument.h"

//template <class State>
//Instrument<State>::Instrument()
//{
//    num_voices = 8;
//}

//template <class State>
//void Instrument<State>::midiCommand(MidiData md)
//{
//    switch (md.status & 0xF0){
//        case NOTEON_HEADER:
//            noteOn(md.data1, md.data2);
//            break;
//        case NOTEOFF_HEADER:
//            noteOff(md.data1);
//            break;
//        case CC_HEADER:
//            switch (md.data1){
//                case CC_MODWHEEL:
//                    modwheel = md.data2;
//                    break;
//                case CC_SOSTENUTO:
//                    sostenuto = false;
//                    if (md.data2 != 0xFF) sostenuto = true;
//                    break;
//                default:
//                    break;
//            }
//            break;
//        default:
//            break;
//    }
//}

//template <class State>
//void Instrument<State>::noteOn(unsigned char note, unsigned char velocity)
//{
//    noteOff(note);
//    States.push_front(new State(note, velocity));
//    if (States.size() > num_voices) delete States.pop_back();
//}
//
//template <class State>
//void Instrument<State>::noteOff(unsigned char note)
//{
//    for (auto const& state : States) {
//        if (state->note == note) {
//            delete state;
//            States.remove(state);
//        }
//    }
//}

//template <class State>
//float Instrument<State>::render()
//{
//    float sample = 0;
//    for (auto const& state : States) sample += render(state);
//    return sample;
//}

//template <class State>
//float Instrument<State>::render(State * state)
//{
//    return 0;
//}