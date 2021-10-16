#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QFileDialog"
#include "QDebug"
#include "cDB.h"
#include "QSize"
#include "QTreeWidget"
#include "QTreeWidgetItem"
#include "QPushButton"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "QIcon"
#include <QRandomGenerator>
#include "commonFunction.h"

bool MainWindow::displayDBTree()
{
    qDebug() << "displayDBTree()";
    hasDB = false;
    ui->treeWidgetDB->clear();
    //QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetDB);
    //root->setExpanded(true);
    db->get_db(&dbWdgt);
    db->get_db(&dbWdgt);
    ui->treeWidgetDB->addTopLevelItem(dbWdgt[0]);
    dbWdgt[0]->setExpanded(true);
    for(int i=0; i<dbWdgt.size(); i++)
        commonFunction::setAllChildExpanded(dbWdgt[i]);
    hasDB = true;

    /*
    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetDB);
    root->setWhatsThis(0, "Root");
    root->setText(0, "Databases");
    root->setExpanded(true);
    ui->treeWidgetDB->addTopLevelItem(root);

    db->get_db(&dbWdgt);

    for(int i=0; i<dbWdgt.size(); i++) {
        //dbWdgt[i]->setExpanded(true);
        root->addChild(dbWdgt[i]);
    }
    hasDB = true;
    */

    /*
    QList<cDBTable> *db_table = new QList<cDBTable>();
    qDebug() << "1. db_table->size() : " << db_table->size();
    if(!db->get_db_table(db_table))
        return false;
    qDebug() << "2. db_table->size() : " << db_table->size();

    if(db_table->size() != 1)
        return false;

    //QTreeWidgetItem *root = static_cast<QTreeWidgetItem*>(&(*db_table)[0]);
    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetDB);
    root->setText(0, (*db_table)[0].name);
    ui->treeWidgetDB->addTopLevelItem(root);
    //ui->treeWidgetDB->addTopLevelItem(root);
    //root->addChild(static_cast<QTreeWidgetItem*>(&(*db_table)[0]));

    for(int i=0; i<db->companies.size(); i++) {
        root->addChild(static_cast<QTreeWidgetItem*>(&db->companies[i]));
    }
    for(int i=0; i<db_table->size(); i++) {
        QList<cCompany> *company = new QList<cCompany>();
        if(!db->get_company(company, (*db_table)[i].id))
            continue;
        for(int j=0; j<company->size(); j++) {
            QTreeWidgetItem *childCompany = new QTreeWidgetItem();
            childCompany->setText(0, (*company)[j].name);
            root->addChild(static_cast<QTreeWidgetItem*>(&childCompany);
            QList<cPlant> *plant = new QList<cPlant>();
            if(!db->get_plant(plant, (*company)[j].id))
                continue;
            for(int k=0; k<plant->size(); k++) {
                QTreeWidgetItem *childPlant = new QTreeWidgetItem();
                childPlant->setText(0, (*plant)[k].name);
                childCompany->addChild(static_cast<QTreeWidgetItem*>(&childPlant);
                QList<cShop> *shop = new QList<cShop>();
                if(!db->get_shop(shop, (*plant)[j].id))
                    continue;
                for(int k=0; k<shop->size(); k++) {
                    QTreeWidgetItem *childShop = new QTreeWidgetItem();
                    childShop->setText(0, (*shop)[k].name);
                    childPlant->addChild(static_cast<QTreeWidgetItem*>(&childShop);
                }
            }
        }
    }
    */
    return false;
}

bool MainWindow::displayModelTree()
{
    qDebug() << "displayModelTree()";
    //delete ui->treeWidgetElement->takeTopLevelItem(0);
    hasDB = false;
    ui->treeWidgetModel->clear();
    /*
    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetModel);
    root->setWhatsThis(0, "Root");
    root->setText(0, "Models");
    root->setExpanded(true);
    ui->treeWidgetModel->addTopLevelItem(root);    
    */

    qDebug() << "before: ";
    for(int i=0; i<db->models.size(); i++)
        qDebug() << "model_id: " << db->models[i].id << ", " << &db->models[i];
    for(int i=0; i<db->elements.size(); i++)
        qDebug() << "element_id: " << db->elements[i].id << ", " << &db->elements[i];
    //QList<QTreeWidgetItem*> modelWdgt;
    db->get_model(&modelWdgt);
    qDebug() << "after: ";
    for(int i=0; i<db->models.size(); i++)
        qDebug() << "model_id: " << db->models[i].id << ", " << &db->models[i];
    for(int i=0; i<db->elements.size(); i++)
        qDebug() << "element_id: " << db->elements[i].id << ", " << &db->elements[i];
    qDebug() << "after : modelWdgt";
    for(int i=0; i<modelWdgt.size(); i++) {
        cModelWidget *mdl = static_cast<cModelWidget*>(modelWdgt[i]);
        mdl->cParent->printInfo("displayModelTree : after : modelWdgt");
    }

    for(int i=0; i<modelWdgt.size(); i++) {
        //root->addChild(modelWdgt[i]);
        ui->treeWidgetModel->addTopLevelItem(modelWdgt[i]);
    }
    hasDB = true;
    qDebug() << "end: displayModelTree()";
    for(int i=0; i<db->models.size(); i++)
        qDebug() << "model_id: " << db->models[i].id << ", " << &db->models[i];
    for(int i=0; i<db->elements.size(); i++)
        qDebug() << "element_id: " << db->elements[i].id << ", " << &db->elements[i];
    qDebug() << "------- end: displayModelTree() ------";
    return false;
}
bool MainWindow::displayElementTree() {
    qDebug() << "displayElementTree()";
    hasDB = false;
    ui->treeWidgetElement->clear();
    /*
    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetElement);
    root->setWhatsThis(0, "Root");
    root->setText(0, "Elements");
    root->setExpanded(true);
    ui->treeWidgetElement->addTopLevelItem(root);
    */

    //qDebug() << "before : eleWdgt";
    /*
    for(int i=0; i<eleWdgt.size(); i++) {
        cElementWidget *elm = static_cast<cElementWidget*>(eleWdgt[i]);
        //elm->cParent->printInfo();
    }
    */
    db->get_element(&eleWdgt);
    //qDebug() << "after : eleWdgt";
    /*
    for(int i=0; i<eleWdgt.size(); i++) {
        cElementWidget *elm = static_cast<cElementWidget*>(eleWdgt[i]);
        //elm->cParent->printInfo();
    }
    */

    for(int i=0; i<eleWdgt.size(); i++) {
        //root->addChild(eleWdgt[i]);
        ui->treeWidgetElement->addTopLevelItem(eleWdgt[i]);
    }
    hasDB = true;
    return false;
}
bool MainWindow::displayRouteTree() {
    qDebug() << "displayRouteTree()";
    hasDB = false;
    ui->treeWidgetRoute->clear();
    //QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetRoute);
    //root->setExpanded(true);
    db->get_routes(&routeWdgt);
    ui->treeWidgetRoute->addTopLevelItem(routeWdgt[0]);
    for(int i=0; i<routeWdgt.size(); i++)
        commonFunction::setAllChildExpanded(routeWdgt[i]);
    hasDB = true;
    return true;
}
