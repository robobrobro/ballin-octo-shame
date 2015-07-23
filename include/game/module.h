#ifndef __OCTO_GAME_MODULE_H__
#define __OCTO_GAME_MODULE_H__

#include "module/module.h"

namespace game
{

const std::string NAME = "octo";

class Module : public module::Module
{
    public:
        Module();

        bool load(void);

        PyObject * version(PyObject * self, PyObject * args);
};

}   // namespace game

#endif

