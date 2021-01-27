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
    bool chk=false;
    while(query.next()) {
        if(!chk)
            elements.clear();
        chk=true;
        elements.push_back(cElement(query.value(0).toInt(), query.value(1).toString(),
                                    query.value(2).toString(), query.value(3).toString()));
    }
    return chk;
}
bool cDB::query_point_table() {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM point");
    query.exec();
    bool chk=false;
    while(query.next()) {
        if(!chk)
            points.clear();
        chk=true;
        points.push_back(cPoint(query.value(0).toInt(), query.value(1).toInt(),
                                query.value(2).toString(), query.value(3).toString(),
                                query.value(4).toString()));
    }
    return chk;
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

void cDB::print_db_table() {
    qDebug() << "----- db_table -----";
    qDebug() << "id | name | description";
    for(int i=0; i<dbTables.size(); i++) {
        qDebug() << dbTables[i].id
                 << " | " << dbTables[i].name
                 << " | " << dbTables[i].desc;
    }
}
void cDB::print_element_table() {
    qDebug() << "----- element_table -----";
    qDebug() << "id | name | image | description";
    for(int i=0; i<elements.size(); i++) {
        qDebug() << elements[i].id
                 << " | " << elements[i].name
                 << " | " << elements[i].image
                 << " | " << elements[i].desc;
    }
}
void cDB::print_point_table() {
    qDebug() << "----- point_table -----";
    qDebug() << "id | element_id | name | config | description";
    for(int i=0; i<points.size(); i++) {
        qDebug() << points[i].id
                 << " | " << points[i].element_id
                 << " | " << points[i].name
                 << " | " << points[i].config
                 << " | " << points[i].desc;
    }
}
void cDB::print_company_table() {
    qDebug() << "----- company_table -----";
    qDebug() << "id | db_id | name | description";
    for(int i=0; i<companies.size(); i++) {
        qDebug() << companies[i].id
                 << " | " << companies[i].db_id
                 << " | " << companies[i].name
                 << " | " << companies[i].desc;
    }
}
void cDB::print_plant_table() {
    qDebug() << "----- plant_table -----";
    qDebug() << "id | company_id | name | description";
    for(int i=0; i<plants.size(); i++) {
        qDebug() << plants[i].id
                 << " | " << plants[i].company_id
                 << " | " << plants[i].name
                 << " | " << plants[i].desc;
    }
}
void cDB::print_shop_table() {
    qDebug() << "----- machine_shop_table -----";
    qDebug() << "id | plant_id | name | description";
    for(int i=0; i<shops.size(); i++) {
        qDebug() << shops[i].id
                 << " | " << shops[i].plant_id
                 << " | " << shops[i].name
                 << " | " << shops[i].desc;
    }
}
void cDB::print_route_table() {
    qDebug() << "----- route_table -----";
    qDebug() << "id | shop_id | name | description";
    for(int i=0; i<routes.size(); i++) {
        qDebug() << routes[i].id
                 << " | " << routes[i].shop_id
                 << " | " << routes[i].name
                 << " | " << routes[i].desc;
    }
}
void cDB::print_models_table() {
    qDebug() << "----- model_table -----";
    qDebug() << "id | name | description";
    for(int i=0; i<models.size(); i++) {
        qDebug() << models[i].id
                 << " | " << models[i].name
                 << " | " << models[i].desc;
    }
}
void cDB::print_machine_table() {
    qDebug() << "----- machine_table -----";
    qDebug() << "id | model_id | name | serial_number | description";
    for(int i=0; i<machines.size(); i++) {
        qDebug() << machines[i].id
                 << " | " << machines[i].model_id
                 << " | " << machines[i].name
                 << " | " << machines[i].serial_number
                 << " | " << machines[i].desc;
    }
}
void cDB::print_element_in_model_table() {
    qDebug() << "----- element_in_model_table -----";
    qDebug() << "id | model_id | element_id | name | description";
    for(int i=0; i<element_in_models.size(); i++) {
        qDebug() << element_in_models[i].id
                 << " | " << element_in_models[i].model_id
                 << " | " << element_in_models[i].element_id
                 << " | " << element_in_models[i].name
                 << " | " << element_in_models[i].desc;
    }
}
void cDB::print_machine_in_route_table() {
    qDebug() << "----- machine_in_route_table -----";
    qDebug() << "id | route_id | machine_id | description";
    for(int i=0; i<machine_in_routes.size(); i++) {
        qDebug() << machine_in_routes[i].id
                 << " | " << machine_in_routes[i].route_id
                 << " | " << machine_in_routes[i].machine_id
                 << " | " << machine_in_routes[i].desc;
    }
}
void cDB::print_point_in_route_table() {
    qDebug() << "----- point_in_route_table -----";
    qDebug() << "id | route_id | machine_id | element_id | point_id | description";
    for(int i=0; i<point_in_routes.size(); i++) {
        qDebug() << point_in_routes[i].id
                 << " | " << point_in_routes[i].route_id
                 << " | " << point_in_routes[i].machine_id
                 << " | " << point_in_routes[i].element_id
                 << " | " << point_in_routes[i].point_id
                 << " | " << point_in_routes[i].desc;
    }
}

bool cDB::link_db_table() {
    if(companies.size()<1)
        return false;
    for(int i=0; i<companies.size(); i++) {
        int indx_id = companies[i].db_id;
        dbTables[indx_id-1].companies.push_back(&companies[i]);
    }
    return true;
}
bool cDB::link_element_table() {
    if(points.size()<1)
        return false;
    for(int i=0; i<points.size(); i++) {
        int indx_id = points[i].element_id;
        elements[indx_id-1].points.push_back(&points[i]);
    }
    if(point_in_routes.size()<1)
        return false;
    for(int i=0; i<point_in_routes.size(); i++) {
        int indx_id = point_in_routes[i].element_id;
        elements[indx_id-1].point_in_routes.push_back(&point_in_routes[i]);
    }
    if(element_in_models.size()<1)
        return false;
    for(int i=0; i<element_in_models.size(); i++) {
        int indx_id = element_in_models[i].element_id;
        elements[indx_id-1].element_in_models.push_back(&element_in_models[i]);
    }
    return true;
}
bool cDB::link_point_table() {
    if(point_in_routes.size()<1)
        return false;
    for(int i=0; i<point_in_routes.size(); i++) {
        int indx_id = point_in_routes[i].point_id;
        points[indx_id-1].point_in_routes.push_back(&point_in_routes[i]);
    }
    return true;
}
bool cDB::link_company_table() {
    if(plants.size()<1)
        return false;
    for(int i=0; i<plants.size(); i++) {
        int indx_id = plants[i].company_id;
        companies[indx_id-1].plants.push_back(&plants[i]);
    }
    return true;
}
bool cDB::link_plant_table() {
    if(shops.size()<1)
        return false;
    for(int i=0; i<shops.size(); i++) {
        int indx_id = shops[i].plant_id;
        plants[indx_id-1].shops.push_back(&shops[i]);
    }
    return true;
}
bool cDB::link_shop_table() {
    if(routes.size()<1)
        return false;
    for(int i=0; i<routes.size(); i++) {
        int indx_id = routes[i].shop_id;
        shops[indx_id-1].routes.push_back(&routes[i]);
    }
    return true;
}
bool cDB::link_route_table() {
    if(machine_in_routes.size()<1)
        return false;
    for(int i=0; i<machine_in_routes.size(); i++) {
        int indx_id = machine_in_routes[i].route_id;
        routes[indx_id-1].machine_in_routes.push_back(&machine_in_routes[i]);
    }
    if(point_in_routes.size()<1)
        return false;
    for(int i=0; i<point_in_routes.size(); i++) {
        int indx_id = point_in_routes[i].route_id;
        routes[indx_id-1].point_in_routes.push_back(&point_in_routes[i]);
    }
    return true;
}
bool cDB::link_models_table() {
    if(machines.size()<1)
        return false;
    for(int i=0; i<machines.size(); i++) {
        int indx_id = machines[i].model_id;
        models[indx_id-1].machines.push_back(&machines[i]);
    }
    if(element_in_models.size()<1)
        return false;
    for(int i=0; i<element_in_models.size(); i++) {
        int indx_id = element_in_models[i].model_id;
        models[indx_id-1].element_in_models.push_back(&element_in_models[i]);
    }
    return true;
}
bool cDB::link_machine_table() {
    if(machine_in_routes.size()<1)
        return false;
    for(int i=0; i<machine_in_routes.size(); i++) {
        int indx_id = machine_in_routes[i].machine_id;
        machines[indx_id-1].machine_in_routes.push_back(&machine_in_routes[i]);
    }
    if(point_in_routes.size()<1)
        return false;
    for(int i=0; i<point_in_routes.size(); i++) {
        int indx_id = point_in_routes[i].machine_id;
        machines[indx_id-1].point_in_routes.push_back(&point_in_routes[i]);
    }
    return true;
}
bool cDB::link_element_in_model_table() {
    return true;
}
bool cDB::link_machine_in_route_table() {
    return true;
}
bool cDB::link_point_in_route_table() {
    return true;
}
