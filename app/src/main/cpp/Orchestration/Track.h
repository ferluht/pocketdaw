//
// Created by Admin on 12.06.2019.
//

#ifndef PD_TRACK_H
#define PD_TRACK_H

#include <stdlib.h>
#include <map>
#include <vector>
#include <queue>
#include "../Instruments/Instrument.h"
#include "../AudioEffects/AudioEffect.h"
#include "../MidiEffects/MidiEffect.h"
#include "Midi.h"
#include "../GUI/GraphicObject.h"

class Track : public GraphicObject{

public:

    int length = 0;
    bool isOn = true, rec = false;
    float volume = 1;
    float pan = 0;
    double last_beat;
    MidiClip * current_clip;

    std::priority_queue<MidiData> MidiQueue;

    std::vector<MidiClip*> MidiClips;

    std::vector<MidiEffect*> MidiEffects;
    InstrumentBase* TrackInstrument;
    std::vector<AudioEffect*> AudioEffects;

    Track();

    float render(double beat);
    inline bool onbeat(double beat, double midi_beat);

    void toggleRec();
    void addAudioEffect(AudioEffect* effect);

    void initInstrument(InstrumentBase* instr);

    void dragHandler(ndk_helper::Vec2 v) override ;
    void dragBegin(ndk_helper::Vec2 v, float xscale, float yscale) override ;
//    void addAudioEffect(AudioEffect* effect);
};


#endif //PD_TRACK_H
