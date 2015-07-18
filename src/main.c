#include <stdlib.h>
#include "debug.h"
#include "scripting.h"
#include "utils/path.h"
#include "utils/string.h"

static int init(int argc, char ** argv);
static void shutdown(void);

int main(int argc, char ** argv)
{
    if (!init(argc, argv))
    {
        DEBUG_ERROR(L"initialization failed\n");
        return 1;
    }
    
    return 0;
}

static int init(int argc, char ** argv)
{
    wchar_t program_name[260] = {0};

    atexit(shutdown);

    /* Initialize scripting engine */
    char_to_wchar(argv[0], program_name);
    if (!scripting_init(program_name)) return 0;

    DEBUG_INFO(L"initialization succeeded\n");

    return 1;
}

static void shutdown(void)
{
    scripting_shutdown();
}

