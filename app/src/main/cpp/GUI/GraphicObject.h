/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//--------------------------------------------------------------------------------
// Teapot Renderer.h
// Renderer for teapots
//--------------------------------------------------------------------------------
#ifndef _TEAPOTRENDERER_H
#define _TEAPOTRENDERER_H

//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
#include <jni.h>
#include <errno.h>

#include <list>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>

#include "NDKHelper.h"

struct SHADER_PARAMS {
  GLuint program_;
  GLuint matrix_projection_;
};

struct Position2D {
    float x;
    float y;
    float z;
    float height;
    float width;
};

//bool operator<(const Position2D &lhs, const Position2D &rhs) {
//    if (lhs.x != rhs.x) return lhs.x > rhs.x;
//    return lhs.y > rhs.y;
//}

class GraphicObject {
public:
    GLuint ibo_;
    GLuint vbo_;
    GLuint vao_square_;
    GLuint texture;

    const char * texture_name;
    const char * vshader;
    const char * fshader;

    ndk_helper::Vec2 drag_from;
    Position2D relative_position_backup;
    float drag_xscale, drag_yscale;

    GraphicObject * parent;

    std::list<GraphicObject*> Graphics;
    Position2D relativePosition;
    Position2D absolutePosition;

    SHADER_PARAMS shader_param_;
    bool LoadShaders(SHADER_PARAMS* params, const char* strVsh,
                   const char* strFsh);

    ndk_helper::Mat4 mat_projection_;
    ndk_helper::Mat4 mat_view_;

    GraphicObject(const char * texture, const char * vshader, const char * fshader);
    GraphicObject(const char * texture, const char * vshader, const char * fshader, GraphicObject * parent);
    virtual ~GraphicObject();
    void Init();
    virtual void Init_() {};

    void SetPosition(float x, float y);
    void SetSize(float x, float y);

    void grender_(float dTime);
    virtual void grender(float dTime) {};

    void Unload();
    void Update();
    void addChildObject(GraphicObject * go);

    GraphicObject * findTapHandler(ndk_helper::Vec2 v);
    virtual void tapHandler(ndk_helper::Vec2 v) {};

    GraphicObject * findDoubleTapHandler(ndk_helper::Vec2 v);
    virtual void doubleTapHandler(ndk_helper::Vec2 v) {};

    GraphicObject * findDragHandler(ndk_helper::Vec2 v, float xscale, float yscale);
    virtual void dragBegin(ndk_helper::Vec2 v, float xscale, float yscale) {};
    virtual void dragHandler(ndk_helper::Vec2 v) {};
    virtual void dragEnd() {};

    GraphicObject * findPinchHandler(ndk_helper::Vec2 v);
    virtual void pinchHandler(ndk_helper::Vec2 v) {};
};

#endif
