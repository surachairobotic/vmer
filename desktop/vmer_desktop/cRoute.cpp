#include "cRoute.h"

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
    return static_cast<QTreeWidgetItem*>(this->pntWdgt);
}

cElement::cElement(const int _id, const QString _name, const QString _image, const QString _desc)
    : id(_id), name(_name), image(_image), desc(_desc)
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
    for(int i=0; i<points.size(); i++)
        this->elmWdgt->addChild(points[i]->get_widget());
    return static_cast<QTreeWidgetItem*>(this->elmWdgt);
}

cDBTable::cDBTable(int _id, QString _name, QString _desc)
    : id(_id), name(_name), desc(_desc)
{
    this->setText(0, _name);
}
bool cDBTable::pushBackCompany(cCompany* comp) {
    companies.push_back(comp);
    this->addChild(comp);
    return true;
}


cCompany::cCompany(int _id, int _db_id, QString _name, QString _desc)
    : id(_id), db_id(_db_id), name(_name), desc(_desc)
{
    this->setText(0, _name);
}
bool cCompany::pushBackPlant(cPlant* plnt) {
    plants.push_back(plnt);
    this->addChild(plnt);
    return true;
}

cPlant::cPlant(int _id, int _company_id, QString _name, QString _desc)
    : id(_id), company_id(_company_id), name(_name), desc(_desc)
{
    this->setText(0, _name);
}
bool cPlant::pushBackShop(cShop* shop) {
    shops.push_back(shop);
    this->addChild(shop);
    return true;
}

cShop::cShop(int _id, int _plant_id, QString _name, QString _desc)
    : id(_id), plant_id(_plant_id), name(_name), desc(_desc)
{
    this->setText(0, _name);
}

cMachine::cMachine(const int _id, const int _model_id, const QString _name, const QString _serial, const QString _desc)
    : id(_id), model_id(_model_id), name(_name), serial_number(_serial), desc(_desc)
{
    this->setText(0, _name);
}

cRoute::cRoute(const int _id, const int _shop_id, const QString _name, const QString _desc)
    : id(_id), shop_id(_shop_id), name(_name), desc(_desc)
{
    this->setText(0, _name);
}
