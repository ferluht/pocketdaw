//
// Created by Admin on 13.08.2019.
//

#include "Operator.h"

Operator::Operator(unsigned int num_voices) : Instrument<OperatorState>(num_voices, L"Operator"){
    GAttachTexture("Textures/effect_canvas.bmp");
    setRatio(1);

    const wchar_t * names[] = {L"A", L"B", L"C", L"D"};

    for (int i = 0; i < 4; i++) {
        sines[i] = new Oscillator(names[i], 1);
        sines[i]->place(0.01, 0.01);
        sines[i]->setWidth(0.98);
        sines[i]->NoHeader();
        GAttach(sines[i]);
        sines[i]->GSetVisible(false);
    }

    op_focus = 0;
    sines[op_focus]->GSetVisible(true);
    MConnect(sines[op_focus]);

    algo = new TexturedMultiButton([this](unsigned int value){setAlgo(value);}, 11, algos);
    algo->place(0.01, 0.6);
    algo->setHeight(0.38);
    GAttach(algo);
    MConnect(algo);

    mode = 0;
    setAlgo(0);

    level = 1;
}

void Operator::setAlgo(unsigned int value) {
    mode = value;
    for (int i = 0; i < 4; i++) {
        sines[i]->OResetPhaseModulator();
        sines[i]->OResetAmpModulator();
    }
    switch (value) {
        case 0:
            sines[0]->OSetPhaseModulator(*sines[1]);
            sines[1]->OSetPhaseModulator(*sines[2]);
            sines[2]->OSetPhaseModulator(*sines[3]);
            break;
        case 1:
            sines[1]->OSetPhaseModulator(&c);
            sines[0]->OSetPhaseModulator(*sines[1]);
            break;
        case 2:
            sines[1]->OSetPhaseModulator(*sines[2]);
            sines[0]->OSetPhaseModulator(&b);
        case 3:
            sines[1]->OSetPhaseModulator(*sines[3]);
            sines[2]->OSetPhaseModulator(*sines[3]);
            sines[0]->OSetPhaseModulator(&b);
        case 4:
            sines[2]->OSetPhaseModulator(*sines[3]);
            sines[1]->OSetPhaseModulator(*sines[2]);
            sines[0]->OSetPhaseModulator(*sines[2]);
        case 5:
            sines[2]->OSetPhaseModulator(*sines[3]);
            sines[1]->OSetPhaseModulator(*sines[2]);
        case 6:
            sines[0]->OSetPhaseModulator(&b);
        case 7:
            sines[2]->OSetPhaseModulator(*sines[3]);
            sines[0]->OSetPhaseModulator(*sines[1]);
        case 8:
            sines[0]->OSetPhaseModulator(*sines[3]);
            sines[1]->OSetPhaseModulator(*sines[3]);
            sines[2]->OSetPhaseModulator(*sines[3]);
        case 9:
            sines[2]->OSetPhaseModulator(*sines[3]);
        default:
            break;
    }
}

void Operator::GSetVisible(bool visible_) {
    IECanvas::GSetVisible(visible_);
    for (int i = 0; i < 4; i++) if (i != op_focus) sines[i]->GSetVisible(false);
}

void Operator::IUpdateState(OperatorState *state, MData md){
    state->note = md.data1;
    for (int i = 0; i < 4; i++){
        if (state->isActive()) state->sinestates[i].setActive(true);
        sines[i]->IUpdateState(&state->sinestates[i], md);
    }
    if (md.data2 != 0) state->setActive(true);
}

GObject * Operator::GTapEnd(const ndk_helper::Vec2& v)
{
    if (visible && sines[op_focus]->globalPosition.contains(v)){
        nextOsc();
    }
    return nullptr;
}

void Operator::prevOsc() {
    MDisconnect(sines[op_focus]);
    sines[op_focus]->GSetVisible(false);
    op_focus --;
    if (op_focus < 0) op_focus = 3;
    MConnect(sines[op_focus]);
    sines[op_focus]->GSetVisible(true);
}

void Operator::nextOsc() {
    MDisconnect(sines[op_focus]);
    sines[op_focus]->GSetVisible(false);
    op_focus ++;
    if (op_focus > 3) op_focus = 0;
    MConnect(sines[op_focus]);
    sines[op_focus]->GSetVisible(true);
}

void Operator::MIn(MData cmd) {
    Instrument<OperatorState>::MIn(cmd);
    if (cmd.status == 0xb0 && cmd.data2 == 0){
        switch (cmd.data1){
            case 0x13:
                nextOsc();
                break;
            case 0x14:
                prevOsc();
                return;
            default:
                break;
        }
    }
    if ((cmd.status != NOTEON_HEADER) || cmd.data2 != 0) graph_phase = 0;
}

void Operator::IARender(OperatorState * state, double beat, float * lsample, float * rsample)
{
    float sample = 0;
    a = 0, b = 0, c = 0, d = 0;

    switch (mode){
        case 0:
            sines[3]->IARender(&state->sinestates[3], beat, &d, &d);
            sines[2]->IARender(&state->sinestates[2], beat, &c, &c);
            sines[1]->IARender(&state->sinestates[1], beat, &b, &b);
            sines[0]->IARender(&state->sinestates[0], beat, &a, &a);
            sample = a;
            break;
        case 1:
            sines[3]->IARender(&state->sinestates[3], beat, &d, &d);
            sines[2]->IARender(&state->sinestates[2], beat, &c, &c);
            c = (*sines[3] + *sines[2])/2;
            sines[1]->IARender(&state->sinestates[1], beat, &b, &b);
            sines[0]->IARender(&state->sinestates[0], beat, &a, &a);
            sample = a;
            break;
        case 2:
            sines[3]->IARender(&state->sinestates[3], beat, &d, &d);
            sines[2]->IARender(&state->sinestates[2], beat, &c, &c);
            sines[1]->IARender(&state->sinestates[1], beat, &b, &b);
            b = (*sines[3] + *sines[1])/2;
            sines[0]->IARender(&state->sinestates[0], beat, &a, &a);
            sample = a;
            break;
        case 3:
            sines[3]->IARender(&state->sinestates[3], beat, &d, &d);
            sines[2]->IARender(&state->sinestates[2], beat, &c, &c);
            sines[1]->IARender(&state->sinestates[1], beat, &b, &b);
            b = (*sines[3] + *sines[2])/2;
            sines[0]->IARender(&state->sinestates[0], beat, &a, &a);
            sample = a;
            break;
        case 4:
            sines[3]->IARender(&state->sinestates[3], beat, &d, &d);
            sines[2]->IARender(&state->sinestates[2], beat, &c, &c);
            sines[1]->IARender(&state->sinestates[1], beat, &b, &b);
            sines[0]->IARender(&state->sinestates[0], beat, &a, &a);
            sample = (a + b) / 2;
            break;
        case 5:
            sines[3]->IARender(&state->sinestates[3], beat, &d, &d);
            sines[2]->IARender(&state->sinestates[2], beat, &c, &c);
            sines[1]->IARender(&state->sinestates[1], beat, &b, &b);
            sines[0]->IARender(&state->sinestates[0], beat, &a, &a);
            sample = (a + b) / 2;
            break;
        case 6:
            sines[3]->IARender(&state->sinestates[3], beat, &d, &d);
            sines[2]->IARender(&state->sinestates[2], beat, &c, &c);
            sines[1]->IARender(&state->sinestates[1], beat, &b, &b);
            b = (b + c + d) / 3;
            sines[0]->IARender(&state->sinestates[0], beat, &a, &a);
            sample = a;
            break;
        case 7:
            sines[3]->IARender(&state->sinestates[3], beat, &d, &d);
            sines[2]->IARender(&state->sinestates[2], beat, &c, &c);
            sines[1]->IARender(&state->sinestates[1], beat, &b, &b);
            sines[0]->IARender(&state->sinestates[0], beat, &a, &a);
            sample = (a + b) / 2;
            break;
        case 8:
            sines[3]->IARender(&state->sinestates[3], beat, &d, &d);
            sines[2]->IARender(&state->sinestates[2], beat, &c, &c);
            sines[1]->IARender(&state->sinestates[1], beat, &b, &b);
            sines[0]->IARender(&state->sinestates[0], beat, &a, &a);
            sample = (a + b + c) / 3;
            break;
        case 9:
            sines[3]->IARender(&state->sinestates[3], beat, &d, &d);
            sines[2]->IARender(&state->sinestates[2], beat, &c, &c);
            sines[1]->IARender(&state->sinestates[1], beat, &b, &b);
            sines[0]->IARender(&state->sinestates[0], beat, &a, &a);
            sample = (a + b + c) / 3;
            break;
        case 10:
            sines[3]->IARender(&state->sinestates[3], beat, &d, &d);
            sines[2]->IARender(&state->sinestates[2], beat, &c, &c);
            sines[1]->IARender(&state->sinestates[1], beat, &b, &b);
            sines[0]->IARender(&state->sinestates[0], beat, &a, &a);
            sample = (a + b + c + d) / 4;
            break;
        default:
            break;
    }

    sample *= level;

    state->setActive(state->sinestates[0].isActive() || state->sinestates[1].isActive() || state->sinestates[2].isActive() || state->sinestates[3].isActive());

    *lsample += sample;
    *rsample += sample;
}