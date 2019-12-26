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
            setColor(BLACK);

            graph = new TimeGraph(points);
            graph->lPlace({0, 0});
            graph->lSetHeight(1);
            graph->lSetWidth(1);
            GAttach(graph);
        }

        inline void update(float sample) {
            graph->update(sample);
        }
    };

}

#endif //PD_PLOT_H
