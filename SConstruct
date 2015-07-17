import os

PYTHON_PATH = os.path.join(Dir('#').abspath, 'lib', 'Python-3.4.3')
PYTHON_LIB = os.path.join(PYTHON_PATH, 'libpython3.4m.a')

BUILD_DIR = './build'
SRC_DIR = './src'
LIB_DIR = './lib'

PROGRAM = 'octo'

env = Environment(
    CPPPATH=[PYTHON_PATH, os.path.join(PYTHON_PATH, 'Include')],
    CCFLAGS='-fno-strict-aliasing -fno-common -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes',
    LIBS=[File(PYTHON_LIB), 'pthread', 'dl', 'util', 'm'],
    PYTHON_PATH=PYTHON_PATH,
    PYTHON_LIB=PYTHON_LIB,
    PROGRAM=PROGRAM,
)

env.SConscript(os.path.join(SRC_DIR, 'SConscript'), variant_dir=BUILD_DIR, duplicate=False, exports='env')
env.SConscript(os.path.join(LIB_DIR, 'SConscript'), variant_dir=os.path.join(BUILD_DIR, 'lib'), duplicate=False, exports='env')

env.Clean(os.curdir, BUILD_DIR)
