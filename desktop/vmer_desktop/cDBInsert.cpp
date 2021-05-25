#include <QDir>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtDebug>
#include "cDB.h"

bool cDB::insert(const cElement *ele) {
    QSqlQuery query(db);
    QString desc = "NULL";
    if(ele->desc != "")
        desc = ele->desc;
    QString msg = QString("INSERT INTO `element` VALUES (%1,'%2','%3','%4',%5)").arg(QString::number(ele->id), ele->name, ele->std_image, ele->image, desc);
    //qDebug() << msg;
    query.prepare(msg);
    query.exec();
    while(query.next()) {
        //qDebug() << query.value(0);
    }
    return false;
}
bool cDB::insert(const cPoint *pnt) {
    QSqlQuery query(db);
    QString desc = "NULL";
    if(pnt->desc != "")
        desc = pnt->desc;

    QString msg = QString("INSERT INTO point (\"element_id\",\"name\",\"config\",\"description\") VALUES (%1,'%2','%3',%4)").arg(
                                                                               QString::number(pnt->element_id),
                                                                               pnt->name,
                                                                               pnt->config,
                                                                               desc);
//    QString msg = QString("INSERT INTO `point` VALUES (%1,%2,'%3','%4',%5)").arg(QString::number(pnt->id),
//                                                                               QString::number(pnt->element_id),
//                                                                               pnt->name,
//                                                                               pnt->config,
//                                                                               desc);
    qDebug().noquote() << msg;
    query.prepare(msg);
    query.exec();
    while(query.next()) {
        //qDebug() << query.value(0);
    }
    return false;
}

bool cDB::insert_model(const int _id, const QString _name, const QString _desc) {
    QSqlQuery query(db);
    QString desc = "NULL";
    if(_desc != "")
        desc = _desc;
    QString msg = QString("INSERT INTO `model` VALUES (%1,'%2','%3')").arg(QString::number(_id),
                                                                           _name,
                                                                           desc);
    qDebug().noquote() << msg;
    query.prepare(msg);
    query.exec();
    while(query.next()) {
        //qDebug() << query.value(0);
    }
    return false;
}
bool cDB::insert_model(const int _id, const QString _name, const QString _desc, const QList<int> &_element_id) {
    int chk=0;
    for(int i=0; i<elements.size(); i++) {
        for(int j=0; j<_element_id.size(); j++) {
            if(elements[i].id == _element_id[j])
                chk+=1;
        }
    }
    if(chk!=_element_id.size())
        return false;
    insert_model(_id, _name, _desc);
    int model_id=_id, idEleInModl=1;
    for(int i=0; i<element_in_models.size(); i++) {
        if(idEleInModl < element_in_models[i].id)
            idEleInModl = element_in_models[i].id;
    }
    idEleInModl+=1;
    for(int i=0; i<_element_id.size(); i++) {
        QString name = QString::number(model_id) + "_" + QString::number(_element_id[i]);
        insert_element_in_model(idEleInModl++, model_id, _element_id[i], name, "^_^");
    }
    return true;
}
bool cDB::insert_element_in_model(const int _id, const int _model_id, const int _element_id, const QString _name, const QString _desc) {
    QSqlQuery query(db);
    QString desc = "NULL";
    if(_desc != "")
        desc = _desc;
    QString msg = QString("INSERT INTO `element_in_model` VALUES (%1,%2,%3,'%4','%5')").arg(QString::number(_id),
                                                                                          QString::number(_model_id),
                                                                                          QString::number(_element_id),
                                                                                          _name,
                                                                                          desc);
    qDebug().noquote() << msg;
    query.prepare(msg);
    query.exec();
    while(query.next()) {
        //qDebug() << query.value(0);
    }
    return false;
}
bool cDB::insert_shop(const int _id, const int _plant_id, const QString _name, const QString _desc) {
    QSqlQuery query(db);
    QString desc = "NULL";
    if(_desc != "")
        desc = _desc;
    QString msg = QString("INSERT INTO `machine_shop` VALUES (%1,%2,'%3','%4')").arg(QString::number(_id),
                                                                                     QString::number(_plant_id),
                                                                                     _name,
                                                                                     desc);
    qDebug().noquote() << msg;
    query.prepare(msg);
    query.exec();
    while(query.next()) {
        //qDebug() << query.value(0);
    }
    return false;
}
bool cDB::insert_shop(const int _id, const int _plant_id, const QString _name, const QString _desc, const QList<int> &_model_id) {
    int chk=0;
    for(int i=0; i<models.size(); i++) {
        for(int j=0; j<_model_id.size(); j++) {
            if(models[i].id == _model_id[j])
                chk+=1;
        }
    }
    if(chk!=_model_id.size())
        return false;
    insert_shop(_id, _plant_id, _name, _desc);
    /*
    int shop_id=_id, idEleInModl=1;
    for(int i=0; i<element_in_models.size(); i++) {
        if(idEleInModl < element_in_models[i].id)
            idEleInModl = element_in_models[i].id;
    }
    idEleInModl+=1;
    for(int i=0; i<_element_id.size(); i++) {
        QString name = QString::number(model_id) + "_" + QString::number(_element_id[i]);
        insert_element_in_model(idEleInModl++, model_id, _element_id[i], name, "^_^");
    }
    */
    return true;
}
