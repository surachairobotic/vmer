#include <QDir>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "cDB.h"


cDB::cDB() {
	db = QSqlDatabase::addDatabase("QSQLITE");
	QDir dir;
	if (!dir.exists("db"))
		dir.mkdir("db");
}

cDB::~cDB() {
	db.close();
}

bool cDB::init(const char* db_file) {
	db.setDatabaseName(db_file ? db_file : "db/db.sqlite3");
	if (!db.open()) {
		qWarning() << db.lastError();
		return false;
	}
	return true;
}

bool cDB::load_script_file(const char* script_file) {
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