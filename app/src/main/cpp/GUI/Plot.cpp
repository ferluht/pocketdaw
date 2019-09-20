//
// Created by ibelikov on 17.07.19.
//

#include "Plot.h"

TimePlot::TimePlot(unsigned int points) {
    GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainColor.fsh");
    GSetColor(0.15, 0.15, 0.15, 1.0);

    graph = new TimeGraph(points);
    graph->place(0, 0);
    graph->setHeight(1);
    graph->setWidth(1);
    GAttach(graph);
}