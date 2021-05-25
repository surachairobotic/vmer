#include "cNewRouteWindow.h"
#include "ui_cNewRouteWindow.h"
#include <QMessageBox>

cNewRouteWindow::cNewRouteWindow(QWidget *parent, int _plantId) :
    QDialog(parent),
    ui(new Ui::cNewRouteWindow),
    plantId(_plantId)
{
    ui->setupUi(this);
}

cNewRouteWindow::~cNewRouteWindow() {
    delete ui;
}
