#ifndef __OCTO_ENGINE_ENGINE_H__
#define __OCTO_ENGINE_ENGINE_H__

#include <memory>

namespace engine
{

struct ctx_t
{
};

class Engine
{
    public:
        Engine(std::shared_ptr<engine::ctx_t> ctx) : _initialized(false) {}
        virtual ~Engine() {}

        virtual bool run() { return true; }

        virtual bool is_initialized(void) const { return _initialized; }

    protected:
        bool _initialized;
};

}   // namespace engine

#endif

