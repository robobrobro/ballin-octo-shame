#include <stdlib.h>
#include "scripting.h"

int main(int argc, char ** argv);
int init(int argc, char ** argv);
void shutdown(void);

int main(int argc, char ** argv)
{
    if (!init(argc, argv)) return 1;
    return 0;
}

int init(int argc, char ** argv)
{
    atexit(shutdown);

    scripting_init(L"octo");

    return 1;
}

void shutdown(void)
{
    scripting_shutdown();
}

