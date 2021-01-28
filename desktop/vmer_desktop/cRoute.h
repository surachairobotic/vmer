#pragma once
#include <QtCore>
#include <QList>
#include <QTreeWidgetItem>

class cDB;

class cPointInRoute;
class cElementInModel;
class cCompany;
class cPlant;
class cShop;
class cRoute;
class cMachine;
class cMachineInRoute;

class cPoint {
public:
    cPoint(const int _id, const int _element_id, const QString _name, const QString _config, const QString _desc);
    ~cPoint();
    int id, element_id;
    QString name, config, desc;
    QList<cPointInRoute*> point_in_routes;
    QTreeWidgetItem item;
};

class cElement {
public:
    cElement(const int _id, const QString _name, const QString _image, const QString _desc);
    int id;
    QString name, image, desc;
    QList<cPoint*> points;
    QList<cPointInRoute*> point_in_routes;
    QList<cElementInModel*> element_in_models;
    QTreeWidgetItem item;
};

class cDBTable {
public:
    cDBTable(int _id, QString _name, QString _desc)
        : id(_id), name(_name), desc(_desc) {;}
    int id;
    QString name, desc;
    QList<cCompany*> companies;
};

class cCompany {
public:
    cCompany(int _id, int _db_id, QString _name, QString _desc)
        : id(_id), db_id(_db_id), name(_name), desc(_desc) {;}
    int id, db_id;
    QString name, desc;
    QList<cPlant*> plants;
};

class cPlant {
public:
    cPlant(int _id, int _company_id, QString _name, QString _desc)
        : id(_id), company_id(_company_id), name(_name), desc(_desc) {;}
    int id, company_id;
    QString name, desc;
    QList<cShop*> shops;
};

class cShop {
public:
    cShop(int _id, int _plant_id, QString _name, QString _desc)
        : id(_id), plant_id(_plant_id), name(_name), desc(_desc) {;}
    int id, plant_id;
    QString name, desc;
    QList<cRoute*> routes;
};

class cModel {
public:
    cModel(const int _id, const QString _name, const QString _desc)
        : id(_id), name(_name), desc(_desc) {}
    int id;
    QString name, desc;
    QList<cMachine*> machines;
    QList<cElementInModel*> element_in_models;
};

class cMachine {
public:
    cMachine(const int _id, const int _model_id, const QString _name, const QString _serial, const QString _desc)
        : id(_id), model_id(_model_id), name(_name), serial_number(_serial), desc(_desc) {}
    int id, model_id;
    QString name, serial_number, desc;
    QList<cMachineInRoute*> machine_in_routes;
    QList<cPointInRoute*> point_in_routes;
};

class cRoute {
public:
    cRoute(const int _id, const int _shop_id, const QString _name, const QString _desc)
        : id(_id), shop_id(_shop_id), name(_name), desc(_desc) {}
    void print() {
        qDebug() << "Route : " << name;
        /*
        for (int i = 0; i < machines.size(); i++) {
            qDebug() << "  Machine : " << machines[i]->name;
            for (int j = 0; j < machines[i].elements.size(); j++) {
                qDebug() << "    Element : " << machines[i].elements[j].name;
                for (int k = 0; k < machines[i].elements[j].points.size(); k++) {
                    qDebug() << "      Point : " << machines[i].elements[j].points[k].name
                        << "\n        " << machines[i].elements[j].points[k].config;
                }
            }
        }
        */
    }
    int id, shop_id;
    QString name, desc;
    QList<cMachineInRoute*> machine_in_routes;
    QList<cPointInRoute*> point_in_routes;
};

class cElementInModel {
public:
    cElementInModel(const int _id, const int _model_id, const int _element_id, const QString _name, const QString _desc)
        : id(_id), model_id(_model_id), element_id(_element_id), name(_name), desc(_desc) {}
    int id, model_id, element_id;
    QString name, desc;
};

class cMachineInRoute {
public:
    cMachineInRoute(const int _id, const int _route_id, const int _machine_id, const QString _desc)
        : id(_id), route_id(_route_id), machine_id(_machine_id), desc(_desc) {}
    int id, route_id, machine_id;
    QString desc;
};

class cPointInRoute {
public:
    cPointInRoute(const int _id, const int _route_id, const int _machine_id, const int _element_id, const int _point_id, const QString _desc)
        : id(_id), route_id(_route_id), machine_id(_machine_id), element_id(_element_id), point_id(_point_id), desc(_desc) {}
    int id, route_id, machine_id, element_id, point_id;
    QString desc;
};
