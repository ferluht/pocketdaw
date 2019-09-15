//
// Created by Admin on 06.07.2019.
//

#ifndef PD_ENCODER_H
#define PD_ENCODER_H

#include <functional>
#include "Knob.h"

class Encoder : public Knob{

private:

    float lower_bound, upper_bound;
    float old_angle;

    inline float angle2value(float angle_) {
        return ((angle_/(float)M_PI+0.5f)/0.75f + 1) / 2 * (upper_bound - lower_bound) + lower_bound;
    }

    inline float value2angle(float value_){
        return (((value_ - lower_bound)/(upper_bound - lower_bound) * 2 - 1) * 0.75f - 0.5f)*(float)M_PI;
    }

    inline void setangle(float angle_){
        if (angle_ < -1.25f*(float)M_PI) {
            angle_ = -1.25f*(float)M_PI;
        }

        if (angle_ > 0.25f*(float)M_PI) {
            angle_ = 0.25f*(float)M_PI;
        }

        wheel->angle = angle_;
        value = angle2value(angle_);
        callback(value);
    }

    std::function<void(float)> callback;

    float value;

    GCanvas * wheel;

public:

    Encoder(wchar_t * label, float default_value_, std::function<void(float)> callback_);
    Encoder(wchar_t * label, float default_value_, std::function<void(float)> callback_, unsigned int default_map_);
    Encoder(wchar_t * label, float default_value_, std::function<void(float)> callback_, float lower_bound_, float upper_bound_);
    Encoder(wchar_t * label, float default_value_, std::function<void(float)> callback_, unsigned int default_map_, float lower_bound_, float upper_bound_);

    inline void MIn(MData cmd) override {
        if (cmd.status == 0xB0 && keymap && cmd.data1 == keymap){
            setvalue(cmd.data2/127.f*(upper_bound - lower_bound) + lower_bound);
        }
    }

    inline void setvalue(float value_){
        if (value_ < lower_bound) value_ = lower_bound;
        if (value_ > upper_bound) value_ = upper_bound;
        value = value_;
        wheel->angle = value2angle(value);
        callback(value);
    }

    Encoder& operator=(const float &value_)
    {
        setvalue(value_);
        return *this;
    }

    Encoder& operator=(const double &value_)
    {
        setvalue(value_);
        return *this;
    }

    Encoder& operator=(const int &value_)
    {
        setvalue(value_);
        return *this;
    }

    operator float() const { return value; }

    void GSetVisible(bool visible_) override ;

    GObject * GDragHandler(const ndk_helper::Vec2& v) override ;
    GObject * GDragBegin(const ndk_helper::Vec2& v) override ;
};


#endif //PD_ENCODER_H
