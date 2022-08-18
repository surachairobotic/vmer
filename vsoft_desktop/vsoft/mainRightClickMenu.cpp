#include "mainwindow.h"
#include "ui_mainwindow.h"
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
#include <QPixmap>
#include <QGraphicsView>
#include <QTableWidget>
#include "commonFunction.h"

void MainWindow::elementRightClickMenu(const QPoint &pos) {
    qDebug("elementRightClickMenu");
    hasDB = false;
    QTreeWidget *tree = ui->treeWidgetElement;
    QTreeWidgetItem *item = tree->itemAt(pos);

    qDebug()<<pos<<item->text(0);
    bool bUpdate=false;

    if(item->whatsThis(0).contains("Element")) {
        qDebug() << "Element";
        QList<QString> txt = {"New Point", "Rename", "Delete Element"};
        QList<QString> status = commonFunction::menu2status(&txt, item->text(0));
        QAction *selected = commonFunction::actionHandle(&txt, this, &status, tree->mapToGlobal(pos));
        if (selected && selected->text() == txt[0]) {
            qDebug() << "selected : " << selected->text();
            cElementWidget* elmWdg = static_cast<cElementWidget*>(item);
            int elmId = elmWdg->cParent->id;
            bool bSuccess = newPoint(elmId);

            qDebug() << "Element->newPoint : " << bSuccess;
            bUpdate=true;

            /*
            updateDatabase();
            QTreeWidgetItem *parent = item->parent();
            int itemIndex = parent->indexOfChild(item);
            parent->removeChild(item);
            parent->insertChild(itemIndex, elmWdg->cParent->get_widget());
            displayElementTree();
            */
        }
        else if(selected && selected->text() == txt[1]) {
            qDebug() << "selected : " << selected->text();
            //item->setSelected(true);

        }
        else if(selected && selected->text() == txt[2]) {
            qDebug() << "selected : " << selected->text();
            cElementWidget* elmWdg = static_cast<cElementWidget*>(item);
            int elmId = elmWdg->cParent->id;
            /*
            delElement(elmWdg);
            for(int i=0; i<elmWdg->cParent->points.size(); i++) {
                int _id = elmWdg->cParent->points[i]->id;
                db->deleteDB("point", _id);
            }
            db->deleteDB("element", elmId);
            */
            db->delete_element(elmId);
            bUpdate=true;

            /*
            updateDatabase();
            QTreeWidgetItem *parent = item->parent();
            int itemIndex = parent->indexOfChild(item);
            parent->removeChild(item);
            parent->insertChild(itemIndex, elm->cParent->get_widget());
            displayElementTree();
            */
        }
    }
    else if(item->whatsThis(0).contains("Point")) {
        qDebug() << "Point";
//        delAct = new QAction("Delete Point", this);
//        delAct->setStatusTip("Delete Point xxxxxxxxxxxxxxx");
//        QMenu menu(this);
//        menu.addAction(delAct);

//        QPoint pt(pos);
//        QAction *selected = menu.exec( tree->mapToGlobal(pt) );

        QString parentName = item->text(0);
        QList<QString> txt = {"Delete Point"};
        QList<QString> status = commonFunction::menu2status(&txt, item->text(0));
        QAction *selected = commonFunction::actionHandle(&txt, this, &status, tree->mapToGlobal(pos));
        if (selected && selected->text() == txt[0]) {
            qDebug() << "selected : " << selected->text();
            cPointWidget* pntWdg = static_cast<cPointWidget*>(item);
            int pntId = pntWdg->cParent->id;
            //db->deleteDB("point", pntId);
            //delPoint(pntWdg);
            db->delete_point(pntId);
            bUpdate=true;
        }
    }
    else {
        qDebug() << "else";
//        newAct = new QAction("New Element", this);
//        newAct->setStatusTip("New Element xxxxxxxxxxxxxxx");

//        QMenu menu(this);
//        menu.addAction(newAct);

//        QPoint pt(pos);
//        QAction *selected = menu.exec( tree->mapToGlobal(pt) );

        QString parentName = item->text(0);
        QList<QString> txt = {"New Element"};
        QList<QString> status = commonFunction::menu2status(&txt, item->text(0));
        QAction *selected = commonFunction::actionHandle(&txt, this, &status, tree->mapToGlobal(pos));
        if (selected && selected->text() == txt[0]) {
            qDebug() << "selected" << selected->text();
            //newElement();
            //bUpdate=true;
            on_actionNew_Element_Window_triggered();
        }
    }
    if(bUpdate)
        displayElementTree();
    hasDB = true;
}

void MainWindow::modelRightClickMenu(const QPoint &pos) {
    qDebug() << "modelRightClickMenu";
    hasDB = false;
    QTreeWidget *tree = ui->treeWidgetModel;
    QTreeWidgetItem *item = tree->itemAt(pos);

    qDebug()<<pos<<item->text(0);
    bool bUpdate=false;

    if(item->whatsThis(0).contains("Element") || item->whatsThis(0).contains("Point")) {
        qDebug() << "contains(...) : " << item->whatsThis(0);
    }
    else if(item->whatsThis(0).contains("Model")) {
        qDebug() << "contains(Model) : " << item->whatsThis(0);
//        delAct = new QAction("Delete", this);
//        delAct->setStatusTip("Delete xxxxxxxxxxxxxxx");
//        renmAct = new QAction("Rename");
//        renmAct->setStatusTip("Rename xxxxxxxxxxxxxxx");
//        QMenu menu(this);
//        menu.addAction(delAct);

//        QPoint pt(pos);
//        QAction *selected = menu.exec( tree->mapToGlobal(pt) );

        QString parentName = item->text(0);
        QList<QString> txt = {"Delete", "Rename"};
        QList<QString> status = commonFunction::menu2status(&txt, item->text(0));
        QAction *selected = commonFunction::actionHandle(&txt, this, &status, tree->mapToGlobal(pos));
        if (selected && selected->text() == txt[0]) {
            qDebug() << "selected : " << selected->text();
            cModelWidget* pntWdg = static_cast<cModelWidget*>(item);
            //int pntId = pntWdg->cParent->id;
            //db->deleteDB("model", pntId);
            //delPoint(pntWdg);
            db->delete_model(pntWdg->cParent->id);
            bUpdate=true;
        }
        else if (selected && selected->text() == txt[1]) {
            qDebug() << "selected : " << selected->text();
            cModelWidget* pntWdg = static_cast<cModelWidget*>(item);
            //db->delete_model(pntWdg->cParent->id);
            bUpdate=true;
        }
    }
    else {
        qDebug() << "else : " << item->whatsThis(0);
//        newAct = new QAction("New Model", this);
//        newAct->setStatusTip("New Model xxxxxxxxxxxxxxx");

//        QMenu menu(this);
//        menu.addAction(newAct);

//        QPoint pt(pos);
//        QAction *selected = menu.exec( tree->mapToGlobal(pt) );

        QString parentName = item->text(0);
        QList<QString> txt = {"New Model"};
        QList<QString> status = commonFunction::menu2status(&txt, item->text(0));
        QAction *selected = commonFunction::actionHandle(&txt, this, &status, tree->mapToGlobal(pos));
        if (selected && selected->text() == txt[0]) {
            qDebug() << "selected" << selected->text();
            on_actionNew_Model_Window_triggered();
            //newModel();
            //bUpdate=true;
        }
    }
    if(bUpdate)
        displayModelTree();
    hasDB = true;
}

void MainWindow::dbRightClickMenu(const QPoint &pos)
{
    qDebug() << "dbRightClickMenu";
    hasDB = false;
    QTreeWidget *tree = ui->treeWidgetDB;
    QTreeWidgetItem *item = tree->itemAt(pos);

    qDebug()<<pos<<item->text(0);
    bool bUpdate=false;

    if(item->whatsThis(0).contains("DBTable")) {
        qDebug() << "contains(DBTable) : " << item->whatsThis(0);
        QString parentName = item->text(0);
        QList<QString> txt = {"New Company", "Rename", "Delete"};
        QList<QString> status = commonFunction::menu2status(&txt, item->text(0));
        QAction *selected = commonFunction::actionHandle(&txt, this, &status, tree->mapToGlobal(pos));
        if (selected && selected->text() == txt[0]) {
            ;
        }
        else if(selected && selected->text() == txt[1]) {
            ;
        }
         else if(selected && selected->text() == txt[2]) {
            ;
        }
    }
    else if(item->whatsThis(0).contains("Company")) {
        qDebug() << "contains(Company) : " << item->whatsThis(0);
        QString parentName = item->text(0);
        QList<QString> txt = {"New Plant", "Rename", "Delete"};
        QList<QString> status = commonFunction::menu2status(&txt, item->text(0));
        QAction *selected = commonFunction::actionHandle(&txt, this, &status, tree->mapToGlobal(pos));
        if (selected && selected->text() == txt[0]) {
            cCompanyWidget *company = static_cast<cCompanyWidget*>(item);
            int companyId = company->cParent->id;
        }
        else if(selected && selected->text() == txt[1]) {
            ;
        }
         else if(selected && selected->text() == txt[2]) {
            ;
        }
    }
    else if(item->whatsThis(0).contains("Plant")) {
        qDebug() << "contains(Plant) : " << item->whatsThis(0);
//        newAct = new QAction("New Shop", this);
//        newAct->setStatusTip("New Shop xxxxxxxxxxxxxxx");

//        QMenu menu(this);
//        menu.addAction(newAct);

//        QPoint pt(pos);
//        QAction *selected = menu.exec( tree->mapToGlobal(pt) );
        QString parentName = item->text(0);
        QList<QString> txt = {"New Shop", "Rename", "Delete"};
        QList<QString> status = commonFunction::menu2status(&txt, item->text(0));
        QAction *selected = commonFunction::actionHandle(&txt, this, &status, tree->mapToGlobal(pos));
        if (selected && selected->text() == txt[0]) {
            qDebug() << "selected" << selected->text();
            cPlantWidget* plntWdg = static_cast<cPlantWidget*>(item);
            int plntId = plntWdg->cParent->id;
            //on_actionNew_Shop_Window_triggered(plntId);
            QString numID = QString::number(plntWdg->cParent->id);
            QString name = plntWdg->cParent->name;
            qDebug() << numID << " : " << name;
            QTreeWidgetItem *itm = newShop(plntId, QString("shop_"+QString::number(newShopCount++)));
            bUpdate = true;
        }
        else if(selected && selected->text() == txt[1]) {
            ;
        }
         else if(selected && selected->text() == txt[2]) {
            ;
        }
    }
    else if(item->whatsThis(0).contains("Shop")) {
        qDebug() << "contains(Shop) : " << item->whatsThis(0);
        QString parentName = item->text(0);
        QList<QString> txt = {"New Machine", "Rename", "Delete"};
        QList<QString> status = commonFunction::menu2status(&txt, item->text(0));
        QAction *selected = commonFunction::actionHandle(&txt, this, &status, tree->mapToGlobal(pos));
        if (selected && selected->text() == txt[0]) {
            qDebug() << "selected" << selected->text();
            cShopWidget* shpWdg = static_cast<cShopWidget*>(item);
            int shpId = shpWdg->cParent->id;
            on_actionNew_Machine_Window_triggered(shpId);
        }
        else if(selected && selected->text() == txt[1]) {
        }
        else if(selected && selected->text() == txt[2]) {
        }
    }
    else if(item->whatsThis(0).contains("Machine")) {
        qDebug() << "contains(Machine) : " << item->whatsThis(0);
        QString parentName = item->text(0);
        QList<QString> txt = {"Rename", "Delete"};
        QList<QString> status = commonFunction::menu2status(&txt, item->text(0));
        QAction *selected = commonFunction::actionHandle(&txt, this, &status, tree->mapToGlobal(pos));
        if (selected && selected->text() == txt[0]) {
            qDebug() << "selected" << selected->text();
        }
        else if(selected && selected->text() == txt[1]) {
            qDebug() << "selected" << selected->text();

        }
        else if(selected && selected->text() == txt[2]) {
        }
    }
    else {
        qDebug() << "else : " << item->whatsThis(0);
    }
    if(bUpdate) {
        displayDBTree();
        displayRouteTree();
    }
    hasDB = true;
}

void MainWindow::routeRightClickMenu(const QPoint &pos) {
    qDebug() << "routeRightClickMenu";
    hasDB = false;
    QTreeWidget *tree = ui->treeWidgetRoute;
    QTreeWidgetItem *item = tree->itemAt(pos);

    qDebug()<<pos<<item->text(0);
    bool bUpdate=false;

    if(item->whatsThis(0).contains("DBTable")) {
        qDebug() << "contains(DBTable) : " << item->whatsThis(0);
    }
    else if(item->whatsThis(0).contains("Company")) {
        qDebug() << "contains(Company) : " << item->whatsThis(0);
//        newAct = new QAction("New Route", this);
//        newAct->setStatusTip("New Route xxxxxxxxxxxxxxx");

//        QMenu menu(this);
//        menu.addAction(newAct);

//        QPoint pt(pos);
//        QAction *selected = menu.exec( tree->mapToGlobal(pt) );
        QString parentName = item->text(0);
        QList<QString> txt = {"New Route"};
        QList<QString> status = commonFunction::menu2status(&txt, item->text(0));
        QAction *selected = commonFunction::actionHandle(&txt, this, &status, tree->mapToGlobal(pos));
        if (selected && selected->text() == txt[0]) {
            qDebug() << "selected" << selected->text();
            cPlantWidget* plntWdg = static_cast<cPlantWidget*>(item);
            int plntId = plntWdg->cParent->id;
            on_actionNew_Route_Window_triggered(plntId);
            bUpdate = true;
        }
    }
    else if(item->whatsThis(0).contains("Route")) {
        qDebug() << "contains(Route) : " << item->whatsThis(0);
        QString parentName = item->text(0);
        QList<QString> txt = {"Rename", "Delete"};
        QList<QString> status = commonFunction::menu2status(&txt, item->text(0));
        QAction *selected = commonFunction::actionHandle(&txt, this, &status, tree->mapToGlobal(pos));
        if (selected && selected->text() == txt[0]) {
        }
        else if (selected && selected->text() == txt[1]) {
        }
    }
    else {
        qDebug() << "else : " << item->whatsThis(0);
    }
    if(bUpdate) {
        displayDBTree();
        displayRouteTree();
    }
    hasDB = true;
}
