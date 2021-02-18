#include <QDir>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtDebug>
#include "cDB.h"

void cDB::print_db_table() {
    qDebug() << "----- db_table -----";
    qDebug() << "id | name | description";
    for(int i=0; i<dbTables.size(); i++) {
        qDebug() << dbTables[i].id
                 << " | " << dbTables[i].name
                 << " | " << dbTables[i].desc;
        qDebug() << "+++  *company  +++";
        for(int j=0; j<dbTables[i].companies.size(); j++)
            qDebug() << dbTables[i].companies[j]->name;
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
        qDebug() << "+++  *points  +++";
        for(int j=0; j<elements[i].points.size(); j++)
            qDebug() << elements[i].points[j]->name;
        qDebug() << "+++  *point_in_routes  +++";
        for(int j=0; j<elements[i].point_in_routes.size(); j++) {
            int indx_route = elements[i].point_in_routes[j]->route_id;
            int indx_machine = elements[i].point_in_routes[j]->machine_id;
            int indx_point = elements[i].point_in_routes[j]->point_id;
            qDebug() << "route:" << routes[indx_route-1].name
                     << "|machine:" << machines[indx_machine-1].name
                     << "|point:" << points[indx_point-1].name;
        }
        qDebug() << "+++  *element_in_models  +++";
        for(int j=0; j<elements[i].element_in_models.size(); j++) {
            int indx_model = elements[i].element_in_models[j]->model_id;
            qDebug() << "model:" << models[indx_model-1].name;
        }
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
        qDebug() << "+++  *plants  +++";
        for(int j=0; j<companies[i].plants.size(); j++)
            qDebug() << companies[i].plants[j]->name;
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
        qDebug() << "+++  *machine  +++";
        for(int j=0; j<plants[i].shops.size(); j++)
            qDebug() << plants[i].shops[j]->name;
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
