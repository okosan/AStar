#-------------------------------------------------
#
# Ported to Qt 5.3, mingw 64 bit
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Astar
TEMPLATE = app

SOURCES +=  main.cpp\
            qmain.cpp\
            stdafx.cpp\
            qcellwidget.cpp\
            map.cpp\
            pathfinder.cpp

HEADERS += \
            qmain.h\
            stdafx.h\
            qcellwidget.h\
            qdlg_about.h\
            qdlg_howto.h\
            qdlg_reposition.h\
            qdlg_resize.h\
            arr2d.h\
            map.h\
            pathfinder.h\
            qpathviewer.h\

FORMS    += qdlg_about.ui\
            qdlg_howto.ui\
            qdlg_reposition.ui\
            qdlg_resize.ui\
            qmain.ui

#RESOURCES += ./resources/qmain.qrc

RESOURCES += \
            resrouces/qmain.qrc
