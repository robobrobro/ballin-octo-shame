#ifndef __OCTO_SCRIPTING_ENGINE_H__
#define __OCTO_SCRIPTING_ENGINE_H__

#include <string>
#include <vector>

#include "engine/engine.h"
#include "scripting/plugin.h"

namespace scripting
{

struct ctx_t : public engine::ctx_t
{
    std::wstring program_name;
};

class Engine : public engine::Engine
{
    public:
        Engine(scripting::ctx_t * ctx);
        ~Engine();

        bool load(const std::wstring & module);
        bool load_dir(const std::wstring & path);

    protected:
        std::vector<scripting::plugin::Plugin *> _modules;
};

}   // namespace scripting

#endif

