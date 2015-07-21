#ifndef __OCTO_SCRIPTING_PLUGIN_H__
#define __OCTO_SCRIPTING_PLUGIN_H__

#include <Python.h>
#include <string>

#include "utils/path.h"

namespace scripting
{

namespace plugin
{

namespace functions
{

/* Plugin API functions */

/********************************************************************
 *  def load(*args, **kwargs)
 * 
 *  DESCRIPTION
 *      called once when the module is first loaded
 *  RETURN
 *      Boolean: True if successful. If this function returns False,
 *      the module is flagged as not ready and will not be called
 *      again until it is reloaded
 *******************************************************************/
const std::string LOAD = "load";

}   // namespace functions

typedef struct _plugin
{
    PyObject *pModule;
    wchar_t module_name[PATH_MAX_LEN + 1];
    bool loaded;

    _plugin() : pModule(NULL), loaded(false)
    {
        memset(module_name, 0, sizeof(module_name));
    }

    _plugin(PyObject * module, wchar_t * name)
        : pModule(module), loaded(false)
    {
        memset(module_name, 0, sizeof(module_name));
        if (name)
        {
            wcsncpy(module_name, name, PATH_MAX_LEN);
        }
    }

    ~_plugin()
    {
        Py_XDECREF(pModule);
        pModule = NULL;
        memset(module_name, 0, sizeof(module_name));
    }

    PyObject * call(const std::string & function, PyObject * pArgs, PyObject * pKwargs) const;
} plugin_t;

}   // namespace plugin

}   // namespace scripting

#endif

