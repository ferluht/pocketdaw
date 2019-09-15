//
// Created by Admin on 12.06.2019.
//

//#include <AudioEffects/Oscilloscope.h>
//#include <GUI/Menu.h>
//#include <AudioEffects/StereoDelay.h>
//#include <AudioEffects/Lissajous.h>
#include <chrono>
#include "Track.h"

GObject * AMGChain::GDragHandler(const ndk_helper::Vec2 &v) {

    auto old_moving_to = moving_to;

    while ((moving_to != AMGObjects.begin()) && (v.x_ < (*moving_to)->globalPosition.x)){
        moving_to --;
    }
    while ((moving_to != AMGObjects.end() - 2) && (v.x_ > (*moving_to)->globalPosition.x + (*moving_to)->globalPosition.width)){
        moving_to ++;
    }

    if (!(*moving_to)->globalPosition.contains(v)) moving_overlay->GSetVisible(false);

    if (old_moving_to != moving_to){
        auto position = (*moving_to)->globalPosition.toRelative(this->globalPosition);
        moving_overlay->place(position.x, position.y, (*moving_to)->z);
        moving_overlay->setHeight(position.height);
        moving_overlay->setWidth(position.width);
        moving_overlay->GSetVisible(true);
    }

    last_touch = v;

    return this;
}

GObject * AMGChain::GDragBegin(const ndk_helper::Vec2 &v) {
    auto go = AMGObject::GFindFocusObject(v);
    moving_from = std::find(AMGObjects.begin(), AMGObjects.end(), go);
    moving_to = moving_from;
//    moving_overlay->GSetVisible(true);
    return this;
}

GObject * AMGChain::GDragEnd(const ndk_helper::Vec2 &v) {
    if ((*moving_to)->globalPosition.contains(last_touch)){
        auto go = (*moving_from);
        AMGChainDel(moving_from - AMGObjects.begin());
        AMGChainInsert(go, moving_to - AMGObjects.begin());
    }
    moving_overlay->GSetVisible(false);
    return nullptr;
}

bool AMGRack::ARender(double beat, float *lsample, float *rsample){
//    MEffects.MRender(beat);
//    Instr->MRender(beat);
    Instr->ARender(beat, lsample, rsample);
//    AEffects.MRender(beat);
    AEffects.ARender(beat, lsample, rsample);
    return true;
}

bool AMGMasterTrack::ARender(float *audioData, int numFrames) {

    double end_beat = beat;

    if (isPlaying) {
        ableton::Link::SessionState state = link.captureAppSessionState();
        bpm = state.tempo();
        std::chrono::microseconds time = link.clock().micros();
        std::chrono::microseconds second{1000000};
        std::chrono::microseconds lag = std::chrono::duration_cast<std::chrono::microseconds> ((second * numFrames) / sample_rate);
        phase = state.phaseAtTime(time + lag, size_denominator);
        beat = state.beatAtTime(time + lag, size_denominator);
    }

    linkButton->progress(phase/size_denominator);

    double increment = bpm / 60.0 / sample_rate;// + (beat - end_beat)/numFrames;

//    MData cmd;
//    cmd.status = 0x80;
//    cmd.data1 = 50;
//    cmd.data2 = 100;
//    MOut(cmd);

    for (int i = 0; i < numFrames; i++) {
        audioData[2*i] = 0;
        audioData[2*i+1] = 0;

        MRender(beat);

        if (*metronome_button) {
            if ((int)phase > (int)last_phase) metronome->tic();
            else if ((int)phase < (int)last_phase) metronome->tac();
            last_phase = phase;
            metronome->ARender(beat, &audioData[2*i], &audioData[2*i + 1]);
        }

        for (auto const& track : Tracks) {
            float l = 0, r = 0;
            track->ARender(beat, &l, &r);
            audioData[2*i] += l;
            audioData[2*i + 1] += r;
        }
        AEffects.ARender(beat, &audioData[2*i], &audioData[2*i + 1]);
        beat += increment;
        phase = fmod(phase + increment, size_denominator);

        masterWaveform->ARender(beat, &audioData[2*i], &audioData[2*i + 1]);
    }

    return true;
}

GObject * AMGTrack::GDragHandler(const ndk_helper::Vec2 &v) {
    x_offset += (v.x_ - drag_from.x_)/globalPosition.width;
    if ((MEffects.ratio + Instr->ratio + AEffects.ratio)/globalPosition.ratio + x_offset + 2*padding < 1)
        x_offset = 1 - (MEffects.ratio + Instr->ratio + AEffects.ratio)/globalPosition.ratio - 2*padding;
    if (x_offset > 0) x_offset = 0;
    Rearrange();
    drag_from = v;
    return this;
}

GObject * AMGTrack::GDragBegin(const ndk_helper::Vec2 &v) {
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