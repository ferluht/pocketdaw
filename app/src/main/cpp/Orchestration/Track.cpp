//
// Created by Admin on 12.06.2019.
//

#include <AudioEffects/Waveform.h>
#include <GUI/Menu.h>
#include <AudioEffects/StereoDelay.h>
#include <AudioEffects/Lissajous.h>
#include "Track.h"

Track::Track():
Canvas(0.1, 0.1, 0.7, 0.7, "Textures/track_canvas.bmp", false)
{
    last_beat = 0;


    std::vector<std::pair<wchar_t *, std::function<void(void)>>> items;
    items.push_back({L"add delay", [this](){addAudioEffect(new Delay(0.2, 0.09));}});
    items.push_back({L"add stereo delay", [this](){addAudioEffect(new StereoDelay(0.7));}});
    items.push_back({L"add waveform", [this](){addAudioEffect(new Waveform(200, 0, 0));}});
    items.push_back({L"add lissajous", [this](){addAudioEffect(new Lissajous(200, 0, 0));}});
    trackMenu = new Menu(items);
    trackMenu->place(0.05, 0.05, 0.9, 0.9);
    trackMenu->setVisible(false);
    trackMenu->attachTo(this);

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

void Track::receiveMIDI(MidiData md)
{
    if (focusObject) {
        focusObject->receiveMIDI(md);
    }

    if (md.status == 0xb0){
        switch (md.data1){
            case 0x13:
                for (auto const& gr : Graphics) gr->setVisible(false);
                focusObject = trackMenu;
                trackMenu->setVisible(true);
                break;
            case 0x14:
                for (auto const& gr : Graphics) gr->setVisible(true);
                focusObject = nullptr;
                trackMenu->setVisible(false);
                break;
            default:
                break;
        }
    }

    MidiQueue.push(md);
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