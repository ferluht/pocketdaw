//
// Created by Admin on 08.06.2019.
//

#ifndef PD_AUDIOENGINE_H
#define PD_AUDIOENGINE_H


#include <aaudio/AAudio.h>
#include "Orchestration/Master.h"

class AudioEngine {
public:

	AudioEngine(Master * master);

    bool start();
    void stop();
    void restart();
    
private:
    Master * master_;
    AAudioStream *stream_;
};


#endif //PD_AUDIOENGINE_H
