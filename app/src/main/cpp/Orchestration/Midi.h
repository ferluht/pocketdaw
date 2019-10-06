//
// Created by Admin on 12.06.2019.
//

#ifndef PD_MIDI_H
#define PD_MIDI_H

#include <Engine/M.h>
#include <GUI/Canvas.h>

class MidiNote : public GCanvas{

public:

    std::list<MidiNote>::iterator position;

    MData start;
    MData end;

    MidiNote() {
        GAttachTexture("Textures/track_canvas.bmp");
    }

//    bool operator< (const MidiNote& l, const MidiNote& r) { return l.start.beat < r.start.beat; }
//    bool operator> (const MidiNote& l, const MidiNote& r) { return l.start.beat > r.start.beat; }
//    bool operator== (const MidiNote& l, const MidiNote& r) { return l.start.beat == r.start.beat; }
};

class MidiClip : public MGCanvas{

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

    MidiClip(){
        GAttachTexture("Textures/midi_canvas.bmp");
        length = 4;
        start = 0;
        last_beat = 0;

        record = true;
        record_automation = false;
        notes_position = notes.begin();
        automation_position = automation.begin();
    }

    void MIn(MData cmd) override {

        MOut(cmd);

        midiLock.lock();

        if (record) {
            cmd.beat = fmod(cmd.beat - start, length);
            if ((cmd.status == NOTEON_HEADER) || (cmd.status == NOTEOFF_HEADER)) {
                notes.insert(notes_position, cmd);
                changed = true;
            } else if (record_automation) {
                automation.insert(automation_position, cmd);
            }
        }

        midiLock.unlock();
    }

    void MRender(double beat) override {

        midiLock.lock();

        double m_beat = fmod(beat - start, length);

        while ((notes_position != notes.end()) && (m_beat >= (*notes_position).beat)) {
            MData cmd = *notes_position;
            cmd.beat = beat;
            MOut(cmd);
            notes_position++;
        }
        if (last_beat > m_beat) {
            while (notes_position != notes.end()){
                MData cmd = *notes_position;
                cmd.beat = beat;
                MOut(cmd);
                notes_position++;
            }
            notes_position = notes.begin();
        }
        last_beat = m_beat;
        while (automation_position != automation.end() && fmod(beat - start, length) > (*automation_position).beat) {
            MData cmd = *automation_position;
            cmd.beat = beat;
            MOut(cmd);
            automation_position++;
        }
        if (automation_position == automation.end()) automation_position = automation.begin();

        midiLock.unlock();
    }

    void GDraw() override {
        MGCanvas::GDraw();
        for (auto const& note : notes) {
            if (note.data2 != 0) {
                auto go = graphic_notes.find(&note);
                if (go == graphic_notes.end()) {
                    auto note_go = new MidiNote();
                    note_go->place(note.beat / length, 1 - (float) (note.data1 - 32) / 10);
                    note_go->setHeight(0.1);
                    note_go->setWidth(0.1);
                    GAttach(note_go);
                    graphic_notes.insert({&note, note_go});
                    changed = true;
                }
            }
        }
    }

    GObject * GFindFocusObject(const ndk_helper::Vec2& point) override {
        if (visible && globalPosition.contains(point)) return this;
        return nullptr;
    }

    GObject * GTapEnd(const ndk_helper::Vec2& v) override {
        midiLock.lock();
        notes.clear();

        for (auto const& gnote : graphic_notes) {
            GDetach(gnote.second);
            //            delete gnote.second;
        }

        graphic_notes.clear();

        notes_position = notes.begin();

        midiLock.unlock();
        changed = true;
        return nullptr;
    }
};

#endif //PD_MIDI_H
