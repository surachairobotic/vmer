#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QFileDialog"
#include "QDebug"
#include "cNewProject.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFocus();
    btn1 = new QPushButton("OK");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_Project_triggered()
{
    cNewProject *w = new cNewProject(this);
    //w->setFocus();
    w->show();
    /*
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "C:/Users/Laylase/Documents/VmerProjects/",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    qDebug() << dir;
    */
}
