QT += core gui printsupport \
    widgets

TARGET = fft3daverage
TEMPLATE = app

INCLUDEPATH += $$PWD/../fft3d/include
LIBS += -L$$PWD/../fft3d/build -ldata3d

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

