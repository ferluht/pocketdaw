//
// Created by ferluht on 17/11/2020.
//

#ifndef POCKETDAW_LOOPER_H
#define POCKETDAW_LOOPER_H


#include <AudioEffects/AudioEffect.h>
#include <GUI/Graph.h>
#include "Saturator.h"
#include "Delay.h"
#include "ConvolutionReverb.h"
#include "PitchShifter.h"
#include "PitchTracker.h"
#include "MoogFilter.h"

class Looper : public AudioEffect {

    GUI::LooperButton * thebutton;
    GUI::Button * clearbutton;
    GUI::Button * syncbutton;
    GUI::Button * mutebutton;
    GUI::XYButton * satbutton;
    GUI::XYButton * revbutton;
    GUI::XYButton * delbutton;
    GUI::XYButton * filterbutton;
    GUI::Button * pitchbutton;


    Saturator * saturator;
    Delay * delay;
    ConvolutionReverb * reverb;
    PitchTracker * pitch;
    RKMoogFilter * filter;

    float s_phase = 0;

    enum LOOPER_STATE {
        STOP,
        PLAY,
        REC,
        OVERDUB
    };

    int looper_state = STOP;

    bool waitingforsync = false;

    unsigned long position = 0;

    std::vector<float> audio;

    float avg = 0, avg_env = 0;

    int tick_counter = 0;

    bool monitoring= false;

public:

    enum LOOPER_EVENT {
        RECORDING_START,
        RECORDING_END,
        OVERDUB_START,
        OVERDUB_END,
        LOOP_END,
        FOCUS,
        CLEAR
    };

    std::function<void(Looper *, LOOPER_EVENT event)> event_callback;

    static const float looper_ratio;

    Looper(std::function<void(Looper *, LOOPER_EVENT event)> event_callback_) : AudioEffect("Looper") {
        GSetRatio(looper_ratio);

        event_callback = event_callback_;

        thebutton = new GUI::LooperButton("loop", [](bool state){});
        thebutton->GPlace({0.01,0.01});
        thebutton->GSetWidth(0.98);

        thebutton->GSetTapEndCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
            thebutton->state = true;
            switch (looper_state) {
                case STOP:
                {
                    if (*syncbutton) {
                        waitingforsync = true;
                        thebutton->setFlashing(true, RED);
                        break;
                    }
                    looper_state = REC;
                    thebutton->setBodyColor(RED);
                    event_callback(this, LOOPER_EVENT::RECORDING_START);
                } break;
                case REC:
                {
                    if (*syncbutton) {
                        waitingforsync = true;
                        thebutton->setFlashing(true, BLUE);
                        break;
                    }
                    looper_state = OVERDUB;
                    thebutton->setBodyColor(BLUE);
                    event_callback(this, LOOPER_EVENT::RECORDING_END);
                } break;
                case OVERDUB:
                {
                    looper_state = PLAY;
                    thebutton->setBodyColor(SALAD);
                    event_callback(this, LOOPER_EVENT::OVERDUB_START);
                } break;
                case PLAY:
                {
                    looper_state = OVERDUB;
                    thebutton->setBodyColor(BLUE);
                    event_callback(this, LOOPER_EVENT::OVERDUB_END);
                } break;
                default:
                    break;
            }
            return nullptr;
        });

        GAttach(thebutton);

        thebutton->setArcColor(GREEN);
        thebutton->setBodyColor(GREY);

        clearbutton = new GUI::Button("clean", [this](bool state){
            clearbutton->state = false;
            looper_state = STOP;
            audio.clear();
            position = 0;
            avg = 0;
            avg_env = 0;
            thebutton->setPosition(0);
            thebutton->state = false;
//            thebutton->setBodyColor(GREY);
            event_callback(this, LOOPER_EVENT::CLEAR);
        });
        clearbutton->GPlace({0.01,0.45});
        clearbutton->GSetWidth(0.48);
        clearbutton->GSetHeight(0.1);
        GAttach(clearbutton);

        mutebutton = new GUI::Button("mute", [this](bool state){});
        mutebutton->GPlace({0.51,0.45});
        mutebutton->GSetWidth(0.48);
        mutebutton->GSetHeight(0.1);
        GAttach(mutebutton);

        syncbutton = new GUI::Button("sync", [this](bool state){});
        syncbutton->GPlace({0.01,0.55});
        syncbutton->GSetWidth(0.48);
        syncbutton->GSetHeight(0.1);
        GAttach(syncbutton);

        saturator = new Saturator();
        satbutton = new GUI::XYButton("satur", "shape", 0, 15, "mix", 0, 1,
                                      [this](float x, float y){
                                          *saturator->factor = x;
                                          *saturator->drywet = y;
                                      });
        satbutton->GPlace({0.51,0.55});
        satbutton->GSetWidth(0.48);
        satbutton->GSetHeight(0.1);
        GAttach(satbutton);

        delay = new Delay();
        *delay->drywet = 0.7;
        delbutton = new GUI::XYButton("delay", "time", 0, 3, "feedback", 0, 1,
                                      [this](float x, float y){
                                          *delay->delay_time = x;
                                          *delay->feedback = y;
                                      }, GUI::XYButton::SCALE::EXP, GUI::XYButton::SCALE::EXP);
        delbutton->GPlace({0.01,0.65});
        delbutton->GSetWidth(0.48);
        delbutton->GSetHeight(0.1);
        GAttach(delbutton);

        char path[200];
        sprintf(path, "%s%s", AEngine::getAEngine().storage_root, "/.pocketdaw/impulse_response_left.wav");
        reverb = new ConvolutionReverb(path);
        revbutton = new GUI::XYButton("reverb", "size", 0, 15, "mix", 0, 1,
                                      [this](float x, float y){
                                          *reverb->drywet = y;
                                      });
        revbutton->GPlace({0.51,0.65});
        revbutton->GSetWidth(0.48);
        revbutton->GSetHeight(0.1);
        GAttach(revbutton);

//        pitch = new PitchTracker(500, 1000);
//        pitch->GPlace({0.2, 0.01});
//        pitch->GSetHeight(0.98);
//        GAttach(pitch);

        pitchbutton = new GUI::Button("pitch", [this](bool state){});
        pitchbutton->GPlace({0.01,0.75});
        pitchbutton->GSetWidth(0.48);
        pitchbutton->GSetHeight(0.1);
        GAttach(pitchbutton);


        filter = new RKMoogFilter();

        filterbutton = new GUI::XYButton("filter", "cutoff", 0, 10000, "resonanse", 0, 5,
                                         [this](float x, float y){
                                             *filter->cutoff_enc = x;
                                             *filter->resonance_enc = y;
                                         }, GUI::XYButton::SCALE::EXP);
        filterbutton->GPlace({0.51,0.75});
        filterbutton->GSetWidth(0.48);
        filterbutton->GSetHeight(0.1);
        GAttach(filterbutton);
    }

    inline float envelope(float sample, float w, float w_env){
        avg = w*sample + (1-w)*avg;
        float i = std::abs(sample - avg);
        avg_env = w_env*i + (1-w_env)*avg_env;
        return avg_env;
    }

    bool sync_tick() {

        if (waitingforsync) switch (looper_state) {
            case STOP:
            {
                looper_state = REC;
                thebutton->setBodyColor(RED);
            } break;
            case REC:
            {
                looper_state = OVERDUB;
                thebutton->setBodyColor(BLUE);
            } break;
            default:
                break;
        }

        waitingforsync = false;
        thebutton->setFlashing(false);

//        tick_counter = (tick_counter + 1) % *sync_division;

        return true;
    }

    GObject *GFindFocusObject(const vecmath::Vec2 &point, std::list<GObject *> * trace) override {
        event_callback(this, LOOPER_EVENT::FOCUS);
        return GUI::AMGCanvas::GFindFocusObject(point, trace);
    }

    void setMonitoring(bool monitoring_) {
        monitoring = monitoring_;
    }

    bool ARender(double beat, float * lsample, float * rsample) override {

//        if (*pitchbutton) {
//
//            s_phase += (2 * M_PI / (AObject::sample_rate / 602.f));
//            if (s_phase > (2 * M_PI)) s_phase = 0;
//            *lsample = sin(s_phase);
//            *rsample = sin(s_phase);
//
//            pitch->ARender(beat, lsample, rsample);
//            sprintf(pitchbutton->labelOn, "%.2f", pitch->freq);
//        }
        if (*satbutton) saturator->ARender(beat, lsample, rsample);
        if (*delbutton) delay->ARender(beat, lsample, rsample);
        if (*revbutton) reverb->ARender(beat, lsample, rsample);

        switch (looper_state) {
            case STOP:
                {
                    if (!monitoring) {
                        *lsample = 0;
                        *rsample = 0;
                    }
                }
                break;
            case REC:
                {
                    audio.push_back(*lsample);
                } break;
            case OVERDUB:
                {
                    int position_latency = (int)position - latency_samples;
                    if (position_latency < 0) position_latency += audio.size();
                    float env = envelope(*lsample * 10, 0.01, 0.01) * 2;
                    if (env > 1) env = 1;
                    if (env < 0) env = 0;
                    audio[position_latency] = (audio[position_latency] * (1 - env) + *lsample * env);
                    *lsample = (audio[position] * (1 - env) + *lsample * env);
                    *rsample = (audio[position] * (1 - env) + *rsample * env);
                    position = (position + 1) % audio.size();
                    thebutton->setPosition((float) (position) / (float) (audio.size()));
                } break;
            case PLAY:
                {
                    if (audio.size() < 1) break;
                    *lsample = (audio[position] * (1 - *lsample) + *lsample);
                    *rsample = (audio[position] * (1 - *rsample) + *rsample);
                    position = (position + 1) % audio.size();
                    thebutton->setPosition((float) (position) / (float) (audio.size()));
                    if (position == 0) event_callback(this, LOOPER_EVENT::LOOP_END);
                } break;
            default:
                break;
        }

        if (*mutebutton) {
            *lsample = 0;
            *rsample = 0;
        }

        if (*filterbutton) filter->ARender(beat, lsample, rsample);

        return true;
    }

//    void GDraw(NVGcontext *nvg) override {
//        AudioEffect::GDraw(nvg);
//        nvgSave(nvg);
//        nvgBeginPath(nvg);
//
//
//
//        nvgClosePath(nvg);
//        nvgRestore(nvg);
//    }

};


#endif //ANDROID_LOOPER_H
