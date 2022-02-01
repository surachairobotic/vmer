#include <QDir>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtDebug>
#include "cDB.h"

bool cDB::update(QString tbName, int id, QString data) {
    QSqlQuery query(db);
    QString msg = QString("UPDATE '%1' SET name='%2' WHERE id=%3").arg(tbName,
                                                                       data,
                                                                       QString::number(id));
    qDebug().noquote() << msg;
    query.prepare(msg);
    query.exec();
    while(query.next()) {
        //qDebug() << query.value(0);
    }
    return false;
}
