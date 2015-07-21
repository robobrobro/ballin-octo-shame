#include "debug.h"
#include "scripting/plugin.h"
#include "utils/color.h"

PyObject * plugin_call_function(PyObject *pModule, const char *function, PyObject *pArgs, PyObject *pKwargs)
{
    PyObject *pModuleName = NULL, *pFunction = NULL, *pResult = NULL;
    wchar_t *module_name = NULL;
    Py_ssize_t module_name_len = 0;

    pModuleName = PyObject_GetAttrString(pModule, "__name__");
    if (pModuleName)
    {
        module_name = PyUnicode_AsWideCharString(pModuleName, &module_name_len);
        Py_DECREF(pModuleName);
    }

    DEBUG_DEBUG(L"calling module function: %ls%ls.%s%ls\n", COLOR_YELLOW,
            module_name ? module_name : L"<unknown>", PLUGIN_API_FUNC_LOAD, COLOR_END);

    /* Call module's load function, if it has one */
    if (PyObject_HasAttrString(pModule, function))
    {
        pFunction = PyObject_GetAttrString(pModule, function);
        if (!pFunction)
        {
            DEBUG_ERROR(L"failed to get module function: %ls%ls.%s%ls\n", COLOR_RED,
                    module_name ? module_name : L"<unknown>", PLUGIN_API_FUNC_LOAD, COLOR_END);
            if (module_name) PyMem_Free(module_name);
            return NULL;
        }

        if (!PyCallable_Check(pFunction))
        {
            DEBUG_ERROR(L"module function %ls%ls.%s%ls is not callable\n", COLOR_RED,
                    module_name ? module_name : L"<unknown>", PLUGIN_API_FUNC_LOAD, COLOR_END);
            if (module_name) PyMem_Free(module_name);
            return NULL;
        }

        if (!pArgs)
        {
            pArgs = PyTuple_New(0);
            if (!pArgs)
            {
                if (module_name) PyMem_Free(module_name);
                Py_DECREF(pFunction);
                DEBUG_ERROR(L"failed to create a new tuple\n");
                return NULL;
            }
        }

        pResult = PyObject_Call(pFunction, pArgs, pKwargs);
        Py_DECREF(pFunction);
        Py_DECREF(pArgs);

        if (!pResult)
        {
            DEBUG_ERROR(L"call to module function %ls%ls.%s%ls failed\n", COLOR_RED,
                    module_name ? module_name : L"<unknown>", PLUGIN_API_FUNC_LOAD, COLOR_END);
            if (module_name) PyMem_Free(module_name);
            return NULL;
        }

    }
    else
    {
        /* The module doesn't have a load function, so return True as if it loaded successfully */
        DEBUG_DEBUG(L"module function %ls%ls.%s%ls not found\n",
                    COLOR_YELLOW, module_name ? module_name : L"<unknown>",
                    PLUGIN_API_FUNC_LOAD, COLOR_END);
        Py_INCREF(Py_True);
        pResult = Py_True;
    }
            
    if (module_name) PyMem_Free(module_name);

    return pResult;
}

