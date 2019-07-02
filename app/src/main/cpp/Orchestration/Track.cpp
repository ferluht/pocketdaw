//
// Created by Admin on 12.06.2019.
//

#include "Track.h"
#include "../MidiEffects/Arpeggiator.h"

Track::Track()
{
    last_beat = 0;
    MidiClip * mdc = createMetronomeMidi();
    MidiClips.insert(MidiClips.begin(), mdc);

    Arpeggiator * arp = new Arpeggiator();
    MidiEffects.push_back(arp);
}

float Track::render(double beat)
{
    float sample = 0;

//    for (auto const& midiEffect : MidiEffects) midiEffect->apply(&MidiQueue, beat);

//    current_clip = nullptr;
//    auto closest_clip = MidiClips.lower_bound(beat);
//    if (closest_clip->second->end > beat) current_clip = closest_clip->second;
    current_clip = MidiClips[0];
    if (current_clip) current_clip->play(&MidiQueue, beat);

    for (auto const& midiEffect : MidiEffects) midiEffect->apply(&MidiQueue, beat);

    while (!MidiQueue.empty()){
        MidiData command = MidiQueue.top();
        TrackInstrument->midiCommand(command);

//        if (rec){
//            if (current_clip == nullptr){
//                current_clip = new MidiClip;
////                addMidiClip(current_clip, beat);
//            }
////            command
//        }

        MidiQueue.pop();
    }

    sample = TrackInstrument->render(beat);

    for (auto const& audioEffect : AudioEffects) sample = audioEffect->apply(sample);

    last_beat += beat;
    return sample;
}

void Track::toggleRec()
{
    rec = !rec;
}

void Track::addAudioEffect(AudioEffect *effect)
{
    AudioEffects.push_back(effect);
}

inline bool Track::onbeat(double beat, double midi_beat)
{
    return ((beat >= midi_beat) && (last_beat < beat));
}