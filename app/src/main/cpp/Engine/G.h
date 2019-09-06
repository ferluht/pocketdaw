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

#include <cerrno>
#include <set>
#include <string>

#include "NDKHelper.h"

enum SHADERS{
    SHADER_BASIC,
    SHADER_ALPHA
};

class Shader {

    static std::map<std::string, Shader> shaders;

public:
    GLuint program_;
    GLint param_view_;
    GLint param_texture_angle_;
    GLint param_z_;
    GLint param_color_;

    Shader();

    static Shader CreateShaderProgram(const char *vsh, const char *fsh);
};

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
        ratio = (height * width) ? width / height : 0;
        changed = true;
    }

    inline virtual void setRatio(float ratio_) {
        ratio = ratio_;
        changed = true;
    }

    inline virtual void setWidth(float width_) {
        width = width_;
        changed = true;
    }

    inline virtual void setHeight(float height_) {
        height = height_;
        changed = true;
    }

    inline virtual void place(float x_, float y_) {
        place(x_, y_, z);
    }

    inline virtual void place(float x_, float y_, float z_) {
        x = x_, y = y_, z = z_;
        changed = true;
    }

    inline virtual void move(ndk_helper::Vec2 v) {
        x = x + v.x_, y = y + v.y_;
        changed = true;
    }

    inline virtual void rotate(float angle_) {
        angle += angle_;
        changed = true;
    }

    inline virtual void setAngle(float angle_) {
        angle = angle_;
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


class GObject : public BBox {
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

    GObject * parent;

    GObject * focusObject;

    std::list<GObject*> Graphics;
    BBox new_position;

    bool visible;
    bool saveRatio;
    bool initialized;

    GLfloat color[4] = {1, 1, 1, 0};

    GObject();
    virtual ~GObject();

    inline void GAttachShaders(const char * vshader, const char * fshader) {
        this->vshader = vshader;
        this->fshader = fshader;
        initialized = false;
    }

    inline void GAttachTexture(const char * texture) {
        this->texture_name = texture;
        initialized = false;
    }

    inline void GSaveRatio(bool saveRatio_) {
        saveRatio = saveRatio_;
        initialized = false;
    }

    void GInit_();
    virtual void GInit() {};

    void GDraw_();
    virtual void GDraw() {};

    void GRender_(float dTime);
    virtual void GRender(float dTime) {};

    virtual void GSetVisible(bool visible_) {
        visible = visible_;
        changed = true;
        for (auto const &gr : Graphics) {
            gr->GSetVisible(visible_);
        }
    }

    void GUnload();

    inline void GSetColor(float r, float g, float b, float a){
        color[0] = r, color[1] = g, color[2] = b, color[3] = a;
    }

    // Hierarchy

    void GAttach(GObject * go);
    void GDetach(GObject * go);

    inline void GAttachTo(GObject * go) { parent = go; }

    // Event handlers

    virtual void GGainFocus() {}

    virtual void GLoseFocus() {}

    virtual GObject * GFindFocusObject(const ndk_helper::Vec2& point)
    {
        for (auto const &gr : Graphics) {
            auto fo = gr->GFindFocusObject(point);
            if (fo) return fo;
        }
        if (visible && globalPosition.contains(point)) return this;
        return nullptr;
    }

    virtual GObject * GTapBegin(const ndk_helper::Vec2& v) {return this;};
    virtual GObject * GTapHandler(const ndk_helper::Vec2& v) {return this;};
    virtual GObject * GTapEnd(const ndk_helper::Vec2& v) {return nullptr;};

    virtual GObject * GDoubleTapBegin(const ndk_helper::Vec2& v) {return this;};
    virtual GObject * GDoubleTapHandler(const ndk_helper::Vec2& v) {return this;};
    virtual GObject * GDoubleTapEnd(const ndk_helper::Vec2& v) {return nullptr;};

    virtual GObject * GDragBegin(const ndk_helper::Vec2& v) {return this;};
    virtual GObject * GDragHandler(const ndk_helper::Vec2& v) {return this;};
    virtual GObject * GDragEnd(const ndk_helper::Vec2& v) {return nullptr;};

    virtual GObject * GPinchBegin(const ndk_helper::Vec2& v) {return this;};
    virtual GObject * GPinchHandler(const ndk_helper::Vec2& v) {return this;};
    virtual GObject * GPinchEnd(const ndk_helper::Vec2& v) {return nullptr;};

};

//-------------------------------------------------------------------------
// Shared state for our app.
//-------------------------------------------------------------------------
struct android_app;


class GEngine {

public:

    ndk_helper::GLContext *gl_context_;

    bool initialized_resources_;
    bool has_focus_;

    static float screen_width;
    static float screen_height;
    static float screen_ratio;

    ndk_helper::Mat4 mat_projection_;
    ndk_helper::Mat4 mat_view_;

    ndk_helper::DoubletapDetector doubletap_detector_;
    ndk_helper::PinchDetector pinch_detector_;
    ndk_helper::DragDetector drag_detector_;
    ndk_helper::TapDetector tap_detector_;
    ndk_helper::PerfMonitor monitor_;

    android_app *app_;

    std::list<GObject *> focus;

    static GEngine& getGEngine()
    {
        static GEngine gengine;
        return gengine;
    }

    void setRoot(GObject * root_) {
        focus.push_back(root_);
    }

    void UpdateFPS(float fFPS);

    void ShowUI();

    void SetState(android_app *app);

    int InitDisplay(android_app *app);

    void UnloadResources();

    void DrawFrame();

    void TermDisplay();

    void TrimMemory();

    bool IsReady();

    void SetupView();


private:
    // Private constructor
    GEngine() {
        gl_context_ = ndk_helper::GLContext::GetInstance();
    }
    ~GEngine() {}
    GEngine(const GEngine&);                 // Prevent copy-construction
    GEngine& operator=(const GEngine&);      // Prevent assignment
};

#endif
