#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"

void MainWindow::on_actionNew_Route_Window_triggered(int plantId) {
    newRouteWin = new cNewRouteWindow(this, plantId);
    newRouteWin->db = this->db;
    newRouteWin->createPlantTable();
    //connect(newRouteWin, &cNewRouteWindow::newRouteAccepted, this, &MainWindow::newShopMain);
    newRouteWin->show();
}

/*
void MainWindow::newShopMain(int _plantId, QString name, QList<int> mdlIds) {
    qDebug() << "newShopMain";
    newShop(_plantId, name, mdlIds);
}

bool MainWindow::newShop(int _plantId, QString name, QList<int> mdlIds) {
    int idM=1;
    for(int i=0; i<db->shops.size(); i++) {
        if(idM <= db->shops[i].id)
            idM = db->shops[i].id;
    }
    idM+=1;
    //qDebug() << idM << ", " << "model_"+QString::number(idM) << ", " << eleIds.size();
    db->insert_shop(idM, _plantId, name, "^_shop_^", mdlIds);
    displayDBTree();
    return true;
}
*/
