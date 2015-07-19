#ifndef __OCTO_PLUGIN_H__
#define __OCTO_PLUGIN_H__

#include <Python.h>

/* Plugin API functions */
#define PLUGIN_API_FUNC_LOAD    "load"

PyObject * plugin_call_function(PyObject *pModule, const char *function, PyObject *pArgs, PyObject *pKwargs);

#endif

