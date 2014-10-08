#pragma once
#include "stable.h"

#include "ui_qdlg_howto.h"

class DlgHowto : public QDialog, public Ui::DlgHowto
{
    Q_OBJECT
public:
    DlgHowto(QWidget *parent = 0,int w=1,int h=1) : QDialog(parent,0)
    {
        //Beep(1000,1000);
        this->setupUi(this);
        this->retranslateUi(this);

        // alerady set up in
        //QApplication::connect(this->okButton,SIGNAL(clicked()),this,SLOT(accept()));
        //QApplication::connect(this->cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
    }

    ~DlgHowto()
    {

    }
};
