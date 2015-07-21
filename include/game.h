#ifndef __OCTO_GAME_H__
#define __OCTO_GAME_H__

#include <wchar.h>
#include "engine.h"
#include "audio/audio.h"
#include "graphics/graphics.h"
#include "scripting/scripting.h"

typedef struct : engine_ctx_t
{
    wchar_t * program_name;
} game_ctx_t;

class Game : public Engine
{
    public:
        Game(game_ctx_t * ctx);
        ~Game();

        bool run();

    protected:
        AudioEngine * _audio_engine;
        GraphicsEngine * _graphics_engine;
        ScriptingEngine * _scripting_engine;
};

#endif

