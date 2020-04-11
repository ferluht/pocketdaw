//
// Created by Admin on 06.07.2019.
//

#ifndef PD_ENCODER_H
#define PD_ENCODER_H

#include <functional>
#include "Knob.h"
#include "styles.h"
#include "Button.h"

namespace GUI {

    class Encoder;

    class EncoderOverlay : public GObject {

        Encoder * enc;

    public:

        EncoderOverlay(Encoder * enc_) : GObject(BOX) {
            enc = enc_;
        }

        void GDraw(NVGcontext * nvg) override;

    };

    class Encoder : public Knob {

    private:

        friend class EncoderButton;
        friend class EncoderOverlay;
        friend class ModulatedEncoder;

        const float wheel_radius = 0.42; // of width
        const float text_height = 0.16; // of space under wheel
        const Vec2 text_position = {0.5, 0.075};
        const Vec2 wheel_center = {0.5, 0.55};
        const float inner_circle_radius = 0.2;
        const int text_max_length = 12;

        float angle = 0;
        float lower_bound, upper_bound;
        float range;
        float old_angle;
        Vec2 drag_from;

        EncoderOverlay * overlay;

        char * label;

        inline float value_range_guard(float value_) {
            if (value_ < lower_bound) value_ = lower_bound;
            if (value_ > upper_bound) value_ = upper_bound;
            return value_;
        }

        inline float angle_range_guard(float angle_) {
            if (angle_ < -1.25f * (float) M_PI) angle_ = -1.25f * (float) M_PI;
            if (angle_ > 0.25f * (float) M_PI) angle_ = 0.25f * (float) M_PI;
            return angle_;
        }

        inline float angle2value(float angle_) {
            angle_ = ((angle_ / (float) M_PI + 0.5f) / 0.75f + 1) / 2 * (upper_bound - lower_bound) + lower_bound;
            return value_range_guard(angle_);
        }

        inline float value2angle(float value_) {
            value_ = (((value_ - lower_bound) / (upper_bound - lower_bound) * 2 - 1) * 0.75f - 0.5f) * (float) M_PI;
            return angle_range_guard(value_);
        }

        inline void setangle(float angle_) {
            angle = angle_range_guard(angle_);
            value = angle2value(angle);
            callback(value);
        }

        std::function<void(float)> callback;

        float value;

    public:

        static const float RATIO;

        Encoder(const char *label_, float default_value_=0, float lower_bound_=-1, float upper_bound_=1,
                std::function<void(float)> callback_=[](float val){},
                unsigned int default_map_=0, unsigned int shape_type_=BOX);

        inline void MIn(MData cmd) override {
            Knob::MIn(cmd);
            if (cmd.status == 0xB0 && keymap && cmd.data1 == keymap) {
                setvalue(cmd.data2 / 127.f * (upper_bound - lower_bound) + lower_bound);
            }
        }

        inline virtual void setvalue(float value_) {
            value = value_range_guard(value_);
            angle = value2angle(value);
            callback(value);
        }

        virtual Encoder &operator=(const float &value_) {
            setvalue(value_);
            return *this;
        }

        virtual Encoder &operator=(const double &value_) {
            setvalue(value_);
            return *this;
        }

        virtual Encoder &operator=(const int &value_) {
            setvalue(value_);
            return *this;
        }

        inline float get() { return value; }

        virtual operator float() { return value; }

        void GSetVisible(bool visible_) override;

        virtual void GDraw(NVGcontext * nvg) override;
    };


    class ModulatedEncoder : public Encoder {

        float base_value;
        float mod_depth;
        float mod;

        bool dragging;

        float old_mod_depth;

    public:

        ModulatedEncoder(const char *label_, float default_value_=0, float lower_bound_=-1, float upper_bound_=1,
                         std::function<void(float)> callback_=[](float val){},
                         unsigned int default_map_=0, unsigned int shape_type_=BOX) :
                         Encoder(label_, default_value_, lower_bound_, upper_bound_, callback_, default_map_, shape_type_) {
            base_value = default_value_;
            mod_depth = 0.3;
            mod = 0;
            dragging = false;

            GSetDragBeginCallback([this](const Vec2& v) -> GUI::GObject * {
                drag_from = v;
                old_angle = angle;
                mapping_mode = true;
                dragging = true;
                old_mod_depth = mod_depth;
                return this;
            });

            GSetDragHandlerCallback([this](const Vec2& v) -> GUI::GObject * {
                overlay->GSetVisible(true);
                angle = old_angle + (v.x - drag_from.x) / 100;
                setangle(angle);
                base_value = (Encoder)(*this);
                mod_depth = old_mod_depth - (v.y - drag_from.y) / 1000;
                if (mod_depth > 1) mod_depth = 1;
                if (mod_depth < 0) mod_depth = 0.01;
                return this;
            });

            GSetDragEndCallback([this](const Vec2& v) -> GUI::GObject * {
                overlay->GSetVisible(false);
                mapping_mode = false;
                dragging = false;
                return this;
            });
        }

        inline void MIn(MData cmd) override {
            Knob::MIn(cmd);
            if (cmd.status == 0xB0 && keymap && cmd.data1 == keymap) {
                base_value = cmd.data2 / 127.f * (upper_bound - lower_bound) + lower_bound;
            }
        }

        ModulatedEncoder &operator=(const float &value_) override {
            base_value = value_range_guard(value_);
            if (!dragging) Encoder::operator=((float)(*this));
            return *this;
        }

        ModulatedEncoder &operator=(const double &value_) override {
            base_value = (float)value_;
            if (!dragging) Encoder::operator=((float)(*this));
            return *this;
        }

        ModulatedEncoder &operator=(const int &value_) override {
            base_value = value_;
            if (!dragging) Encoder::operator=((float)(*this));
            return *this;
        }

        ModulatedEncoder &operator*=(const float &value_) {
            mod_depth = value_;
            return *this;
        }

        ModulatedEncoder &operator<<(const float &value_) {
            mod = value_;
            if (!dragging) Encoder::operator=((float)(*this));
            return *this;
        }

        ModulatedEncoder &operator<<(const int &value_) {
            mod = value_;
            if (!dragging) Encoder::operator=((float)(*this));
            return *this;
        }

        ModulatedEncoder &operator<<(const double &value_) {
            mod = (float)value_;
            if (!dragging) Encoder::operator=((float)(*this));
            return *this;
        }

//        inline float get() { return value; }

        operator float() override { return value_range_guard(base_value + range * mod_depth * mod); }

        virtual void GDraw(NVGcontext * nvg) override;

    };

}

#endif //PD_ENCODER_H
