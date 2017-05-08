TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lboost_system -lpthread

SOURCES += main.cpp \
    localserver.cpp

HEADERS += \
    localserver.h

