#include "cNewProject.h"
#include "ui_cnewproject.h"

cNewProject::cNewProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cNewProject),
    myparent(parent)
{
    ui->setupUi(this);
    ui->btnBrowse->setIcon(QIcon(":/icon/folder16.png"));
}

cNewProject::~cNewProject()
{
    delete ui;
    //parent->setFocus();
}
