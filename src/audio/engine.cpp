#include "debug.h"
#include "audio/engine.h"

audio::Engine::Engine(std::shared_ptr<audio::ctx_t> ctx)
    : engine::Engine::Engine(ctx)
{
    DEBUG_DEBUG(L"initializing audio engine\n");
    this->_initialized = true;
    DEBUG_DEBUG(L"audio engine initialized\n");
}

audio::Engine::~Engine()
{
    DEBUG_DEBUG(L"shutting down audio engine\n");
    this->_initialized = false;
    DEBUG_DEBUG(L"audio engine shut down successfully\n");
}

