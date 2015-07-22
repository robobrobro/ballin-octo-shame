#include <functional>
#include "game/module.h"

/* TODO
 * static std::shared_ptr<game::Module::Module> g_module;
 * extern "C" PyObject * version(PyObject * self, PyObject * args)
 * {
 *      return g_module->version(self, args);
 * }
 */

game::Module::Module() : module::Module::Module("octo")
{
    using namespace std::placeholders;

    static PyMethodDef methods[] = {
        {"version", std::bind(&game::Module::version, this, _1, _2), METH_VARARGS,
            "Get the version of the octo engine"},
        {NULL, NULL, 0, NULL},
    };

    this->_methods = std::make_shared<PyMethodDef>(&methods);
}

PyObject * game::Module::version(PyObject * self, PyObject * args)
{
    return PyLong_FromLong(1);
}

