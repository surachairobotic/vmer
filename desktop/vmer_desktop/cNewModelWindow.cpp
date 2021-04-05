#include "cNewModelWindow.h"
#include "ui_cNewModelWindow.h"

cNewModelWindow::cNewModelWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cNewModelWindow)
{
    ui->setupUi(this);
}

cNewModelWindow::~cNewModelWindow()
{
    delete ui;
}
