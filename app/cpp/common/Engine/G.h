#ifndef PD_GRAPHIC_OBJECT_H
#define PD_GRAPHIC_OBJECT_H

//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
#include <jni.h>
#include <errno.h>

#include <list>

#include <android/sensor.h>
#include <android/log.h>

#include <cerrno>
#include <set>
#include <string>

#include <bgfx/bgfx.h>
#include <GUI/vectordisplay/vectordisplay.h>
#include <ndk_helper/NDKHelper.h>

#include <common/nanovg/nanovg.h>

typedef ndk_helper::Vec2 Vec2;

namespace GUI {

    class Shape {

        bool fixed_global;

    public:

        Shape * parent;

        struct Position {
            Vec2 c;
            Vec2 s;
            float angle;
            float ratio;

            Position () {}
        };

        Position local, global;

        bool changed;

        Shape () {
            parent = NULL;

            fixed_global = false;

            local.c = {0, 0};
            local.s = {0, 0};
            local.angle = 0;
            local.ratio = 0;

            global.c = {0, 0};
            global.s = {0, 0};
            global.ratio = 0;
        }

        inline void attach(Shape * parent_) { parent = parent_; }

        inline void detach() { parent = NULL; }

        inline virtual void lPlace(Vec2 c_) { local.c = c_; }

        inline virtual void lMove(Vec2 v) { local.c += v; }

        inline virtual void lRotate(float angle_) { local.angle += angle_; }

        inline virtual void lSetAngle(float angle_) { local.angle = angle_; }

        inline virtual void setRatio(float ratio_) { local.ratio = ratio_; }

        inline virtual void lSetWidth(float width_) { local.s.x = width_; }

        inline virtual void lSetHeight(float height_) { local.s.y = height_; }

        inline virtual void setGlobal(Vec2 c_, Vec2 s_) {
            global.c = c_;
            global.s = s_;
            global.ratio = s_.x / s_.y;
            fixed_global = true;
        }

        inline virtual void fixGlobal() { fixed_global = true; }
        inline virtual void freeGlobal() { fixed_global = false; }

        inline virtual bool contains(const Vec2 &v) {}

        void updateGlobal();
    };

    class BoxShape : public Shape {
    public:

        BoxShape() : Shape() {}

        inline virtual bool contains(const Vec2 &v) {
            return containsX(v) && containsY(v);
        }

        inline virtual bool containsX(const Vec2 &v) {
            return (global.c.x < v.x) && (global.c.x + global.s.x > v.x);
        }

        inline virtual bool containsY(const Vec2 &v) {
            return (global.c.y < v.y) && (global.c.y + global.s.y > v.y);
        }
    };

    class CircleShape : public Shape {

    public:

        CircleShape() : Shape() {}

        inline virtual bool contains(const Vec2 &v) {
            return sqrt(pow(global.c.x + global.s.x/2 - v.x, 2) + pow(global.c.y + global.s.y/2 - v.y, 2)) < global.s.x / 2;
        }

        inline virtual bool containsX(const Vec2 &v) {
            return (global.c.x < v.x) && (global.c.x + global.s.x > v.x);
        }

        inline virtual bool containsY(const Vec2 &v) {
            return (global.c.y < v.y) && (global.c.y + global.s.y > v.y);
        }

        inline virtual void lSetWidth(float width_) {
            local.s.x = width_;
            local.s.y = width_;
            updateGlobal();
        }

        inline virtual void lSetHeight(float height_) {
            lSetWidth(height_);
        }


    };

    enum SHAPE_TYPES {
        BOX,
        CIRCLE
    };

//    template <class ShapeClass>
//    class GObject : public ShapeClass {
//        static_assert(std::is_base_of<Shape, ShapeClass>::value, "ShapeClass must derive from Shape");

    class GObject {

    public:

        Shape * shape;
        GObject *parent;
        std::list<GObject *> Graphics;

        bool visible;
        bool initialized;
        bool infocus;

        GObject(float scale_=1) {
            this->parent = nullptr;
            this->visible = true;
            this->initialized = false;
            infocus = false;
        }

        virtual ~GObject() {}

        virtual void GInit() {};

        void setShapeType(SHAPE_TYPES shape_type){

            switch (shape_type){
                case BOX:
                    shape = new BoxShape();
                    break;
                case CIRCLE:
                    shape = new CircleShape();
                    break;
                default:
                    assert(0);
                    break;
            }
        }

        void GDraw_(NVGcontext *nvg) {
            if (!initialized) GInit();

            if (parent)
                GPlace();

            shape->updateGlobal();

            GDraw(nvg);
        }

        virtual void GDraw(NVGcontext *nvg) {};

        virtual void GPlace() {};

        void GRender_(NVGcontext *nvg, float dTime) {

            if (visible) {

                GDraw_(nvg);

                GRender(nvg, dTime);

                for (auto gr = Graphics.rbegin(); gr != Graphics.rend(); ++gr) {
                    if (!(*gr)->infocus) (*gr)->GRender_(nvg, dTime);
                }
            }

            infocus = false;
        }

        virtual void GRender(NVGcontext *nvg, float dTime) {};

        virtual void GSetVisible(bool visible_) {
            visible = visible_;
            for (auto const &gr : Graphics) {
                gr->GSetVisible(visible_);
            }
        }

        inline void markInFocus() { infocus = true; }

        // Hierarchy

        virtual void GAttach(GObject *go) {
            go->GAttachTo(this);
            Graphics.push_front(go);
        }

        virtual void GDetach(GObject *go) {
            Graphics.remove(go);
            go->shape->detach();
        }

        inline void GAttachTo(GObject *go) {
            parent = go;
            shape->attach(go->shape);
        }

        // Event handlers

        virtual void GGainFocus() {
//            infocus = true;
        }

        virtual void GLoseFocus() {
//            infocus = false;
        }

        virtual GObject *GFindFocusObject(const Vec2 &point, std::list<GObject *> * trace) {
            for (auto const &gr : Graphics) {
                auto fo = gr->GFindFocusObject(point, trace);
                if (fo) {
                    trace->push_front(this);
                    return fo;
                }
            }
            if (visible && shape->contains(point)) {
                trace->push_front(this);
                return this;
            }
            return nullptr;
        }

        virtual GObject *GTapBegin(const Vec2 &v) { return this; };

        virtual GObject *GTapHandler(const Vec2 &v) { return this; };

        virtual GObject *GTapEnd(const Vec2 &v) { return this; };

        virtual GObject *GDoubleTapBegin(const Vec2 &v) { return this; };

        virtual GObject *GDoubleTapHandler(const Vec2 &v) { return this; };

        virtual GObject *GDoubleTapEnd(const Vec2 &v) { return this; };

        virtual GObject *GDragBegin(const Vec2 &v) { return this; };

        virtual GObject *GDragHandler(const Vec2 &v) { return this; };

        virtual GObject *GDragEnd(const Vec2 &v) { return this; };

        virtual GObject *GPinchBegin(const Vec2 &v) { return this; };

        virtual GObject *GPinchHandler(const Vec2 &v) { return this; };

        virtual GObject *GPinchEnd(const Vec2 &v) { return this; };

    };


    template <class DstType>
    bool IsType(const GObject* src)
    {
        return dynamic_cast<const DstType*>(src) != nullptr;
    }


    //-------------------------------------------------------------------------
// Shared state for our app.
//-------------------------------------------------------------------------
    struct android_app;


    class GEngine {

    public:

        static float screen_width;
        static float screen_height;
        static float screen_ratio;

        ndk_helper::DoubletapDetector doubletap_detector_;
        ndk_helper::PinchDetector pinch_detector_;
        ndk_helper::DragDetector drag_detector_;
        ndk_helper::TapDetector tap_detector_;
        ndk_helper::PerfMonitor monitor_;

        android_app *app_;

        NVGcontext * nvg;

        std::list<GObject *> focusStack;

        static GEngine &getGEngine() {
            static GEngine gengine;
            return gengine;
        }

        void setRoot(GObject *root_) {
            focusStack.push_back(root_);
        }

        void setDisplay(NVGcontext * nvg_, float sw, float sh) {
            nvg = nvg_;
            screen_width = sw;
            screen_height = sh;
            screen_ratio = sw / sh;
        }

        void focusOn(GObject *focus_obj) {
            if (focus_obj && (focus_obj != focusStack.back())) {
                focus_obj->GGainFocus();
                focusStack.push_back(focus_obj);
            }
        }

        void unfocus() {
            if (focusStack.size() != 1) {
                focusStack.back()->GLoseFocus();
                focusStack.pop_back();
            }
        }

        void Render() {
            // Set view 0 default viewport.
            bgfx::setViewRect(0, 0, 0, uint16_t(screen_width), uint16_t(screen_height) );

            // This dummy draw call is here to make sure that view 0 is cleared
            // if no other draw calls are submitted to view 0.
            bgfx::touch(0);

            nvgBeginFrame(nvg, float(screen_width), float(screen_height), 1.0f);

            for (auto const& root : focusStack) root->markInFocus();

            for (auto const& root : focusStack) root->GRender_(nvg, (float) monitor_.GetCurrentTime());

            nvgEndFrame(nvg);

            bgfx::frame();
        }

        GObject * FindFocusObject(const Vec2 &point) {
            GObject * ret = nullptr;
            for (auto root = focusStack.rbegin(); root != focusStack.rend(); ++root) {
                if ((*root)->shape->contains(point)) {
                    std::list<GObject *> trace;
                    ret = (*root)->GFindFocusObject(point, &trace);
                    focusStack.erase(std::next(root).base(), focusStack.end());
                    for (auto const& obj : trace) obj->GGainFocus();
                    focusStack.splice(focusStack.end(), trace);
                    return ret;
                } else {
                    (*root)->GLoseFocus();
                }
            }
        }

        GEngine(const GEngine &) {}

    private:
        // Private constructor
        GEngine() {

        }

        ~GEngine() {}

        // Prevent copy-construction
        GEngine &operator=(const GEngine &);      // Prevent assignment
    };

}

#endif
