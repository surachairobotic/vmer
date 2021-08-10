#pragma once
#include <QtCore>
#include <QtSql/QSqlDatabase>
#include <QDataWidgetMapper>
#include "cRoute.h"
#include "cModel.h"

#include<iostream>
using namespace std;

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
    bool query_all();

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
    bool link_all();

    bool get_db_table(QList<cDBTable> *dbTable);
    bool get_shop(QList<cShop> *shop, int plant_id);
    bool get_company(QList<cCompany> *company, int db_id);
    bool get_plant(QList<cPlant> *plant, int company_id);

    bool get_element(QTreeWidgetItem *item, cElement *elm);
    bool get_element(QList<QTreeWidgetItem*> *eleWdgt);
    bool get_point(QTreeWidgetItem *item, cPoint *pnt);
    bool get_model(QList<QTreeWidgetItem*> *modelWdgt);
    bool get_db(QList<QTreeWidgetItem*> *dbWdgt);
    bool get_routes(QList<QTreeWidgetItem*> *routeWdgt);
    bool get_plants(QList<QTreeWidgetItem*> *plntWdgt, bool _checkBox=false);

    bool insert(const cElement *ele);
    bool insert(const cPoint *pnt);
    bool insert(const cMachine *itm);
    bool insert(const cRoute *itm);
    bool insert_model(const int _id, const QString _name, const QString _desc);
    bool insert_model(const int _id, const QString _name, const QString _desc, const QList<int> &_element_id);
    bool insert_element_in_model(const int _id, const int _model_id, const int _element_id, const QString _name, const QString _desc);
    bool insert_shop(const int _id, const int _plant_id, const QString _name, const QString _desc);
    bool insert_shop(const int _id, const int _plant_id, const QString _name, const QString _desc, const QList<int> &_model_id);

    bool delete_data(const QString _table, const int _id);
    bool delete_point(const int _id);
    bool delete_element(const int _id);
    bool delete_element_in_model(const int _id);
    bool delete_model(const int _id);
    //bool delete(const cMachine *itm);

    bool update(QString tbName, int id, QString data);
    void checkBox(QTreeWidgetItem *wg, bool _chkBox);

    bool clear();

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

    QList<cElementWidget> elementsWdgt;
    QList<cPointWidget> pointsWdgt;
    QList<cModelWidget*> modelWdgt;

    QDataWidgetMapper *mapper;
    void testMapper();

    void deleteDB(QString table, int id);

    template<typename T>
    void pushDiffOnly(QList<T> *x, T *y, QList<int> *vIndx);

private:
    QSqlDatabase db;
};
