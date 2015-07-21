#ifndef __OCTO_SCRIPTING_PLUGIN_H__
#define __OCTO_SCRIPTING_PLUGIN_H__

#include <Python.h>
#include <string>

#include "utils/path.h"

namespace scripting
{

namespace plugin
{

namespace functions
{

/* Plugin API functions */

/********************************************************************
 *  def load(*args, **kwargs)
 * 
 *  DESCRIPTION
 *      called once when the module is first loaded
 *  RETURN
 *      Boolean: True if successful. If this function returns False,
 *      the module is flagged as not ready and will not be called
 *      again until it is reloaded
 *******************************************************************/
const std::string LOAD = "load";

}   // namespace functions

class Plugin
{
    public:
        Plugin();
        Plugin(PyObject * module, const std::wstring & name);
        ~Plugin();
    
        PyObject * call(const std::string & function, PyObject * pArgs, PyObject * pKwargs) const;

        PyObject * module(void) { return _module; }
        std::wstring name(void) const;
        std::wstring name(const std::string & function) const;
        std::wstring module_name(void) const { return _module_name; }
        bool loaded(void) const { return _loaded; }
        void loaded(bool l) { _loaded = l; }
   
    protected: 
        PyObject *_module;
        std::wstring _module_name;
        bool _loaded;
};

}   // namespace plugin

}   // namespace scripting

#endif

