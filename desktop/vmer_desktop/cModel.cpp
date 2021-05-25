#include "cModel.h"
#include "cRoute.h"

cModel::cModel(const int _id, const QString _name, const QString _desc)
    : id(_id), name(_name), desc(_desc)
{
    mdlWdg = nullptr;
}
cModel::cModel(const int _id, const QString _name, const QString _desc, QList<cElement*> _elements) {
    cModel(_id, _name, _desc);
    this->elements = _elements;
}
void cModel::printInfo() {
    qDebug() << id << ", " << name << ", " << desc;
    for(int i=0; i<elements.size(); i++) {
        qDebug() << ":: " << elements[i]->id << ", " << elements[i]->name;
    }
}
void cModel::printInfo(QString msg) {
    qDebug() << "msg: " << msg << ", " << id << ", " << name << ", " << desc;
    qDebug() << "address of this: " << this;
    for(int i=0; i<elements.size(); i++) {
        qDebug() << "address :: " << elements[i];
        qDebug() << ":: " << elements[i]->id << ", " << elements[i]->name;
    }
}
QTreeWidgetItem* cModel::get_widget() {
    this->mdlWdg = new cModelWidget();
    this->mdlWdg->setText(0, name);
    this->mdlWdg->setWhatsThis(0, "Model");
    this->mdlWdg->cParent = this;
    /*
    for(int i=0; i<element_in_models.size(); i++) {
        if(this->id == element_in_models[i]->model_id) {
            ;
            //element_in_models[i]->
            //this->mdlWdg->addChild(element_in_models[i]->get_widget());
        }
    }
    */
    return static_cast<QTreeWidgetItem*>(this->mdlWdg);
}

