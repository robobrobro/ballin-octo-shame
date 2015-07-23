#include "debug.h"
#include "scripting/plugin.h"
#include "utils/color.h"
        
scripting::plugin::Plugin::Plugin() : _module(NULL), _loaded(false)
{
}

scripting::plugin::Plugin::Plugin(PyObject * module, const std::wstring & name)
    : _module(module), _module_name(name), _loaded(false)
{
}

scripting::plugin::Plugin::~Plugin()
{
    Py_XDECREF(this->_module);
}

PyObject * scripting::plugin::Plugin::call(const std::string & function, PyObject * pArgs, PyObject * pKwargs) const
{
    PyObject *pFunction = NULL, *pResult = NULL;
    
    if (!this->_module) return NULL;

    DEBUG_DEBUG(L"calling module function: %ls\n", this->name(function).c_str());

    /* Call module's load function, if it has one */
    if (PyObject_HasAttrString(this->_module, function.c_str()))
    {
        pFunction = PyObject_GetAttrString(this->_module, function.c_str());
        if (!pFunction)
        {
            DEBUG_ERROR(L"failed to get module function: %ls\n", this->name(function).c_str());
            return NULL;
        }

        if (!PyCallable_Check(pFunction))
        {
            DEBUG_ERROR(L"module function %ls is not callable\n", this->name(function).c_str());
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
            DEBUG_ERROR(L"call to module function %ls failed\n", this->name(function).c_str());
            return NULL;
        }

    }
    else
    {
        /* The module doesn't have a load function, so return True as if it loaded successfully */
        DEBUG_DEBUG(L"module function %ls not found\n", this->name(function).c_str());
        Py_INCREF(Py_True);
        pResult = Py_True;
    }
            
    return pResult;
}

std::wstring scripting::plugin::Plugin::name(void) const
{
    wchar_t mbstr[100] = {0}; 
    swprintf(mbstr, sizeof(mbstr) - 1, L"%ls%ls%ls", COLOR_CYAN, this->_module_name.c_str(),
            COLOR_END); 
    std::wstring name = mbstr;
    return name;
}

std::wstring scripting::plugin::Plugin::name(const std::string & function) const
{
    wchar_t mbstr[100] = {0}; 
    swprintf(mbstr, sizeof(mbstr) - 1, L"%ls%ls.%ls%s%ls", COLOR_CYAN, this->_module_name.c_str(),
            COLOR_BLUE, function.c_str(), COLOR_END); 
    std::wstring name = mbstr;
    return name;
}
