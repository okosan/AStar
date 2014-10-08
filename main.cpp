#include "stable.h"

#include "qmain.h"

//Q_IMPORT_PLUGIN(qjpeg)
//Q_IMPORT_PLUGIN(qtiff)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setStyle(new QPlastiqueStyle);// "plastique");
    WndMain w;
    a.installEventFilter(&w);
    w.show();

    return a.exec();
}
