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

SOURCES +=  main.cpp\
            qmain.cpp\
            qcellwidget.cpp\
            map.cpp\
            pathfinder.cpp \
            stable.cpp \
    cell.cpp \
    qpathviewer.cpp

HEADERS += \
            qmain.h\
            qcellwidget.h\
            qdlg_about.h\
            qdlg_howto.h\
            qdlg_reposition.h\
            qdlg_resize.h\
            arr2d.h\
            map.h\
            pathfinder.h\
            qpathviewer.h\
            stable.h \
    cell.h \
    arr2d_impl.h

FORMS    += qdlg_about.ui\
            qdlg_howto.ui\
            qdlg_reposition.ui\
            qdlg_resize.ui\
            qmain.ui

RESOURCES += \
            resrouces/qmain.qrc
