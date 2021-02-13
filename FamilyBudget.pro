QT       += core gui widgets sql

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    catalogwidget.cpp \
    database.cpp \
    main.cpp \
    mainwindow.cpp \
    registrywidget.cpp

HEADERS += \
    catalogwidget.h \
    database.h \
    defines.h \
    mainwindow.h \
    registrywidget.h

FORMS += \
    catalogwidget.ui \
    mainwindow.ui \
    registrywidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
