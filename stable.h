#ifndef STABLE_PRECOMPILED
#define STABLE_PRECOMPILED


#ifdef __cplusplus
#include <cmath>
#include <ctime>
#include <cassert>
#include <limits>

#include <cstdlib>
#include <cstdio>

#include <vector>
#include <list>
#include <deque>

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QMainWindow>
#include <QInputEvent>
#include <QFileDialog>
#include <QApplication>

#include <QGraphicsView>
#include <QGraphicsOpacityEffect>
#include <QGraphicsRectItem>
#include <QGraphicsOpacityEffect>

#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>

#endif

struct RECT
{
    int left, right, top, bottom;
};

void xfBeep(int freq = 1000, int duration = 100);

bool xfBetween(double val, double val1, double val2);

bool xfBetweenExcl(double val, double val1, double val2);

#endif
