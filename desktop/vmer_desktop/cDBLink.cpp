#include <QDir>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtDebug>
#include "cDB.h"

bool cDB::link_db_table() {
    if(companies.size()<1)
        return false;
    for(int i=0; i<companies.size(); i++) {
        int indx_id = companies[i].db_id;
        dbTables[indx_id-1].pushBackCompany(&companies[i]);
    }
    return true;
}
bool cDB::link_element_table() {
    for(int j=0; j<elements.size(); j++) {
        elements[j].points.clear();
    }
    if(points.size()<1)
        return false;
    for(int i=0; i<points.size(); i++) {
        int indx_id = points[i].element_id;
        //elements[indx_id-1].item.addChild(static_cast<QTreeWidgetItem*>(&points[i]));
        for(int j=0; j<elements.size(); j++) {
            if(elements[j].id == indx_id) {
                elements[j].points.push_back(&points[i]);
                break;
            }
        }
    }
    /*
    if(element_in_models.size()<1)
        return false;
    for(int i=0; i<element_in_models.size(); i++) {
        int indx_id = element_in_models[i].element_id;
        for(int j=0; j<elements.size(); j++) {
            if(elements[j].id == indx_id) {
                elements[j].element_in_models.push_back(&element_in_models[i]);
                break;
            }
        }
    }
    if(point_in_routes.size()<1)
        return false;
    for(int i=0; i<point_in_routes.size(); i++) {
        int indx_id = point_in_routes[i].element_id;
        elements[indx_id-1].point_in_routes.push_back(&point_in_routes[i]);
    }
    */
    return true;
}
bool cDB::link_point_table() {
    /*
    if(point_in_routes.size()<1)
        return false;
    for(int i=0; i<point_in_routes.size(); i++) {
        int indx_id = point_in_routes[i].point_id;
        points[indx_id-1].point_in_routes.push_back(&point_in_routes[i]);
    }
    */
    return true;
}
bool cDB::link_company_table() {
    if(plants.size()<1)
        return false;
    for(int i=0; i<plants.size(); i++) {
        int indx_id = plants[i].company_id;
        companies[indx_id-1].pushBackPlant(&plants[i]);
    }
    return true;
}
bool cDB::link_plant_table() {
    if(shops.size()<1)
        return false;
    for(int i=0; i<shops.size(); i++) {
        int indx_id = shops[i].plant_id;
        plants[indx_id-1].pushBackShop(&shops[i]);
    }
    return true;
}
bool cDB::link_shop_table() {
    if(routes.size()<1)
        return false;
    for(int i=0; i<routes.size(); i++) {
        int indx_id = routes[i].shop_id;
        shops[indx_id-1].routes.push_back(&routes[i]);
    }
    return true;
}
bool cDB::link_route_table() {
    if(machine_in_routes.size()<1)
        return false;
    for(int i=0; i<machine_in_routes.size(); i++) {
        int indx_id = machine_in_routes[i].route_id;
        routes[indx_id-1].machine_in_routes.push_back(&machine_in_routes[i]);
    }
    if(point_in_routes.size()<1)
        return false;
    for(int i=0; i<point_in_routes.size(); i++) {
        int indx_id = point_in_routes[i].route_id;
        routes[indx_id-1].point_in_routes.push_back(&point_in_routes[i]);
    }
    return true;
}
bool cDB::link_models_table() {
    /*
    if(machines.size()<1)
        return false;
    for(int i=0; i<machines.size(); i++) {
        int indx_id = machines[i].model_id;
        models[indx_id-1].machines.push_back(&machines[i]);
    }
    */
    for(int i=0; i<models.size(); i++) {
        models[i].elements.clear();
    }

    if(models.size()<1)
        return false;
    if(element_in_models.size()<1)
        return false;
    if(elements.size()<1)
        return false;

    for(int i=0; i<element_in_models.size(); i++) {
        int modelIndx = 0;
        int state=0;
        for(int j=0; j<models.size(); j++) {
            if(models[j].id == element_in_models[i].model_id) {
                modelIndx = j;
                state++;
                break;
            }
        }
        int elementIndx = 0;
        for(int k=0; k<elements.size(); k++) {
            if(elements[k].id == element_in_models[i].element_id) {
                elementIndx = k;
                state++;
                break;
            }
        }
        if(state==2) {
            models[modelIndx].elements.push_back(&elements[elementIndx]);
            qDebug() << "model.id: " << models[modelIndx].id << ", element.id: " << elements[elementIndx].id;
            int s = models[modelIndx].elements.size()-1;
            qDebug() << "models[modelIndx].elements[size-1]: " << models[modelIndx].elements[s] << "elements[elementIndx]: " << &elements[elementIndx];
        }
    }

    //qDebug() << "link_print_info_model, " << models.size();
    for(int i=0; i<models.size(); i++) {
        //qDebug() << "[" << i << "]";
        models[i].printInfo();
        //qDebug() << "==" << i << "==";
    }
    return true;
}
bool cDB::link_machine_table() {
    if(machine_in_routes.size()<1)
        return false;
    for(int i=0; i<machine_in_routes.size(); i++) {
        int indx_id = machine_in_routes[i].machine_id;
        machines[indx_id-1].machine_in_routes.push_back(&machine_in_routes[i]);
    }
    if(point_in_routes.size()<1)
        return false;
    for(int i=0; i<point_in_routes.size(); i++) {
        int indx_id = point_in_routes[i].machine_id;
        machines[indx_id-1].point_in_routes.push_back(&point_in_routes[i]);
    }
    return true;
}
bool cDB::link_element_in_model_table() {
    return true;
}
bool cDB::link_machine_in_route_table() {
    return true;
}
bool cDB::link_point_in_route_table() {
    return true;
}
bool cDB::link_all() {
    link_db_table();
    link_element_table();
    link_point_table();
    link_company_table();
    link_plant_table();
    link_shop_table();
    link_route_table();
    link_models_table();
    link_machine_table();
    link_element_in_model_table();
    link_machine_in_route_table();
    link_point_in_route_table();
    return false;
}
