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


class Position2D {
public:
    float x;
    float y;
    float z;
    float height;
    float width;
    float angle;

    Position2D() : x(0), y(0), z(0), height(0), width(0), angle(0) {}

    Position2D(float x_, float y_, float height_, float width_)
            : x(x_), y(y_), z(0), height(height_), width(width_), angle(0) {}

    inline void place(float x_, float y_, float height_, float width_) {
        place(x_, y_, height_, width_, 0);
    }

    inline void place(float x_, float y_, float height_, float width_, float rotation_) {
        x = x_, y = y_, height = height_, width = width_, angle = rotation_;
    }

    inline void move(ndk_helper::Vec2 v) {
        x = x + v.x_, y = y + v.y_;
    }

    inline void rotate(float angle) {
        this->angle += angle;
    }

    inline void set_angle(float angle) {
        this->angle = angle;
    }

    inline bool contains(const ndk_helper::Vec2 &v) {
        return ((x - width / 2 < v.x_) && (x + width / 2 > v.x_)
                && (y - height / 2 < v.y_) && (y + height / 2 > v.y_));
    }

    inline Position2D toRelative(Position2D ref) {
        Position2D rel;
        rel.x = (x - ref.x) / ref.width;
        rel.y = (y - ref.y) / ref.height;
        rel.z = 0;
        rel.height = height / ref.height;
        rel.width = width / ref.width;
        rel.angle = -ref.angle;
        return rel;
    }

    inline Position2D fromRelative(Position2D rel)
    {
        Position2D ref;
        ref.x = x + rel.x * width;
        ref.y = y + rel.y * height;
        ref.z = 0;
        ref.height = height / rel.height;
        ref.width = width / rel.width;
        ref.angle = angle + rel.angle;
        return rel;
    }
};

//bool operator =(const Position2D& a, const Position2D& b) {
//    return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z) &&
//            (a.height == b.height) && (a.width == b.width) &&
//            (a.rotation == b.rotation));
//}

class GraphicObject : public Position2D {
public:

    GLuint ibo_;
    GLuint vbo_;
    GLuint vao_;
    GLuint texture;
    SHADER shader;

    const char * texture_name;
    const char * vshader;
    const char * fshader;

    ndk_helper::Vec2 drag_from;
    Position2D relative_position_backup;

    GraphicObject * parent;

    std::list<GraphicObject*> Graphics;
    Position2D new_position;

    GraphicObject();
    GraphicObject(const char * texture);
    GraphicObject(const char * vshader, const char * fshader);
    GraphicObject(const char * texture, const char * vshader, const char * fshader);
    virtual ~GraphicObject();


    void init_();
    virtual void init() {};

    void draw_();
    virtual void draw() {};

    void grender_(float dTime);
    virtual void grender(float dTime) {};

    void unload();

    // Hierarchy

    void attach(GraphicObject * go);
    void detach(GraphicObject * go);

    inline void attachTo(GraphicObject * go) { parent = go; }

    // Event handlers

    virtual GraphicObject * findFocusObject(const ndk_helper::Vec2& point)
    {
        for (auto const &gr : Graphics) if (gr->contains(point)) return gr->findFocusObject(point);
        return this;
    }

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
