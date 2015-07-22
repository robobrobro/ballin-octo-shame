#include "debug.h"
#include "game/engine.h"
#include "utils/string.h"

int main(int argc, char ** argv)
{
    /* Initialize game engine */
    auto game_ctx = std::make_shared<game::ctx_t>();
    game_ctx->program_name = utils::string::cstr_to_wstr(argv[0]);
    game::Engine game(game_ctx);
    if (!game.is_initialized())
    {
        DEBUG_ERROR(L"initialization failed\n");
        return 1;
    }

    /* Run the game */
    return game.run() ? 0 : 1;
}

