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
#include "ndk_helper/NDKHelper.h"

#include "nanovg/nanovg.h"

typedef ndk_helper::Vec2 Vec2;

namespace GUI {

    enum SHAPE_TYPES {
        BOX,
        CIRCLE
    };

    class Shape {

        unsigned int type;

    public:

        struct Position {
            Vec2 c;
            Vec2 s;
            float ratio;

            Position () {}
        };

        Position local, global;

        bool changed;

        Shape (unsigned int type_=BOX) {
            type = type_;

            local.c = {0, 0};
            local.s = {0, 0};
            local.ratio = 0;

            global.c = {0, 0};
            global.s = {0, 0};
            global.ratio = 0;
        }

        inline virtual void GPlace(Vec2 c_) { local.c = c_; }

        inline virtual void GSetRatio(float ratio_) { local.ratio = ratio_; }

        inline virtual void GSetWidth(float width_) {
            switch (type) {
                case BOX:
                    local.s.x = width_;
                    break;
                case CIRCLE:
                    local.s.x = width_;
                    local.s.y = width_;
                    break;
                default:
                    break;
            }
        }

        inline virtual void GSetHeight(float height_) {
            switch (type) {
                case BOX:
                    local.s.y = height_;
                    break;
                case CIRCLE:
                    local.s.x = height_;
                    local.s.y = height_;
                    break;
                default:
                    break;
            }
        }

        inline virtual bool GContains(const Vec2 &v) {
            switch (type) {
                case BOX:
                    return GContainsX(v) && GContainsY(v);
                case CIRCLE:
                    return sqrt(pow(global.c.x + global.s.x/2 - v.x, 2) + pow(global.c.y + global.s.y/2 - v.y, 2)) < global.s.x / 2;
                default:
                    return false;
            }
        }

        inline virtual bool GContainsX(const Vec2 &v) {
            switch (type) {
                case BOX:
                    return (global.c.x < v.x) && (global.c.x + global.s.x > v.x);
                case CIRCLE:
                    return (global.c.x < v.x) && (global.c.x + global.s.x > v.x);
                default:
                    return false;
            }
        }

        inline virtual bool GContainsY(const Vec2 &v) {
            switch (type) {
                case BOX:
                    return (global.c.y < v.y) && (global.c.y + global.s.y > v.y);
                case CIRCLE:
                    return (global.c.y < v.y) && (global.c.y + global.s.y > v.y);
                default:
                    return false;
            }
        }

        void GUpdateGlobalPosition(Shape *parent);
    };


    class GObject : public Shape {

    public:

        std::list<GObject *> Graphics;

        GObject * parent;

        bool visible;
        bool initialized;
        bool infocus;

        GObject(unsigned int shape_type_) : Shape(shape_type_) {
            this->visible = true;
            this->initialized = false;
            infocus = false;
            parent = nullptr;
        }

        virtual ~GObject() {}

        virtual void GInit() {};

        void GDraw_(NVGcontext *nvg) {
            if (!initialized) GInit();

            GUpdateGlobalPosition(parent);

            GDraw(nvg);
        }

        virtual void GDraw(NVGcontext *nvg) {};

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

        inline void GMarkInFocus() { infocus = true; }

        // Hierarchy

        virtual void GAttach(GObject *go) {
            go->GAttachTo(this);
            Graphics.push_front(go);
        }

        virtual void GDetach(GObject *go) {
            Graphics.remove(go);
            go->parent = nullptr;
        }

        inline void GAttachTo(GObject *go) {
            this->parent = go;
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
            if (visible && this->GContains(point)) {
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

            for (auto const& root : focusStack) root->GMarkInFocus();

            for (auto const& root : focusStack) root->GRender_(nvg, (float) monitor_.GetCurrentTime());

            nvgEndFrame(nvg);

            bgfx::frame();
        }

        GObject * FindFocusObject(const Vec2 &point) {
            GObject * ret = nullptr;
            for (auto root = focusStack.rbegin(); root != focusStack.rend(); ++root) {
                if ((*root)->GContains(point)) {
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
