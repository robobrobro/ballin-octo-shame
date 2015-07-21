#ifndef __OCTO_GRAPHICS_ENGINE_H__
#define __OCTO_GRAPHICS_ENGINE_H__

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
};

}   // namespace graphics

#endif

