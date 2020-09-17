//
// Created by Admin on 06.07.2019.
//

#ifndef PD_TEXTURE_H
#define PD_TEXTURE_H

#include <EGL/egl.h>
#include <GLES/gl.h>

namespace ndk_helper {

    namespace texture {

        GLuint loadBMP(const char *imagepath);

    }
}  // namespace ndkHelper

#endif //PD_TEXTURE_H
