#ifndef __OCTO_DEBUG_H__
#define __OCTO_DEBUG_H__

#ifdef DEBUG
#   include <Python.h>
#   include <wchar.h>
#   include "utils/time.h"
#   define DEBUG_DEBUG(...) \
    fwprintf(stderr, L"\033[33mDEBUG\033[0m %s \033[92m%s:%d:%s\033[0m ", utils::time::timestamp().c_str(), __FILE__, __LINE__, __FUNCTION__);\
    fwprintf(stderr, __VA_ARGS__);
#   define DEBUG_INFO(...) \
    fwprintf(stderr, L"\033[37mINFO \033[0m %s \033[92m%s:%d:%s\033[0m ", utils::time::timestamp().c_str(), __FILE__, __LINE__, __FUNCTION__);\
    fwprintf(stderr, __VA_ARGS__); 
#   define DEBUG_ERROR(...) \
    fwprintf(stderr, L"\033[31mERROR\033[0m %s \033[92m%s:%d:%s\033[0m ", utils::time::timestamp().c_str(), __FILE__, __LINE__, __FUNCTION__);\
    fwprintf(stderr, __VA_ARGS__); \
    if (PyErr_Occurred()) PyErr_Print();
#else
#   define DEBUG_DEBUG(...)
#   define DEBUG_INFO(...)
#   define DEBUG_ERROR(...)
#   define DEBUG_ERROR_PYTHON()
#endif

#endif

