#include "cNewMachineWindow.h"
#include "ui_cNewMachineWindow.h"

cNewMachineWindow::cNewMachineWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cNewMachineWindow)
{
    ui->setupUi(this);
    for(int i=0; i<db->models.size(); i++) {
        ui->comboBox->addItem(db->models[i].name);
    }
}

cNewMachineWindow::~cNewMachineWindow()
{
    delete ui;
}
