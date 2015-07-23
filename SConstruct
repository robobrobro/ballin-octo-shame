import os
import sys

BUILD_DIR = os.path.join(Dir('#').abspath, 'build')
INCLUDE_DIR = os.path.join(Dir('#').abspath, 'include')
SRC_DIR = os.path.join(Dir('#').abspath, 'src')
LIB_DIR = os.path.join(Dir('#').abspath, 'lib')

PROGRAM = 'octo'

PYTHON_DIR = 'Python-3.4.3'
PYTHON_VER = 'libpython3.4m'
PYTHON_PATH = os.path.join(LIB_DIR, PYTHON_DIR)

# Determine target operating system
OPERATING_SYSTEM_DICT = {
    'linux': {
        'names': ['linux', 'lin', 'lnx'],
        'python': '{ver}.a'.format(ver=PYTHON_VER),
        'CXX': 'g++',
        'CCFLAGS': '',
        'LINKFLAGS': '',
    },
    'mac': {
        'names': ['mac', 'osx'],
        'python': '{ver}.a'.format(ver=PYTHON_VER),
        'CXX': 'g++-4.9',
        'CCFLAGS': ' -D_GLIBCXX_FULLY_DYNAMIC_STRING=1', 
        'LINKFLAGS': '',
    },
    'windows': {
        'names': ['windows', 'win'],
        'python': '{ver}.dll'.format(ver=PYTHON_VER),
        'CXX': 'g++',
        'CCFLAGS': '',
        'LINKFLAGS': '',
    },
}

operating_system = ARGUMENTS.get('os', 'linux').lower()
for k, v in OPERATING_SYSTEM_DICT.iteritems():
    if operating_system in v['names']:
        operating_system = k
        break
if operating_system not in OPERATING_SYSTEM_DICT.keys():
    sys.stderr.write('error: invalid os: {os}\nvalid choices: {choices}\n'.format(os=operating_system,
        choices=', '.join(OPERATING_SYSTEM_DICT.keys())))
    sys.exit(2)

BUILD_DIR = os.path.join(BUILD_DIR, operating_system)

# Determine target configuration
TARGET_DICT = {
    'debug': ['debug', 'dbg'],
    'release': ['release', 'rel'],
}
TARGET_CHOICES = [t for _, v in TARGET_DICT.iteritems() for t in v]

target = ARGUMENTS.get('target', 'release').lower()
for k, v in TARGET_DICT.iteritems():
    if target in v:
        target = k
        break
if target not in TARGET_CHOICES:
    sys.stderr.write('error: invalid target: {target}\nvalid choices: {choices}\n'.format(target=target,
        choices=', '.join(TARGET_CHOICES)))
    sys.exit(2)

BUILD_DIR = os.path.join(BUILD_DIR, target)
PYTHON_LIB = os.path.join(BUILD_DIR, OPERATING_SYSTEM_DICT[operating_system]['python'])

CCFLAGS = '-fno-strict-aliasing -fno-common -fdiagnostics-color=auto -fwrapv -Wall -std=c++11'
if target == 'debug': CCFLAGS += ' -DDEBUG -g3 -ggdb3'
else: CCFLAGS += ' -O2 -O3 -DNDEBUG -s'
CCFLAGS += OPERATING_SYSTEM_DICT[operating_system]['CCFLAGS']

env = Environment(
    CXX=OPERATING_SYSTEM_DICT[operating_system]['CXX'],
    CPPPATH=[PYTHON_PATH, os.path.join(PYTHON_PATH, 'Include'), INCLUDE_DIR],
    CCFLAGS=CCFLAGS,
    LINKFLAGS=OPERATING_SYSTEM_DICT[operating_system]['LINKFLAGS'],
    LIBS=[
        File(PYTHON_LIB), 'pthread', 'dl', 'util', 'm',
        'sfml-graphics', 'sfml-window', 'sfml-system', 'sfml-audio',
        'stdc++',
    ],
    PYTHON_PATH=PYTHON_PATH,
    PYTHON_LIB=PYTHON_LIB,
    PYTHON_DIR=PYTHON_DIR,
    BUILD_DIR=BUILD_DIR,
    SRC_DIR=SRC_DIR,
    PROGRAM=PROGRAM,
    OS=operating_system,
)

env.SConscript(
    os.path.join(SRC_DIR, 'SConscript'),
    variant_dir=BUILD_DIR,
    duplicate=False,
    exports='env',
)
env.SConscript(
    os.path.join(LIB_DIR, 'SConscript'),
    variant_dir=os.path.join(BUILD_DIR, 'lib'),
    duplicate=False,
    exports='env',
)

env.Clean(os.curdir, BUILD_DIR)

