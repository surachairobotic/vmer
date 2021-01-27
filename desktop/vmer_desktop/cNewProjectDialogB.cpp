#include "cNewProjectDialogB.h"
#include "ui_cNewProjectDialogB.h"

cNewProjectDialogB::cNewProjectDialogB(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cNewProjectDialogB)
{
    ui->setupUi(this);
}

cNewProjectDialogB::~cNewProjectDialogB()
{
    delete ui;
}
