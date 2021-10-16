#include <QDir>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtDebug>
#include "cDB.h"

void cDB::deleteDB(QString table, int id) {
    QSqlQuery query(db);

    QString msg = QString("DELETE FROM %1 WHERE id = %2").arg(table, QString::number(id));
    qDebug().noquote() << msg;
    query.prepare(msg);
    query.exec();
    while(query.next()) {
        //qDebug() << query.value(0);
    }
}

bool cDB::delete_data(const QString _table, const int _id) {
    QSqlQuery query(db);
    QString msg = QString("DELETE FROM '%1' WHERE id=(%2)").arg(_table, QString::number(_id));
    qDebug().noquote() << msg;
    query.prepare(msg);
    query.exec();
    while(query.next()) {
        //qDebug() << query.value(0);
    }
    return false;
}
bool cDB::delete_point(const int _id) {
    this->delete_data("point", _id);
    return false;
}
bool cDB::delete_element(const int _id) {
    for(int i=0; i<points.size(); i++)
        if(points[i].element_id == _id)
            this->delete_point(points[i].id);
    this->delete_data("element", _id);
    return false;
}
bool cDB::delete_element_in_model(const int _id) {
    this->delete_data("element_in_model", _id);
    return false;
}
bool cDB::delete_model(const int _id) {
    for(int i=0; i<element_in_models.size(); i++)
        if(element_in_models[i].model_id == _id)
            this->delete_element_in_model(element_in_models[i].id);
    this->delete_data("model", _id);
    return false;
}

//bool cDB::delete(const cMachine *itm) {

//}
