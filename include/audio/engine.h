#ifndef __OCTO_AUDIO_ENGINE_H__
#define __OCTO_AUDIO_ENGINE_H__

#include "engine/engine.h"

namespace audio
{

struct ctx_t : public engine::ctx_t
{
};

class Engine : public engine::Engine
{
    public:
        Engine(audio::ctx_t * ctx);
        ~Engine();
};

}   // namespace audio

#endif

