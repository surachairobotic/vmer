#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"

void MainWindow::on_actionNew_Route_Window_triggered(int plantId) {
    newRouteWin = new cNewRouteWindow(this, plantId);
    newRouteWin->setWindowTitle("New Route");
    newRouteWin->db = this->db;
    newRouteWin->createPlantTable();
    connect(newRouteWin, &cNewRouteWindow::accepted, this, &MainWindow::newRouteMain);
    newRouteWin->show();
}

void MainWindow::newRouteMain() {
    qDebug() << "newRouteMain";
    db->query_route_table();
    db->link_plant_table();
    db->link_route_table();
    displayRouteTree();
}
