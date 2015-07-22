#ifndef __OCTO_SCRIPTING_ENGINE_H__
#define __OCTO_SCRIPTING_ENGINE_H__

#include <memory>
#include <string>
#include <vector>

#include "engine/engine.h"
#include "scripting/plugin.h"

namespace scripting
{

const std::wstring BUILTINS_DIR = L".builtins";
const std::wstring PLUGINS_DIR = L"plugins";

struct ctx_t : public engine::ctx_t
{
    std::wstring program_name;
};

class Engine : public engine::Engine
{
    public:
        Engine(std::shared_ptr<scripting::ctx_t> ctx);
        ~Engine();

        bool load(const std::wstring & module);
        bool load_dir(const std::wstring & path);
        bool load_plugins(void);

    protected:
        std::vector<std::shared_ptr<scripting::plugin::Plugin>> _modules;
        std::wstring _plugins_path;
};

}   // namespace scripting

#endif

