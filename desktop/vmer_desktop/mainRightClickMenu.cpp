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
#include <QPixmap>
#include <QGraphicsView>
#include <QTableWidget>

void MainWindow::elementRightClickMenu(const QPoint &pos) {
    qDebug("elementRightClickMenu");
    hasDB = false;
    QTreeWidget *tree = ui->treeWidgetElement;
    QTreeWidgetItem *item = tree->itemAt(pos);

    qDebug()<<pos<<item->text(0);
    bool bUpdate=false;

    if(item->whatsThis(0).contains("Element")) {
        newAct = new QAction("New Point", this);
        newAct->setStatusTip("New Point xxxxxxxxxxxxxxx");
        delAct = new QAction("Delete Element", this);
        delAct->setStatusTip("Delete Element xxxxxxxxxxxxxxx");

        QMenu menu(this);
        menu.addAction(newAct);
        menu.addAction(delAct);

        QPoint pt(pos);
        QAction *selected = menu.exec( tree->mapToGlobal(pt) );
        if (selected && selected->text() == "New Point") {
            qDebug() << "selected : " << selected->text();
            cElementWidget* elmWdg = static_cast<cElementWidget*>(item);
            int elmId = elmWdg->cParent->id;
            newPoint(elmId);
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
        else if(selected && selected->text() == "Delete Element") {
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
        delAct = new QAction("Delete Point", this);
        delAct->setStatusTip("Delete Point xxxxxxxxxxxxxxx");
        QMenu menu(this);
        menu.addAction(delAct);

        QPoint pt(pos);
        QAction *selected = menu.exec( tree->mapToGlobal(pt) );
        if (selected && selected->text() == "Delete Point") {
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
        newAct = new QAction("New Element", this);
        newAct->setStatusTip("New Element xxxxxxxxxxxxxxx");

        QMenu menu(this);
        menu.addAction(newAct);

        QPoint pt(pos);
        QAction *selected = menu.exec( tree->mapToGlobal(pt) );
        if (selected && selected->text() == "New Element") {
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
        delAct = new QAction("Delete Model", this);
        delAct->setStatusTip("Delete Model xxxxxxxxxxxxxxx");
        QMenu menu(this);
        menu.addAction(delAct);

        QPoint pt(pos);
        QAction *selected = menu.exec( tree->mapToGlobal(pt) );
        if (selected && selected->text() == "Delete Model") {
            qDebug() << "selected : " << selected->text();
            cModelWidget* pntWdg = static_cast<cModelWidget*>(item);
            //int pntId = pntWdg->cParent->id;
            //db->deleteDB("model", pntId);
            //delPoint(pntWdg);
            db->delete_model(pntWdg->cParent->id);
            bUpdate=true;
        }
    }
    else {
        qDebug() << "else : " << item->whatsThis(0);
        newAct = new QAction("New Model", this);
        newAct->setStatusTip("New Model xxxxxxxxxxxxxxx");

        QMenu menu(this);
        menu.addAction(newAct);

        QPoint pt(pos);
        QAction *selected = menu.exec( tree->mapToGlobal(pt) );
        if (selected && selected->text() == "New Model") {
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
        jsonAct = new QAction("Export JSON file", this);
        jsonAct->setStatusTip("Export JSON file.");
        QMenu menu(this);
        menu.addAction(jsonAct);

        QPoint pt(pos);
        QAction *selected = menu.exec( tree->mapToGlobal(pt) );
        if (selected && selected->text() == "Export JSON file") {
            ;
        }
    }
    else if(item->whatsThis(0).contains("Company")) {
        qDebug() << "contains(Company) : " << item->whatsThis(0);
    }
    else if(item->whatsThis(0).contains("Plant")) {
        qDebug() << "contains(Plant) : " << item->whatsThis(0);
        newAct = new QAction("New Shop", this);
        newAct->setStatusTip("New Shop xxxxxxxxxxxxxxx");

        QMenu menu(this);
        menu.addAction(newAct);

        QPoint pt(pos);
        QAction *selected = menu.exec( tree->mapToGlobal(pt) );
        if (selected && selected->text() == "New Shop") {
            qDebug() << "selected" << selected->text();
            cPlantWidget* plntWdg = static_cast<cPlantWidget*>(item);
            int plntId = plntWdg->cParent->id;
            on_actionNew_Shop_Window_triggered(plntId);
            bUpdate = true;
        }
    }
    else if(item->whatsThis(0).contains("Shop")) {
        qDebug() << "contains(Shop) : " << item->whatsThis(0);
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
    }
    else if(item->whatsThis(0).contains("Plant")) {
        qDebug() << "contains(Plant) : " << item->whatsThis(0);
        newAct = new QAction("New Route", this);
        newAct->setStatusTip("New Route xxxxxxxxxxxxxxx");

        QMenu menu(this);
        menu.addAction(newAct);

        QPoint pt(pos);
        QAction *selected = menu.exec( tree->mapToGlobal(pt) );
        if (selected && selected->text() == "New Route") {
            qDebug() << "selected" << selected->text();
            cPlantWidget* plntWdg = static_cast<cPlantWidget*>(item);
            int plntId = plntWdg->cParent->id;
            on_actionNew_Route_Window_triggered(plntId);
            bUpdate = true;
        }
    }
    else if(item->whatsThis(0).contains("Route")) {
        qDebug() << "contains(Route) : " << item->whatsThis(0);
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
