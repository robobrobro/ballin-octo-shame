#ifndef __OCTO_SCRIPTING_H__
#define __OCTO_SCRIPTING_H__

#include <wchar.h>
#include <vector>

#include "engine.h"
#include "scripting/plugin.h"

typedef struct : engine_ctx_t
{
    wchar_t * program_name;
} scripting_engine_ctx_t;

class ScriptingEngine : public Engine
{
    public:
        ScriptingEngine(scripting_engine_ctx_t * ctx);
        ~ScriptingEngine();

        bool load(const wchar_t * module);
        bool load_dir(const wchar_t * path);

    protected:
        std::vector<plugin_t *> _modules;
};

#endif

