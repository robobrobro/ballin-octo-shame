#include "debug.h"
#include "game.h"
#include "utils/string.h"

int main(int argc, char ** argv)
{
    /* Initialize game engine */
    wchar_t program_name[PATH_MAX_LEN + 1] = {0};
    char_to_wchar(argv[0], program_name);
    game_ctx_t game_ctx;
    game_ctx.program_name = program_name;
    Game game(&game_ctx);
    if (!game.is_initialized())
    {
        DEBUG_ERROR(L"initialization failed\n");
        return 1;
    }

    /* Run the game */
    return game.run() ? 0 : 1;
}

