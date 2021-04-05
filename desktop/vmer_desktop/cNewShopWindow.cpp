#include "cNewShopWindow.h"
#include "ui_cNewShopWindow.h"

cNewShopWindow::cNewShopWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cNewShopWindow)
{
    ui->setupUi(this);
}

cNewShopWindow::~cNewShopWindow()
{
    delete ui;
}
