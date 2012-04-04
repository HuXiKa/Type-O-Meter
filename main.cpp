#include <QtGui/QApplication>
#include "typeometerwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TypeOMeterWidget w;
    w.show();

    return a.exec();
}
