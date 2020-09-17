//
// Created by Admin on 12.06.2019.
//

#ifndef PD_MIDI_H
#define PD_MIDI_H

#include <M.h>
#include <GUI/Canvas.h>
#include <GUI/styles.h>

class MidiNote : public GUI::AMGCanvas{

public:

    std::list<MidiNote>::iterator position;

    MData start;
    MData end;

    MidiNote() {
//        GAttachTexture("Textures/track_canvas.bmp");
        setColor(DARK);
    }

//    bool operator< (const MidiNote& l, const MidiNote& r) { return l.start.beat < r.start.beat; }
//    bool operator> (const MidiNote& l, const MidiNote& r) { return l.start.beat > r.start.beat; }
//    bool operator== (const MidiNote& l, const MidiNote& r) { return l.start.beat == r.start.beat; }
};

class MidiClip : public GUI::AMGCanvas{

public:

    std::mutex midiLock;

    std::list<MData> notes;
    std::list<MData>::iterator notes_position;
    std::list<MData> automation;
    std::list<MData>::iterator automation_position;

    std::map<const MData *, MidiNote *> graphic_notes;

    double length;
    double start;
    double last_beat;
    bool record;
    bool record_automation;
    bool hot_restart;

    float root_note = 32, old_root_note;
    float vert_notes = 6;
    Vec2 drag_start;

    MidiClip(){
//        GAttachTexture("Textures/midi_canvas.bmp");
        length = 8;
        start = 0;
        last_beat = 0;

        record = false;
//        record_automation = true;
        hot_restart = false;
        notes_position = notes.begin();
        automation_position = automation.begin();

        setColor(GUI::GEngine::ui_theme->MIDI_BODY_COLOR);

        GSetDragBeginCallback([this](const Vec2& v) -> GUI::GObject * {
            drag_start = v;
            old_root_note = root_note;
            return this;
        });

        GSetDragHandlerCallback([this](const Vec2& v) -> GUI::GObject * {
            root_note = old_root_note + (int)((v.y - drag_start.y) / (global.s.y / vert_notes));
            return this;
        });

        GSetDoubleTapEndCallback([this](const Vec2& v) -> GUI::GObject * {
            midiLock.lock();
            notes.clear();
            for (auto const& gnote : graphic_notes) GDetach(gnote.second);
            graphic_notes.clear();
            notes_position = notes.begin();
            midiLock.unlock();
            changed = true;
            return nullptr;
        });
    }

    void MIn(MData cmd) override {

        if (((cmd.status & 0xF0) == 0xB0) && (cmd.data1 == 1)) {
            length = (int)((cmd.data2 + 1) / 8);
            if (length == 0) length = 1;
        }

        if (((cmd.status & 0xF0) == 0xB0) && (cmd.data1 == 0x17) && (cmd.data2 > 0)) {
            for (auto it = notes.begin(); it != notes.end(); ++it) {
                if ((*it).data2 > 0) (*it).beat = static_cast<int>((*it).beat/0.25 + 0.5) * 0.25;
            }
        }

        MOut(cmd);

        midiLock.lock();

        if (record) {
            cmd.beat = fmod(cmd.beat - start, length);
            if (((cmd.status & 0xF0) == NOTEON_HEADER) || ((cmd.status & 0xF0) == NOTEOFF_HEADER)) {
                notes.insert(notes_position, cmd);
                changed = true;
            }
            if (record_automation) {
                notes.insert(notes_position, cmd);
            }
        }

        midiLock.unlock();
    }

    void GSetHeight(float height) override {
        GUI::AMGCanvas::GSetHeight(height);
        if (height < 0.5) vert_notes = 6;
        else vert_notes = 24;
    }

    inline void MCHotRestart() {
        hot_restart = true;
    }

    inline void MCCopyTo(MidiClip * dest) {
        std::copy(notes.begin(), notes.end(), std::back_inserter(dest->notes));
    }

    void MRender(double beat) override {
//        return;
        midiLock.lock();

        double m_beat = fmod(beat - start, length);

        while ((notes_position != notes.end()) && (m_beat >= (*notes_position).beat)) {
            MData cmd = *notes_position;
            cmd.beat = beat;
            if (!hot_restart) MOut(cmd);
            notes_position++;
        }
        if (last_beat > m_beat) {
            while (notes_position != notes.end()){
                MData cmd = *notes_position;
                cmd.beat = beat;
                if (!hot_restart) MOut(cmd);
                notes_position++;
            }
            notes_position = notes.begin();
        }
        last_beat = m_beat;
//        while (automation_position != automation.end() && m_beat >= (*automation_position).beat) {
//            MData cmd = *automation_position;
//            cmd.beat = beat;
//            if (!hot_restart) MOut(cmd);
//            automation_position++;
//        }
//        if (automation_position == automation.end()) automation_position = automation.begin();

        hot_restart = false;

        midiLock.unlock();
    }

    void GDraw(NVGcontext * nvg) override {
        GUI::AMGCanvas::GDraw(nvg);
        for (auto const& note : notes) {
            if ((note.status & 0xF0 ) == 0xB0) continue;
            if (note.data2 != 0) {
//                auto go = graphic_notes.find(&note);
//                if (go == graphic_notes.end()) {
//                    auto note_go = new MidiNote();
//                    note_go->GPlace({static_cast<float>(note.beat / length), 1 - (float) (note.data1 - 32) / 10});
//                    note_go->GSetHeight(0.1);
//                    note_go->GSetWidth(0.1);
//                    GAttach(note_go);
//                    graphic_notes.insert({&note, note_go});
//                    changed = true;
//                }

                float x = static_cast<float>(note.beat / length);
                float y = 1 - (float) (note.data1 - root_note) / vert_notes;

                if (x < 0 || x > 1 || y < 0 || y > 1 - 1/vert_notes) continue;

                nvgBeginPath(nvg);
                nvgRect(nvg, global.c.x + global.s.x * x, global.c.y + global.s.y * (1 - (float) (note.data1 - root_note) / vert_notes), 50, global.s.y/vert_notes);
                nvgFillColor(nvg, GUI::GEngine::ui_theme->MIDI_NOTE_COLOR);
                nvgFill(nvg);
                nvgClosePath(nvg);
            }
        }

        for (int i = 0; i < length; i++) {
            for (int j = 0; j < 16; j++) {
                nvgBeginPath(nvg);
                nvgRect(nvg, global.c.x + global.s.x * ((float)i / length + (float)j/16), global.c.y, 1, global.s.y);
                nvgFillColor(nvg, GUI::GEngine::ui_theme->MIDI_DELIMITER_COLOR);
                nvgFill(nvg);
                nvgClosePath(nvg);
            }
            nvgBeginPath(nvg);
            nvgRect(nvg, global.c.x + global.s.x * (float)i / length, global.c.y, 2, global.s.y);
            nvgFillColor(nvg, GUI::GEngine::ui_theme->MIDI_DELIMITER_COLOR);
            nvgFill(nvg);
            nvgClosePath(nvg);
        }

        for (int i = 1; i < vert_notes; i++) {
            nvgBeginPath(nvg);
            nvgRect(nvg, global.c.x, global.c.y + i * global.s.y / vert_notes, global.s.x, 2);
            nvgFillColor(nvg, GUI::GEngine::ui_theme->MIDI_DELIMITER_COLOR);
            nvgFill(nvg);
            nvgClosePath(nvg);
        }

        nvgBeginPath(nvg);
        nvgRect(nvg, global.c.x + global.s.x * last_beat / length, global.c.y, 2, global.s.y);
        nvgFillColor(nvg, RED);
        nvgFill(nvg);
        nvgClosePath(nvg);
    }

    GObject * GFindFocusObject(const Vec2 &point, std::list<GObject *> * trace) override {
        if (visible && GContains(point)) {
            trace->push_front(this);
            return this;
        }
        return nullptr;
    }

};

#endif //PD_MIDI_H
