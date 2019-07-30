#ifndef PD_GRAPHIC_OBJECT_H
#define PD_GRAPHIC_OBJECT_H

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

#include "GUI/Shader.h"
#include "NDKHelper.h"

class BBox {
public:
    float x;
    float y;
    float z;
    float height;
    float width;
    float angle;
    float ratio;

    bool changed;

    BBox() : BBox (0, 0, 0, 0) {}

    BBox(float x_, float y_, float height_, float width_) : BBox(x_, y_, 0, height_, width_, 0) {}

    BBox(float x_, float y_, float z_, float height_, float width_, float angle_)
            : x(x_), y(y_), z(z_), height(height_), width(width_), angle(angle_) {
        ratio = width / height;
        changed = true;
    }

    inline virtual void place(float x_, float y_, float height_, float width_) {
        place(x_, y_, height_, width_, 0);
    }

    inline virtual void place(float x_, float y_, float height_, float width_, float rotation_) {
        x = x_, y = y_, height = height_, width = width_, angle = rotation_, ratio = width_/height_;
        changed = true;
    }

    inline virtual void move(ndk_helper::Vec2 v) {
        x = x + v.x_, y = y + v.y_;
        changed = true;
    }

    inline virtual void rotate(float angle) {
        this->angle += angle;
        changed = true;
    }

    inline virtual void set_angle(float angle) {
        this->angle = angle;
        changed = true;
    }

    inline virtual bool contains(const ndk_helper::Vec2 &v) {
        return ((x < v.x_) && (x + width > v.x_)
                && (y < v.y_) && (y + height > v.y_));
    }

    inline virtual BBox toRelative(BBox ref) {
        BBox rel;
        rel.x = (x - ref.x) / ref.width;
        rel.y = (y - ref.y) / ref.height;
        rel.z = 0;
        rel.height = height / ref.height;
        rel.width = width / ref.width;
        rel.angle = -ref.angle;
        return rel;
    }

    inline virtual BBox fromRelative(BBox rel)
    {
        BBox ref;
        ref.x = x + rel.x * width;
        ref.y = y + rel.y * height;
        ref.z = 0;
        ref.height = height / rel.height;
        ref.width = width / rel.width;
        ref.angle = angle + rel.angle;
        return rel;
    }
};


class GraphicObject : public BBox {
public:

    GLuint ibo_;
    GLuint vbo_;
    GLuint vao_;
    GLuint texture;
    Shader shader;

    const char * texture_name;
    const char * vshader;
    const char * fshader;

    ndk_helper::Vec2 drag_from;
    BBox globalPosition;

    GraphicObject * parent;

    std::list<GraphicObject*> Graphics;
    BBox new_position;

    bool visible;
    bool saveRatio;

    GraphicObject(float x_, float y_, float z_, float height_, float width_, float angle_,
                  const char * texture, const char * vshader, const char * fshader, bool saveRatio_);
    virtual ~GraphicObject();

    void init_();
    virtual void init() {};

    void draw_();
    virtual void draw() {};

    void grender_(float dTime);
    virtual void grender(float dTime) {};

    void setVisible(bool visible_);

    void unload();

    // Hierarchy

    void attach(GraphicObject * go);
    void detach(GraphicObject * go);

    inline void attachTo(GraphicObject * go) { parent = go; }

    // Event handlers

    virtual GraphicObject * findFocusObject(const ndk_helper::Vec2& point)
    {
        for (auto const &gr : Graphics)
            if (gr->visible && gr->globalPosition.contains(point))
                return gr->findFocusObject(point);
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
