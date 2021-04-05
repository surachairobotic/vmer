#ifndef CNEWSHOPWINDOW_H
#define CNEWSHOPWINDOW_H

#include <QDialog>

namespace Ui {
class cNewShopWindow;
}

class cNewShopWindow : public QDialog
{
    Q_OBJECT

public:
    explicit cNewShopWindow(QWidget *parent = nullptr);
    ~cNewShopWindow();

private:
    Ui::cNewShopWindow *ui;
};

#endif // CNEWSHOPWINDOW_H
