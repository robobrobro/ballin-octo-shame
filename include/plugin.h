#ifndef __OCTO_PLUGIN_H__
#define __OCTO_PLUGIN_H__

#include <Python.h>
#include "utils/path.h"

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
#define PLUGIN_API_FUNC_LOAD    "load"

typedef struct
{
    PyObject *pModule;
    wchar_t module_name[PATH_MAX_LEN + 1];
    int loaded;
} plugin_t;

PyObject * plugin_call_function(PyObject *pModule, const char *function, PyObject *pArgs, PyObject *pKwargs);

#endif

