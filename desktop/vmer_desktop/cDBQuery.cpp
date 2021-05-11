#include <QDir>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtDebug>
#include "cDB.h"

bool cDB::load_script_file(const char *script_file) {
    QFile file(script_file);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << file.errorString();
        return false;
    }
    QSqlQuery query(db);

    // https://python5.com/q/mfyjygok
    QStringList queries = QTextStream(&file).readAll().split(';');
    foreach(QString txt, queries) {
        if (txt.trimmed().isEmpty()) {
            continue;
        }
        //qDebug() << txt;
        if (!query.exec(txt))
        {
            qWarning() << query.lastError();
            return false;
        }
        query.finish();
    }
    return true;
}

/*
bool cDB::get_route(const int route_id, cRoute& route) {
    QSqlQuery query(db);
    query.prepare("SELECT name FROM route WHERE id=?");
    query.addBindValue(route_id);
    query.exec();
    if (!query.next()) {
        qWarning() << "Invalid route ID";
        return false;
    }
    route.name = query.value(0).toString();
    route.machines.clear();

    query.prepare("SELECT machine.name AS machine_name \
      , element_in_model.name AS element_name \
      , point.name AS point_name \
      , point_in_route.config AS config \
    FROM route,machine_in_route,machine,model,element_in_model,element,point,point_in_route \
    WHERE route.id=? AND \
      machine_in_route.route_id=route.id AND \
      machine.id=machine_in_route.machine_id AND \
      model.id=machine.model_id AND \
      element_in_model.model_id=model.id AND \
      element.id=element_in_model.element_id AND \
      point.element_id=element.id AND \
      point_in_route.point_id=point.id AND \
      point_in_route.element_id=element.id AND \
      point_in_route.machine_id=machine.id AND \
      point_in_route.route_id=route.id \
    ORDER BY route.id ASC,machine.id ASC,element.id,point.id");
    query.addBindValue(route_id);
    query.exec();
    while (query.next()) {
        const QString machine_name = query.value("machine_name").toString()
            , element_name = query.value("element_name").toString()
            , point_name = query.value("point_name").toString();
        cMachine* machine = NULL;
        cElement* element = NULL;
        for(int i=0;i<route.machines.size();i++){
            if (route.machines[i].name == machine_name) {
                machine = &route.machines[i];
                break;
            }
        }
        if (!machine) {
            route.machines.push_back(cMachine(machine_name));
            machine = &route.machines[route.machines.size() - 1];
        }
        for (int i = 0; i < machine->elements.size(); i++) {
            if (machine->elements[i].name == element_name) {
                element = &machine->elements[i];
                break;
            }
        }
        if (!element) {
            machine->elements.push_back(cElement(element_name));
            element = &machine->elements[machine->elements.size() - 1];
        }

        element->points.push_back(cPoint(
            query.value("point_name").toString(),
            query.value("config").toString()
        ));
    }
    return true;
}
*/

bool cDB::get_db_table(QList<cDBTable> *dbTable) {
    dbTable->clear();
    QSqlQuery query(db);
    query.prepare("SELECT * FROM db");
    //query.addBindValue(route_id);
    query.exec();
    bool chk=false;
    while(query.next()) {
        chk=true;
        qDebug() << query.value(0).toString() << ", " << query.value(1).toString() << ", " << query.value(2).toString();
        dbTable->push_back(cDBTable(query.value(0).toInt(), query.value(1).toString(),
                                    query.value(2).toString()));
    }
    return chk;
}
bool cDB::get_company(QList<cCompany> *company, int db_id) {
    company->clear();
    QSqlQuery query(db);
    query.prepare("SELECT * FROM company WHERE db_id=?");
    query.addBindValue(db_id);
    query.exec();
    bool chk=false;
    while(query.next()) {
        chk=true;
        qDebug() << query.value(0).toString() << ", " << query.value(1).toString() << ", " << query.value(2).toString();
        company->push_back(cCompany(query.value(0).toInt(), query.value(1).toInt(),
                                    query.value(2).toString(), query.value(3).toString()));
    }
    return chk;
}
bool cDB::get_plant(QList<cPlant> *plant, int company_id) {
    plant->clear();
    QSqlQuery query(db);
    query.prepare("SELECT * FROM plant WHERE company_id=?");
    query.addBindValue(company_id);
    query.exec();
    bool chk=false;
    while(query.next()) {
        chk=true;
        qDebug() << query.value(0).toString() << ", " << query.value(1).toString() << ", " << query.value(2).toString();
        plant->push_back(cPlant(query.value(0).toInt(), query.value(1).toInt(),
                                query.value(2).toString(), query.value(3).toString()));
    }
    return chk;
}
bool cDB::get_shop(QList<cShop> *shop, int plant_id) {
    shop->clear();
    QSqlQuery query(db);
    query.prepare("SELECT * FROM machine_shop WHERE plant_id=?");
    query.addBindValue(plant_id);
    query.exec();
    bool chk=false;
    while(query.next()) {
        chk=true;
        qDebug() << query.value(0).toString() << ", " << query.value(1).toString() << ", " << query.value(2).toString();
        shop->push_back(cShop(query.value(0).toInt(), query.value(1).toInt(),
                              query.value(2).toString(), query.value(3).toString()));
    }
    return chk;
}

bool cDB::query_all() {
    this->query_element_table();
    this->query_point_table();
    /*
    this->query_db_table();
    this->query_company_table();
    this->query_plant_table();
    this->query_shop_table();
    this->query_route_table();
    this->query_models_table();
    this->query_machine_table();
    this->query_element_in_model_table();
    this->query_machine_in_route_table();
    this->query_point_in_route_table();
    */
    return false;
}

bool cDB::query_db_table() {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM db");
    query.exec();
    bool chk=false;
    while(query.next()) {
        if(!chk)
            dbTables.clear();
        chk=true;
        dbTables.push_back(cDBTable(query.value(0).toInt(), query.value(1).toString(),
                                    query.value(2).toString()));
    }
    return chk;
}
bool cDB::query_element_table() {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM element");
    query.exec();
    QList<int> indx;
    for(int i=0; i<elements.size(); i++)
        indx.push_back(i);
    while(query.next()) {
        cElement data(query.value(0).toInt(), query.value(1).toString(),
                      query.value(2).toString(), query.value(3).toString(),
                      query.value(4).toString());
        bool chk=false;
        for(int i=0; indx.size(); i++) {
            if(elements[indx[i]]==data) {
                indx.removeAt(i);
                chk=true;
                break;
            }
        }
        if(!chk) {
            elements.push_back(data);
        }
    }
    for(int i=indx.size()-1; i>=0; i--) {
        elements.removeAt(indx[i]);
    }
    return false;
}
bool cDB::query_point_table() {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM point");
    query.exec();
    QList<int> indx;
    for(int i=0; i<points.size(); i++)
        indx.push_back(i);
    while(query.next()) {
        cPoint data(query.value(0).toInt(), query.value(1).toInt(),
                     query.value(2).toString(), query.value(3).toString(),
                     query.value(4).toString());
        bool chk=false;
        for(int i=0; indx.size(); i++) {
            if(points[indx[i]]==data) {
                indx.removeAt(i);
                chk=true;
                break;
            }
        }
        if(!chk) {
            points.push_back(data);
        }
    }
    for(int i=indx.size()-1; i>=0; i--) {
        points.removeAt(indx[i]);
    }
    return false;
}
bool cDB::query_company_table() {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM company");
    query.exec();
    bool chk=false;
    while(query.next()) {
        if(!chk)
            companies.clear();
        chk=true;
        companies.push_back(cCompany(query.value(0).toInt(), query.value(1).toInt(),
                                     query.value(2).toString(), query.value(3).toString()));
    }
    return chk;
}
bool cDB::query_plant_table() {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM plant");
    query.exec();
    bool chk=false;
    while(query.next()) {
        if(!chk)
            plants.clear();
        chk=true;
        plants.push_back(cPlant(query.value(0).toInt(), query.value(1).toInt(),
                                query.value(2).toString(), query.value(3).toString()));
    }
    return chk;
}
bool cDB::query_shop_table() {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM machine_shop");
    query.exec();
    bool chk=false;
    while(query.next()) {
        if(!chk)
            shops.clear();
        chk=true;
        shops.push_back(cShop(query.value(0).toInt(), query.value(1).toInt(),
                              query.value(2).toString(), query.value(3).toString()));
    }
    return chk;
}
bool cDB::query_route_table() {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM route");
    query.exec();
    bool chk=false;
    while(query.next()) {
        if(!chk)
            routes.clear();
        chk=true;
        routes.push_back(cRoute(query.value(0).toInt(), query.value(1).toInt(),
                                query.value(2).toString(), query.value(3).toString()));
    }
    return chk;
}
bool cDB::query_models_table() {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM model");
    query.exec();
    bool chk=false;
    while(query.next()) {
        if(!chk)
            models.clear();
        chk=true;
        models.push_back(cModel(query.value(0).toInt(), query.value(1).toString(),
                                query.value(2).toString()));
    }
    return chk;
}
bool cDB::query_machine_table() {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM machine");
    query.exec();
    bool chk=false;
    while(query.next()) {
        if(!chk)
            machines.clear();
        chk=true;
        machines.push_back(cMachine(query.value(0).toInt(), query.value(1).toInt(),
                                    query.value(2).toString(), query.value(3).toString(),
                                    query.value(4).toString()));
    }
    return chk;
}
bool cDB::query_element_in_model_table() {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM element_in_model");
    query.exec();
    bool chk=false;
    while(query.next()) {
        if(!chk)
            element_in_models.clear();
        chk=true;
        element_in_models.push_back(cElementInModel(query.value(0).toInt(), query.value(1).toInt(),
                                                    query.value(2).toInt(), query.value(3).toString(),
                                                    query.value(4).toString()));
    }
    return chk;
}
bool cDB::query_machine_in_route_table() {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM machine_in_route");
    query.exec();
    bool chk=false;
    while(query.next()) {
        if(!chk)
            machine_in_routes.clear();
        chk=true;
        machine_in_routes.push_back(cMachineInRoute(query.value(0).toInt(), query.value(1).toInt(),
                                                    query.value(2).toInt(), query.value(3).toString()));
    }
    return chk;
}
bool cDB::query_point_in_route_table() {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM point_in_route");
    query.exec();
    bool chk=false;
    while(query.next()) {
        if(!chk)
            point_in_routes.clear();
        chk=true;
        point_in_routes.push_back(cPointInRoute(query.value(0).toInt(), query.value(1).toInt(),
                                                query.value(2).toInt(), query.value(3).toInt(),
                                                query.value(4).toInt(), query.value(5).toString()));
    }
    return chk;
}
