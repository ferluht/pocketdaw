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
#include "GraphicEngine.h"

struct SHADER_PARAMS {
  GLuint program_;
  GLuint matrix_projection_;
};

class Position2D {
public:
    float x;
    float y;
    float z;
    float height;
    float width;
    float rotation;

    Position2D() : x(0), y(0), z(0), height(0), width(0), rotation(0) {}

    Position2D(float x_, float y_, float height_, float width_)
            : x(x_), y(y_), z(0), height(height_), width(width_), rotation(0) {}

    inline void move(ndk_helper::Vec2 v)
    {
        x = x + v.x_, y = y + v.y_;
    }

    inline void rotate(float angle)
    {
        rotation += angle;
    }

    inline bool contains(const ndk_helper::Vec2& v)
    {
        return ((x - width/2 < v.x_) && (x + width/2 > v.x_)
            && (y - height/2 < v.y_) && (y + height/2 > v.y_));
    }
};

//bool operator =(const Position2D& a, const Position2D& b) {
//    return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z) &&
//            (a.height == b.height) && (a.width == b.width) &&
//            (a.rotation == b.rotation));
//}

class GraphicObject {
public:

    GraphicEngine * eng;

    GLuint ibo_;
    GLuint vbo_;
    GLuint vao_;
    GLuint texture;

    const char * texture_name;
    const char * vshader;
    const char * fshader;

    ndk_helper::Vec2 drag_from;
    Position2D relative_position_backup;

    GraphicObject * parent;

    std::list<GraphicObject*> Graphics;
    Position2D position;
    Position2D new_position;

    SHADER_PARAMS shader_param_;
    bool LoadShaders(SHADER_PARAMS* params, const char* strVsh,
                   const char* strFsh);

    GraphicObject();
    GraphicObject(const char * vshader, const char * fshader);
    GraphicObject(const char * texture, const char * vshader, const char * fshader);
    GraphicObject(const char * texture, const char * vshader, const char * fshader, GraphicObject * parent);
    virtual ~GraphicObject();
    void Init_();
    virtual void Init() {};

    virtual void draw() {};
    void grender_(float dTime);
    virtual void grender(float dTime) {};

    void Unload();

    void update();

    void addChildObject(GraphicObject * go);
    void delChildObject(GraphicObject * go);

    GraphicObject * findFocusObject(const ndk_helper::Vec2& v);

    virtual void tapBegin(const ndk_helper::Vec2& v) {};
    virtual void tapHandler(const ndk_helper::Vec2& v) {};
    virtual void tapEnd() {};

    virtual void doubleTapBegin(const ndk_helper::Vec2& v) {};
    virtual void doubleTapHandler(const ndk_helper::Vec2& v) {};
    virtual void doubleTapEnd() {};

    virtual void dragBegin(const ndk_helper::Vec2& v) {};
    virtual void dragHandler(const ndk_helper::Vec2& v) {};
    virtual void dragEnd() {};

    virtual void pinchBegin(const ndk_helper::Vec2& v) {};
    virtual void pinchHandler(const ndk_helper::Vec2& v) {};
    virtual void pinchEnd() {};

};

#endif
