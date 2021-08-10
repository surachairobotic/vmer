#ifndef CNEWMACHINEWINDOW_H
#define CNEWMACHINEWINDOW_H

#include <QDialog>
#include "cDB.h"
#include <QAbstractButton>

namespace Ui {
class cNewMachineWindow;
}

class cNewMachineWindow : public QDialog
{
    Q_OBJECT

public:
    explicit cNewMachineWindow(QWidget *parent = nullptr);
    ~cNewMachineWindow();
    void initData();

    cDB *db;
    int shpId;

Q_SIGNALS:
    void newMachineAccepted(QString name, int mdlId, int shpId, QString serial, QString desc);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    void accept();
    void reject();

    Ui::cNewMachineWindow *ui;
    QString machineName;
    int model_ids;
};

#endif // CNEWMACHINEWINDOW_H
