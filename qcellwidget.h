#pragma once
#include "stable.h"

#include "map.h"
#include "arr2d.h"

const QColor     COLOR_WALL(55,155,255);
const QColor     COLOR_SRC(255,64,0xFF);
const QColor     COLOR_DST(0xFF,0xA0,0);
const QColor     COLOR_WHITE(0xFF,0xFF,0xFF);
const QBrush     BRUSH_WALL(COLOR_WALL);
const QBrush     BRUSH_SRC(COLOR_SRC);
const QBrush     BRUSH_DST(COLOR_DST);
const QBrush     BRUSH_CLEAR;
const QBrush     BRUSH_WHITE(COLOR_WHITE);

const QColor    COLOR_RECTBOUND(106,181,255);
const QPen      PEN_RECTBOUND(COLOR_RECTBOUND);

const QColor     COLOR_SEL(128,128,0);
const QColor     COLOR_SELFIL(64,128,32);

const QBrush     BRUSH_SELBORDER(COLOR_SEL);
const QBrush     BRUSH_SELFIL(COLOR_SELFIL);
const QPen          PEN_SEL(BRUSH_SELBORDER,3);

class QCellWidget : public QGraphicsView
{
    Q_OBJECT
private:
    static const int DEFAULT_CELLSIZE = 48;

    MAPDATA *map;
    QGraphicsScene *gs;

    ARR2D<QGraphicsRectItem *> arrRects;
    QGraphicsRectItem *selectRect;
    QGraphicsOpacityEffect *selectEffect;

    int cellsize;
    int w,h;
    int selx,sely;
    float fzoom;
    bool f_changed;

    int init_selection();
    bool selection_initialized();
    int update_selection();
    int free_selection(); // frees resources taken by selection rect & effects

public:
    QCellWidget(QWidget *parent = 0) : QGraphicsView(parent)
    {
        w = 0;
        h = 0;
        map = 0;
        gs = 0;
        selectRect = 0;

        selectEffect = NULL;
        selectRect = NULL;

        this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    }

    ~QCellWidget() {  }

    int init(MAPDATA *map_,QGraphicsScene *gs_,int cellsize_ = DEFAULT_CELLSIZE);
    inline int getCellAtPos(float x, float y, int &ix, int &iy);
    inline bool isValidCell(int ix,int iy);
    inline int getCellRect(int ix, int iy, RECT &rc);
    inline int getCellOrignF(int ix, int iy, QRectF &rcf);
    inline int setCellDst(int ix,int iy);
    inline int setCellSrc(int ix,int iy);
    inline int setCellWall(int ix,int iy);
    inline int setCellClear(int ix, int iy);
    int setCellValue(int ix, int iy,float val);
    int free(); // free all objects (requires initialization after)
    int clear(); // clear map and objects stands previous
    // if full update - delete objects and recreate, else - size of map is previous
    // and update cells colors, dest & src cells
    int update(int fullupdate = 0);

    int setCellSize(int side_size);
    void zoomIn();
    void zoomOut();
    void zoomTo(float value);
    void zoomReset();
    void zoomFitAll();
    inline float zoom() { return fzoom; }

    bool hasSelection();
    int selection(int &ix,int &iy);
    float selCost();
    int clearSelection();
    int selectItem(int ix,int iy);

    int triggerItem(int ix,int iy,QInputEvent *ie);
    int triggerItem(int ix,int iy,bool shift=0,bool ctrl=0, bool alt=0);

    virtual void mousePressEvent(QMouseEvent * me);
    virtual void keyPressEvent(QKeyEvent *ke);

    int saveToPicture(QString fn);
    int saveToPrinter(QPrinter *prn);

    int getMapRect(QRectF &rcf);

    QColor getGradientCellColor(float fCost);

    int loadMapCostFromImage(QString fn, bool f_replace_walls = 0);
    int saveMapCostToImage(QString fn);

    int translateMap(int tx,int ty,float fill_value);

    QRectF getSceneRect(int w_,int h_)
    {
        return QRectF(0,0,w_*cellsize,h_*cellsize);
    }

    QRectF getSceneRectBorder(int w_,int h_)
    {
        return QRectF(-cellsize/2,-cellsize/2,(w_+1)*cellsize,(h_+1)*cellsize);
    }

public slots:


signals:
        void selectionChanged(int six,int siy);
        void mapChanged();
};


