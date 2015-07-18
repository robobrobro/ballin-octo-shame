#include "scripting.h"
#include "debug.h"
#include "utils/path.h"
#include "utils/string.h"

#include <Python.h>
#include <math.h>

static PyObject *g_module_list = NULL;
static void debug_python_info(void);

int scripting_init(wchar_t * program_name)
{
    wchar_t *program_dir = NULL, *builtins_dir = NULL, *plugins_dir = NULL;
    PyObject *pSysPath = NULL, *pPluginsDir = NULL;
        
    if (Py_IsInitialized())
    {
        DEBUG_ERROR(L"python already initialized\n");    
        return 0;
    }

    /* Set Python interpreter's program name */
    Py_SetProgramName(program_name);

    program_dir = path_get_dir(program_name);
    if (!program_dir)
    {
        DEBUG_ERROR(L"failed to get directory path to the program\n");
        return 0;
    }

    /* Set Python interpreter's module search path */
    builtins_dir = path_join(program_dir, L".builtins", NULL); 
    
    if (!builtins_dir)
    {
        free(program_dir);
        DEBUG_ERROR(L"failed to get builtins path\n");
        return 0;
    }

    Py_SetPath(builtins_dir);
    
    /* Initialize Python interpreter */
    Py_Initialize();
    //Py_InitializeEx(0);
    DEBUG_INFO(L"Python scripting engine initialized\n");
    
    /* Append plugins directory to Python's sys.path */
    pSysPath = PySys_GetObject("path");
    if (!pSysPath)
    {
        free(program_dir);
        /* TODO get exception */
        DEBUG_ERROR(L"failed to get Python sys.path\n");
        return 0;
    }

    plugins_dir = path_join(program_dir, L"plugins", NULL); 
    free(program_dir);
    
    if (!plugins_dir)
    {
        DEBUG_ERROR(L"failed to get plugins path\n");
        return 0;
    }

    pPluginsDir = PyUnicode_FromWideChar(plugins_dir, wcslen(plugins_dir));
    free(plugins_dir);

    if (!pPluginsDir)
    {
        /* TODO get exception */
        DEBUG_ERROR(L"failed to decode plugins path\n");
        return 0;
    }

    if (PyList_Append(pSysPath, pPluginsDir) < 0)
    {
        DEBUG_ERROR(L"failed to append plugins path to Python sys.path\n");
        Py_DECREF(pPluginsDir);
        return 0;
    }
        
    Py_DECREF(pPluginsDir);

    /* Create modules list */
    g_module_list = PyList_New(0);

    debug_python_info();

    return 1;
}

void scripting_shutdown(void)
{
    /* Finalize Python interpreter */
    if (!Py_IsInitialized())
    {
        DEBUG_ERROR(L"python not initialized\n");
        return;
    }

    /* Destroy module list */
    if (g_module_list)
    {
        Py_DECREF(g_module_list);
        g_module_list = NULL;
    }

    Py_Finalize();
}

int scripting_load(const char *module)
{
    PyObject *pModuleName = NULL, *pModule = NULL, *pOtherModule = NULL, *pOtherModuleName = NULL;
    PyObject *pAttrName = NULL;
    Py_ssize_t module_list_len = 0, module_idx = 0, other_module_name_len = 0;
    wchar_t *other_module_name = 0, module_name[260] = {0};

    if (!Py_IsInitialized())
    {
        DEBUG_ERROR(L"python not initialized\n");
        return 0;
    }

    pModuleName = PyUnicode_FromString(module); 
    if (!pModuleName)
    {
        /* TODO get exception */
        DEBUG_ERROR(L"failed to decode module name: %s\n", module);
        return 0;
    }

    /* Determine if the module needs to be added to the list or reloaded */
    if (!g_module_list)
    {
        Py_DECREF(pModuleName);
        DEBUG_ERROR(L"failed to add module to list: module list is NULL\n");
        return 0;
    }

    /* Search the module list for the module */
    pAttrName = PyUnicode_FromString("__name__");
    if (!pAttrName)
    {
        Py_DECREF(pModuleName);
        DEBUG_ERROR(L"failed to encode \"__name__\"\n");
        return 0;
    }

    module_list_len = PyList_Size(g_module_list);
    for (module_idx = 0; module_idx < module_list_len; ++module_idx)
    {
        pOtherModule = PyList_GetItem(g_module_list, module_idx);
        if (!pOtherModule)
        {
            DEBUG_ERROR(L"failed to get module from module list\n");
        }
        else
        {
            pOtherModuleName = PyObject_GetAttr(pOtherModule, pAttrName);
            if (!pOtherModuleName)
            {
                DEBUG_ERROR(L"failed to get module's name from module list\n");
            }
            else
            {
                other_module_name = PyUnicode_AsWideCharString(pOtherModuleName, &other_module_name_len);
                if (!other_module_name)
                {
                    DEBUG_ERROR(L"failed to decode module's name from module list\n");
                }
                else
                {
                    DEBUG_DEBUG(L"module[\033[1m%d\033[0m] = \033[33m%ls\033[0m\n", module_idx, other_module_name);

                    char_to_wchar(module, module_name);
                    if (wcsncmp(module_name, other_module_name, fmin(wcslen(module_name),
                                    wcslen(other_module_name))) == 0)
                    {
                        pModule = pOtherModule;
                    }

                    PyMem_Free(other_module_name);
                }

                Py_DECREF(pOtherModuleName);
            }
        }
    }

    if (!pModule)
    {
        pModule = PyImport_Import(pModuleName);
        Py_DECREF(pModuleName);
    
        if (!pModule)
        {
            /* TODO get exception */
            DEBUG_ERROR(L"failed to load module: \033[31m%s\033[0m\n", module);
            return 0;
        }
        
        if (PyList_Append(g_module_list, pModule) < 0)
        {
            DEBUG_ERROR(L"failed to add module to list: \033[31m%s\033[0m\n", module);
            Py_DECREF(pModule);
            return 0;
        }
    }
    else
    {
        Py_DECREF(pModuleName);
        pModule = PyImport_ReloadModule(pModule);
        
        if (!pModule)
        {
            /* TODO get exception */
            DEBUG_ERROR(L"failed to reload module: \033[31m%s\033[0m\n", module);
            return 0;
        }
    }

    Py_DECREF(pModule);

    DEBUG_INFO(L"loaded module: \033[33m%s\033[0m\n", module);

    return 1;
}

static void debug_python_info(void)
{
#ifdef DEBUG
    wchar_t *tmp = NULL, *path = NULL, temp[30] = {0};
    int idx = 0;
    size_t len = 0;
    char version[1000] = {0}, *compiler = NULL;
    PyObject *pSysPath = NULL, *pPath = NULL;
    Py_ssize_t sys_path_len = 0, path_idx = 0, path_len = 0;

    if (!Py_IsInitialized())
    {
        DEBUG_ERROR(L"python not initialized\n");    
        return;
    }

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
   
    pSysPath = PySys_GetObject("path");
    if (!pSysPath)
    {
        /* TODO get exception */
        DEBUG_ERROR(L"failed to get Python sys.path\n");
        return;
    }

    sys_path_len = PyList_Size(pSysPath);
    for (path_idx = 0; path_idx < sys_path_len; ++path_idx)
    {
        pPath = PyList_GetItem(pSysPath, path_idx);
        if (!pPath)
        {
            DEBUG_ERROR(L"failed to get Python sys.path[%d]\n", path_idx);
        }
        else
        {
            path = PyUnicode_AsWideCharString(pPath, &path_len);
            if (!path)
            {
                DEBUG_ERROR(L"failed to decode Python sys.path[%d]\n", path_idx);
            }
            else
            {
                swprintf(temp, sizeof(temp) / sizeof(wchar_t) - 1, L"sys.path[%d]", path_idx);
                DEBUG_DEBUG(L"\033[33m%-20ls\033[0m %ls\n", temp, path); 
                PyMem_Free(path);
            }
        }
    }
#endif
}

