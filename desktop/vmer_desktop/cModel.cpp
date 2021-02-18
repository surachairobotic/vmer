#include "cModel.h"

cModel::cModel(const int _id, const QString _name, const QString _desc)
    : id(_id), name(_name), desc(_desc)
{
    ;
}
void cModel::printInfo() {
    qDebug() << id << ", " << name << ", " << desc;
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
