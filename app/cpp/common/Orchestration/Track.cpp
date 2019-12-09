//
// Created by Admin on 12.06.2019.
//

//#include <AudioEffects/Oscilloscope.h>
//#include <GUI/Menu.h>
//#include <AudioEffects/StereoDelay.h>
//#include <AudioEffects/Lissajous.h>
#include <chrono>
#include "Track.h"

GUI::GObject * AMGTrack::GDragHandler(const ndk_helper::Vec2 &v) {
    x_offset += (v.x - drag_from.x)/shape->global.s.x;
    if ((MEffects.shape->local.ratio + Instr->shape->local.ratio + AEffects.shape->local.ratio)/shape->global.ratio + x_offset + 2*padding < 1)
        x_offset = 1 - (MEffects.shape->local.ratio + Instr->shape->local.ratio + AEffects.shape->local.ratio)/shape->global.ratio - 2*padding;
    if (x_offset > 0) x_offset = 0;
//    changed = true;
    drag_from = v;
    return this;
}

GUI::GObject * AMGTrack::GDragBegin(const ndk_helper::Vec2 &v) {
    drag_from = v;
    return this;
}

//AMGTrack::AMGTrack()
//{
//    std::vector<std::pair<wchar_t *, std::function<void(void)>>> items;
////    items.push_back({L"add delay", [this](){AAddEffect(new Delay(0.2, 0.09));}});
////    items.push_back({L"add stereo delay", [this](){AAddEffect(new StereoDelay(0.7));}});
////    items.push_back({L"add waveform", [this](){AAddEffect(new Waveform(200, 0, 0));}});
////    items.push_back({L"add lissajous", [this](){AAddEffect(new Lissajous(200, 0, 0));}});
////    trackMenu = new Menu(items);
////    trackMenu->place(0.05, 0.05, 0.9, 0.9);
////    trackMenu->GSetVisible(false);
////    trackMenu->GAttachTo(this);
//
////    MidiClip * mdc = createMetronomeMidi();
////    MidiClips.insert(MidiClips.begin(), mdc);
//
////    Arpeggiator * arp = new Arpeggiator();
////    MidiEffects.push_back(arp);
//}

//void Track::toggleRec()
//{
//    rec = !rec;
//}
//
//void Track::initInstrument(InstrumentBase *instr)
//{
//    TrackInstrument = instr;
//    instr->place(0.01, 0.05, 0.9, 0.3);
//    attach(instr);
//}
//
//void Track::addAudioEffect(AudioEffect *effect)
//{
//    if (!AudioEffects.empty()) {
//        effect->place(AudioEffects.back()->x + AudioEffects.back()->width + 0.01f, AudioEffects.back()->y, 0.9, 0.2);
//    } else {
//        effect->place(TrackInstrument->x + TrackInstrument->width + 0.01f, TrackInstrument->y, 0.9, 0.2);
//    }
//    AudioEffects.push_back(effect);
//    attach(effect);
//}
//
//inline bool Track::onbeat(double beat, double midi_beat)
//{
//    return ((beat >= midi_beat) && (last_beat < beat));
//}
//
//void Track::MIn(MData cmd){
//    if (focusObject) {
//        focusObject->MIn(cmd);
//    }
//
//    if (cmd.status == 0xb0){
//        switch (cmd.data1){
//            case 0x13:
//                for (auto const& gr : Graphics) gr->setVisible(false);
//                focusObject = trackMenu;
//                trackMenu->setVisible(true);
//                break;
//            case 0x14:
//                for (auto const& gr : Graphics) gr->setVisible(true);
//                focusObject = nullptr;
//                trackMenu->setVisible(false);
//                break;
//            default:
//                break;
//        }
//    }
//
//    MidiQueue.push(cmd);
//}
//
//void Track::dragHandler(const ndk_helper::Vec2& v) {
////    new_position.y = v.y_;
////    if (new_position.y > 0) position.y = 0;
////    update();
//}
//
//void Track::dragBegin(const ndk_helper::Vec2& v) {
////    drag_from = v;
////    new_position = position;
//}