#include "debug.h"
#include "graphics/module.h"
#include "graphics/engine.h"
#include "utils/color.h"

static graphics::Module * _module = NULL;

static PyObject * _set_window_size(PyObject * self, PyObject * args, PyObject * kwargs)
{
    return _module->set_window_size(self, args, kwargs);
}
    
static PyMethodDef _method_defs[] = {
    {"set_window_size", (PyCFunction) _set_window_size, METH_VARARGS | METH_KEYWORDS,
     "Set the window size"},
    {NULL, NULL, 0, NULL},
};

static PyModuleDef _module_def = {
    PyModuleDef_HEAD_INIT, graphics::NAME.c_str(), NULL, -1, _method_defs, NULL, NULL, NULL, NULL
};

static PyObject * _init(void)
{
    return PyModule_Create(&_module_def);
}

graphics::Module::Module()
    : module::Module::Module(graphics::NAME)
{
    _module = this;
}

bool graphics::Module::load(void)
{
    return PyImport_AppendInittab(this->_name.c_str(), _init) != -1;
}
        
void graphics::Module::set_graphics_engine(std::shared_ptr<graphics::Engine> graphics_engine)
{
    this->_graphics_engine = graphics_engine;
}

PyObject * graphics::Module::set_window_size(PyObject * self, PyObject * args, PyObject * kwargs)
{
    unsigned int width = 0, height = 0;
    static char * keywords[] = {"width", "height", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "II", keywords, &width, &height))
        return NULL;

    DEBUG_DEBUG(L"setting window size to %ls%d%lsx%ls%d%ls\n",
            COLOR_WHITE, width, COLOR_END, COLOR_WHITE, height, COLOR_END);

    if (!this->_graphics_engine)
    {
        PyErr_SetString(PyExc_RuntimeError, "graphics engine is NULL");
        return NULL;
    }

    if (!this->_graphics_engine->set_window_size(width, height))
    {
        PyErr_SetString(PyExc_RuntimeError, "failed to set window size");
        return NULL;
    }

    Py_RETURN_NONE;
}

