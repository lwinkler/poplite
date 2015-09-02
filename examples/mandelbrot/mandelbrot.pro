QT += widgets

HEADERS       = mandelbrotwidget.h \
                gen/renderthread.iface.hpp
SOURCES       = main.cpp \
                mandelbrotwidget.cpp \
                renderthread.cpp

INCLUDEPATH += gen \
               ../..

LIBS +=  -lboost_serialization -lboost_system -lboost_thread -lboost_serialization -lboost_context -lboost_log -lboost_program_options -lpthread -lboost_system -lboost_thread -lboost_context -lboost_log -lboost_program_options -lpthread

QMAKE_CXXFLAGS += -std=c++11

unix:!mac:!vxworks:!integrity:!haiku:LIBS += -lm

# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/threads/mandelbrot
INSTALLS += target
