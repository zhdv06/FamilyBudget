QT       += core gui widgets sql

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    catalogwidget.cpp \
    creditwidget.cpp \
    database.cpp \
    debitwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    registrywidget.cpp \
    reportwidget.cpp

HEADERS += \
    catalogwidget.h \
    creditwidget.h \
    database.h \
    debitwidget.h \
    defines.h \
    iwidget.h \
    mainwindow.h \
    registrywidget.h \
    reportwidget.h

FORMS += \
    catalogwidget.ui \
    creditwidget.ui \
    debitwidget.ui \
    mainwindow.ui \
    registrywidget.ui \
    reportwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
