#ifndef CNEWMODELWINDOW_H
#define CNEWMODELWINDOW_H

#include <QDialog>
#include <cDB.h>

namespace Ui {
class cNewModelWindow;
}

class cNewModelWindow : public QDialog
{
    Q_OBJECT

public:
    explicit cNewModelWindow(QWidget *parent = nullptr);
    ~cNewModelWindow();

    cDB *db;
    void createElementTable();

private slots:
    void on_pushButton_clicked();

private:
    Ui::cNewModelWindow *ui;
};

#endif // CNEWMODELWINDOW_H
