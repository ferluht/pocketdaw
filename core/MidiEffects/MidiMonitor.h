//
// Created by ferluht on 14/04/2020.
//

#ifndef PD_MIDIMONITOR_H
#define PD_MIDIMONITOR_H


#include "MidiEffect.h"
#include <AMGEngine.h>

class MidiMonitor : public MidiEffect {

    std::list<MData> messages;

    const float text_height = 0.05;
    const float height_spacing = 0.1;
    const float view_height = 0.9;

public:

    MidiMonitor() : MidiEffect("MidiMonitor") {
        GSetRatio(0.55);
    }

    void MIn(MData cmd) override {
        MOut(cmd);
        messages.push_back(cmd);
        if (messages.size() > view_height / (text_height * (1 + height_spacing)) - 1)
            messages.pop_front();
    }

    void GDraw(NVGcontext * nvg) override {
        MidiEffect::GDraw(nvg);

        nvgBeginPath(nvg);
        nvgRect(nvg, global.c.x + 2, global.c.y + 2, global.s.x - 4, global.s.y - 4);
        nvgFillColor(nvg, BLACK);
        nvgFill(nvg);

//
//        nvgStrokeColor(nvg, BLACK);
//        nvgStroke(nvg);
//
//        nvgFontSize(nvg, global.s.y * 0.7);
//        nvgFontFace(nvg, "sans");
//        nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
//
//        nvgFillColor(nvg, BLACK);
//        char * label = labelOff;
//        if (state) label = labelOn;
//        nvgText(nvg, global.c.x + global.s.x/2, global.c.y + global.s.y/2, label, NULL);
//        nvgClosePath(nvg);

        nvgFontSize(nvg, text_height * global.s.y);
        nvgFontFace(nvg, "sans");
        nvgFillColor(nvg, WHITE);
        nvgTextAlign(nvg, NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
        nvgText(nvg, global.c.x + 5, global.c.y + global.s.y * 0.12, "beat   status   data1   data2", NULL);
        nvgClosePath(nvg);

        nvgBeginPath(nvg);
        nvgRect(nvg, global.c.x + 5, global.c.y + global.s.y * (0.1 + text_height), global.s.x - 10, 2);
        nvgFill(nvg);
        nvgClosePath(nvg);

        int i = 1;
        for (auto message : messages) {
            nvgFontSize(nvg, text_height * global.s.y);
            nvgFontFace(nvg, "sans");

            if ((message.status & 0xF0) == 0x80) nvgFillColor(nvg, RED);
            else if ((message.status & 0xF0) == 0x90) nvgFillColor(nvg, GREEN);
            else if ((message.status & 0xF0) == 0xB0) nvgFillColor(nvg, BLUE);
            else nvgFillColor(nvg, YELLOW);

            nvgTextAlign(nvg, NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
            char text[50];
            sprintf(text, "%.2f    0x%x    0x%x    %d", fmod(message.beat, 100), message.status, message.data1, message.data2);
            nvgText(nvg, global.c.x + 10, global.c.y + global.s.y * (0.13 + text_height * (1 + height_spacing) * i), text, NULL);
            nvgClosePath(nvg);
            i ++;
        }
    }
};


#endif //PD_MIDIMONITOR_H
