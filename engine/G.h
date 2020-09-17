#ifndef PD_GRAPHIC_OBJECT_H
#define PD_GRAPHIC_OBJECT_H

//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
#ifndef TARGET_IOS
    #include <jni.h>
    #include <android/sensor.h>
    #include <android/log.h>
#endif

#include <errno.h>

#include <list>

#include <cerrno>
#include <set>
#include <string>
#include <mutex>

#include <bgfx/bgfx.h>
#include <vecmath.h>
#include <perfMonitor.h>

#include <nanovg/nanovg.h>

namespace GUI {

    enum SHAPE_TYPES {
        BOX,
        CIRCLE
    };

    class Shape {

        unsigned int type;

    public:

        struct Position {
            vecmath::Vec2 c;
            vecmath::Vec2 s;
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
            if (type_ == CIRCLE) local.ratio = 1;

            global.c = {0, 0};
            global.s = {0, 0};
            global.ratio = 0;
        }

        inline virtual void GPlace(vecmath::Vec2 c_) { local.c = c_; }

        inline virtual void GSetRatio(float ratio_) { local.ratio = ratio_; }

        inline virtual void GSetWidth(float width_) {
            switch (type) {
                case BOX:
                    local.s.x = width_;
                    break;
                case CIRCLE:
                    local.s.x = width_;
                    local.s.y = 0;
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
                    local.s.x = 0;
                    local.s.y = height_;
                    break;
                default:
                    break;
            }
        }

        inline virtual bool GContains(const vecmath::Vec2 &v) {
            switch (type) {
                case BOX:
                    return GContainsX(v) && GContainsY(v);
                case CIRCLE:
                    return sqrt(pow(global.c.x + global.s.x/2 - v.x, 2) + pow(global.c.y + global.s.y/2 - v.y, 2)) < global.s.x / 2;
                default:
                    return false;
            }
        }

        inline virtual bool GContainsX(const vecmath::Vec2 &v) {
            switch (type) {
                case BOX:
                    return (global.c.x < v.x) && (global.c.x + global.s.x > v.x);
                case CIRCLE:
                    return (global.c.x < v.x) && (global.c.x + global.s.x > v.x);
                default:
                    return false;
            }
        }

        inline virtual bool GContainsY(const vecmath::Vec2 &v) {
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

        std::function<GUI::GObject *(const vecmath::Vec2& v)> tap_begin_callback;
        std::function<GUI::GObject *(const vecmath::Vec2& v)> tap_handler_callback;
        std::function<GUI::GObject *(const vecmath::Vec2& v)> tap_end_callback;
        std::function<GUI::GObject *(const vecmath::Vec2& v)> drag_begin_callback;
        std::function<GUI::GObject *(const vecmath::Vec2& v)> drag_handler_callback;
        std::function<GUI::GObject *(const vecmath::Vec2& v)> drag_end_callback;
        std::function<GUI::GObject *(const vecmath::Vec2& v)> double_tap_begin_callback;
        std::function<GUI::GObject *(const vecmath::Vec2& v)> double_tap_handler_callback;
        std::function<GUI::GObject *(const vecmath::Vec2& v)> double_tap_end_callback;
        std::function<GUI::GObject *(const vecmath::Vec2& v)> pinch_begin_callback;
        std::function<GUI::GObject *(const vecmath::Vec2& v)> pinch_handler_callback;
        std::function<GUI::GObject *(const vecmath::Vec2& v)> pinch_end_callback;

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

            tap_begin_callback = nullptr;
            tap_handler_callback = nullptr;
            tap_end_callback = nullptr;

            drag_begin_callback = nullptr;
            drag_handler_callback = nullptr;
            drag_end_callback = nullptr;

            double_tap_begin_callback = nullptr;
            double_tap_handler_callback = nullptr;
            double_tap_end_callback = nullptr;

            pinch_begin_callback = nullptr;
            pinch_handler_callback = nullptr;
            pinch_end_callback = nullptr;
        }

        virtual ~GObject() {}

        virtual void GInit() {};

        void GDraw_(NVGcontext *nvg) {
//            if (!initialized) GInit();

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

        virtual GObject *GFindFocusObject(const vecmath::Vec2 &point, std::list<GObject *> * trace) {
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

        inline void GSetTapBeginCallback(std::function<GUI::GObject *(const vecmath::Vec2& v)> tap_begin_callback_) {
            tap_begin_callback = tap_begin_callback_;
        }

        inline void GSetTapHandlerCallback(std::function<GUI::GObject *(const vecmath::Vec2& v)> tap_handler_callback_) {
            tap_handler_callback = tap_handler_callback_;
        }

        inline void GSetTapEndCallback(std::function<GUI::GObject *(const vecmath::Vec2& v)> tap_end_callback_) {
            tap_end_callback = tap_end_callback_;
        }

        inline void GSetDragBeginCallback(std::function<GUI::GObject *(const vecmath::Vec2& v)> drag_begin_callback_) {
            drag_begin_callback = drag_begin_callback_;
        }

        inline void GSetDragHandlerCallback(std::function<GUI::GObject *(const vecmath::Vec2& v)> drag_handler_callback_) {
            drag_handler_callback = drag_handler_callback_;
        }

        inline void GSetDragEndCallback(std::function<GUI::GObject *(const vecmath::Vec2& v)> drag_end_callback_) {
            drag_end_callback = drag_end_callback_;
        }

        inline void GSetDoubleTapBeginCallback(std::function<GUI::GObject *(const vecmath::Vec2& v)> double_tap_begin_callback_) {
            double_tap_begin_callback = double_tap_begin_callback_;
        }

        inline void GSetDoubleTapHandlerCallback(std::function<GUI::GObject *(const vecmath::Vec2& v)> double_tap_handler_callback_) {
            double_tap_handler_callback = double_tap_handler_callback_;
        }

        inline void GSetDoubleTapEndCallback(std::function<GUI::GObject *(const vecmath::Vec2& v)> double_tap_end_callback_) {
            double_tap_end_callback = double_tap_end_callback_;
        }

        inline void GSetPinchBeginCallback(std::function<GUI::GObject *(const vecmath::Vec2& v)> pinch_begin_callback_) {
            pinch_begin_callback = pinch_begin_callback_;
        }

        inline void GSetPinchHandlerCallback(std::function<GUI::GObject *(const vecmath::Vec2& v)> pinch_handler_callback_) {
            pinch_handler_callback = pinch_handler_callback_;
        }

        inline void GSetPinchEndCallback(std::function<GUI::GObject *(const vecmath::Vec2& v)> pinch_end_callback_) {
            pinch_end_callback = pinch_end_callback_;
        }

        GObject *GTapBegin(const vecmath::Vec2 &v) {
            if (tap_begin_callback) return tap_begin_callback(v);
            return this;
        }

        GObject *GTapHandler(const vecmath::Vec2 &v) {
            if (tap_handler_callback) return tap_handler_callback(v);
            return this;
        }

        GObject *GTapEnd(const vecmath::Vec2 &v) {
            if (tap_end_callback) return tap_end_callback(v);
            return this;
        }

        GObject *GDoubleTapBegin(const vecmath::Vec2 &v) {
            if (double_tap_begin_callback) return double_tap_begin_callback(v);
            return this;
        }

        GObject *GDoubleTapHandler(const vecmath::Vec2 &v) {
            if (double_tap_handler_callback) return double_tap_handler_callback(v);
            return this;
        }

        GObject *GDoubleTapEnd(const vecmath::Vec2 &v) {
            if (double_tap_end_callback) return double_tap_end_callback(v);
            return this;
        }

        GObject *GDragBegin(const vecmath::Vec2 &v) {
            if (drag_begin_callback) return drag_begin_callback(v);
            return this;
        }

        GObject *GDragHandler(const vecmath::Vec2 &v) {
            if (drag_handler_callback) return drag_handler_callback(v);
            return this;
        }

        GObject *GDragEnd(const vecmath::Vec2 &v) {
            if (drag_end_callback) return drag_end_callback(v);
            return this;
        }

        GObject *GPinchBegin(const vecmath::Vec2 &v) {
            if (pinch_begin_callback) return pinch_begin_callback(v);
            return this;
        }

        GObject *GPinchHandler(const vecmath::Vec2 &v) {
            if (pinch_handler_callback) return pinch_handler_callback(v);
            return this;
        }

        GObject *GPinchEnd(const vecmath::Vec2 &v) {
            if (pinch_end_callback) return pinch_end_callback(v);
            return this;
        }

    };


    template <class DstType>
    bool IsType(const GObject* src)
    {
        return dynamic_cast<const DstType*>(src) != nullptr;
    }


    static struct Theme {

        NVGcolor BACKGROUND_COLOR = nvgRGB(14,13,18);
        NVGcolor FULLSCREEN_MENU_BODY_COLOR = nvgRGB(30,30,38);
        NVGcolor HEADER_COLOR = nvgRGB(27,21,35);
        NVGcolor ENCODER_VALUE_ARC_COLOR = nvgRGB(60, 242, 19);
        NVGcolor ENCODER_MODULATION_ARC_COLOR = nvgRGB(0,156,245);
        NVGcolor GRAPH_COLOR = nvgRGB(60, 242, 19);
        NVGcolor PLOT_BODY_COLOR = nvgRGB(24,23,31);
        NVGcolor MIDI_BODY_COLOR = nvgRGB(14,13,20);
        NVGcolor MIDI_DELIMITER_COLOR = nvgRGB(100,100,150);
        NVGcolor MIDI_NOTE_COLOR = nvgRGB(0,156,245);
        NVGcolor IECANVAS_BODY_COLOR = nvgRGB(24,23,31);
        NVGcolor MIDI_EFFECT_BODY_COLOR;
        NVGcolor AUDIO_EFFECT_BODY_COLOR;
        NVGcolor BUTTON_OFF_COLOR = nvgRGBA(0,0,0,0);
        NVGcolor BUTTON_ON_COLOR = nvgRGBA(0,0,0,0);
        NVGcolor BUTTON_TEXT_OFF_COLOR = nvgRGB(80,80,80);
        NVGcolor BUTTON_TEXT_ON_COLOR = nvgRGB(250,250,250);
        NVGcolor BUTTON_OUTLINE_OFF_COLOR = nvgRGBA(0,0,0,0);
        NVGcolor BUTTON_OUTLINE_ON_COLOR = nvgRGBA(0,0,0,0);
        NVGcolor PROGRESS_BUTTON_BODY_COLOR = nvgRGB(60,242,19);
        NVGcolor ENCODER_BODY_COLOR = nvgRGB(56,49,56);
        NVGcolor ENCODER_TEXT_COLOR = nvgRGB(250,250,250);
        NVGcolor ENCODER_OUTLINE_COLOR = nvgRGBA(0,0,0,0);

    } DEFAULT_THEME;


    class GEngine {

    public:

        static float screen_width;
        static float screen_height;
        static float screen_ratio;

        static struct Theme * ui_theme;

        std::mutex renderLock;
        std::mutex overlayLock;

        perfmonitor::PerfMonitor monitor_;

        NVGcontext * nvg;

        std::set<GObject *> overlays;

        std::list<GObject *> focusStack;
        std::list<GObject *> focusStackCopy;

        double render_time = 0;
        double last_render_time = 0;
        float fps;

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

        void unfocusTo(GObject * uf) {
            while ((focusStack.size() != 1) && (focusStack.back() != uf)) {
                focusStack.back()->GLoseFocus();
                focusStack.pop_back();
            }
            if (focusStack.size() != 1) {
                focusStack.back()->GLoseFocus();
                focusStack.pop_back();
            }
        }

        virtual void SetTheme(struct Theme * theme_) {

        }

        void Render() {

            struct timespec res;
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &res);

            render_time = res.tv_sec + (double) res.tv_nsec / 1e9;
            fps = 1 / static_cast<float>(render_time - last_render_time);

            last_render_time = render_time;

            // Set view 0 default viewport.
            bgfx::setViewRect(0, 0, 0, uint16_t(screen_width), uint16_t(screen_height) );

            // This dummy draw call is here to make sure that view 0 is cleared
            // if no other draw calls are submitted to view 0.
            bgfx::touch(0);

            nvgBeginFrame(nvg, float(screen_width), float(screen_height), 1.0f);

            renderLock.lock();

            for (auto const& root : focusStack) focusStackCopy.push_back(root);

            renderLock.unlock();

            for (auto const& root : focusStackCopy) root->GMarkInFocus();

            for (auto const& root : focusStackCopy) root->GRender_(nvg, (float) monitor_.GetCurrentTime());

            focusStackCopy.clear();

            overlayLock.lock();

            for (auto const& overlay : overlays) overlay->GRender_(nvg, (float) monitor_.GetCurrentTime());

            overlayLock.unlock();

            nvgEndFrame(nvg);

            bgfx::frame();
        }

        void addOverlay(GObject * go) {
            overlayLock.lock();
            overlays.insert(go);
            overlayLock.unlock();
        }

        void delOverlay(GObject * go) {
            overlays.erase(go);
        }

        GObject * FindFocusObject(const vecmath::Vec2 &point) {

            renderLock.lock();

            GObject * ret = nullptr;
            for (auto root = focusStack.rbegin(); root != focusStack.rend(); ++root) {
                if ((*root)->GContains(point)) {
                    std::list<GObject *> trace;
                    ret = (*root)->GFindFocusObject(point, &trace);
                    focusStack.erase(std::next(root).base(), focusStack.end());
                    for (auto const& obj : trace) obj->GGainFocus();
                    focusStack.splice(focusStack.end(), trace);
                    renderLock.unlock();
                    return ret;
                } else {
                    (*root)->GLoseFocus();
                }
            }

            renderLock.unlock();
            
            return ret;
        }

        GEngine(const GEngine &) {}

    private:
        // Private constructor
        GEngine() {
            ui_theme = &DEFAULT_THEME;
        }

        ~GEngine() {}

        // Prevent copy-construction
        GEngine &operator=(const GEngine &);      // Prevent assignment
    };

}

#endif
