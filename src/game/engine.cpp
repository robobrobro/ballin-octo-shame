#include "debug.h"
#include "game/engine.h"
#include "utils/color.h"
#include "utils/path.h"

game::Engine::Engine(game::ctx_t * ctx)
    : engine::Engine::Engine((engine::ctx_t *)ctx)
    , _audio_engine(NULL)
    , _graphics_engine(NULL)
    , _scripting_engine(NULL)
{
    if (!ctx)
    {
        DEBUG_ERROR(L"ctx is NULL\n");
        return;
    }

    /* Initialize graphics engine */
    graphics::ctx_t graphics_engine_ctx;
    this->_graphics_engine = new graphics::Engine(&graphics_engine_ctx);
    if (!this->_graphics_engine)
    {
        DEBUG_ERROR(L"%ls%s%ls\n", COLOR_RED, strerror(errno), COLOR_END);
        return;
    }
    if (!this->_graphics_engine->is_initialized())
    {
        delete this->_graphics_engine;
        this->_graphics_engine = NULL;
        return;
    }
    
    /* Initialize audio engine */
    audio::ctx_t audio_engine_ctx;
    this->_audio_engine = new audio::Engine(&audio_engine_ctx);
    if (!this->_audio_engine)
    {
        DEBUG_ERROR(L"%ls%s%ls\n", COLOR_RED, strerror(errno), COLOR_END);
        return;
    }
    if (!this->_audio_engine->is_initialized())
    {
        delete this->_audio_engine;
        this->_audio_engine = NULL;
        return;
    }
    
    /* Initialize scripting engine */
    scripting::ctx_t scripting_engine_ctx;
    scripting_engine_ctx.program_name = ctx->program_name;
    this->_scripting_engine = new scripting::Engine(&scripting_engine_ctx);
    if (!this->_scripting_engine)
    {
        DEBUG_ERROR(L"%ls%s%ls\n", COLOR_RED, strerror(errno), COLOR_END);
        return;
    }
    if (!this->_scripting_engine->is_initialized())
    {
        delete this->_scripting_engine;
        this->_scripting_engine = NULL;
        return;
    }

    this->_initialized = true;
    DEBUG_INFO(L"game engine initialized successfully\n");
}

game::Engine::~Engine()
{
    if (!this->_initialized) return;

    // Shut down scripting engine
    if (this->_scripting_engine)
    {
        delete this->_scripting_engine;
        this->_scripting_engine = NULL;
    }

    // Shut down audio engine
    if (this->_audio_engine)
    {
        delete this->_audio_engine;
        this->_audio_engine = NULL;
    }
    
    // Shut down graphics engine
    if (this->_graphics_engine)
    {
        delete this->_graphics_engine;
        this->_graphics_engine = NULL;
    }

    this->_initialized = false;
    DEBUG_INFO(L"game engine shut down\n");
}

bool game::Engine::run()
{
    if (!this->_initialized)
    {
        DEBUG_ERROR(L"game not initialized\n");
        return false;
    }

    DEBUG_INFO(L"running game engine\n");

    return true;
}

