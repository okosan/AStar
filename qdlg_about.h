﻿#pragma once
#include "stable.h"

#include "ui_qdlg_about.h"

class DlgAbout : public QDialog, public Ui::DlgAbout
{
    Q_OBJECT
public:
    DlgAbout(QWidget *parent = 0,int w=1,int h=1) : QDialog(parent,0)
    {
        Q_UNUSED(w);
        Q_UNUSED(h);
        this->setupUi(this);
        this->retranslateUi(this);
    }

    ~DlgAbout() { }
};
