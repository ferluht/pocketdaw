//
// Created by ferluht on 18/04/2020.
//

#include "Saturator.h"


Saturator::Saturator() : AudioEffect("Saturator") {
    GSetRatio(0.3f);

    drywet = new GUI::AnalogEncoder("dry/wet", -1);
    drywet->GPlace({0.25, 0.4});
    drywet->GSetHeight(0.2);
    GAttach(drywet);
    MConnect(drywet);

    plot = new GUI::Plot<GUI::TimeGraph>(30);
    plot->GPlace({0.01, 0.01});
    plot->GSetWidth(0.98);
    plot->GSetHeight(0.98*0.3);
    GAttach(plot);

    type = 0;

    factor = new GUI::AnalogEncoder("factor", 1, 1, 15, [this](float value){
//        redraw_wave();
    });
    factor->GPlace({0.25, 0.7});
    factor->GSetHeight(0.2);
    GAttach(factor);
    MConnect(factor);

    redraw_wave();

    plot->GSetTapEndCallback([this](const Vec2& v) -> GUI::GObject * {
        type ++;
        if (type > 3) type = 0;
        redraw_wave();
        return nullptr;
    });
}

void Saturator::redraw_wave() {
    for (float amp = -1; amp < 1; amp += 2./30.) {
        plot->update(shaper(amp));
    }
}

bool Saturator::ARender(double beat, float *lsample, float *rsample){
    if(enabled()) {
        float prop = (*drywet + 1)/2;
        *lsample = shaper(*lsample) * prop + *lsample * (1 - prop);
        *rsample = shaper(*rsample) * prop + *rsample * (1 - prop);
    }
    return enabled();
}