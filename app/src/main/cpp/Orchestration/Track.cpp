//
// Created by Admin on 12.06.2019.
//

#include "Track.h"

Track::Track():
Canvas(0.1, 0.1, 0.7, 0.7, "Textures/track_canvas.bmp", false)
{
    last_beat = 0;
//    MidiClip * mdc = createMetronomeMidi();
//    MidiClips.insert(MidiClips.begin(), mdc);

//    Arpeggiator * arp = new Arpeggiator();
//    MidiEffects.push_back(arp);
}

void Track::render(double beat, float * lsample, float * rsample)
{
    float sample = 0;

//    for (auto const& midiEffect : MidiEffects) midiEffect->apply(&MidiQueue, beat);

//    current_clip = nullptr;
//    auto closest_clip = MidiClips.lower_bound(beat);
//    if (closest_clip->second->end > beat) current_clip = closest_clip->second;
//    current_clip = MidiClips[0];
//    if (current_clip) current_clip->play(&MidiQueue, beat);

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

    TrackInstrument->render(beat, lsample, rsample);

    for (auto const& audioEffect : AudioEffects) audioEffect->apply(lsample, rsample);

    last_beat += beat;
}

void Track::toggleRec()
{
    rec = !rec;
}

void Track::initInstrument(InstrumentBase *instr)
{
    TrackInstrument = instr;
    instr->place(0.01, 0.05, 0.9, 0.3);
    attach(instr);
}

void Track::addAudioEffect(AudioEffect *effect)
{
    if (!AudioEffects.empty()) {
        effect->place(AudioEffects.back()->x + AudioEffects.back()->width + 0.01f, AudioEffects.back()->y, 0.9, 0.2);
    } else {
        effect->place(TrackInstrument->x + TrackInstrument->width + 0.01f, TrackInstrument->y, 0.9, 0.2);
    }
    AudioEffects.push_back(effect);
    attach(effect);
}

inline bool Track::onbeat(double beat, double midi_beat)
{
    return ((beat >= midi_beat) && (last_beat < beat));
}

void Track::dragHandler(const ndk_helper::Vec2& v) {
//    new_position.y = v.y_;
//    if (new_position.y > 0) position.y = 0;
//    update();
}

void Track::dragBegin(const ndk_helper::Vec2& v) {
//    drag_from = v;
//    new_position = position;
}