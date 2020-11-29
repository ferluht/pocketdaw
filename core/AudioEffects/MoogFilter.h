//
// Created by Admin on 13.09.2019.
//

#ifndef PD_MOOGFILTER_H
#define PD_MOOGFILTER_H


#include <GUI/Encoder.h>
#include "AudioEffect.h"
#include "Filters/MoogRKSimulationModel.h"

class MoogFilter : public AudioEffect
{
    GUI::AnalogEncoder * cutoff_enc;
    GUI::AnalogEncoder * resonance_enc;
    GUI::AnalogEncoder * drive_enc;

public:
    MoogFilter();
    void init();
    void calc();
    float getCutoff();
    void setCutoff(float c);
    float getRes();
    void setRes(float r);
    bool ARender(double beat, float * lsample, float * rsample) override;
    void MRender(double beat) override;
protected:
    float cutoff;
    float res;
    float fs;
    float y1,y2,y3,y4;
    float oldx;
    float oldy1,oldy2,oldy3;
    float x;
    float r;
    float p;
    float k;
};


class RKMoogFilter : public AudioEffect
{

    RKSimulationMoog * fl, * fr;

public:

    GUI::AnalogEncoder * cutoff_enc;
    GUI::AnalogEncoder * resonance_enc;

    RKMoogFilter() : AudioEffect("Moog"){
        GSetRatio(0.2);

        fl = new RKSimulationMoog(sample_rate);
        fr = new RKSimulationMoog(sample_rate);

        cutoff_enc = new GUI::AnalogEncoder("cutoff", 1000, 0, 15000);
        cutoff_enc->GPlace({0.09, 0.1});
        cutoff_enc->GSetHeight(0.2);
        GAttach(cutoff_enc);
        MConnect(cutoff_enc);

        resonance_enc = new GUI::AnalogEncoder("resonance", 0, 0, 10);
        resonance_enc->GPlace({0.09, 0.4});
        resonance_enc->GSetHeight(0.2);
        GAttach(resonance_enc);
        MConnect(resonance_enc);
    }

    bool ARender(double beat, float * lsample, float * rsample) override {

        fl->SetCutoff(*cutoff_enc);
        fr->SetCutoff(*cutoff_enc);

        fl->SetResonance(*resonance_enc);
        fr->SetResonance(*resonance_enc);

        fl->Process(lsample, 1);
        fr->Process(rsample, 1);
        return true;
    }

    void MRender(double beat) override {
        cutoff_enc->MRender(beat);
        resonance_enc->MRender(beat);
    }
};


#endif //PD_MOOGFILTER_H
