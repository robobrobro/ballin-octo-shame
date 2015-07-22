#ifndef __OCTO_AUDIO_ENGINE_H__
#define __OCTO_AUDIO_ENGINE_H__

#include <memory>
#include "engine/engine.h"
#include "scripting/engine.h"

namespace audio
{

struct ctx_t : public engine::ctx_t
{
    std::shared_ptr<scripting::Engine> scripting_engine;
};

class Engine : public engine::Engine
{
    public:
        Engine(std::shared_ptr<audio::ctx_t> ctx);
        ~Engine();
};

}   // namespace audio

#endif

