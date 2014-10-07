#pragma once

#include "stdafx.h"

#include "ui_qdlg_resize.h"

class DlgResizeMap : public QDialog, public Ui::dlgResize
{
    Q_OBJECT
public:
    DlgResizeMap(QWidget *parent = 0,int w=1,int h=1) : QDialog(parent,0)
    {
        //Beep(1000,1000);
        this->setupUi(this);
        this->retranslateUi(this);
        this->spbWidth->setValue(w);
        this->spbHeight->setValue(h);
        W = w;
        H = h;
        QApplication::connect(this->spbWidth,SIGNAL(valueChanged(int)),this,SLOT(slotSetWidth(int)));
        QApplication::connect(this->spbHeight,SIGNAL(valueChanged(int)),this,SLOT(slotSetHeight(int)));

        // alerady set up in 
        //QApplication::connect(this->okButton,SIGNAL(clicked()),this,SLOT(accept()));
        //QApplication::connect(this->cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
    }

    ~DlgResizeMap()
    {

    }

    int width() { return W; };
    int height() { return H; };

    private slots:
        void slotSetHeight(int h) { H = h; }
        void slotSetWidth(int w) { W = w; }

private:
    int W;
    int H;
};