#include "scripting.h"
#include "debug.h"
#include "list.h"
#include "plugin.h"
#include "utils/color.h"
#include "utils/path.h"
#include "utils/string.h"

#include <Python.h>
#include <dirent.h>
#include <math.h>

static list_t *g_module_list = NULL; 
static void module_list_destroy_callback(void * data);
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
        debug_python_info();
        free(program_dir);
        /* TODO get exception */
        DEBUG_ERROR(L"failed to get Python sys.path\n");
        return 0;
    }

    plugins_dir = path_join(program_dir, L"plugins", NULL); 
    free(program_dir);
    
    if (!plugins_dir)
    {
        debug_python_info();
        DEBUG_ERROR(L"failed to get plugins path\n");
        return 0;
    }

    pPluginsDir = PyUnicode_FromWideChar(plugins_dir, wcslen(plugins_dir));

    if (!pPluginsDir)
    {
        free(plugins_dir);
        debug_python_info();
        /* TODO get exception */
        DEBUG_ERROR(L"failed to decode plugins path\n");
        return 0;
    }

    if (PyList_Append(pSysPath, pPluginsDir) < 0)
    {
        free(plugins_dir);
        debug_python_info();
        Py_DECREF(pPluginsDir);
        DEBUG_ERROR(L"failed to append plugins path to Python sys.path\n");
        return 0;
    }
        
    Py_DECREF(pPluginsDir);
    
    debug_python_info();

    /* Create modules list */
    g_module_list = list_create();
    if (!g_module_list)
    {
        free(plugins_dir);
        DEBUG_ERROR(L"failed to create plugins list\n");
        return 0;
    }

    /* Load all modules in the plugins directory */
    if (!scripting_load_dir(plugins_dir))
    {
        free(plugins_dir);
        DEBUG_ERROR(L"failed to load plugins\n");
        return 0;
    }
    
    free(plugins_dir);

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
        list_destroy(g_module_list, module_list_destroy_callback);
        g_module_list = NULL;
    }

    Py_Finalize();
}

int scripting_load(const wchar_t * module)
{
    PyObject *pModuleName = NULL, *pModule = NULL, *pResult = NULL;
    Py_ssize_t module_len = 0;
    list_t *list_curr = NULL;
    plugin_t *plugin = NULL;
    unsigned int module_idx = 0;
    int retval = 0;
        
    DEBUG_DEBUG(L"attempting to load module %ls%ls%ls\n", COLOR_YELLOW, module, COLOR_END);

    if (!Py_IsInitialized())
    {
        DEBUG_ERROR(L"python not initialized\n");
        return 0;
    }

    module_len = wcsnlen(module, PATH_MAX_LEN);

    /* Determine if the module needs to be added to the list or reloaded */
    if (!g_module_list)
    {
        DEBUG_ERROR(L"failed to add module to list: module list is NULL\n");
        return 0;
    }

    /* Search for the module in the module list. If it exists, reload it */
    for (list_curr = g_module_list->front, module_idx = 0; list_curr; list_curr = list_curr->next, ++module_idx)
    {
        plugin = (plugin_t *)list_curr->data;
        if (!plugin)
        {
            DEBUG_ERROR(L"failed to get plugin from module list\n");
            continue;
        }

        if (!plugin->pModule)
        {
            DEBUG_ERROR(L"failed to get module from module list\n");
            continue;
        }

        DEBUG_DEBUG(L"module[%ls%d%ls] = %ls%ls%ls (%ls%lsloaded%ls)\n", COLOR_BOLD, module_idx, COLOR_END,
                COLOR_YELLOW, plugin->module_name, COLOR_END,
                plugin->loaded ? COLOR_GREEN : COLOR_RED,
                plugin->loaded ? L"" : L"not ", COLOR_END);

        if (wcsncmp(module, plugin->module_name, fmin(module_len, wcsnlen(plugin->module_name, PATH_MAX_LEN))) == 0)
        {
            pModule = plugin->pModule;
            break;
        }
    }

    if (!pModule)
    {
        DEBUG_DEBUG(L"module not found: importing module %ls%ls%ls\n", COLOR_YELLOW, module, COLOR_END);

        /* Convert name of the module to a Python unicode string */
        pModuleName = PyUnicode_FromWideChar(module, module_len); 
        if (!pModuleName)
        {
            DEBUG_ERROR(L"failed to encode module name: %ls%ls%ls\n", COLOR_RED, module, COLOR_END);
            return 0;
        }

        /* Import the module */
        pModule = PyImport_Import(pModuleName);
        Py_DECREF(pModuleName);
    
        if (!pModule)
        {
            DEBUG_ERROR(L"failed to load module: %ls%ls%ls\n", COLOR_RED, module, COLOR_END);
            return 0;
        }
       
        /* Append the module to the plugin list */
        plugin = (plugin_t *)malloc(sizeof(plugin_t));
        if (!plugin)
        {
            DEBUG_ERROR(L"failed to allocate memory for plugin for module: %ls%ls%ls\n",
                    COLOR_RED, module, COLOR_END);
            return 0;
        }

        plugin->pModule = pModule;
        memset(plugin->module_name, 0, sizeof(plugin->module_name));
        wcsncpy(plugin->module_name, module, sizeof(plugin->module_name) / sizeof(wchar_t) - 1);
        plugin->loaded = 0;

        if (!list_append(g_module_list, plugin))
        {
            Py_DECREF(pModule);
            free(plugin);
            DEBUG_ERROR(L"failed to add module to list: %ls%ls%ls\n", COLOR_RED, module, COLOR_END);
            return 0;
        }
    }
    else
    {
        DEBUG_DEBUG(L"module found: reloading module %ls%ls%ls\n", COLOR_YELLOW, module, COLOR_END);

        /* The module has already been imported, so reload it */
        pModule = PyImport_ReloadModule(pModule);
        
        if (!pModule)
        {
            DEBUG_ERROR(L"failed to reload module: %ls%ls%ls\n", COLOR_RED, module, COLOR_END);
            return 0;
        }
    }

    /* Call the module's load function, as specified by the plugin API */
    pResult = plugin_call_function(pModule, PLUGIN_API_FUNC_LOAD, NULL, NULL);
    
    if (!pResult)
    {
        plugin->loaded = 0;
        return 0; 
    }

    if (!PyBool_Check(pResult))
    {
        Py_DECREF(pResult);
        plugin->loaded = 0;
        DEBUG_ERROR(L"module function %ls%ls.%s%ls failed to return a Boolean\n", COLOR_RED, module,
                PLUGIN_API_FUNC_LOAD, COLOR_END);
        return 0;
    }

    plugin->loaded = retval = pResult == Py_True;
    Py_DECREF(pResult);

    if (!retval)
    {
        DEBUG_ERROR(L"failed to load module: %ls%ls%ls\n", COLOR_RED, module, COLOR_END);
    }
    else
    {
        DEBUG_INFO(L"successfully loaded module: %ls%ls%ls\n", COLOR_YELLOW, module, COLOR_END);
    }

    return retval;
}
   
int scripting_load_dir(const wchar_t * path)
{
    int retval = 1;
    DIR *dir = NULL;
    char mb_path[PATH_MAX_LEN + 1] = {0};
    struct dirent *ent = NULL;
    wchar_t ent_name[PATH_MAX_LEN + 1] = {0}, *ent_name_no_ext = NULL;

    wcstombs(mb_path, path, PATH_MAX_LEN);

    dir = opendir(mb_path);
    if (!dir)
    {
        DEBUG_ERROR(L"failed to open plugins directory: %s\n", strerror(errno));
        return 0;
    }

    while ((ent = readdir(dir)))
    {
        char_to_wchar(ent->d_name, ent_name);
        if (path_has_ext(ent_name, L".py"))
        {
            DEBUG_DEBUG(L"found plugin: %ls%s%ls\n", COLOR_YELLOW, ent->d_name, COLOR_END);

            ent_name_no_ext = path_trim_ext(ent_name);
            if (ent_name_no_ext)
            {
                retval = scripting_load(ent_name_no_ext) && retval;
                free(ent_name_no_ext);
            }
        }
    }

    closedir(dir);

    return retval;
}

static void module_list_destroy_callback(void * data)
{
    plugin_t *plugin = NULL;
    
    if (!data) return;

    plugin = (plugin_t *)data;
    Py_XDECREF(plugin->pModule);
    plugin->pModule = NULL;
    memset(plugin->module_name, 0, sizeof(plugin->module_name));
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
        DEBUG_DEBUG(L"%ls%-20ls%ls %ls\n", COLOR_YELLOW, L"Program Name", COLOR_END, tmp);
    tmp = Py_GetProgramFullPath();
    if (tmp)
        DEBUG_DEBUG(L"%ls%-20ls%ls %ls\n", COLOR_YELLOW, L"Program Full Path", COLOR_END, tmp);
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
    DEBUG_DEBUG(L"%ls%-20ls%ls %s\n", COLOR_YELLOW, L"Version", COLOR_END, version);
    if (compiler)
        DEBUG_DEBUG(L"%ls%-20ls%ls %s\n", COLOR_YELLOW, L"Compiler", COLOR_END, compiler);
    tmp = Py_GetPath();
    if (tmp)
        DEBUG_DEBUG(L"%ls%-20ls%ls %ls\n", COLOR_YELLOW, L"Module Path", COLOR_END, tmp);
    tmp = Py_GetPrefix();
    if (tmp)
        DEBUG_DEBUG(L"%ls%-20ls%ls %ls\n", COLOR_YELLOW, L"Prefix", COLOR_END, tmp);
    tmp = Py_GetExecPrefix();
    if (tmp)
        DEBUG_DEBUG(L"%ls%-20ls%ls %ls\n", COLOR_YELLOW, L"Exec Prefix", COLOR_END, tmp);
    DEBUG_DEBUG(L"%ls%-20ls%ls %s\n", COLOR_YELLOW, L"Platform", COLOR_END, Py_GetPlatform());
   
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
                DEBUG_DEBUG(L"%ls%-20ls%ls %ls\n", COLOR_YELLOW, temp, COLOR_END, path); 
                PyMem_Free(path);
            }
        }
    }
#endif
}

