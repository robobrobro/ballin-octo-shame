#ifndef __OCTO_GRAPHICS_MODULE_H__
#define __OCTO_GRAPHICS_MODULE_H__

#include <memory>
#include "graphics/engine.h"
#include "module/module.h"

namespace graphics
{

const std::string NAME = "graphics";

class Module : public module::Module
{
    public:
        Module();

        bool load(void);
        void set_graphics_engine(std::shared_ptr<graphics::Engine> graphics_engine);

        PyObject * set_window_size(PyObject * self, PyObject * args, PyObject * kwargs);

    protected:
        std::shared_ptr<graphics::Engine> _graphics_engine;
};

}   // namespace graphics

#endif

