#include "scripting.h"

#include <Python.h>
#include <wchar.h>

void scripting_init(wchar_t * program_name);
void scripting_shutdown();

void scripting_init(wchar_t * program_name)
{
    if (!Py_IsInitialized())
    {
        Py_Initialize();
        Py_SetProgramName(program_name);
    }
}

void scripting_shutdown()
{
    if (Py_IsInitialized())
    {
        Py_Initialize();
    }
}

