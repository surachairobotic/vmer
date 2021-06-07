#ifndef CNEWROUTEWINDOW_H
#define CNEWROUTEWINDOW_H

#include <QDialog>
#include "cDB.h"

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

Q_SIGNALS:
    void newRouteAccepted(int _plantId, QString name, QList<int> eleIds);

private slots:
    //void on_btn_add_clicked();
    //void on_btn_del_clicked();
    //void on_buttonBox_accepted();

private:
    Ui::cNewRouteWindow *ui;
    int plantId;
    QString routeName;
    QList<int> model_ids;
    QList<QTreeWidgetItem*> plntWdgt;
};

#endif // CNEWROUTEWINDOW_H
