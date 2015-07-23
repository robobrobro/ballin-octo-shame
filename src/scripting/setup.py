from distutils.core import setup, Extension

setup(name='octo',
      version='1.0',
      ext_modules=[Extension('octo.graphics', ['../graphics/module.cpp'])],
      include_dirs=['../../include'],
     )

