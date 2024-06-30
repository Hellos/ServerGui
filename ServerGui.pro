QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h

FORMS += \
    MainWindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../ResourcesServer/build/unknown-Debug/release/ -lResourcesServer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../ResourcesServer/build/unknown-Debug/debug/ -lResourcesServer
else:unix: LIBS += -L$$PWD/../ResourcesServer/build/unknown-Debug/ -lResourcesServer

INCLUDEPATH += $$PWD/../ResourcesServer
DEPENDPATH += $$PWD/../ResourcesServer
