#ifndef __OCTO_GAME_MODULE_H__
#define __OCTO_GAME_MODULE_H__

#include "module/module.h"

namespace game
{

class Module : public module::Module
{
    public:
        Module();

        PyObject * version(PyObject * self, PyObject * args);
};

}   // namespace game

#endif

