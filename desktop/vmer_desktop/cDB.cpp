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
    //qDebug() << db_file;
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
        //qDebug() << elements[i].item.text(0) << "==" << item->text(0);
        if(elements[i].item.text(0) == item->text(0)) {
            //qDebug() << elm->name;
            //elm = &(elements[i]);
            elm->image = elements[i].image;
            elm->desc = elements[i].desc;
            elm->name = elements[i].name;
            elm->id = elements[i].id;
            //qDebug() << elm->name;
            //qDebug() << "return true;";
            return true;
        }
    }
    */
    return false;
}
bool cDB::get_point(QTreeWidgetItem *item, cPoint *pnt) {
    /*
    for(int i=0; i<points.size(); i++) {
        //qDebug() << points[i].item.text(0) << "==" << item->text(0);
        if(points[i].item.text(0) == item->text(0)) {
            //qDebug() << pnt->name;
            //pnt = &(points[i]);
            pnt->config = points[i].config;
            pnt->desc = points[i].desc;
            pnt->name = points[i].name;
            pnt->id = points[i].id;
            //qDebug() << pnt->name;
            //qDebug() << "return true;";
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

    QString msg = QString("INSERT INTO point (\"element_id\",\"name\",\"config\",\"description\") VALUES (%1,%2,'%3',%4)").arg(
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
        //qDebug() << query.value(0);
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
        //qDebug() << "_modelWdgt[" << i << "] : " << (*_modelWdgt)[i]->text(0);
        for(int j=0; j<(*_modelWdgt)[i]->childCount(); j++) {
            //qDebug() << "-----> _modelWdgt[" << i << "].child[" << j << "] : " << (*_modelWdgt)[i]->child(j)->text(0);
        }
    }

    //qDebug() << "get_model_print_info_model, " << models.size();
    for(int i=0; i<models.size(); i++) {
        //qDebug() << "[" << i << "]";
        models[i].printInfo();
        //qDebug() << "==" << i << "==";
    }
    return true;
}

bool cDB::get_db(QList<QTreeWidgetItem*> *_dbWdgt) {
    query_db_table();
    query_company_table();
    query_plant_table();
    query_shop_table();
    query_route_table();
    query_machine_in_route_table();
    query_machine_table();

    link_db_table();
    link_company_table();
    link_plant_table();
    link_shop_table();
    link_route_table();
    link_machine_in_route_table();
    link_machine_table();

    _dbWdgt->clear();
    for(int i=0; i<dbTables.size(); i++) {
        QTreeWidgetItem *dbItm = dbTables[i].get_widget();
        //dbItm->setExpanded(true);
        for(int j=0; j<dbTables[i].companies.size(); j++) {
            QTreeWidgetItem *comWdgt = dbTables[i].companies[j]->get_widget();
            //comWdgt->setExpanded(true);
            for(int k=0; k<dbTables[i].companies[j]->plants.size(); k++) {
                QTreeWidgetItem *plntWdgt = dbTables[i].companies[j]->plants[k]->get_widget();
                //plntWdgt->setExpanded(true);
                for(int l=0; l<dbTables[i].companies[j]->plants[k]->shops.size(); l++) {
                    QTreeWidgetItem *shpWdgt = dbTables[i].companies[j]->plants[k]->shops[l]->get_widget();
                    //shpWdgt->setExpanded(true);
                    plntWdgt->addChild(shpWdgt);
                }
                comWdgt->addChild(plntWdgt);
            }
            dbItm->addChild(comWdgt);
        }
        _dbWdgt->push_back(dbItm);
    }
    return false;
}

bool cDB::get_element(QList<QTreeWidgetItem*> *_eleWdgt) {
    query_element_table();
    query_point_table();

    link_element_table();

    _eleWdgt->clear();
    for(int i=0; i<elements.size(); i++) {
        QTreeWidgetItem *eleItm = elements[i].get_widget();
        //qDebug() << "eleItm[" << i << "] : " << eleItm->text(0);
        for(int j=0; j<elements[i].points.size(); j++) {
            QTreeWidgetItem *pntItm = elements[i].points[j]->get_widget();
            //qDebug() << "pntItm[" << j << "] : " << pntItm->text(0);
            eleItm->addChild(pntItm);
        }
        _eleWdgt->push_back(eleItm);
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
                                                                           _desc);
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
                                                                                          _desc);
    qDebug().noquote() << msg;
    query.prepare(msg);
    query.exec();
    while(query.next()) {
        //qDebug() << query.value(0);
    }
    return false;
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
