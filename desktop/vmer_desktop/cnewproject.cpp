#include "cNewProject.h"
#include "ui_cnewproject.h"

cNewProject::cNewProject(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cNewProject)
{
    ui->setupUi(this);
}

cNewProject::~cNewProject()
{
    delete ui;
}
