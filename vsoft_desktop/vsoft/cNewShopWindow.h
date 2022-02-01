#ifndef CNEWSHOPWINDOW_H
#define CNEWSHOPWINDOW_H

#include <QDialog>
#include "cDB.h"

namespace Ui {
class cNewShopWindow;
}

class cNewShopWindow : public QDialog
{
    Q_OBJECT

public:
    explicit cNewShopWindow(QWidget *parent = nullptr, int _plantId=-1);
    ~cNewShopWindow();

    void createModelTable();

    cDB *db;

Q_SIGNALS:
    void newShopAccepted(int _plantId, QString name, QList<int> eleIds);

private slots:
    void on_btn_add_clicked();
    void on_btn_del_clicked();
    void on_buttonBox_accepted();

private:
    Ui::cNewShopWindow *ui;
    int plantId;
    QString shpName;
    QList<int> model_ids;
};

#endif // CNEWSHOPWINDOW_H
