#pragma once
#include <QtCore>
#include <QtSql/QSqlDatabase>
#include "cRoute.h"

class cDB {
public:
	cDB();
    cDB(QString db_file);
    ~cDB();

    bool init(const char *db_file = NULL);
    bool load_script_file(const char *script_file);
	bool get_route(const int route_id, cRoute &route);

    bool query_db_table();
    bool query_element_table();
    bool query_point_table();
    bool query_company_table();
    bool query_plant_table();
    bool query_shop_table();
    bool query_route_table();
    bool query_models_table();
    bool query_machine_table();
    bool query_element_in_model_table();
    bool query_machine_in_route_table();
    bool query_point_in_route_table();

    void print_db_table();
    void print_element_table();
    void print_point_table();
    void print_company_table();
    void print_plant_table();
    void print_shop_table();
    void print_route_table();
    void print_models_table();
    void print_machine_table();
    void print_element_in_model_table();
    void print_machine_in_route_table();
    void print_point_in_route_table();

    bool link_db_table();
    bool link_element_table();
    bool link_point_table();
    bool link_company_table();
    bool link_plant_table();
    bool link_shop_table();
    bool link_route_table();
    bool link_models_table();
    bool link_machine_table();
    bool link_element_in_model_table();
    bool link_machine_in_route_table();
    bool link_point_in_route_table();

    bool get_db_table(QList<cDBTable> *dbTable);
    bool get_shop(QList<cShop> *shop, int plant_id);
    bool get_company(QList<cCompany> *company, int db_id);
    bool get_plant(QList<cPlant> *plant, int company_id);

    bool get_element(QTreeWidgetItem *item, cElement *elm);
    bool get_point(QTreeWidgetItem *item, cPoint *pnt);

    QList<cDBTable> dbTables;
    QList<cElement> elements;
    QList<cPoint> points;
    QList<cCompany> companies;
    QList<cPlant> plants;
    QList<cShop> shops;
    QList<cRoute> routes;
    QList<cModel> models;
    QList<cMachine> machines;
    QList<cElementInModel> element_in_models;
    QList<cMachineInRoute> machine_in_routes;
    QList<cPointInRoute> point_in_routes;

protected slots:
    void deletePoint(int id);

private:
    QSqlDatabase db;
};
