//
// Created by Admin on 13.06.2019.
//

#include <GUI/Encoder.h>
#include "Arpeggiator.h"

Arpeggiator::Arpeggiator() : Arpeggiator(1) {}

Arpeggiator::Arpeggiator(double scale_)
{
    GAttachTexture("Textures/effect_canvas.bmp");

    auto enc = new Encoder(L"scale", 0, [this](float value){
        this->scale = value/2 + 0.5f;
    }, 5);
    enc->place(0.02, 0.65, 0.25, 0.25);
    GAttach(enc);
    MConnect(enc);

    scale = scale_;
    last_played_note = 0;
    cycles = 0;
}

void Arpeggiator::MIn(MData cmd) {
    if ((cmd.status & 0xF0) == NOTEON_HEADER) {
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
        cycles = (int)(beat / scale);
    }
}