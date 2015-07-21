#include <Python.h>
#include <dirent.h>
#include <vector>

#include "debug.h"
#include "scripting/plugin.h"
#include "scripting/engine.h"
#include "utils/color.h"
#include "utils/path.h"
#include "utils/string.h"

static void debug_python_info(void);

scripting::Engine::Engine(scripting::ctx_t * ctx)
    : engine::Engine::Engine((engine::ctx_t *)ctx)
{
    wchar_t *program_dir = NULL, *builtins_dir = NULL, *plugins_dir = NULL;
    PyObject *pSysPath = NULL, *pPluginsDir = NULL;

    if (!ctx)
    {
        DEBUG_ERROR(L"ctx is NULL\n");
        return;
    }

    if (this->_initialized)
    {
        DEBUG_ERROR(L"Python scripting engine already initialized\n");
        return;
    }

    if (Py_IsInitialized())
    {
        DEBUG_ERROR(L"Python already initialized\n");    
        return;
    }

    /* Set Python interpreter's program name */
    Py_SetProgramName(ctx->program_name);

    program_dir = path_get_dir(ctx->program_name);
    if (!program_dir)
    {
        DEBUG_ERROR(L"failed to get directory path to the program\n");
        return;
    }

    /* Set Python interpreter's module search path */
    builtins_dir = path_join(program_dir, L".builtins", NULL); 
    
    if (!builtins_dir)
    {
        free(program_dir);
        DEBUG_ERROR(L"failed to get builtins path\n");
        return;
    }

    Py_SetPath(builtins_dir);
    
    /* Initialize Python interpreter */
    Py_Initialize();
    //Py_InitializeEx(0);
    this->_initialized = true;
    DEBUG_INFO(L"Python scripting engine initialized\n");
    
    /* Append plugins directory to Python's sys.path */
    pSysPath = PySys_GetObject("path");
    if (!pSysPath)
    {
        free(program_dir);
        this->_initialized = false;
        debug_python_info();
        DEBUG_ERROR(L"failed to get Python sys.path\n");
        return;
    }

    plugins_dir = path_join(program_dir, L"plugins", NULL); 
    free(program_dir);
    
    if (!plugins_dir)
    {
        this->_initialized = false;
        debug_python_info();
        DEBUG_ERROR(L"failed to get plugins path\n");
        return;
    }

    pPluginsDir = PyUnicode_FromWideChar(plugins_dir, wcslen(plugins_dir));

    if (!pPluginsDir)
    {
        free(plugins_dir);
        this->_initialized = false;
        debug_python_info();
        DEBUG_ERROR(L"failed to decode plugins path\n");
        return;
    }

    if (PyList_Append(pSysPath, pPluginsDir) < 0)
    {
        free(plugins_dir);
        Py_DECREF(pPluginsDir);
        this->_initialized = false;
        debug_python_info();
        DEBUG_ERROR(L"failed to append plugins path to Python sys.path\n");
        return;
    }
        
    Py_DECREF(pPluginsDir);
    
    debug_python_info();

    /* Load all modules in the plugins directory */
    if (!this->load_dir(plugins_dir))
    {
        free(plugins_dir);
        this->_initialized = false;
        DEBUG_ERROR(L"failed to load plugins\n");
        return;
    }
    
    free(plugins_dir);

    this->_initialized = true;
}

scripting::Engine::~Engine()
{
    /* Destroy module list */
    for (std::vector<scripting::plugin::Plugin *>::iterator iter = this->_modules.begin(); iter != this->_modules.end(); ++iter)
    {
        delete *iter;
    }
    this->_modules.clear();

    /* Finalize Python interpreter */
    if (Py_IsInitialized())
    {
        Py_Finalize();
        DEBUG_INFO(L"Python scripting engine shut down successfully\n");
    }

    this->_initialized = false;
}

bool scripting::Engine::load(const std::wstring & module)
{
    PyObject *pModuleName = NULL, *pModule = NULL, *pResult = NULL;
    scripting::plugin::Plugin *plugin = NULL, *other_plugin = NULL;
        
    DEBUG_DEBUG(L"attempting to load module %ls%ls%ls\n", COLOR_CYAN, module.c_str(), COLOR_END);

    if (!this->_initialized)
    {
        DEBUG_ERROR(L"scripting engine not initialized\n");
        return false;
    }

    /* Determine if the module needs to be added to the list or reloaded */
    /* Search for the module in the module list. If it exists, reload it */
    for (std::vector<scripting::plugin::Plugin *>::size_type idx = 0; idx < this->_modules.size(); ++idx)
    {
        other_plugin = this->_modules[idx];
        if (!other_plugin)
        {
            DEBUG_ERROR(L"failed to get other_plugin from module list\n");
            continue;
        }

        if (!other_plugin->module())
        {
            DEBUG_ERROR(L"failed to get module from module list\n");
            continue;
        }

        DEBUG_DEBUG(L"module[%ls%d%ls] = %ls (%ls%lsloaded%ls)\n",
                COLOR_BOLD, idx, COLOR_END,
                other_plugin->name().c_str(),
                other_plugin->loaded() ? COLOR_GREEN : COLOR_RED,
                other_plugin->loaded() ? L"" : L"not ", COLOR_END);

        if (module == other_plugin->module_name())
        {
            plugin = other_plugin;
            break;
        }
    }

    if (!plugin)
    {
        DEBUG_DEBUG(L"module not found: importing...\n");

        /* Convert name of the module to a Python unicode string */
        pModuleName = PyUnicode_FromWideChar(module.c_str(), module.size()); 
        if (!pModuleName)
        {
            DEBUG_ERROR(L"failed to encode module name\n");
            return false;
        }

        /* Import the module */
        pModule = PyImport_Import(pModuleName);
        Py_DECREF(pModuleName);
    
        if (!pModule)
        {
            DEBUG_ERROR(L"failed to load module\n");
            return false;
        }
       
        /* Append the module to the plugin list */
        plugin = new scripting::plugin::Plugin(pModule, module);
        if (!plugin)
        {
            DEBUG_ERROR(L"failed to allocate memory for module\n");
            return false;
        }

        this->_modules.push_back(plugin);
    }
    else
    {
        DEBUG_DEBUG(L"module found: reloading module %ls\n", plugin->name().c_str());

        /* The module has already been imported, so reload it */
        pModule = PyImport_ReloadModule(pModule);
       
        if (!pModule)
        {
            DEBUG_ERROR(L"failed to reload module: %ls\n", plugin->name().c_str());
            return false;
        }
    }

    /* Call the module's load function, as specified by the plugin API */
    plugin->loaded(false);
    pResult = plugin->call(scripting::plugin::functions::LOAD, NULL, NULL);
    
    if (!pResult)
    {
        return false;
    }

    if (!PyBool_Check(pResult))
    {
        Py_DECREF(pResult);
        DEBUG_ERROR(L"module function %ls failed to return a Boolean\n",
                plugin->name(scripting::plugin::functions::LOAD.c_str()).c_str());
        return false;
    }

    plugin->loaded(pResult == Py_True);
    Py_DECREF(pResult);

    if (!plugin->loaded())
    {
        DEBUG_ERROR(L"module %ls failed to load\n", plugin->name().c_str());
    }
    else
    {
        DEBUG_INFO(L"successfully loaded module: %ls\n", plugin->name().c_str());
    }

    return plugin->loaded();
}
   
bool scripting::Engine::load_dir(const wchar_t * path)
{
    DIR *dir = NULL;
    char mb_path[PATH_MAX_LEN + 1] = {0};
    struct dirent *ent = NULL;
    wchar_t ent_name[PATH_MAX_LEN + 1] = {0}, *ent_name_no_ext = NULL;
    bool retval = true;

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
            DEBUG_DEBUG(L"found plugin: %ls%s%ls\n", COLOR_WHITE, ent->d_name, COLOR_END);

            ent_name_no_ext = path_trim_ext(ent_name);
            if (ent_name_no_ext)
            {
                retval = this->load(std::wstring(ent_name_no_ext)) && retval;
                free(ent_name_no_ext);
            }
        }
    }

    closedir(dir);

    return retval;
}

static void debug_python_info(void)
{
#ifdef DEBUG
    wchar_t *tmp = NULL, *path = NULL, temp[30] = {0};
    size_t idx = 0;
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
        DEBUG_DEBUG(L"%ls%-20ls%ls %ls\n", COLOR_WHITE, L"Program Name", COLOR_END, tmp);
    tmp = Py_GetProgramFullPath();
    if (tmp)
        DEBUG_DEBUG(L"%ls%-20ls%ls %ls\n", COLOR_WHITE, L"Program Full Path", COLOR_END, tmp);
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
    DEBUG_DEBUG(L"%ls%-20ls%ls %s\n", COLOR_WHITE, L"Version", COLOR_END, version);
    if (compiler)
        DEBUG_DEBUG(L"%ls%-20ls%ls %s\n", COLOR_WHITE, L"Compiler", COLOR_END, compiler);
    tmp = Py_GetPath();
    if (tmp)
        DEBUG_DEBUG(L"%ls%-20ls%ls %ls\n", COLOR_WHITE, L"Module Path", COLOR_END, tmp);
    DEBUG_DEBUG(L"%ls%-20ls%ls %s\n", COLOR_WHITE, L"Platform", COLOR_END, Py_GetPlatform());
   
    pSysPath = PySys_GetObject("path");
    if (!pSysPath)
    {
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
                DEBUG_DEBUG(L"%ls%-20ls%ls %ls\n", COLOR_WHITE, temp, COLOR_END, path); 
                PyMem_Free(path);
            }
        }
    }
#endif
}

