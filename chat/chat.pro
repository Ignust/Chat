TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    server.cpp \
    manager.cpp \
    eventhandler.cpp

HEADERS += \
    server.hpp \
    manager.hpp \
    mail.hpp \
    eventhandler.hpp \
    client.h
