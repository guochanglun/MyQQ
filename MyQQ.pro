#-------------------------------------------------
#
# Project created by QtCreator 2017-06-24T14:03:26
#
#-------------------------------------------------

QT       += core gui network script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyQQ
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        logindialog.cpp \
    qqdialog.cpp \
    qpeopleitem.cpp \
    constval.cpp \
    clickablelabel.cpp \
    searchitem.cpp \
    applyitem.cpp \
    qregister.cpp

HEADERS  += logindialog.h \
    qqdialog.h \
    qpeopleitem.h \
    constval.h \
    clickablelabel.h \
    searchitem.h \
    applyitem.h \
    qregister.h

FORMS    += logindialog.ui \
    qqdialog.ui \
    qpeopleitem.ui \
    chatitem.ui \
    searchitem.ui \
    applyitem.ui \
    qregister.ui

RESOURCES += \
    src.qrc
