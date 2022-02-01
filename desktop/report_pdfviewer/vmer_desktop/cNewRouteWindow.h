#ifndef CNEWROUTEWINDOW_H
#define CNEWROUTEWINDOW_H

#include <QDialog>
#include "cDB.h"
#include <QAbstractButton>

namespace Ui {
class cNewRouteWindow;
}

class cNewRouteWindow : public QDialog
{
public:
    Q_OBJECT

public:
    explicit cNewRouteWindow(QWidget *parent = nullptr, int _plantId=-1);
    ~cNewRouteWindow();

    cDB *db;
    void createPlantTable();
    QJsonDocument tree2json(const QTreeWidget *wdgt, const QString routeName, const int routeId);
    QString recursiveTreeItem(const QTreeWidgetItem *wdgt);
    QJsonObject* recursiveJsonItem(const QTreeWidgetItem *wdgt);
    void saveJson(QJsonDocument document, QString fileName);

Q_SIGNALS:
    void newRouteAccepted(int _plantId, QString name, QList<int> eleIds);

public slots:
    void accept();
    void reject();

private slots:
    //void on_btn_add_clicked();
    //void on_btn_del_clicked();
    //void on_buttonBox_accepted();

    void on_treeWidget_left_itemChanged(QTreeWidgetItem *item, int column);

    void on_treeWidget_left_itemClicked(QTreeWidgetItem *item, int column);

    void on_buttonBox_clicked(QAbstractButton *button);

private:

    void checkStateProcess(QTreeWidgetItem *item, int column);
    void checkAllChild(QTreeWidgetItem *item, Qt::CheckState state);
    void checkAllParent(QTreeWidgetItem *item, Qt::CheckState state);
    void checkAllChildNotDiff(QTreeWidgetItem *item);

    Ui::cNewRouteWindow *ui;
    int plantId;
    QString routeName;
    QList<int> model_ids;
    QList<QTreeWidgetItem*> plntWdgt;
};

#endif // CNEWROUTEWINDOW_H
