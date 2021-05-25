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

class cPoint;
class cPointWidget : public QTreeWidgetItem {
public:
    //cPointWidget() {
        //QObject::connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
        //             SLOT(name_change(QTreeWidgetItem*, int)));
        //QObject::connect((QTreeWidgetItem*)this, &QTreeWidgetItem::itemChanged, this, [=](int){});//, &cPointWidget::name_change);
    //}
    cPoint *cParent;
private slots:
    void name_change(QTreeWidgetItem* itm, int col);
};

class cPoint {
public:
    cPoint(const int _id, const int _element_id, const QString _name, const QString _config, const QString _desc);
    ~cPoint();
    void printInfo();
    friend bool operator==(const cPoint& l, const cPoint& r);
    QTreeWidgetItem* get_widget();
    int id, element_id;
    QString name, config, desc;
    QList<cPointInRoute*> point_in_routes;
    cPointWidget *pntWdgt;
};

class cElement;
class cElementWidget : public QTreeWidgetItem {
public:
        cElement *cParent;
};
class cElement {
public:
    cElement(const int _id, const QString _name, const QString _std_image, const QString _image, const QString _desc);
    bool pushBackPoint(cPoint* pnt);
    void printInfo();
    //override void clear();
    friend bool operator==(const cElement& l, const cElement& r);
    QTreeWidgetItem* get_widget();
    int id;
    QString name, std_image, image, desc;
    QList<cPoint*> points;
    QList<cPointInRoute*> point_in_routes;
    QList<cElementInModel*> element_in_models;
    cElementWidget *elmWdgt;
};

class cDBTable;
class cDBTableWidget : public QTreeWidgetItem {
public:
        cDBTable *cParent;
};
class cDBTable {
public:
    cDBTable(int _id, QString _name, QString _desc);
    bool pushBackCompany(cCompany* comp);
    QTreeWidgetItem* get_widget();
    int id;
    QString name, desc;
    QList<cCompany*> companies;
    cDBTableWidget *dbWdgt;
};

class cCompany;
class cCompanyWidget : public QTreeWidgetItem {
public:
        cCompany *cParent;
};
class cCompany {
public:
    cCompany(int _id, int _db_id, QString _name, QString _desc);
    bool pushBackPlant(cPlant* plnt);
    QTreeWidgetItem* get_widget();
    int id, db_id;
    QString name, desc;
    QList<cPlant*> plants;
    cCompanyWidget *comWdgt;
};

/*
class cCompany;
class cCompanyWidget : public QTreeWidgetItem {
    cCompany *company;
}
*/

class cPlant;
class cPlantWidget : public QTreeWidgetItem {
public:
        cPlant *cParent;
};
class cPlant {
public:
    cPlant(int _id, int _company_id, QString _name, QString _desc);
    bool pushBackShop(cShop* _shop);
    bool pushBackRoute(cRoute* _route);
    QTreeWidgetItem* get_widget();
    int id, company_id;
    QString name, desc;
    QList<cShop*> shops;
    QList<cRoute*> routes;
    cPlantWidget *plntWdgt;
};

class cShop;
class cShopWidget : public QTreeWidgetItem {
public:
    cShop *cParent;
};
class cShop {
public:
    cShop(int _id, int _plant_id, QString _name, QString _desc);
    QTreeWidgetItem* get_widget();
    int id, plant_id;
    QString name, desc;
    cShopWidget *shpWdgt;
};

class cMachine : public QTreeWidgetItem {
public:
    cMachine(const int _id, const int _model_id, const QString _name, const QString _serial, const QString _desc);
    int id, model_id;
    QString name, serial_number, desc;
    QList<cMachineInRoute*> machine_in_routes;
    QList<cPointInRoute*> point_in_routes;
};

class cRoute;
class cRouteWidget : public QTreeWidgetItem {
public:
    cRoute *cParent;
};

class cRoute : public QTreeWidgetItem {
public:
    cRoute(const int _id, const int _plant_id, const QString _name, const QString _desc);
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
    QTreeWidgetItem* get_widget();
    int id, plant_id;
    QString name, desc;
    QList<cMachineInRoute*> machine_in_routes;
    QList<cPointInRoute*> point_in_routes;
    cRouteWidget *routeWdgt;
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
