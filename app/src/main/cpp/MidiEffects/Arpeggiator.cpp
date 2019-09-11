//
// Created by Admin on 13.06.2019.
//

#include <GUI/Encoder.h>
#include "Arpeggiator.h"

Arpeggiator::Arpeggiator() : Arpeggiator(1) {}

Arpeggiator::Arpeggiator(double scale_)
{
    GAttachTexture("Textures/effect_canvas.bmp");
    setRatio(0.5);

    auto opname = new Text("Fonts/Roboto-Regular.ttf", L"Arpeggiator");
    opname->place(0.03, 0.03);
    opname->setHeight(0.05);
    GAttach(opname);

    auto enc_release = new Encoder(L"rate", 0, [this](float value) {
//        this->scale = (value + 1)/2 * 10;
    }, 8);
    enc_release->place(0.2, 0.2);
    enc_release->setHeight(0.4);
    GAttach(enc_release);
    MConnect(enc_release);

    scale = scale_;
    last_played_note = 0;
    cycles = 0;
    isplaying = false;
    last_played_beat = 0;
    gate = 0.4;
}

void Arpeggiator::MIn(MData cmd) {
    if (((cmd.status & 0xF0) == NOTEON_HEADER) || ((cmd.status & 0xF0) == NOTEOFF_HEADER)) {
        if (cmd.data2) notes.insert({cmd.data1, cmd});
        else notes.erase(cmd.data1);
    } else {
        MOut(cmd);
    }
}

void Arpeggiator::MRender(double beat) {
    if (!notes.empty() && (int)(beat / scale) > cycles){
        auto i = notes.upper_bound(last_played_note);
        if (i == notes.end()) i = notes.begin();
        MData md = i->second;
        md.beat = beat;
        MOut(md);
        last_played_note = md.data1;
        last_played_beat = beat;
        cycles = (int)(beat / scale);
        isplaying = true;
    }
    if ( isplaying && ((beat - last_played_beat) / scale > gate)){
        MData md;
        md.status = 0x80;
        md.data1 = last_played_note;
        md.data2 = 0;
        md.beat = beat;
        MOut(md);
        isplaying = false;
    }
}