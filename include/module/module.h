#ifndef __OCTO_MODULE_H__
#define __OCTO_MODULE_H__

#include <Python.h>
#include <memory>
#include <string>

namespace module
{

class Module
{
    public:
        Module(const std::string & name);
        PyObject * load(void);

    protected:
        std::string _name;
        std::shared_ptr<PyModuleDef> _def;
        std::shared_ptr<PyMethodDef> _methods;
};

}   // namespace module

#endif

