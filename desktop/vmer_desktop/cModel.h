#ifndef CMODEL_H
#define CMODEL_H

#include <QtCore>
#include <QList>
#include <QTreeWidgetItem>

class cElement;
class cModel;
class cModelWidget : public QTreeWidgetItem {
public:
    cModelWidget() {
        cParent = nullptr;
    }
    cModel *cParent;
};
class cModel {
public:
    cModel(const int _id, const QString _name, const QString _desc);
    cModel(const int _id, const QString _name, const QString _desc, QList<cElement*> _elements);
    void printInfo();
    void printInfo(QString msg);
    friend bool operator==(const cModel& l, const cModel& r);
    QTreeWidgetItem* get_widget();
    int id;
    QString name, desc;
    QList<cElement*> elements;
    //QList<cMachine*> machines;
    //QList<cElementInModel*> element_in_models;
    cModelWidget *mdlWdg;
};

#endif // CMODEL_H
