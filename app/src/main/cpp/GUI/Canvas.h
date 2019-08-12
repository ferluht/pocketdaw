//
// Created by ibelikov on 15.07.19.
//

#ifndef PD_GCanvas_H
#define PD_GCanvas_H


#include "Engine/Engine.h"

class GCanvas : virtual public GObject{
public:

    GCanvas(){
        GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainRect.fsh");
    }

    void GDraw() override ;
    void GRender(float dTime) override ;
};

class AGCanvas : virtual public AGObject, virtual public GCanvas {};
class MGCanvas : virtual public MGObject, virtual public GCanvas {};
class AMGCanvas : virtual public AMGObject, virtual public GCanvas {};


#endif //PD_GCanvas_H
 