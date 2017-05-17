TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lboost_system
LIBS += -lpthread

SOURCES += main.cpp \
    localserver.cpp \
    databasemanager.cpp \
    mongomanager.cpp

HEADERS += \
    localserver.h \
    databasemanager.h \
    mongomanager.h

