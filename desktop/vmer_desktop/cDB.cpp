#include <QDir>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtDebug>
#include "cDB.h"


cDB::cDB() {
    db = QSqlDatabase::addDatabase("QSQLITE");
}

cDB::cDB(QString db_file) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    qDebug() << db_file;
    this->init(db_file.toStdString().c_str());
}

cDB::~cDB() {
	db.close();
}

bool cDB::init(const char *db_file) {
	db.setDatabaseName(db_file ? db_file : "db/db.sqlite3");
	if (!db.open()) {
		qWarning() << db.lastError();
		return false;
	}
	return true;
}

bool cDB::get_element(QTreeWidgetItem *item, cElement *elm) {
    /*
    for(int i=0; i<elements.size(); i++) {
        qDebug() << elements[i].item.text(0) << "==" << item->text(0);
        if(elements[i].item.text(0) == item->text(0)) {
            qDebug() << elm->name;
            //elm = &(elements[i]);
            elm->image = elements[i].image;
            elm->desc = elements[i].desc;
            elm->name = elements[i].name;
            elm->id = elements[i].id;
            qDebug() << elm->name;
            qDebug() << "return true;";
            return true;
        }
    }
    */
    return false;
}
bool cDB::get_point(QTreeWidgetItem *item, cPoint *pnt) {
    /*
    for(int i=0; i<points.size(); i++) {
        qDebug() << points[i].item.text(0) << "==" << item->text(0);
        if(points[i].item.text(0) == item->text(0)) {
            qDebug() << pnt->name;
            //pnt = &(points[i]);
            pnt->config = points[i].config;
            pnt->desc = points[i].desc;
            pnt->name = points[i].name;
            pnt->id = points[i].id;
            qDebug() << pnt->name;
            qDebug() << "return true;";
            return true;
        }
    }
    */
    return false;
}

bool cDB::insert(const cElement *ele) {
    QSqlQuery query(db);
    QString desc = "NULL";
    if(ele->desc != "")
        desc = ele->desc;
    QString msg = QString("INSERT INTO `element` VALUES (%1,'%2','%3',%4)").arg(QString::number(ele->id), ele->name, ele->image, desc);
    qDebug() << msg;
    query.prepare(msg);
    query.exec();
    while(query.next()) {
        qDebug() << query.value(0);
    }
    return false;
}
bool cDB::insert(const cPoint *pnt) {
    QSqlQuery query(db);
    QString desc = "NULL";
    if(pnt->desc != "")
        desc = pnt->desc;
    QString msg = QString("INSERT INTO `point` VALUES (%1,%2,'%3','%4',%5)").arg(QString::number(pnt->id),
                                                                               QString::number(pnt->element_id),
                                                                               pnt->name,
                                                                               pnt->config,
                                                                               desc);
    qDebug().noquote() << msg;
    query.prepare(msg);
    query.exec();
    while(query.next()) {
        qDebug() << query.value(0);
    }
    return false;
}

bool cDB::clear() {
    elements.clear();
    points.clear();
    /*
    dbTables.clear();
    companies.clear();
    plants.clear();
    shops.clear();
    routes.clear();
    models.clear();
    machines.clear();
    element_in_models.clear();
    machine_in_routes.clear();
    point_in_routes.clear();
    */

    elementsWdgt.clear();
    pointsWdgt.clear();
    return false;
}

void cDB::deleteDB(QString table, int id) {
    QSqlQuery query(db);

    QString msg = QString("DELETE FROM %1 WHERE id = %2").arg(table, QString::number(id));
    qDebug().noquote() << msg;
    query.prepare(msg);
    query.exec();
    while(query.next()) {
        qDebug() << query.value(0);
    }
}

bool cDB::get_model(QList<QTreeWidgetItem*> *_modelWdgt) {
    query_models_table();
    query_element_in_model_table();
    query_element_table();

    link_element_in_model_table();
    link_element_table();
    link_models_table();

    print_models_table();
    print_element_in_model_table();
    print_element_table();

    _modelWdgt->clear();
    for(int i=0; i<models.size(); i++) {
        QTreeWidgetItem* _mdlItem = models[i].get_widget();
        for(int j=0; j<models[i].elements.size(); j++) {
            _mdlItem->addChild(models[i].elements[j]->get_widget());
        }
        _modelWdgt->push_back(_mdlItem);
    }

    for(int i=0; i<_modelWdgt->size(); i++) {
        qDebug() << "_modelWdgt[" << i << "] : " << (*_modelWdgt)[i]->text(0);
        for(int j=0; j<(*_modelWdgt)[i]->childCount(); j++) {
            qDebug() << "-----> _modelWdgt[" << i << "].child[" << j << "] : " << (*_modelWdgt)[i]->child(j)->text(0);
        }
    }
    return true;
}
