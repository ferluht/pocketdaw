//
// Created by ibelikov on 30.12.19.
//

#ifndef PD_ANALOGENCODER_H
#define PD_ANALOGENCODER_H


#include "Encoder.h"
#include "Jack.h"

namespace GUI {

    class AnalogEncoder : public ModulatedEncoder {

//        Encoder * encoder;
        Jack * jack;

    public:

        AnalogEncoder(const char *label_, float default_value_=0, float lower_bound_=-1, float upper_bound_=1,
                      std::function<void(float)> callback_=[](float val){},
                      unsigned int default_map_=0, unsigned int shape_type_=BOX) :
                ModulatedEncoder(label_, default_value_, lower_bound_, upper_bound_, callback_, default_map_, shape_type_) {
//            GSetRatio(Encoder::RATIO + Jack::RATIO);

//            encoder = new Encoder(label_, 0);
//            encoder->GPlace({0, 0});
//            encoder->GSetHeight(1);
//            GAttach(encoder);

            jack = new Jack(Jack::INPUT);
            jack->GPlace({0.5, 0.54});
            jack->GSetWidth(0.5);
            GAttach(jack);
        }

        void MRender(double beat) override {
            if (jack->isConnected()) *this << *jack;
            else *this << 0;
        }

        GObject *GFindFocusObject(const Vec2 &point, std::list<GObject *> * trace) override {
            if (jack->visible && jack->GContains(point)) {
                trace->push_front(jack);
                trace->push_front(this);
                return jack;
            }
            if (visible && GContains(point)) {
                trace->push_front(this);
                return this;
            }
            return nullptr;
        }

    };

}

#endif //PD_ANALOGENCODER_H
