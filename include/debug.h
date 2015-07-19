#ifndef __OCTO_DEBUG_H__
#define __OCTO_DEBUG_H__

#ifdef DEBUG
#   include <Python.h>
#   include <wchar.h>
#   define DEBUG_DEBUG(...) \
    fwprintf(stderr, L"\033[1mDEBUG\033[0m %s:%s:%d ", __FILE__, __FUNCTION__, __LINE__);\
    fwprintf(stderr, __VA_ARGS__);
#   define DEBUG_INFO(...) \
    fwprintf(stderr, L"\033[37mINFO\033[0m %s:%s:%d ", __FILE__, __FUNCTION__, __LINE__);\
    fwprintf(stderr, __VA_ARGS__); 
#   define DEBUG_ERROR(...) \
    fwprintf(stderr, L"\033[31mERROR\033[0m %s:%s:%d ", __FILE__, __FUNCTION__, __LINE__);\
    fwprintf(stderr, __VA_ARGS__); \
    if (PyErr_Occurred()) PyErr_Print();
#else
#   define DEBUG_DEBUG(...)
#   define DEBUG_INFO(...)
#   define DEBUG_ERROR(...)
#   define DEBUG_ERROR_PYTHON()
#endif

#endif

