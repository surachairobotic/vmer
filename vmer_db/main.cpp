#include <QtCore/QCoreApplication>
#include <QtSql/QSqlError>
#include "cDB.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cDB db;
    if (!db.init()) {
        return a.exec();
    }
    if (!db.load_script_file("Z:\\git\\visualstudio\\vmer\\doc\\vmer_sqlite.sql")) {
        return a.exec();
    }
    cRoute route;
    db.get_route(1, route);
    route.print();

    printf("end ...\n");
    return a.exec();
}
