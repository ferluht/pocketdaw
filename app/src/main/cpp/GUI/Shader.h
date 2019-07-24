//
// Created by ibelikov on 24.07.19.
//

#ifndef PD_SHADER_H
#define PD_SHADER_H

#include <EGL/egl.h>
#include <GLES/gl.h>
#include "NDKHelper.h"

enum SHADERS{
    SHADER_BASIC,
    SHADER_ALPHA
};

class Shader {

public:
    GLuint program_;
    GLint param_view_;
    GLint param_texture_angle_;

    Shader();

    static Shader CreateShaderProgram(const char *vsh, const char *fsh);
};


#endif //PD_SHADER_H
