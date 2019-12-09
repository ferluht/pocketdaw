//
// Created by ibelikov on 15.07.19.
//

#ifndef PD_GCanvas_H
#define PD_GCanvas_H

#include <common/GUI/vectordisplay/vectordisplay.h>
#include "Engine/Engine.h"

namespace GUI {

    class GCanvas : virtual public GObject {

    public:

        GCanvas() {
            setShapeType(BOX);
        }

        void GDraw(NVGcontext * nvg) override;

        void GRender(NVGcontext * nvg, float dTime) override;
    };

    class AGCanvas : virtual public AGObject, virtual public GCanvas {
    };

    class MGCanvas : virtual public MGObject, virtual public GCanvas {
    };

    class AMGCanvas : virtual public AMGObject, virtual public GCanvas {
    };

}

#endif //PD_GCanvas_H
 