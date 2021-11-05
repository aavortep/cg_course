QT       += core gui
QT += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += "/mingw64/include/c++/11.2.0/"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    canvas.cpp \
    edge.cpp \
    light.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    point.cpp

HEADERS += \
    camera.h \
    canvas.h \
    edge.h \
    light.h \
    mainwindow.h \
    model.h \
    point.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    metronome_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
