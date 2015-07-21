#include "debug.h"
#include "graphics/graphics.h"

GraphicsEngine::GraphicsEngine(graphics_engine_ctx_t * ctx)
    : Engine::Engine((engine_ctx_t *)ctx)
{
    this->_initialized = true;
    DEBUG_INFO(L"graphics engine initialized\n");
}

GraphicsEngine::~GraphicsEngine()
{
    DEBUG_INFO(L"graphics engine shut down successfully\n");
}

