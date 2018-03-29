#pragma once
#include "stable.h"

#include "pathfinder.h"
#include "qcellwidget.h"

class PATHVIEWER
{
public:
    PATHVIEWER();

    int init(PATHFINDER *pathfinder_,
             QCellWidget *mapview_,
             QGraphicsScene *gs_);

    int clearPath();

    int addPoint(int ix, int iy);

    int update(int fullupdate = 0);

private:
    class PATHPOINT
    {
    public:
        PATHPOINT();

        int ix;
        int iy;
        float cx;
        float cy;
        QGraphicsEllipseItem *grEllipse;
    };

    QVector<PATHPOINT> vPoints;
    QPainterPath path;
    QGraphicsPathItem *grPath;

    PATHFINDER *pathfinder;
    QGraphicsScene *gs;
    QCellWidget *mapview;

    QColor    clrPathLine;
    QPen      pnPathLine;

    QColor    clrDstE;
    QColor    clrDstI;
    QBrush    brDstI;
    QPen      pnDstE;

    QColor    clrSrcE;
    QColor    clrSrcI;
    QBrush    brSrcI;
    QPen      pnSrcE;

    QColor    clrNorI;
    QColor    clrNorE;
    QBrush    brNorI;
    QPen      pnNorE;
};
