TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += include

HEADERS += \
    include/Flecha.h \
    include/Jugador.h

SOURCES += \
    main.cpp \
    src/Flecha.cpp \
    src/Jugador.cpp
