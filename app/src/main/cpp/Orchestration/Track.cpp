//
// Created by Admin on 12.06.2019.
//

#include "Track.h"

Track::Track():
Canvas(0, 0, 500, 700, "Textures/effect_canvas.bmp")
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

void Track::initInstrument(InstrumentBase *instr)
{
    TrackInstrument = instr;
//    instr->position.x = position.x;
//    instr->position.y = position.y;
//    instr->position.height = position.height * 0.2f;
//    instr->position.width = position.width;
    attach(instr);
}

void Track::addAudioEffect(AudioEffect *effect)
{
//    effect->position.x = position.x;
//    effect->position.y = TrackInstrument->position.y + TrackInstrument->position.height + 0.01f;
//    if (!AudioEffects.empty()) {
//        effect->position.x = AudioEffects.back()->position.x;
//        effect->position.y = AudioEffects.back()->position.y + AudioEffects.back()->position.height + 0.01f;
//    }
//    effect->position.height = position.height * 0.1f;
//    effect->position.width = position.width;
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