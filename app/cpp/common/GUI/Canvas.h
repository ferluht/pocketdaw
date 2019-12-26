//
// Created by ibelikov on 15.07.19.
//

#ifndef PD_GCanvas_H
#define PD_GCanvas_H

#include <common/GUI/vectordisplay/vectordisplay.h>
#include "Engine/Engine.h"

namespace GUI {

    class AMGCanvas : public AMGObject {

        NVGcolor color;

    public:

        AMGCanvas() : AMGObject(BOX) {
            color.a = 0;
        }

        inline void setColor(NVGcolor color_) { color = color_; }

        void GDraw(NVGcontext * nvg) override;
    };


}

#endif //PD_GCanvas_H
 