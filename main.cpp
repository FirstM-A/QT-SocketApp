#include "widget.h"
#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    Client p;
    p.show();

    return a.exec();
}
