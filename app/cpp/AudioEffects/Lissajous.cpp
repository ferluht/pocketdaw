//
// Created by Admin on 30.07.2019.
//

#include <GUI/Encoder.h>
#include <GUI/Text.h>
#include "Lissajous.h"

Lissajous::Lissajous(float n, float x_, float y_) : AudioEffect(x_,y_,0,0){

    window = n;

    attach(new Text("Fonts/Roboto-Regular.ttf", L"lissajous", 0.05, 0.05, 2, 0.11));

    attach(new Encoder(0, 0.05, 0.7, 2, 0.25, "Textures/encoder.bmp",
                       [this](float state){
                           this->window = (state/2 + 0.5f) * 200 + 1;
                       }));

    graph = new XYGraph(300, 0.5, 0.5, 3, 0.9, 0.9);
    attach(graph);
}

void Lissajous::apply(float *lsample, float *rsample) {
    graph->update(*lsample, *rsample);
}