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

Q_SIGNALS:
    void newModelAccepted(QString name, QList<int> eleIds);

private slots:
    void on_btn_add_clicked();
    void on_btn_del_clicked();
    void on_buttonBox_accepted();

private:
    void print_element_ids();

    Ui::cNewModelWindow *ui;
    QString mdlName;
    QList<int> element_ids;
};

#endif // CNEWMODELWINDOW_H
