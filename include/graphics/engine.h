#ifndef __OCTO_GRAPHICS_ENGINE_H__
#define __OCTO_GRAPHICS_ENGINE_H__

#include <SFML/Graphics.hpp>
#include "engine/engine.h"

namespace graphics
{

struct ctx_t : public engine::ctx_t
{
};

class Engine : public engine::Engine
{
    public:
        Engine(graphics::ctx_t * ctx);
        ~Engine();

        bool run(void);

    protected:
        sf::RenderWindow * _window;
};

}   // namespace graphics

#endif

