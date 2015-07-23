#include "game/module.h"
#include "game/version.h"

static game::Module * _module = NULL;

static PyObject * _version(PyObject * self, PyObject * args)
{
    return _module->version(self, args);
}
    
static PyMethodDef _method_defs[] = {
    {"version", _version, METH_NOARGS, "Get the version of the octo engine"},
    {NULL, NULL, 0, NULL},
};

static PyModuleDef _module_def = {
    PyModuleDef_HEAD_INIT, game::NAME.c_str(), NULL, -1, _method_defs, NULL, NULL, NULL, NULL
};

static PyObject * _init(void)
{
    return PyModule_Create(&_module_def);
}

game::Module::Module() : module::Module::Module(game::NAME)
{
    _module = this;
}

bool game::Module::load(void)
{
    return PyImport_AppendInittab(this->_name.c_str(), _init) != -1;
}

PyObject * game::Module::version(PyObject * self, PyObject * args)
{
    return Py_BuildValue("u", game::VERSION.c_str());
}

