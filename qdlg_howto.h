﻿#pragma once
#include "stable.h"

#include "ui_qdlg_howto.h"

class DlgHowto : public QDialog, public Ui::DlgHowto
{
    Q_OBJECT
public:
    DlgHowto(QWidget *parent = 0,int w=1,int h=1) : QDialog(parent,0)
    {
        Q_UNUSED(w);
        Q_UNUSED(h);

        this->setupUi(this);
        this->retranslateUi(this);
    }

    ~DlgHowto()
    {

    }
};
