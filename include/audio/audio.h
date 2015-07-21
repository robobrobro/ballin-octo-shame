#ifndef __OCTO_AUDIO_AUDIO_H__
#define __OCTO_AUDIO_AUDIO_H__

#include "engine.h"

typedef struct : engine_ctx_t
{
} audio_engine_ctx_t;

class AudioEngine : public Engine
{
    public:
        AudioEngine(audio_engine_ctx_t * ctx);
        ~AudioEngine();
};


#endif

