#include "mainwindow.h"

#include <QApplication>
#include <cDB.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "A";
    cDB db;
    if (!db.init()) {
        return a.exec();
    }
    qDebug() << "B";
    if (!db.load_script_file("/home/probook/vmer/desktop/vpower/vpower/vmer_sqlite.sql")) {
        return a.exec();
    }
    qDebug() << "C";
    cRoute route;
    db.get_route(1, route);
    route.print();

    qDebug() << "D";
    MainWindow w;
    w.show();
    return a.exec();
}
