#-------------------------------------------------
#
# Ported to Qt 5.3, mingw 64 bit
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Astar
TEMPLATE = app

PRECOMPILED_HEADER = stable.h
win32-msvc* {
PRECOMPILED_SOURCE = stable.cpp
}

SOURCES += \
    main.cpp \
    qmain.cpp \
    qcellwidget.cpp \
    stable.cpp \
    qpathviewer.cpp \
    pathfinder/cell.cpp \
    pathfinder/map.cpp \
    pathfinder/pathfinder.cpp

HEADERS += \
    qmain.h \
    qcellwidget.h \
    qdlg_about.h \
    qdlg_howto.h \
    qdlg_reposition.h \
    qdlg_resize.h \
    qpathviewer.h \
    stable.h \
    pathfinder/arr2d.h \
    pathfinder/arr2d_impl.h \
    pathfinder/cell.h \
    pathfinder/map.h \
    pathfinder/pathfinder.h




FORMS    += qdlg_about.ui\
            qdlg_howto.ui\
            qdlg_reposition.ui\
            qdlg_resize.ui\
            qmain.ui

RESOURCES += resources/qmain.qrc


