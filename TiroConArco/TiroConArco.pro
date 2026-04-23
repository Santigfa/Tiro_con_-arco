TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += include

HEADERS += \
    include/Flecha.h \
    include/Jugador.h \
    include/Objetivo.h \
    include/ObjetivoFijo.h \
    include/Portal.h

SOURCES += \
    main.cpp \
    src/Flecha.cpp \
    src/Jugador.cpp \
    src/Objetivo.cpp \
    src/ObjetivoFijo.cpp \
    src/Portal.cpp
