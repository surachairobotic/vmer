#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H
#include <QList>
#include <QString>
#include <QLayout>
#include <QPoint>
#include <QAction>
#include <QTreeWidgetItem>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTableWidget>
#include <QHeaderView>
#include <QMenu>


namespace commonFunction{

QList<QString> pointJson(QString msg);
void createTable(const QList<QList<QString>> *data, QLayout *layout);
void clearLayout(QLayout* layout, bool deleteWidgets = true);
QAction* actionHandle(const QList<QString> *txt, QWidget *parent, const QList<QString> *status, QPoint pos);
QList<QString> menu2status(QList<QString> *txt, QString parentName);
void setAllChildExpanded(QTreeWidgetItem *itm);
template<typename T> int findMaxId(QList<T> *itm);
template<typename T> QString findNameById(QList<T> *itm, int id);
template<typename T> int findIndexById(QList<T> *itm, int id);

}

template<typename  T>
int commonFunction::findMaxId(QList<T> *itm) {
    int id=1;
    for(int i=0; i<(*itm).size(); i++) {
        if(id<=(*itm)[i].id)
            id=(*itm)[i].id;
    }
    return id;
}
template<typename T>
QString commonFunction::findNameById(QList<T> *itm, int id) {
    for(int i=0; i<(*itm).size(); i++) {
        if((*itm)[i].id == id)
            return (*itm)[i].name;
    }
    return "";
}
template<typename T>
int commonFunction::findIndexById(QList<T> *itm, int id) {
    for(int i=0; i<(*itm).size(); i++) {
        if((*itm)[i].id == id)
            return i;
    }
    return -1;
}

#endif // COMMONFUNCTION_H
