QT += core gui printsupport \
    widgets

TARGET = fft3daverage
TEMPLATE = app

INCLUDEPATH += $$PWD/../fft3d
LIBS += -L$$PWD/../lib -ldata3d

SOURCES += main.cpp \
           mainwindow.cpp \
           processing.cpp \
           qcustomplot.cpp \
           iqcustomplot.cpp

HEADERS += \
           mainwindow.h \
           processing.h \
           qcustomplot.h \
           iqcustomplot.h

DESTDIR = $$PWD/../bin
