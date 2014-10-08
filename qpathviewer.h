#pragma once
#include "stable.h"

#include "pathfinder.h"
#include "qcellwidget.h"

const float DEFAULT_CIRCLE_RADIUS = 6.5f;

class PATHVIEWER
{
private:
    class PATHPOINT
    {
    public:
        int ix,iy;
        QGraphicsEllipseItem *grEllipse;
        float cx,cy;
        PATHPOINT(): grEllipse(0),cx(0),cy(0),ix(0),iy(0) { };

        /*
        ~PATHPOINT()
        {
        if (grEllipse) delete grEllipse;
        };
        */
    };

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


private:
    QVector<PATHPOINT> vPoints;
    QPainterPath path;
    QGraphicsPathItem *grPath;

    PATHFINDER *pathfinder;
    QGraphicsScene *gs;
    QCellWidget *mapview;

public:
    PATHVIEWER() :
        grPath(0),
        pathfinder(0),gs(0),mapview(0)
    {
        clrPathLine = QColor(0x99,0x00,0x00);
        pnPathLine = QPen(clrPathLine,3);

        clrDstI = QColor(0x66,0x77,0xFF);
        clrDstE = QColor(0x66,0x44,0xFF);
        brDstI = QBrush(clrDstI);
        pnDstE = QPen(clrDstE,3);

        clrSrcI = QColor(0xff,0x44,0x66);
        clrSrcE = QColor(0xdd,0x00,0x00);
        brSrcI = QBrush(clrSrcI);
        pnSrcE = QPen(clrSrcE,3);

        clrNorI = QColor(0x11,0xbb,0x00);
        clrNorE = QColor(0x77,0xFF,0x11);
        brNorI = QBrush(clrNorI);
        pnNorE = QPen(clrNorE,3);
    }

    int init(PATHFINDER *pathfinder_,QCellWidget *mapview_,
        QGraphicsScene *gs_)
    {
        pathfinder = pathfinder_;
        gs = gs_;
        mapview = mapview_;
        vPoints.clear();

        return 0;
    }

    int clearPath()
    {
        if (!grPath) return 1;
        path = QPainterPath();

        grPath->setPath(path);
        PATHPOINT pp;

        foreach(pp,vPoints)
        {
            if (pp.grEllipse)
            {
                gs->removeItem(pp.grEllipse);
                delete pp.grEllipse;
                pp.grEllipse = 0;

            };
        };
        vPoints.clear();
        return 0;
    }

    int addPoint(int ix,int iy)
    {
        QRectF rcf;
        if (XFAIL(mapview->getCellOrignF(ix,iy,rcf))) return 1;

        float cx = rcf.left() + rcf.width()/2.f;
        float cy = rcf.top() + rcf.height()/2.f;

        QRectF rcfCircle(cx - DEFAULT_CIRCLE_RADIUS,cy - DEFAULT_CIRCLE_RADIUS,
            2.f*DEFAULT_CIRCLE_RADIUS, 2.f*DEFAULT_CIRCLE_RADIUS);

        PATHPOINT pp;

        pp.ix = ix;
        pp.iy = iy;
        pp.cx = cx;
        pp.cy = cy;

        if (vPoints.isEmpty())
        {
            path.moveTo(cx,cy);
            grPath = gs->addPath(path,pnPathLine);

            pp.grEllipse = gs->addEllipse(rcfCircle,pnSrcE,brSrcI);
        }
        else
        {
            path.lineTo(cx,cy);
            grPath->setPath(path);

            pp.grEllipse = gs->addEllipse(rcfCircle,pnNorE,brNorI);
        }
        vPoints.push_back(pp);

        return 0;
    }

    int update(int fullupdate = 0)
    {
        clearPath();
        // compose path here
        //pathfinder->genPathList();
        CELL cell;
        foreach(cell,pathfinder->pathList)
        {
            addPoint(cell.ix,cell.iy);
        };

        /*
        addPoint(5,5);
        addPoint(5,10);
        addPoint(9,10);
        addPoint(10,10);
        //addPoint(3,12);
        addPoint(11,10);
        addPoint(11,14);
        */

        return 0;
    }

};
