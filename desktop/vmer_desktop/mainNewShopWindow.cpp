#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"

void MainWindow::on_actionNew_Shop_Window_triggered(int plantId) {
    newShpWin = new cNewShopWindow(this, plantId);
    newShpWin->db = this->db;
    newShpWin->createModelTable();
    connect(newShpWin, &cNewShopWindow::newShopAccepted, this, &MainWindow::newShopMain);
    newShpWin->show();
}

void MainWindow::newShopMain(int _plantId, QString name, QList<int> mdlIds) {
    qDebug() << "newShopMain";
    newShop(_plantId, name, mdlIds);
}

//QTreeWidgetItem* MainWindow::newShop(int _plantId, QString name, QList<int> mdlIds) {
QTreeWidgetItem* MainWindow::newShop(int _plantId, QString name) {
    int idM=1;
    for(int i=0; i<db->shops.size(); i++) {
        if(idM <= db->shops[i].id)
            idM = db->shops[i].id;
    }
    idM+=1;
    qDebug() << "newShop --> _plantId : " << _plantId;
    //db->insert_shop(idM, _plantId, name, "^_shop_^", mdlIds);
    db->insert_shop(idM, _plantId, name, "^_shop_^");
    //displayDBTree();

    int indx = db->shops.size()-1;
    return db->shops[indx].get_widget();
    //return true;
}

QTreeWidgetItem* MainWindow::newShop(int _plantId, QString name, QList<int> mdlIds) {
    int idM=1;
    for(int i=0; i<db->shops.size(); i++) {
        if(idM <= db->shops[i].id)
            idM = db->shops[i].id;
    }
    idM+=1;
    //qDebug() << idM << ", " << "model_"+QString::number(idM) << ", " << eleIds.size();
    db->insert_shop(idM, _plantId, name, "^_shop_^", mdlIds);
    //db->insert_shop(idM, _plantId, name, "^_shop_^");
    //displayDBTree();

    int indx = db->shops.size()-1;
    return db->shops[indx].get_widget();
    //return true;
}
