//
// Created by ibelikov on 16.12.19.
//

#ifndef PD_SLIDER_H
#define PD_SLIDER_H

#include "Knob.h"
#include "styles.h"

namespace GUI {

    class Slider : public Knob {

        float min;
        float max;
        float value;
        char * label;

        const float bottom_text_height = 0.07;
        const float top_text_height = 0.1;
        const float slider_width = 0.8;
        const float slider_height = 0.07;

        vecmath::Vec2 drag_from;
        float old_value;

    public:

        Slider(const char * label_, float val_, float min_, float max_);

        void GDraw(NVGcontext * nvg) override;

        GObject *DragBegin(const vecmath::Vec2 &v) ;

        GObject *DragHandler(const vecmath::Vec2 &v) ;

        operator float() const { return value; }

    };

}

#endif //PD_SLIDER_H
