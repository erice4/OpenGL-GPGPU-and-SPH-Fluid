# -------------------------------------------------
# Project created by QtCreator 2009-11-05T12:31:57
# -------------------------------------------------
# qt 5 wants this may cause errors with 4
isEqual(QT_MAJOR_VERSION, 5) {cache() }

# We will use the built in Qt lib template
TEMPLATE = lib
CONFIG += staticlib
CONFIG+=c++11


SOURCES +=  $$PWD/src/ShaderManager.cpp \
    src/SSBO.cpp \
    src/BaseGPGPU.cpp \
    src/ComputeShader.cpp \
    src/TransformFeedback.cpp

HEADERS +=  $$PWD/include/ShaderManager.h \
    include/SSBO.h \
    include/BaseGPGPU.h \
    include/ComputeShader.h \
    include/TransformFeedback.h

isEqual(QT_MAJOR_VERSION, 5) {DEFINES +=QT5BUILD }

# This is the output target we want to create
TARGET = lib/DoGPU
# this is where we want to put the intermediate build files ( ../obj)
OBJECTS_DIR = obj
QMAKE_LFLAGS-= -headerpad_max_install_names
QMAKE_LFLAGS_SHLIB -= -single_module
QMAKE_LFLAGS_VERSION=
QMAKE_LFLAGS_COMPAT_VERSION=
QMAKE_LFLAGS_SONAME=
# use this to suppress some warning from boost
unix:QMAKE_CXXFLAGS_WARN_ON += "-Wno-unused-parameter"
macx:QMAKE_MAC_SDK = macosx10.11

#set some flags for sse etc
unix:QMAKE_CXXFLAGS+=  -msse -msse2 -msse3 -fms-extensions -O3
QMAKE_CFLAGS+= -DGLEW_NO_GLU -DGLEW_STATIC
unix:QMAKE_CXXFLAGS_WARN_ON += -Wno-builtin-macro-redefined -isystem
macx:DEFINES +=GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

INCLUDEPATH += ./include
unix:INCLUDEPATH += /usr/include/GL

unix:LIBS += -L/usr/local/lib


# in this case unix is also mac so we need to exclude mac from the unix build
win32|unix:!macx{
        # now define some linux specific flags
        unix:QMAKE_CXXFLAGS+=  -march=native
        #unix:DEFINES += LINUX
        unix:LIBS+= -L/usr/lib64 -lGL -lGLU -lX11

}





