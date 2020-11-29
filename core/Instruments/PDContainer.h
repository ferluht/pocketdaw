//
// Created by ferluht on 16/10/2020.
//

#ifndef ANDROID_PDCONTAINER_H
#define ANDROID_PDCONTAINER_H

#include "Instrument.h"
//#include <libpd/cpp/PdBase.hpp>
#include <libpd/libpd_wrapper/z_libpd.h>
#include <GUI/Encoder.h>

class PDContainer : public Instrument<InstrumentState> {
public:

    float inbuf[2][128], outbuf[2][128];
    int scounter = 0;
    int ibuf = 0;
    int active_buffer_index = 0;

    std::thread * pd_thread;
    std::mutex run_lock;
    std::mutex stop_lock;
    std::mutex end_lock;
    std::mutex destroy_lock;

    std::vector<GUI::AnalogEncoder *> encoders;

    PDContainer(char * file, char * dir) : Instrument<InstrumentState> (1, "PDContainer") {
//        pd::PdBase pdbase;
//        pd::PdObject pdObject;
//        pd::libpd_init_audio(1, 2, sample_rate);

        GSetRatio(0.7);

        libpd_init();
        libpd_init_audio(2, 2, sample_rate);

        libpd_start_message(1); // one entry in list
        libpd_add_float(1.0f);
        libpd_finish_message("pd", "dsp");

        // open patch       [; pd open file folder(
        libpd_openfile(file, dir);

        for (int i = 0; i < 6; i++) {
            char recv[20] = "";
            sprintf(recv, "enc%d", i + 1);
            if (libpd_exists(recv)) {
                encoders.push_back(new GUI::AnalogEncoder(recv, 0, 0, 1));
                encoders.back()->GSetHeight(0.25);
                encoders.back()->GPlace({(i / 3) * 0.3f + 0.05f, (i % 3) * 0.3f});
                GAttach(encoders.back());
                MConnect(encoders.back());
            }
        }

        run_lock.lock();
        stop_lock.unlock();
        end_lock.unlock();
        destroy_lock.lock();
        pd_thread = new std::thread(PDThread, this);
    }

    ~PDContainer() {
        destroy_lock.unlock();
    }

    void PDThreadWait() {
        end_lock.lock();
        end_lock.unlock();
    }

    void MIn(MData cmd) override {
        switch (cmd.status & 0xF0) {
            case NOTEON_HEADER:
            case NOTEOFF_HEADER:
                libpd_noteon(cmd.status & 0x0F, cmd.data1, cmd.data2);
                break;
            default:
                break;
        }
        MOut(cmd);
    }

    inline void MRender(double beat) override {
        for (auto const& obj : encoders) obj->MRender(beat);
    }

    bool ARender(double beat, float * lsample, float * rsample) override {
        if (scounter == 0) {
            PDThreadWait();
            stop_lock.lock();
            active_buffer_index = ibuf;

            for (auto const& enc : encoders) {
                if (enc->val_changed)
                    libpd_float(enc->label, *enc);
            }

            stop_lock.unlock();
            end_lock.lock();
            run_lock.unlock();

//            libpd_process_float(1, inbuf[ibuf], outbuf[ibuf]);

            ibuf ^= 1;
        }
        inbuf[ibuf][scounter*2] = *lsample;
        inbuf[ibuf][scounter*2 + 1] = *rsample;
        *lsample = outbuf[ibuf][2*scounter];
        *rsample = outbuf[ibuf][2*scounter + 1];
        scounter = (scounter + 1) % 64;
        return 1;
    }

    static void PDThread(PDContainer * container) {
        while(!container->destroy_lock.try_lock()) {
            container->run_lock.lock();
            container->stop_lock.lock();

            libpd_process_float(1, container->inbuf[container->active_buffer_index],
                    container->outbuf[container->active_buffer_index]);

            container->stop_lock.unlock();
            container->end_lock.unlock();
        }
    }
};


#endif //ANDROID_PDCONTAINER_H
