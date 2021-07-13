#ifndef CNEWMACHINEWINDOW_H
#define CNEWMACHINEWINDOW_H

#include <QDialog>
#include "cDB.h"

namespace Ui {
class cNewMachineWindow;
}

class cNewMachineWindow : public QDialog
{
    Q_OBJECT

public:
    explicit cNewMachineWindow(QWidget *parent = nullptr);
    ~cNewMachineWindow();

    cDB *db;

private:
    Ui::cNewMachineWindow *ui;
    int shpId;
    QString machineName;
    int model_ids;
};

#endif // CNEWMACHINEWINDOW_H
