#include "cRoute.h"
#include <QObject>

cPoint::cPoint(const int _id, const int _element_id, const QString _name, const QString _config, const QString _desc)
    : id(_id), element_id(_element_id), name(_name), config(_config), desc(_desc)
{
    ;
}
cPoint::~cPoint() {

}
void cPoint::printInfo() {
    qDebug() << id << ", " << element_id << ", " << name << ", " << config << ", " << desc;
}
QTreeWidgetItem* cPoint::get_widget() {
    this->pntWdgt = new cPointWidget();
    this->pntWdgt->setText(0, name);
    this->pntWdgt->setWhatsThis(0, "Point");
    this->pntWdgt->cParent = this;
    this->pntWdgt->setFlags(this->pntWdgt->flags() | Qt::ItemIsEditable);
    return static_cast<QTreeWidgetItem*>(this->pntWdgt);
}
bool operator==(const cPoint& l, const cPoint& r) {
    if((l.id!=r.id) || (l.element_id!=r.element_id) || (l.name!=r.name) || (l.config!=r.config) || (l.desc!=r.desc))
        return false;
    return true;
}

cElement::cElement(const int _id, const QString _name, const QString _std_image, const QString _image, const QString _desc)
    : id(_id), name(_name), std_image(_std_image), image(_image), desc(_desc)
{
    ;
}
bool cElement::pushBackPoint(cPoint* pnt) {
    points.push_back(pnt);
    //this->elmWdgt->addChild(pnt->get_widget());
    return true;
}
void cElement::printInfo() {
    qDebug() << id << ", " << name << ", " << image << ", " << desc;
}
QTreeWidgetItem* cElement::get_widget() {
    this->elmWdgt = new cElementWidget();
    this->elmWdgt->setText(0, name);
    this->elmWdgt->setWhatsThis(0, "Element");
    this->elmWdgt->cParent = this;
    //for(int i=0; i<points.size(); i++)
    //    this->elmWdgt->addChild(points[i]->get_widget());
    return static_cast<QTreeWidgetItem*>(this->elmWdgt);
}
bool operator==(const cElement& l, const cElement& r) {
    if((l.id!=r.id) || (l.name!=r.name) || (l.image!=r.image) || (l.desc!=r.desc))
        return false;
    return true;
}

cDBTable::cDBTable(int _id, QString _name, QString _desc)
    : id(_id), name(_name), desc(_desc)
{
    ;
}
bool cDBTable::pushBackCompany(cCompany* comp) {
    companies.push_back(comp);
    //this->addChild(comp);
    return true;
}
QTreeWidgetItem* cDBTable::get_widget() {
    this->dbWdgt = new cDBTableWidget();
    this->dbWdgt->setText(0, name);
    this->dbWdgt->setWhatsThis(0, "DBTable");
    this->dbWdgt->cParent = this;
    return static_cast<QTreeWidgetItem*>(this->dbWdgt);
}


cCompany::cCompany(int _id, int _db_id, QString _name, QString _desc)
    : id(_id), db_id(_db_id), name(_name), desc(_desc)
{
    ;
}
bool cCompany::pushBackPlant(cPlant* plnt) {
    plants.push_back(plnt);
    //this->addChild(plnt);
    return true;
}
QTreeWidgetItem* cCompany::get_widget() {
    this->comWdgt = new cCompanyWidget();
    this->comWdgt->setText(0, name);
    this->comWdgt->setWhatsThis(0, "Company");
    this->comWdgt->cParent = this;
    return static_cast<QTreeWidgetItem*>(this->comWdgt);
}

cPlant::cPlant(int _id, int _company_id, QString _name, QString _desc)
    : id(_id), company_id(_company_id), name(_name), desc(_desc)
{
}
bool cPlant::pushBackShop(cShop* _shop) {
    shops.push_back(_shop);
    //this->addChild(shop);
    return true;
}
bool cPlant::pushBackRoute(cRoute* _route) {
    routes.push_back(_route);
    return true;
}
QTreeWidgetItem* cPlant::get_widget() {
    this->plntWdgt = new cPlantWidget();
    this->plntWdgt->setText(0, name);
    this->plntWdgt->setWhatsThis(0, "Plant");
    this->plntWdgt->cParent = this;
    return static_cast<QTreeWidgetItem*>(this->plntWdgt);
}

cShop::cShop(int _id, int _plant_id, QString _name, QString _desc)
    : id(_id), plant_id(_plant_id), name(_name), desc(_desc)
{
}
QTreeWidgetItem* cShop::get_widget() {
    this->shpWdgt = new cShopWidget();
    this->shpWdgt->setText(0, name);
    this->shpWdgt->setWhatsThis(0, "Shop");
    this->shpWdgt->cParent = this;
    return static_cast<QTreeWidgetItem*>(this->shpWdgt);
}
QTreeWidgetItem* cRoute::get_widget() {
    this->routeWdgt = new cRouteWidget();
    this->routeWdgt->setText(0, name);
    this->routeWdgt->setWhatsThis(0, "Route");
    this->routeWdgt->cParent = this;
    return static_cast<QTreeWidgetItem*>(this->routeWdgt);
}
cMachine::cMachine(const int _id, const int _model_id, const QString _name, const QString _serial, const QString _desc)
    : id(_id), model_id(_model_id), name(_name), serial_number(_serial), desc(_desc)
{
    this->setText(0, _name);
}

cRoute::cRoute(const int _id, const int _plant_id, const QString _name, const QString _desc)
    : id(_id), plant_id(_plant_id), name(_name), desc(_desc)
{
    this->setText(0, _name);
}
