#ifndef __OCTO_GRAPHICS_ENGINE_H__
#define __OCTO_GRAPHICS_ENGINE_H__

#include <memory>
#include <SFML/Graphics.hpp>
#include <string>
#include "engine/engine.h"
#include "graphics/window.h"
#include "scripting/engine.h"

namespace graphics
{

struct ctx_t : public engine::ctx_t
{
    std::shared_ptr<scripting::Engine> scripting_engine;
    std::shared_ptr<Window> window;
};

class Engine : public engine::Engine
{
    public:
        Engine(std::shared_ptr<graphics::ctx_t> ctx);
        ~Engine();

        bool run(void);

    protected:
        std::shared_ptr<Window> _window;
};

}   // namespace graphics

#endif

