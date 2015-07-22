#include "debug.h"
#include "game/engine.h"
#include "game/module.h"
#include "graphics/window.h"
#include "utils/color.h"
#include "utils/path.h"

game::Engine::Engine(std::shared_ptr<game::ctx_t> ctx)
    : engine::Engine::Engine(ctx)
    , _audio_engine(NULL)
    , _graphics_engine(NULL)
    , _scripting_engine(NULL)
{
    DEBUG_DEBUG(L"initializing game engine\n");

    /* Initialize scripting engine */
    auto scripting_engine_ctx = std::make_shared<scripting::ctx_t>();
    scripting_engine_ctx->program_name = ctx->program_name;
    scripting_engine_ctx->modules.push_back(std::make_shared<game::Module>());
    this->_scripting_engine = std::make_shared<scripting::Engine>(scripting_engine_ctx);
    if (!this->_scripting_engine->is_initialized()) return;

    /* Initialize graphics engine */
    auto graphics_engine_ctx = std::make_shared<graphics::ctx_t>();
    graphics_engine_ctx->scripting_engine = this->_scripting_engine;
    auto window = std::make_shared<graphics::Window>(L"octo test", sf::VideoMode(800, 600));// TODO load from plugin main.py or similar
    graphics_engine_ctx->window = window;
    this->_graphics_engine = std::make_shared<graphics::Engine>(graphics_engine_ctx);
    if (!this->_graphics_engine->is_initialized()) return;
    
    /* Initialize audio engine */
    auto audio_engine_ctx = std::make_shared<audio::ctx_t>();
    audio_engine_ctx->scripting_engine = this->_scripting_engine;
    this->_audio_engine = std::make_shared<audio::Engine>(audio_engine_ctx);
    if (!this->_audio_engine->is_initialized()) return;
    
    // Load plugins. This is done after all the engines are initialized so that the modules' load
    // functions can access the engines.
    if (!this->_scripting_engine->load_plugins()) return;
    
    this->_initialized = true;
    DEBUG_DEBUG(L"game engine initialized successfully\n");
}

game::Engine::~Engine()
{
    DEBUG_DEBUG(L"shutting down game engine\n");
    this->_initialized = false;
    DEBUG_DEBUG(L"game engine shut down\n");
}

bool game::Engine::run()
{
    if (!this->_initialized)
    {
        DEBUG_ERROR(L"game not initialized\n");
        return false;
    }

    DEBUG_DEBUG(L"running game engine\n");

    // TODO run each engine in its own thread (if possible) and join
    this->_graphics_engine->run();

    return true;
}

