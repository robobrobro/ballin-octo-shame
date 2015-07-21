#include "debug.h"
#include "scripting/plugin.h"
#include "utils/color.h"

PyObject * scripting::plugin::plugin_t::call(const std::string & function, PyObject * pArgs, PyObject * pKwargs) const
{
    PyObject *pFunction = NULL, *pResult = NULL;
    
    if (!this->pModule) return NULL;

    DEBUG_DEBUG(L"calling module function: %ls%ls.%s%ls\n", COLOR_YELLOW,
            this->module_name, function.c_str(), COLOR_END);

    /* Call module's load function, if it has one */
    if (PyObject_HasAttrString(pModule, function.c_str()))
    {
        pFunction = PyObject_GetAttrString(pModule, function.c_str());
        if (!pFunction)
        {
            DEBUG_ERROR(L"failed to get module function: %ls%ls.%s%ls\n", COLOR_RED,
                    this->module_name, function.c_str(), COLOR_END);
            return NULL;
        }

        if (!PyCallable_Check(pFunction))
        {
            DEBUG_ERROR(L"module function %ls%ls.%s%ls is not callable\n", COLOR_RED,
                    this->module_name, function.c_str(), COLOR_END);
            return NULL;
        }

        if (!pArgs)
        {
            pArgs = PyTuple_New(0);
            if (!pArgs)
            {
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
                    this->module_name, function.c_str(), COLOR_END);
            return NULL;
        }

    }
    else
    {
        /* The module doesn't have a load function, so return True as if it loaded successfully */
        DEBUG_DEBUG(L"module function %ls%ls.%s%ls not found\n",
                    COLOR_YELLOW, this->module_name, function.c_str(), COLOR_END);
        Py_INCREF(Py_True);
        pResult = Py_True;
    }
            
    return pResult;
}

