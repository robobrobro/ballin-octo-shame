#ifndef __OCTO_MODULE_H__
#define __OCTO_MODULE_H__

#include <Python.h>
#include <string>

namespace module
{

class Module
{
    public:
        Module(const std::string & name) : _name(name) {}
        std::string name(void) const { return _name; }
        virtual bool load(void) = 0;

    protected:
        std::string _name;
};

}   // namespace module

#endif

