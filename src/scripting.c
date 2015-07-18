#include "scripting.h"
#include "debug.h"
#include "utils/path.h"

#include <Python.h>

static void debug_python_info(void);

int scripting_init(wchar_t * program_name)
{
    wchar_t *program_dir = NULL, *plugins_dir = NULL;
    int retval = 0;
        
    if (!Py_IsInitialized())
    {
        /* Set Python interpreter's program name */
        Py_SetProgramName(program_name);

        program_dir = path_get_dir(program_name);
        if (!program_dir)
        {
            DEBUG_ERROR(L"failed to get directory path to the program\n");
            return 0;
        }

        /* Set Python interpreter's module search path */
        plugins_dir = path_join(program_dir, L"plugins", NULL);
        free(program_dir);
        
        if (!plugins_dir)
        {
            DEBUG_ERROR(L"error: failed to get plugins path\n");
            return 0;
        }

        Py_SetPath(plugins_dir);
        
        /* Initialize Python interpreter */
        Py_Initialize();
        //Py_InitializeEx(0);
        
        debug_python_info();

        retval = 1;
    }
    else
    {
        DEBUG_ERROR(L"python already initialized\n");    
    }

    return retval;
}

void scripting_shutdown(void)
{
    if (Py_IsInitialized())
    {
        Py_Initialize();
    }
    else
    {
        DEBUG_ERROR(L"python not initialized\n");
    }
}

static void debug_python_info(void)
{
#ifdef DEBUG
    wchar_t * tmp = NULL;
    int idx = 0;
    size_t len = 0;
    char version[1000] = {0}, *compiler = NULL;

    DEBUG_DEBUG(L"Python scripting engine initialized\n");
    tmp = Py_GetProgramName();
    if (tmp)
        DEBUG_DEBUG(L"\033[33m%-20ls\033[0m %ls\n", L"Program Name", tmp);
    tmp = Py_GetProgramFullPath();
    if (tmp)
        DEBUG_DEBUG(L"\033[33m%-20ls\033[0m %ls\n", L"Program Full Path", tmp);
    strncpy(version, Py_GetVersion(), sizeof(version) - 1);
    len = strlen(version);
    for (idx = 0; idx < len; ++idx)
    {
        if (version[idx] == '\n')
        {
            version[idx] = 0;
            compiler = &version[idx + 1];
            break;
        }
    }
    DEBUG_DEBUG(L"\033[33m%-20ls\033[0m %s\n", L"Version", version);
    if (compiler)
        DEBUG_DEBUG(L"\033[33m%-20ls\033[0m %s\n", L"Compiler", compiler);
    tmp = Py_GetPath();
    if (tmp)
        DEBUG_DEBUG(L"\033[33m%-20ls\033[0m %ls\n", L"Module Path", tmp);
    tmp = Py_GetPrefix();
    if (tmp)
        DEBUG_DEBUG(L"\033[33m%-20ls\033[0m %ls\n", L"Prefix", tmp);
    tmp = Py_GetExecPrefix();
    if (tmp)
        DEBUG_DEBUG(L"\033[33m%-20ls\033[0m %ls\n", L"Exec Prefix", tmp);
    DEBUG_DEBUG(L"\033[33m%-20ls\033[0m %s\n", L"Platform", Py_GetPlatform());
#endif
}

