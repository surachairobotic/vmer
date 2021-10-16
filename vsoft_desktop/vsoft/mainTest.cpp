#include "mainwindow.h"
#include "ui_mainwindow.h"
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
        qDebug() << "print_db_table : " << db->print_db_table();
        qDebug() << "print_element_table : " << db->print_element_table();
        qDebug() << "print_point_table : " << db->print_point_table();
        qDebug() << "print_company_table : " << db->print_company_table();
        qDebug() << "print_plant_table : " << db->print_plant_table();
        qDebug() << "print_shop_table : " << db->print_shop_table();
        qDebug() << "print_route_table : " << db->print_route_table();
        qDebug() << "print_models_table : " << db->print_models_table();
        qDebug() << "print_machine_table : " << db->print_machine_table();
        qDebug() << "print_element_in_model_table : " << db->print_element_in_model_table();
        qDebug() << "print_machine_in_route_table : " << db->print_machine_in_route_table();
        qDebug() << "print_point_in_route_table : " << db->print_point_in_route_table();

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
