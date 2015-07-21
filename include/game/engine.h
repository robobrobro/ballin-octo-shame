#ifndef __OCTO_GAME_ENGINE_H__
#define __OCTO_GAME_ENGINE_H__

#include <wchar.h>
#include "engine/engine.h"
#include "audio/engine.h"
#include "graphics/engine.h"
#include "scripting/engine.h"

namespace game
{

struct ctx_t : public engine::ctx_t
{
    wchar_t * program_name;
};

class Engine : public engine::Engine
{
    public:
        Engine(game::ctx_t * ctx);
        ~Engine();

        bool run();

    protected:
        audio::Engine * _audio_engine;
        graphics::Engine * _graphics_engine;
        scripting::Engine * _scripting_engine;
};

}   // namespace game

#endif

