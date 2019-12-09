//
// Created by Admin on 13.09.2019.
//

#include "MoogFilter.h"

MoogFilter::MoogFilter() : AudioEffect(L"Filter")
{
    setRatio(0.45f);

    fs=48000.0;

    init();

    cutoff_enc = new Encoder(L"cutoff", 1, [this](float value) {
        setCutoff ((value + 1)*8000);
    }, 1);
    cutoff_enc->place(0.25, 0.1);
    cutoff_enc->setHeight(0.25);
    GAttach(cutoff_enc);
    MConnect(cutoff_enc);

    resonance_enc = new Encoder(L"resonance", -1, [this](float value) {
        setRes ((value + 1)/2);
    }, 1);
    resonance_enc->place(0.25, 0.4);
    resonance_enc->setHeight(0.25);
    GAttach(resonance_enc);
    MConnect(resonance_enc);
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

    if (!*isOn) return false;

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
    return *isOn;
}

float MoogFilter::getCutoff()
{ return cutoff; }

void MoogFilter::setCutoff(float c)
{ cutoff=c; calc(); }

float MoogFilter::getRes()
{ return res; }

void MoogFilter::setRes(float r)
{ res=r; calc(); }