#include "debug.h"
#include "audio/audio.h"

AudioEngine::AudioEngine(audio_engine_ctx_t * ctx)
    : Engine::Engine((engine_ctx_t *)ctx)
{
    this->_initialized = true;
    DEBUG_INFO(L"audio engine initialized\n");
}

AudioEngine::~AudioEngine()
{
    DEBUG_INFO(L"audio engine shut down successfully\n");
}

