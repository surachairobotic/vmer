#ifndef CNEWELEMENTWINDOW_H
#define CNEWELEMENTWINDOW_H

#include <QDialog>

namespace Ui {
class cNewElementWindow;
}

class cNewElementWindow : public QDialog
{
    Q_OBJECT

public:
    explicit cNewElementWindow(QWidget *parent = nullptr);
    ~cNewElementWindow();

Q_SIGNALS:
    void newElementAccepted(int x, int y, int z);
    //void setMainwindow(MainWindow *main);

private slots:
    void on_btn_addPoint_clicked();
    void on_btn_addTemplate_clicked();
    void on_buttonBox_accepted();
    void on_btn_editImage_clicked();

private:
    void add_graphics_standard();

    Ui::cNewElementWindow *ui;
    //MainWindow *main;
};

#endif // CNEWELEMENTWINDOW_H
