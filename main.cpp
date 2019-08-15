#include "widget.h"
#include <QApplication>
#include "ip-fetcher.h"

char addr[128];
int main(int argc, char *argv[])
{
    fetchIp( addr, sizeof(addr) );
    //qDebug("%s\n", addr);

    QApplication a(argc, argv);

    Widget w;

    w.show();
    return a.exec();
}
