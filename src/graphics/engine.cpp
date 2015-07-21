#include "debug.h"
#include "graphics/engine.h"

graphics::Engine::Engine(graphics::ctx_t * ctx)
    : engine::Engine::Engine((engine::ctx_t *)ctx)
{
    this->_initialized = true;
    DEBUG_INFO(L"graphics engine initialized\n");
}

graphics::Engine::~Engine()
{
    DEBUG_INFO(L"graphics engine shut down successfully\n");
}

