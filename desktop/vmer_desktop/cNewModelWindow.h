#ifndef CNEWMODELWINDOW_H
#define CNEWMODELWINDOW_H

#include <QDialog>

namespace Ui {
class cNewModelWindow;
}

class cNewModelWindow : public QDialog
{
    Q_OBJECT

public:
    explicit cNewModelWindow(QWidget *parent = nullptr);
    ~cNewModelWindow();

private:
    Ui::cNewModelWindow *ui;
};

#endif // CNEWMODELWINDOW_H
