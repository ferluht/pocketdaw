//
// Created by Admin on 13.08.2019.
//

#include "Operator.h"

Operator::Operator(unsigned int num_voices) : Instrument<OperatorState>(num_voices, L"Operator"){
    GAttachTexture("Textures/effect_canvas.bmp");
    setRatio(1);

    const wchar_t * names[] = {L"A", L"B", L"C", L"D"};

    for (int i = 0; i < 4; i++) {
        sines[i] = new Sine(names[i], 1);
        sines[i]->place(0.03, 0.13);
        sines[i]->setWidth(0.94);
        sines[i]->setRatio(0.94/0.44);
        GAttach(sines[i]);
        sines[i]->GSetVisible(false);
    }

    op_focus = 0;
    sines[op_focus]->GSetVisible(true);
    MConnect(sines[op_focus]);


    graph = new TimeGraph(200);
    graph->place(0.03, 0.6);
    graph->setHeight(0.4);
    graph->setWidth(0.94);
    GAttach(graph);
    graph_phase = 0;


    enc_mode = new Encoder(L"mode", -1, [this](float value) {
        mode = (int) (2.2 * (value + 1));
    }, 4);
    enc_mode->place(0.05, 0.8);
    enc_mode->setHeight(0.2);
    GAttach(enc_mode);
    MConnect(enc_mode);


    mode = 0;

    level = 1;
}

void Operator::GSetVisible(bool visible_) {
    sines[op_focus]->GSetVisible(true);
    sines[op_focus]->sinename->GSetVisible(true);
}

void Operator::IUpdateState(OperatorState *state, MData md){
    if (md.data2 != 0) state->setActive(true);
    state->note = md.data1;
    for (int i = 0; i < 4; i++){
        sines[i]->IUpdateState(&state->sinestates[i], md);
    }
}

void Operator::MIn(MData cmd) {
    Instrument<OperatorState>::MIn(cmd);
    if (cmd.status == 0xb0 && cmd.data2 == 0){
        switch (cmd.data1){
            case 0x15:
                MDisconnect(sines[op_focus]);
                sines[op_focus]->GSetVisible(false);
                sines[op_focus]->sinename->GSetVisible(false);
                op_focus ++;
                if (op_focus > 3) op_focus = 3;
                MConnect(sines[op_focus]);
                sines[op_focus]->GSetVisible(true);
                sines[op_focus]->sinename->GSetVisible(true);
                break;
            case 0x16:
                MDisconnect(sines[op_focus]);
                sines[op_focus]->GSetVisible(false);
                sines[op_focus]->sinename->GSetVisible(false);
                op_focus --;
                if (op_focus < 0) op_focus = 0;
                MConnect(sines[op_focus]);
                sines[op_focus]->GSetVisible(true);
                sines[op_focus]->sinename->GSetVisible(true);
                return;
            default:
                break;
        }
    }
    if ((cmd.status != NOTEON_HEADER) || cmd.data2 != 0) graph_phase = 0;
}

bool Operator::ARender(double beat, float *lsample, float *rsample) {
    Instrument<OperatorState>::ARender(beat, lsample, rsample);
    if (graph_phase < 200) {
        graph->update(*lsample);
        graph_phase ++;
    }
    return true;
}

void Operator::IARender(OperatorState * state, double beat, float * lsample, float * rsample)
{
    float sample = 0;
    float a = 0, b = 0, c = 0, d = 0;

    sines[0]->IARender(&state->sinestates[0], beat, &a, &a);
    sines[1]->IARender(&state->sinestates[1], beat, &b, &b);
    sines[2]->IARender(&state->sinestates[2], beat, &c, &c);
    sines[3]->IARender(&state->sinestates[3], beat, &d, &d);

    switch (mode){
        case 0:
            sample = (a + b + c + d) / (sines[0]->level + sines[1]->level + sines[2]->level + sines[3]->level);
            break;
        case 1:
            sample = (a + b + c * d) / (sines[0]->level + sines[1]->level + sines[2]->level * sines[3]->level);
            break;
        case 2:
            sample = (a + b * c * d) / (sines[0]->level + sines[1]->level * sines[2]->level * sines[3]->level);
            break;
        case 3:
            sample = (a * b * c * d) / (sines[0]->level * sines[1]->level * sines[2]->level * sines[3]->level);
            break;
        default:
            break;
    }

    sample *= level;

    state->setActive(state->sinestates[0].active || state->sinestates[1].active || state->sinestates[2].active || state->sinestates[3].active);

    *lsample += sample;
    *rsample += sample;
}