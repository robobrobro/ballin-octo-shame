#include "debug.h"
#include "audio/engine.h"

audio::Engine::Engine(audio::ctx_t * ctx)
    : engine::Engine::Engine((engine::ctx_t *)ctx)
{
    this->_initialized = true;
    DEBUG_INFO(L"audio engine initialized\n");
}

audio::Engine::~Engine()
{
    DEBUG_INFO(L"audio engine shut down successfully\n");
}

