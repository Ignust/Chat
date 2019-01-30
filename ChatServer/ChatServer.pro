TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    Server.cpp \
    Manager.cpp \
    EventHandler.cpp \
    DataBase.cpp

HEADERS += \
    Server.hpp \
    Manager.hpp \
    MailTypes.hpp \
    EventHandler.hpp \
    ClientTypes.hpp \
    DataBase.hpp \
    Tools.hpp
