#include "stdafx.h"
#include "qmain.h"

const int TMR_INTERVAL = 4;

void WndMain::initMenuCommands()
{
    QApplication::connect(ui.actionExit,SIGNAL(triggered()),this,SLOT(slotFileExit()));
    QApplication::connect(ui.actionLoad,SIGNAL(triggered()),this,SLOT(slotFileLoad()));
    QApplication::connect(ui.actionSave,SIGNAL(triggered()),this,SLOT(slotFileSave()));
    QApplication::connect(ui.actionSaveAs,SIGNAL(triggered()),this,SLOT(slotFileSaveAs()));
    QApplication::connect(ui.actionAShallow,SIGNAL(triggered()),this,SLOT(slotFindPathShallow()));
    QApplication::connect(ui.actionADeep,SIGNAL(triggered()),this,SLOT(slotFindPathDeep()));
    QApplication::connect(ui.actionClearMap,SIGNAL(triggered()),this,SLOT(slotClearMap()));
    QApplication::connect(ui.actionResizeMap,SIGNAL(triggered()),this,SLOT(slotResizeMap()));
    QApplication::connect(ui.actionHowto,SIGNAL(triggered()),this,SLOT(slotHowto()));
    QApplication::connect(ui.actionAbout,SIGNAL(triggered()),this,SLOT(slotAbout()));
    QApplication::connect(ui.actionExportAsPicture,SIGNAL(triggered()),this,SLOT(slotExportAsPicture()));
    QApplication::connect(ui.actionPrint,SIGNAL(triggered()),this,SLOT(slotPrint()));
    QApplication::connect(ui.actionLoadCostMap,SIGNAL(triggered()),this,SLOT(slotLoadCostMapImage()));
    QApplication::connect(ui.actionSaveCostMap,SIGNAL(triggered()),this,SLOT(slotSaveCostMapImage()));
    QApplication::connect(ui.actionRepositionMap,SIGNAL(triggered()),this,SLOT(slotRepositionMap()));
}

int WndMain::LoadMap(QString fn)
{
    if (fn.isEmpty()) return 1;
    map.loadFromFile(fn.toLatin1().data());

    pathfinder.clearPath();
    pathviewer.update(1);

    ui.cellView->update(1);

    ui.sbZoom->setValue(ui.cellView->zoom()*100.0f);

    return 0;
}

int WndMain::SaveMap(QString fn)
{
    if (fn.isEmpty())
        return 1;
    map.saveToFile(fn.toLatin1().data());

    return 0;
}

WndMain::WndMain(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    mapWDefault = mapHDefault = 20;
    prevZoom = 0;

    ui.setupUi(this);

    connect(ui.btnRedraw,SIGNAL(clicked()),this,SLOT(slotRedrawScene()));
    connect(ui.btnAstar,SIGNAL(clicked()),this,SLOT(slotFindPathShallow()));
    //connect(ui.btnClose,SIGNAL(clicked()),this,SLOT(slotFileExit()));

    connect(ui.sbZoom,SIGNAL(valueChanged(double)),this,SLOT(slotZoomTo(double)));

    QApplication::connect(ui.cellView,SIGNAL(selectionChanged(int,int)),this,SLOT(slotSelectionChanged(int,int)));
    QApplication::connect(ui.cellView,SIGNAL(mapChanged()),this,SLOT(slotSceneChanged()));
    QApplication::connect(ui.sliderCost,SIGNAL(valueChanged(int)),this,SLOT(slotSelectionValueChanged(int)));

    //ui.mainToolBar->addAction(QIcon(),"abc");

    //connect(this,SIGNAL(closeEvent(QCloseEvent* a)),this,SLOT(SaveConfig()));
    this->setFocusPolicy(Qt::StrongFocus);
    this->showMaximized();

    initMenuCommands();

    initZoom();

    ui.cellView->init(&map,&gs);

    ui.gfxCellSelection->setScene(&gsSelection);

    pathfinder.init(&map);
    pathviewer.init(&pathfinder,ui.cellView,&gs);

    LoadConfig();

    InitMap();

    //ui.centralWidget
    //this->installEventFilter(this);

    ui.cellView->setFocus(Qt::OtherFocusReason);




    if (0)
    {
        tmrRedraw.setInterval(TMR_INTERVAL);
        tmrRedraw.start();
        connect(&tmrRedraw,SIGNAL(timeout()),this,SLOT(RedrawSceneSlot()));
    };
}

void WndMain::LoadConfig()
{
    FILE *f = fopen("config.txt","rt");
    char szBuf[1024];
    if (f)
    {
        fscanf(f,"%s",szBuf); // FIXME: bad design
        strCurFile = QString::fromLatin1(szBuf);
        fclose(f);
    }
}

void WndMain::SaveConfig()
{
    if (! (strCurFile.isEmpty() || strCurFile.isNull()) )
    {
        FILE *f = fopen("config.txt","wt");
        if (f)
        {
            // store relative path in config
            QDir currentDir = QDir::current();
            QString strRelPath = currentDir.relativeFilePath(strCurFile);

            fprintf(f,"%s",strRelPath.toLatin1().data());
            fclose(f);
        }
    }     
}

int WndMain::InitMap()
{
    // if curFile is not empty - load it

    if ((strCurFile.isEmpty())||(strCurFile.isNull()))
    {
        // file is empty -> create map default
        map.allocate(mapWDefault,mapHDefault);
        ui.cellView->update(1);
    }
    else
    {
        map.loadFromFile(strCurFile.toLatin1().data());
        ui.cellView->update(1);
    }

    ui.sbZoom->setValue(ui.cellView->zoom()*100.0f);


    return 0;     
}

WndMain::~WndMain()
{

}

void WndMain::slotRedrawScene()
{
    pathviewer.clearPath();
    ui.cellView->repaint();
}

void WndMain::findPath(int flags)
{
    xfBeep(500,50);

    //XTimer xtimer;
    //xtimer.begin();

    pathfinder.find(flags); // 

    //xtimer.end();

    pathviewer.update();

    /*
    QString newTittle = QString("Total path cost = %1 in %2 mSec").
    arg(pathfinder.getPathLength()).arg(xtimer.getLastTime());
    this->setWindowTitle(newTittle);
    */

    xfBeep(800,50);
}


/*
void WndMain::focusOutEvent(QFocusEvent *fe)
{
//Beep(1000,1000);
//if (fe->lostFocus()) Beep(1000,1000);

};
*/

void WndMain::slotZoomNormal()
{
    ui.cellView->zoomReset();
    ui.sbZoom->setValue(ui.cellView->zoom()*100);
}

void WndMain::slotZoomIn()
{
    ui.cellView->zoomIn();
    ui.sbZoom->setValue(ui.cellView->zoom()*100);
}

void WndMain::slotZoomOut()
{
    ui.cellView->zoomOut();

    // update spin box control value
    ui.sbZoom->setValue(ui.cellView->zoom()*100);
}

void WndMain::slotZoomTo(double z)
{
    float fz = (float)(z)/100.0f;
    if (ui.cellView->zoom() != fz)
        ui.cellView->zoomTo(fz);
}

void WndMain::slotSelectionChanged(int six,int siy)
{
    //Beep(3000,50);
    if (ui.cellView->isValidCell(six,siy))
    {
        // activate group box
        ui.gbSelection->setEnabled(true);
        float fCost;
        if (XFAIL(map.getCost(six,siy,fCost))) return;

        if (xfBetweenExcl(fCost,0.01,1))
        {
            ui.sliderCost->setValue(fCost*100);
            ui.sliderCost->setEnabled(true);

            ui.lblCost->setText(QString("Land Cell: %1").arg(fCost));
            QBrush brCostDiff = QBrush(ui.cellView->getGradientCellColor(fCost));
            ui.gfxCellSelection->setBackgroundBrush(brCostDiff);
        }
        else
        {
            if (fCost == CELL_WALL)
            {
                ui.sliderCost->setEnabled(true);
                ui.sliderCost->setValue(100);
                ui.lblCost->setText(QString("Wall-E Cell"));
                ui.gfxCellSelection->setBackgroundBrush(BRUSH_WALL);
            }
            if (fCost < 0.01)
            {
                ui.sliderCost->setEnabled(true);
                ui.sliderCost->setValue(0);
                ui.lblCost->setText(QString("Clear Cell"));
                ui.gfxCellSelection->setBackgroundBrush(BRUSH_CLEAR);
            }
            if (fCost == CELL_DST)
            {
                ui.sliderCost->setEnabled(false);
                ui.lblCost->setText(QString("Dest Cell"));
                ui.gfxCellSelection->setBackgroundBrush(BRUSH_DST);
            }
            if (fCost == CELL_SRC)
            {
                ui.sliderCost->setEnabled(false);
                ui.lblCost->setText(QString("Source Cell"));
                ui.gfxCellSelection->setBackgroundBrush(BRUSH_SRC);
            }



        }


    }
    else
    {
        ui.gbSelection->setEnabled(false);
    }


}

void WndMain::slotSelectionValueChanged(int val)
{
    if (ui.cellView->hasSelection())
    {
        float fCost = (float)val / 100.0f;
        int six,siy;
        ui.cellView->selection(six,siy);
        ui.cellView->setCellValue(six,siy,fCost);
        ui.lblCost->setText(QString("Land Cell: %1").arg(fCost));
    }
}

void WndMain::slotLoadCostMapImage()
{
    QString retFn = QFileDialog::getOpenFileName(this,tr("Load Map Costs from Picture"),
        "",
        tr(     "All picture formats (*.bmp *.png *.jpg *.jpeg *.jpe);;" \
        "Windows bitmap (*.bmp);;"\
        "PNG (*.png);;"\
        "JPG (*.jpg,*.jpeg,*.jpe)"));
    if (retFn.isNull()||retFn.isEmpty()) return;

    ui.cellView->loadMapCostFromImage(retFn,0);
}

void WndMain::slotSaveCostMapImage()
{
    QString retFn = QFileDialog::getSaveFileName(this,tr("Save Map Costs to Picture"),
        "",
        tr(     "All picture formats (*.bmp *.png *.jpg *.jpeg *.jpe);;" \
        "Windows bitmap (*.bmp);;"\
        "PNG (*.png);;"\
        "JPG (*.jpg,*.jpeg,*.jpe)"));
    if (retFn.isNull()||retFn.isEmpty()) return;

    ui.cellView->saveMapCostToImage(retFn);
}

void WndMain::slotRepositionMap()
{
    DlgReposition dlgReposition(this,map.width(),map.height());

    if (dlgReposition.exec() == QDialog::Accepted)
    {
        int tx,ty;
        dlgReposition.getTXY(tx,ty);
        ui.cellView->translateMap(tx,ty,0.0f);
        xfBeep(100,100);
    }

}
