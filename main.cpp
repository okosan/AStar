#include "stable.h"

#include "qmain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WndMain w;
    a.installEventFilter(&w);
    w.show();

    return a.exec();
}
