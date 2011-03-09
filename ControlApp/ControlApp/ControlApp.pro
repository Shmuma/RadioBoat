#-------------------------------------------------
#
# Project created by QtCreator 2011-03-07T10:52:10
#
#-------------------------------------------------

QT       += core gui

TARGET = ControlApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ControllerConnector.cpp \
    BoatController.cpp \
    optionsdialog.cpp

HEADERS  += mainwindow.h \
    BoatController.hpp \
    ControllerConnector.hpp \
    BoatController.hpp \
    libusb/libusb.h \
    optionsdialog.hpp

FORMS   += mainwindow.ui \
    optionsdialog.ui
LIBS    += -L../ControlApp/libusb -lusb-1.0
