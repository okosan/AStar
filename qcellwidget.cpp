#include "stable.h"

#include "qcellwidget.h"


bool QCellWidget::isValidCell(int ix,int iy)
{
    if (map) return map->isValid(ix,iy); // simple envelope
    return 0;
}

int QCellWidget::init_selection()
{
    // create sel box
    QRectF rcfSel;
    if (cellsize > 2.0f)
        rcfSel = QRectF(1.0,1.0f,cellsize-2.0f,cellsize-2.0f);
    else
        rcfSel = QRectF(0.0,0.0f,cellsize,cellsize);
    selectRect = gs->addRect(rcfSel,PEN_SEL,BRUSH_SELFIL);
    selectRect->hide(); // no selection visible by default
    selectRect->setZValue(1.0f); // topmost

    selectEffect = new QGraphicsOpacityEffect();
    selectEffect->setOpacity(0.65f);
    selectRect->setGraphicsEffect(selectEffect);
    selx = -1;
    sely = -1;

    return 0;
}

bool QCellWidget::selection_initialized()
{
    if (selectRect != NULL) return true;
    return false;
}

int QCellWidget::free_selection()
{
    if (gs)
    {
        if (selectRect)
        {
            gs->removeItem(selectRect);
            delete selectRect;
            selectRect = NULL;
        }
    }

    if (selectEffect)
    {
        delete selectEffect;
        selectEffect = NULL;
    }
    selx = sely = -1;
    return 0;
}

int QCellWidget::init(MAPDATA *map_,QGraphicsScene *gs_,int cellsize_)
{
    cellsize = cellsize_;
    map = map_;
    gs = gs_;

    fzoom = 1.0;
    selx = -1;
    sely = -1;

    this->setScene(gs);

    return 0;
}

inline int QCellWidget::getCellAtPos(float x, float y, int &ix, int &iy)
{
    ix=(int)(floor(x/cellsize));
    iy=(int)(floor(y/cellsize));
    if (isValidCell(ix,iy)) return 0;
    return 1; // cell index is invalid
}

inline int QCellWidget::getCellRect(int ix, int iy, RECT &rc)
{
    rc.left = ix*cellsize;
    rc.right = (ix+1)*cellsize;
    rc.top = iy*cellsize;
    rc.bottom = (iy+1)*cellsize;
    return 0;
}

inline int QCellWidget::getCellOrignF(int ix, int iy, QRectF &rcf)
{
    rcf = QRectF((float)(ix*cellsize),(float)(iy*cellsize),
        (float)(cellsize),(float)(cellsize));
    return 0;
}

inline int QCellWidget::setCellDst(int ix,int iy)
{
    if (isValidCell(ix,iy))
    {
        map->setCost(ix,iy,CELL_DST);

        QGraphicsRectItem * clickRect;
        if (XFAIL(arrRects.getItem(ix,iy,clickRect))) return 2;
        clickRect->setBrush(BRUSH_DST);

        return 0;
    }
    return 1;
}

inline int QCellWidget::setCellSrc(int ix,int iy)
{
    if (isValidCell(ix,iy))
    {
        map->setCost(ix,iy,CELL_SRC);

        QGraphicsRectItem * clickRect;
        if (XFAIL(arrRects.getItem(ix,iy,clickRect))) return 2;
        clickRect->setBrush(BRUSH_SRC);

        return 0;
    }
    return 1;
}

inline int QCellWidget::setCellWall(int ix,int iy)
{
    if (isValidCell(ix,iy))
    {
        map->setCost(ix,iy,CELL_WALL);

        QGraphicsRectItem *clickRect;
        if (XFAIL(arrRects.getItem(ix,iy,clickRect))) return 2;
        clickRect->setBrush(BRUSH_WALL);

        return 0;
    }
    return 1;
}

inline int QCellWidget::setCellClear(int ix, int iy)
{
    if (isValidCell(ix,iy))
    {
        map->setCost(ix,iy,CELL_CLEAR);

        QGraphicsRectItem * clickRect;
        if (XFAIL(arrRects.getItem(ix,iy,clickRect))) return 2;
        clickRect->setBrush(BRUSH_CLEAR);

        return 0;
    }
    return 1;
}

int QCellWidget::setCellValue(int ix, int iy,float val)
{
    if (val == CELL_CLEAR) return setCellClear(ix,iy);
    if (val == CELL_WALL) return setCellWall(ix,iy);
    if (isValidCell(ix,iy))
    {
        map->setCost(ix,iy,val);

        QGraphicsRectItem * clickRect;
        if (XFAIL(arrRects.getItem(ix,iy,clickRect))) return 2;

        QBrush brCostDiff = QBrush(getGradientCellColor(val));
        clickRect->setBrush(brCostDiff);

        return 0;
    }
    return 1;
}

int QCellWidget::free() // free all objects (requires initialization after)
{
    // delete all rectangles
    for (int iy = 0; iy < arrRects.height(); iy++)
        for (int ix = 0; ix < arrRects.width(); ix++)
        {
            QGraphicsRectItem *grRect;
            if (XFAIL(arrRects.getItem(ix,iy,grRect))) return 1;
            if (grRect) gs->removeItem(grRect);
            delete grRect;
            arrRects.setItem(ix,iy,NULL);
        };

        // free array memory
        arrRects.destroy();
        w = 0;
        h = 0;

        return 0;
}

int QCellWidget::clear() // clear map and objects stands previous
{
    map->clear();
    this->update(0);

    return 0;
}

// if full update - delete objects and recreate, else - size of map is previous
// and update cells colors, dest & src cells
int QCellWidget::update(int fullupdate)
{
    /*
    create cells rects to correspond map data
    set cells position
    set cells brush
    */

    if (!gs || !map) return 1;

    // if there is no fullupdate - check if it is needed?
    if (!fullupdate)
    {
        if (w != map->width() || h != map->height()) fullupdate = 1;
        if (arrRects.width() == 0 || arrRects.height() == 0) fullupdate = 1;
    }

    if (fullupdate)
    {
        free();

        //gs->clear();

        w = map->width();
        h = map->height();

        if (XFAIL(arrRects.alloc(w,h))) return 2; // unable to realloc

        QGraphicsScene gsNull;
        this->setScene(&gsNull);
        gs->setSceneRect(getSceneRectBorder(w,h));

        /*
        // not parallel
        for (int iy = 0; iy < h; iy++)
        for (int ix = 0; ix < w; ix++)
        {
        QGraphicsRectItem *rci;
        if (XFAIL(arrRects.getItem(ix,iy,rci))) return 2;
        QRectF rcf;
        getCellOrignF(ix,iy,rcf);
        float fCost;
        if (XFAIL(map->getCost(ix,iy,fCost))) return 3;

        // create new rect depending on cell cost
        if (fCost==CELL_CLEAR)     rci = gs->addRect(rcf,PEN_RECTBOUND);
        else
        if (fCost==CELL_WALL)     rci = gs->addRect(rcf,PEN_RECTBOUND,BRUSH_WALL);
        else
        if (fCost==CELL_SRC)     rci = gs->addRect(rcf,PEN_RECTBOUND,BRUSH_SRC);
        else
        if (fCost==CELL_DST)     rci = gs->addRect(rcf,PEN_RECTBOUND,BRUSH_DST);
        else
        if (xfBetween(fCost,0,1))
        {
        QBrush brCostDiff = QBrush(getGradientCellColor(fCost));
        rci = gs->addRect(rcf,PEN_RECTBOUND,brCostDiff);
        }

        if (XFAIL(arrRects.setItem(ix,iy,rci))) return 4;
        };
        */

        int iy;
        //#pragma om p parallel for private(iy)
        for (iy = 0; iy < h; iy++)
            for (int ix = 0; ix < w; ix++)
            {
                QGraphicsRectItem *rci;
                arrRects.getItem(ix,iy,rci);
                QRectF rcf;
                getCellOrignF(ix,iy,rcf);
                float fCost;
                map->getCost(ix,iy,fCost);

                // create new rect depending on cell cost
                if (fCost==CELL_CLEAR)     rci = gs->addRect(rcf,PEN_RECTBOUND);
                else
                    if (fCost==CELL_WALL)     rci = gs->addRect(rcf,PEN_RECTBOUND,BRUSH_WALL);
                    else
                        if (fCost==CELL_SRC)     rci = gs->addRect(rcf,PEN_RECTBOUND,BRUSH_SRC);
                        else
                            if (fCost==CELL_DST)     rci = gs->addRect(rcf,PEN_RECTBOUND,BRUSH_DST);
                            else
                                if (xfBetween(fCost,0,1))
                                {
                                    QBrush brCostDiff = QBrush(getGradientCellColor(fCost));
                                    rci = gs->addRect(rcf,PEN_RECTBOUND,brCostDiff);
                                }

                                arrRects.setItem(ix,iy,rci);
            };

            this->setAlignment(Qt::AlignCenter);
            this->setResizeAnchor(QGraphicsView::AnchorViewCenter);

            this->setScene(gs);

            this->zoomFitAll();



    }
    else
    {
        // no realloc
        for (int iy = 0; iy < h; iy++)
            for (int ix = 0; ix < w; ix++)
            {
                QGraphicsRectItem *rci;
                if (XFAIL(arrRects.getItem(ix,iy,rci))) return 2;
                if (!rci) return 3;

                float fCost;
                if (XFAIL(map->getCost(ix,iy,fCost))) return 4;

                // modify brush because map structure has modified
                if (fCost==CELL_CLEAR)
                {
                    rci->setBrush(BRUSH_CLEAR);
                    continue;
                }
                if (fCost==CELL_WALL)
                {
                    rci->setBrush(BRUSH_WALL);
                    continue;
                }
                if (fCost==CELL_SRC)
                {
                    rci->setBrush(BRUSH_SRC);
                    continue;
                }
                if (fCost==CELL_DST)
                {
                    rci->setBrush(BRUSH_DST);
                    continue;
                }

                if (xfBetween(fCost,0,1))
                {
                    QBrush brCostDiff = QBrush(getGradientCellColor(fCost));
                    rci->setBrush(brCostDiff);
                    continue;
                }
            };
    };

    //QApplication::beep();

    if (!selection_initialized()) init_selection();

    return 0;
}

int QCellWidget::setCellSize(int side_size)
{
    if (side_size > 1)
    {
        cellsize = side_size;
        update(1);
        return 0;
    }
    return 1;
}

void QCellWidget::zoomIn()
{
    fzoom += 0.25;
    if (fzoom > 10) fzoom = 10;
    zoomTo(fzoom);
}

void QCellWidget::zoomOut()
{
    // get ceil value of current zoom

    // decrement index
    // get needed zoom value from array[index]

    fzoom -= 0.25;
    if (fzoom < 0.25) fzoom = 0.25;
    zoomTo(fzoom);
}

void QCellWidget::zoomTo(float value)
{
    QTransform transf;
    transf.reset();
    transf.scale(value,value);
    this->setTransform(transf);
}

void QCellWidget::zoomReset()
{
    fzoom = 1.0;
    //this->resetTransform();
    QTransform transf;
    transf.reset();
    transf.scale(fzoom,fzoom);
    this->setTransform(transf);
}

void QCellWidget::zoomFitAll()
{
    // determine zoom
    float border = cellsize/2.0;

    this->fitInView(QRectF(-border,-border,w*cellsize+2*border,h*cellsize+2*border),Qt::KeepAspectRatio);

    float cx = ((float)w/2.0f) * cellsize;
    float cy = ((float)h/2.0f) * cellsize;

    this->centerOn(cx,cy);
    //fzoom =
    QTransform transf = this->transform();
    fzoom = transf.m11();
}

bool QCellWidget::hasSelection()
{
    if ((selx != -1)&&(sely != -1)) return 1;
    return 0;
}

int QCellWidget::selection(int &ix,int &iy)
{
    ix = selx;
    iy = sely;
    return 0;
}

float QCellWidget::selCost()
{
    float cost_;
    if (XFAIL(map->getCost(selx,sely,cost_))) return CELL_WALL;
    return cost_;
}

int QCellWidget::clearSelection()
{
    if (!selectRect) return 1;
    selectRect->hide();
    selx = -1;
    sely = -1;
    emit selectionChanged(-1,-1);
    return 0;
}

int QCellWidget::selectItem(int ix,int iy)
{
    if (!selectRect) return 1;
    if (!isValidCell(ix,iy)) return 2;
    if ((selx != ix)||(sely != iy))
    {
        selx = ix;
        sely = iy;

        QRectF rcfSel;
        getCellOrignF(ix,iy,rcfSel);
        selectRect->setPos(rcfSel.left(),rcfSel.top());
        selectRect->show();
        emit selectionChanged(selx,sely);
    }
    return 0;
}

int QCellWidget::triggerItem(int ix,int iy,QInputEvent *ie)
{
    bool shift     = (ie->modifiers() & Qt::ShiftModifier)? 1:0;
    bool ctrl     = (ie->modifiers() & Qt::ControlModifier)? 1:0;
    bool alt     = (ie->modifiers() & Qt::AltModifier)? 1:0;
    triggerItem(ix,iy,shift,ctrl,alt);
    return 0;
}

int QCellWidget::triggerItem(int ix,int iy,bool shift,bool ctrl, bool alt)
{
    //Beep(1000,30);
    if (!isValidCell(ix,iy)) return 1;

    float fCost;
    if (XFAIL(map->getCost(ix,iy,fCost))) return 1;

    if (fCost == CELL_CLEAR)
    {
        // cell is clear
        if (shift) setCellSrc(ix,iy);
        else
            if (ctrl)     setCellDst(ix,iy);
            else setCellWall(ix,iy);
    }
    else
    {
        if (shift)
            if (fCost == CELL_SRC) setCellClear(ix,iy);
            else setCellSrc(ix,iy);
        else
            if (ctrl)
                if (fCost == CELL_DST) setCellClear(ix,iy);
                else setCellDst(ix,iy);
            else
                setCellClear(ix,iy);
    }
    return 0;
}

void QCellWidget::mousePressEvent(QMouseEvent *me)
{
    QPointF posView = this->mapToScene(me->pos());

    int ix,iy;

    int posResult = getCellAtPos(posView.x(),posView.y(),ix,iy);

    if (me->button() == Qt::RightButton)
    {
        if (XFAIL(posResult))
        {
            if (this->hasSelection()) clearSelection();
            return;
        };

        selectItem(ix,iy);
        return;
    }

    if (XFAIL(posResult)) return;

    triggerItem(ix,iy,me);

    return;
}

void QCellWidget::keyPressEvent(QKeyEvent *ke)
{
    int key = ke->key();
    switch(key)
    {
    case(Qt::Key_Right):
        if (!hasSelection()) selectItem(0,0);
        else
        {
            if (XFAIL(selectItem(selx+1,sely))) break;
        }
        break;
    case(Qt::Key_Left):
        if (!hasSelection()) selectItem(0,0);
        else
        {
            if (XFAIL(selectItem(selx-1,sely))) break;
        }
        break;
    case(Qt::Key_Up):
        if (!hasSelection()) selectItem(0,0);
        else
        {
            if (XFAIL(selectItem(selx,sely-1))) break;
        }
        break;
    case(Qt::Key_Down):
        if (!hasSelection()) selectItem(0,0);
        else
        {
            if (XFAIL(selectItem(selx,sely+1))) break;
        }
        break;
    case(Qt::Key_Space):
        triggerItem(selx,sely,ke);
        break;

    case(Qt::Key_Plus):
        this->zoomIn();
        emit(mapChanged());
        break;
    case(Qt::Key_Minus):
        this->zoomOut();
        emit(mapChanged());
        break;
    case(Qt::Key_Equal):
        this->zoomFitAll();
        emit(mapChanged());
        break;

    }
}

int QCellWidget::saveToPicture(QString fn)
{
    QRectF mpr;
    if (XFAIL(getMapRect(mpr))) return 1;

    QImage image(mpr.width(),mpr.height(),QImage::Format_RGB888);
    QPainter painter(&image);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    image.fill(0xFFFFFFFF);
    gs->render(&painter);
    if (image.save(fn)==false)
    {
        xfBeep(3000,50);
        /*QMessageBox msgError(QString("failed"),
        QString("failed to save image"),
        QMessageBox::Icon::Critical,
        QMessageBox::Ok,0,0);
        msgError.exec();
        */
        return 1;
    }

    return 0;
}

int QCellWidget::saveToPrinter(QPrinter *prn)
{
    QRectF mpr;
    if (XFAIL(getMapRect(mpr))) return 1;

    prn->setFullPage(0);
    QPainter painter;
    painter.begin(prn);

    //QRectF
    QRectF ppr = prn->pageRect();

    float scalef = 0.8f;
    float pw = ppr.width() * scalef;
    float ph = ppr.height() * scalef;
    float cx = (ppr.right() + ppr.left() ) /2.0f;
    float cy = (ppr.bottom() + ppr.top() ) /2.0f;
    QRectF ppr2 = QRectF(cx - pw/2,cy - ph / 2,pw,ph);

    gs->render(&painter,ppr2,mpr);

    painter.end();

    return 0;
}

int QCellWidget::getMapRect(QRectF &rcf)
{
    if (w == 0 || h == 0) return 1;
    float widthf = w * cellsize;
    float heightf = h * cellsize;
    rcf = QRectF(0,0,widthf,heightf);

    return 0;
}

QColor QCellWidget::getGradientCellColor(float fCost)
{
    int max = 250;
    int min = 32;
    int color = (int)((1.0-fCost)*(max-min)+min);
    //return QColor(color,color,color);
    return QColor(16,color,32);
}

// int QCellWidget::loadMapCostFromImage(QString fn, int originx, int originy, bool f_replace_walls)
// read into map where?
int QCellWidget::loadMapCostFromImage(QString fn, bool f_replace_walls)
{
    QImage imgld;
    if (imgld.load(fn) == false) return 1; // default format is not speceifed
    QImage img = imgld.convertToFormat(QImage::Format_RGB888); // 24 bit image
    if (img.isNull()) return 2;

    // 255 means wall
    // 0 - means clear

    int imgw = img.width();
    int imgh = img.height();

    if ( imgw < w || imgh < h ) return 1; // not suitable size : fix this in next version !!!

    for (int iy = 0; iy < h; iy++)
        for (int ix = 0; ix < w; ix++)
        {
            if (f_replace_walls)
            {

            }
            else
            {
                float fCurrentCost;
                if (XFAIL(map->getCost(ix,iy,fCurrentCost))) return 3;
                if (fCurrentCost == CELL_WALL)
                    continue;
            }

            QRgb rgbPixel = img.pixel(ix,iy);
            float fTargetCost = (float)(rgbPixel&0xFF) / 255.0f;

            if (XFAIL(map->setCost(ix,iy,fTargetCost))) return 4;
        }

        this->update(0);

        return 0;
}

int QCellWidget::saveMapCostToImage(QString fn)
{
    if (w <= 0 || h <= 0) return 1;
    QImage img(w,h,QImage::Format_RGB888); // 24 bit image. Qimage optimized for per-pixel operations
    if (img.isNull()) return 2;

    // 255 means wall
    // 0 - means clear

    for (int iy = 0; iy < h; iy++)
    {
        for (int ix = 0; ix < w; ix++)
        {
            float fCurrentCost;
            if (XFAIL(map->getCost(ix,iy,fCurrentCost))) return 3;
            if (fCurrentCost == CELL_SRC || fCurrentCost == CELL_DST)
                fCurrentCost = 0; // Replace src & dst costs with zeros

            img.setPixel(ix,iy,std::min((int)(fCurrentCost*255.0f),255));
        }
    }

    img.save(fn);

    return 0;
}

int QCellWidget::translateMap(int tx,int ty,float fill_value)
{
    map->translation(tx,ty,fill_value);
    this->update(0);
    return 0;
}
