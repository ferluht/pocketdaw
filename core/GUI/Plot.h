//
// Created by ibelikov on 17.07.19.
//

#ifndef PD_PLOT_H
#define PD_PLOT_H

#include "Graph.h"
#include "Canvas.h"

namespace GUI {

    template <class T>
    class Plot : public AMGCanvas {

    public:
        T *graph;

        Plot(unsigned int points) {
            setColor(GEngine::ui_theme->PLOT_BODY_COLOR);

            graph = new TimeGraph(points);
            graph->GPlace({0, 0});
            graph->GSetHeight(1);
            graph->GSetWidth(1);
            GAttach(graph);
        }

        inline void update(float sample) {
            graph->update(sample);
        }

        virtual GObject *GFindFocusObject(const vecmath::Vec2 &point, std::list<GObject *> * trace) {
            if (visible && this->GContains(point)) {
                trace->push_front(this);
                return this;
            }
            return nullptr;
        }
    };

//    template <class T>
//    class DraggablePlot : public Plot<T> {
//
//        std::function<GUI::GObject *(const vecmath::Vec2& v)> tap_end_callback;
//        std::function<GUI::GObject *(const vecmath::Vec2& v)> drag_begin_callback;
//        std::function<GUI::GObject *(const vecmath::Vec2& v)> drag_callback;
//        std::function<GUI::GObject *(const vecmath::Vec2& v)> drag_end_callback;
//
//    public:
//
//        DraggablePlot(unsigned int points,
//                      std::function<GUI::GObject *(const vecmath::Vec2& v)> tap_end_callback_,
//                      std::function<GUI::GObject *(const vecmath::Vec2& v)> drag_begin_callback_,
//                      std::function<GUI::GObject *(const vecmath::Vec2& v)> drag_callback_,
//                      std::function<GUI::GObject *(const vecmath::Vec2& v)> drag_end_callback_
//                      ) : Plot<T>(points) {
//            tap_end_callback = tap_end_callback_;
//            drag_callback = drag_callback_;
//            drag_begin_callback = drag_begin_callback_;
//            drag_end_callback = drag_end_callback_;
//        }
//
//
//
//    };

}

#endif //PD_PLOT_H
