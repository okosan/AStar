#pragma once
#include "stable.h"


#include "ui_qmain.h"
#include "qdlg_resize.h"
#include "qdlg_howto.h"
#include "qdlg_about.h"
#include "qdlg_reposition.h"

#include "map.h"

#include "qcellwidget.h"
#include "qpathviewer.h"

#include "pathfinder.h"


const float FZOOM_FIT = 10000.0f;
const float FZOOM_FITWIDTH = 11000.0f;


class WndMain : public QMainWindow, public Ui::WndMain
{
    Q_OBJECT

public:
    WndMain(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~WndMain();

    int InitMap();

    //virtual void focusOutEvent(QFocusEvent *fe);

    void findPath(int flags);

    public slots:
        void slotRedrawScene();

        void slotFindPathShallow() { findPath(PATHFINDER::FINDFLAG_SHALLOW); }
        void slotFindPathDeep() { findPath(PATHFINDER::FINDFLAG_DEEP); }

        void slotZoomNormal();
        void slotZoomIn();
        void slotZoomOut();
        void slotZoomTo(double z);

        void slotSelectionChanged(int six, int siy);
        void slotSelectionValueChanged(int val);

        void slotExportAsPicture()
        {
            QString retFn = QFileDialog::getSaveFileName(this,tr("Save Map as Pictre"),
                "",
                tr(     "Windows bitmap (*.bmp);;"\
                "PNG (*.png)"));
            // dialog save file
            ui.cellView->saveToPicture(retFn);
        }

        void slotPrint()
        {
            QPrinter prn(QPrinter::HighResolution);
            char bufStr[256];
            sprintf(bufStr,"Map%dx%d",map.width(),map.height());
            prn.setDocName(QString(bufStr));
            prn.setPageSize(QPrinter::A4);

            QPrintDialog dlgPrint(&prn,this);
            dlgPrint.exec();

            ui.cellView->saveToPrinter(&prn);
        }

        void slotClearMap()
        {
            pathfinder.clearPath();
            pathviewer.update(1);

            ui.cellView->clear();
        }

        void slotHowto()
        {
            DlgHowto dlgHowto(this);
            dlgHowto.exec();
        }

        void slotAbout()
        {
            DlgAbout dlgAbout(this);
            dlgAbout.exec();
        }

        void slotResizeMap()
        {
            int w = map.width();
            int h = map.height();

            DlgResizeMap dlgResize(this,w,h);
            if (dlgResize.exec() == QDialog::Accepted)
            {
                int w1 = dlgResize.width();
                int h1 = dlgResize.height();

                if (w1 == w && h1 == h) return;

                pathviewer.clearPath();
                pathfinder.clearPath();
                pathviewer.update(1);

                map.resize(w1,h1);

                ui.cellView->update(1);

            }
        }

        // int reason = 0;
        void slotSceneChanged()
        {
            ui.sbZoom->setValue(ui.cellView->zoom());

        }

        void slotFileExit()
        {
            //Beep(1000,100);
            this->close();
        }

        void slotFileLoad()
        {
            //QFileDialog filedlg;
            QString retFn = QFileDialog::getOpenFileName(this,tr("Open Map File"),
                "Maps/",
                tr("Map file (*.map.txt)"));
            if (!XFAIL(LoadMap(retFn)))
                strCurFile = retFn;
        }

        void slotFileSave()
        {
            if (strCurFile.isEmpty())
                emit(slotFileSaveAs());
            SaveMap(strCurFile);
        }

        void slotFileSaveAs()
        {
            QString retFn = QFileDialog::getSaveFileName(this,tr("Save Map File"),
                "Maps/",
                tr("Map file (*.map.txt)"));

            if (!XFAIL(SaveMap(retFn)))
                strCurFile = retFn;
        }



        void slotLoadCostMapImage();
        void slotSaveCostMapImage();
        void slotRepositionMap();



        int LoadMap(QString fn);
        int SaveMap(QString fn);

        void LoadConfig();
        void SaveConfig();

private:
    Ui::WndMain ui;
    QGraphicsScene gs;
    QTimer tmrRedraw;

    MAPDATA map;
    PATHFINDER pathfinder;
    PATHVIEWER pathviewer;

    QString strCurFile;

    void initMenuCommands();

    int mapWDefault,mapHDefault;
    QGraphicsScene gsSelection;

    QStringList lstZoomStr;
    QVector<float> vZoom;
    QRegExp     reZoom;
    QRegExpValidator *prevZoom;

    void addZoom(float percents)
    {
        QString strZoomDesc;
        if (percents == FZOOM_FIT)
            strZoomDesc = QString("Fit In View");
        else
            if (percents == FZOOM_FITWIDTH)
                strZoomDesc = QString("Fit Width");
            else
                strZoomDesc = QString("%1%").arg((int)percents);
        lstZoomStr << strZoomDesc;
        vZoom.push_back(percents);
    }

    void initZoom()
    {
        addZoom(500.0f);
        addZoom(200.0f);
        addZoom(150.0f);
        addZoom(100.0f);
        addZoom(75.0f);
        addZoom(50.0f);
        addZoom(25.0f);
        addZoom(10.0f);
        addZoom(5.0f);
        addZoom(FZOOM_FIT);
        addZoom(FZOOM_FITWIDTH);

        if (prevZoom==0)
        {
            reZoom = QRegExp("[1-9]\\d{0,3}");
            prevZoom = new QRegExpValidator(reZoom,0);
            ui.cbZoom->setValidator(prevZoom);
        }

        ui.cbZoom->addItems(lstZoomStr);
    }

protected:
    void closeEvent(QCloseEvent *event)
    {
        Q_UNUSED(event);
        SaveConfig();
    }

};
