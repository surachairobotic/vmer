#pragma once
#include <QtCore>
#include <QtSql/QSqlDatabase>
#include "cRoute.h"

class cDB {
public:
	cDB();
	~cDB();

	bool init(const char* db_file = NULL);
	bool load_script_file(const char* script_file);
	bool get_route(const int route_id, cRoute &route);

private:
	QSqlDatabase db;
};