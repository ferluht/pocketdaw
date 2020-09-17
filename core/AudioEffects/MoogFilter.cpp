//
// Created by Admin on 13.09.2019.
//

#include "MoogFilter.h"

MoogFilter::MoogFilter() : AudioEffect("Filter")
{
    GSetRatio(0.2);

    fs=48000.0;

    init();

    cutoff_enc = new GUI::AnalogEncoder("cutoff", 25, 0, 100, [this](float value) {
        setCutoff (value*value);
    }, 1);
    cutoff_enc->GPlace({0.09, 0.1});
    cutoff_enc->GSetHeight(0.2);
    GAttach(cutoff_enc);
    MConnect(cutoff_enc);

    resonance_enc = new GUI::AnalogEncoder("resonance", -1, -1, 1, [this](float value) {
        setRes ((value + 1)/2);
    }, 1);
    resonance_enc->GPlace({0.09, 0.4});
    resonance_enc->GSetHeight(0.2);
    GAttach(resonance_enc);
    MConnect(resonance_enc);

    drive_enc = new GUI::AnalogEncoder("drive", -1, -1, 1, [this](float value) {}, 1);
    drive_enc->GPlace({0.09, 0.7});
    drive_enc->GSetHeight(0.2);
    GAttach(drive_enc);
    MConnect(drive_enc);
}

void MoogFilter::MRender(double beat) {
    cutoff_enc->MRender(beat);
    resonance_enc->MRender(beat);
}

void MoogFilter::init()
{
// initialize values
    y1=y2=y3=y4=oldx=oldy1=oldy2=oldy3=0;
    calc();
};

void MoogFilter::calc()
{
    float f = (cutoff+cutoff) / fs; //[0 - 1]
    p=f*(1.8f-0.8f*f);
    k=p+p-1.f;

    float t=(1.f-p)*1.386249f;
    float t2=12.f+t*t;
    r = res*(t2+6.f*t)/(t2-6.f*t);
};

bool MoogFilter::ARender(double beat, float *lsample, float *rsample) {

    if (!enabled()) return false;

    float input = (*lsample + *rsample)/2;
// process input
    x = input - r*y4;

//Four cascaded onepole filters (bilinear transform)
    y1= x*p +  oldx*p - k*y1;
    y2=y1*p + oldy1*p - k*y2;
    y3=y2*p + oldy2*p - k*y3;
    y4=y3*p + oldy3*p - k*y4;

//Clipper band limited sigmoid
    y4-=(y4*y4*y4)/6.f;

    oldx = x; oldy1 = y1; oldy2 = y2; oldy3 = y3;
    *lsample = y4;
    *rsample = y4;
    return enabled();
}

float MoogFilter::getCutoff()
{ return cutoff; }

void MoogFilter::setCutoff(float c)
{ cutoff=c; calc(); }

float MoogFilter::getRes()
{ return res; }

void MoogFilter::setRes(float r)
{ res=r; calc(); }