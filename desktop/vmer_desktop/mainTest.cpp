#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"

void MainWindow::on_actionECHO_DATABASE_triggered() {
    if(!hasDB)
        qDebug() << "No databases.";
    else {
        /*
        cRoute route;
        db->get_route(1, route);
        route.print();
        */
        displayDBTree();
        //displayElementTree();
    }
}

void MainWindow::on_actionQUERY_ALL_TABLES_triggered()
{
    updateDatabase();

    db->print_db_table();
    db->print_element_table();
    db->print_point_table();
    db->print_company_table();
    db->print_plant_table();
    db->print_shop_table();
    db->print_route_table();
    db->print_models_table();
    db->print_machine_table();
    db->print_element_in_model_table();
    db->print_machine_in_route_table();
    db->print_point_in_route_table();
}

void MainWindow::updateDatabase()
{
    if(!hasDB)
        qDebug() << "No Database.";
    else {
        /*
        qDebug() << "query_db_table : " << db->query_db_table();
        qDebug() << "query_element_table : " << db->query_element_table();
        qDebug() << "query_point_table : " << db->query_point_table();
        qDebug() << "query_company_table : " << db->query_company_table();
        qDebug() << "query_plant_table : " << db->query_plant_table();
        qDebug() << "query_shop_table : " << db->query_shop_table();
        qDebug() << "query_route_table : " << db->query_route_table();
        qDebug() << "query_models_table : " << db->query_models_table();
        qDebug() << "query_machine_table : " << db->query_machine_table();
        qDebug() << "query_element_in_model_table : " << db->query_element_in_model_table();
        qDebug() << "query_machine_in_route_table : " << db->query_machine_in_route_table();
        qDebug() << "query_point_in_route_table : " << db->query_point_in_route_table();

        db->link_db_table();
        db->link_company_table();
        db->link_element_table();
        db->link_point_table();
        db->link_plant_table();
        db->link_shop_table();
        db->link_route_table();
        db->link_models_table();
        db->link_machine_table();
        db->link_element_in_model_table();
        db->link_machine_in_route_table();
        db->link_point_in_route_table();
        */

        db->clear();
        db->query_all();
        db->link_all();
    }
}
