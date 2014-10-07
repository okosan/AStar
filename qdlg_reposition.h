#pragma once

#include "stdafx.h"

#include "ui_qdlg_reposition.h"

#include <QGraphicsRectItem>
#include <QGraphicsOpacityEffect>
#include <QObject>

const float rect_side = 70.0f;

namespace Dlg_Reposition
{
    const QColor    COLOR_BK(0,255,128);
    const QColor    COLOR_ORIGINAL(128,0,0xFF);
    const QColor    COLOR_MOVE(0,128,0);

    const QBrush    BRUSH_BK(COLOR_BK);
    const QBrush    BRUSH_ORIGINAL(COLOR_ORIGINAL);
    const QBrush    BRUSH_MOVE(COLOR_MOVE);

    const QColor    COLOR_RECTBOUND(106,181,255);
    const QPen      PEN_RECTBOUND(COLOR_RECTBOUND);
}

class XGraphicsRectItemRepo : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    XGraphicsRectItemRepo( const QRectF & rect, QGraphicsItem * parent = 0 ): 
        QGraphicsRectItem (rect, parent = 0 )
    {

    }

protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value)
    {
        if (change == ItemPositionChange && scene())
        {
            // value is the new position.
            QPointF newPos = value.toPointF();
            //QRectF rect(-rect_side+1,-rect_side+1,rect_side*2-2,rect_side*2-2);
            QRectF rect(-rect_side,-rect_side,rect_side*2,rect_side*2);
            if (!rect.contains(newPos))
            {
                // Keep the item inside the scene rect.
                newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
                newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
                emit positionChanged(newPos.x(),newPos.y());
                return newPos;
            }
            emit positionChanged(newPos.x(),newPos.y());

        }
        return QGraphicsItem::itemChange(change, value);
    }

signals:
    void positionChanged(int posX,int posY);
};


class DlgReposition : public QDialog, public Ui::DlgReposition
{
    Q_OBJECT

private:
    QGraphicsScene gs;
    QGraphicsRectItem *rcMain;
    XGraphicsRectItemRepo *rcNew;
    QGraphicsOpacityEffect grEffect;

    int tx,ty;
    int w,h;

    /*
    int positionXToTranslate(int posx)
    {
    return (float)posx/(float)rect_side * (float)w;
    }

    int positionYToTranslate(int posy)
    {
    return (float)posy/(float)rect_side * (float)h;
    }
    */

    int txToPosX(int tx)
    {
        return ((float)tx/(float)w)*rect_side;
    }

    int tyToPosY(int ty)
    {
        return ((float)ty/(float)h)*rect_side;
    }

    void updateRectPos()
    {
        if (rcNew)
        {
            rcNew->setPos(txToPosX(tx),tyToPosY(ty));
            gfxPosition->centerOn(rect_side/2.0f,rect_side/2.0f);
        }
        updateText();
    }

    void updateText()
    {
        lblPosition->setText(QString("tx = %1, ty = %2").arg(tx).arg(ty));
    }

public:
    DlgReposition(QWidget *parent = 0,int w_ = -1,int h_ = -1) : QDialog(parent,0)
    {
        this->setupUi(this);
        this->retranslateUi(this);

        w = w_;
        h = h_;

        tx = ty = 0;
        updateText();

        sbHorizontal->setMinimum(-w);
        sbHorizontal->setMaximum(w);

        sbVertical->setMinimum(-h);
        sbVertical->setMaximum(h);

        rcMain = gs.addRect(QRectF(-rect_side/2.0f,-rect_side/2.0f,rect_side,rect_side),
            Dlg_Reposition::PEN_RECTBOUND,
            Dlg_Reposition::BRUSH_ORIGINAL);

        rcNew = new XGraphicsRectItemRepo(QRectF(-rect_side/2.0f,-rect_side/2.0f,rect_side,rect_side));
        rcNew->setPen(Dlg_Reposition::PEN_RECTBOUND);
        rcNew->setBrush(Dlg_Reposition::BRUSH_MOVE);
        gs.addItem(rcNew);

        rcNew->setGraphicsEffect(&grEffect);

        rcNew->setFlags(QGraphicsItem::ItemIsMovable|
            //QGraphicsItem::ItemSendsScenePositionChanges|
            QGraphicsItem::ItemSendsGeometryChanges);
        gs.setBackgroundBrush(Dlg_Reposition::BRUSH_BK);
        gs.setSceneRect(QRectF(-1.1*rect_side,-1.1*rect_side,rect_side*2.2,rect_side*2.2));



        gfxPosition->centerOn(rect_side/2.0f,rect_side/2.0f);
        gfxPosition->setAlignment(Qt::AlignCenter);
        //gfxPosition->setInteractive(false);
        gfxPosition->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        gfxPosition->setResizeAnchor(QGraphicsView::AnchorViewCenter);
        //gfxPosition->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);

        gfxPosition->setScene(&gs);





        //this->layout()->setSizeConstraint(QLayout::SetFixedSize);

        // alerady set up in 
        QApplication::connect(sbVertical,SIGNAL(valueChanged(int)),this,SLOT(slotVerticalSbChanged(int)) );
        QApplication::connect(sbHorizontal,SIGNAL(valueChanged(int)),this,SLOT(slotHorizontalSbChanged(int)) );

        QApplication::connect(rcNew,SIGNAL(positionChanged(int,int)),this,SLOT(slotRectangleMoved(int,int)) );
    }

    ~DlgReposition()
    {

    }

    void getTXY(int &retTx,int &retTy)
    {
        retTx = tx;
        retTy = ty;
    }


    private slots:
        void slotHorizontalSbChanged(int valueX)
        {
            tx = valueX;
            updateRectPos();
        }

        void slotVerticalSbChanged(int valueY)
        {
            // direct correspondance between translation and slider
            ty = valueY;
            updateRectPos();
        }    

        void slotRectangleMoved(int posX,int posY)
        {
            // x & y are screen coordinates of rectangle
            // tx,ty - absolute coords

            // scroll values:
            //Beep(100,100);
            int scrX = (float)posX/((float)rect_side)*(float)w;
            int scrY = (float)posY/((float)rect_side)*(float)h;
            if (tx == scrX && ty == scrY) return;

            tx = scrX;
            ty = scrY;

            sbHorizontal->setValue(scrX);
            sbVertical->setValue(scrY);

            updateText();
        }
};
