//
// Created by ibelikov on 17.07.19.
//

#ifndef PD_PLOT_H
#define PD_PLOT_H

#include "Graph.h"
#include "Canvas.h"

class TimePlot : public MGCanvas{

public:
    TimeGraph * graph;

    TimePlot(unsigned int points);

    inline void update(float sample) {
        graph->update(sample);
    }
};


#endif //PD_PLOT_H
