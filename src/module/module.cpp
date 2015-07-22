#include "debug.h"
#include "module/module.h"

module::Module::Module(const std::string & name)
   : _name(name),
   , _def(std::make_shared<PyModuleDef>())
{
    this->_def->m_base = PyModuleDef_HEAD_INIT;
    this->_def->m_name = this->_name.c_str();
    this->_def->m_doc = NULL;
    this->_def->m_size = -1;
    this->_def->m_methods = this->_methods;
    this->_def->m_reload = NULL;
    this->_def->m_traverse = NULL;
    this->_def->m_clear = NULL;
    this->_def->m_free = NULL;
}

PyObject * module::Module::load(void)
{
    DEBUG_DEBUG(L"loading module %ls%ls%ls\n", COLOR_WHITE, this->_name.c_str(), COLOR_END);
    PyObject * pModule = PyModule_Create(this->_def);
    if (!pModule)
        DEBUG_ERROR(L"failed to load module\n");
    return pModule;
}

