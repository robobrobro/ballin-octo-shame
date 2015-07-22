#ifndef __OCTO_GAME_ENGINE_H__
#define __OCTO_GAME_ENGINE_H__

#include <memory>
#include <string>
#include "engine/engine.h"
#include "audio/engine.h"
#include "graphics/engine.h"
#include "scripting/engine.h"

namespace game
{

struct ctx_t : public engine::ctx_t
{
    std::wstring program_name;
};

class Engine : public engine::Engine
{
    public:
        Engine(std::shared_ptr<game::ctx_t> ctx);
        ~Engine();

        bool run();

    protected:
        std::shared_ptr<audio::Engine> _audio_engine;
        std::shared_ptr<graphics::Engine> _graphics_engine;
        std::shared_ptr<scripting::Engine> _scripting_engine;
};

}   // namespace game

#endif

